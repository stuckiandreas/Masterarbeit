//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionSoftwareListsInputViewModel.cs" company="VAT Vakuumventile AG">
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
    using Events.EventArgs.SelectedItem;
    using Events.EventArgs.UserFeedback;
    using Model;

    /// <summary>
    /// View model to defined the software list of an single software. 
    /// Every firmware (specific software for the valve) has some compatible software for the motion controller and interface.
    /// The list includes the system and the software. If the user select a system the software selection list update automatically.
    /// That means the user is able only select an software which is saved in database.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.InputBaseViewModel" />
    public class DefinedSelectionSoftwareListsInputViewModel : InputBaseViewModel
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
        /// List of the selection system items.
        /// </summary>
        private ObservableCollection<DefinedSelectionDoubleListInputViewModel> itemListFilter;

        /// <summary>
        /// Type of the combo box element.
        /// </summary>
        private SelectionItemType selectionItemType;

        /// <summary>
        /// Header of the group box
        /// </summary>
        private string header;

        public DefinedSelectionSoftwareListsInputViewModel(
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
            this.itemListFilter = new ObservableCollection<DefinedSelectionDoubleListInputViewModel>();

            this.AddCommand = new ActionCommand(this.OnAddCommand);
            this.DeleteCommand = new ActionCommand(this.OnDeleteCommand);
        }

        /// <summary>
        /// Occurs when the user defined the selection system item in the combo box.
        /// </summary>
        public event EventHandler<SelectedItemEventArgs> SelectedItemEvent;

        public string Header
        {
            get
            {
                return this.header;
            }
        }

        public ObservableCollection<DefinedSelectionDoubleListInputViewModel> ItemListFilter
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

        public override void SubscribeEvents()
        {
            base.SubscribeEvents();

            foreach (DefinedSelectionDoubleListInputViewModel item in this.ItemListFilter)
                item.DoubleClickEvent += this.Item_DoubleClickEvent;
        }

        public override void UnsubscribeEvents()
        {
            foreach (DefinedSelectionDoubleListInputViewModel item in this.ItemListFilter)
                item.DoubleClickEvent -= this.Item_DoubleClickEvent;

            base.UnsubscribeEvents();
        }

        private void OnAddCommand(object parameter)
        {
            try
            {
                this.UnsubscribeEvents();

                DefinedSelectionDoubleListInputViewModel content = new DefinedSelectionDoubleListInputViewModel(
                this.viewModelEvents,
                string.Empty,
                string.Empty,
                this.selectionItemType,
                this.versionManagementModel,
                false);

                content.SubscribeEvents();
                content.Init();
                this.ItemListFilter.Add(content);

                this.SubscribeEvents();
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
                this.UnsubscribeEvents();

                if (this.ItemListFilter.Count > 0)
                {
                    this.ItemListFilter.RemoveAt(this.ItemListFilter.Count - 1);
                }
                else
                {
                    this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("No Item to delete"));
                }

                //Subscribe events
                this.SubscribeEvents();
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void Item_DoubleClickEvent(object sender, Events.EventArgs.SelectedItem.SelectedItemEventArgs e)
        {
            this.OnSelectedItemEvent(sender, e);
        }

        private void OnSelectedItemEvent(object sender, SelectedItemEventArgs e)
        {
            if (this.SelectedItemEvent != null)
                this.SelectedItemEvent(sender, e);
        }
    }
}
