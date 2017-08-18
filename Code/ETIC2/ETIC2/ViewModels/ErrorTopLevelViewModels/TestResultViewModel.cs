//-----------------------------------------------------------------------
// <copyright file="TestResultViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ErrorTopLevelViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using Events;

    /// <summary>
    /// Second Level of the Error Master Detail View. Shows the TestResult entrys.
    /// </summary>
    public class TestResultViewModel : BasisViewModel
    {
        public TestResultViewModel(ViewModelEvents viewModelEvents, Model.Application.ErrorTopLevel.TestResult testResult, ObservableCollection<FirmwareWithTestHardwareViewModel> firmwareWithTestHardwareViewModelList)
            : base(viewModelEvents)
        {
            this.StartTime = testResult.StartTime;
            this.EndTime = testResult.EndTime;
            this.TestVersion = testResult.TestVersion;
            this.FirmwareWithTestHardwareViewModels = firmwareWithTestHardwareViewModelList;
        }

        public DateTime StartTime
        {
            get;
            set;
        }

        public DateTime EndTime
        {
            get;
            set;
        }

        public short TestVersion
        {
            get;
            set;
        }

        public ObservableCollection<FirmwareWithTestHardwareViewModel> FirmwareWithTestHardwareViewModels
        {
            get;
            private set;
        }
    }
}
