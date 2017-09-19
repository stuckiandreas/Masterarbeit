//-----------------------------------------------------------------------
// <copyright file="DatabaseItemViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.BuglistViewModels
{
    using System;
    using ETIC2.Model.Application.BuglistView;
    
    /// <summary>
    /// One buglist item that can be displayed in the database application.
    /// </summary>
    public class DatabaseItemViewModel
    {
        public DatabaseItemViewModel(Buglist buglistItem)
        {
            this.FailureType = buglistItem.FailureType;
            this.StatusType = buglistItem.StatusType;
            this.ControllerType = buglistItem.ControllerType;
            this.HardwareIdentificationLevel1 = buglistItem.HardwareIdentificationLevel1;
            this.HardwareIdentificationLevel2 = buglistItem.HardwareIdentificationLevel2;
            this.Bug = buglistItem.Bug;
            this.Comment = buglistItem.Comment;
            this.Priority = buglistItem.Priority;
            this.DateFound = buglistItem.DateFound;
            this.DateFixed = buglistItem.DateFixed;
        }

        public string FailureType
        {
            get;
            set;
        }

        public string StatusType
        {
            get;
            set;
        }

        public string ControllerType
        {
            get;
            set;
        }

        public string HardwareIdentificationLevel1
        {
            get;
            set;
        }

        public string HardwareIdentificationLevel2
        {
            get;
            set;
        }

        public string Bug
        {
            get;
            set;
        }

        public string Comment
        {
            get;
            set;
        }

        public string Priority
        {
            get;
            set;
        }

        public DateTime DateFound
        {
            get;
            set;
        }

        public DateTime DateFixed
        {
            get;
            set;
        }
    }
}
