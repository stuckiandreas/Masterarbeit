//-----------------------------------------------------------------------
// <copyright file="TestResult.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class TestResult
    {
        public int Id;

        public DateTime StartTime;

        public DateTime EndTime;

        public string Result;

        public short TestVersion;

        public TestResult()
        {
        }
    }
}
