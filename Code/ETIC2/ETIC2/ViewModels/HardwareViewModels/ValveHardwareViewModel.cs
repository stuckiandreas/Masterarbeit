//-----------------------------------------------------------------------
// <copyright file="ValveHardwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.HardwareViewModels
{
    using Events;
    using Model.Application.HardwareView;
    using System.Collections.ObjectModel;
    
    /// <summary>
    /// First Level of the Hardware Master Detail View. Shows the ValveHardware entrys.
    /// </summary>
    public class ValveHardwareViewModel : BasisViewModel
    {
        public ValveHardwareViewModel(ViewModelEvents viewModelEvents, ValveHardware valveHardware, ObservableCollection<TestCollectionResultAndInitialStateFirmwareViewModel> testCollectionResultAndInitialStateFirmwareViewModelList)
            : base(viewModelEvents)
        {
            this.ValveSerie = valveHardware.ValveSerie;
            this.InterfaceType = valveHardware.InterfaceType;
            this.ControllerType = valveHardware.ControllerType;
            this.OptionType = valveHardware.OptionType;
            this.ExternalIsolationValve = valveHardware.ExternalIsolationValve;
            this.ControllerHardwareVersion = valveHardware.ControllerHardwareVersion;
            this.InterfaceHardwareVersion = valveHardware.InterfaceHardwareVersion;
            this.ControllerAssemblyVariant = valveHardware.ControllerAssemblyVariant;
            this.InterfaceAssemblyVariant = valveHardware.InterfaceAssemblyVariant;
            this.Module1Type = valveHardware.Module1Type;
            this.Module1HardwareVersion = valveHardware.Module1HardwareVersion;
            this.Module1AssemblyVariant = valveHardware.Module1AssemblyVariant;
            this.Module2Type = valveHardware.Module2Type;
            this.Module2HardwareVersion = valveHardware.Module2HardwareVersion;
            this.Module2AssemblyVariant = valveHardware.Module2AssemblyVariant;
            this.Module3Type = valveHardware.Module3Type;
            this.Module3HardwareVersion = valveHardware.Module3HardwareVersion;
            this.Module3AssemblyVariant = valveHardware.Module3AssemblyVariant;
            this.Module4Type = valveHardware.Module4Type;
            this.Module4HardwareVersion = valveHardware.Module4HardwareVersion;
            this.Module4AssemblyVariant = valveHardware.Module4AssemblyVariant;
            this.TestCollectionResultAndInitialStateFirmwareViewModels = testCollectionResultAndInitialStateFirmwareViewModelList;
        }

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

        public ObservableCollection<TestCollectionResultAndInitialStateFirmwareViewModel> TestCollectionResultAndInitialStateFirmwareViewModels
        {
            get;
            private set;
        }
    }
}
