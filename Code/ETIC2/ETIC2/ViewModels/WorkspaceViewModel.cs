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
    using System.Windows.Input;
    using System.Windows.Threading;
    using ActionCommands;
    using ETIC2.Model.Application;
    using Events;
    using Events.EventArgs.Error;

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
        /// Data grid with software information items
        /// </summary>
        private readonly FirmwareDatabaseDataGridViewModel firmwareDatabaseDataGridViewModel;

        /// <summary>
        /// Set the actual active view model
        /// </summary>
        private object selectedViewModel;

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

            this.allDatabaseDataGridViewModels = new ObservableCollection<BasisViewModel>();
            this.allDatabaseDataGridViewModels.Add(new FirmwareDatabaseDataGridViewModel(viewModelEvents));
            this.allDatabaseDataGridViewModels.Add(new DatabaseDataGridViewModel2(viewModelEvents));
            this.firmwareDatabaseDataGridViewModel 
                = (FirmwareDatabaseDataGridViewModel)this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();

            this.FirmwareCommand = new BaseCommand(this.OpenFirmware);
            this.DatabaseCommand = new BaseCommand(this.OpenDatabase);
        }

        public ICommand FirmwareCommand { get; set; }

        public ICommand DatabaseCommand { get; set; }

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

        private void OpenFirmware(object obj)
        {
            this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is FirmwareDatabaseDataGridViewModel).Single();
        }

        private void OpenDatabase(object obj)
        {
            this.SelectedViewModel = this.allDatabaseDataGridViewModels.Where(x => x is DatabaseDataGridViewModel2).Single();
        }

        public override void SubscribeEvents()
        {
            // Subscribe own model events
            this.firmwareDatabaseDataGridViewModel.SubscribeEvents();
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent += this.DatabaseDataGridViewModel_RefreshChangedEvent;

            // Subscribe base class events
            base.SubscribeEvents();
        }
  
        public override void UnsubscribeEvents()
        {
            // Unsubscribe own model events
            this.firmwareDatabaseDataGridViewModel.UnsubscribeEvents();
            this.firmwareDatabaseDataGridViewModel.RefreshChangedEvent -= this.DatabaseDataGridViewModel_RefreshChangedEvent;

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

                this.ReloadDataGrid();
            }
            catch (Exception ex)
            {
                this.ViewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
        }

        /// <summary>
        /// Write database information in the DatabaseDataGridView
        /// </summary>
        private void ReloadDataGrid()
        {
            this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Clear();

            List<InitialStateFirmware> initialStateFirmwareList = this.etic2Model.InitialStateFirmwareDatabaseAccessManager.GetApplicationInitialStateFirmwares();
            List<TestCollectionResultWithValveHardware> testCollectionResultWithValveHardwareListInitialStateFirmwareFilter;
            List<TestCollectionResultWithValveHardware> testCollectionResultWithValveHardwareList = this.etic2Model.TestCollectionResultWithHardwareDatabaseAccessManager.GetApplicationTestCollectionResultsWithValveHardware();
            ObservableCollection<TestCollectionResultWithValveHardwareViewModel> testCollectionResultWithValveHardwareViewModelList;
            List<TestResult> testResultListTestCollectionResultWithValveHardwareFilter;
            List<TestResult> testResultList = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResults();
            ObservableCollection<TestResultViewModel> testResultViewModelList;
            List<TestErrorMessage> testErrorMessageListTestResultFilter;
            ObservableCollection<TestErrorMessageViewModel> testErrorMessageViewModelList;

            //Level 1 InitialStateValve (includes TestCollectionResultWithValveHardware List)
            foreach (InitialStateFirmware initialStateFirmware in initialStateFirmwareList)
            {
                testCollectionResultWithValveHardwareViewModelList = new ObservableCollection<TestCollectionResultWithValveHardwareViewModel>(); 
                testCollectionResultWithValveHardwareListInitialStateFirmwareFilter = this.etic2Model.TestCollectionResultWithHardwareDatabaseAccessManager.GetApplicationTestCollectionResultsWithValveHardwareWithInitialStateFirmwareFilter(initialStateFirmware.Id);

                //Level 2 TestCollectionResultWithValveHardware (includes TestResult List)
                foreach (TestCollectionResultWithValveHardware testCollectionResultWithValveHardware in testCollectionResultWithValveHardwareListInitialStateFirmwareFilter)
                {
                    testResultViewModelList = new ObservableCollection<TestResultViewModel>();
                    testResultListTestCollectionResultWithValveHardwareFilter = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);

                    //Level 3 TestResult (includes TestErrorMessage List)
                    foreach (TestResult testResult in testResultListTestCollectionResultWithValveHardwareFilter)
                    {
                        testErrorMessageViewModelList = new ObservableCollection<TestErrorMessageViewModel>();
                        testErrorMessageListTestResultFilter = this.etic2Model.TestErrorMessageDatabaseAccessManager.GetApplicationTestErrorMessagesWithTestResultFilter(testResult.Id);

                        //Level 4 TestErrorMessage
                        foreach (TestErrorMessage testErrorMessage in testErrorMessageListTestResultFilter)
                            testErrorMessageViewModelList.Add(new TestErrorMessageViewModel(this.ViewModelEvents, testErrorMessage));

                        testResultViewModelList.Add(new TestResultViewModel(this.ViewModelEvents, testResult, testErrorMessageViewModelList));
                    }

                    testResultList = this.etic2Model.TestResultDatabaseAccessManager.GetApplicationTestResultsWithTestCollectionResultFilter(testCollectionResultWithValveHardware.Id);
                    testCollectionResultWithValveHardwareViewModelList.Add(new TestCollectionResultWithValveHardwareViewModel(this.ViewModelEvents, testCollectionResultWithValveHardware, testResultViewModelList));
                }

                this.firmwareDatabaseDataGridViewModel.InitialStateFirmwareViewModels.Add(new InitialStateFirmwareViewModel(this.ViewModelEvents, initialStateFirmware, testCollectionResultWithValveHardwareViewModelList));
            }
        }
    }
}
