//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ErrorTopLevelViewModels
{
    using System.Collections.ObjectModel;
    using ETIC2.Events;

    /// <summary>
    /// First Level of the Error Master Detail View. Shows the TestErrorMessage entrys.
    /// </summary>
    public class TestErrorMessageViewModel : BasisViewModel
    {
        public TestErrorMessageViewModel(ViewModelEvents viewModelEvents, Model.Application.ErrorTopLevel.TestErrorMessage testErrorMessage, ObservableCollection<TestResultViewModel> testResultViewModelList)
            : base(viewModelEvents)
        {
            this.Description = testErrorMessage.Description;
            this.TestResultViewModels = testResultViewModelList;
        }

        public string Description
        {
            get;
            set;
        }

        public ObservableCollection<TestResultViewModel> TestResultViewModels
        {
            get;
            private set;
        }
    }
}
