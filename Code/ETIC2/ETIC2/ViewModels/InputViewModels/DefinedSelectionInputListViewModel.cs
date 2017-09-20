//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionInputListViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.InputViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows.Input;
    using ActionCommands;
    using ETIC2.Events;
    using Events.EventArgs.Dialog;
    using Events.EventArgs.Error;
    using Events.EventArgs.UserFeedback;
    using ExecuteDialogViewModels;
    using Model.Application;

    /// <summary>
    /// View model in which a label and a defined text block can be defined.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.InputViewModels.InputBaseViewModel" />
    public class DefinedSelectionInputListViewModel : InputBaseViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

        /// <summary>
        /// Possible item to fill the combo box field. This are defined items. 
        /// Mostly the user are able to define over the dialog window.
        /// </summary>
        private List<DefinedSelectionItemViewModel> itemListFilter;

        /// <summary>
        /// Type of the combo box element.
        /// </summary>
        private SelectionItemType selectionItemType;

        /// <summary>
        /// Name of the selected item in the combo box.
        /// </summary>
        private DefinedSelectionItemViewModel selectedItemFilter;

        /// <summary>
        /// Defined if the context menu is visible
        /// </summary>
        private bool isContextMenuVisible;

        /// <summary>
        /// The is label available
        /// </summary>
        private bool isLabelAvailable;

        /// <summary>
        /// The is delete item button pressed
        /// </summary>
        protected bool isDeleteItemButtonPressed;

        public DefinedSelectionInputListViewModel(
            ViewModelEvents viewModelEvents,
            string label,
            string selectedItem,
            SelectionItemType selectionItemType,
            ETIC2Model etic2Model,
            bool isContextMenuVisible)
            : base(viewModelEvents, label)
        {
            this.selectionItemType = selectionItemType;
            this.etic2Model = etic2Model;
            this.viewModelEvents = viewModelEvents;
            this.isContextMenuVisible = isContextMenuVisible;
            this.isDeleteItemButtonPressed = false;

            this.AddCommand = new ActionCommand(this.OnAddCommand);
            this.EditCommand = new ActionCommand(this.OnEditCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand);
        }

        /// <summary>
        /// Occurs when the user changed the combo box field.
        /// </summary>
        protected event EventHandler ComboboxItemChanged;

        /// <summary>
        /// Gets or sets the item list.
        /// </summary>
        /// <value>
        /// The item list.
        /// </value>
        public List<DefinedSelectionItemViewModel> ItemListFilter
        {
            get
            {
                return this.itemListFilter;
            }

            set
            {
                this.itemListFilter = value;
                this.OnPropertyChanged("ItemListFilter");
            }
        }

        /// <summary>
        /// Gets or sets the selected item.
        /// </summary>
        /// <value>
        /// The selected item.
        /// </value>
        public DefinedSelectionItemViewModel SelectedItemFilter
        {
            get
            {
                return this.selectedItemFilter;
            }

            set
            {
                this.selectedItemFilter = value;
                this.OnPropertyChanged("SelectedItemFilter");
                this.OnComboboxItemChanged();
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether context menu is visible
        /// </summary>
        public bool IsLabelAvailable
        {
            get
            {
                return this.isLabelAvailable;
            }

            set
            {
                this.isLabelAvailable = true;
                this.OnPropertyChanged("IsLabelAvailable");
            }
        }

        public bool IsContextMenuVisible
        {
            get
            {
                return this.isContextMenuVisible;
            }

            set
            {
                this.isContextMenuVisible = value;
                this.OnPropertyChanged("IsContextMenuVisible");
            }
        }

        public bool IsDeleteItemButtonPressed
        {
            get
            {
                return this.isDeleteItemButtonPressed;
            }

            set
            {
                this.isDeleteItemButtonPressed = value;
                this.OnPropertyChanged("IsDeleteItemButtonPressed");
            }
        }

        public ICommand AddCommand
        {
            get;
            set;
        }

        public ICommand EditCommand
        {
            get;
            set;
        }

        public ICommand DeleteCommand
        {
            get;
            set;
        }

        public override void Init()
        {
            base.Init();

            this.ItemListFilter = this.GetItemList();
            this.SelectedItemFilter = this.ItemListFilter.Where(x => x == this.selectedItemFilter).FirstOrDefault();
        }

        public override void SubscribeEvents()
        {
            this.viewModelEvents.DeleteSelectionItemsInList += this.ViewModelEvents_DeleteSelectionItemsInList;

            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            this.viewModelEvents.DeleteSelectionItemsInList -= this.ViewModelEvents_DeleteSelectionItemsInList;

            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        private void OnAddCommand(object parameter)
        {
            try
            {
                SaveExecuteDialogViewModel saveExecuteDialogViewModel = new SaveExecuteDialogViewModel(this.viewModelEvents, "Save", this.etic2Model, this.selectionItemType);
                DialogEventArgs eventArgs = new DialogEventArgs(saveExecuteDialogViewModel, "Save", 100, 300);
                this.viewModelEvents.OnOpenDialog(this, eventArgs);

                this.ItemListFilter = this.GetItemList();
                this.SelectedItemFilter = this.ItemListFilter.LastOrDefault();
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnEditCommand(object parameter)
        {
            try
            {
                if (this.SelectedItemFilter != null)
                {
                    EditExecuteDialogViewModel editExecuteDialogViewModel = new EditExecuteDialogViewModel(this.viewModelEvents, "Save", this.etic2Model, this.selectionItemType, this.SelectedItemFilter.SelectedItem);
                    DialogEventArgs eventArgs = new DialogEventArgs(editExecuteDialogViewModel, "Edit", 100, 300);
                    this.viewModelEvents.OnOpenDialog(this, eventArgs);

                    //set the selectionItem
                    this.ItemListFilter = this.GetItemList();
                    foreach (var item in this.ItemListFilter)
                    {
                        if (item.SelectedItem == editExecuteDialogViewModel.SelectedItem)
                            this.SelectedItemFilter = item;
                    }

                    //Reload database
                    this.viewModelEvents.OnRenameSelectionItemInDatabase();
                }
                else
                    this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item is select to edit"));
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Called when [delete command].
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnDeleteCommand(object parameter)
        {
            try
            {
                if (this.SelectedItemFilter != null)
                {
                    DeleteExecuteDialogViewModel deletExecuteDialogViewModel = new DeleteExecuteDialogViewModel(this.viewModelEvents, "Delete", this.etic2Model, this.selectionItemType, this.SelectedItemFilter.SelectedItem);
                    DialogEventArgs eventArgs = new DialogEventArgs(deletExecuteDialogViewModel, "Delete", 100, 300);
                    this.viewModelEvents.OnOpenDialog(this, eventArgs);

                    //inform the user, if the delete action was not sucessfull
                    if (deletExecuteDialogViewModel.ResultDeleteOperation != string.Empty)
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs(deletExecuteDialogViewModel.ResultDeleteOperation));

                    //empty item
                    else
                        this.SelectedItemFilter = null;
                }
                else
                    this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item is select to delete"));
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Get items
        /// </summary>
        /// <returns>List with defined seletion items</returns>
        private List<DefinedSelectionItemViewModel> GetItemList()
        {
            List<DefinedSelectionItemViewModel> result = new List<DefinedSelectionItemViewModel>();

            try
            {
                switch (this.selectionItemType)
                {
                    case SelectionItemType.FailureType:
                        result = this.etic2Model.BuglistSelectedItem.GetFailureTypes().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    case SelectionItemType.StatusType:
                        result = this.etic2Model.BuglistSelectedItem.GetStatusTypes().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    case SelectionItemType.Priority:
                        result = this.etic2Model.BuglistSelectedItem.GetPriorities().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    case SelectionItemType.ControllerType:
                        result = this.etic2Model.BuglistSelectedItem.GetControllerTypes().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    case SelectionItemType.HardwareIdentificationLevel1:
                        result = this.etic2Model.BuglistSelectedItem.GetHardwareIdentificationLevels1().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    case SelectionItemType.HardwareIdentificationLevel2:
                        result = this.etic2Model.BuglistSelectedItem.GetHardwareIdentificationLevels2().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Name }).ToList();
                        break;
                    default:
                        throw new NotImplementedException("Unknown SelectionItemType; " + this.selectionItemType);
                }
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }

            return result;
        }

        /// <summary>
        /// Fire combo box item changed event
        /// </summary>
        private void OnComboboxItemChanged()
        {
            if (this.ComboboxItemChanged != null)
                this.ComboboxItemChanged(this, null);
        }

        /// <summary>
        /// Handles the DeleteSelectionItemsInList event of the ViewModelEvents control.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_DeleteSelectionItemsInList(object sender, EventArgs e)
        {
            this.IsDeleteItemButtonPressed = true;
        }
    }
}
