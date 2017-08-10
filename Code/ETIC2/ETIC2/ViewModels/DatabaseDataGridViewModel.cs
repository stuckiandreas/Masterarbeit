//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System.Collections.Generic;
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
        private List<InitialStateFirmwareViewModel> initialStateFirmwareViewModels;

        /// <summary>
        /// List of all test test collection result entries in the grid view.
        /// </summary>
        private List<TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModels;

        /// <summary>
        /// List of all test test result entries in the grid view.
        /// </summary>
        private List<TestResultViewModel> testResultVieModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public DatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.initialStateFirmwareViewModels = new List<InitialStateFirmwareViewModel>();
            this.testCollectionResultWithValveHardwareViewModels = new List<TestCollectionResultWithValveHardwareViewModel>();
            this.testResultVieModels = new List<TestResultViewModel>();
        }

        public List<InitialStateFirmwareViewModel> InitialStateFirmwareViewModels
        {
            get
            {
                return this.initialStateFirmwareViewModels;
            }
        }

        public List<TestCollectionResultWithValveHardwareViewModel> TestCollectionResultWithValveHardwareViewModels
        {
            get
            {
                return this.testCollectionResultWithValveHardwareViewModels;
            }
        }

        public List<TestResultViewModel> TestResultViewModels
        {
            get
            {
                return this.testResultVieModels;
            }
        }
    }
}
