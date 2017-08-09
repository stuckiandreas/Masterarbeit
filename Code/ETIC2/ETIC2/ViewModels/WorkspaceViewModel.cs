//-----------------------------------------------------------------------
// <copyright file="WorkspaceViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.ViewModels
{
    using System.Collections.Generic;
    using ETIC2.Model.Application;
    using Events;
    
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
        private readonly DatabaseDataGridViewModel databaseDataGridViewModel;
        
        /// <summary>
        /// Reference to the model
        /// </summary>
        private readonly ETIC2Model etic2Model;

        public WorkspaceViewModel(ViewModelEvents viewModelEvents, ETIC2Model etic2Model)
            : base(viewModelEvents)
        {
            this.etic2Model = etic2Model;
            this.ViewModelEvents = viewModelEvents;
            this.databaseDataGridViewModel = new DatabaseDataGridViewModel(viewModelEvents);
        }

        public DatabaseDataGridViewModel DatabaseDataGridViewModel
        {
            get
            {
                return this.databaseDataGridViewModel;
            }
        }

        public override void SubscribeEvents()
        {
            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        public override void Init()
        {
            // Init base class
            base.Init();

            this.ReloadDataGrid();
        }

        /// <summary>
        /// Write database information in the DatabaseDataGridView
        /// </summary>
        private void ReloadDataGrid()
        {
            this.databaseDataGridViewModel.InitialStateFirmwareViewModels.Clear();

            List<InitialStateFirmware> initialStateFirmwareList = this.etic2Model.InitialStateFirmwareDatabaseAccessManager.GetApplicationInitialStateFirmwares();

            foreach (InitialStateFirmware initialStateFirmware in initialStateFirmwareList)
                this.databaseDataGridViewModel.InitialStateFirmwareViewModels.Add(new InitialStateFirmwareViewModel(initialStateFirmware));
        }
    }
}
