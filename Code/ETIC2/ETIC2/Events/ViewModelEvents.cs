//-----------------------------------------------------------------------
// <copyright file="ViewModelEvents.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events
{
    using System;
    using EventArgs.Cursor;
    using EventArgs.DatabaseAccess;
    using EventArgs.Dialog;
    using EventArgs.Error;
    using EventArgs.UserFeedback;

    /// <summary>
    /// Helper class to encapsulate events that should be available in all ViewModels.
    /// Because always the same instance of events has to be used, the reference of this helper class must be provided each time a ViewModel is instanced.
    /// The helper class is needed because events may not be used as a parameter for reference assignment.
    /// </summary>
    public class ViewModelEvents
    {
        /// <summary>
        /// Occurs when an error happens.
        /// </summary>
        public event EventHandler<ErrorHandlerEventArgs> HandleError;

        /// <summary>
        /// Occurs when the user should be inform over an event.
        /// </summary>
        public event EventHandler<UserFeedbackEventArgs> UserFeedback;

        /// <summary>
        /// Occurs when set or get an selection item or to show some information in about option.
        /// Also uses to define the database settings.
        /// </summary>
        public event EventHandler<DialogEventArgs> OpenDialog;

        /// <summary>
        /// Occurs when in the background the data in the database changed and the user want to use GUI in the same time.
        /// </summary>
        public event EventHandler<CursorLoadingEventArgs> CursorLoading;

        /// <summary>
        /// Occurs when the user want to change the database settings.
        /// </summary>
        public event EventHandler<DatabaseAccessEventArgs> ChangeDatabaseSettings;
  
        /// <summary>
        /// Occurs when [delete selection items in list].
        /// </summary>
        public event EventHandler DeleteSelectionItemsInList;

        public void OnHandleError(object sender, ErrorHandlerEventArgs errorEventArgs)
        {
            if (this.HandleError != null)
                this.HandleError(sender, errorEventArgs);
        }

        public void OnUserFeedback(object sender, UserFeedbackEventArgs userFeedbackEventArgs)
        {
            if (this.UserFeedback != null)
                this.UserFeedback(sender, userFeedbackEventArgs);
        }

        public void OnOpenDialog(object sender, DialogEventArgs dialogEventArgs)
        {
            if (this.OpenDialog != null)
                this.OpenDialog(sender, dialogEventArgs);
        }

        public void OnCursorLoading(object sender, CursorLoadingEventArgs cursorLoadingEventArgs)
        {
            if (this.CursorLoading != null)
                this.CursorLoading(sender, cursorLoadingEventArgs);
        }

        public void OnChangeDatabaseSettings(object sender, DatabaseAccessEventArgs databaseAccessEventArgs)
        {
            if (this.ChangeDatabaseSettings != null)
                this.ChangeDatabaseSettings(sender, databaseAccessEventArgs);
        }

        public void OnDeleteSelectionItemsInList()
        {
            if (DeleteSelectionItemsInList != null)
                DeleteSelectionItemsInList(this, null);
        }
    }
}
