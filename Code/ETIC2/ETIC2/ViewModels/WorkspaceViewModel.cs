//-----------------------------------------------------------------------
// <copyright file="WorkspaceViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using System.Linq;
    using System.Windows.Threading;
    using Events;
    using Events.EventArgs.Error;
    using Model.Application;
    using Model.Application.FirmwareView;
    using Model.Application.General;
    using Model.Application.HardwareView;
    using Events.EventArgs.BuglistItem;
    /// <summary>
    /// Define the diffrent DatabaseDataGridViewModel. Everyone shows a diffrent master detail structure.
    /// </summary>
    public enum DatabaseDataGridViewModel
    {
        Firmware,
        Hardware,
        Error,
        Buglist
    }

    /// <summary>
    /// Mainly View Model. Interface to the model. Included the dataBaseDataGrid with the diffrent views (Firmware, Hardware + Error). 
    /// Only references to the Model are allowed. References to Views are not allowed
    /// </summary>
    /// <seealso cref="BasisViewModel" />
    public class WorkspaceViewModel : BasisViewModel
    {
        /// <summary>
        /// List with all databaseDataGridViewModels. Need to change the view.
        /// </summary>
        private readonly ObservableCollection<BasisViewModel> allDatabaseDataGridViewModels;

        /// <summary>
        /// Reference to the model
        /// </summary>
        private readonly ETIC2Model etic2Model;

        /// <summary>
        /// Firmware Top Level DataGrid
        /// </summary>
        private readonly FirmwareDatabaseDataGridViewModel firmwareDatabaseDataGridViewModel;

        /// <summary>
        /// Hardware Top Level DataGrid
        /// </summary>
        private readonly HardwareDatabaseDataGridViewModel hardwareDatabaseDataGridViewModel;

        /// <summary>
        /// Error DataGrid
        /// </summary>
        private readonly ErrorDatabaseDataGridViewModel errorDatabaseDataGridViewModel;

        /// <summary>
        /// Detail information view to edit and save buglist entries
        /// </summary>
        private readonly DetailedInformationViewModel detailedInformationViewModel;

        /// <summary>
        /// Set the actual active view model
        /// </summary>
        private object selectedViewModel;

        /// <summary>
        /// Active DatabaseDataGridViewModel
        /// </summary>
        private string databaseDataGridSelectedItem;

        /// <summary>
        /// Defined if the DetailView is active (only if buglist is selected)
        /// </summary>
        private bool detailInformationViewVisibility;

        /// <summary>
        /// View model events with several handlers
        /// </summary>
        public readonly ViewModelEvents ViewModelEvents;

        public WorkspaceViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            this.etic2Model = etic2Model;
            this.ViewModelEvents = viewModelEvents;
            this.firmwareDatabaseDataGridViewModel = new FirmwareDatabaseDataGridViewModel(viewModelEvents);
            this.detailedInformationViewModel = new DetailedInformationViewModel(viewModelEvents, this.etic2Model);

            //Initialize a list with all DatabaseDataGridViewModels 
            this.allDatabaseDataGridViewModels = new ObservableCollection<BasisViewModel>();
            this.allDatabaseDataGridViewModels.Add(new FirmwareDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new ErrorDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new HardwareDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new BuglistDatabaseDataGridViewModel(viewModelEvents));

            //Set Reference of the view model in the list (if not, the data will not set)
            this.firmwareDatabaseDataGridViewModel
                = (FirmwareDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();
            this.hardwareDatabaseDataGridViewModel
                = (HardwareDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is HardwareDatabaseDataGridViewModel).Single();
            this.errorDatabaseDataGridViewModel
                = (ErrorDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is ErrorDatabaseDataGridViewModel).Single();

            //Fill ComboBox List entries for diffrent DatabaseDataGridViews
            this.DatabaseDataGridItems = new ObservableCollection<string>();
            this.DatabaseDataGridItems.Add(DatabaseDataGridViewModel.Firmware.ToString());
            this.DatabaseDataGridItems.Add(DatabaseDataGridViewModel.Hardware.ToString());
            this.DatabaseDataGridItems.Add(DatabaseDataGridViewModel.Error.ToString());
            this.databaseDataGridSelectedItem = DatabaseDataGridViewModel.Firmware.ToString();
            this.SetActiveDatabaseDataGridViewModel();
            this.detailInformationViewVisibility = false;
        }

        public ObservableCollection<string> DatabaseDataGridItems { get; }

        public DetailedInformationViewModel DetailedInformationViewModel
        {
            get
            {
                return this.detailedInformationViewModel;
            }
        }

        public string DatabaseDataGridSelectedItem
        {
            get
            {
                return this.databaseDataGridSelectedItem;
            }

            set
            {
                this.databaseDataGridSelectedItem = value;
                this.SetActiveDatabaseDataGridViewModel();
            }
        }

        public object SelectedViewModel
        {
            get
            {
                return this.selectedViewModel;
            }

            set
            {
                this.selectedViewModel = value;
                this.OnPropertyChanged("SelectedViewModel");
            }
        }

        public FirmwareDatabaseDataGridViewModel FirmwareDatabaseDataGridViewModel
        {
            get
            {
                return this.firmwareDatabaseDataGridViewModel;
            }
        }

        public HardwareDatabaseDataGridViewModel HardwareDatabaseDataGridViewModel
        {
            get
            {
                return this.hardwareDatabaseDataGridViewModel;
            }
        }

        public ErrorDatabaseDataGridViewModel ErrorDatabaseDataGridViewModel
        {
            get
            {
                return this.errorDatabaseDataGridViewModel;
            }
        }

        public bool DetailInformationViewVisibility
        {
            get
            {
                return detailInformationViewVisibility;
            }

            set
            {
                this.detailInformationViewVisibility = value;
                this.OnPropertyChanged("DetailInformationViewVisibility");
            }
        }

        public override void SubscribeEvents()
        {
            // Subscribe own model events
            this.firmwareDatabaseDataGridViewModel.SubscribeEvents();
            this.ViewModelEvents.ChangeDatabaseSettings += this.ViewModelEvents_ChangeDatabaseSettings;
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent += this.FirmwareDatabaseDataGridViewModel_RefreshChangedEvent;
            this.hardwareDatabaseDataGridViewModel.RefreshChangedEvent += this.HardwareDatabaseDataGridViewModel_RefreshChangedEvent;
            this.errorDatabaseDataGridViewModel.RefreshChangedEvent += this.ErrorDatabaseDataGridViewModel_RefreshChangedEvent;
            this.detailedInformationViewModel.ItemChangedEvent += DetailedInformationViewModel_ItemChangedEvent;

            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            // Unsubscribe own model events
            this.firmwareDatabaseDataGridViewModel.UnsubscribeEvents();
            this.ViewModelEvents.ChangeDatabaseSettings -= this.ViewModelEvents_ChangeDatabaseSettings;
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent -= this.FirmwareDatabaseDataGridViewModel_RefreshChangedEvent;
            this.hardwareDatabaseDataGridViewModel.RefreshChangedEvent -= this.HardwareDatabaseDataGridViewModel_RefreshChangedEvent;
            this.errorDatabaseDataGridViewModel.RefreshChangedEvent -= this.ErrorDatabaseDataGridViewModel_RefreshChangedEvent;
            this.detailedInformationViewModel.ItemChangedEvent -= DetailedInformationViewModel_ItemChangedEvent;

            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        public override void Init()
        {
            //Add an empty entrys if no empty entry exists in database yet. This is needed to allow the user to leave the selection empty.
            //Add an empty entry if no empty entry exist in database yet. So its possible to delete an item in a list.
            //Otherwise the user must create an own empty entry.
            if (!this.etic2Model.BuglistSelectedItem.GetFailureTypes()
                 .Any(x => string.IsNullOrEmpty(x.Name)))
                this.etic2Model.BuglistSelectedItem.AddFailureType(string.Empty);

            if (!this.etic2Model.BuglistSelectedItem.GetStatusTypes()
                 .Any(x => string.IsNullOrEmpty(x.Name)))
                this.etic2Model.BuglistSelectedItem.AddStatusType(string.Empty);

            if (!this.etic2Model.BuglistSelectedItem.GetHardwareIdentificationLevels1()
                 .Any(x => string.IsNullOrEmpty(x.Name)))
                this.etic2Model.BuglistSelectedItem.AddHardwareIdentificationLevel1(string.Empty);

            if (!this.etic2Model.BuglistSelectedItem.GetHardwareIdentificationLevels2()
                 .Any(x => string.IsNullOrEmpty(x.Name)))
                this.etic2Model.BuglistSelectedItem.AddHardwareIdentificationLevel2(string.Empty);

            if (!this.etic2Model.BuglistSelectedItem.GetPriorities()
                 .Any(x => string.IsNullOrEmpty(x.Name)))
                this.etic2Model.BuglistSelectedItem.AddStatusType(string.Empty);

            this.RefreshDataGrid();

            // Init base class
            base.Init();
        }

        private void ViewModelEvents_ChangeDatabaseSettings(object sender, Events.EventArgs.DatabaseAccess.DatabaseAccessEventArgs e)
        {
            try
            {
                Dispatcher.CurrentDispatcher.InvokeAsync(() => this.etic2Model.BuildDatabaseContext());
                Dispatcher.CurrentDispatcher.InvokeAsync(() => this.RefreshDataGrid());
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void FirmwareDatabaseDataGridViewModel_RefreshChangedEvent(object sender, System.EventArgs e)
        {
            try
            {
                //Refreshes the database context. If not, the database not recordgnized when data entries has changed
                this.etic2Model.BuildDatabaseContext();
                this.RefreshDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void HardwareDatabaseDataGridViewModel_RefreshChangedEvent(object sender, EventArgs e)
        {
            try
            {
                //Refreshes the database context. If not, the database not recordgnized when data entries has changed
                this.etic2Model.BuildDatabaseContext();
                this.RefreshDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void ErrorDatabaseDataGridViewModel_RefreshChangedEvent(object sender, EventArgs e)
        {
            try
            {
                //Refreshes the database context. If not, the database not recordgnized when data entries has changed
                this.etic2Model.BuildDatabaseContext();
                this.RefreshDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void DetailedInformationViewModel_ItemChangedEvent(object sender, Events.EventArgs.BuglistItem.ItemEventArgs e)
        {
            try
            {
                if (!(sender is DetailedInformationViewModel))
                    return;

                //Add new Item to the collection
                if (e is AddNewItemEventArgs)
                    this.AddItem((AddNewItemEventArgs)e);

                //Save existing item
                if (e is SaveItemEventArgs)
                    this.SaveItem(e.DatabaseItemViewModel);

                //Delete existing item
                if (e is DeleteItemEventArgs)
                    this.DeleteItem(e.DatabaseItemViewModel);
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// Refresh all DataGrid with actual data (Firmware, Hardware + Error View)
        /// </summary>
        private void RefreshDataGrid()
        {
            this.LoadFirmwareDatabaseDataGrid();
            this.LoadHardwareDatabaseDataGrid();
            this.LoadErrorDatabaseDataGrid();
        }

        /// <summary>
        /// Write only the data for the FirmwareDatabaseDataGridViewModel
        /// </summary>
        private void LoadFirmwareDatabaseDataGrid()
        {
            this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Clear();

            List<InitialStateFirmware> initialStateFirmwareList = 
                this.etic2Model.InitialStateFirmware.GetApplicationInitialStateFirmwares();
            List<TestCollectionResultAndValveHardware> testCollectionResultAndValveHardwareListInitialStateFirmwareFilter;
            ObservableCollection<FirmwareViewModels.TestCollectionResultAndValveHardwareViewModel> 
                testCollectionResultAndValveHardwareViewModelList;
            List<TestResult> testResultListTestCollectionResultAndValveHardwareFilter;
            List<TestResult> testResultList = this.etic2Model.TestResult.GetApplicationTestResults();
            ObservableCollection<General.TestResultViewModel> testResultViewModelList;
            List<TestErrorMessage> testErrorMessageListTestResultFilter;
            ObservableCollection<General.TestErrorMessageViewModel> testErrorMessageViewModelList;

            //Level 1 InitialStateValve (includes TestCollectionResultAndValveHardware List)
            foreach (InitialStateFirmware initialStateFirmware in initialStateFirmwareList)
            {
                testCollectionResultAndValveHardwareViewModelList = 
                    new ObservableCollection<FirmwareViewModels.TestCollectionResultAndValveHardwareViewModel>();
                testCollectionResultAndValveHardwareListInitialStateFirmwareFilter = 
                    this.etic2Model.TestCollectionResult.GetApplicationTestCollectionResultsAndValveHardwareWithInitialStateFirmwareFilter(initialStateFirmware.Id);

                //Level 2 TestCollectionResultAndValveHardware (includes TestResult List)
                foreach (TestCollectionResultAndValveHardware testCollectionResultWithValveHardware in testCollectionResultAndValveHardwareListInitialStateFirmwareFilter)
                {
                    testResultViewModelList = new ObservableCollection<General.TestResultViewModel>();
                    testResultListTestCollectionResultAndValveHardwareFilter = 
                        this.etic2Model.TestResult.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);

                    //Level 3 TestResult (includes TestErrorMessage List)
                    foreach (Model.Application.General.TestResult testResult in testResultListTestCollectionResultAndValveHardwareFilter)
                    {
                        testErrorMessageViewModelList = new ObservableCollection<General.TestErrorMessageViewModel>();
                        testErrorMessageListTestResultFilter = 
                            this.etic2Model.TestErrorMessage.GetApplicationTestErrorMessagesWithTestResultFilter(testResult.Id);

                        //Level 4 TestErrorMessage
                        foreach (Model.Application.General.TestErrorMessage testErrorMessage in testErrorMessageListTestResultFilter)
                            testErrorMessageViewModelList.Add(new General.TestErrorMessageViewModel(this.ViewModelEvents, testErrorMessage));

                        testResultViewModelList.Add(
                            new General.TestResultViewModel(this.ViewModelEvents, testResult, testErrorMessageViewModelList));
                    }

                    testResultList = 
                        this.etic2Model.TestResult.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);
                    testCollectionResultAndValveHardwareViewModelList.Add(
                        new FirmwareViewModels.TestCollectionResultAndValveHardwareViewModel(this.ViewModelEvents, testCollectionResultWithValveHardware, testResultViewModelList));
                }

                this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Add(
                    new FirmwareViewModels.InitialStateFirmwareViewModel(this.ViewModelEvents, initialStateFirmware, testCollectionResultAndValveHardwareViewModelList));
            }
        }

        /// <summary>
        /// Write only the data for the FirmwareDatabaseDataGridViewModel
        /// </summary>
        private void LoadHardwareDatabaseDataGrid()
        {
            this.hardwareDatabaseDataGridViewModel.ValveHardwareViewModels.Clear();

            List<ValveHardware> valveHardwareList = this.etic2Model.ValveHardware.GetApplicationValveHardwares();
            List<TestCollectionResultAndInitialStateFirmware> testCollectionResultAndInitialStateFirmwareListValveHardwareFilter;
            ObservableCollection<HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel> 
                testCollectionResultAndInitialStateFirmwareViewModelList;
            List<TestResult> testResultListTestCollectionResultWithValveHardwareFilter;
            List<TestResult> testResultList = this.etic2Model.TestResult.GetApplicationTestResults();
            ObservableCollection<General.TestResultViewModel> testResultViewModelList;
            List<TestErrorMessage> testErrorMessageListTestResultFilter;
            ObservableCollection<General.TestErrorMessageViewModel> testErrorMessageViewModelList;

            //Level 1 ValveHardware (includes TestCollectionResultAndInitialStateFirmware List)
            foreach (ValveHardware valveHardware in valveHardwareList)
            {
                testCollectionResultAndInitialStateFirmwareViewModelList = 
                    new ObservableCollection<HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel>();
                testCollectionResultAndInitialStateFirmwareListValveHardwareFilter = 
                    this.etic2Model.TestCollectionResult.GetApplicationTestCollectionResultsAndInitialStateFirmwareWithValveHardwareFilter(valveHardware.Id);

                //Level 2 TestCollectionResultAndInitialStateFirmware (includes TestResult List)
                foreach (TestCollectionResultAndInitialStateFirmware testCollectionResultAndInitialStateFirmware in testCollectionResultAndInitialStateFirmwareListValveHardwareFilter)
                {
                    testResultViewModelList = new ObservableCollection<General.TestResultViewModel>();
                    testResultListTestCollectionResultWithValveHardwareFilter = 
                        this.etic2Model.TestResult.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultAndInitialStateFirmware.Id);

                    //Level 3 TestResult (includes TestErrorMessage List)
                    foreach (TestResult testResult in testResultListTestCollectionResultWithValveHardwareFilter)
                    {
                        testErrorMessageViewModelList = new ObservableCollection<General.TestErrorMessageViewModel>();
                        testErrorMessageListTestResultFilter = 
                            this.etic2Model.TestErrorMessage.GetApplicationTestErrorMessagesWithTestResultFilter(testResult.Id);

                        //Level 4 TestErrorMessage
                        foreach (TestErrorMessage testErrorMessage in testErrorMessageListTestResultFilter)
                            testErrorMessageViewModelList.Add(new General.TestErrorMessageViewModel(this.ViewModelEvents, testErrorMessage));

                        testResultViewModelList.Add(
                            new General.TestResultViewModel(this.ViewModelEvents, testResult, testErrorMessageViewModelList));
                    }

                    testResultList = 
                        this.etic2Model.TestResult.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultAndInitialStateFirmware.Id);
                    testCollectionResultAndInitialStateFirmwareViewModelList.Add(
                        new HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel(this.ViewModelEvents, testCollectionResultAndInitialStateFirmware, testResultViewModelList));
                }

                this.hardwareDatabaseDataGridViewModel.ValveHardwareViewModels.Add(
                    new HardwareViewModels.ValveHardwareViewModel(this.ViewModelEvents, valveHardware, testCollectionResultAndInitialStateFirmwareViewModelList));
            }
        }

        /// <summary>
        /// Write only the data for the ErrorDatabaseDataGridViewModel
        /// </summary>
        private void LoadErrorDatabaseDataGrid()
        {
            this.errorDatabaseDataGridViewModel.TestErrorMessageViewModels.Clear();

            List<Model.Application.ErrorView.TestErrorMessage> testErrorMessageItems = 
                this.etic2Model.TestErrorMessage.GetApplicationTestErrorMessages();

            foreach (Model.Application.ErrorView.TestErrorMessage testErrorMessageItem in testErrorMessageItems)
                this.errorDatabaseDataGridViewModel.TestErrorMessageViewModels.Add(
                    new ErrorTopLevelViewModels.TestErrorMessageViewModel(this.ViewModelEvents, testErrorMessageItem));
        }

        /// <summary>
        /// Sets the active DatabaseDataGridViewModel. Change only the reference of the view model.
        /// </summary>
        private void SetActiveDatabaseDataGridViewModel()
        {
            if (this.databaseDataGridSelectedItem == DatabaseDataGridViewModel.Firmware.ToString())
            {
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();
                this.detailInformationViewVisibility = false;
            }
            else if (this.databaseDataGridSelectedItem == DatabaseDataGridViewModel.Hardware.ToString())
            {
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is HardwareDatabaseDataGridViewModel).Single();
                this.detailInformationViewVisibility = false;
            }
            else if (this.databaseDataGridSelectedItem == DatabaseDataGridViewModel.Error.ToString())
            {
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is ErrorDatabaseDataGridViewModel).Single();
                this.detailInformationViewVisibility = false;
            }
            else
            {
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is BuglistDatabaseDataGridViewModel).Single();
                this.detailInformationViewVisibility = true;
            }    
        }
    }
}
