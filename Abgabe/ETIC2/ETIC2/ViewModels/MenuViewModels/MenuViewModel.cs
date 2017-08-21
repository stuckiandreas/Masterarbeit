//-----------------------------------------------------------------------
// <copyright file="MenuViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.MenuViewModels
{
    using System;
    using System.IO;
    using System.Windows.Input;
    using AboutDialogViewModels;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Dialog;
    using Events.EventArgs.Error;
    using Events.EventArgs.UserFeedback;
    using ExecuteDialogViewModels;
    using Model.Application;

    /// <summary>
    /// Menu view model is shown at the top of the etic2 program. 
    /// At the moment the menu bar has the option of the about dialog which shows the actual version of the application and database.
    /// Further to set the connection settings for the database. Also to close the application over the menu view.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.BasisViewModel" />
    public class MenuViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

        public MenuViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.etic2Model = etic2Model;

            this.AboutCommand = new ActionCommand(this.OnAboutCommand);
            this.ExitCommand = new ActionCommand(this.OnExitCommand);
            this.SettingsCommand = new ActionCommand(this.OnSettingsCommand);
            this.ImprovementsCommand = new ActionCommand(this.OnImprovementsCommand);
        }

        /// <summary>
        /// Gets the about view which shows the version and company information.
        /// </summary>
        /// <value>
        /// The about command.
        /// </value>
        public ICommand AboutCommand
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the setting dialog, in which the database connection can edited.
        /// </summary>
        /// <value>
        /// The settings command.
        /// </value>
        public ICommand SettingsCommand
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets the view which shows the improvement list, in which all user can describes their improvements.
        /// </summary>
        /// <value>
        /// The improvements command.
        /// </value>
        public ICommand ImprovementsCommand
        {
            get;
            private set;
        }

        public ICommand ExitCommand
        {
            get;
            private set;
        }

        private void OnAboutCommand(object parameter)
        {
            try
            {
                this.viewModelEvents.OnOpenDialog(this, new DialogEventArgs(new AboutDialogViewModel(this.viewModelEvents, this.etic2Model), "About", 250, 350));
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnSettingsCommand(object parameter)
        {
            try
            {
                this.viewModelEvents.OnOpenDialog(this, new DialogEventArgs(new SaveSettingsExecuteDialogViewModel(this.viewModelEvents, "Save", this.etic2Model, "Database Name: ", "Server Name: ", Properties.Settings.Default.DatabaseName, Properties.Settings.Default.ServerName), "Settings", 120, 350));
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnImprovementsCommand(object parameter)
        {
            try
            {
                FileInfo fi = new FileInfo(@"\\Hq.vat\chvat\metpd\Development\Software\ETIC2\Verbesserungsvorschläge\Verbesserungsvorschläge_ETIC2.xlsx");
                if (fi.Exists)
                {
                    System.Diagnostics.Process.Start(@"\\Hq.vat\chvat\metpd\Development\Software\ETIC2\Verbesserungsvorschläge\Verbesserungsvorschläge_ETIC2.xlsx");
                }
                else
                {
                    //file doesn't exist
                    this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("improvements list file doesn't exist"));
                }
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnExitCommand(object parameter)
        {
            try
            {
                //Close database connection

                App.Current.Shutdown();
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }
    }
}
