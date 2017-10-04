//-----------------------------------------------------------------------
// <copyright file="WorkspaceViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows;
    using System.Windows.Threading;
    using Events;
    using Events.EventArgs.Error;
    using Events.EventArgs.SoftwareItem;
    using Model; 
    using Model.Database;
    using VersionManagement.Events.EventArgs.UserFeedback;

    /// <summary>
    /// Mainly View Model. Interface to the model. Included the detailInformation and dataBaseDataGrid view. 
    /// Only references to the Model are allowed. References to Views are not allowed
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class WorkspaceViewModel : BasisViewModel
    {
        /// <summary>
        /// Data grid with software information items
        /// </summary>
        private readonly DatabaseDataGridViewModel databaseDataGridViewModel;

        /// <summary>
        /// Detail information view to edit and save entries
        /// </summary>
        private readonly DetailedInformationViewModel detailedInformationViewModel;

        /// <summary>
        /// Reference to the model
        /// </summary>
        private readonly VersionManagementModel versionManagementModel;

        /// <summary>
        /// Before a item is saving in the database, the database will update. In this time the selection item is changing (need this information to prevent a warning)
        /// </summary>
        private bool isReloadDataGrid = false;

        /// <summary>
        /// View model events with several handlers
        /// </summary>
        public readonly ViewModelEvents ViewModelEvents;

        public WorkspaceViewModel(ViewModelEvents viewModelEvents, VersionManagementModel versionManagementModel)
            : base(viewModelEvents)
        {
            this.versionManagementModel = versionManagementModel;
            this.ViewModelEvents = viewModelEvents;
            this.databaseDataGridViewModel = new DatabaseDataGridViewModel(viewModelEvents);
            this.detailedInformationViewModel = new DetailedInformationViewModel(viewModelEvents, this.versionManagementModel);
        }

        public DetailedInformationViewModel DetailedInformationViewModel
        {
            get
            {
                return this.detailedInformationViewModel;
            }
        }

        public DatabaseDataGridViewModel DatabaseDataGridViewModel
        {
            get
            {
                return this.databaseDataGridViewModel;
            }
        }

        public override void SubscribeEvents()
        {
            // Subscribe own model events
            this.detailedInformationViewModel.SubscribeEvents();
            this.databaseDataGridViewModel.SelectedItemChanged += this.DatabaseDataGridViewModel_SelectedItemChanged;
            this.databaseDataGridViewModel.RefreshDataGridChangedEvent += this.DatabaseDataGridViewModel_RefreshDataGridChangedEvent;
            this.detailedInformationViewModel.ItemChangedEvent += this.DetailedInformationViewModel_ItemChangedEvent;
            this.detailedInformationViewModel.SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent += this.DetailedInformationViewModel_SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent;
            this.ViewModelEvents.ChangeDatabaseSettings += this.ViewModelEvents_ChangeDatabaseSettings;
            this.ViewModelEvents.RenameSelectionItemInDatabase += this.ViewModelEvents_RenameSelectionItemInDatabase;

            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            // Unsubscribe own model events
            this.detailedInformationViewModel.ItemChangedEvent -= this.DetailedInformationViewModel_ItemChangedEvent;
            this.databaseDataGridViewModel.SelectedItemChanged -= this.DatabaseDataGridViewModel_SelectedItemChanged;
            this.databaseDataGridViewModel.RefreshDataGridChangedEvent -= this.DatabaseDataGridViewModel_RefreshDataGridChangedEvent;
            this.detailedInformationViewModel.SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent -= this.DetailedInformationViewModel_SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent;
            this.detailedInformationViewModel.UnsubscribeEvents();
            this.ViewModelEvents.ChangeDatabaseSettings -= this.ViewModelEvents_ChangeDatabaseSettings;
            this.ViewModelEvents.RenameSelectionItemInDatabase -= this.ViewModelEvents_RenameSelectionItemInDatabase;

            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        public override void Init()
        {
            //Add an empty entrys if no empty entry exists in database yet. This is needed to allow the user to leave the selection empty.
            //Add an empty entry if no empty entry exist in database yet. So its possible to delete an item in a list.
            //Otherwise the user must create an own empty entry.
            if (!this.versionManagementModel.SelectionItem.GetAuthors()
                 .Any(x => string.IsNullOrEmpty(x.Author)))
                this.versionManagementModel.SelectionItem.AddAuthor(string.Empty);

            if (!this.versionManagementModel.SelectionItem.GetCustomers()
                 .Any(x => string.IsNullOrEmpty(x.Customer)))
                this.versionManagementModel.SelectionItem.AddCustomer(string.Empty);

            if (!this.versionManagementModel.SelectionItem.GetBaseSoftwares()
                 .Any(x => string.IsNullOrEmpty(x.BaseSoftware)))
                this.versionManagementModel.SelectionItem.AddBaseSoftware(string.Empty);

            if (!this.versionManagementModel.SelectionItem.GetBaseSoftwares()
                 .Any(x => x.BaseSoftware == "trunk"))
                this.versionManagementModel.SelectionItem.AddBaseSoftware("trunk");

            if (!this.versionManagementModel.SelectionItem.GetDocumentTypes()
                 .Any(x => string.IsNullOrEmpty(x.DocumentType)))
                this.versionManagementModel.SelectionItem.AddDocumentType(string.Empty);

            if (!this.versionManagementModel.SelectionItem.GetPropertyTypes()
                 .Any(x => string.IsNullOrEmpty(x.PropertyType)))
                this.versionManagementModel.SelectionItem.AddPropertyType(string.Empty, string.Empty);

            if (!this.versionManagementModel.SelectionItem.GetSystems()
                 .Any(x => string.IsNullOrEmpty(x.System)))
                this.versionManagementModel.SelectionItem.AddSystem(string.Empty);

            this.ReloadDataGrid();

            // Init childs

            // Init base class
            base.Init();
        }

        private void SaveItem(DatabaseItemViewModel databaseItemViewModel)
        {
            //update database view first
            this.ReloadDataGrid();

            //check if the user select a system item
            if (string.IsNullOrEmpty(databaseItemViewModel.System))
            {
                this.ViewModelEvents.OnUserFeedback(this, new UserFeedbackInfoEventArgs("Select system item"));
                return;
            }

            //Add software to the base selection item, if the entry not exist
            if (!this.versionManagementModel.SelectionItem.GetBaseSoftwares()
                .Any(x => x.BaseSoftware == databaseItemViewModel.Software))
                this.versionManagementModel.SelectionItem.AddBaseSoftware(databaseItemViewModel.Software);

            //Add software to the systemSoftware table if in entry not exist
            if (!this.IsSoftwareInSystemsTable(databaseItemViewModel.System, databaseItemViewModel.Software))
                this.versionManagementModel.SelectionItem.AddSystemSoftware(databaseItemViewModel.System, databaseItemViewModel.Software);

            //Create new item if software item has identification number -1
            if (this.detailedInformationViewModel.IsNewItemSet)
            {
                SoftwareVersions item = this.detailedInformationViewModel.GetSoftwareItem();

                //check if the software already exist in the database
                List<SoftwareVersions> items = this.versionManagementModel.SoftwareVersionsItem.GetSoftwareVersions();
                if (items.Any(x => x.Software == databaseItemViewModel.Software))
                {
                    this.ViewModelEvents.OnUserFeedback(this, new UserFeedbackInfoEventArgs(string.Format("{0} does already exist in database and can not be saved again!", databaseItemViewModel.Software)));
                    return;
                }

                this.versionManagementModel.SoftwareVersionsItem.AddSoftwareVersion(item);

                SoftwareVersions softwareItem = this.versionManagementModel.SoftwareVersionsItem.GetItemFromDatabase(item.Software);

                if (softwareItem != null)
                    this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel(softwareItem));
            }
            else
            {
                this.versionManagementModel.SoftwareVersionsItem.UpdateItemInDatabase(new SoftwareVersions()
                {
                    Id = databaseItemViewModel.ItemIdentification,
                    Software = databaseItemViewModel.Software,
                    Base = databaseItemViewModel.BaseSoftware,
                    System = databaseItemViewModel.System,
                    Customer = databaseItemViewModel.Customer,
                    Author = databaseItemViewModel.Author,
                    Date = databaseItemViewModel.Date,
                    PssNumber = databaseItemViewModel.PssNumber,
                    Description = databaseItemViewModel.Description,
                    PrivateInfosBugs = databaseItemViewModel.PrivateInfosBugs,
                    Properties = databaseItemViewModel.Properties,
                    Softwares = databaseItemViewModel.Softwares,
                    Documents = databaseItemViewModel.Documents
                });
            }

            this.ReloadDataGrid();
            this.databaseDataGridViewModel.SelectedDatabaseItemViewModel = this.databaseDataGridViewModel.DatabaseItemViewModels.Where(
                x => x.Software == databaseItemViewModel.Software).FirstOrDefault();

            //clear selection item list which no link ID exist anymore
            this.versionManagementModel.SelectionItem.ClearSelectionItemListEntrysWithoutSoftwareVersionsLink();
        }

        private void DeleteItem(DatabaseItemViewModel databaseItemViewModel)
        {
            //update database view first (possible that the software item dosn't exist anymore)
            this.ReloadDataGrid();

            //Request the user if he want to delete permanently
            UserFeedbackQuestionEventArgs deleteItemFinal = new UserFeedbackQuestionEventArgs(
                string.Format("Do you really want to delete {0} ?", databaseItemViewModel.Software));
            this.ViewModelEvents.OnUserFeedback(this, deleteItemFinal);

            if (deleteItemFinal.Result == MessageBoxResult.Yes)
            {
                //check if firmware is in test result (if yes -> not allowed to remove the firmware)
                if (this.CheckIfSoftwareIsInTestResult(databaseItemViewModel) == false)
                {
                    this.versionManagementModel.SoftwareVersionsItem.DeleteSoftwareVersion(
                        databaseItemViewModel.Software);

                    //Set first database item as selected item in the datagrid
                    this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel =
                        this.DatabaseDataGridViewModel.DatabaseItemViewModels.FirstOrDefault();

                    //Delete the software in the base selection item
                    if (this.versionManagementModel.SelectionItem.GetBaseSoftwares()
                        .Any(x => x.BaseSoftware == databaseItemViewModel.Software.ToString()))
                        this.versionManagementModel.SelectionItem.DeleteBaseSoftware(databaseItemViewModel.Software);

                    //Delete the software in the systemsoftware selection item
                    if (this.IsSoftwareInSystemsTable(databaseItemViewModel.System, databaseItemViewModel.Software))
                        this.versionManagementModel.SelectionItem.DeleteSystemSoftware(databaseItemViewModel.System, databaseItemViewModel.Software);

                    this.ReloadDataGrid();

                    this.databaseDataGridViewModel.SelectedDatabaseItemViewModel = this.databaseDataGridViewModel.DatabaseItemViewModels.FirstOrDefault();
                }
                else
                    this.ViewModelEvents.OnUserFeedback(this, new UserFeedbackInfoEventArgs(string.Format("{0} does already exist in test result table!", databaseItemViewModel.Software)));
            }
        }

        private void AddItem(AddNewItemEventArgs addNewItemEventArgs)
        {
            //Add new SoftwareItem with identification number -1 to indicate temporary value
            //Add the Software and Properties from the selected Software Item, only if an Software Item exist in the DataGrid
            if (this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel != null)
            {
                if (addNewItemEventArgs.NewItemType == NewItemType.Version)
                {
                    //New Software Name add 1 to the last number, if the last char is a number
                    this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel()
                    {
                        ItemIdentification = -1,
                        Software = this.NewSoftwareVersion(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software),
                        BaseSoftware = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software,
                        System = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.System,
                        Properties = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Properties,
                    });
                }
                else if (addNewItemEventArgs.NewItemType == NewItemType.Revision)
                {
                    //New Software Name add 1 to the last number, if the last char is a number
                    this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel()
                    {
                        ItemIdentification = -1,
                        Software = this.NewSoftwareRevision(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software),
                        BaseSoftware = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software,
                        System = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.System,
                        Customer = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Customer,
                        Properties = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Properties,
                    });
                }
            }
            else
            {
                this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel()
                {
                    ItemIdentification = -1,
                });
            }
        }

        private void ReloadDataGrid()
        {
            this.isReloadDataGrid = true;

            //Refreshes the database context. If not, the database not recordgnized when data entries has changed
            this.versionManagementModel.BuildDatabaseContext();

            this.databaseDataGridViewModel.DatabaseItemViewModels.Clear();

            List<SoftwareVersions> items = this.versionManagementModel.SoftwareVersionsItem.GetSoftwareVersions();

            foreach (SoftwareVersions item in items)
                this.databaseDataGridViewModel.DatabaseItemViewModels.Add(new DatabaseItemViewModel(item));

            this.isReloadDataGrid = false;
        }

        private bool IsSoftwareInSystemsTable(string system, string software)
        {
            List<Systems> systemList = this.versionManagementModel.SelectionItem.GetSystems();

            foreach (var systemItem in systemList)
            {
                if (systemItem.System == system)
                {
                    foreach (var systemSoftware in systemItem.SystemSoftwares)
                    {
                        if (systemSoftware.Software == software)
                            return true;
                    }
                }
            }

            return false;
        }

        private void DetailedInformationViewModel_SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent(object sender, Events.EventArgs.SelectedItem.SelectedItemEventArgs e)
        {
            try
            {
                DatabaseItemViewModel item = this.databaseDataGridViewModel.DatabaseItemViewModels.Where(x => x.Software == e.Item).FirstOrDefault();
                this.detailedInformationViewModel.SetDetailedInformation(item);
                this.databaseDataGridViewModel.SelectedDatabaseItemViewModel = item;
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void DatabaseDataGridViewModel_SelectedItemChanged(object sender, EventArgs e)
        {
            try
            {
                //only to change the selelected item, if the user want it explicit (when user edit a new software item)
                if (this.detailedInformationViewModel.IsNewItemSet && this.isReloadDataGrid == false)
                {
                    UserFeedbackQuestionEventArgs changeWithoutSaving = new UserFeedbackQuestionEventArgs("Are you sure to change the software without saving?");
                    this.ViewModelEvents.OnUserFeedback(this, changeWithoutSaving);

                    if (changeWithoutSaving.Result == MessageBoxResult.No)
                        return;
                }

                if (this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel != null)
                    this.detailedInformationViewModel.SetDetailedInformation(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel);
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void DatabaseDataGridViewModel_RefreshDataGridChangedEvent(object sender, EventArgs e)
        {
            try
            {
                this.ReloadDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void DetailedInformationViewModel_ItemChangedEvent(object sender, ItemEventArgs e)
        {
            try
            {
                if (!(sender is DetailedInformationViewModel))
                    return;

                //Add new Item to the collection
                if (e is AddNewItemEventArgs)
                    this.AddItem((AddNewItemEventArgs)e);

                //Save existing item
                if (e is SaveItemEventArgs)
                    this.SaveItem(e.DatabaseItemViewModel);

                //Delete existing item
                if (e is DeleteItemEventArgs)
                    this.DeleteItem(e.DatabaseItemViewModel);
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// After the User edit the selection item, the database has to be refresh
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="Events.EventArgs.DatabaseAccess.DatabaseAccessEventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_ChangeDatabaseSettings(object sender, Events.EventArgs.DatabaseAccess.DatabaseAccessEventArgs e)
        {
            try
            {
                Dispatcher.CurrentDispatcher.InvokeAsync(() => this.Init());
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// Handles the RenameSelectionItemInDatabase event of the ViewModelEvents control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_RenameSelectionItemInDatabase(object sender, EventArgs e)
        {
            try
            {
                this.ReloadDataGrid();

                // Init base class
                base.Init();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// Checks if software is in test result. If yes, the software item is not allowed to remove from the database.
        /// </summary>
        /// <param name="databaseItemViewModel">The database item view model.</param>
        /// <returns>result if the software item exist in the test result table</returns>
        private bool CheckIfSoftwareIsInTestResult(DatabaseItemViewModel databaseItemViewModel)
        {
            SoftwareVersions softwareVersions = this.versionManagementModel.SoftwareVersionsItem.GetItemFromDatabase(databaseItemViewModel.Software);

            //first to seperate by system
            switch (databaseItemViewModel.System)
            {
                case "IC2":
                    return this.versionManagementModel.TestResult.IfValveFirmwwareIsUsedInTestResult(softwareVersions);
                case "Motion Controller":
                    return this.versionManagementModel.TestResult.IfMotionControllerFirmwwareIsUsedInTestResult(softwareVersions);
                case "netXECAT":
                case "netXDeviceNet":
                    return this.versionManagementModel.TestResult.IfInterfaceFirmwwareIsUsedInTestResult(softwareVersions);
                default:
                    return false;
            }
        }

        /// <summary>
        /// Increment the software version. If its not possible, it set the old software version. (F01.0C.00.xy)
        /// </summary>
        /// <param name="oldSoftwareName">old software name</param>
        /// <returns>string of the software revision</returns>
        private string NewSoftwareRevision(string oldSoftwareName)
        {
            bool findDotChar = false;
            int countNumbersofSubVersion = 0;
            for (int i = oldSoftwareName.Length - 1; i > 0; i--)
            {
                if (oldSoftwareName[i] == '.')
                {
                    findDotChar = true;
                    break;
                }
                else countNumbersofSubVersion++;
            }

            if (findDotChar)
            {
                string subSoftwareVersion = DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Substring(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Length - countNumbersofSubVersion, countNumbersofSubVersion);
                int numericValue;
                if (int.TryParse(subSoftwareVersion, out numericValue))
                {
                    string numericString = (numericValue + 1).ToString();
                    string numericStringWithZeroChars = null;

                    //fill revision string with 0 (example 01)
                    for (int i = 0; i < (subSoftwareVersion.Length - numericString.Length); i++)
                    {
                        numericStringWithZeroChars = numericStringWithZeroChars + '0';
                    }

                    numericStringWithZeroChars = numericStringWithZeroChars + numericString;
                    string newSoftwareVersion = DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Substring(0, DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Length - countNumbersofSubVersion) + numericStringWithZeroChars;
                    return newSoftwareVersion;
                }
            }

            return oldSoftwareName;
        }

        /// <summary>
        /// Increment the software version. If its not possible, it set the old software version. (F01.0C.xy.00)
        /// </summary>
        /// <param name="oldSoftwareName">old software name</param>
        /// <returns>string of the software revision</returns>
        private string NewSoftwareVersion(string oldSoftwareName)
        {
            bool findDotChar = false;
            int countNumbersofSubVersion = 0, findDotCharCount = 0, indexStartSubVersion = 0;
            for (int i = oldSoftwareName.Length - 1; i > 0; i--)
            {
                if (oldSoftwareName[i] == '.')
                {
                    findDotCharCount++;
                    if (findDotCharCount == 2)
                    {
                        findDotChar = true;
                        break;
                    }
                    else
                    {
                        countNumbersofSubVersion++;
                        indexStartSubVersion = countNumbersofSubVersion;
                    }
                }
                else countNumbersofSubVersion++;
            }

            if (findDotChar)
            {
                string subSoftwareVersion = DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Substring(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Length - countNumbersofSubVersion, countNumbersofSubVersion - indexStartSubVersion);
                int numericValue;
                if (int.TryParse(subSoftwareVersion, out numericValue))
                {
                    string numericString = (numericValue + 1).ToString();
                    string numericStringWithZeroChars = null;
                    string nummericStringZeroRevision = null;
                    
                    //fill version string with 0 (example 01)
                    for (int i = 0; i < (subSoftwareVersion.Length - numericString.Length); i++)
                    {
                        numericStringWithZeroChars = numericStringWithZeroChars + '0';
                    }

                    //fill revision (example .00)
                    for (int i = 0; i < indexStartSubVersion; i++)
                    {
                        if (i == 0) nummericStringZeroRevision = nummericStringZeroRevision + ".";
                        else nummericStringZeroRevision = nummericStringZeroRevision + '0';
                    }

                    numericStringWithZeroChars = numericStringWithZeroChars + numericString;
                    string newSoftwareVersion = DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Substring(0, DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software.Length - countNumbersofSubVersion) + numericStringWithZeroChars + nummericStringZeroRevision;
                    return newSoftwareVersion;
                }
            }

            return oldSoftwareName;
        }
    }
}
