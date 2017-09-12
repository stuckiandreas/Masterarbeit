//-----------------------------------------------------------------------
// <copyright file="SelectedItemEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.SelectedItem
{
    /// <summary>
    /// Event which send a selected item. Practice example to update the detailViewModel
    /// </summary>
    /// <seealso cref="System.EventArgs" />
    public class SelectedItemEventArgs : System.EventArgs
    {
        public SelectedItemEventArgs(string item)
        {
            this.Item = item;
        }

        public string Item
        {
            get;
            set;
        }
    }
}
