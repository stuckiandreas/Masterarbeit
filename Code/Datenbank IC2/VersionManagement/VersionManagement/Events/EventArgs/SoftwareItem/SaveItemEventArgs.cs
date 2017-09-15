﻿//-----------------------------------------------------------------------
// <copyright file="SaveItemEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.SoftwareItem
{
    using VersionManagement.ViewModels;

    /// <summary>
    /// Event after the user push the save button
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.SoftwareItem.ItemEventArgs" />
    public class SaveItemEventArgs : ItemEventArgs
    {
        public SaveItemEventArgs(DatabaseItemViewModel databaseItemViewModel)
            : base(databaseItemViewModel)
        {
        }
    }
}