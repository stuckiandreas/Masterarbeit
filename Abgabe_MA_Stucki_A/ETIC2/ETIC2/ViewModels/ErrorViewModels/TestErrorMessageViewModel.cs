//-----------------------------------------------------------------------
// <copyright file="TestErrorMessageViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.ErrorTopLevelViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using ETIC2.Events;

    /// <summary>
    /// Shows the TestErrorMessage entrys. Only a DataGrid View without any Levels
    /// </summary>
    public class TestErrorMessageViewModel : BasisViewModel
    {
        public TestErrorMessageViewModel(ViewModelEvents viewModelEvents, Model.Application.ErrorView.TestErrorMessage testErrorMessage)
            : base(viewModelEvents)
        {
            this.Description = testErrorMessage.Description;
            this.TestName = testErrorMessage.TestName;
            this.TestVersion = testErrorMessage.TestVersion;
            this.StartTime = testErrorMessage.StartTime;
            this.EndTime = testErrorMessage.EndTime;
            this.ValveFirmware = testErrorMessage.ValveFirmware;
            this.ValveFirmwareReleaseTime = testErrorMessage.ValveFirmwareReleaseTime;
            this.MotionControllerFirmware = testErrorMessage.MotionControllerFirmware;
            this.InterfaceFirmware = testErrorMessage.InterfaceFirmware;
            this.DriveParameterID = testErrorMessage.DriveParameterID;
            this.ConfigurationParameterID = testErrorMessage.ConfigurationParameterID;
            this.TestCollection = testErrorMessage.TestCollection;
            this.ValveSerie = testErrorMessage.ValveSerie;
            this.InterfaceType = testErrorMessage.InterfaceType;
            this.ControllerType = testErrorMessage.ControllerType;
            this.OptionType = testErrorMessage.OptionType;
            this.ExternalIsolationValve = testErrorMessage.ExternalIsolationValve;
            this.ControllerHardwareVersion = testErrorMessage.ControllerHardwareVersion;
            this.InterfaceHardwareVersion = testErrorMessage.InterfaceHardwareVersion;
            this.ControllerAssemblyVariant = testErrorMessage.ControllerAssemblyVariant;
            this.InterfaceAssemblyVariant = testErrorMessage.InterfaceAssemblyVariant;
            this.Module1Type = testErrorMessage.Module1Type;
            this.Module1HardwareVersion = testErrorMessage.Module1HardwareVersion;
            this.Module1AssemblyVariant = testErrorMessage.Module1AssemblyVariant;
            this.Module2Type = testErrorMessage.Module2Type;
            this.Module2HardwareVersion = testErrorMessage.Module2HardwareVersion;
            this.Module2AssemblyVariant = testErrorMessage.Module2AssemblyVariant;
            this.Module3Type = testErrorMessage.Module3Type;
            this.Module3HardwareVersion = testErrorMessage.Module3HardwareVersion;
            this.Module3AssemblyVariant = testErrorMessage.Module3AssemblyVariant;
            this.Module4Type = testErrorMessage.Module4Type;
            this.Module4HardwareVersion = testErrorMessage.Module4HardwareVersion;
            this.Module4AssemblyVariant = testErrorMessage.Module4AssemblyVariant;
        }

        public string Description
        {
            get;
            set;
        }

        public string TestName
        {
            get;
            set;
        }

        public short TestVersion
        {
            get;
            set;
        }

        public DateTime StartTime
        {
            get;
            set;
        }

        public DateTime EndTime
        {
            get;
            set;
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
