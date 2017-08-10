//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using ETIC2.Model.Application;

    public class TestErrorMessageViewModel
    {
        public TestErrorMessageViewModel(TestErrorMessage testErrorMessage)
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
