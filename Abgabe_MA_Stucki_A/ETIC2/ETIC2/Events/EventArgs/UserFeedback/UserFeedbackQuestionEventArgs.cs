//-----------------------------------------------------------------------
// <copyright file="UserFeedbackQuestionEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.UserFeedback
{
    using System.Windows;

    /// <summary>
    /// Event arguments with a question message.
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.UserFeedback.UserFeedbackEventArgs" />
    public class UserFeedbackQuestionEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackQuestionEventArgs(string questionMessage)
        {
            this.Caption = "Confirmation";
            this.Message = questionMessage;
            this.Image = MessageBoxImage.Question;
            this.Button = MessageBoxButton.YesNo;
        }
    }
}
