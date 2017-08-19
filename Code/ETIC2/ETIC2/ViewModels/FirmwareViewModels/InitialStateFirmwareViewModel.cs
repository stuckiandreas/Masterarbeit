//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.FirmwareViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using ETIC2.Model.Application.FirmwareView;
    using Events;

    /// <summary>
    /// First Level of the Firmware Master Detail View. Shows the initialStateFirmware entrys.
    /// </summary>
    public class InitialStateFirmwareViewModel : BasisViewModel
    {
        public InitialStateFirmwareViewModel(ViewModelEvents viewModelEvents, InitialStateFirmware initialStateFirmware, ObservableCollection<TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModelList)
            : base(viewModelEvents)
        {
            this.ValveFirmware = initialStateFirmware.ValveFirmware;
            this.ValveFirmwareReleaseTime = initialStateFirmware.ValveFirmwareReleaseTime;
            this.MotionControllerFirmware = initialStateFirmware.MotionControllerFirmware;
            this.InterfaceFirmware = initialStateFirmware.InterfaceFirmware;
            this.DriveParameterID = initialStateFirmware.DriveParameterID;
            this.ConfigurationParameterID = initialStateFirmware.ConfigurationParameterID;
            this.TestCollection = initialStateFirmware.TestCollection;
            this.TestCollectionResultWithValveHardwareViewModels = testCollectionResultWithValveHardwareViewModelList;
        }

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

        public ObservableCollection<TestCollectionResultWithValveHardwareViewModel> TestCollectionResultWithValveHardwareViewModels
        { 
            get; 
            private set; 
        }
    }
}
