//-----------------------------------------------------------------------
// <copyright file="StatusbarViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.StatusbarViewModels
{
    using Events;

    /// <summary>
    /// Status view model is shown at the bottom of the version management application. It shows the actually status of the database connection.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class StatusbarViewModel : BasisViewModel
    {
        /// <summary>
        /// Status of the application
        /// </summary>
        private string status;

        public StatusbarViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
        }

        public string Status
        {
            get
            {
                return this.status;
            }

            set
            {
                this.status = value;
                this.OnPropertyChanged("Status");
            }
        }
    }
}
