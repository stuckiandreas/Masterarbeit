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
        /// List of all test initial state firmware entries in the grid view.
        /// </summary>
        private ObservableCollection<InitialStateFirmwareViewModel> initialStateFirmwareViewModels;

        /// <summary>
        /// List of all test test collection result entries in the grid view.
        /// </summary>
        private ObservableCollection<TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.initialStateFirmwareViewModels = new ObservableCollection<InitialStateFirmwareViewModel>();
            this.testCollectionResultWithValveHardwareViewModels = new ObservableCollection<TestCollectionResultWithValveHardwareViewModel>();
        }

        public ObservableCollection<InitialStateFirmwareViewModel> InitialStateFirmwareViewModels
        {
            get
            {
                return this.initialStateFirmwareViewModels;
            }
        }

        public ObservableCollection<TestCollectionResultWithValveHardwareViewModel> TestCollectionResultWithValveHardwareViewModels
        {
            get
            {
                return this.testCollectionResultWithValveHardwareViewModels;
            }
        }
    }
}
