﻿//-----------------------------------------------------------------------
// <copyright file="AddNewItemEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.SoftwareItem
{
    public enum NewItemType
    {
        Version,
        Revision
    }

    /// <summary>
    /// Event after the user push the new button
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.SoftwareItem.ItemEventArgs" />
    public class AddNewItemEventArgs : ItemEventArgs
    {
        public NewItemType NewItemType;

        public AddNewItemEventArgs(NewItemType newItemType)
        {
            this.NewItemType = newItemType;
        }
    }
}