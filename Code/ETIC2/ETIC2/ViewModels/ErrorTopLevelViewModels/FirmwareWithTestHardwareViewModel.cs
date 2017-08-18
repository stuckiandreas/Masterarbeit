//-----------------------------------------------------------------------
// <copyright file="FirmwareWithTestHardwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ErrorTopLevelViewModels
{
    using System;
    using ETIC2.Events;

    /// <summary>
    /// Third Level of the Error Master Detail View. Shows the FirmwareWithTestHardware entrys.
    /// </summary>
    public class FirmwareWithTestHardwareViewModel : BasisViewModel
    {
        public FirmwareWithTestHardwareViewModel(ViewModelEvents viewModelEvents, Model.Application.ErrorTopLevel.FirmwareWithTestHardware firmwareWithTestHardware)
            : base(viewModelEvents)
        {
            this.ValveFirmware = firmwareWithTestHardware.ValveFirmware;
            this.ValveFirmwareReleaseTime = firmwareWithTestHardware.ValveFirmwareReleaseTime;
            this.MotionControllerFirmware = firmwareWithTestHardware.MotionControllerFirmware;
            this.InterfaceFirmware = firmwareWithTestHardware.InterfaceFirmware;
            this.DriveParameterID = firmwareWithTestHardware.DriveParameterID;
            this.ConfigurationParameterID = firmwareWithTestHardware.ConfigurationParameterID;
            this.TestCollection = firmwareWithTestHardware.TestCollection;
            this.ValveSerie = firmwareWithTestHardware.ValveSerie;
            this.InterfaceType = firmwareWithTestHardware.InterfaceType;
            this.ControllerType = firmwareWithTestHardware.ControllerType;
            this.OptionType = firmwareWithTestHardware.OptionType;
            this.ExternalIsolationValve = firmwareWithTestHardware.ExternalIsolationValve;
            this.ControllerHardwareVersion = firmwareWithTestHardware.ControllerHardwareVersion;
            this.InterfaceHardwareVersion = firmwareWithTestHardware.InterfaceHardwareVersion;
            this.ControllerAssemblyVariant = firmwareWithTestHardware.ControllerAssemblyVariant;
            this.InterfaceAssemblyVariant = firmwareWithTestHardware.InterfaceAssemblyVariant;
            this.Module1Type = firmwareWithTestHardware.Module1Type;
            this.Module1HardwareVersion = firmwareWithTestHardware.Module1HardwareVersion;
            this.Module1AssemblyVariant = firmwareWithTestHardware.Module1AssemblyVariant;
            this.Module2Type = firmwareWithTestHardware.Module2Type;
            this.Module2HardwareVersion = firmwareWithTestHardware.Module2HardwareVersion;
            this.Module2AssemblyVariant = firmwareWithTestHardware.Module2AssemblyVariant;
            this.Module3Type = firmwareWithTestHardware.Module3Type;
            this.Module3HardwareVersion = firmwareWithTestHardware.Module3HardwareVersion;
            this.Module3AssemblyVariant = firmwareWithTestHardware.Module3AssemblyVariant;
            this.Module4Type = firmwareWithTestHardware.Module4Type;
            this.Module4HardwareVersion = firmwareWithTestHardware.Module4HardwareVersion;
            this.Module4AssemblyVariant = firmwareWithTestHardware.Module4AssemblyVariant;
        }

        //Firmware
        public string ValveFirmware
        {
            get;
            set;
        }

        public DateTime ValveFirmwareReleaseTime
        {
            get;
            set;
        }

        public string MotionControllerFirmware
        {
            get;
            set;
        }

        public string InterfaceFirmware
        {
            get;
            set;
        }

        public string DriveParameterID
        {
            get;
            set;
        }

        public string ConfigurationParameterID
        {
            get;
            set;
        }

        public string TestCollection
        {
            get;
            set;
        }

        //Hardware 
        public string ValveSerie
        {
            get;
            set;
        }

        public string InterfaceType
        {
            get;
            set;
        }

        public string ControllerType
        {
            get;
            set;
        }

        public string OptionType
        {
            get;
            set;
        }

        public bool ExternalIsolationValve
        {
            get;
            set;
        }

        public string ControllerHardwareVersion
        {
            get;
            set;
        }

        public string InterfaceHardwareVersion
        {
            get;
            set;
        }

        public string ControllerAssemblyVariant
        {
            get;
            set;
        }

        public string InterfaceAssemblyVariant
        {
            get;
            set;
        }

        public string Module1Type
        {
            get;
            set;
        }

        public string Module1HardwareVersion
        {
            get;
            set;
        }

        public string Module1AssemblyVariant
        {
            get;
            set;
        }

        public string Module2Type
        {
            get;
            set;
        }

        public string Module2HardwareVersion
        {
            get;
            set;
        }

        public string Module2AssemblyVariant
        {
            get;
            set;
        }

        public string Module3Type
        {
            get;
            set;
        }

        public string Module3HardwareVersion
        {
            get;
            set;
        }

        public string Module3AssemblyVariant
        {
            get;
            set;
        }

        public string Module4Type
        {
            get;
            set;
        }

        public string Module4HardwareVersion
        {
            get;
            set;
        }

        public string Module4AssemblyVariant
        {
            get;
            set;
        }
    }
}
