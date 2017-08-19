//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Gets the Information from the TestErrorMessage table
    /// </summary>
    public class TestErrorMessage : DatabaseAccessManager
    {
        public TestErrorMessage()
        {
        }

        /// <summary>
        /// Gets a List with all TestResult Entries.
        /// </summary>
        /// <param name="testResultId">The test result identifier.</param>
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<EntityFramework.TestErrorMessage> GetEntityFrameworkTestErrorMessages()
        {
            return databaseContext.TestErrorMessage.ToList();
        }

        /// <summary>
        /// Gets a List with all TestResult Entries which has the TestResult value.
        /// </summary>
        /// <param name="testResultId">The test result identifier.</param>
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<EntityFramework.TestErrorMessage> GetEntityFrameworkTestErrorMessagesWithTestResultFilter(int testResultId)
        {
            return databaseContext.TestErrorMessage.Where(x => x.TestResult_Id == testResultId).ToList();
        }

        /// <summary>
        /// Gets a List with all TestErrorMessage Entries -> Application Type
        /// </summary>
        /// <param name="testResultId">The test result identifier.</param>
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<Application.ErrorView.TestErrorMessage> GetApplicationTestErrorMessages()
        {
            List<EntityFramework.TestErrorMessage> testErrorMessageDatabaseList = this.GetEntityFrameworkTestErrorMessages();
            List<Application.ErrorView.TestErrorMessage> testErrorMessageList = new List<Application.ErrorView.TestErrorMessage>();
            Application.ErrorView.TestErrorMessage emptyTestErrorMessage;

            foreach (var testErrorMessageDatabase in testErrorMessageDatabaseList)
            {

            }

            return testErrorMessageList;
        }

        /// <summary>
        /// Gets a List with all TestErrorMessage Entries which has the TestResult value -> Application Type
        /// </summary>
        /// <param name="testResultId">The test result identifier.</param>
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<Application.FirmwareView.TestErrorMessage> GetApplicationTestErrorMessagesWithTestResultFilter(int testResultId)
        {
            List<EntityFramework.TestErrorMessage> testErrorMessageDatabaseList = this.GetEntityFrameworkTestErrorMessagesWithTestResultFilter(testResultId);
            List<Application.FirmwareView.TestErrorMessage> testErrorMessageList = new List<Application.FirmwareView.TestErrorMessage>();
            Application.FirmwareView.TestErrorMessage emptyTestErrorMessage;

            foreach (var testErrorMessageDatabase in testErrorMessageDatabaseList)
            {
                emptyTestErrorMessage = new Application.FirmwareView.TestErrorMessage() { Description = default(string) };
                emptyTestErrorMessage.Description = testErrorMessageDatabase.Description;

                testErrorMessageList.Add(emptyTestErrorMessage);
            }

            return testErrorMessageList;
        }
    }
}
