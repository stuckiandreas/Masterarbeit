//-----------------------------------------------------------------------
// <copyright file="TestCollectionResultWithValveHardware.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Model.Application.FirmwareView
{
    using System;

    /// <summary>
    /// Seperate Entity Framework Datatype from Application. Without references
    /// </summary>
    public class TestCollectionResultWithValveHardware
    {
        //TestCollectionResult
        public int Id;

        public DateTime ExecutionTime;

        public string UserName;

        public string AbortType;

        public short CountErrorTest;

        //Valve Hardware
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

        public TestCollectionResultWithValveHardware()
        {
        }
    }
}
