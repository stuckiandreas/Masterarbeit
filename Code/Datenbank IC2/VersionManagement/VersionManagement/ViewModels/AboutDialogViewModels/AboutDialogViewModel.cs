//-----------------------------------------------------------------------
// <copyright file="AboutDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.AboutDialogViewModels
{
    using System;
    using System.Reflection;
    using System.Windows;
    using System.Windows.Input;
    using Events;
    using Events.EventArgs.Error;
    using Model;

    /// <summary>
    /// Window which enabled to edit the database connection. Also open a exel list for edit the improvements list. 
    /// Also a view with the application version and option to close the apllication.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class AboutDialogViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public AboutDialogViewModel(
            ViewModelEvents viewModelEvents,
            VersionManagementModel versionManagementModel)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;

            Assembly executingAssembly = Assembly.GetExecutingAssembly();
            AssemblyCompanyAttribute company = (AssemblyCompanyAttribute)executingAssembly.GetCustomAttributes(typeof(AssemblyCompanyAttribute), false)[0];
            this.FirmwareDatabaseVersion = this.TranslateVersionNumberToString(Properties.Settings.Default.FirmwareDatabaseVersion);
            this.DatabaseVersion = this.TranslateVersionNumberToString(versionManagementModel.DatabaseConnectionSettings.GetDatabaseVersion());

            this.Product = "Firmware Database";
            this.Company = company.Company;
        }

        public string Product
        {
            get;
            private set;
        }

        public string FirmwareDatabaseVersion
        {
            get;
            private set;
        }

        public string DatabaseVersion
        {
            get;
            private set;
        }

        public string Company
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets or sets the execute command.
        /// </summary>
        /// <value>
        /// The execute command.
        /// </value>
        public ICommand CloseCommand
        {
            get;
            set;
        }

        /// <summary>
        /// Called when [close command].
        /// </summary>
        /// <param name="parameter">The parameter.</param>
        private void OnCloseCommand(object parameter)
        {
            try
            {
                Application.Current.Windows[1].Close();
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Translates the firmware database version to string with points beetwen the numbers.
        /// </summary>
        /// <param name="firmwareDatabaseNumber">The firmware database number.</param>
        /// <returns>Gets the database version string with point notation</returns>
        private string TranslateVersionNumberToString(int firmwareDatabaseNumber)
        {
            string firmwareDatabaseString = firmwareDatabaseNumber.ToString();
            int firmwareDtabaseStringLength = firmwareDatabaseString.Length;
            string firmwareDatabaseStringWithPoints = string.Empty;

            //edit points into the firmware database versin string
            for (int indexFirmwareDatabaseIndex = 0; indexFirmwareDatabaseIndex < firmwareDtabaseStringLength; indexFirmwareDatabaseIndex++)
            {
                firmwareDatabaseStringWithPoints = firmwareDatabaseStringWithPoints + firmwareDatabaseString[indexFirmwareDatabaseIndex];
                if (indexFirmwareDatabaseIndex != (firmwareDtabaseStringLength - 1))
                    firmwareDatabaseStringWithPoints = firmwareDatabaseStringWithPoints + ".";
            }

            return firmwareDatabaseStringWithPoints;
        }
    }
}
