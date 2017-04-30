using System;
using System.Reflection;
using System.Windows;
using VersionManagement.Model;
using VersionManagement.ViewModels;
using VersionManagement.Views;
// The application itself knows the Model, the ViewModels and the Views

namespace VersionManagement
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private ViewModelEvents viewModelEvents;

        private MainWindow mainWindow;
        private MainWindowViewModel mainWindowViewModel;
        private VersionManagementModel model;

        public App()
        {

        }

        protected override void OnStartup(StartupEventArgs e)
        {
            try
            {
                base.OnStartup(e);

                // Create model
                //TODO Create analyzer here

                // Create GUI
                viewModelEvents = new ViewModelEvents();
                InitMainWindow();
                SubscribeEvents();

                // Start model (e.g. workers)
            }
            catch (Exception ex)
            {
                //log.Error("Unexpected error in OnStartup", ex);
            }
        }

        protected override void OnExit(ExitEventArgs e)
        {
            try
            {
                UnsubscribeEvents();

                // If needed, stop some workers in the Model
                //TODO Stop analyzer here

                base.OnExit(e);
            }
            catch (Exception ex)
            {
                //log.Error("Unexpected error in OnExit", ex);
            }
        }

        private void InitMainWindow()
        {
            model = new VersionManagementModel();
            mainWindowViewModel = new MainWindowViewModel(viewModelEvents, model);
            mainWindow = new MainWindow();
            mainWindow.DataContext = mainWindowViewModel;
            mainWindow.Show();
        }

        private void SubscribeEvents()
        {
            viewModelEvents.HandleError += new EventHandler<ErrorHandlerEventArgs>(viewModelEvents_HandleError);
            viewModelEvents.UserFeedback += new EventHandler<UserFeedbackEventArgs>(viewModelEvents_UserFeedback);

            mainWindowViewModel.SubscribeEvents();
        }

        private void UnsubscribeEvents()
        {
            viewModelEvents.HandleError -= new EventHandler<ErrorHandlerEventArgs>(viewModelEvents_HandleError);
            viewModelEvents.UserFeedback -= new EventHandler<UserFeedbackEventArgs>(viewModelEvents_UserFeedback);

            mainWindowViewModel.UnsubscribeEvents();
        }

        /// <summary>
        /// Handles unexpected exceptions from the views (gui thread) and main application.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Application_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            //log.Error("Unexpected error in GUI thread", e.Exception);

            viewModelEvents.OnUserFeedback(sender, new UserFeedbackUnexpectedErrorEventArgs(e.Exception));
            e.Handled = true;
        }

        /// <summary>
        /// Handles ViewModel exceptions
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void viewModelEvents_HandleError(object sender, ErrorHandlerEventArgs e)
        {
            if (e is UnexpectedErrorHandlerEventArgs)
            {
                viewModelEvents.OnUserFeedback(sender, new UserFeedbackUnexpectedErrorEventArgs(((UnexpectedErrorHandlerEventArgs)e).Exception));
            }
            else if (e is ExpectedErrorHandlerEventArgs)
            {
                viewModelEvents.OnUserFeedback(sender, new UserFeedbackErrorEventArgs(((ExpectedErrorHandlerEventArgs)e).ErrorText));
            }
            else
            {
                throw new NotImplementedException("Unknown ViewModelErrorHandlerEventArgs; " + e.GetType().Name);
            }
        }

        /// <summary>
        /// Handles UserFeedbacks
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void viewModelEvents_UserFeedback(object sender, UserFeedbackEventArgs e)
        {
            e.Result = MessageBox.Show(mainWindow, e.Message, e.Caption, e.Button, e.Image);
        }
    }

    /// <summary>
    /// Exception used for this application to throw further. If such an exception is catched,
    /// it is an expected error and the details are already logged where the error occured
    /// </summary>
    public class VersionManagementExceptionException : ApplicationException
    {
        public VersionManagementExceptionException(string message)
            : base(message)
        {
            //TODO Needed?, Right place?
        }

        public VersionManagementExceptionException(string message, Exception innerException)
            : base(message, innerException)
        {

        }
    }
}
