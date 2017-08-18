//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridViewModel2.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using ETIC2.Events;

    public class DatabaseDataGridViewModel2 : BasisViewModel
    {
        private ViewModelEvents viewModelEvents;

        public DatabaseDataGridViewModel2(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
        }
    }
}