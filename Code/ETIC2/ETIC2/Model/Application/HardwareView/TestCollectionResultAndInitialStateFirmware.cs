//-----------------------------------------------------------------------
// <copyright file="TestCollectionResultAndInitialStateFirmware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.HardwareView
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class TestCollectionResultAndInitialStateFirmware
    {
        //Test Collection Result
        public int Id;

        public DateTime ExecutionTime;

        public string UserName;

        public string AbortType;

        public short CountErrorTest;

        //Initial State Firmware
        public string ValveFirmware;

        public DateTime ValveFirmwareReleaseTime;

        public string MotionControllerFirmware;

        public string InterfaceFirmware;

        public string DriveParameterID;

        public string ConfigurationParameterID;

        public string TestCollection;

        public TestCollectionResultAndInitialStateFirmware()
        {
        }
    }
}
