//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmwareDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using System.Linq;

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
        /// Gets a List with all InitialStateFirmware Entries. Included with SoftwareVersionsId and TestCollectionId. With ID's -> EntityFramework Type InitialStateFirmware
        /// </summary>
        /// <returns>List with all InitialStateFirmware Entries</returns>
        public List<EntityFramework.InitialStateFirmware> GetEntityFrameworkInitialStateFirmwares()
        {
            return databaseContext.InitialStateFirmware.ToList();
        }

        /// <summary>
        /// Gets a List with all InitailStateFirmware Entries. Without any ID's -> Application Type InitialStateValve
        /// </summary>
        /// <returns>List with all InitailStateFirmware Entries</returns>
        public List<Application.InitialStateFirmware> GetApplicationInitialStateFirmwares()
        {
            List<EntityFramework.InitialStateFirmware> initialStateFirmwareDatabaseList = this.GetEntityFrameworkInitialStateFirmwares();
            List<Application.InitialStateFirmware> initialStateFirmwareList = new List<Application.InitialStateFirmware>();
            Application.InitialStateFirmware tempInitialStateFirmware = null;

            foreach (var initialStateFirmwareDatabase in initialStateFirmwareDatabaseList)
            {
                tempInitialStateFirmware.ValveFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_Firmware);
                tempInitialStateFirmware.ValveFirmwareReleaseTime = initialStateFirmwareDatabase.ValveFirmwareReleaseTime;
                tempInitialStateFirmware.MotionControllerFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_MotionController);
                tempInitialStateFirmware.InterfaceFirmware = this.GetSoftwareVersionsName((int)initialStateFirmwareDatabase.SoftwareVersions_Id_Interface);
                tempInitialStateFirmware.DriveParameterID = this.GetDriveParameterID((int)initialStateFirmwareDatabase.DriveParameterFile_ID);
                tempInitialStateFirmware.ConfigurationParameterID = this.GetConfigurationParameterID((int)initialStateFirmwareDatabase.ConfigurationParameterFile_ID);
                tempInitialStateFirmware.TestCollection = this.GetTestCollectionName((int)initialStateFirmwareDatabase.TestCollection_Id);

                initialStateFirmwareList.Add(tempInitialStateFirmware);
            }

            return initialStateFirmwareList;
        }

        /// <summary>
        /// Gets the name of the test collection with the test collection id entry.
        /// </summary>
        /// <param name="testCollectionId">The test collection identifier.</param>
        /// <returns>name of the test collection</returns>
        public string GetTestCollectionName(int testCollectionId)
        {
            return databaseContext.TestCollection.Where(x => x.Id == testCollectionId).FirstOrDefault().Name;
        }

        /// <summary>
        /// Gets the name of the software versions with the software versions id entry.
        /// </summary>
        /// <param name="softwareVersionsId">The software versions identifier.</param>
        /// <returns>name of the software versions</returns>
        public string GetSoftwareVersionsName(int softwareVersionsId)
        {
            return databaseContext.SoftwareVersions.Where(x => x.Id == softwareVersionsId).FirstOrDefault().Software;
        }

        /// <summary>
        /// Gets the drive parameter string id with the drive parameter id entry.
        /// </summary>
        /// <param name="driveParameterId">The drive parameter identifier.</param>
        /// <returns>drive parameter string id</returns>
        public string GetDriveParameterID(int driveParameterId)
        {
            return databaseContext.DriveParameterFile.Where(x => x.Id == driveParameterId).FirstOrDefault().Name;
        }

        /// <summary>
        /// Gets the configuration parameter string id with the drive parameter id entry.
        /// </summary>
        /// <param name="configurationParameterId">The configuration parameter identifier.</param>
        /// <returns>configuration parameter string id</returns>
        public string GetConfigurationParameterID(int configurationParameterId)
        {
            return databaseContext.ConfigurationParameterFile.Where(x => x.Id == configurationParameterId).FirstOrDefault().Name;
        }
    }
}
