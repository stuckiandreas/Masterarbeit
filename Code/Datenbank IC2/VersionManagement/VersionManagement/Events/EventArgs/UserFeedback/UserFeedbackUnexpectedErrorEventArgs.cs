//-----------------------------------------------------------------------
// <copyright file="UserFeedbackUnexpectedErrorEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.UserFeedback
{
    using System;
    using System.Windows;

    /// <summary>
    /// User receives an unexpected error event.
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.UserFeedback.UserFeedbackEventArgs" />
    public class UserFeedbackUnexpectedErrorEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackUnexpectedErrorEventArgs(Exception exception)
        {
            this.Caption = "Unexpected Error";
            this.Message = exception.ToString();
            this.Image = MessageBoxImage.Error;
            this.Button = MessageBoxButton.OK;
        }
    }
}
