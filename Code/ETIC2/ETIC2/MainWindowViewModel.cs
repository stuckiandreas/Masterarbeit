//-----------------------------------------------------------------------
// <copyright file="MainWindowViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2
{
    using ETIC2.Model.Application;
    using ETIC2.ViewModels;
    using Events;
    using ViewModels.MenuViewModels;
    using ViewModels.StatusbarViewModels;

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
        }

        public override void UnsubscribeEvents()
        {
            base.UnsubscribeEvents();
        }
    }
}
