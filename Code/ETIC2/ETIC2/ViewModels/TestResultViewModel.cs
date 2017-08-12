//-----------------------------------------------------------------------
// <copyright file="TestResultViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using ETIC2.Model.Application;
    using Events;

    /// <summary>
    /// Third Level of the Master Detail View. Listet the ResultTest entrys.
    /// </summary>
    public class TestResultViewModel : BasisViewModel
    {
        public TestResultViewModel(ViewModelEvents viewModelEvents, TestResult testResult, ObservableCollection<TestErrorMessageViewModel> testErrorMessageViewModelList)
            : base(viewModelEvents)
        {
            this.StartTime = testResult.StartTime;
            this.EndTime = testResult.EndTime;
            this.Result = testResult.Result;
            this.TestVersion = testResult.TestVersion;
            this.TestErrorMessageViewModels = testErrorMessageViewModelList;
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

        public string Result
        {
            get;
            set;
        }

        public short TestVersion
        {
            get;
            set;
        }

        public ObservableCollection<TestErrorMessageViewModel> TestErrorMessageViewModels
        {
            get;
            private set;
        }
    }
}
