//-----------------------------------------------------------------------
// <copyright file="DatabaseAccessEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.Events.EventArgs.DatabaseAccess
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
        public DatabaseAccessEventArgs(string databaseName, DatabaseAvailability databaseAvailability)
        {
            DatabaseAvailability = databaseAvailability;
            this.DatabaseName = databaseName;
        }

        public DatabaseAvailability DatabaseAvailability
        {
            get;
        }

        public string DatabaseName
        {
            get;
        }
    }
}
