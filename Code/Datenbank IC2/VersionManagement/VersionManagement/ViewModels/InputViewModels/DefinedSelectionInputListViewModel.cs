//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionInputListViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows.Input;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Dialog;
    using Events.EventArgs.Error;
    using ExecuteDialogViewModels;
    using Model;
    using VersionManagement.Events.EventArgs.UserFeedback;

    /// <summary>
    /// View model in which a label and a defined text block can be defined.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.InputBaseViewModel" />
    public class DefinedSelectionInputListViewModel : InputBaseViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

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
            VersionManagementModel versionManagementModel,
            bool isContextMenuVisible)
            : base(viewModelEvents, label)
        {
            this.selectionItemType = selectionItemType;
            this.versionManagementModel = versionManagementModel;
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
                //By the properties list the textinput field is the tooltip
                if (this.selectionItemType == SelectionItemType.Properties)
                {
                    SaveExecuteDoubleDialogViewModel saveExecuteDoubleDialogViewModel = new SaveExecuteDoubleDialogViewModel(this.viewModelEvents, "Save", this.versionManagementModel, "Property Type: ", "Description: ", string.Empty, string.Empty);
                    DialogEventArgs eventArgs = new DialogEventArgs(saveExecuteDoubleDialogViewModel, "Save", 120, 400);
                    this.viewModelEvents.OnOpenDialog(this, eventArgs);

                    this.ItemListFilter = this.GetItemList();
                    this.SelectedItemFilter = this.ItemListFilter.LastOrDefault();
                }
                else
                {
                    SaveExecuteDialogViewModel saveExecuteDialogViewModel = new SaveExecuteDialogViewModel(this.viewModelEvents, "Save", this.versionManagementModel, this.selectionItemType);
                    DialogEventArgs eventArgs = new DialogEventArgs(saveExecuteDialogViewModel, "Save", 100, 300);
                    this.viewModelEvents.OnOpenDialog(this, eventArgs);

                    this.ItemListFilter = this.GetItemList();
                    this.SelectedItemFilter = this.ItemListFilter.LastOrDefault();
                }
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
                //By the properties list the textinput field is the tooltip
                if (this.selectionItemType == SelectionItemType.Properties)
                {
                    if (this.SelectedItemFilter != null)
                    {
                        EditExecuteDoubleDialogViewModel editExecuteDoubleDialogViewModel = new EditExecuteDoubleDialogViewModel(this.viewModelEvents, "Save", this.versionManagementModel, "Property Type: ", "Description: ", this.SelectedItemFilter.SelectedItem, this.SelectedItemFilter.Description);
                        DialogEventArgs eventArgs = new DialogEventArgs(editExecuteDoubleDialogViewModel, "Edit", 120, 400);
                        this.viewModelEvents.OnOpenDialog(this, eventArgs);

                        //set the selectionItem
                        this.ItemListFilter = this.GetItemList();
                        foreach (var item in this.ItemListFilter)
                        {
                            if (item.SelectedItem == editExecuteDoubleDialogViewModel.TextInputViewModel1.TextInput && item.Description == editExecuteDoubleDialogViewModel.TextInputViewModel2.TextInput)
                                this.SelectedItemFilter = item;
                        }

                        //Reload database
                        this.viewModelEvents.OnRenameSelectionItemInDatabase();
                    }
                    else
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item is select to edit"));
                }
                else
                {
                    if (this.SelectedItemFilter != null)
                    {
                        EditExecuteDialogViewModel editExecuteDialogViewModel = new EditExecuteDialogViewModel(this.viewModelEvents, "Save", this.versionManagementModel, this.selectionItemType, this.SelectedItemFilter.SelectedItem);
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
                //Properties list has an type and a description entry
                if (this.selectionItemType == SelectionItemType.Properties)
                {
                    if (this.SelectedItemFilter != null)
                    {
                        DeleteExecuteDoubleDialogViewModel deleteExecuteDoubleDialogViewModel = new DeleteExecuteDoubleDialogViewModel(this.viewModelEvents, "Delete", this.versionManagementModel, this.selectionItemType, this.SelectedItemFilter.SelectedItem, this.SelectedItemFilter.Description);
                        DialogEventArgs eventArgs = new DialogEventArgs(deleteExecuteDoubleDialogViewModel, "Delete", 120, 400);
                        this.viewModelEvents.OnOpenDialog(this, eventArgs);

                        //inform the user, if the delete action was not sucessfull
                        if (deleteExecuteDoubleDialogViewModel.ResultDeleteOperation != string.Empty)
                            this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs(deleteExecuteDoubleDialogViewModel.ResultDeleteOperation));
                        
                        //empty item
                        else
                            this.SelectedItemFilter = null;
                    }
                    else
                        this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item is select to delete"));
                }
                else
                {
                    if (this.SelectedItemFilter != null)
                    {
                        DeleteExecuteDialogViewModel deletExecuteDialogViewModel = new DeleteExecuteDialogViewModel(this.viewModelEvents, "Delete", this.versionManagementModel, this.selectionItemType, this.SelectedItemFilter.SelectedItem);
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
                    case SelectionItemType.Author:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetAuthors().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Author }).ToList();
                        break;
                    case SelectionItemType.Base:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetBaseSoftwares().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.BaseSoftware }).ToList();
                        break;
                    case SelectionItemType.System:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetSystems().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.System }).ToList();
                        break;
                    case SelectionItemType.Customer:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetCustomers().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.Customer }).ToList();
                        break;
                    case SelectionItemType.DocumentType:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetDocumentTypes().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.DocumentType }).ToList();
                        break;
                    case SelectionItemType.Properties:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetPropertyTypes().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.PropertyType, Description = x.PropertyDescription }).ToList();
                        break;
                    case SelectionItemType.Softwares:
                        result = this.versionManagementModel.SelectionItemDatabaseManager.GetSystems().Select(x => new DefinedSelectionItemViewModel() { SelectedItem = x.System }).ToList();
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
