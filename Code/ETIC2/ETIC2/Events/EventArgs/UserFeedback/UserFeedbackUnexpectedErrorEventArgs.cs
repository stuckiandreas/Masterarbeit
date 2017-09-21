//-----------------------------------------------------------------------
// <copyright file="UserFeedbackUnexpectedErrorEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.UserFeedback
{
    using System;
    using System.Windows;

    /// <summary>
    /// User receives an unexpected error event.
    /// </summary>
    /// <seealso cref="ETIC2.Events.EventArgs.UserFeedback.UserFeedbackEventArgs" />
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
