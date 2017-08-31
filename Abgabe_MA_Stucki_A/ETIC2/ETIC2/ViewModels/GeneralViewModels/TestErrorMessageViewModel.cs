//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.General
{
    using ETIC2.Model.Application.General;
    using Events;

    /// <summary>
    /// Forth Level of the Firmware and Hardware Master Detail View. Shows the TestErrorMessage entrys.
    /// </summary>
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
