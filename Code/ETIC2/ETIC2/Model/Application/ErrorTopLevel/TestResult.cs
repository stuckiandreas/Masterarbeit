//-----------------------------------------------------------------------
// <copyright file="TestResult.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.ErrorTopLevel
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class TestResult
    {
        public DateTime StartTime;

        public DateTime EndTime;

        public short TestVersion;

        public int TestCollectionResultId;

        public TestResult()
        {
        }
    }
}
