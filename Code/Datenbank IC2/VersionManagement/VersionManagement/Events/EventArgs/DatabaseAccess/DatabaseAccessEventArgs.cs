//-----------------------------------------------------------------------
// <copyright file="DatabaseAccessEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Events.EventArgs.DatabaseAccess
{
    /// <summary>
    /// Enumeration for the database available state.
    /// </summary>
    public enum DatabaseAvailability
    {
        Available,
        NotAvailable
    }

    /// <summary>
    /// Event argument for the database access
    /// </summary>
    /// <seealso cref="System.EventArgs" />
    public class DatabaseAccessEventArgs : System.EventArgs
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="DatabaseAccessEventArgs"/> class.
        /// </summary>
        /// <param name="databaseName">Name of the database.</param>
        /// <param name="databaseAvailability">The database availability.</param>
        public DatabaseAccessEventArgs(string databaseName, DatabaseAvailability databaseAvailability)
        {
            DatabaseAvailability = databaseAvailability;
            this.DatabaseName = databaseName;
        }

        /// <summary>
        /// Gets the database availability.
        /// </summary>
        /// <value>
        /// The database availability.
        /// </value>
        public DatabaseAvailability DatabaseAvailability
        {
            get;
        }

        /// <summary>
        /// Gets the name of the database.
        /// </summary>
        /// <value>
        /// The name of the database.
        /// </value>
        public string DatabaseName
        {
            get;
        }
    }
}
