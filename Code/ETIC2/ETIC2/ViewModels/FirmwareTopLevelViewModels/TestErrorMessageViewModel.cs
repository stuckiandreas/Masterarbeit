//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using ETIC2.Model.Application;
    using Events;

    public class TestErrorMessageViewModel : BasisViewModel
    {
        public TestErrorMessageViewModel(ViewModelEvents viewModelEvents, TestErrorMessage testErrorMessage)
            : base(viewModelEvents)
        {
            this.Description = testErrorMessage.Description;
        }

        public string Description
        {
            get;
            set;
        }
    }
}
