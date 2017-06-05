//-----------------------------------------------------------------------
// <copyright file="MainWindowViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2
{
    using System;
    using ETIC2.Model.Application;
    using ETIC2.ViewModels;

    /// <summary>
    /// Main Window which has the child menu for status, menu and workspace
    /// </summary>
    /// <seealso cref="ETIC2.ViewModels.BasisViewModel" />
    public class MainWindowViewModel : BasisViewModel
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// References to the model
        /// </summary>
        private ETIC2Model etic2Model;

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

        /// <summary>
        /// Initializes a new instance of the <see cref="MainWindowViewModel"/> class.
        /// </summary>
        /// <param name="viewModelEventHandler">The view model event handler.</param>
        /// <param name="etic2Model">The etic2 model.</param>
        public MainWindowViewModel(ViewModelEvents viewModelEventHandler, ETIC2Model etic2Model)
            : base(viewModelEventHandler)
        {
            this.viewModelEvents = viewModelEventHandler;

            // Store the model
            this.etic2Model = etic2Model;

            // Create child ViewModels
            this.menuViewModel = new MenuViewModel(viewModelEventHandler, etic2Model);
            this.statusbarViewModel = new StatusbarViewModel(viewModelEventHandler);
            this.workspaceViewModel = new WorkspaceViewModel(viewModelEventHandler, etic2Model);
        }

        /// <summary>
        /// Gets the menu view model.
        /// </summary>
        /// <value>
        /// The menu view model.
        /// </value>
        public MenuViewModel MenuViewModel
        {
            get
            {
                return this.menuViewModel;
            }
        }

        /// <summary>
        /// Gets the status bar view model.
        /// </summary>
        /// <value>
        /// The status bar view model.
        /// </value>
        public StatusbarViewModel StatusbarViewModel
        {
            get
            {
                return this.statusbarViewModel;
            }
        }

        /// <summary>
        /// Gets the workspace view model.
        /// </summary>
        /// <value>
        /// The workspace view model.
        /// </value>
        public WorkspaceViewModel WorkspaceViewModel
        {
            get
            {
                return this.workspaceViewModel;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether this instance is workspace enabled.
        /// </summary>
        /// <value>
        /// <c>true</c> if this instance is workspace enabled; otherwise, <c>false</c>.
        /// </value>
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

        /// <summary>
        /// Initializes this instance.
        /// </summary>
        public override void Init()
        {
            base.Init();
            this.workspaceViewModel.Init();
        }

        /// <summary>
        /// Subscribe Events and/or call subscribe on child viewModels
        /// </summary>
        public override void SubscribeEvents()
        {
            base.SubscribeEvents();
        }

        /// <summary>
        /// Unsubscribes the events.
        /// </summary>
        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
        }
    }
}
