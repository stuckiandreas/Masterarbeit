//-----------------------------------------------------------------------
// <copyright file="SaveSettingsExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ExecuteDialogViewModels
{
    using Events;
    using Events.EventArgs.DatabaseAccess;
    using Model;
    using Model.Application;

    /// <summary>
    /// View model which has two labels with inherent text input. One is for the server name and the other for database name.
    /// After the save button is push, the database try to connect with this specific information.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.ExecuteDialogViewModels.ExecuteDialogViewModel" />
    public class SaveSettingsExecuteDialogViewModel : SaveExecuteDoubleDialogViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

        public SaveSettingsExecuteDialogViewModel(
            ViewModelEvents viewModelEvents,
            string buttonText,
            ETIC2Model etic2Model,
            string label1,
            string label2,
            string textInput1,
            string textInput2)
            : base(viewModelEvents, buttonText, etic2Model, label1, label2, textInput1, textInput2)
        {
            this.viewModelEvents = viewModelEvents;
            this.etic2Model = etic2Model;
        }

        /// <summary>
        /// Save the inputs from the settings dialog window
        /// </summary>
        public override void Execute()
        {
            this.etic2Model.InitialStateFirmwareDatabaseAccessManager.BuildDatabaseContext(TextInputViewModel1.TextInput, TextInputViewModel2.TextInput);
            this.etic2Model.TestCollectionResultWithHardwareDatabaseAccessManager.BuildDatabaseContext(TextInputViewModel1.TextInput, TextInputViewModel2.TextInput);
            this.etic2Model.TestResultDatabaseAccessManager.BuildDatabaseContext(TextInputViewModel1.TextInput, TextInputViewModel2.TextInput);
            this.etic2Model.TestErrorMessageDatabaseAccessManager.BuildDatabaseContext(TextInputViewModel1.TextInput, TextInputViewModel2.TextInput);

            Properties.Settings.Default.DatabaseName = TextInputViewModel1.TextInput;
            Properties.Settings.Default.ServerName = TextInputViewModel2.TextInput;
            Properties.Settings.Default.Save();

            if (this.etic2Model.InitialStateFirmwareDatabaseAccessManager.IsDatabaseAvailable())
                this.viewModelEvents.OnChangeDatabaseSettings(this, new DatabaseAccessEventArgs(Properties.Settings.Default.DatabaseName, DatabaseAvailability.Available));
        }
    }
}
