using System;
using System.Windows.Input;
using VersionManagement.Model;
// Only references to the Model are allowed. References to Views are not allowed

namespace VersionManagement.ViewModels
{
    public class WorkspaceViewModel : BasisViewModel
    {
        // Reference to the model
        private VersionManagementModel model;
        private int numberClicked;

        // For this ViewModel relevant information
        private string status;

        public WorkspaceViewModel(ViewModelEvents currentEvents)
            : base(currentEvents)
        {
            // Store the model
            model = new VersionManagementModel();

            // Initialize ViewModel relevant things
            Init();
            Status = "...";
        }

        private void Init()
        {
            // Create all commands
            ExitCommand = new ActionCommand(OnExitCommand);
            Command = new ActionCommand(OnCommand);
        }

        public override void SubscribeEvents()
        {
            // Subscribe own model events

            // Subscribe base class events
            base.SubscribeEvents();
        }

        public override void UnsubscribeEvents()
        {
            // Unsubscribe own model events

            // Unsubscribe base class events
            base.UnsubscribeEvents();
        }

        public override void Refresh()
        {
            // Refresh own informations

            // Refresh childs

            // Refresh base class
            base.Refresh();
        }

        // Command declarations (First, the command property, then the On..Command method, then (if existing) the On..CommandCanExecute method
        public ICommand Command
        {
            get;
            private set;
        }

        private void OnCommand(object parameter)
        {
            try
            {

                //OnUserFeedback(this, new UserFeedback"InfoEventArgs("Mein Name ist Flinit und ich bin " + model.Age + " Jahre alt"));
                Console.WriteLine("Hallo hier bin ich");
                Status = (++numberClicked).ToString();
            }
            catch (Exception ex)
            {
                //log.Error("Unexpected error; " + ex.Message, ex);

                OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        public ICommand AboutCommand
        {
            get;
            private set;
        }

        public ICommand ExitCommand
        {
            get;
            private set;
        }

        private void OnExitCommand(object parameter)
        {
            try
            {
                App.Current.Shutdown();
            }
            catch (VersionManagementExceptionException ex)
            {
                // Already logged

                OnHandleError(this, new ExpectedErrorHandlerEventArgs(ex.Message));
            }
            catch (Exception ex)
            {
                //log.Error("Unexpected error; " + ex.Message, ex);

                OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        // For this ViewModel relevant informations

        public string Status
        {
            get
            {
                return status;
            }
            private set
            {
                status = value;
                OnPropertyChanged("Status");
            }
        }
    }
}
