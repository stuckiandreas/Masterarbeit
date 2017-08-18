//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.FirmwareTopLevel
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class InitialStateFirmware
    {
        public int Id;

        public string ValveFirmware;

        public DateTime ValveFirmwareReleaseTime;

        public string MotionControllerFirmware;

        public string InterfaceFirmware;

        public string DriveParameterID;

        public string ConfigurationParameterID;

        public string TestCollection;

        public InitialStateFirmware()
        {
        }
    }
}
