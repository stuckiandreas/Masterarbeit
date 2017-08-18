//-----------------------------------------------------------------------
// <copyright file="FirmwareWithTestHardware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.ErrorTopLevel
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class FirmwareWithTestHardware
    {
        //Firmware
        public string ValveFirmware;

        public DateTime ValveFirmwareReleaseTime;

        public string MotionControllerFirmware;

        public string InterfaceFirmware;

        public string DriveParameterID;

        public string ConfigurationParameterID;

        public string TestCollection;

        //Hardware
        public string ValveSerie;

        public string InterfaceType;

        public string ControllerType;

        public string OptionType;

        public bool ExternalIsolationValve;

        public string ControllerHardwareVersion;

        public string InterfaceHardwareVersion;

        public string ControllerAssemblyVariant;

        public string InterfaceAssemblyVariant;

        public string Module1Type;

        public string Module1HardwareVersion;

        public string Module1AssemblyVariant;

        public string Module2Type;

        public string Module2HardwareVersion;

        public string Module2AssemblyVariant;

        public string Module3Type;

        public string Module3HardwareVersion;

        public string Module3AssemblyVariant;

        public string Module4Type;

        public string Module4HardwareVersion;

        public string Module4AssemblyVariant;

        public FirmwareWithTestHardware()
        {
        }
    }
}
