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
    using ETIC2.Model.Application;
    using Events;
    using Events.EventArgs.Error;
    using Model.Application.FirmwareView;
    using Model.Application.ErrorView;
    using ErrorTopLevelViewModels;
    
    /// <summary>
    /// Define the diffrent DatabaseDataGridViewModel. Everyone shows a diffrent master detail structure.
    /// </summary>
    public enum DatabseDataGridViewModel
    {
        Firmware,
        Hardware,
        Error
    }

    /// <summary>
    /// Mainly View Model. Interface to the model. Included the detailInformation and dataBaseDataGrid view. 
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
        /// Error Top Level DataGrid
        /// </summary>
        private readonly ErrorDatabaseDataGridViewModel errorDatabaseDataGridViewModel;

        /// <summary>
        /// Set the actual active view model
        /// </summary>
        private object selectedViewModel;

        /// <summary>
        /// Active DatabaseDataGridViewModel
        /// </summary>
        private string databaseDataGridSelectedItem;

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

            //Initialize a list with all DatabaseDataGridViewModels 
            this.allDatabaseDataGridViewModels = new ObservableCollection<BasisViewModel>();
            this.allDatabaseDataGridViewModels.Add(new FirmwareDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new ErrorDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new DatabaseDataGridViewModel2(viewModelEvents));

            //Set Reference of the view model in the list (if not, the data will not set)
            this.firmwareDatabaseDataGridViewModel
                = (FirmwareDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();
            this.errorDatabaseDataGridViewModel
                = (ErrorDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is ErrorDatabaseDataGridViewModel).Single();

            //Fill ComboBox List entries for diffrent DatabaseDataGridViews
            this.DatabaseDataGridItems = new ObservableCollection<string>();
            this.DatabaseDataGridItems.Add(DatabseDataGridViewModel.Firmware.ToString());
            this.DatabaseDataGridItems.Add(DatabseDataGridViewModel.Hardware.ToString());
            this.DatabaseDataGridItems.Add(DatabseDataGridViewModel.Error.ToString());
            this.databaseDataGridSelectedItem = DatabseDataGridViewModel.Firmware.ToString();
            this.SetActiveDatabaseDataGridViewModel();
        }

        public ObservableCollection<string> DatabaseDataGridItems { get; }

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

        public ErrorDatabaseDataGridViewModel ErrorDatabaseDataGridViewModel
        {
            get
            {
                return this.errorDatabaseDataGridViewModel;
            }
        }

        public override void SubscribeEvents()
        {
            // Subscribe own model events
            this.firmwareDatabaseDataGridViewModel.SubscribeEvents();
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent += this.DatabaseDataGridViewModel_RefreshChangedEvent;
            this.errorDatabaseDataGridViewModel.RefreshChangedEvent += ErrorDatabaseDataGridViewModel_RefreshChangedEvent;

            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            // Unsubscribe own model events
            this.firmwareDatabaseDataGridViewModel.UnsubscribeEvents();
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent -= this.DatabaseDataGridViewModel_RefreshChangedEvent;
            this.errorDatabaseDataGridViewModel.RefreshChangedEvent -= ErrorDatabaseDataGridViewModel_RefreshChangedEvent;

            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        public override void Init()
        {
            // Init base class
            base.Init();

            this.ReloadDataGrid();
        }

        private void ViewModelEvents_ChangeDatabaseSettings(object sender, Events.EventArgs.DatabaseAccess.DatabaseAccessEventArgs e)
        {
            try
            {
                Dispatcher.CurrentDispatcher.InvokeAsync(() => this.Init());
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        private void DatabaseDataGridViewModel_RefreshChangedEvent(object sender, System.EventArgs e)
        {
            try
            {
                //update database -> event
                this.etic2Model.BuildDatabaseContext();

                this.LoadFirmwareDatabaseDataGrid();
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
                //update database -> event
                this.etic2Model.BuildDatabaseContext();

                this.LoadErrorDatabaseDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// Write database information in the DatabaseDataGridView. Depends which View is selected.
        /// </summary>
        private void ReloadDataGrid()
        {
            if (this.databaseDataGridSelectedItem == DatabseDataGridViewModel.Firmware.ToString())
                this.LoadFirmwareDatabaseDataGrid();
            else if (this.databaseDataGridSelectedItem == DatabseDataGridViewModel.Error.ToString())
                this.LoadErrorDatabaseDataGrid();
        }

        /// <summary>
        /// Write only the data for the FirmwareDatabaseDataGridViewModel
        /// </summary>
        private void LoadFirmwareDatabaseDataGrid()
        {
            this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Clear();

            List<InitialStateFirmware> initialStateFirmwareList = this.etic2Model.InitialStateFirmwareDatabaseAccessManager.GetApplicationInitialStateFirmwares();
            List<TestCollectionResultWithValveHardware> testCollectionResultWithValveHardwareListInitialStateFirmwareFilter;
            ObservableCollection<FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModelList;
            List<TestResult> testResultListTestCollectionResultWithValveHardwareFilter;
            List<TestResult> testResultList = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResults();
            ObservableCollection<FirmwareViewModels.TestResultViewModel> testResultViewModelList;
            List<Model.Application.FirmwareView.TestErrorMessage> testErrorMessageListTestResultFilter;
            ObservableCollection<FirmwareViewModels.TestErrorMessageViewModel> testErrorMessageViewModelList;

            //Level 1 InitialStateValve (includes TestCollectionResultWithValveHardware List)
            foreach (InitialStateFirmware initialStateFirmware in initialStateFirmwareList)
            {
                testCollectionResultWithValveHardwareViewModelList = new ObservableCollection<FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel>();
                testCollectionResultWithValveHardwareListInitialStateFirmwareFilter = this.etic2Model.TestCollectionResultWithHardwareDatabaseAccessManager.GetApplicationTestCollectionResultsWithValveHardwareWithInitialStateFirmwareFilter(initialStateFirmware.Id);

                //Level 2 TestCollectionResultWithValveHardware (includes TestResult List)
                foreach (TestCollectionResultWithValveHardware testCollectionResultWithValveHardware in testCollectionResultWithValveHardwareListInitialStateFirmwareFilter)
                {
                    testResultViewModelList = new ObservableCollection<FirmwareViewModels.TestResultViewModel>();
                    testResultListTestCollectionResultWithValveHardwareFilter = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);

                    //Level 3 TestResult (includes TestErrorMessage List)
                    foreach (TestResult testResult in testResultListTestCollectionResultWithValveHardwareFilter)
                    {
                        testErrorMessageViewModelList = new ObservableCollection<FirmwareViewModels.TestErrorMessageViewModel>();
                        testErrorMessageListTestResultFilter = this.etic2Model.TestErrorMessageDatabaseAccessManager.GetApplicationTestErrorMessagesWithTestResultFilter(testResult.Id);

                        //Level 4 TestErrorMessage
                        foreach (Model.Application.FirmwareView.TestErrorMessage testErrorMessage in testErrorMessageListTestResultFilter)
                            testErrorMessageViewModelList.Add(new FirmwareViewModels.TestErrorMessageViewModel(this.ViewModelEvents, testErrorMessage));

                        testResultViewModelList.Add(new FirmwareViewModels.TestResultViewModel(this.ViewModelEvents, testResult, testErrorMessageViewModelList));
                    }

                    testResultList = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);
                    testCollectionResultWithValveHardwareViewModelList.Add(new FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel(this.ViewModelEvents, testCollectionResultWithValveHardware, testResultViewModelList));
                }

                this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Add(new FirmwareViewModels.InitialStateFirmwareViewModel(this.ViewModelEvents, initialStateFirmware, testCollectionResultWithValveHardwareViewModelList));
            }
        }

        /// <summary>
        /// Write only the data for the ErrorDatabaseDataGridViewModel
        /// </summary>
        private void LoadErrorDatabaseDataGrid()
        {

        }

        /// <summary>
        /// Sets the active DatabaseDataGridViewModel. Change only the reference of the view model.
        /// </summary>
        private void SetActiveDatabaseDataGridViewModel()
        {
            if (this.databaseDataGridSelectedItem == DatabseDataGridViewModel.Firmware.ToString())
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();
            else if (this.databaseDataGridSelectedItem == DatabseDataGridViewModel.Hardware.ToString())
                this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is DatabaseDataGridViewModel2).Single();
        }
    }
}
