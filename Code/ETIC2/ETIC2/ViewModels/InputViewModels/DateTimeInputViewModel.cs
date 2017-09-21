//-----------------------------------------------------------------------
// <copyright file="DateTimeInputViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.InputViewModels
{
    using System;
    using ETIC2.Events;

    /// <summary>
    /// View model in which a label and a date input can be defined.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.InputViewModels.InputBaseViewModel" />
    public class DateTimeInputViewModel : InputBaseViewModel
    {
        /// <summary>
        /// Date time input
        /// </summary>
        private DateTime? dateTimeInput;

        public DateTimeInputViewModel(ViewModelEvents viewModelEvents, string label, DateTime? dateTimeInput)
            : base(viewModelEvents, label)
        {
            this.DateTimeInput = dateTimeInput;
        }

        public DateTime? DateTimeInput
        {
            get
            {
                return this.dateTimeInput;
            }

            set
            {
                this.dateTimeInput = value;
                this.OnPropertyChanged("DateTimeInput");
            }
        }
    }
}
