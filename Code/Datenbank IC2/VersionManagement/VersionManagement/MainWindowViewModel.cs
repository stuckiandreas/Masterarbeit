//-----------------------------------------------------------------------
// <copyright file="MainWindowViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement
{
    using System;
    using Events;
    using Events.EventArgs.DatabaseAccess;
    using Events.EventArgs.Error;
    using Model;
    using ViewModels;
    using ViewModels.MenuViewModels;
    using ViewModels.StatusbarViewModels;

    /// <summary>
    /// This is the main window of the VersionManagement tool.
    /// </summary>
    public class MainWindowViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private VersionManagementModel versionManagementModel;

        // Child ViewModels    
            
        /// <summary>
        /// Reference view model of the status bar. This is shown in the main window at the bottom.
        /// </summary>
        private StatusbarViewModel statusbarViewModel;

        /// <summary>
        /// Reference of the menu in the main view which is at the top. Includes the option for change the database settings.
        /// </summary>
        private MenuViewModel menuViewModel;

        /// <summary>
        /// Reference of the workspace view model. Interface to the model. Included the detailInformation and dataBaseDataGrid view.
        /// </summary>
        private WorkspaceViewModel workspaceViewModel;

        /// <summary>
        /// Is used to disabled the main window if no connection to the database is available.
        /// </summary>
        private bool isWorkspaceEnabled;

        public MainWindowViewModel(ViewModelEvents viewModelEventHandler, VersionManagementModel versionManagementModel)
            : base(viewModelEventHandler)
        {
            LogFile.Write("In MainWindowViewModel");
            this.viewModelEvents = viewModelEventHandler;

            // Store the model
            this.versionManagementModel = versionManagementModel;

            // Create child ViewModels
            this.menuViewModel = new MenuViewModel(viewModelEventHandler, versionManagementModel);
            this.statusbarViewModel = new StatusbarViewModel(viewModelEventHandler);
            this.workspaceViewModel = new WorkspaceViewModel(viewModelEventHandler, versionManagementModel);

            this.IsWorkspaceEnabled = false;
            LogFile.Write("End MainWindowViewModel");
        }

        public MenuViewModel MenuViewModel
        {
            get
            {
                return this.menuViewModel;
            }
        }

        public StatusbarViewModel StatusbarViewModel
        {
            get
            {
                return this.statusbarViewModel;
            }
        }

        public WorkspaceViewModel WorkspaceViewModel
        {
            get
            {
                return this.workspaceViewModel;
            }
        }

        public bool IsWorkspaceEnabled
        {
            get
            {
                return this.isWorkspaceEnabled;
            }

            set
            {
                this.isWorkspaceEnabled = value;
                this.OnPropertyChanged("IsWorkspaceEnabled");
            }
        }

        public override void Init()
        {
            base.Init();
            this.workspaceViewModel.Init();
        }

        public override void SubscribeEvents()
        {
            base.SubscribeEvents();
            this.workspaceViewModel.SubscribeEvents();
            this.versionManagementModel.SoftwareVersionsItem.DatabaseAccessEvent += this.SoftwareVersionsItem_DatabaseAccessEvent;
            this.versionManagementModel.SelectionItem.DatabaseAccessEvent += this.SoftwareVersionsItem_DatabaseAccessEvent;
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
            this.workspaceViewModel.UnsubscribeEvents();
            this.versionManagementModel.SoftwareVersionsItem.DatabaseAccessEvent -= this.SoftwareVersionsItem_DatabaseAccessEvent;
            this.versionManagementModel.SelectionItem.DatabaseAccessEvent -= this.SoftwareVersionsItem_DatabaseAccessEvent;
        }

        /// <summary>
        /// If the database access manager sends an database access event than the workspace view is enabled, that mean the main window is not bright gray. 
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="DatabaseAccessEventArgs"/> instance containing the event data.</param>
        private void SoftwareVersionsItem_DatabaseAccessEvent(object sender, DatabaseAccessEventArgs e)
        {
            try
            {
                this.IsWorkspaceEnabled = true;

                this.statusbarViewModel.Status = string.Format("Database: {0}  -  Status: {1}", e.DatabaseName, e.DatabaseAvailability.ToString());

                if (e.DatabaseAvailability != DatabaseAvailability.Available)
                    this.IsWorkspaceEnabled = false;
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }           
        }
    }
}