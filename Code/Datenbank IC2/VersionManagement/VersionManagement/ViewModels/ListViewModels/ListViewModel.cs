//-----------------------------------------------------------------------
// <copyright file="DetailedInformationViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ListViewModels
{
    using Events;
    using InputViewModels;
    using System.Collections.ObjectModel;
    /// <summary>
    /// Base view model for all list for the items or selection item to have an overview
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public abstract class ListViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;
        /// <summary>
        /// The item list filter
        /// </summary>
        private ObservableCollection<DefinedSelectionInputListViewModel> itemListFilter;
        /// <summary>
        /// The header
        /// </summary>
        private string header;

        public ListViewModel(ViewModelEvents viewModelEvents,
            string label,
            ObservableCollection<DefinedSelectionInputListViewModel> itemListFilter) : base(viewModelEvents)
        {
            this.header = label;
            this.itemListFilter = itemListFilter;
        }

        /// <summary>
        /// Gets the header.
        /// </summary>
        /// <value>
        /// The header.
        /// </value>
        public string Header
        {
            get
            {
                return header;
            }
        }

        /// <summary>
        /// Gets or sets the item list filter.
        /// </summary>
        /// <value>
        /// The item list filter.
        /// </value>
        public ObservableCollection<DefinedSelectionInputListViewModel> ItemListFilter
        {
            get
            {
                return itemListFilter;
            }
            set
            {
                itemListFilter = value;
                OnPropertyChanged("ItemListFilter");
            }
        }
    }
}
