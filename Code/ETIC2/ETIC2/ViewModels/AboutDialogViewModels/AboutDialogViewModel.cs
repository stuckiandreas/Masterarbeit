//-----------------------------------------------------------------------
// <copyright file="AboutDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------


namespace ETIC2.ViewModels.AboutDialogViewModels
{
    using System;
    using System.Reflection;
    using System.Windows;
    using System.Windows.Input;
    using ETIC2.Events;
    using Events.EventArgs.Error;
    using Model.Application;

    /// <summary>
    /// Window which enabled to edit the database connection. Also open a exel list for edit the improvements list. Also a view with the application version and option to close the apllication.
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.BasisViewModel" />
    public class AboutDialogViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public AboutDialogViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;

            Assembly executingAssembly = Assembly.GetExecutingAssembly();
            AssemblyCompanyAttribute company = (AssemblyCompanyAttribute)executingAssembly.GetCustomAttributes(typeof(AssemblyCompanyAttribute), false)[0];
            this.ETIC2Version = this.TranslateVersionNumberToString(Properties.Settings.Default.ETIC2Version);
            this.DatabaseVersion = this.TranslateVersionNumberToString(etic2Model.DatabaseConnectionSettingsDatabaseAccessManager.GetDatabaseVersion());

            this.Product = "ETIC2";
            this.Company = company.Company;
        }

        public string Product
        {
            get;
            private set;
        }

        public string ETIC2Version
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

        public ICommand CloseCommand
        {
            get;
            set;
        }

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
        /// <returns>database string with point notation</returns>
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
