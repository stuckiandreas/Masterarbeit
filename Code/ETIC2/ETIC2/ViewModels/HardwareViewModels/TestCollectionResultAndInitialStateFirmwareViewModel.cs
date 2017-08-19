//-----------------------------------------------------------------------
// <copyright file="TestCollectionResultAndInitialStateFirmwareViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels.HardwareViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using ETIC2.Model.Application.HardwareView;
    using Events;
    using General;

    /// <summary>
    /// Second Level of the Hardware Master Detail View. Shows the TestCollectionResult and InitialStateFirmware entrys.
    /// </summary>
    public class TestCollectionResultAndInitialStateFirmwareViewModel : BasisViewModel
    {
        public TestCollectionResultAndInitialStateFirmwareViewModel(ViewModelEvents viewModelEvents, TestCollectionResultAndInitialStateFirmware testCollectionResultAndInitialStateFirmware, ObservableCollection<TestResultViewModel> testResultViewModelList)
            : base(viewModelEvents)
        {
            this.ExecutionTime = testCollectionResultAndInitialStateFirmware.ExecutionTime;
            this.UserName = testCollectionResultAndInitialStateFirmware.UserName;
            this.AbortType = testCollectionResultAndInitialStateFirmware.AbortType;
            this.CountErrorTest = testCollectionResultAndInitialStateFirmware.CountErrorTest;
            this.ValveFirmware = testCollectionResultAndInitialStateFirmware.ValveFirmware;
            this.ValveFirmwareReleaseTime = testCollectionResultAndInitialStateFirmware.ValveFirmwareReleaseTime;
            this.MotionControllerFirmware = testCollectionResultAndInitialStateFirmware.MotionControllerFirmware;
            this.InterfaceFirmware = testCollectionResultAndInitialStateFirmware.InterfaceFirmware;
            this.DriveParameterID = testCollectionResultAndInitialStateFirmware.DriveParameterID;
            this.ConfigurationParameterID = testCollectionResultAndInitialStateFirmware.ConfigurationParameterID;
            this.TestCollection = testCollectionResultAndInitialStateFirmware.TestCollection;
            this.TestResultViewModels = testResultViewModelList;
        }
        //Test Collection Result
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

        //Initial State Firmware
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

        public ObservableCollection<TestResultViewModel> TestResultViewModels
        {
            get;
            private set;
        }
    }
}
