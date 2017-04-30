using VersionManagement.Model;
using VersionManagement.ViewModels;

namespace VersionManagement
{
    public class MainWindowViewModel : BasisViewModel
    {
        // References to the model
        private VersionManagementModel versionManagementModelodel;

        // Child ViewModels
        private WorkspaceViewModel workspaceViewModel;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="viewModelEventHandler"></param>
        /// <param name="currentWorkspaceModel"></param>
        public MainWindowViewModel(ViewModelEvents viewModelEventHandler, VersionManagementModel currentVersionManagementModel)
            : base(viewModelEventHandler)
        {
            // Store the model
            versionManagementModelodel = currentVersionManagementModel;

            // Create child ViewModels
            workspaceViewModel = new WorkspaceViewModel(viewModelEventHandler);
        }


        /// <summary>
        /// Link to child ViewModels (needed for binding)
        /// </summary>
        public WorkspaceViewModel WorkspaceViewModel
        {
            get
            {
                return workspaceViewModel;
            }
        }

        /// <summary>
        /// Refresh and/or call fresh on child viewModels
        /// </summary>
        public override void Refresh()
        {
            base.Refresh();
        }

        /// <summary>
        /// Subscribe Events and/or call subscribe on child viewModels
        /// </summary>
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
