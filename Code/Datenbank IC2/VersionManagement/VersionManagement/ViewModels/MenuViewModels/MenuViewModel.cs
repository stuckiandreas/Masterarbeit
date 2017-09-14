//-----------------------------------------------------------------------
// <copyright file="MenuViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.MenuViewModels
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
    using Model;

    /// <summary>
    /// Menu view model is shown at the top of the version management program. At the moment the menu bar has the option of the about dialog which shows the actual version.
    /// Further to set the connection settings for the database. Also to close the application over the menu view.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class MenuViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

        public MenuViewModel(ViewModelEvents viewModelEvents, VersionManagementModel versionManagementModel)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.versionManagementModel = versionManagementModel;

            this.AboutCommand = new ActionCommand(this.OnAboutCommand);
            this.ExitCommand = new ActionCommand(this.OnExitCommand);
            this.SettingsCommand = new ActionCommand(this.OnSettingsCommand);
            this.ImprovementsCommand = new ActionCommand(this.OnImprovementsCommand);
            this.ReleaseNoteCommand = new ActionCommand(this.OnReleaseNoteCommand);
        }

        public ICommand AboutCommand
        {
            get;
            private set;
        }

        public ICommand SettingsCommand
        {
            get;
            private set;
        }

        public ICommand ExitCommand
        {
            get;
            private set;
        }

        public ICommand ImprovementsCommand
        {
            get;
            private set;
        }

        public ICommand ReleaseNoteCommand
        {
            get;
            private set;
        }

        private void OnAboutCommand(object parameter)
        {
            try
            {
                this.viewModelEvents.OnOpenDialog(this, new DialogEventArgs(new AboutDialogViewModel(this.viewModelEvents, this.versionManagementModel), "About", 250, 350));
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
                this.viewModelEvents.OnOpenDialog(this, new DialogEventArgs(new SaveSettingsExecuteDialogViewModel(this.viewModelEvents, "Save", this.versionManagementModel, "Database Name: ", "Server Name: ", Properties.Settings.Default.DatabaseName, Properties.Settings.Default.ServerName), "Settings", 120, 350));
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// OnImprovementsCommand open the exel list with the improvements
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnImprovementsCommand(object parameter)
        {
            try
            {
                FileInfo fi = new FileInfo(@"\\Hq.vat\chvat\metpd\Development\Software\Firmware Database\Verbesserungsvorschläge\Verbesserungsvorschläge_Firmware_Database.xlsx");
                if (fi.Exists)
                {
                    System.Diagnostics.Process.Start(@"\\Hq.vat\chvat\metpd\Development\Software\Firmware Database\Verbesserungsvorschläge\Verbesserungsvorschläge_Firmware_Database.xlsx");
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

        /// <summary>
        /// OnReleaseNoteCommand open the word document with the release notes
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnReleaseNoteCommand(object parameter)
        {
            try
            {
                FileInfo fi = new FileInfo(@"\\Hq.vat\chvat\metpd\Development\Software\Firmware Database\Dokumente\Release Note.docx");
                if (fi.Exists)
                {
                    System.Diagnostics.Process.Start(@"\\Hq.vat\chvat\metpd\Development\Software\Firmware Database\Dokumente\Release Note.docx");
                }
                else
                {
                    //file doesn't exist
                    this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("release note document doesn't exist"));
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
                App.Current.Shutdown();
            }
            catch (Exception ex)
            {
                this.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }
    }
}
