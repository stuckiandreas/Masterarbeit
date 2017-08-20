//-----------------------------------------------------------------------
// <copyright file="TestErrorMessage.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
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
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<Application.ErrorView.TestErrorMessage> GetApplicationTestErrorMessages()
        {
            List<EntityFramework.TestErrorMessage> testErrorMessageDatabaseList = this.GetEntityFrameworkTestErrorMessages();
            List<Application.ErrorView.TestErrorMessage> testErrorMessageList = new List<Application.ErrorView.TestErrorMessage>();
            EntityFramework.TestResult testResult;
            EntityFramework.TestCollectionResult testCollectionResult;
            EntityFramework.InitialStateFirmware initialStateFirmware;
            Application.ErrorView.TestErrorMessage emptyTestErrorMessage;

            foreach (var testErrorMessageDatabase in testErrorMessageDatabaseList)
            {
                //Get TestResult values
                testResult = this.GetEntityFrameworkTestResult((int)testErrorMessageDatabase.TestResult_Id);

                //Get ValveHardware
                testCollectionResult = this.GetEntityFrameworkTestCollectionResult((int)testResult.TestCollectionResult_Id);

                //Get InitialStateFirmware
                initialStateFirmware = this.GetEntityFrameworkInitialStateFirmware((int)testCollectionResult.InitialStateFirmware_Id);

                emptyTestErrorMessage = new Application.ErrorView.TestErrorMessage()
                {
                    Description = default(string), TestName = default(string), TestVersion = default(short),
                    StartTime = default(DateTime), EndTime = default(DateTime), ValveFirmware = default(string),
                    ValveFirmwareReleaseTime = default(DateTime), MotionControllerFirmware = default(string),
                    InterfaceFirmware = default(string), DriveParameterID = default(string), ConfigurationParameterID = default(string),
                    TestCollection = default(string), ValveSerie = default(string), InterfaceType = default(string),
                    ControllerType = default(string), OptionType = default(string), ExternalIsolationValve = default(bool),
                    ControllerHardwareVersion = default(string), InterfaceHardwareVersion = default(string),
                    ControllerAssemblyVariant = default(string), InterfaceAssemblyVariant = default(string), Module1Type = default(string),
                    Module1HardwareVersion = default(string), Module1AssemblyVariant = default(string), Module2Type = default(string),
                    Module2HardwareVersion = default(string), Module2AssemblyVariant = default(string), Module3Type = default(string),
                    Module3HardwareVersion = default(string), Module3AssemblyVariant = default(string), Module4Type = default(string),
                    Module4HardwareVersion = default(string), Module4AssemblyVariant = default(string)
                };

                emptyTestErrorMessage.Description = testErrorMessageDatabase.Description;
                emptyTestErrorMessage.TestVersion = this.GetTestVersion((int)testResult.TestVersion_Id);
                emptyTestErrorMessage.TestName = this.GetTestName((int)testResult.TestVersion_Id);
                emptyTestErrorMessage.StartTime = testResult.StartTime;
                emptyTestErrorMessage.EndTime = testResult.EndTime;
                emptyTestErrorMessage.ValveFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_Firmware);
                emptyTestErrorMessage.ValveFirmwareReleaseTime = initialStateFirmware.ValveFirmwareReleaseTime;
                emptyTestErrorMessage.MotionControllerFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_MotionController);
                emptyTestErrorMessage.InterfaceFirmware = this.GetSoftwareVersionsName((int)initialStateFirmware.SoftwareVersions_Id_Interface);
                emptyTestErrorMessage.DriveParameterID = this.GetDriveParameterID((int)initialStateFirmware.DriveParameterFile_ID);
                emptyTestErrorMessage.ConfigurationParameterID = this.GetConfigurationParameterID((int)initialStateFirmware.ConfigurationParameterFile_ID);
                emptyTestErrorMessage.TestCollection = this.GetTestCollectionName((int)initialStateFirmware.TestCollection_Id);
                emptyTestErrorMessage.ValveSerie = this.GetValveSerieEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.InterfaceType = this.GetInterfaceTypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.ControllerType = this.GetControllerTypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.OptionType = this.GetOptionTypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.ExternalIsolationValve = this.GetExternalIsolationValve((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.ControllerHardwareVersion = this.GetControllerHardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.InterfaceHardwareVersion = this.GetInterfaceHardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.ControllerAssemblyVariant = this.GetControllerAssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.InterfaceAssemblyVariant = this.GetInterfaceAssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module1Type = this.GetModule1TypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module1HardwareVersion = this.GetModule1HardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module1AssemblyVariant = this.GetModule1AssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module2Type = this.GetModule2TypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module2HardwareVersion = this.GetModule2HardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module2AssemblyVariant = this.GetModule2AssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module3Type = this.GetModule3TypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module3HardwareVersion = this.GetModule3HardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module3AssemblyVariant = this.GetModule3AssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module4Type = this.GetModule4TypeEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module4HardwareVersion = this.GetModule4HardwareVersionEnum((int)testCollectionResult.ValveHardware_Id);
                emptyTestErrorMessage.Module4AssemblyVariant = this.GetModule4AssemblyVariantEnum((int)testCollectionResult.ValveHardware_Id);

                testErrorMessageList.Add(emptyTestErrorMessage);
            }

            return testErrorMessageList;
        }

        /// <summary>
        /// Gets a List with all TestErrorMessage Entries which has the TestResult value -> Application Type
        /// </summary>
        /// <param name="testResultId">The test result identifier.</param>
        /// <returns>List with all TestErrorMessage Entries</returns>
        public List<Application.General.TestErrorMessage> GetApplicationTestErrorMessagesWithTestResultFilter(int testResultId)
        {
            List<EntityFramework.TestErrorMessage> testErrorMessageDatabaseList = 
                this.GetEntityFrameworkTestErrorMessagesWithTestResultFilter(testResultId);
            List<Application.General.TestErrorMessage> testErrorMessageList = new List<Application.General.TestErrorMessage>();
            Application.General.TestErrorMessage emptyTestErrorMessage;

            foreach (var testErrorMessageDatabase in testErrorMessageDatabaseList)
            {
                emptyTestErrorMessage = new Application.General.TestErrorMessage() { Description = default(string) };
                emptyTestErrorMessage.Description = testErrorMessageDatabase.Description;

                testErrorMessageList.Add(emptyTestErrorMessage);
            }

            return testErrorMessageList;
        }
    }
}
