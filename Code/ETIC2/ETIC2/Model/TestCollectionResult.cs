//-----------------------------------------------------------------------
// <copyright file="TestCollectionResult.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Gets the Information from the TestCollectionResult table
    /// Shows a single test collection result. With the important information how many tests are failed
    /// </summary>
    public class TestCollectionResult : DatabaseAccessManager
    {
        public TestCollectionResult()
        {
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult Entries which has the InitialSateFirmware value. Included with Id for hardware definitions.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<EntityFramework.TestCollectionResult> GetEntityFrameworkTestCollectionResultsInitialStateFirmwareFilter(int initialStateFirmwareId)
        {
            return databaseContext.TestCollectionResult.Where(x => x.InitialStateFirmware_Id == initialStateFirmwareId).ToList();
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult Entries which has the ValveHardware value. Included with Id for firmware definitions.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<EntityFramework.TestCollectionResult> GetEntityFrameworkTestCollectionResultsValveHardwareFilter(int valveHardwareId)
        {
            return databaseContext.TestCollectionResult.Where(x => x.ValveHardware_Id == valveHardwareId).ToList();
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult and ValveHardware Entries which has the InitialSateFirmware value. Without the Id from database Application Type.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<Application.FirmwareView.TestCollectionResultAndValveHardware> GetApplicationTestCollectionResultsAndValveHardwareWithInitialStateFirmwareFilter(int initialStateFirmwareId)
        {
            List<EntityFramework.TestCollectionResult> testCollectionResultDatabaseList = this.GetEntityFrameworkTestCollectionResultsInitialStateFirmwareFilter(initialStateFirmwareId);
            List<Application.FirmwareView.TestCollectionResultAndValveHardware> testCollectionResultAndValveHardwareList = new List<Application.FirmwareView.TestCollectionResultAndValveHardware>();
            Application.FirmwareView.TestCollectionResultAndValveHardware emptyTestCollectionResultAndValveHardware;

            foreach (var testCollectionResultDatabase in testCollectionResultDatabaseList)
            {
                emptyTestCollectionResultAndValveHardware = new Application.FirmwareView.TestCollectionResultAndValveHardware() { Id = default(int), ExecutionTime = default(DateTime), UserName = default(string), AbortType = default(string), CountErrorTest = default(int),
                    ValveSerie = default(string), InterfaceType = default(string), ControllerType = default(string), OptionType = default(string), ExternalIsolationValve = default(bool), ControllerHardwareVersion = default(string), InterfaceHardwareVersion = default(string), ControllerAssemblyVariant = default(string),
                    InterfaceAssemblyVariant = default(string), Module1Type = default(string), Module1HardwareVersion = default(string), Module1AssemblyVariant = default(string), Module2Type = default(string), Module2HardwareVersion = default(string), Module2AssemblyVariant = default(string),
                    Module3Type = default(string), Module3HardwareVersion = default(string), Module3AssemblyVariant = default(string), Module4Type = default(string), Module4HardwareVersion = default(string), Module4AssemblyVariant = default(string) };

                emptyTestCollectionResultAndValveHardware.Id = testCollectionResultDatabase.Id;
                emptyTestCollectionResultAndValveHardware.ExecutionTime = testCollectionResultDatabase.ExecutionDateTime;
                emptyTestCollectionResultAndValveHardware.UserName = testCollectionResultDatabase.UserName;
                emptyTestCollectionResultAndValveHardware.AbortType = this.GetAbortName((int)testCollectionResultDatabase.AbortType_Id);
                emptyTestCollectionResultAndValveHardware.CountErrorTest = testCollectionResultDatabase.CountErrorTest;
                emptyTestCollectionResultAndValveHardware.ValveSerie = this.GetValveSerieEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.InterfaceType = this.GetInterfaceTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.ControllerType = this.GetControllerTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.OptionType = this.GetOptionTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.ExternalIsolationValve = this.GetExternalIsolationValve((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.ControllerHardwareVersion = this.GetControllerHardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.InterfaceHardwareVersion = this.GetInterfaceHardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.ControllerAssemblyVariant = this.GetControllerAssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.InterfaceAssemblyVariant = this.GetInterfaceAssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module1Type = this.GetModule1TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module1HardwareVersion = this.GetModule1HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module1AssemblyVariant = this.GetModule1AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module2Type = this.GetModule2TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module2HardwareVersion = this.GetModule2HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module2AssemblyVariant = this.GetModule2AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module3Type = this.GetModule3TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module3HardwareVersion = this.GetModule3HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module3AssemblyVariant = this.GetModule3AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module4Type = this.GetModule4TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module4HardwareVersion = this.GetModule4HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResultAndValveHardware.Module4AssemblyVariant = this.GetModule4AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);

                testCollectionResultAndValveHardwareList.Add(emptyTestCollectionResultAndValveHardware);
            }

            return testCollectionResultAndValveHardwareList;
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult and InitialStateFirmware Entries which has the ValveHardwareId value. Without the Id from database Application Type.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<Application.HardwareView.TestCollectionResultAndInitialStateFirmware> GetApplicationTestCollectionResultsAndInitialStateFirmwareWithValveHardwareFilter(int valveHardwareId)
        {
            List<EntityFramework.TestCollectionResult> testCollectionResultDatabaseList = this.GetEntityFrameworkTestCollectionResultsValveHardwareFilter(valveHardwareId);
            List<Application.HardwareView.TestCollectionResultAndInitialStateFirmware> testCollectionResultAndInitialStateFirmwareList = new List<Application.HardwareView.TestCollectionResultAndInitialStateFirmware>();
            Application.HardwareView.TestCollectionResultAndInitialStateFirmware emptyTestCollectionResultAndInitialStateFirmware;
            EntityFramework.InitialStateFirmware initialStateFirmware;

            foreach (var testCollectionResultDatabase in testCollectionResultDatabaseList)
            {
                emptyTestCollectionResultAndInitialStateFirmware = new Application.HardwareView.TestCollectionResultAndInitialStateFirmware() { Id = default(int), ExecutionTime = default(DateTime), UserName = default(string), AbortType = default(string), CountErrorTest = default(int),
                    ValveFirmware = default(string), ValveFirmwareReleaseTime = default(DateTime), MotionControllerFirmware = default(string), InterfaceFirmware = default(string), DriveParameterID = default(string), ConfigurationParameterID = default(string), TestCollection = default(string) };

                emptyTestCollectionResultAndInitialStateFirmware.Id = testCollectionResultDatabase.Id;
                emptyTestCollectionResultAndInitialStateFirmware.ExecutionTime = testCollectionResultDatabase.ExecutionDateTime;
                emptyTestCollectionResultAndInitialStateFirmware.UserName = testCollectionResultDatabase.UserName;
                emptyTestCollectionResultAndInitialStateFirmware.AbortType = this.GetAbortName((int)testCollectionResultDatabase.AbortType_Id);
                emptyTestCollectionResultAndInitialStateFirmware.CountErrorTest = testCollectionResultDatabase.CountErrorTest;
                initialStateFirmware = GetEntityFrameworkInitialStateFirmware((int)testCollectionResultDatabase.InitialStateFirmware_Id);
                emptyTestCollectionResultAndInitialStateFirmware.ValveFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_Firmware);
                emptyTestCollectionResultAndInitialStateFirmware.ValveFirmwareReleaseTime = initialStateFirmware.ValveFirmwareReleaseTime;
                emptyTestCollectionResultAndInitialStateFirmware.MotionControllerFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_MotionController);
                emptyTestCollectionResultAndInitialStateFirmware.InterfaceFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_Interface);
                emptyTestCollectionResultAndInitialStateFirmware.DriveParameterID = this.GetDriveParameterID((int)initialStateFirmware.DriveParameterFile_ID);
                emptyTestCollectionResultAndInitialStateFirmware.ConfigurationParameterID = this.GetConfigurationParameterID((int)initialStateFirmware.ConfigurationParameterFile_ID);
                emptyTestCollectionResultAndInitialStateFirmware.TestCollection = this.GetTestCollectionName((int)initialStateFirmware.TestCollection_Id);

                testCollectionResultAndInitialStateFirmwareList.Add(emptyTestCollectionResultAndInitialStateFirmware);
            }

            return testCollectionResultAndInitialStateFirmwareList;
        }


        private string GetAbortName(int abortTypeId)
        {
            return databaseContext.AbortType.Where(x => x.Id == abortTypeId).FirstOrDefault().Name;
        }
    }
}
