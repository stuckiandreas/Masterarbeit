//-----------------------------------------------------------------------
// <copyright file="ETIC2Model.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.Model.Application
{
    /// <summary>
    /// Model which request the data from database for the view
    /// </summary>
    public class ETIC2Model
    {
        public ETIC2Model()
        {
            this.DatabaseConnectionSettingsDatabaseAccessManager = new DatabaseConnectionSettings();
            this.InitialStateFirmwareDatabaseAccessManager = new InitialStateFirmware();
            this.TestCollectionResultWithHardwareDatabaseAccessManager = new TestCollectionResultWithHardware();
            this.TestResultDatabaseAccessManager = new TestResult();
            this.TestErrorMessageDatabaseAccessManager = new TestErrorMessage();
        }

        public DatabaseConnectionSettings DatabaseConnectionSettingsDatabaseAccessManager { get; }

        public InitialStateFirmware InitialStateFirmwareDatabaseAccessManager { get; }

        public TestCollectionResultWithHardware TestCollectionResultWithHardwareDatabaseAccessManager { get; }

        public TestResult TestResultDatabaseAccessManager { get; }

        public TestErrorMessage TestErrorMessageDatabaseAccessManager { get; }

        /// <summary>
        /// Builds the database context. Neccesary to update the view with actual data from the database.
        /// </summary>
        public void BuildDatabaseContext()
        {
            string serverName = ETIC2.Properties.Settings.Default.ServerName;
            string databaseName = ETIC2.Properties.Settings.Default.DatabaseName;

            this.InitialStateFirmwareDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.TestCollectionResultWithHardwareDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.TestResultDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.TestErrorMessageDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.DatabaseConnectionSettingsDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
        }
    }
}
