//-----------------------------------------------------------------------
// <copyright file="TestResultDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Gets the Information from the TestResult table
    /// Shows a single test result. Important if the test are failed to see the first three error messages
    /// </summary>
    public class TestResultDatabaseAccessManager : DatabaseAccessManager
    {
        public TestResultDatabaseAccessManager()
        {
        }

        /// <summary>
        /// Gets a List with all TestResult Entries. Included with Id for Result and TestVersion definitions.
        /// </summary>
        /// <param name="testCollectionResultId">The test collection result identifier.</param>
        /// <returns>List with all TestResult Entries</returns>
        public List<EntityFramework.TestResult> GetEntityFrameworkTestResults(int testCollectionResultId)
        {
            return databaseContext.TestResult.Where(x => x.TestCollectionResult_Id == testCollectionResultId).ToList();
        }

        /// <summary>
        /// Gets the application test results.
        /// </summary>
        /// <param name="testCollectionResultId">The test collection result identifier.</param>
        /// <returns>List with all TestResult Entries</returns>
        public List<Application.TestResult> GetApplicationTestResults(int testCollectionResultId)
        {
            List<EntityFramework.TestResult> testResultDatabaseList = this.GetEntityFrameworkTestResults(testCollectionResultId);
            List<Application.TestResult> testResultList = new List<Application.TestResult>();
            Application.TestResult emptyTestResult;

            foreach (var testResultDatabase in testResultDatabaseList)
            {
                emptyTestResult = new Application.TestResult() { Id = default(int), StartTime = default(DateTime), EndTime = default(DateTime), Result = default(string), TestVersion = default(short) };
                emptyTestResult.Id = testResultDatabase.Id;
                emptyTestResult.StartTime = testResultDatabase.StartTime;
                emptyTestResult.EndTime = testResultDatabase.EndTime;
                emptyTestResult.Result = this.GetResultName((int)testResultDatabase.ResultType_Id);
                emptyTestResult.TestVersion = this.GetTestVersion((int)testResultDatabase.TestVersion_Id);

                testResultList.Add(emptyTestResult);
            }

            return testResultList;
        }

        public string GetResultName(int resultTypeId)
        {
            return databaseContext.ResultType.Where(x => x.Id == resultTypeId).FirstOrDefault().Name;
        }

        public short GetTestVersion(int testVersionId)
        {
            return databaseContext.TestVersion.Where(x => x.Id == testVersionId).FirstOrDefault().Number;
        }
    }
}
