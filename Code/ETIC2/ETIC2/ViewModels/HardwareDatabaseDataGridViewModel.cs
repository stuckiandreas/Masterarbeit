//-----------------------------------------------------------------------
// <copyright file="HardwareDatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows.Input;
    using ActionCommands;
    using ETIC2.Events;
    using Events.EventArgs.Error;
    using General;

    /// <summary>
    /// Contains the hardware data grid data items.
    /// </summary>
    public class HardwareDatabaseDataGridViewModel : BasisViewModel
    {
        /// <summary>
        /// List of all valve hardware entries in the grid view.
        /// </summary>
        private ObservableCollection<HardwareViewModels.ValveHardwareViewModel> valveHardwareViewModels;

        /// <summary>
        /// List of all test collection result and initial state firmware entries in the grid view.
        /// </summary>
        private ObservableCollection<HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel>
            testCollectionResultAndInitialStateFirmwareViewModels;

        /// <summary>
        /// List of all test result entries in the grid view.
        /// </summary>
        private ObservableCollection<TestResultViewModel> testResultVieModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public HardwareDatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.RefreshCommand = new ActionCommand(this.OnRefreshCommand);

            this.viewModelEvents = viewModelEvents;
            this.valveHardwareViewModels = new ObservableCollection<HardwareViewModels.ValveHardwareViewModel>();
            this.testCollectionResultAndInitialStateFirmwareViewModels =
                new ObservableCollection<HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel>();
            this.testResultVieModels = new ObservableCollection<TestResultViewModel>();
        }

        /// <summary>
        /// Event to update the Firmware DatabaseDataGriedView
        /// </summary>
        public event EventHandler<System.EventArgs> RefreshChangedEvent;

        public ObservableCollection<HardwareViewModels.ValveHardwareViewModel> ValveHardwareViewModels
        {
            get { return this.valveHardwareViewModels; }
        }

        public ObservableCollection<HardwareViewModels.TestCollectionResultAndInitialStateFirmwareViewModel>
            TestCollectionResultAndInitialStateFirmwareViewModels
        {
            get { return this.testCollectionResultAndInitialStateFirmwareViewModels; }
        }

        public ObservableCollection<TestResultViewModel> TestResultViewModels
        {
            get { return this.testResultVieModels; }
        }

        public ICommand RefreshCommand { get; private set; }

        public override void SubscribeEvents()
        {
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
        }

        private void OnRefreshCommand(object parameter)
        {
            try
            {
                this.OnRefreshChangedEvent(this, new System.EventArgs());
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnRefreshChangedEvent(object sender, System.EventArgs refreshEventArgs)
        {
            if (this.RefreshChangedEvent != null)
                this.RefreshChangedEvent(sender, refreshEventArgs);
        }
    }
}