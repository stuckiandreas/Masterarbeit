//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System.Collections.ObjectModel;
    using Events;

    /// <summary>
    /// Contains data grid data items.
    /// </summary>
    public class DatabaseDataGridViewModel : BasisViewModel
    {
        /// <summary>
        /// List of all software items in the grid view.
        /// </summary>
        private ObservableCollection<InitialStateFirmwareViewModel> initialStateFirmwareViewModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.initialStateFirmwareViewModels = new ObservableCollection<InitialStateFirmwareViewModel>();
        }

        public ObservableCollection<InitialStateFirmwareViewModel> InitialStateFirmwareViewModels
        {
            get
            {
                return this.initialStateFirmwareViewModels;
            }
        }
    }
}
