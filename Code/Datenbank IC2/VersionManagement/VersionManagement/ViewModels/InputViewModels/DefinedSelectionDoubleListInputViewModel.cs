//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionDoubleListInputViewModel.cs" company="VAT Vakuumventile AG">
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
    using Events.EventArgs.SelectedItem;
    using Model;
    using VersionManagement.Events.EventArgs.Error;
    using VersionManagement.Model.Database;

    /// <summary>
    /// View model in which a defined type and a text block can be defined. 
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.DefinedSelectionInputListViewModel" />
    public class DefinedSelectionDoubleListInputViewModel : DefinedSelectionInputListViewModel
    {
        /// <summary>
        /// List of the matches software after the user defined the system. 
        /// </summary>
        private List<string> itemListDetail;

        /// <summary>
        /// Selected software item in the right combo box.
        /// </summary>
        private string selectedItemDetail;

        /// <summary>
        /// Defined if the context menu is visible
        /// </summary>
        private bool isContextMenuVisible;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DefinedSelectionDoubleListInputViewModel(
            ViewModelEvents viewModelEvents,
            string comboBoxItemFilter,
            string comboBoxItemDetail,
            SelectionItemType selectionItemType,
            VersionManagementModel versionManagementModel,
            bool isContextMenuVisible)
            : base(viewModelEvents, string.Empty, comboBoxItemFilter, selectionItemType, versionManagementModel, true)
        {
            this.isContextMenuVisible = isContextMenuVisible;
            this.versionManagementModel = versionManagementModel;
            this.viewModelEvents = viewModelEvents;

            this.SelectSoftwareCommand = new ActionCommand(this.OnSelectSoftwareCommand);
        }

        /// <summary>
        /// Occurs when the user double click on the software combo box in the software tab. 
        /// After the double click the detail view model shows this software item.
        /// </summary>
        public event EventHandler<SelectedItemEventArgs> DoubleClickEvent;

        public ICommand SelectSoftwareCommand
        {
            get;
            set;
        }

        public List<string> ItemListDetail
        {
            get
            {
                return this.itemListDetail;
            }

            set
            {
                this.itemListDetail = value;
                this.OnPropertyChanged("ItemListDetail");
            }
        }

        public string SelectedItemDetail
        {
            get
            {
                return this.selectedItemDetail;
            }

            set
            {
                this.selectedItemDetail = value;
                this.OnPropertyChanged("SelectedItemDetail");
            }
        }

        public override void Init()
        {
            base.Init();
        }

        public override void SubscribeEvents()
        {
            base.SubscribeEvents();
            this.ComboboxItemChanged += this.DefinedSelectionDoubleListInputViewModel_ComboboxItemChanged;
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
            this.ComboboxItemChanged -= this.DefinedSelectionDoubleListInputViewModel_ComboboxItemChanged;
        }

        private void OnSelectSoftwareCommand(object parameter)
        {
            try
            {
                this.OnDoubleClickEvent(this, new SelectedItemEventArgs(this.selectedItemDetail));
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private List<string> GetItemList()
        {
            List<string> systemSoftwareList = new List<string>();
            try
            {
                List<Systems> systemList = this.versionManagementModel.SelectionItem.GetSystems();

                //possible that the SelectedItemFilter is not selected at this time
                if (this.SelectedItemFilter != null)
                {
                    foreach (var systemItem in systemList)
                    {
                        if (systemItem.System.Equals(this.SelectedItemFilter.SelectedItem))
                        {
                            foreach (var systemSoftware in systemItem.SystemSoftwares)
                            {
                                systemSoftwareList.Add(systemSoftware.Software);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }

            return systemSoftwareList;
        }

        private void DefinedSelectionDoubleListInputViewModel_ComboboxItemChanged(object sender, EventArgs e)
        {
            try
            {
                this.ItemListDetail = this.GetItemList();
                this.SelectedItemDetail = this.ItemListDetail.FirstOrDefault();
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }            
        }

        private void OnDoubleClickEvent(object sender, SelectedItemEventArgs item)
        {
            if (this.DoubleClickEvent != null)
                this.DoubleClickEvent(this, item);
        }
    }
}
