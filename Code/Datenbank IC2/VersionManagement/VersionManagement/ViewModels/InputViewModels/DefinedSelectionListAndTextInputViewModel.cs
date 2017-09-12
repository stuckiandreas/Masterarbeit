//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionListAndTextInputViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using Events;
    using Model;

    /// <summary>
    /// View model in which a defined type and a text block can be defined. 
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.InputViewModels.DefinedSelectionInputListViewModel" />
    public class DefinedSelectionListAndTextInputViewModel : DefinedSelectionInputListViewModel
    {
        /// <summary>
        /// Text input view model of the selection item
        /// </summary>
        private string textBoxInput;

        public DefinedSelectionListAndTextInputViewModel(
            ViewModelEvents viewModelEvents,
            string comboBoxItem,
            string textBoxInput,
            SelectionItemType selectionItemType,
            VersionManagementModel versionManagementModel,
            bool isContextMenuVisible)
            : base(viewModelEvents, string.Empty, comboBoxItem, selectionItemType, versionManagementModel, isContextMenuVisible)
        {
            this.TextBoxInput = textBoxInput;
        }

        public string TextBoxInput
        {
            get
            {
                return this.textBoxInput;
            }

            set
            {
                this.textBoxInput = value;
                this.OnPropertyChanged("TextBoxInput");
            }
        }

        public override void Init()
        {
            base.Init();
        }
    }
}
