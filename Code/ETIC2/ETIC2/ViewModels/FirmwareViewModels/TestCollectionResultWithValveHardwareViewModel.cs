//-----------------------------------------------------------------------
// <copyright file="TestCollectionResultWithValveHardwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.FirmwareViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using ETIC2.Model.Application.FirmwareView;
    using Events;

    /// <summary>
    /// Second Level of the Firmware Master Detail View. Shows the TestCollectionResultWithValveHardware entrys.
    /// </summary>
    public class TestCollectionResultWithValveHardwareViewModel : BasisViewModel
    {
        public TestCollectionResultWithValveHardwareViewModel(ViewModelEvents viewModelEvents, Model.Application.FirmwareView.TestCollectionResultWithValveHardware testCollectionResultWithValveHardware, ObservableCollection<TestResultViewModel> testResultViewModelList)
            : base(viewModelEvents)
        {
            this.ExecutionTime = testCollectionResultWithValveHardware.ExecutionTime;
            this.UserName = testCollectionResultWithValveHardware.UserName;
            this.AbortType = testCollectionResultWithValveHardware.AbortType;
            this.CountErrorTest = testCollectionResultWithValveHardware.CountErrorTest;
            this.ValveSerie = testCollectionResultWithValveHardware.ValveSerie;
            this.InterfaceType = testCollectionResultWithValveHardware.InterfaceType;
            this.ControllerType = testCollectionResultWithValveHardware.ControllerType;
            this.OptionType = testCollectionResultWithValveHardware.OptionType;
            this.ExternalIsolationValve = testCollectionResultWithValveHardware.ExternalIsolationValve;
            this.ControllerHardwareVersion = testCollectionResultWithValveHardware.ControllerHardwareVersion;
            this.InterfaceHardwareVersion = testCollectionResultWithValveHardware.InterfaceHardwareVersion;
            this.ControllerAssemblyVariant = testCollectionResultWithValveHardware.ControllerAssemblyVariant;
            this.InterfaceAssemblyVariant = testCollectionResultWithValveHardware.InterfaceAssemblyVariant;
            this.Module1Type = testCollectionResultWithValveHardware.Module1Type;
            this.Module1HardwareVersion = testCollectionResultWithValveHardware.Module1HardwareVersion;
            this.Module1AssemblyVariant = testCollectionResultWithValveHardware.Module1AssemblyVariant;
            this.Module2Type = testCollectionResultWithValveHardware.Module2Type;
            this.Module2HardwareVersion = testCollectionResultWithValveHardware.Module2HardwareVersion;
            this.Module2AssemblyVariant = testCollectionResultWithValveHardware.Module2AssemblyVariant;
            this.Module3Type = testCollectionResultWithValveHardware.Module3Type;
            this.Module3HardwareVersion = testCollectionResultWithValveHardware.Module3HardwareVersion;
            this.Module3AssemblyVariant = testCollectionResultWithValveHardware.Module3AssemblyVariant;
            this.Module4Type = testCollectionResultWithValveHardware.Module4Type;
            this.Module4HardwareVersion = testCollectionResultWithValveHardware.Module4HardwareVersion;
            this.Module4AssemblyVariant = testCollectionResultWithValveHardware.Module4AssemblyVariant;
            this.TestResultViewModels = testResultViewModelList;
        }

        //TestCollectionResult
        public DateTime ExecutionTime
        {
            get;
            set;
        }

        public string UserName
        {
            get;
            set;
        }

        public string AbortType
        {
            get;
            set;
        }

        public short CountErrorTest
        {
            get;
            set;
        }

        //ValveHardware
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

        public ObservableCollection<TestResultViewModel> TestResultViewModels
        {
            get;
            private set;
        }
    }
}
