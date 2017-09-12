//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels
{
    using System;
    using System.Collections.ObjectModel;
    using Events;
    using VersionManagement.Events.EventArgs.Error;

    /// <summary>
    /// Contains data grid data items.
    /// </summary>
    public class DatabaseDataGridViewModel : BasisViewModel
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

        /// <summary>
        /// Initializes a new instance of the <see cref="DatabaseDataGridViewModel"/> class.
        /// </summary>
        /// <param name="viewModelEvents">The view model events.</param>
        public DatabaseDataGridViewModel(ViewModelEvents viewModelEvents)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.databaseItemViewModels = new ObservableCollection<DatabaseItemViewModel>();
        }

        /// <summary>
        /// Occurs when the selected item has changed.
        /// </summary>
        public event EventHandler SelectedItemChanged;

        /// <summary>
        /// Gets or sets the selected database item view model. View in the detailInformationViewModel
        /// </summary>
        /// <value>
        /// The selected database item view model.
        /// </value>
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

        /// <summary>
        /// Gets the database item view models.
        /// </summary>
        /// <value>
        /// The database item view models.
        /// </value>
        public ObservableCollection<DatabaseItemViewModel> DatabaseItemViewModels
        {
            get
            {
                return this.databaseItemViewModels;
            }
        }

        /// <summary>
        /// Called when the selected item change in the data grid.
        /// </summary>
        private void OnSelectedItemChanged()
        {
            if (this.SelectedItemChanged != null)
                this.SelectedItemChanged(this, null);
        }
    }
}
