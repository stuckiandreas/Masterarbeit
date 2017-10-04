//-----------------------------------------------------------------------
// <copyright file="BuglistItem.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using EntityFramework;

    /// <summary>
    /// Get, set or delete the buglist item in the database.
    /// </summary>
    /// <seealso cref="ETIC2.Model.DatabaseAccessManager" />
    public class BuglistItem : DatabaseAccessManager
    {
        public BuglistItem()
        {
        }

        /// <summary>
        /// Gets the application buglist item. (based on the bug and date found information)
        /// </summary>
        /// <param name="bug">The bug.</param>
        /// <param name="dateFound">The date found.</param>
        /// <returns>Buglist item which has the same Bug description and date found entry</returns>
        public Application.BuglistView.Buglist GetApplicationBuglistItem(string bug, DateTime dateFound)
        {
            Application.BuglistView.Buglist emptyBuglist;
            EntityFramework.Buglist buglistItemDatabase = databaseContext.Buglist.Where(x => x.Bug == bug && x.DateFound == dateFound).FirstOrDefault();

            emptyBuglist = new Application.BuglistView.Buglist()
            {
                Id = default(int), FailureType = default(string), StatusType = default(string), ControllerType = default(string), HardwareIdentificationLevel1 = default(string),
                HardwareIdentificationLevel2 = default(string), Bug = default(string), Comment = default(string), Priority = default(string), DateFound = default(DateTime), DateFixed = default(DateTime)
            };
            emptyBuglist.Id = buglistItemDatabase.Id;
            emptyBuglist.FailureType = this.GetFailureType((int)buglistItemDatabase.FailureType_Id);
            emptyBuglist.StatusType = this.GetStatusType((int)buglistItemDatabase.StatusType_Id);
            emptyBuglist.ControllerType = this.GetControllerTypeEnum((int)buglistItemDatabase.ControllerType_Id);
            emptyBuglist.HardwareIdentificationLevel1 = this.GetHardwareIdentificationLevel1((int)buglistItemDatabase.HardwareIdentificationLevel1_Id);
            emptyBuglist.HardwareIdentificationLevel2 = this.GetHardwareIdentificationLevel1((int)buglistItemDatabase.HardwareIdentificationLevel2_Id);
            emptyBuglist.Bug = buglistItemDatabase.Bug;
            emptyBuglist.Comment = buglistItemDatabase.Comment;
            emptyBuglist.Priority = this.GetPriority((int)buglistItemDatabase.Priority_Id);

            return emptyBuglist;
        }

        /// <summary>
        /// Gets the buglist in application datatype (without any references).
        /// </summary>
        /// <returns>Buglist (Application Datatype)</returns>
        public List<Application.BuglistView.Buglist> GetApplicationBuglist()
        {
            List<EntityFramework.Buglist> buglistDatabase = this.GetEntityFrameworkBuglist();
            List<Application.BuglistView.Buglist> buglist = new List<Application.BuglistView.Buglist>();
            Application.BuglistView.Buglist emptyBuglist;

            foreach (var buglistItemDatabase in buglistDatabase)
            {
                emptyBuglist = new Application.BuglistView.Buglist()
                {
                    Id = default(int), FailureType = default(string), StatusType = default(string), ControllerType = default(string), HardwareIdentificationLevel1 = default(string),
                    HardwareIdentificationLevel2 = default(string), Bug = default(string), Comment = default(string), Priority = default(string), DateFound = default(DateTime), DateFixed = default(DateTime)
                };
                emptyBuglist.Id = buglistItemDatabase.Id;
                emptyBuglist.FailureType = this.GetFailureType((int)buglistItemDatabase.FailureType_Id);
                emptyBuglist.StatusType = this.GetStatusType((int)buglistItemDatabase.StatusType_Id);
                emptyBuglist.ControllerType = this.GetControllerTyp((int)buglistItemDatabase.ControllerType_Id);
                emptyBuglist.HardwareIdentificationLevel1 = this.GetHardwareIdentificationLevel1((int)buglistItemDatabase.HardwareIdentificationLevel1_Id);
                emptyBuglist.HardwareIdentificationLevel2 = this.GetHardwareIdentificationLevel2((int)buglistItemDatabase.HardwareIdentificationLevel2_Id);
                emptyBuglist.Bug = buglistItemDatabase.Bug;
                emptyBuglist.Comment = buglistItemDatabase.Comment;
                emptyBuglist.Priority = this.GetPriority((int)buglistItemDatabase.Priority_Id);
                emptyBuglist.DateFound = buglistItemDatabase.DateFound;
                emptyBuglist.DateFixed = buglistItemDatabase.DateFixed;

                buglist.Add(emptyBuglist);
            }

            return buglist;
        }

        /// <summary>
        /// Adds a buglist item in the database.
        /// </summary>
        /// <param name="buglistItem">The buglist item.</param>
        public void AddBuglistItem(Application.BuglistView.Buglist buglistItem)
        {
            EntityFramework.Buglist buglistDatabase = new Buglist();
            buglistDatabase.Id = buglistItem.Id;
            buglistDatabase.FailureType_Id = this.GetFailureTypId(buglistItem.FailureType);
            buglistDatabase.StatusType_Id = this.GetStatusTypeId(buglistItem.StatusType);
            buglistDatabase.ControllerType_Id = this.GetControllerTypId(buglistItem.ControllerType);
            buglistDatabase.HardwareIdentificationLevel1_Id = this.GetHardwareIdentificationLevel1Id(buglistItem.HardwareIdentificationLevel1);
            buglistDatabase.HardwareIdentificationLevel2_Id = this.GetHardwareIdentificationLevel2Id(buglistItem.HardwareIdentificationLevel2);
            buglistDatabase.Bug = buglistItem.Bug;
            buglistDatabase.Comment = buglistItem.Comment;
            buglistDatabase.Priority_Id = this.GetPriorityId(buglistItem.Priority);
            buglistDatabase.DateFound = buglistItem.DateFound;
            buglistDatabase.DateFixed = buglistItem.DateFixed;

            databaseContext.Buglist.Add(buglistDatabase);
            databaseContext.SaveChanges();
        }

        /// <summary>
        /// Deletes a buglist item from the database.
        /// </summary>
        /// <param name="buglistId">The buglist identifier.</param>
        public void DeleteBuglistItem(int buglistId)
        {
            EntityFramework.Buglist buglistItemDatabase = databaseContext.Buglist.Where(x => x.Id == buglistId).FirstOrDefault();

            if (buglistItemDatabase != null)
            {
                databaseContext.Buglist.Remove(buglistItemDatabase);
                databaseContext.SaveChanges();
            }
        }

        /// <summary>
        /// Updates the buglist item in database.
        /// </summary>
        /// <param name="buglistItem">The buglist item.</param>
        public void UpdateBuglistItemInDatabase(Application.BuglistView.Buglist buglistItem)
        {
            Buglist buglistItemToUpdate = databaseContext.Buglist.Where(x => x.Id == buglistItem.Id).FirstOrDefault();

            if (buglistItemToUpdate != null)
            {
                buglistItemToUpdate.FailureType_Id = this.GetFailureTypId(buglistItem.FailureType);
                buglistItemToUpdate.StatusType_Id = this.GetStatusTypeId(buglistItem.StatusType);
                buglistItemToUpdate.ControllerType_Id = this.GetStatusTypeId(buglistItem.StatusType);
                buglistItemToUpdate.HardwareIdentificationLevel1_Id = this.GetHardwareIdentificationLevel1Id(buglistItem.HardwareIdentificationLevel1);
                buglistItemToUpdate.HardwareIdentificationLevel2_Id = this.GetHardwareIdentificationLevel2Id(buglistItem.HardwareIdentificationLevel2);
                buglistItemToUpdate.Bug = buglistItem.Bug;
                buglistItemToUpdate.Comment = buglistItem.Comment;
                buglistItemToUpdate.Priority_Id = this.GetPriorityId(buglistItem.Priority);
                buglistItemToUpdate.DateFound = buglistItem.DateFound;
                buglistItemToUpdate.DateFixed = buglistItem.DateFixed;
            }

            databaseContext.SaveChanges();
        }

        private List<EntityFramework.Buglist> GetEntityFrameworkBuglist()
        {
            return databaseContext.Buglist.ToList();
        }

        private string GetFailureType(int failureTypeId)
        {
            return databaseContext.FailureType.Where(x => x.Id == failureTypeId).FirstOrDefault().Name;
        }

        private int GetFailureTypId(string failureTypeName)
        {
            return databaseContext.FailureType.Where(x => x.Name == failureTypeName).FirstOrDefault().Id;
        }

        private string GetControllerTyp(int controllerTypeId)
        {
            return databaseContext.ControllerType.Where(x => x.Id == controllerTypeId).FirstOrDefault().Enum;
        }

        private int GetControllerTypId(string controllerTypeName)
        {
            return databaseContext.ControllerType.Where(x => x.Enum == controllerTypeName).FirstOrDefault().Id;
        }

        private string GetStatusType(int statusTypeId)
        {
            return databaseContext.StatusType.Where(x => x.Id == statusTypeId).FirstOrDefault().Name;
        }

        private int GetStatusTypeId(string statusTypeName)
        {
            return databaseContext.StatusType.Where(x => x.Name == statusTypeName).FirstOrDefault().Id;
        }

        private string GetHardwareIdentificationLevel1(int hardwareIdentificationLevel1Id)
        {
            return databaseContext.HardwareIdentificationLevel1.Where(x => x.Id == hardwareIdentificationLevel1Id).FirstOrDefault().Name;
        }

        private int GetHardwareIdentificationLevel1Id(string hardwareIdentificationLevel1Name)
        {
            return databaseContext.HardwareIdentificationLevel1.Where(x => x.Name == hardwareIdentificationLevel1Name).FirstOrDefault().Id;
        }

        private string GetHardwareIdentificationLevel2(int hardwareIdentificationLevel2Id)
        {
            return databaseContext.HardwareIdentificationLevel2.Where(x => x.Id == hardwareIdentificationLevel2Id).FirstOrDefault().Name;
        }

        private int GetHardwareIdentificationLevel2Id(string hardwareIdentificationLevel2Name)
        {
            return databaseContext.HardwareIdentificationLevel2.Where(x => x.Name == hardwareIdentificationLevel2Name).FirstOrDefault().Id;
        }

        private string GetPriority(int priorityId)
        {
            return databaseContext.Priority.Where(x => x.Id == priorityId).FirstOrDefault().Name;
        }

        private int GetPriorityId(string priorityName)
        {
            return databaseContext.Priority.Where(x => x.Name == priorityName).FirstOrDefault().Id;
        }
    }
}
