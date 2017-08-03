//-----------------------------------------------------------------------
// <copyright file="CursorLoadingEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.Events.EventArgs.Cursor
{
    /// <summary>
    /// Event is using, when actual data is reading from database
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
