//-----------------------------------------------------------------------
// <copyright file="DefinedSelectionItemViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.InputViewModels
{
    /// <summary>
    /// Is used in the DefinedSelectionInputListViewModel
    /// </summary>
    public class DefinedSelectionItemViewModel
    {
        public DefinedSelectionItemViewModel()
        {
        }

        public string SelectedItem
        {
            get;
            set;
        }

        public string Description
        {
            get;
            set;
        }
    }
}
