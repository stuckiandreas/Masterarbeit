//-----------------------------------------------------------------------
// <copyright file="FirmwareDatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows.Input;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Error;

    /// <summary>
    /// Contains the firmware data grid data items.
    /// </summary>
    public class FirmwareDatabaseDataGridViewModel : BasisViewModel
    {
        /// <summary>
        /// List of all test initial state firmware entries in the grid view.
        /// </summary>
        private ObservableCollection<FirmwareViewModels.InitialStateFirmwareViewModel> initialStateFirmwareViewModels;

        /// <summary>
        /// List of all test collection result entries in the grid view.
        /// </summary>
        private ObservableCollection<FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel>
            testCollectionResultWithValveHardwareViewModels;

        /// <summary>
        /// List of all test result entries in the grid view.
        /// </summary>
        private ObservableCollection<FirmwareViewModels.TestResultViewModel> testResultVieModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public FirmwareDatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.RefreshCommand = new ActionCommand(this.OnRefreshCommand, this.OnCanExecuteRefreshCommand);

            this.viewModelEvents = viewModelEvents;
            this.initialStateFirmwareViewModels = new ObservableCollection<FirmwareViewModels.InitialStateFirmwareViewModel>();
            this.testCollectionResultWithValveHardwareViewModels =
                new ObservableCollection<FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel>();
            this.testResultVieModels = new ObservableCollection<FirmwareViewModels.TestResultViewModel>();
        }

        /// <summary>
        /// Event to update the Firmware DatabaseDataGriedView
        /// </summary>
        public event EventHandler<System.EventArgs> RefreshChangedEvent;

        public ObservableCollection<FirmwareViewModels.InitialStateFirmwareViewModel> InitialStateFirmwareViewModels
        {
            get { return this.initialStateFirmwareViewModels; }
        }

        public ObservableCollection<FirmwareViewModels.TestCollectionResultWithValveHardwareViewModel>
            TestCollectionResultWithValveHardwareViewModels
        {
            get { return this.testCollectionResultWithValveHardwareViewModels; }
        }

        public ObservableCollection<FirmwareViewModels.TestResultViewModel> TestResultViewModels
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

        private bool OnCanExecuteRefreshCommand(object parameter)
        {
            return true;
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
