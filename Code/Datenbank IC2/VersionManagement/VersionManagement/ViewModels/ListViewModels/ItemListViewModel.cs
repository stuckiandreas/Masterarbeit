using System;
//-----------------------------------------------------------------------
// <copyright file="DetailedInformationViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ListViewModels
{
    using InputViewModels;
    using System.Collections.ObjectModel;
    using VersionManagement.Events;

    public class ItemListViewModel : ListViewModel 
    {
        public ItemListViewModel(ViewModelEvents viewModelEvents,
            string label,
            ObservableCollection<DefinedSelectionInputListViewModel> itemListFilter) : base (viewModelEvents, label, itemListFilter)
        {

        }
    }
}
