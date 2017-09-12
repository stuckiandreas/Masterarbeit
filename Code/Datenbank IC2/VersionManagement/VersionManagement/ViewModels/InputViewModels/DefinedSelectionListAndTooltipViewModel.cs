//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionListAndTooltipViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.InputViewModels
{
    using Model;
    using VersionManagement.Events;

    public class DefinedSelectionListAndTooltipViewModel : DefinedSelectionInputListViewModel
    {
        public DefinedSelectionListAndTooltipViewModel(
            ViewModelEvents viewModelEvents,
            string comboBoxItem,
            SelectionItemType selectionItemType,
            VersionManagementModel versionManagementModel,
            bool isContextMenuVisible)
            : base(viewModelEvents, string.Empty, comboBoxItem, selectionItemType, versionManagementModel, isContextMenuVisible)
        {
        }

        public override void Init()
        {
            base.Init();
        }
    }
}
