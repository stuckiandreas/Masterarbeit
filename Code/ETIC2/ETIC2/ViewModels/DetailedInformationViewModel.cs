//-----------------------------------------------------------------------
// <copyright file="DetailedInformationViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Linq;
    using System.Windows.Input;
    using ActionCommands;
    using BuglistViewModels;
    using ETIC2.Model.Application;
    using Events;
    using Events.EventArgs.BuglistItem;
    using Events.EventArgs.Error;
    using InputViewModels;
    using Model.Application.BuglistView;
    
    /// <summary>
    /// Shows the detail information of the selected buglist item in the databaseDataGrid
    /// </summary>
    public class DetailedInformationViewModel : BasisViewModel
    {
        //Input labels
        private const string FailureTypeLabel = "Failure Type:";

        private const string StatusTypeLabel = "Status Type:";

        private const string ControllerTypeLabel = "Controller Type:";

        private const string HardwareIdentificationLevel1Label = "Hardware Identification Level 1:";

        private const string HardwareIdentificationLevel2Label = "Hardware Identification Level 2:";

        private const string BugLabel = "Bug:";

        private const string CommentLabel = "Comment:";

        private const string PriorityLabel = "Priority:";

        private const string DateFoundLabel = "Date Found:";

        private const string DateFixedLabel = "Date Fixed:";

        private readonly ETIC2Model etic2Model;

        //Inputs
        private DefinedSelectionInputListViewModel failureType;

        private DefinedSelectionInputListViewModel statusType;

        private DefinedSelectionInputListViewModel controllerType;

        private DefinedSelectionInputListViewModel hardwareIdentificationLevel1;

        private DefinedSelectionInputListViewModel hardwareIdentificationLevel2;

        private TextBoxInputViewModel bug;

        private TextBoxInputViewModel comment;

        private DefinedSelectionInputListViewModel priority;

        private DateTimeInputViewModel dateFound;

        private DateTimeInputViewModel dateFixed;

        /// <summary>
        /// Reference to the database item view model. 
        /// </summary>
        private DatabaseItemViewModel databaseItemViewModel;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DetailedInformationViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            // Create all commands
            this.NewCommand = new ActionCommand(this.OnNewCommand, this.OnCanExecuteNewCommand);
            this.SaveCommand = new ActionCommand(this.OnSaveCommand, this.OnCanExecuteSaveCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand, this.OnCanExecuteDeleteCommand);

            this.viewModelEvents = viewModelEvents;
            this.etic2Model = etic2Model;

            this.InitInputs();
        }

        /// <summary>
        /// Occurs when an item changed in the detail information view. Example when the user define a new software item.
        /// </summary>
        public event EventHandler<ItemEventArgs> ItemChangedEvent;

        public DefinedSelectionInputListViewModel FailureType
        {
            get
            {
                return this.failureType;
            }

            set
            {
                this.failureType = value;
                this.OnPropertyChanged("FailureType");
            }
        }

        public DefinedSelectionInputListViewModel StatusType
        {
            get
            {
                return this.statusType;
            }

            set
            {
                this.statusType = value;
                this.OnPropertyChanged("StatusType");
            }
        }

        public DefinedSelectionInputListViewModel Priority
        {
            get
            {
                return this.priority;
            }

            set
            {
                this.priority = value;
                this.OnPropertyChanged("Priority");
            }
        }

        public DefinedSelectionInputListViewModel ControllerType
        {
            get
            {
                return this.controllerType;
            }

            set
            {
                this.controllerType = value;
                this.OnPropertyChanged("ControllerType");
            }
        }


        public DefinedSelectionInputListViewModel HardwareIdentificationLevel1
        {
            get
            {
                return this.hardwareIdentificationLevel1;
            }

            set
            {
                this.hardwareIdentificationLevel1 = value;
                this.OnPropertyChanged("HardwareIdentificationLevel1");
            }
        }


        public DefinedSelectionInputListViewModel HardwareIdentificationLevel2
        {
            get
            {
                return this.hardwareIdentificationLevel2;
            }

            set
            {
                this.hardwareIdentificationLevel2 = value;
                this.OnPropertyChanged("HardwareIdentificationLevel2");
            }
        }


        public TextBoxInputViewModel Bug
        {
            get
            {
                return this.bug;
            }

            set
            {
                this.bug = value;
                this.OnPropertyChanged("Bug");
            }
        }


        public TextBoxInputViewModel Comment
        {
            get
            {
                return this.comment;
            }

            set
            {
                this.comment = value;
                this.OnPropertyChanged("Comment");
            }
        }

        public DateTimeInputViewModel DateFound
        {
            get
            {
                return this.dateFound;
            }

            set
            {
                this.dateFound = value;
                this.OnPropertyChanged("DateFound");
            }
        }

        public DateTimeInputViewModel DateFixed
        {
            get
            {
                return this.dateFixed;
            }

            set
            {
                this.dateFixed = value;
                this.OnPropertyChanged("DateFixed");
            }
        }

        public ICommand NewCommand
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
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
        }

        /// <summary>
        /// Gets the buglist item which is actually selected in the data grid view. (main window under half)
        /// </summary>
        /// <returns>single software item</returns>
        public Buglist GetBuglistItem()
        {
            DatabaseItemViewModel item = this.GetDatabaseItemViewModel();

            //only save dateFixed if the status type is not open
            if (databaseItemViewModel.StatusType == "open")
            {
                return new Buglist()
                {
                    Id = item.ItemIdentification,
                    FailureType = item.FailureType,
                    StatusType = item.StatusType,
                    Priority = item.Priority,
                    ControllerType = item.ControllerType,
                    HardwareIdentificationLevel1 = item.HardwareIdentificationLevel1,
                    HardwareIdentificationLevel2 = item.HardwareIdentificationLevel2,
                    DateFound = item.DateFound,
                    DateFixed = null,
                    Bug = item.Bug,
                    Comment = item.Comment
                };
            }
            else
            {
                return new Buglist()
                {
                    Id = item.ItemIdentification,
                    FailureType = item.FailureType,
                    StatusType = item.StatusType,
                    Priority = item.Priority,
                    ControllerType = item.ControllerType,
                    HardwareIdentificationLevel1 = item.HardwareIdentificationLevel1,
                    HardwareIdentificationLevel2 = item.HardwareIdentificationLevel2,
                    DateFound = item.DateFound,
                    DateFixed = item.DateFixed,
                    Bug = item.Bug,
                    Comment = item.Comment
                };
            }
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
                FailureType = item.FailureType,
                StatusType = item.StatusType,
                Priority = item.Priority,
                ControllerType = item.ControllerType,
                HardwareIdentificationLevel1 = item.HardwareIdentificationLevel1,
                HardwareIdentificationLevel2 = item.HardwareIdentificationLevel2,
                DateFound = item.DateFound,
                DateFixed = item.DateFixed,
                Bug = item.Bug,
                Comment = item.Comment
            };

            this.FillInputs(
                this.databaseItemViewModel.FailureType,
                this.databaseItemViewModel.StatusType,
                this.databaseItemViewModel.Priority,
                this.databaseItemViewModel.ControllerType,
                this.databaseItemViewModel.HardwareIdentificationLevel1,
                this.databaseItemViewModel.HardwareIdentificationLevel2,
                this.databaseItemViewModel.DateFound,
                this.databaseItemViewModel.DateFixed,
                this.databaseItemViewModel.Bug,
                this.databaseItemViewModel.Comment);
        }

        /// <summary>
        /// Called when the user want to save a buglist item.
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        /// <returns>Result if save command are executable</returns>
        public bool OnCanExecuteSaveCommand(object parameter)
        {
            return this.Bug != null && this.DateFound != null && !string.IsNullOrEmpty(this.Bug.TextInput) && !string.IsNullOrEmpty(this.DateFound.ToString());
        }

        /// <summary>
        /// All bugllist item information are generate a new instance. This are the items which are shows in the detailed information view (upper half of the main window).
        /// </summary>
        private void InitInputs()
        {
            this.FailureType = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                FailureTypeLabel,
                string.Empty,
                SelectionItemType.FailureType,
                this.etic2Model,
                true);
            this.FailureType.ToolTip = "Right click to edit the failure type list";

            this.StatusType = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                StatusTypeLabel,
                string.Empty,
                SelectionItemType.StatusType,
                this.etic2Model,
                true);
            this.StatusType.ToolTip = "Right click to edit the status type list";

            this.Priority = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                PriorityLabel,
                string.Empty,
                SelectionItemType.Priority,
                this.etic2Model,
                true);
            this.Priority.ToolTip = "Right click to edit the priority type list";

            this.ControllerType = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                ControllerTypeLabel,
                string.Empty,
                SelectionItemType.ControllerType,
                this.etic2Model,
                false);

            this.HardwareIdentificationLevel1 = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                HardwareIdentificationLevel1Label,
                string.Empty,
                SelectionItemType.HardwareIdentificationLevel1,
                this.etic2Model,
                true);
            this.HardwareIdentificationLevel1.ToolTip = "Right click to edit the hardware identification level 1 list";

            this.HardwareIdentificationLevel2 = new DefinedSelectionInputListViewModel(
                this.viewModelEvents,
                HardwareIdentificationLevel2Label,
                string.Empty,
                SelectionItemType.HardwareIdentificationLevel2,
                this.etic2Model,
                true);
            this.HardwareIdentificationLevel2.ToolTip = "Right click to edit the hardware identification level 2 list";

            this.DateFound = new DateTimeInputViewModel(this.viewModelEvents, DateFoundLabel, DateTime.UtcNow);
            this.DateFixed = new DateTimeInputViewModel(this.viewModelEvents, DateFixedLabel, null);
            this.Bug = new TextBoxInputViewModel(this.viewModelEvents, BugLabel, string.Empty);
            this.Comment = new TextBoxInputViewModel(this.viewModelEvents, CommentLabel, string.Empty);
        }

        private void FillInputs(
            string failureType, 
            string statusType,
            string priority, 
            string controllerType, 
            string hardwareIdentificationLevel1, 
            string hardwareIdentificationLevel2, 
            DateTime dateFound, 
            DateTime? dateFixed, 
            string bug, 
            string comment)
        {
            this.FailureType.Init();
            this.FailureType.SelectedItemFilter = this.FailureType.ItemListFilter.Where(x => x.SelectedItem == failureType).FirstOrDefault();

            this.StatusType.Init();
            this.StatusType.SelectedItemFilter = this.StatusType.ItemListFilter.Where(x => x.SelectedItem == statusType).FirstOrDefault();

            this.Priority.Init();
            this.Priority.SelectedItemFilter = this.Priority.ItemListFilter.Where(x => x.SelectedItem == priority).FirstOrDefault();

            this.ControllerType.Init();
            this.ControllerType.SelectedItemFilter = this.ControllerType.ItemListFilter.Where(x => x.SelectedItem == controllerType).FirstOrDefault();

            this.HardwareIdentificationLevel1.Init();
            this.HardwareIdentificationLevel1.SelectedItemFilter = this.HardwareIdentificationLevel1.ItemListFilter.Where(x => x.SelectedItem == hardwareIdentificationLevel1).FirstOrDefault();

            this.HardwareIdentificationLevel2.Init();
            this.HardwareIdentificationLevel2.SelectedItemFilter = this.HardwareIdentificationLevel2.ItemListFilter.Where(x => x.SelectedItem == hardwareIdentificationLevel2).FirstOrDefault();

            this.DateFound.DateTimeInput = dateFound;
            this.DateFixed.DateTimeInput = dateFixed;

            this.Bug.TextInput = bug;
            this.Comment.TextInput = comment;
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
                //only save dateFixed if the status type is not open
                if (this.databaseItemViewModel.StatusType == "open")
                {
                    return new DatabaseItemViewModel()
                    {
                        ItemIdentification = this.databaseItemViewModel.ItemIdentification,
                        FailureType = this.FailureType.SelectedItemFilter.SelectedItem,
                        StatusType = this.StatusType.SelectedItemFilter.SelectedItem,
                        Priority = this.Priority.SelectedItemFilter.SelectedItem,
                        ControllerType = this.ControllerType.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel1 = this.HardwareIdentificationLevel1.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel2 = this.HardwareIdentificationLevel2.SelectedItemFilter.SelectedItem,
                        DateFound = (DateTime)this.DateFound.DateTimeInput,
                        DateFixed = null,
                        Bug = this.Bug.TextInput,
                        Comment = this.Comment.TextInput
                    };
                }
                else
                {
                    return new DatabaseItemViewModel()
                    {
                        ItemIdentification = this.databaseItemViewModel.ItemIdentification,
                        FailureType = this.FailureType.SelectedItemFilter.SelectedItem,
                        StatusType = this.StatusType.SelectedItemFilter.SelectedItem,
                        Priority = this.Priority.SelectedItemFilter.SelectedItem,
                        ControllerType = this.ControllerType.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel1 = this.HardwareIdentificationLevel1.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel2 = this.HardwareIdentificationLevel2.SelectedItemFilter.SelectedItem,
                        DateFound = (DateTime)this.DateFound.DateTimeInput,
                        DateFixed = this.DateFixed.DateTimeInput,
                        Bug = this.Bug.TextInput,
                        Comment = this.Comment.TextInput
                    };
                }
            }
            else
            {
                //only save dateFixed if the status type is not open
                if (this.databaseItemViewModel.StatusType == "open")
                {
                    return new DatabaseItemViewModel()
                    {
                        ItemIdentification = 0,
                        FailureType = this.FailureType.SelectedItemFilter.SelectedItem,
                        StatusType = this.StatusType.SelectedItemFilter.SelectedItem,
                        Priority = this.Priority.SelectedItemFilter.SelectedItem,
                        ControllerType = this.ControllerType.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel1 = this.HardwareIdentificationLevel1.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel2 = this.HardwareIdentificationLevel2.SelectedItemFilter.SelectedItem,
                        DateFound = (DateTime)this.DateFound.DateTimeInput,
                        DateFixed = null,
                        Bug = this.Bug.TextInput,
                        Comment = this.Comment.TextInput
                    };
                }
                else
                {
                    return new DatabaseItemViewModel()
                    {
                        ItemIdentification = 0,
                        FailureType = this.FailureType.SelectedItemFilter.SelectedItem,
                        StatusType = this.StatusType.SelectedItemFilter.SelectedItem,
                        Priority = this.Priority.SelectedItemFilter.SelectedItem,
                        ControllerType = this.ControllerType.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel1 = this.HardwareIdentificationLevel1.SelectedItemFilter.SelectedItem,
                        HardwareIdentificationLevel2 = this.HardwareIdentificationLevel2.SelectedItemFilter.SelectedItem,
                        DateFound = (DateTime)this.DateFound.DateTimeInput,
                        DateFixed = this.DateFixed.DateTimeInput,
                        Bug = this.Bug.TextInput,
                        Comment = this.Comment.TextInput
                    };
                }
            }
        }

        private void OnNewCommand(object parameter)
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

        private bool OnCanExecuteNewCommand(object parameter)
        {
            return !this.IsNewItemSet;
        }

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

        private DateTime GetDateTime(string dateTimeString)
        {
            if (string.IsNullOrEmpty(dateTimeString))
                return DateTime.Now;

            return Convert.ToDateTime(dateTimeString);
        }
    }
}
