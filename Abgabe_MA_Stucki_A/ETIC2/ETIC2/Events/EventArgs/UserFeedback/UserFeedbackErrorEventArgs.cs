//-----------------------------------------------------------------------
// <copyright file="UserFeedbackErrorEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.UserFeedback
{
    using System.Windows;

    /// <summary>
    /// Message if the user operated wrong
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.UserFeedback.UserFeedbackEventArgs" />
    public class UserFeedbackErrorEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackErrorEventArgs(string errorMessage)
        {
            this.Caption = "Error";
            this.Message = errorMessage;
            this.Image = MessageBoxImage.Error;
            this.Button = MessageBoxButton.OK;
        }
    }
}
