//-----------------------------------------------------------------------
// <copyright file="InitialStateFirmwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using ETIC2.Model.Application;

    /// <summary>
    /// First Level of the Master Detail View. Listet the initialStateFirmware entrys.
    /// </summary>
    public class InitialStateFirmwareViewModel
    {
        public InitialStateFirmwareViewModel(InitialStateFirmware initialStateFirmware, List<TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModel)
        {
            this.ValveFirmware = initialStateFirmware.ValveFirmware;
            this.ValveFirmwareReleaseTime = initialStateFirmware.ValveFirmwareReleaseTime;
            this.MotionControllerFirmware = initialStateFirmware.MotionControllerFirmware;
            this.InterfaceFirmware = initialStateFirmware.InterfaceFirmware;
            this.DriveParameterID = initialStateFirmware.DriveParameterID;
            this.ConfigurationParameterID = initialStateFirmware.ConfigurationParameterID;
            this.TestCollection = initialStateFirmware.TestCollection;
            this.TestCollectionResultWithValveHardwareViewModels = testCollectionResultWithValveHardwareViewModel;
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

        public List<TestCollectionResultWithValveHardwareViewModel> TestCollectionResultWithValveHardwareViewModels
        { 
            get; 
            private set; 
        }
    }
}
