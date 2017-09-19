//-----------------------------------------------------------------------
// <copyright file="BuglistViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using ETIC2.Events;

    /// <summary>
    /// Included detailInformation of a single buglist item and all database items in the databaseDataGrid view
    /// Only references to the Model are allowed. References to Views are not allowed
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.BasisViewModel" />
    public class BuglistViewModel : BasisViewModel
    {
        public BuglistViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
        }
    }
}
