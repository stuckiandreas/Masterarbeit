//-----------------------------------------------------------------------
// <copyright file="DeleteItemEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.BuglistItem
{
    using ViewModels.BuglistViewModels;

    /// <summary>
    /// Event after the user push the delete button
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.SoftwareItem.ItemEventArgs" />
    public class DeleteItemEventArgs : ItemEventArgs
    {
        public DeleteItemEventArgs(DatabaseItemViewModel databaseItemViewModel)
            : base(databaseItemViewModel)
        {
        }
    }
}
