//-----------------------------------------------------------------------
// <copyright file="TestCollectionResultWithHardwareDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
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
    public class TestCollectionResultWithHardware : DatabaseAccessManager
    {
        public TestCollectionResultWithHardware()
        {
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult Entries. Included with Id for hardware definitions.
        /// </summary>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<EntityFramework.TestCollectionResult> GetEntityFrameworkTestCollectionResults()
        {
            return databaseContext.TestCollectionResult.ToList();
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult Entries which has the InitialSateFirmware value. Included with Id for hardware definitions.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<EntityFramework.TestCollectionResult> GetEntityFrameworkTestCollectionResultsWithInitialStateFirmwareFilter(int initialStateFirmwareId)
        {
            return databaseContext.TestCollectionResult.Where(x => x.InitialStateFirmware_Id == initialStateFirmwareId).ToList();
        }

        /// <summary>
        /// Gets a List with all TestCollectionResult Entries which has the InitialSateFirmware value. Without the Id from database Application Type.
        /// </summary>
        /// <param name="initialStateFirmwareId">The initial state firmware identifier.</param>
        /// <returns>
        /// List with all TestCollectionResult Entries
        /// </returns>
        public List<Application.FirmwareView.TestCollectionResultWithValveHardware> GetApplicationTestCollectionResultsWithValveHardwareWithInitialStateFirmwareFilter(int initialStateFirmwareId)
        {
            List<EntityFramework.TestCollectionResult> testCollectionResultDatabaseList = this.GetEntityFrameworkTestCollectionResultsWithInitialStateFirmwareFilter(initialStateFirmwareId);
            List<Application.FirmwareView.TestCollectionResultWithValveHardware> testCollectionResultList = new List<Application.FirmwareView.TestCollectionResultWithValveHardware>();
            Application.FirmwareView.TestCollectionResultWithValveHardware emptyTestCollectionResult;

            foreach (var testCollectionResultDatabase in testCollectionResultDatabaseList)
            {
                emptyTestCollectionResult = new Application.FirmwareView.TestCollectionResultWithValveHardware() { Id = default(int), ExecutionTime = default(DateTime), UserName = default(string), AbortType = default(string), CountErrorTest = default(int), ValveSerie = default(string), InterfaceType = default(string), ControllerType = default(string), OptionType = default(string), ExternalIsolationValve = default(bool), ControllerHardwareVersion = default(string), InterfaceHardwareVersion = default(string), ControllerAssemblyVariant = default(string), InterfaceAssemblyVariant = default(string), Module1Type = default(string), Module1HardwareVersion = default(string), Module1AssemblyVariant = default(string), Module2Type = default(string), Module2HardwareVersion = default(string), Module2AssemblyVariant = default(string), Module3Type = default(string), Module3HardwareVersion = default(string), Module3AssemblyVariant = default(string), Module4Type = default(string), Module4HardwareVersion = default(string), Module4AssemblyVariant = default(string) };
                emptyTestCollectionResult.Id = testCollectionResultDatabase.Id;
                emptyTestCollectionResult.ExecutionTime = testCollectionResultDatabase.ExecutionDateTime;
                emptyTestCollectionResult.UserName = testCollectionResultDatabase.UserName;
                emptyTestCollectionResult.AbortType = this.GetAbortName((int)testCollectionResultDatabase.AbortType_Id);
                emptyTestCollectionResult.CountErrorTest = testCollectionResultDatabase.CountErrorTest;
                emptyTestCollectionResult.ValveSerie = this.GetValveSerieEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.InterfaceType = this.GetInterfaceTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.ControllerType = this.GetControllerTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.OptionType = this.GetOptionTypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.ExternalIsolationValve = this.GetExternalIsolationValve((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.ControllerHardwareVersion = this.GetControllerHardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.InterfaceHardwareVersion = this.GetInterfaceHardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.ControllerAssemblyVariant = this.GetControllerAssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.InterfaceAssemblyVariant = this.GetInterfaceAssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module1Type = this.GetModule1TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module1HardwareVersion = this.GetModule1HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module1AssemblyVariant = this.GetModule1AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module2Type = this.GetModule2TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module2HardwareVersion = this.GetModule2HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module2AssemblyVariant = this.GetModule2AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module3Type = this.GetModule3TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module3HardwareVersion = this.GetModule3HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module3AssemblyVariant = this.GetModule3AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module4Type = this.GetModule4TypeEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module4HardwareVersion = this.GetModule4HardwareVersionEnum((int)testCollectionResultDatabase.ValveHardware_Id);
                emptyTestCollectionResult.Module4AssemblyVariant = this.GetModule4AssemblyVariantEnum((int)testCollectionResultDatabase.ValveHardware_Id);

                testCollectionResultList.Add(emptyTestCollectionResult);
            }

            return testCollectionResultList;
        }

        private string GetAbortName(int abortTypeId)
        {
            return databaseContext.AbortType.Where(x => x.Id == abortTypeId).FirstOrDefault().Name;
        }

        private string GetValveSerieEnum(int valveHardwareId)
        {
            int valveSerieId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ValveSerie_Id;
            return databaseContext.ValveSerie.Where(x => x.Id == valveSerieId).FirstOrDefault().Enum;
        }

        private string GetInterfaceTypeEnum(int valveHardwareId)
        {
            int interfaceTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceType_Id;
            return databaseContext.InterfaceType.Where(x => x.Id == interfaceTypeId).FirstOrDefault().Enum;
        }

        private string GetControllerTypeEnum(int valveHardwareId)
        {
            int controllerTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerType_Id;
            return databaseContext.ControllerType.Where(x => x.Id == controllerTypeId).FirstOrDefault().Enum;
        }

        private string GetOptionTypeEnum(int valveHardwareId)
        {
            int optionTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().OptionType_Id;
            return databaseContext.OptionType.Where(x => x.Id == optionTypeId).FirstOrDefault().Enum;
        }

        private bool GetExternalIsolationValve(int valveHardwareId)
        {
            short externalIsolationValveOption;
            externalIsolationValveOption = databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ExternalIsolationValveOption;
            if (externalIsolationValveOption == 0)
                return false;
            else
                return true;
        }

        private string GetControllerHardwareVersionEnum(int valveHardwareId)
        {
            int controllerHardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerHardwareVersion_Id;
            return databaseContext.ControllerHardwareVersion.Where(x => x.Id == controllerHardwareVersion).FirstOrDefault().Enum;
        }

        private string GetInterfaceHardwareVersionEnum(int valveHardwareId)
        {
            int interfaceHardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceHardwareVersion_Id;
            return databaseContext.InterfaceHardwareVersion.Where(x => x.Id == interfaceHardwareVersion).FirstOrDefault().Enum;
        }

        private string GetControllerAssemblyVariantEnum(int valveHardwareId)
        {
            int controllerAssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerAssemblyVariant_Id;
            return databaseContext.ControllerAssemblyVariant.Where(x => x.Id == controllerAssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetInterfaceAssemblyVariantEnum(int valveHardwareId)
        {
            int interfaceAssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceAssemblyVariant_Id;
            return databaseContext.InterfaceAssemblyVariant.Where(x => x.Id == interfaceAssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetModule1TypeEnum(int valveHardwareId)
        {
            int module1Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1Type_Id;
            return databaseContext.Module1Type.Where(x => x.Id == module1Type).FirstOrDefault().Enum;
        }

        private string GetModule1HardwareVersionEnum(int valveHardwareId)
        {
            int module1HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1HardwareVersion_Id;
            return databaseContext.Module1HardwareVersion.Where(x => x.Id == module1HardwareVersion).FirstOrDefault().Enum;
        }

        private string GetModule1AssemblyVariantEnum(int valveHardwareId)
        {
            int module1AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1AssemblyVariant_Id;
            return databaseContext.Module1AssemblyVariant.Where(x => x.Id == module1AssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetModule2TypeEnum(int valveHardwareId)
        {
            int module2Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2Type_Id;
            return databaseContext.Module2Type.Where(x => x.Id == module2Type).FirstOrDefault().Enum;
        }

        private string GetModule2HardwareVersionEnum(int valveHardwareId)
        {
            int module2HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2HardwareVersion_Id;
            return databaseContext.Module2HardwareVersion.Where(x => x.Id == module2HardwareVersion).FirstOrDefault().Enum;
        }

        private string GetModule2AssemblyVariantEnum(int valveHardwareId)
        {
            int module2AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2AssemblyVariant_Id;
            return databaseContext.Module2AssemblyVariant.Where(x => x.Id == module2AssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetModule3TypeEnum(int valveHardwareId)
        {
            int module3Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3Type_Id;
            return databaseContext.Module3Type.Where(x => x.Id == module3Type).FirstOrDefault().Enum;
        }

        private string GetModule3HardwareVersionEnum(int valveHardwareId)
        {
            int module3HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3HardwareVersion_Id;
            return databaseContext.Module3HardwareVersion.Where(x => x.Id == module3HardwareVersion).FirstOrDefault().Enum;
        }

        private string GetModule3AssemblyVariantEnum(int valveHardwareId)
        {
            int module3AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3AssemblyVariant_Id;
            return databaseContext.Module3AssemblyVariant.Where(x => x.Id == module3AssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetModule4TypeEnum(int valveHardwareId)
        {
            int module4Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4Type_Id;
            return databaseContext.Module4Type.Where(x => x.Id == module4Type).FirstOrDefault().Enum;
        }

        private string GetModule4HardwareVersionEnum(int valveHardwareId)
        {
            int module4HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4HardwareVersion_Id;
            return databaseContext.Module4HardwareVersion.Where(x => x.Id == module4HardwareVersion).FirstOrDefault().Enum;
        }

        private string GetModule4AssemblyVariantEnum(int valveHardwareId)
        {
            int module4AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4AssemblyVariant_Id;
            return databaseContext.Module4AssemblyVariant.Where(x => x.Id == module4AssemblyVariant).FirstOrDefault().Enum;
        }
    }
}
