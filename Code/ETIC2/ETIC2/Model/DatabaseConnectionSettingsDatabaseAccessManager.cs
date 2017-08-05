//-----------------------------------------------------------------------
// <copyright file="DatabaseConnectionSettingsDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    /// <summary>
    /// Class is used to ask and set the database version.
    /// </summary>
    /// <seealso cref="DatabaseAccessManager" />
    public class DatabaseConnectionSettingsDatabaseAccessManager : DatabaseAccessManager
    {
        /// <summary>
        /// Database version string. Signal word in config data.
        /// </summary>
        public string DatabaseVersionString = "database version";

        public DatabaseConnectionSettingsDatabaseAccessManager()
        {
        }

        public List<ConfigData> GetConfigData()
        {
            return databaseContext.ConfigData.ToList();
        }

        public void AddConfigData(string configString, int configNumber)
        {
            databaseContext.ConfigData.Add(new ConfigData { ConfigString = configString, ConfigNumber = configNumber });
            databaseContext.SaveChanges();
        }

        public int GetDatabaseVersion()
        {
            List<ConfigData> configDataList = this.GetConfigData();

            foreach (var configData in configDataList)
            {
                if (configData.ConfigString == this.DatabaseVersionString)
                    return (int)configData.ConfigNumber;
            }

            //first database version starts by 101
            return 0;
        }

        public void EditDatabaseVersion(int configNumber)
        {
            List<ConfigData> configDataList = this.GetConfigData();

            foreach (var configData in configDataList)
            {
                if (configData.ConfigString == this.DatabaseVersionString)
                {
                    configData.ConfigNumber = configNumber;
                    databaseContext.SaveChanges();
                }
            }
        }
    }
}
