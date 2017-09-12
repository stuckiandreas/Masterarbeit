//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionListsInputViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows.Input;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Error;
    using Model;

    /// <summary>
    /// View model in which a list of defined type and a text block can be defined. 
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.InputBaseViewModel" />
    public class DefinedSelectionListsInputViewModel : InputBaseViewModel
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
        /// Filter item view models
        /// </summary>
        private ObservableCollection<DefinedSelectionInputListViewModel> itemListFilter;

        /// <summary>
        /// Type of the combo box element.
        /// </summary>
        private SelectionItemType selectionItemType;

        /// <summary>
        /// Header of the group box
        /// </summary>
        private string header;

        public DefinedSelectionListsInputViewModel(
            ViewModelEvents viewModelEvents,
            string label,
            SelectionItemType selectionItemType,
            VersionManagementModel versionManagementModel)
            : base(viewModelEvents, label)
        {
            this.selectionItemType = selectionItemType;
            this.versionManagementModel = versionManagementModel;
            this.viewModelEvents = viewModelEvents;
            this.header = label;
            this.itemListFilter = new ObservableCollection<DefinedSelectionInputListViewModel>();

            this.AddCommand = new ActionCommand(this.OnAddCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand);
        }

        public string Header
        {
            get
            {
                return this.header;
            }
        }

        public ICommand AddCommand
        {
            get;
            set;
        }

        public ICommand DeleteCommand
        {
            get;
            set;
        }

        public ObservableCollection<DefinedSelectionInputListViewModel> ItemListFilter
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

        private void OnAddCommand(object parameter)
        {
            try
            {
                //By the properties list the textinput field is the tooltip
                if (this.selectionItemType == SelectionItemType.Properties)
                {
                    DefinedSelectionListAndTooltipViewModel content = new DefinedSelectionListAndTooltipViewModel(
                        this.viewModelEvents,
                        string.Empty,
                        this.selectionItemType,
                        this.versionManagementModel,
                        true);

                    content.Init();
                    this.ItemListFilter.Add(content);
                }
                else
                {
                    DefinedSelectionListAndTextInputViewModel content = new DefinedSelectionListAndTextInputViewModel(
                        this.viewModelEvents,
                        string.Empty,
                        string.Empty,
                        this.selectionItemType,
                        this.versionManagementModel,
                        true);

                    content.Init();
                    this.ItemListFilter.Add(content);
                }
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }      
        }

        private void OnDeleteCommand(object parameter)
        {
            this.viewModelEvents.OnDeleteSelectionItemsInList();
            /*try
            {
                if (ItemListFilter.Count > 0)
                {
                    ItemListFilter.RemoveAt(ItemListFilter.Count - 1);
                }
                else
                {
                    viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item to delete"));
                }
            }
            catch (Exception ex)
            {
                viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }*/  
        }
    }
}
