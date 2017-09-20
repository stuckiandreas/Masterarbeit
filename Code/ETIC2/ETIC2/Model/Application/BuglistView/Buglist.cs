//-----------------------------------------------------------------------
// <copyright file="Buglist.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.BuglistView
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class Buglist
    {
        public int Id;

        public string FailureType;

        public string StatusType;

        public string ControllerType;

        public string HardwareIdentificationLevel1;

        public string HardwareIdentificationLevel2;

        public string Bug;

        public string Comment;

        public string Priority;

        public DateTime DateFound;

        public DateTime? DateFixed;

        public Buglist()
        {
        }
    }
}
