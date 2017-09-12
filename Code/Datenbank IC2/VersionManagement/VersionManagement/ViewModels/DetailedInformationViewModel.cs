//-----------------------------------------------------------------------
// <copyright file="DetailedInformationViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows.Input;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Error;
    using Events.EventArgs.SelectedItem;
    using Events.EventArgs.SoftwareItem;
    using Events.EventArgs.UserFeedback;
    using InputViewModels;
    using Model;
    using Model.Database;

    /// <summary>
    /// Shows the detail information of the selected software item in the databaseDataGrid
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class DetailedInformationViewModel : BasisViewModel
    {
        //Input labels        

        private const string SoftwareLabel = "Software: ";

        private const string BaseSoftwareLabel = "Base: ";

        private const string SystemLabel = "System: ";

        private const string CustomerLabel = "Customer: ";

        private const string AuthorLabel = "Author: ";

        private const string DateLabel = "Date: ";

        private const string PssNumberLabel = "PSS Nr.: ";

        private const string DescriptionLabel = "Description: ";

        private const string PrivateInfosBugsLabel = "Findings: ";

        private const string SoftwaresLabel = "Softwares: ";

        private const string DocumentsLabel = "Documents: ";

        private const string PropertiesLabel = "Properties: ";

        private readonly VersionManagementModel versionManagementModel;

        //Inputs        
        private TextInputViewModel software;

        private TextInputViewModel pssNumber;

        private TextBoxInputViewModel description;

        private TextBoxInputViewModel privateInfosBugs;

        private DefinedSelectionInputListViewModel baseSoftware;

        private DefinedSelectionInputListViewModel customer;
 
        private DefinedSelectionInputListViewModel system;
 
        private DefinedSelectionInputListViewModel author;

        private DateTimeInputViewModel date;

        private DefinedSelectionListsInputViewModel documentsDefinedList;

        private DefinedSelectionSoftwareListsInputViewModel softwaresDefinedList;

        private DefinedSelectionListsInputViewModel propertiesDefinedList;

        /// <summary>
        /// Reference to the database item view model. 
        /// </summary>
        private DatabaseItemViewModel databaseItemViewModel;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DetailedInformationViewModel(ViewModelEvents viewModelEvents, VersionManagementModel versionManagementModel)
            : base(viewModelEvents)
        {
            // Create all commands
            this.AddCommand = new ActionCommand(this.OnAddCommand, this.OnCanExecuteAddCommand);
            this.SaveCommand = new ActionCommand(this.OnSaveCommand, this.OnCanExecuteSaveCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand, this.OnCanExecuteDeleteCommand);

            this.viewModelEvents = viewModelEvents;
            this.versionManagementModel = versionManagementModel;

            this.InitInputs();
        }

        /// <summary>
        /// Occurs when an item changed in the detail information view. Example when the user define a new software item.
        /// </summary>
        public event EventHandler<ItemEventArgs> ItemChangedEvent;

        /// <summary>
        /// Occurs when the system filter is defined in the list of software tab. 
        /// The user defined here a supported motion controller or interface software to the main software which is defined under software name.
        /// </summary>
        public event EventHandler<SelectedItemEventArgs> SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent;

        public TextInputViewModel Software
        {
            get
            {
                return this.software;
            }

            set
            {
                this.software = value;
                this.OnPropertyChanged("Software");
            }
        }

        public TextInputViewModel PssNumber
        {
            get
            {
                return this.pssNumber;
            }

            set
            {
                this.pssNumber = value;
                this.OnPropertyChanged("PssNumber");
            }
        }

        public TextBoxInputViewModel Description
        {
            get
            {
                return this.description;
            }

            set
            {
                this.description = value;
                this.OnPropertyChanged("Description");
            }
        }

        public TextBoxInputViewModel PrivateInfosBugs
        {
            get
            {
                return this.privateInfosBugs;
            }

            set
            {
                this.privateInfosBugs = value;
                this.OnPropertyChanged("PrivateInfosBugs");
            }
        }

        public DefinedSelectionInputListViewModel BaseSoftware
        {
            get
            {
                return this.baseSoftware;
            }

            set
            {
                this.baseSoftware = value;
                this.OnPropertyChanged("BaseSoftware");
            }
        }

        public DefinedSelectionInputListViewModel Customer
        {
            get
            {
                return this.customer;
            }

            set
            {
                this.customer = value;
                this.OnPropertyChanged("Customer");
            }
        }

        public DefinedSelectionInputListViewModel System
        {
            get
            {
                return this.system;
            }

            set
            {
                this.system = value;
                this.OnPropertyChanged("System");
            }
        }

        public DefinedSelectionInputListViewModel Author
        {
            get
            {
                return this.author;
            }

            set
            {
                this.author = value;
                this.OnPropertyChanged("Author");
            }
        }

        public DateTimeInputViewModel Date
        {
            get
            {
                return this.date;
            }

            set
            {
                this.date = value;
                this.OnPropertyChanged("Date");
            }
        }

        public DefinedSelectionSoftwareListsInputViewModel SoftwaresDefinedList
        {
            get
            {
                return this.softwaresDefinedList;
            }

            set
            {
                this.softwaresDefinedList = value;
                this.OnPropertyChanged("SoftwaresDefinedList");
            }
        }

        public DefinedSelectionListsInputViewModel DocumentsDefinedList
        {
            get
            {
                return this.documentsDefinedList;
            }

            set
            {
                this.documentsDefinedList = value;
                this.OnPropertyChanged("DocumentsDefinedList");
            }
        }

        public DefinedSelectionListsInputViewModel PropertiesDefinedList
        {
            get
            {
                return this.propertiesDefinedList;
            }

            set
            {
                this.propertiesDefinedList = value;
                this.OnPropertyChanged("PropertiesDefinedList");
            }
        }

        public ICommand AddCommand
        {
            get;
            private set;
        }

        public ICommand SaveCommand
        {
            get;
            private set;
        }

        public ICommand DeleteCommand
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets a value indicating whether a new item has been set the identification number is -1 and indicates this state.
        /// </summary>
        /// <value>
        /// <c>true</c> if this instance is new item set; otherwise, <c>false</c>.
        /// </value>
        public bool IsNewItemSet
        {
            get
            {
                return this.databaseItemViewModel != null && this.databaseItemViewModel.ItemIdentification == -1;
            }
        }

        public override void SubscribeEvents()
        {
            base.SubscribeEvents();
            this.SoftwaresDefinedList.SelectedItemEvent += this.SoftwaresDefinedList_SelectedItemEvent;
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
            this.SoftwaresDefinedList.SelectedItemEvent -= this.SoftwaresDefinedList_SelectedItemEvent;
            this.SoftwaresDefinedList.UnsubscribeEvents();
        }

        /// <summary>
        /// Gets the software versions which is actually selected in the data grid view. (main window under half)
        /// </summary>
        /// <returns>single software item</returns>
        public SoftwareVersions GetSoftwareItem()
        {
            DatabaseItemViewModel item = this.GetDatabaseItemViewModel();

            return new SoftwareVersions()
            {
                Id = item.ItemIdentification,
                Software = item.Software,
                Base = item.BaseSoftware,
                System = item.System,
                Customer = item.Customer,
                Author = item.Author,
                Date = item.Date,
                PssNumber = item.PssNumber,
                Description = item.Description,
                PrivateInfosBugs = item.PrivateInfosBugs,
                Properties = item.Properties,
                Softwares = item.Softwares,
                Documents = item.Documents
            };
        }

        /// <summary>
        /// Add new instance to avoid update of data grid when editing information in detail view.
        /// </summary>
        /// <param name="item">single software item in database</param>
        public void SetDetailedInformation(DatabaseItemViewModel item)
        {
            this.databaseItemViewModel = new DatabaseItemViewModel()
            {
                ItemIdentification = item.ItemIdentification,
                Software = item.Software,
                BaseSoftware = item.BaseSoftware,
                System = item.System,
                Customer = item.Customer,
                Author = item.Author,
                Date = item.Date,
                PssNumber = item.PssNumber,
                Description = item.Description,
                PrivateInfosBugs = item.PrivateInfosBugs,
                Softwares = item.Softwares,
                Documents = item.Documents,
                Properties = item.Properties
            };

            this.FillInputs(
                this.databaseItemViewModel.Software,
                this.databaseItemViewModel.BaseSoftware,
                this.databaseItemViewModel.System,
                this.databaseItemViewModel.Customer,
                this.databaseItemViewModel.Author,
                this.databaseItemViewModel.Date.ToString(),
                this.databaseItemViewModel.PssNumber.ToString(),
                this.databaseItemViewModel.Description,
                this.databaseItemViewModel.PrivateInfosBugs,
                this.databaseItemViewModel.Softwares,
                this.databaseItemViewModel.Documents,
                this.databaseItemViewModel.Properties);
        }

        /// <summary>
        /// Called when the user want to save a software item.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        /// <returns>Result if save command are executable</returns>
        public bool OnCanExecuteSaveCommand(object parameter)
        {
            return this.Software != null && !string.IsNullOrEmpty(this.Software.TextInput);
        }

        /// <summary>
        /// All software item information are generate a new instance. This are the items which are shows in the detailed information view (upper half of the main window).
        /// </summary>
        private void InitInputs()
        {
            this.Software = new TextInputViewModel(this.viewModelEvents, SoftwareLabel, string.Empty);

            this.BaseSoftware = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                BaseSoftwareLabel,
                string.Empty,
                SelectionItemType.Base,
                this.versionManagementModel,
                false);

            this.System = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                SystemLabel,
                string.Empty,
                SelectionItemType.System,
                this.versionManagementModel,
                true);
            this.System.ToolTip = "Right click to edit the system list";

            this.Customer = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                CustomerLabel,
                string.Empty,
                SelectionItemType.Customer,
                this.versionManagementModel,
                true);
            this.Customer.ToolTip = "Right click to edit the customer list";

            this.Author = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                AuthorLabel,
                string.Empty,
                SelectionItemType.Author,
                this.versionManagementModel,
                true);
            this.Author.ToolTip = "Right click to edit the author list";

            this.Date = new DateTimeInputViewModel(this.viewModelEvents, DateLabel, DateTime.UtcNow);
            this.PssNumber = new TextInputViewModel(this.viewModelEvents, PssNumberLabel, string.Empty);
            this.Description = new TextBoxInputViewModel(this.viewModelEvents, DescriptionLabel, string.Empty);
            this.PrivateInfosBugs = new TextBoxInputViewModel(this.viewModelEvents, PrivateInfosBugsLabel, string.Empty);

            //Init softwares
            this.SoftwaresDefinedList = new DefinedSelectionSoftwareListsInputViewModel(this.viewModelEvents, SoftwaresLabel, SelectionItemType.Softwares, this.versionManagementModel);

            //Init documents
            this.DocumentsDefinedList = new DefinedSelectionListsInputViewModel(this.viewModelEvents, DocumentsLabel, SelectionItemType.DocumentType, this.versionManagementModel);

            //Init properties
            this.PropertiesDefinedList = new DefinedSelectionListsInputViewModel(this.viewModelEvents, PropertiesLabel, SelectionItemType.Properties, this.versionManagementModel);
        }

        /// <summary>
        /// Set the software information fields(example: software, base software, etc).
        /// </summary>
        /// <param name="software">The software.</param>
        /// <param name="baseSoftware">The base software.</param>
        /// <param name="system">The system.</param>
        /// <param name="customer">The customer.</param>
        /// <param name="author">The author.</param>
        /// <param name="date">The date.</param>
        /// <param name="pssNumber">The PSS number.</param>
        /// <param name="description">The description.</param>
        /// <param name="privateInfosBugs">The private infos bugs.</param>
        /// <param name="softwares">The softwares.</param>
        /// <param name="documents">The documents.</param>
        /// <param name="properties">The properties.</param>
        private void FillInputs(
            string software, string baseSoftware, string system, string customer, string author, string date, string pssNumber, string description, string privateInfosBugs, List<Softwares> softwares, List<Documents> documents, List<Properties> properties)
        {
            this.Software.TextInput = software;

            this.BaseSoftware.Init();
            this.BaseSoftware.SelectedItemFilter = this.BaseSoftware.ItemListFilter.Where(x => x.SelectedItem == baseSoftware).FirstOrDefault();

            //object is not allowed to be null
            if (this.BaseSoftware.SelectedItemFilter == null)
                this.BaseSoftware.SelectedItemFilter = new DefinedSelectionItemViewModel() { SelectedItem = string.Empty };

            this.System.Init();
            this.System.SelectedItemFilter = this.System.ItemListFilter.Where(x => x.SelectedItem == system).FirstOrDefault();

            //object is not allowed to be null
            if (this.System.SelectedItemFilter == null)
                this.System.SelectedItemFilter = new DefinedSelectionItemViewModel() { SelectedItem = string.Empty };

            this.Customer.Init();
            this.Customer.SelectedItemFilter = this.Customer.ItemListFilter.Where(x => x.SelectedItem == customer).FirstOrDefault();

            //object is not allowed to be null
            if (this.Customer.SelectedItemFilter == null)
                this.Customer.SelectedItemFilter = new DefinedSelectionItemViewModel() { SelectedItem = string.Empty };

            this.Author.Init();
            this.Author.SelectedItemFilter = this.Author.ItemListFilter.Where(x => x.SelectedItem == author).FirstOrDefault();

            //object is not allowed to be null
            if (this.Author.SelectedItemFilter == null) this.Author.SelectedItemFilter = new DefinedSelectionItemViewModel() { SelectedItem = string.Empty };

            this.Date.DateTimeInput = this.GetDateTime(date);
            this.PssNumber.TextInput = pssNumber;
            this.Description.TextInput = description;
            this.PrivateInfosBugs.TextInput = privateInfosBugs;

            //Fill softwares
            this.SoftwaresDefinedList.UnsubscribeEvents();
            this.SoftwaresDefinedList.ItemListFilter.Clear();

            foreach (Softwares softwareItem in softwares)
            {
                DefinedSelectionDoubleListInputViewModel definedSelectionDoubleListInputViewModel = new DefinedSelectionDoubleListInputViewModel(
                        this.viewModelEvents,
                        softwareItem.System,
                        softwareItem.Software,
                        SelectionItemType.Softwares,
                        this.versionManagementModel,
                        false);

                definedSelectionDoubleListInputViewModel.SubscribeEvents();
                definedSelectionDoubleListInputViewModel.Init();
                this.SoftwaresDefinedList.ItemListFilter.Add(definedSelectionDoubleListInputViewModel);
                definedSelectionDoubleListInputViewModel.SelectedItemFilter = definedSelectionDoubleListInputViewModel.ItemListFilter.Where(x => x.SelectedItem == softwareItem.System).FirstOrDefault();
                definedSelectionDoubleListInputViewModel.SelectedItemDetail = definedSelectionDoubleListInputViewModel.ItemListDetail.Where(x => x == softwareItem.Software).FirstOrDefault();
            }

            this.SoftwaresDefinedList.SubscribeEvents();

            //Fill documents
            this.DocumentsDefinedList.ItemListFilter.Clear();

            foreach (Documents document in documents)
            {
                DefinedSelectionListAndTextInputViewModel item = new DefinedSelectionListAndTextInputViewModel(
                        this.viewModelEvents,
                        document.DocumentType,
                        document.Document,
                        SelectionItemType.DocumentType,
                        this.versionManagementModel,
                        true);

                item.Init();
                this.DocumentsDefinedList.ItemListFilter.Add(item);
                item.SelectedItemFilter = item.ItemListFilter.Where(x => x.SelectedItem == document.DocumentType).FirstOrDefault();
            }

            //Fill properties
            this.PropertiesDefinedList.ItemListFilter.Clear();

            foreach (Properties property in properties)
            {
                DefinedSelectionListAndTooltipViewModel item = new DefinedSelectionListAndTooltipViewModel(
                        this.viewModelEvents,
                        property.PropertyType,
                        SelectionItemType.Properties,
                        this.versionManagementModel,
                        true);

                item.Init();
                this.PropertiesDefinedList.ItemListFilter.Add(item);
                item.SelectedItemFilter = item.ItemListFilter.Where(x => x.SelectedItem == property.PropertyType).FirstOrDefault();
                item.SelectedItemFilter.Description = property.Description;
            }
        }

        /// <summary>
        /// Gets the software of the selected item in the data grid (under half of the main window).
        /// </summary>
        /// <returns>single database item</returns>
        private DatabaseItemViewModel GetDatabaseItemViewModel()
        {
            //only if the grid is never loaded before
            if (this.databaseItemViewModel != null)
            {
                return new DatabaseItemViewModel()
                {
                    ItemIdentification = this.databaseItemViewModel.ItemIdentification,
                    Software = this.Software.TextInput,
                    BaseSoftware = this.BaseSoftware.SelectedItemFilter.SelectedItem,
                    System = this.System.SelectedItemFilter.SelectedItem,
                    Customer = this.Customer.SelectedItemFilter.SelectedItem,
                    Author = this.Author.SelectedItemFilter.SelectedItem,
                    Date = this.GetDateTime(this.Date.DateTimeInput.ToString()),
                    PssNumber = this.GetPssNumber(this.PssNumber.TextInput),
                    Description = this.Description.TextInput,
                    PrivateInfosBugs = this.PrivateInfosBugs.TextInput,
                    Properties = this.GetProperties(this.PropertiesDefinedList),
                    Softwares = this.GetSoftwares(this.SoftwaresDefinedList),
                    Documents = this.GetDocuments(this.DocumentsDefinedList)
                };
            }
            else
            {
                return new DatabaseItemViewModel()
                {
                    ItemIdentification = 0,
                    Software = this.Software.TextInput,
                    BaseSoftware = this.BaseSoftware.SelectedItemFilter.SelectedItem,
                    System = this.System.SelectedItemFilter.SelectedItem,
                    Customer = this.Customer.SelectedItemFilter.SelectedItem,
                    Author = this.Author.SelectedItemFilter.SelectedItem,
                    Date = this.GetDateTime(this.Date.DateTimeInput.ToString()),
                    PssNumber = this.GetPssNumber(this.PssNumber.TextInput),
                    Description = this.Description.TextInput,
                    PrivateInfosBugs = this.PrivateInfosBugs.TextInput,
                    Properties = this.GetProperties(this.PropertiesDefinedList),
                    Softwares = this.GetSoftwares(this.SoftwaresDefinedList),
                    Documents = this.GetDocuments(this.DocumentsDefinedList)
                };
            }
        }

        /// <summary>
        /// Get the list of property. Every Property has a type and a description.
        /// </summary>
        /// <param name="propertiesDefinedList">The properties defined list.</param>
        /// <returns>List of Properties</returns>
        private List<Properties> GetProperties(DefinedSelectionListsInputViewModel propertiesDefinedList)
        {
            List<Properties> propertiesList = new List<Properties>();

            foreach (var item in propertiesDefinedList.ItemListFilter)
            {
                if (item is DefinedSelectionListAndTooltipViewModel)
                {
                    if (item.SelectedItemFilter != null)
                    {
                        //the empty entrys delete in the list
                        if (item.SelectedItemFilter.SelectedItem != string.Empty)
                        {
                            propertiesList.Add(new Properties()
                            {
                                Description = ((DefinedSelectionListAndTooltipViewModel)item).SelectedItemFilter.Description,
                                PropertyType = ((DefinedSelectionListAndTooltipViewModel)item).SelectedItemFilter.SelectedItem
                            });
                        }     
                    }
                    else
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("Property type is required"));
                }
            }

            return propertiesList;
        }

        /// <summary>
        /// Gets the documents. Every document has a type and a link where the file is save.
        /// </summary>
        /// <param name="documentsDefinedList">The documents defined list.</param>
        /// <returns>list of documents</returns>
        private List<Documents> GetDocuments(DefinedSelectionListsInputViewModel documentsDefinedList)
        {
            List<Documents> documentsList = new List<Documents>();

            foreach (var item in documentsDefinedList.ItemListFilter)
            {
                if (item is DefinedSelectionListAndTextInputViewModel)
                {
                    //only save a Document, if an document type is defined
                    if (item.SelectedItemFilter != null)
                    {
                        //the empty entrys delete in the list
                        if (item.SelectedItemFilter.SelectedItem != string.Empty)
                        {
                            documentsList.Add(new Documents()
                            {
                                Document = ((DefinedSelectionListAndTextInputViewModel)item).TextBoxInput,
                                DocumentType = ((DefinedSelectionListAndTextInputViewModel)item).SelectedItemFilter.SelectedItem
                            });
                        }
                    }
                    else
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("Document type is required"));
                }
            }

            return documentsList;
        }

        /// <summary>
        /// Gets the list of software. Every single item has a system and software name. 
        /// The system name is used to predefined the available software versions.
        /// </summary>
        /// <param name="softwaresDefinedList">The software defined list.</param>
        /// <returns>list of softwares</returns>
        private List<Softwares> GetSoftwares(DefinedSelectionSoftwareListsInputViewModel softwaresDefinedList)
        {
            List<Softwares> softwareList = new List<Softwares>();

            foreach (var item in softwaresDefinedList.ItemListFilter)
            {
                if (item is DefinedSelectionDoubleListInputViewModel)
                {
                    if (item.SelectedItemFilter != null)
                    {
                        //the empty entrys delete in the list
                        if (item.SelectedItemFilter.SelectedItem != string.Empty)
                        {
                            softwareList.Add(new Softwares()
                            {
                                System = ((DefinedSelectionDoubleListInputViewModel)item).SelectedItemFilter.SelectedItem,
                                Software = ((DefinedSelectionDoubleListInputViewModel)item).SelectedItemDetail
                            });
                        }    
                    }
                    else
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("System type in the software list is required"));
                }
            }

            return softwareList;
        }

        /// <summary>
        /// Gets the date time.
        /// </summary>
        /// <param name="dateTimeString">The date time string.</param>
        /// <returns>date time</returns>
        private DateTime? GetDateTime(string dateTimeString)
        {
            if (string.IsNullOrEmpty(dateTimeString))
                return DateTime.Now;

            return Convert.ToDateTime(dateTimeString);
        }

        /// <summary>
        /// Gets the PSS number. Is a company relevant number which has 6 digits.
        /// </summary>
        /// <param name="pssNumberString">The PSS number string.</param>
        /// <returns>pss number</returns>
        private int? GetPssNumber(string pssNumberString)
        {
            if (string.IsNullOrEmpty(pssNumberString))
                return null;


            return Convert.ToInt32(pssNumberString);
        }

        /// <summary>
        /// Called when the add button is push on the main window.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnAddCommand(object parameter)
        {
            try
            {
                this.OnItemChangedEvent(this, new AddNewItemEventArgs());
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Decided if the new button is visible. If the item identification is set to -1, then its clear that the button is push before without save.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        /// <returns>result if add command is executable</returns>
        private bool OnCanExecuteAddCommand(object parameter)
        {
            return !this.IsNewItemSet;
        }

        /// <summary>
        /// Called when the add button is push on the main window.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnSaveCommand(object parameter)
        {
            try
            {
                this.OnItemChangedEvent(this, new SaveItemEventArgs(this.GetDatabaseItemViewModel()));
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Called when the delete button is push on the main window.].
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnDeleteCommand(object parameter)
        {
            try
            {
                this.OnItemChangedEvent(this, new DeleteItemEventArgs(this.GetDatabaseItemViewModel()));
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private bool OnCanExecuteDeleteCommand(object parameter)
        {
            try
            {
                return !this.IsNewItemSet;
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
                return false;
            }
        }

        private void OnItemChangedEvent(object sender, ItemEventArgs addNewItemEventArgs)
        {
            if (this.ItemChangedEvent != null)
                this.ItemChangedEvent(sender, addNewItemEventArgs);
        }

        private void SoftwaresDefinedList_SelectedItemEvent(object sender, Events.EventArgs.SelectedItem.SelectedItemEventArgs e)
        {
            this.OnSelectedItemFromDefinedSelectionDoubleListInputViewModelEvent(sender, e);
        }

        /// <summary>
        /// Event to indicate a selection change on the defined selection double list input view model
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="SelectedItemEventArgs"/> instance containing the event data.</param>
        private void OnSelectedItemFromDefinedSelectionDoubleListInputViewModelEvent(object sender, SelectedItemEventArgs e)
        {
            if (this.SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent != null)
                this.SelectedItemFromDefinedSelectionDoubleListInputViewModelEvent(sender, e);
        }
    }
}
