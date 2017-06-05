//-----------------------------------------------------------------------
// <copyright file="WorkspaceViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.ViewModels
{
    using ETIC2.Model.Application;

    /// <summary>
    /// Mainly View Model. Interface to the model. Included the detailInformation and dataBaseDataGrid view. 
    /// Only references to the Model are allowed. References to Views are not allowed
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public class WorkspaceViewModel : BasisViewModel
    {
        /// <summary>
        /// View model events with several handlers
        /// </summary>
        public readonly ViewModelEvents ViewModelEvents;
        
        /// <summary>
        /// Data grid with software information items
        /// </summary>
        private readonly DatabaseDataGridViewModel dataGridViewatabaseDataGridViewModel;
        
        /// <summary>
        /// Reference to the model
        /// </summary>
        private readonly ETIC2Model etic2Model;

        /// <summary>
        /// Initializes a new instance of the <see cref="WorkspaceViewModel"/> class.
        /// </summary>
        /// <param name="viewModelEvents">The view model events.</param>
        /// <param name="etic2Model">The etic2 model.</param>
        public WorkspaceViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
        }

        /// <summary>
        /// Gets the database data grid view model.
        /// </summary>
        /// <value>
        /// The database data grid view model.
        /// </value>
        public DatabaseDataGridViewModel DatabaseDataGridViewModel
        {
            get
            {
                return this.databaseDataGridViewModel;
            }
        }

        /// <summary>
        /// Subscribes the events.
        /// </summary>
        public override void SubscribeEvents()
        {
            // Subscribe base class events
            base.SubscribeEvents();
        }

        /// <summary>
        /// Unsubscribes the events.
        /// </summary>
        public override void UnsubscribeEvents()
        {
            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        /// <summary>
        /// Initializes this instance.
        /// </summary>
        public override void Init()
        {
            // Init base class
            base.Init();
        }
    }
}
