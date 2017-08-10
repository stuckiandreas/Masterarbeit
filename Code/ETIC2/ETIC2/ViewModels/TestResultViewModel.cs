﻿//-----------------------------------------------------------------------
// <copyright file="TestResultViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using ETIC2.Model.Application;

    /// <summary>
    /// Third Level of the Master Detail View. Listet the ResultTest entrys.
    /// </summary>
    public class TestResultViewModel
    {
        public TestResultViewModel(TestResult testResult)
        {
            this.StartTime = testResult.StartTime;
            this.EndTime = testResult.EndTime;
            this.Result = testResult.Result;
            this.TestVersion = testResult.TestVersion;
        }

        public DateTime StartTime
        {
            get;
            set;
        }

        public DateTime EndTime
        {
            get;
            set;
        }

        public string Result
        {
            get;
            set;
        }

        public short TestVersion
        {
            get;
            set;
        }
    }
}
