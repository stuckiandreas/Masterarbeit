//-----------------------------------------------------------------------
// <copyright file="CursorLoadingEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.Cursor
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    /// <summary>
    /// Event which send a selected item. Practice example to update the detailViewModel
    /// </summary>
    /// <seealso cref="System.EventArgs" />
    public class CursorLoadingEventArgs : System.EventArgs
    {
        public CursorLoadingEventArgs(bool isCursorLoading)
        {
            this.IsCursorLoading = isCursorLoading;
        }

        public bool IsCursorLoading
        {
            get;
            private set;
        }
    }
}
