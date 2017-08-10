//-----------------------------------------------------------------------
// <copyright file="TestResultDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
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

        public List<Application.TestResult> GetApplicationTestResults()
        {
            List<EntityFramework.TestResult> testResultDatabaseList = this.GetEntityFrameworkTestResults();
            List<Application.InitialStateFirmware> initialStateFirmwareList = new List<Application.InitialStateFirmware>();
            Application.InitialStateFirmware emptyInitialStateFirmware;

            foreach (var initialStateFirmwareDatabase in initialStateFirmwareDatabaseList)
            {
                emptyInitialStateFirmware = new Application.InitialStateFirmware() { Id = default(int), ValveFirmware = default(string), ValveFirmwareReleaseTime = default(DateTime), MotionControllerFirmware = default(string), InterfaceFirmware = default(string), DriveParameterID = default(string), ConfigurationParameterID = default(string), TestCollection = default(string) };
                emptyInitialStateFirmware.Id = initialStateFirmwareDatabase.Id;
                emptyInitialStateFirmware.ValveFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_Firmware);
                emptyInitialStateFirmware.ValveFirmwareReleaseTime = initialStateFirmwareDatabase.ValveFirmwareReleaseTime;
                emptyInitialStateFirmware.MotionControllerFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_MotionController);
                emptyInitialStateFirmware.InterfaceFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_Interface);
                emptyInitialStateFirmware.DriveParameterID = this.GetDriveParameterID((int)initialStateFirmwareDatabase.DriveParameterFile_ID);
                emptyInitialStateFirmware.ConfigurationParameterID = this.GetConfigurationParameterID((int)initialStateFirmwareDatabase.ConfigurationParameterFile_ID);
                emptyInitialStateFirmware.TestCollection = this.GetTestCollectionName((int)initialStateFirmwareDatabase.TestCollection_Id);

                initialStateFirmwareList.Add(emptyInitialStateFirmware);
            }

            return initialStateFirmwareList;
        }
    }
}
