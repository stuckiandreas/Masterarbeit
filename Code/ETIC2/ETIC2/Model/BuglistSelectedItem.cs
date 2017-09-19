//-----------------------------------------------------------------------
// <copyright file="BuglistSelectedItem.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model
{
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Get, add or delete selection item from the buglist view in the database
    /// Attention: Controller Type is not editable (used from test information)
    /// </summary>
    public class BuglistSelectedItem : DatabaseAccessManager
    {
        public BuglistSelectedItem()
        {
        }

        public List<Application.BuglistView.ControllerType> GetControllerTypes()
        {
            List<EntityFramework.ControllerType> controllerTypesDatabase = databaseContext.ControllerType.ToList();
            List<Application.BuglistView.ControllerType> controllerTypes = new List<Application.BuglistView.ControllerType>();
            Application.BuglistView.ControllerType emptyConrollerType;

            foreach (var controllerTypeDatabase in controllerTypesDatabase)
            {
                emptyConrollerType = new Application.BuglistView.ControllerType() { Id = default(int), Name = default(string) };
                emptyConrollerType.Id = controllerTypeDatabase.Id;
                emptyConrollerType.Name = controllerTypeDatabase.Enum;

                controllerTypes.Add(emptyConrollerType);
            }

            return controllerTypes;
        }

        public List<Application.BuglistView.FailureType> GetFailureTypes()
        {
            List<EntityFramework.FailureType> failureTypesDatabase = databaseContext.FailureType.ToList();
            List<Application.BuglistView.FailureType> failureTypes = new List<Application.BuglistView.FailureType>();
            Application.BuglistView.FailureType emptyFailureType;

            foreach (var failureTypeDatabase in failureTypesDatabase)
            {
                emptyFailureType = new Application.BuglistView.FailureType() { Id = default(int), Name = default(string) };
                emptyFailureType.Id = failureTypeDatabase.Id;
                emptyFailureType.Name = failureTypeDatabase.Name;

                failureTypes.Add(emptyFailureType);
            }

            return failureTypes;
        }

        public List<Application.BuglistView.HardwareIdentificationLevel1> GetHardwareIdentificationLevels1()
        {
            List<EntityFramework.HardwareIdentificationLevel1> hardwareIdentificationLevels1Database = databaseContext.HardwareIdentificationLevel1.ToList();
            List<Application.BuglistView.HardwareIdentificationLevel1> hardwareIdentificationLevels1 = new List<Application.BuglistView.HardwareIdentificationLevel1>();
            Application.BuglistView.HardwareIdentificationLevel1 emptyHardwareIdentificationLevel1;

            foreach (var hardwareIdentificationLevel1 in hardwareIdentificationLevels1Database)
            {
                emptyHardwareIdentificationLevel1 = new Application.BuglistView.HardwareIdentificationLevel1() { Id = default(int), Name = default(string) };
                emptyHardwareIdentificationLevel1.Id = hardwareIdentificationLevel1.Id;
                emptyHardwareIdentificationLevel1.Name = hardwareIdentificationLevel1.Name;

                hardwareIdentificationLevels1.Add(emptyHardwareIdentificationLevel1);
            }

            return hardwareIdentificationLevels1;
        }

        public List<Application.BuglistView.HardwareIdentificationLevel2> GetHardwareIdentificationLevels2()
        {
            List<EntityFramework.HardwareIdentificationLevel2> hardwareIdentificationLevels2Database = databaseContext.HardwareIdentificationLevel2.ToList();
            List<Application.BuglistView.HardwareIdentificationLevel2> hardwareIdentificationLevels2 = new List<Application.BuglistView.HardwareIdentificationLevel2>();
            Application.BuglistView.HardwareIdentificationLevel2 emptyHardwareIdentificationLevel2;

            foreach (var hardwareIdentificationLevel2 in hardwareIdentificationLevels2Database)
            {
                emptyHardwareIdentificationLevel2 = new Application.BuglistView.HardwareIdentificationLevel2() { Id = default(int), Name = default(string) };
                emptyHardwareIdentificationLevel2.Id = hardwareIdentificationLevel2.Id;
                emptyHardwareIdentificationLevel2.Name = hardwareIdentificationLevel2.Name;

                hardwareIdentificationLevels2.Add(emptyHardwareIdentificationLevel2);
            }

            return hardwareIdentificationLevels2;
        }

        public List<Application.BuglistView.StatusType> GetStatusTypes()
        {
            List<EntityFramework.StatusType> statusTypesDatabase = databaseContext.StatusType.ToList();
            List<Application.BuglistView.StatusType> statusTypes = new List<Application.BuglistView.StatusType>();
            Application.BuglistView.StatusType emptyStatusType;

            foreach (var statusTypeDatabase in statusTypesDatabase)
            {
                emptyStatusType = new Application.BuglistView.StatusType() { Id = default(int), Name = default(string) };
                emptyStatusType.Id = statusTypeDatabase.Id;
                emptyStatusType.Name = statusTypeDatabase.Name;

                statusTypes.Add(emptyStatusType);
            }

            return statusTypes;
        }

        public List<Application.BuglistView.Priority> GetPriorities()
        {
            List<EntityFramework.Priority> prioritiesDatabase = databaseContext.Priority.ToList();
            List<Application.BuglistView.Priority> priorities = new List<Application.BuglistView.Priority>();
            Application.BuglistView.Priority emptyPriority;

            foreach (var priorityDatabase in prioritiesDatabase)
            {
                emptyPriority = new Application.BuglistView.Priority() { Id = default(int), Name = default(string) };
                emptyPriority.Id = priorityDatabase.Id;
                emptyPriority.Name = priorityDatabase.Name;

                priorities.Add(emptyPriority);
            }

            return priorities;
        }

        public void AddFailureType(string failureTypeName)
        {
            databaseContext.FailureType.Add(new EntityFramework.FailureType { Name = failureTypeName });
            databaseContext.SaveChanges();
        }

        public void AddStatusType(string statusTypeName)
        {
            databaseContext.StatusType.Add(new EntityFramework.StatusType { Name = statusTypeName });
            databaseContext.SaveChanges();
        }

        public void AddPriority(string priorityName)
        {
            databaseContext.Priority.Add(new EntityFramework.Priority { Name = priorityName });
            databaseContext.SaveChanges();
        }

        public void AddHardwareIdentificationLevel1(string hardwareIdentificationLevel1Name)
        {
            databaseContext.HardwareIdentificationLevel1.Add(new EntityFramework.HardwareIdentificationLevel1 { Name = hardwareIdentificationLevel1Name });
            databaseContext.SaveChanges();
        }

        public void AddHardwareIdentificationLevel2(string hardwareIdentificationLevel2Name)
        {
            databaseContext.HardwareIdentificationLevel2.Add(new EntityFramework.HardwareIdentificationLevel2 { Name = hardwareIdentificationLevel2Name });
            databaseContext.SaveChanges();
        }

        public void EditFailureType(int failureTypeId, string failureTypeName)
        {
            EntityFramework.FailureType failureTypeToUpdate = databaseContext.FailureType.Where(x => x.Id == failureTypeId).FirstOrDefault();

            if (failureTypeToUpdate != null) failureTypeToUpdate.Name = failureTypeName;

            databaseContext.SaveChanges();
        }

        public void EditPriority(int priorityId, string priorityName)
        {
            EntityFramework.Priority priorityToUpdate = databaseContext.Priority.Where(x => x.Id == priorityId).FirstOrDefault();

            if (priorityToUpdate != null) priorityToUpdate.Name = priorityName;

            databaseContext.SaveChanges();
        }

        public void EditStatusType(int statusTypeId, string statusTypeName)
        {
            EntityFramework.StatusType statusTypeToUpdate = databaseContext.StatusType.Where(x => x.Id == statusTypeId).FirstOrDefault();

            if (statusTypeToUpdate != null) statusTypeToUpdate.Name = statusTypeName;

            databaseContext.SaveChanges();
        }

        public void EditHardwareIdentificationLevelel1(int hardwareIdentificationLevel1Id, string hardwareIdentificationLevel1Name)
        {
            EntityFramework.HardwareIdentificationLevel1 hardwareIdentificationLevel1ToUpdate =
                databaseContext.HardwareIdentificationLevel1.Where(x => x.Id == hardwareIdentificationLevel1Id).FirstOrDefault();

            if (hardwareIdentificationLevel1ToUpdate != null) hardwareIdentificationLevel1ToUpdate.Name = hardwareIdentificationLevel1Name;

            databaseContext.SaveChanges();
        }

        public void EditHardwareIdentificationLevelel2(int hardwareIdentificationLevel2Id, string hardwareIdentificationLevel2Name)
        {
            EntityFramework.HardwareIdentificationLevel2 hardwareIdentificationLevel2ToUpdate =
                databaseContext.HardwareIdentificationLevel2.Where(x => x.Id == hardwareIdentificationLevel2Id).FirstOrDefault();

            if (hardwareIdentificationLevel2ToUpdate != null) hardwareIdentificationLevel2ToUpdate.Name = hardwareIdentificationLevel2Name;

            databaseContext.SaveChanges();
        }

        public void DeleteFailureType(int failureTypeId)
        {
            EntityFramework.FailureType failureType = databaseContext.FailureType.Where(x => x.Id == failureTypeId).FirstOrDefault();

            if (failureType != null)
            {
                databaseContext.FailureType.Remove(failureType);
                databaseContext.SaveChanges();
            }
        }

        public void DeletePriority(int priorityId)
        {
            EntityFramework.Priority priority = databaseContext.Priority.Where(x => x.Id == priorityId).FirstOrDefault();

            if (priority != null)
            {
                databaseContext.Priority.Remove(priority);
                databaseContext.SaveChanges();
            }
        }

        public void DeleteStatusType(int statusTypeId)
        {
            EntityFramework.StatusType statusType = databaseContext.StatusType.Where(x => x.Id == statusTypeId).FirstOrDefault();

            if (statusType != null)
            {
                databaseContext.StatusType.Remove(statusType);
                databaseContext.SaveChanges();
            }
        }

        public void DeleteHardwareIdentificationLevel1(int hardwareIdentificationLevel1Id)
        {
            EntityFramework.HardwareIdentificationLevel1 hardwareIdentificationLevel1 = 
                databaseContext.HardwareIdentificationLevel1.Where(x => x.Id == hardwareIdentificationLevel1Id).FirstOrDefault();

            if (hardwareIdentificationLevel1 != null)
            {
                databaseContext.HardwareIdentificationLevel1.Remove(hardwareIdentificationLevel1);
                databaseContext.SaveChanges();
            }
        }

        public void DeleteHardwareIdentificationLevel2(int hardwareIdentificationLevel2Id)
        {
            EntityFramework.HardwareIdentificationLevel2 hardwareIdentificationLevel2 =
                databaseContext.HardwareIdentificationLevel2.Where(x => x.Id == hardwareIdentificationLevel2Id).FirstOrDefault();

            if (hardwareIdentificationLevel2 != null)
            {
                databaseContext.HardwareIdentificationLevel2.Remove(hardwareIdentificationLevel2);
                databaseContext.SaveChanges();
            }
        }
    }
}
