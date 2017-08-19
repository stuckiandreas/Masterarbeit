//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.FirmwareViewModels
{
    using ETIC2.Model.Application.FirmwareView;
    using Events;

    /// <summary>
    /// Forth Level of the Firmware Master Detail View. Shows the TestErrorMessage entrys.
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
