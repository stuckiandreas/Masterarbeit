//-----------------------------------------------------------------------
// <copyright file="Converter.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using ETIC2.Model.Application;
    using ETIC2.Model.EntityFramework;

    /// <summary>
    /// Convert all Model Data Classes for the view models from the entity framework
    /// </summary>
    public class Converter
    {
        public InitialStateFirmwareDatabaseAccessManager initialStateFirmwareDatabaseAccessManager;

        public Converter()
        {
        }

        public List<Application.InitialStateFirmware> ConvertInitialStateFirmware()
        {
            initialStateFirmwareDatabaseAccessManager = new InitialStateFirmwareDatabaseAccessManager();
            List<EntityFramework.InitialStateFirmware> initialStateFirmwareDatabaseList;
            List<Application.InitialStateFirmware> initialStateFirmwareList =  new List<Application.InitialStateFirmware>();
            Application.InitialStateFirmware tempInitialStateFirmware = null;

            initialStateFirmwareDatabaseList = initialStateFirmwareDatabaseAccessManager.GetInitialStateFirmwares();
            foreach(var InitialStateFirmwareDatabase in initialStateFirmwareDatabaseList)
            {
                tempInitialStateFirmware.ValveFirmware = initialStateFirmwareDatabaseAccessManager.GetSoftwareVersionsName((int)InitialStateFirmwareDatabase.SoftwareVersions_Id_Firmware);
                tempInitialStateFirmware.ValveFirmwareReleaseTime = InitialStateFirmwareDatabase.ValveFirmwareReleaseTime;
                tempInitialStateFirmware.MotionControllerFirmware = initialStateFirmwareDatabaseAccessManager.GetSoftwareVersionsName((int)InitialStateFirmwareDatabase.SoftwareVersions_Id_MotionController);
                tempInitialStateFirmware.InterfaceFirmware = initialStateFirmwareDatabaseAccessManager.GetSoftwareVersionsName((int)InitialStateFirmwareDatabase.SoftwareVersions_Id_Interface);
                tempInitialStateFirmware.DriveParameterID = initialStateFirmwareDatabaseAccessManager.GetDriveParameterID((int)InitialStateFirmwareDatabase.DriveParameterFile_ID);
                tempInitialStateFirmware.ConfigurationParameterID = initialStateFirmwareDatabaseAccessManager.GetConfigurationParameterID((int)InitialStateFirmwareDatabase.ConfigurationParameterFile_ID);
                tempInitialStateFirmware.TestCollection = initialStateFirmwareDatabaseAccessManager.GetTestCollectionName((int)InitialStateFirmwareDatabase.TestCollection_Id);

                initialStateFirmwareList.Add(tempInitialStateFirmware);
            }
            return initialStateFirmwareList;
        }
    }
}

//Nach Adrian sollte das die Klasse static sein