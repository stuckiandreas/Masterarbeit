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
            this.DatabaseConnectionSettings = new DatabaseConnectionSettings();
            this.InitialStateFirmware = new InitialStateFirmware();
            this.TestCollectionResult = new TestCollectionResult();
            this.TestResult = new TestResult();
            this.TestErrorMessage = new TestErrorMessage();
            this.ValveHardware = new ValveHardware();
        }

        public DatabaseConnectionSettings DatabaseConnectionSettings { get; }

        public InitialStateFirmware InitialStateFirmware { get; }

        public TestCollectionResult TestCollectionResult { get; }

        public TestResult TestResult { get; }

        public TestErrorMessage TestErrorMessage { get; }

        public ValveHardware ValveHardware { get; }

        /// <summary>
        /// Builds the database context. Neccesary to update the view with actual data from the database.
        /// </summary>
        public void BuildDatabaseContext()
        {
            string serverName = ETIC2.Properties.Settings.Default.ServerName;
            string databaseName = ETIC2.Properties.Settings.Default.DatabaseName;

            this.InitialStateFirmware.BuildDatabaseContext(databaseName, serverName);
            this.TestCollectionResult.BuildDatabaseContext(databaseName, serverName);
            this.TestResult.BuildDatabaseContext(databaseName, serverName);
            this.TestErrorMessage.BuildDatabaseContext(databaseName, serverName);
            this.ValveHardware.BuildDatabaseContext(databaseName, serverName);
            this.DatabaseConnectionSettings.BuildDatabaseContext(databaseName, serverName);
        }
    }
}
