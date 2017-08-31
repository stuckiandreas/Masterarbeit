//-----------------------------------------------------------------------
// <copyright file="ErrorDatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
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
    /// Contains the error data grid data items.
    /// </summary>
    public class ErrorDatabaseDataGridViewModel : BasisViewModel
    {
        /// <summary>
        /// List of all test error messages entries in the grid view.
        /// </summary>
        private ObservableCollection<ErrorTopLevelViewModels.TestErrorMessageViewModel> testErrorMessageViewModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public ErrorDatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.RefreshCommand = new ActionCommand(this.OnRefreshCommand, this.OnCanExecuteRefreshCommand);

            this.viewModelEvents = viewModelEvents;
            this.testErrorMessageViewModels = new ObservableCollection<ErrorTopLevelViewModels.TestErrorMessageViewModel>();
        }

        /// <summary>
        /// Event to update the Error DatabaseDataGriedView
        /// </summary>
        public event EventHandler<System.EventArgs> RefreshChangedEvent;

        public ObservableCollection<ErrorTopLevelViewModels.TestErrorMessageViewModel> TestErrorMessageViewModels
        {
            get { return this.testErrorMessageViewModels; }
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
