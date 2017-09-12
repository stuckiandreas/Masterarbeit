//-----------------------------------------------------------------------
// <copyright file="ViewModelEvents.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events
{
    using System;
    using EventArgs.Cursor;
    using EventArgs.Dialog;
    using EventArgs.Error;
    using EventArgs.UserFeedback;
    using VersionManagement.Events.EventArgs.DatabaseAccess;

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
        /// Occurs when the user rename a selection item.
        /// </summary>
        public event EventHandler RenameSelectionItemInDatabase;

        /// <summary>
        /// Occurs when [delete selection items in list].
        /// </summary>
        public event EventHandler DeleteSelectionItemsInList;

        public void OnHandleError(object sender, ErrorHandlerEventArgs errorEventArgs)
        {
            if (this.HandleError != null)
                this.HandleError(sender, errorEventArgs);
        }

        /// <summary>
        /// Called when the user should be inform over an event.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="userFeedbackEventArgs">The <see cref="UserFeedbackEventArgs"/> instance containing the event data.</param>
        public void OnUserFeedback(object sender, UserFeedbackEventArgs userFeedbackEventArgs)
        {
            if (this.UserFeedback != null)
                this.UserFeedback(sender, userFeedbackEventArgs);
        }

        /// <summary>
        /// Called when set or get an selection item or to show some information in about option.
        /// Also uses to define the database settings.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="dialogEventArgs">The <see cref="DialogEventArgs"/> instance containing the event data.</param>
        public void OnOpenDialog(object sender, DialogEventArgs dialogEventArgs)
        {
            if (this.OpenDialog != null)
                this.OpenDialog(sender, dialogEventArgs);
        }

        /// <summary>
        /// Called when in the background the data in the database changed and the user want to use GUI in the same time.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="cursorLoadingEventArgs">The <see cref="CursorLoadingEventArgs"/> instance containing the event data.</param>
        public void OnCursorLoading(object sender, CursorLoadingEventArgs cursorLoadingEventArgs)
        {
            if (this.CursorLoading != null)
                this.CursorLoading(sender, cursorLoadingEventArgs);
        }

        /// <summary>
        /// Called when the user want to change the database settings.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="databaseAccessEventArgs">The <see cref="DatabaseAccessEventArgs"/> instance containing the event data.</param>
        public void OnChangeDatabaseSettings(object sender, DatabaseAccessEventArgs databaseAccessEventArgs)
        {
            if (this.ChangeDatabaseSettings != null)
                this.ChangeDatabaseSettings(sender, databaseAccessEventArgs);
        }

        public void OnRenameSelectionItemInDatabase()
        {
            if (RenameSelectionItemInDatabase != null)
                RenameSelectionItemInDatabase(this, null);
        }

        public void OnDeleteSelectionItemsInList()
        {
            if (DeleteSelectionItemsInList != null)
                DeleteSelectionItemsInList(this, null);
        }
    }
}
