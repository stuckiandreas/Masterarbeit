//-----------------------------------------------------------------------
// <copyright file="VersionManagementModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    /// <summary>
    /// Model which handles the save option of a software item in the database.
    /// Also the request from the GUI to show the software item which are saved in the database.
    /// </summary>
    public class VersionManagementModel
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="VersionManagementModel"/> class.
        /// </summary>
        public VersionManagementModel()
        {
            this.SoftwareVersionsItem = new SoftwareVersionsItem();
            this.SelectionItem = new SelectionItem();
            this.DatabaseConnectionSettings = new DatabaseConnectionSettings();
            this.TestResult = new TestResult();
        }

        public SoftwareVersionsItem SoftwareVersionsItem { get; }

        public SelectionItem SelectionItem { get; }
       
        public DatabaseConnectionSettings DatabaseConnectionSettings { get; }

        public TestResult TestResult { get; }

        /// <summary>
        /// Builds the database context. Neccesary to update the view with actual data from the database.
        /// </summary>
        public void BuildDatabaseContext()
        {
            string serverName = Properties.Settings.Default.ServerName;
            string databaseName = Properties.Settings.Default.DatabaseName;

            this.SoftwareVersionsItem.BuildDatabaseContext(databaseName, serverName);
            this.SelectionItem.BuildDatabaseContext(databaseName, serverName);
            this.TestResult.BuildDatabaseContext(databaseName, serverName);
            this.DatabaseConnectionSettings.BuildDatabaseContext(databaseName, serverName);
        }
    }
}
