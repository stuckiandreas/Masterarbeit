//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Gets the Information from the InitialStateFirmware table
    /// Neccesary to define a initial state firmware because the TTIC2 application needs this information to update valve hardware before the test collection starts
    /// </summary>
    public class InitialStateFirmware : DatabaseAccessManager
    {
        public InitialStateFirmware()
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
        public List<Application.FirmwareView.InitialStateFirmware> GetApplicationInitialStateFirmwares()
        {
            List<EntityFramework.InitialStateFirmware> initialStateFirmwareDatabaseList = this.GetEntityFrameworkInitialStateFirmwares();
            List<Application.FirmwareView.InitialStateFirmware> initialStateFirmwareList = new List<Application.FirmwareView.InitialStateFirmware>();
            Application.FirmwareView.InitialStateFirmware emptyInitialStateFirmware;

            foreach (var initialStateFirmwareDatabase in initialStateFirmwareDatabaseList)
            {
                emptyInitialStateFirmware = new Application.FirmwareView.InitialStateFirmware() { Id = default(int), ValveFirmware = default(string), ValveFirmwareReleaseTime = default(DateTime), MotionControllerFirmware = default(string), InterfaceFirmware = default(string), DriveParameterID = default(string), ConfigurationParameterID = default(string), TestCollection = default(string) };
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
