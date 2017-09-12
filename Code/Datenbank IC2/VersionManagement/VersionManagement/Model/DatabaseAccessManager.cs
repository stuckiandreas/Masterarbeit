//-----------------------------------------------------------------------
// <copyright file="DatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using System;
    using System.Data.SqlClient;
    using Database;
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

        /// <summary>
        /// Finalizes an instance of the <see cref="DatabaseAccessManager"/> class.
        /// </summary>
        ~DatabaseAccessManager()
        {
            this.databaseContext.Dispose();
        }

        /// <summary>
        /// Occurs when when the user change the database settings.
        /// </summary>
        public event EventHandler<DatabaseAccessEventArgs> DatabaseAccessEvent;

        public string DatabaseName
        {
            get
            {
                return this.databaseContext.Database.Connection.DataSource;
            }
        }

        /// <summary>
        /// Determines if a connection is possible to the database.
        /// </summary>
        /// <returns>
        ///   <c>true</c> if [is database available]; otherwise, <c>false</c>.
        /// </returns>
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

        /// <summary>
        /// Builds the database context. This contains the database and server name.
        /// </summary>
        /// <param name="databaseName">Name of the database.</param>
        /// <param name="serverName">Name of the server.</param>
        public void BuildDatabaseContext(string databaseName, string serverName)
        {
            string sqlConnectionString = this.GetSqlConnectionString(databaseName, serverName);
            this.databaseContext = new SoftwareVersionsDatabaseContext(sqlConnectionString);
        }

        /// <summary>
        /// Called when the connection with the database is available.
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="eventArgs">The <see cref="DatabaseAccessEventArgs"/> instance containing the event data.</param>
        protected void OnDatabaseAccessEvent(object sender, DatabaseAccessEventArgs eventArgs)
        {
            if (this.DatabaseAccessEvent != null)
                this.DatabaseAccessEvent(this, eventArgs);
        }

        /// <summary>
        /// Gets the SQL connection string.
        /// </summary>
        /// <param name="databaseName">Name of the database.</param>
        /// <param name="serverName">Name of the server.</param>
        /// <returns>SQL connection string</returns>
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
