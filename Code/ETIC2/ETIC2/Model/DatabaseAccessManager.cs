//-----------------------------------------------------------------------
// <copyright file="DatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Data.SqlClient;
    using System.Linq;
    using EntityFramework;
    using Events.EventArgs.DatabaseAccess;

    /// <summary>
    /// Managed the access to the database
    /// </summary>
    public abstract class DatabaseAccessManager
    {
        /// <summary>
        /// The database context
        /// </summary>
        protected SoftwareVersionsDatabaseContext databaseContext;

        public DatabaseAccessManager()
        {
        }


        ~DatabaseAccessManager()
        {
            this.databaseContext.Dispose();
        }

        /// <summary>
        /// Occurs when the user change the database settings.
        /// </summary>
        public event EventHandler<DatabaseAccessEventArgs> DatabaseAccessEvent;

        public string DatabaseName
        {
            get
            {
                return this.databaseContext.Database.Connection.DataSource;
            }
        }

        public bool IsDatabaseAvailable()
        {
            try
            {
                this.databaseContext.Database.Connection.Open();
                this.databaseContext.Database.Connection.Close();
                this.OnDatabaseAccessEvent(this, new DatabaseAccessEventArgs(this.databaseContext.Database.Connection.DataSource, DatabaseAvailability.Available));
            }
            catch (Exception)
            {
                //No database...
                this.OnDatabaseAccessEvent(this, new DatabaseAccessEventArgs(this.databaseContext.Database.Connection.DataSource, DatabaseAvailability.NotAvailable));
                return false;
            }

            return true;
        }

        public void BuildDatabaseContext(string databaseName, string serverName)
        {
            string sqlConnectionString = this.GetSqlConnectionString(databaseName, serverName);
            this.databaseContext = new SoftwareVersionsDatabaseContext(sqlConnectionString);
        }

        protected void OnDatabaseAccessEvent(object sender, DatabaseAccessEventArgs eventArgs)
        {
            if (this.DatabaseAccessEvent != null)
                this.DatabaseAccessEvent(this, eventArgs);
        }

        //Get Database information

        //Initial State Firmware
        public EntityFramework.InitialStateFirmware GetEntityFrameworkInitialStateFirmware(int initialStateFirmwareId)
        {
            return databaseContext.InitialStateFirmware.Where(x => x.Id == initialStateFirmwareId).FirstOrDefault();
        }

        public string GetTestCollectionName(int testCollectionId)
        {
            return databaseContext.TestCollection.Where(x => x.Id == testCollectionId).FirstOrDefault().Name;
        }

        public string GetSoftwareVersionsName(int softwareVersionsId)
        {
            return databaseContext.SoftwareVersions.Where(x => x.Id == softwareVersionsId).FirstOrDefault().Software;
        }

        public string GetDriveParameterID(int driveParameterId)
        {
            return databaseContext.DriveParameterFile.Where(x => x.Id == driveParameterId).FirstOrDefault().Name;
        }

        public string GetConfigurationParameterID(int configurationParameterId)
        {
            return databaseContext.ConfigurationParameterFile.Where(x => x.Id == configurationParameterId).FirstOrDefault().Name;
        }

        //Test Collection Result
        public EntityFramework.TestCollectionResult GetEntityFrameworkTestCollectionResult(int testCollectionResultId)
        {
            return databaseContext.TestCollectionResult.Where(x => x.Id == testCollectionResultId).FirstOrDefault();
        }

        //Test Result
        public EntityFramework.TestResult GetEntityFrameworkTestResult(int testResultId)
        {
            return databaseContext.TestResult.Where(x => x.Id == testResultId).FirstOrDefault();
        }

        public short GetTestVersion(int testVersionId)
        {
            return databaseContext.TestVersion.Where(x => x.Id == testVersionId).FirstOrDefault().Number;
        }

        public string GetTestName(int testVersionId)
        {
            int testInformationId = (int)databaseContext.TestVersion.Where(x => x.Id == testVersionId).FirstOrDefault().TestInformation_Id;
            return databaseContext.TestInformation.Where(x => x.Id == testInformationId).FirstOrDefault().Name;
        }

        //Valve Hardware
        public string GetValveSerieEnum(int valveHardwareId)
        {
            int valveSerieId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ValveSerie_Id;
            return databaseContext.ValveSerie.Where(x => x.Id == valveSerieId).FirstOrDefault().Enum;
        }

        public string GetInterfaceTypeEnum(int valveHardwareId)
        {
            int interfaceTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceType_Id;
            return databaseContext.InterfaceType.Where(x => x.Id == interfaceTypeId).FirstOrDefault().Enum;
        }

        public string GetControllerTypeEnum(int valveHardwareId)
        {
            int controllerTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerType_Id;
            return databaseContext.ControllerType.Where(x => x.Id == controllerTypeId).FirstOrDefault().Enum;
        }

        public string GetOptionTypeEnum(int valveHardwareId)
        {
            int optionTypeId = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().OptionType_Id;
            return databaseContext.OptionType.Where(x => x.Id == optionTypeId).FirstOrDefault().Enum;
        }

        public bool GetExternalIsolationValve(int valveHardwareId)
        {
            short externalIsolationValveOption;
            externalIsolationValveOption = databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ExternalIsolationValveOption;
            if (externalIsolationValveOption == 0)
                return false;
            else
                return true;
        }

        public string GetControllerHardwareVersionEnum(int valveHardwareId)
        {
            int controllerHardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerHardwareVersion_Id;
            return databaseContext.ControllerHardwareVersion.Where(x => x.Id == controllerHardwareVersion).FirstOrDefault().Enum;
        }

        public string GetInterfaceHardwareVersionEnum(int valveHardwareId)
        {
            int interfaceHardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceHardwareVersion_Id;
            return databaseContext.InterfaceHardwareVersion.Where(x => x.Id == interfaceHardwareVersion).FirstOrDefault().Enum;
        }

        public string GetControllerAssemblyVariantEnum(int valveHardwareId)
        {
            int controllerAssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().ControllerAssemblyVariant_Id;
            return databaseContext.ControllerAssemblyVariant.Where(x => x.Id == controllerAssemblyVariant).FirstOrDefault().Enum;
        }

        public string GetInterfaceAssemblyVariantEnum(int valveHardwareId)
        {
            int interfaceAssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().InterfaceAssemblyVariant_Id;
            return databaseContext.InterfaceAssemblyVariant.Where(x => x.Id == interfaceAssemblyVariant).FirstOrDefault().Enum;
        }

        public string GetModule1TypeEnum(int valveHardwareId)
        {
            int module1Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1Type_Id;
            return databaseContext.Module1Type.Where(x => x.Id == module1Type).FirstOrDefault().Enum;
        }

        public string GetModule1HardwareVersionEnum(int valveHardwareId)
        {
            int module1HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1HardwareVersion_Id;
            return databaseContext.Module1HardwareVersion.Where(x => x.Id == module1HardwareVersion).FirstOrDefault().Enum;
        }

        public string GetModule1AssemblyVariantEnum(int valveHardwareId)
        {
            int module1AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module1AssemblyVariant_Id;
            return databaseContext.Module1AssemblyVariant.Where(x => x.Id == module1AssemblyVariant).FirstOrDefault().Enum;
        }

        public string GetModule2TypeEnum(int valveHardwareId)
        {
            int module2Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2Type_Id;
            return databaseContext.Module2Type.Where(x => x.Id == module2Type).FirstOrDefault().Enum;
        }

        public string GetModule2HardwareVersionEnum(int valveHardwareId)
        {
            int module2HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2HardwareVersion_Id;
            return databaseContext.Module2HardwareVersion.Where(x => x.Id == module2HardwareVersion).FirstOrDefault().Enum;
        }

        public string GetModule2AssemblyVariantEnum(int valveHardwareId)
        {
            int module2AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module2AssemblyVariant_Id;
            return databaseContext.Module2AssemblyVariant.Where(x => x.Id == module2AssemblyVariant).FirstOrDefault().Enum;
        }

        public string GetModule3TypeEnum(int valveHardwareId)
        {
            int module3Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3Type_Id;
            return databaseContext.Module3Type.Where(x => x.Id == module3Type).FirstOrDefault().Enum;
        }

        public string GetModule3HardwareVersionEnum(int valveHardwareId)
        {
            int module3HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3HardwareVersion_Id;
            return databaseContext.Module3HardwareVersion.Where(x => x.Id == module3HardwareVersion).FirstOrDefault().Enum;
        }

        public string GetModule3AssemblyVariantEnum(int valveHardwareId)
        {
            int module3AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module3AssemblyVariant_Id;
            return databaseContext.Module3AssemblyVariant.Where(x => x.Id == module3AssemblyVariant).FirstOrDefault().Enum;
        }

        public string GetModule4TypeEnum(int valveHardwareId)
        {
            int module4Type = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4Type_Id;
            return databaseContext.Module4Type.Where(x => x.Id == module4Type).FirstOrDefault().Enum;
        }

        public string GetModule4HardwareVersionEnum(int valveHardwareId)
        {
            int module4HardwareVersion = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4HardwareVersion_Id;
            return databaseContext.Module4HardwareVersion.Where(x => x.Id == module4HardwareVersion).FirstOrDefault().Enum;
        }

        public string GetModule4AssemblyVariantEnum(int valveHardwareId)
        {
            int module4AssemblyVariant = (int)databaseContext.ValveHardware.Where(x => x.Id == valveHardwareId).FirstOrDefault().Module4AssemblyVariant_Id;
            return databaseContext.Module4AssemblyVariant.Where(x => x.Id == module4AssemblyVariant).FirstOrDefault().Enum;
        }

        private string GetSqlConnectionString(string databaseName, string serverName)
        {
            // Initialize the connection string builder for the
            // underlying provider.
            SqlConnectionStringBuilder sqlBuilder = new SqlConnectionStringBuilder();

            // Set the properties for the data source.
            sqlBuilder.DataSource = serverName;
            sqlBuilder.InitialCatalog = databaseName;
            sqlBuilder.IntegratedSecurity = true;
            sqlBuilder.MultipleActiveResultSets = true;

            // Build the SqlConnection connection string.
            return sqlBuilder.ToString();
        }
    }
}
