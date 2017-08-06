//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmwareDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using System.Linq;
    using ETIC2.Model.EntityFramework;

    /// <summary>
    /// Gets the Information from the InitialStateFirmware table
    /// Neccesary to define a initial state firmware because the TTIC2 application needs this information to update valve hardware before the test collection starts
    /// </summary>
    public class InitialStateFirmwareDatabaseAccessManager : DatabaseAccessManager
    {
        public InitialStateFirmwareDatabaseAccessManager()
        {
        }

        /// <summary>
        /// Gets a List with all InitialStateValveFirmware Entries. Included with SoftwareVersionsId and TestCollectionId.
        /// </summary>
        /// <returns></returns>
        public List<InitialStateFirmware> GetInitialStateFirmwares()
        {
            return databaseContext.InitialStateFirmware.ToList();
        }

        /// <summary>
        /// Gets the name of the test collection with the test collection id entry.
        /// </summary>
        /// <param name="TestCollectionId">The test collection identifier.</param>
        /// <returns></returns>
        public string GetTestCollectionName(int TestCollectionId)
        {
            return databaseContext.TestCollection.Where(x => x.Id == TestCollectionId).FirstOrDefault().Name;
        }

        /// <summary>
        /// Gets the name of the software versions with the software versions id entry.
        /// </summary>
        /// <param name="SoftwareVersionsId">The software versions identifier.</param>
        /// <returns></returns>
        public string GetSoftwareVersionsName(int SoftwareVersionsId)
        {
            return databaseContext.SoftwareVersions.Where(x => x.Id == SoftwareVersionsId).FirstOrDefault().Software;
        }

        /// <summary>
        /// Gets the drive parameter string id with the drive parameter id entry.
        /// </summary>
        /// <param name="DriveParameterId">The drive parameter identifier.</param>
        /// <returns></returns>
        public string GetDriveParameterID(int DriveParameterId)
        {
            return databaseContext.DriveParameterFile.Where(x => x.Id == DriveParameterId).FirstOrDefault().Name;
        }

        /// <summary>
        /// Gets the configuration parameter string id with the drive parameter id entry.
        /// </summary>
        /// <param name="ConfigurationParameterId">The configuration parameter identifier.</param>
        /// <returns></returns>
        public string GetConfigurationParameterID(int ConfigurationParameterId)
        {
            return databaseContext.ConfigurationParameterFile.Where(x => x.Id == ConfigurationParameterId).FirstOrDefault().Name;
        }
    }
}
