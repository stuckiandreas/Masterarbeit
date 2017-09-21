//-----------------------------------------------------------------------
// <copyright file="UserFeedbackInfoEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.UserFeedback
{
    using System.Windows;

    /// <summary>
    /// Event arguments for the user information. Including a information Messages. 
    /// </summary>
    /// <seealso cref="ETIC2.Events.EventArgs.UserFeedback.UserFeedbackEventArgs" />
    public class UserFeedbackInfoEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackInfoEventArgs(string infoMessage)
        {
            this.Caption = "Information";
            this.Message = infoMessage;
            this.Image = MessageBoxImage.Information;
            this.Button = MessageBoxButton.OK;
        }
    }
}
