//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application
{
    using System;

    public class InitialStateFirmware
    {
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
