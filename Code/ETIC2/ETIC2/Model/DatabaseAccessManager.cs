//-----------------------------------------------------------------------
// <copyright file="DatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Data.SqlClient;
    using EntityFramework;
    using Events.EventArgs.DatabaseAccess;

    /// <summary>
    /// Managed the access to the database
    /// </summary>
    public abstract class DatabaseAccessManager
    {
        /// <summary>
        /// The database context
        /// </summary>
        protected SoftwareVersionsDatabaseContext databaseContext;

        public DatabaseAccessManager()
        {
        }


        ~DatabaseAccessManager()
        {
            this.databaseContext.Dispose();
        }

        /// <summary>
        /// Occurs when the user change the database settings.
        /// </summary>
        public event EventHandler<DatabaseAccessEventArgs> DatabaseAccessEvent;

        public string DatabaseName
        {
            get
            {
                return this.databaseContext.Database.Connection.DataSource;
            }
        }

        public bool IsDatabaseAvailable()
        {
            try
            {
                this.databaseContext.Database.Connection.Open();
                this.databaseContext.Database.Connection.Close();
                this.OnDatabaseAccessEvent(this, new DatabaseAccessEventArgs(this.databaseContext.Database.Connection.DataSource, DatabaseAvailability.Available));
            }
            catch (Exception)
            {
                //No database...
                this.OnDatabaseAccessEvent(this, new DatabaseAccessEventArgs(this.databaseContext.Database.Connection.DataSource, DatabaseAvailability.NotAvailable));
                return false;
            }

            return true;
        }

        public void BuildDatabaseContext(string databaseName, string serverName)
        {
            string sqlConnectionString = this.GetSqlConnectionString(databaseName, serverName);
            this.databaseContext = new SoftwareVersionsDatabaseContext(sqlConnectionString);
        }

        protected void OnDatabaseAccessEvent(object sender, DatabaseAccessEventArgs eventArgs)
        {
            if (this.DatabaseAccessEvent != null)
                this.DatabaseAccessEvent(this, eventArgs);
        }

        private string GetSqlConnectionString(string databaseName, string serverName)
        {
            // Initialize the connection string builder for the
            // underlying provider.
            SqlConnectionStringBuilder sqlBuilder = new SqlConnectionStringBuilder();

            // Set the properties for the data source.
            sqlBuilder.DataSource = serverName;
            sqlBuilder.InitialCatalog = databaseName;
            sqlBuilder.IntegratedSecurity = true;
            sqlBuilder.MultipleActiveResultSets = true;

            // Build the SqlConnection connection string.
            return sqlBuilder.ToString();
        }
    }
}
