//-----------------------------------------------------------------------
// <copyright file="DatabaseConnectionSettingsDatabaseAccessManager.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Model
{
    using System.Collections.Generic;
    using System.Linq;
    using VersionManagement.Model.Database;

    public class DatabaseConnectionSettingsDatabaseAccessManager : DatabaseAccessManager
    {
        /// <summary>
        /// The database version string
        /// </summary>
        public string DatabaseVersionString = "database version";

        public DatabaseConnectionSettingsDatabaseAccessManager()
        {
        }

        public List<Users> GetUsers()
        {
            return databaseContext.Users.ToList();
        }

        public List<ConfigData> GetConfigData()
        {
            return databaseContext.ConfigData.ToList();
        }

        public void AddUsers(string userName, int userRight)
        {
            databaseContext.Users.Add(new Users { UserName = userName, UserRight = userRight });
            databaseContext.SaveChanges();
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
