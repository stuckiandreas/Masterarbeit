//-----------------------------------------------------------------------
// <copyright file="ETIC2Model.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.Model.Application
{
    /// <summary>
    /// Model which request the data from database for the views
    /// </summary>
    public class ETIC2Model
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="ETIC2Model"/> class.
        /// </summary>
        public ETIC2Model()
        {
            this.InitialStateValveDatabaseAccessManager = new InitialStateValveDatabaseAccessManager();
            this.TestCollectionResultDatabaseAccessManager = new TestCollectionResultDatabaseAccessManager();
            this.TestResultDatabaseAccessManager = new TestResultDatabaseAccessManager();
        }

        /// <summary>
        /// Gets the initial state valve database access manager.
        /// </summary>
        /// <value>
        /// The initial state valve database access manager.
        /// </value>
        public InitialStateValveDatabaseAccessManager InitialStateValveDatabaseAccessManager { get; }

        /// <summary>
        /// Gets the test collection result database access manager.
        /// </summary>
        /// <value>
        /// The test collection result database access manager.
        /// </value>
        public TestCollectionResultDatabaseAccessManager TestCollectionResultDatabaseAccessManager { get; }

        /// <summary>
        /// Gets the test result database access manager.
        /// </summary>
        /// <value>
        /// The test result database access manager.
        /// </value>
        public TestResultDatabaseAccessManager TestResultDatabaseAccessManager { get; }
    }
}
