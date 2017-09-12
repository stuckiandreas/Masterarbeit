//-----------------------------------------------------------------------
// <copyright file="VersionManagementModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using Database;

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
            this.SelectionItemDatabaseManager = new SelectionItemDatabaseAccessManager();
            this.DatabaseConnectionSettingsDatabaseAccessManager = new DatabaseConnectionSettingsDatabaseAccessManager();
        }

        /// <summary>
        /// Gets the software versions database access manager.
        /// </summary>
        /// <value>
        /// The software versions database access manager.
        /// </value>
        public SoftwareVersionsDatabaseAccessManager SoftwareVersionsDatabaseAccessManager { get; }

        /// <summary>
        /// Gets the selection item database manager.
        /// </summary>
        /// <value>
        /// The selection item database manager.
        /// </value>
        public SelectionItemDatabaseAccessManager SelectionItemDatabaseManager { get; }
       
        /// <summary>
        /// Gets the database connection settings database access manager.
        /// </summary>
        /// <value>
        /// The database connection settings database access manager.
        /// </value>
        public DatabaseConnectionSettingsDatabaseAccessManager DatabaseConnectionSettingsDatabaseAccessManager { get; }
    }
}
