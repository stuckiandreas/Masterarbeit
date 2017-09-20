//-----------------------------------------------------------------------
// <copyright file="ItemEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.BuglistItem
{
    using ViewModels.BuglistViewModels;

    /// <summary>
    /// Base event class for all button event sub classes
    /// </summary>
    /// <seealso cref="System.EventArgs" />
    public abstract class ItemEventArgs : System.EventArgs
    {
        public ItemEventArgs()
        {
        }

        public ItemEventArgs(DatabaseItemViewModel databaseItemViewModel)
        {
            DatabaseItemViewModel = databaseItemViewModel;
        }

        public DatabaseItemViewModel DatabaseItemViewModel
        {
            get;
            set;
        }
    }
}
