//-----------------------------------------------------------------------
// <copyright file="BuglistDatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using System.Windows.Input;
    using ActionCommands;
    using BuglistViewModels;
    using ETIC2.Events;
    using Events.EventArgs.Error;

    /// <summary>
    /// Contains buglist data grid items
    /// </summary>
    public class BuglistDatabaseDataGridViewModel : BasisViewModel
    {
        /// <summary>
        /// Has the actual selected software item in the grid view.
        /// </summary>
        private DatabaseItemViewModel selectedDatabaseItemViewModel;

        /// <summary>
        /// List of all software items in the grid view.
        /// </summary>
        private ObservableCollection<DatabaseItemViewModel> databaseItemViewModels;

        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        public BuglistDatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.RefreshCommand = new ActionCommand(this.OnRefreshCommand);

            this.viewModelEvents = viewModelEvents;
            this.databaseItemViewModels = new ObservableCollection<DatabaseItemViewModel>();
        }

        /// <summary>
        /// Occurs when the selected item has changed.
        /// </summary>
        public event EventHandler SelectedItemChanged;

        /// <summary>
        /// Event to update the DatabaseDataGriedView
        /// </summary>
        public event EventHandler<System.EventArgs> RefreshDataGridChangedEvent;

        public DatabaseItemViewModel SelectedDatabaseItemViewModel
        {
            get
            {
                return this.selectedDatabaseItemViewModel;
            }

            set
            {
                this.selectedDatabaseItemViewModel = value;
                this.OnSelectedItemChanged();
                this.OnPropertyChanged("SelectedDatabaseItemViewModel");
            }
        }

        public ObservableCollection<DatabaseItemViewModel> DatabaseItemViewModels
        {
            get
            {
                return this.databaseItemViewModels;
            }
        }

        public ICommand RefreshCommand { get; private set; }

        private void OnRefreshCommand(object parameter)
        {
            try
            {
                this.OnRefreshDataGridChangedEvent(this, new System.EventArgs());
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        private void OnRefreshDataGridChangedEvent(object sender, EventArgs refreshEventArgs)
        {
            if (this.RefreshDataGridChangedEvent != null)
                this.RefreshDataGridChangedEvent(sender, refreshEventArgs);
        }

        private void OnSelectedItemChanged()
        {
            if (this.SelectedItemChanged != null)
                this.SelectedItemChanged(this, null);
        }
    }
}
