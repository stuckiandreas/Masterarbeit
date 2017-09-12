﻿//-----------------------------------------------------------------------
// <copyright file="WorkspaceViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
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
            this.detailedInformationViewModel.SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent -= this.DetailedInformationViewModel_SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent;
            this.databaseDataGridViewModel.SelectedItemChanged -= this.DatabaseDataGridViewModel_SelectedItemChanged;
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
            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetAuthors()
                 .Any(x => string.IsNullOrEmpty(x.Author)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddAuthor(string.Empty);

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetCustomers()
                 .Any(x => string.IsNullOrEmpty(x.Customer)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddCustomer(string.Empty);

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares()
                 .Any(x => string.IsNullOrEmpty(x.BaseSoftware)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddBaseSoftware(string.Empty);

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares()
                 .Any(x => x.BaseSoftware == "trunk"))
                this.versionManagementModel.SelectionItemDatabaseManager.AddBaseSoftware("trunk");

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetDocumentTypes()
                 .Any(x => string.IsNullOrEmpty(x.DocumentType)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddDocumentType(string.Empty);

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetPropertyTypes()
                 .Any(x => string.IsNullOrEmpty(x.PropertyType)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddPropertyType(string.Empty, string.Empty);

            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetSystems()
                 .Any(x => string.IsNullOrEmpty(x.System)))
                this.versionManagementModel.SelectionItemDatabaseManager.AddSystem(string.Empty);

            this.ReloadDataGrid();

            // Init childs

            // Init base class
            base.Init();
        }

        private void SaveItem(DatabaseItemViewModel databaseItemViewModel)
        {
            //check if the user select a system item
            if (string.IsNullOrEmpty(databaseItemViewModel.System))
            {
                this.ViewModelEvents.OnUserFeedback(this, new UserFeedbackInfoEventArgs("Select system item"));
                return;
            }

            //Add software to the base selection item, if the entry not exist
            if (!this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares()
                .Any(x => x.BaseSoftware == databaseItemViewModel.Software))
                this.versionManagementModel.SelectionItemDatabaseManager.AddBaseSoftware(databaseItemViewModel.Software);

            //Add software to the systemSoftware table if in entry not exist
            if (!this.IsSoftwareInSystemsTable(databaseItemViewModel.System, databaseItemViewModel.Software))
                this.versionManagementModel.SelectionItemDatabaseManager.AddSystemSoftware(databaseItemViewModel.System, databaseItemViewModel.Software);

            //Create new item if software item has identification number -1
            if (this.detailedInformationViewModel.IsNewItemSet)
            {
                SoftwareVersions item = this.detailedInformationViewModel.GetSoftwareItem();

                //check if the software already exist in the database
                List<SoftwareVersions> items = this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.GetSoftwareVersions();
                if (items.Any(x => x.Software == databaseItemViewModel.Software))
                {
                    this.ViewModelEvents.OnUserFeedback(this, new UserFeedbackInfoEventArgs(string.Format("{0} does already exist in database and can not be saved again!", databaseItemViewModel.Software)));
                    return;
                }

                this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.AddSoftwareVersion(item);

                SoftwareVersions softwareItem = this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.GetItemFromDatabase(item.Software);

                if (softwareItem != null)
                    this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel(softwareItem));
            }
            else
            {
                this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.UpdateItemInDatabase(new SoftwareVersions()
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
            this.versionManagementModel.SelectionItemDatabaseManager.ClearSelectionItemListEntrysWithoutSoftwareVersionsLink();
        }

        private void DeleteItem(DatabaseItemViewModel databaseItemViewModel)
        {
            //Request the user if he want to delete permanently
            UserFeedbackQuestionEventArgs deleteItemFinal = new UserFeedbackQuestionEventArgs(
                string.Format("Do you really want to delete {0} ?", databaseItemViewModel.Software));
            this.ViewModelEvents.OnUserFeedback(this, deleteItemFinal);

            if (deleteItemFinal.Result == MessageBoxResult.Yes)
            {
                int deleteItemIdentificationNumber =
                    this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.ItemIdentification;

                this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.DeleteSoftwareVersion(
                    databaseItemViewModel.Software);

                //Set first database item as selected item in the datagrid
                this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel =
                    this.DatabaseDataGridViewModel.DatabaseItemViewModels.FirstOrDefault();

                //Delete the software in the base selection item
                if (this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares()
                    .Any(x => x.BaseSoftware == databaseItemViewModel.Software.ToString()))
                    this.versionManagementModel.SelectionItemDatabaseManager.DeleteBaseSoftware(databaseItemViewModel.Software);

                //Delete the software in the systemsoftware selection item
                if (this.IsSoftwareInSystemsTable(databaseItemViewModel.System, databaseItemViewModel.Software))
                    this.versionManagementModel.SelectionItemDatabaseManager.DeleteSystemSoftware(databaseItemViewModel.System, databaseItemViewModel.Software);

                this.ReloadDataGrid();

                this.databaseDataGridViewModel.SelectedDatabaseItemViewModel = this.databaseDataGridViewModel.DatabaseItemViewModels.FirstOrDefault();
            }
        }

        private void AddItem()
        {
            //Add new SoftwareItem with identification number -1 to indicate temporary value
            //Add the Software and Properties from the selected Software Item, only if an Software Item exist in the DataGrid
            if (this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel != null)
            {
                //New Software Name add 1 to the last number, if the last char is a number
                this.detailedInformationViewModel.SetDetailedInformation(new DatabaseItemViewModel()
                {
                    ItemIdentification = -1,
                    Software = this.NewSoftwareRevision(DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software),
                    BaseSoftware = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Software,
                    Properties = this.DatabaseDataGridViewModel.SelectedDatabaseItemViewModel.Properties,
                });
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
            this.databaseDataGridViewModel.DatabaseItemViewModels.Clear();

            List<SoftwareVersions> items = this.versionManagementModel.SoftwareVersionsDatabaseAccessManager.GetSoftwareVersions();

            foreach (SoftwareVersions item in items)
                this.databaseDataGridViewModel.DatabaseItemViewModels.Add(new DatabaseItemViewModel(item));

            //add SelectionItem in WorkspaceViewModel
            List<Authors> authors = this.versionManagementModel.SelectionItemDatabaseManager.GetAuthors();
            List<BaseSoftwares> bases = this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares();
            List<Customers> customers = this.versionManagementModel.SelectionItemDatabaseManager.GetCustomers();
            List<Systems> systems = this.versionManagementModel.SelectionItemDatabaseManager.GetSystems();
            List<DocumentTypes> documentTypes = this.versionManagementModel.SelectionItemDatabaseManager.GetDocumentTypes();
            List<PropertyTypes> propertyTypes = this.versionManagementModel.SelectionItemDatabaseManager.GetPropertyTypes();
        }

        private bool IsSoftwareInSystemsTable(string system, string software)
        {
            List<Systems> systemList = this.versionManagementModel.SelectionItemDatabaseManager.GetSystems();

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
                //only to change the selelected item, if the user want it explicit
                if (this.detailedInformationViewModel.IsNewItemSet)
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

        private void DetailedInformationViewModel_ItemChangedEvent(object sender, ItemEventArgs e)
        {
            try
            {
                if (!(sender is DetailedInformationViewModel))
                    return;

                //Add new Item to the collection
                if (e is AddNewItemEventArgs)
                    this.AddItem();

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
        /// Increment the software version. If its not possible, he sets the software version to the selection item.
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
    }
}
