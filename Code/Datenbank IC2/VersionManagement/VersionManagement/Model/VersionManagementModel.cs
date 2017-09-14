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
            this.SoftwareVersionsDatabaseAccessManager = new SoftwareVersionsDatabaseAccessManager();
            this.SelectionItemDatabaseAccessManager = new SelectionItemDatabaseAccessManager();
            this.DatabaseConnectionSettingsDatabaseAccessManager = new DatabaseConnectionSettingsDatabaseAccessManager();
        }

        public SoftwareVersionsDatabaseAccessManager SoftwareVersionsDatabaseAccessManager { get; }

        public SelectionItemDatabaseAccessManager SelectionItemDatabaseAccessManager { get; }
       
        public DatabaseConnectionSettingsDatabaseAccessManager DatabaseConnectionSettingsDatabaseAccessManager { get; }

        /// <summary>
        /// Builds the database context. Neccesary to update the view with actual data from the database.
        /// </summary>
        public void BuildDatabaseContext()
        {
            string serverName = Properties.Settings.Default.ServerName;
            string databaseName = Properties.Settings.Default.DatabaseName;

            this.SoftwareVersionsDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.SelectionItemDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.DatabaseConnectionSettingsDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
        }
    }
}
