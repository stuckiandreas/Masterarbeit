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
            this.InitialStateValveDatabaseAccessManager = new InitialStateValveDatabaseAccessManager();
            this.TestCollectionResultDatabaseAccessManager = new TestCollectionResultDatabaseAccessManager();
            this.TestResultDatabaseAccessManager = new TestResultDatabaseAccessManager();
        }

        public InitialStateValveDatabaseAccessManager InitialStateValveDatabaseAccessManager { get; }

        public TestCollectionResultDatabaseAccessManager TestCollectionResultDatabaseAccessManager { get; }

        public TestResultDatabaseAccessManager TestResultDatabaseAccessManager { get; }
    }
}
