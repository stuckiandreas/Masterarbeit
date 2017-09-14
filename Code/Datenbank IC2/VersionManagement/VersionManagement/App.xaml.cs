//-----------------------------------------------------------------------
// <copyright file="App.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

// The application itself knows the Model, the ViewModels and the Views

namespace VersionManagement
{
    using System;
    using System.Windows;
    using Events;
    using Events.EventArgs.Dialog;
    using Events.EventArgs.Error;
    using Events.EventArgs.UserFeedback;
    using Model;

    /// <summary>
    /// Interaction logic for <c>App.xaml</c>
    /// </summary>
    public partial class App : Application
    {
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// On the main window is the data grid with the software item in the database visible. 
        /// Further the detail Information of the selected software item.
        /// </summary>
        private MainWindow mainWindow;

        /// <summary>
        /// Main window in which all view models are defined. The base of this view models is the workspace view model. 
        /// Here the detailInformationView and the DatabaseDataGridView are generated 
        /// </summary>
        private MainWindowViewModel mainWindowViewModel;

        /// <summary>
        /// Get and set the software item directly from the database with the entity framework.
        /// </summary>
        private VersionManagementModel model;

        /// <summary>
        /// Initializes a new instance of the <see cref="App"/> class.
        /// </summary>
        public App()
        {
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            try
            {
                base.OnStartup(e);

                // Create model

                // Create GUI
                this.viewModelEvents = new ViewModelEvents();
                this.InitMainWindow();
                this.SubscribeEvents();
                this.Init();

                // Start model (e.g. workers)
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        protected override void OnExit(ExitEventArgs e)
        {
            try
            {
                this.UnsubscribeEvents();

                // If needed, stop some workers in the Model

                base.OnExit(e);
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// First the database will initialized. 
        /// After the main window starts.
        /// </summary>
        private void InitMainWindow()
        {
            //to delete the last logfile
            LogFile.Delete();
            LogFile.Write("In InitMainWindow");
            this.model = new VersionManagementModel();
            
            string serverName = VersionManagement.Properties.Settings.Default.ServerName;
            string databaseName = VersionManagement.Properties.Settings.Default.DatabaseName;

            this.model.SelectionItemDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.model.SoftwareVersionsDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);
            this.model.DatabaseConnectionSettingsDatabaseAccessManager.BuildDatabaseContext(databaseName, serverName);

            this.mainWindowViewModel = new MainWindowViewModel(this.viewModelEvents, this.model);
            this.mainWindow = new MainWindow();
            this.mainWindow.DataContext = this.mainWindowViewModel;
            this.mainWindow.Show();
        }

        /// <summary>
        /// Check if the database is available. If result is true, the main window will load.
        /// </summary>
        private void Init()
        {
            if (!this.model.SoftwareVersionsDatabaseAccessManager.IsDatabaseAvailable())
            {
                string errorText = "Database is not available! \nDatabaseName: " + this.model.SoftwareVersionsDatabaseAccessManager.DatabaseName;
                this.viewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(errorText));
            }
            else
            {
                this.CheckDatabaseVersion();
                this.mainWindowViewModel.Init();
            }          
        }

        /// <summary>
        /// Subscribes the events.
        /// </summary>
        private void SubscribeEvents()
        {
            this.viewModelEvents.HandleError += new EventHandler<ErrorHandlerEventArgs>(this.ViewModelEvents_HandleError);
            this.viewModelEvents.UserFeedback += new EventHandler<UserFeedbackEventArgs>(this.ViewModelEvents_UserFeedback);
            this.viewModelEvents.OpenDialog += this.ViewModelEvents_OpenDialog;
            this.viewModelEvents.CursorLoading += this.ViewModelEvents_CursorLoading;

            this.mainWindowViewModel.SubscribeEvents();
        }

        /// <summary>
        /// Opens the dialog window. In which a selection item can defined or deleted. 
        /// Also the open dialog is used to define the database settings.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_OpenDialog(object sender, EventArgs e)
        {
            DialogWindowViewModel dialogWindowViewModel = new DialogWindowViewModel(this.viewModelEvents, ((DialogEventArgs)e).ViewModel, ((DialogEventArgs)e).WindowTitle, ((DialogEventArgs)e).WindowHeight, ((DialogEventArgs)e).WindowWidth);
            DialogWindow window = new DialogWindow();
            window.DataContext = dialogWindowViewModel;
            window.ShowDialog();
        }

        /// <summary>
        /// Unsubscribes the events.
        /// </summary>
        private void UnsubscribeEvents()
        {
            this.viewModelEvents.HandleError -= new EventHandler<ErrorHandlerEventArgs>(this.ViewModelEvents_HandleError);
            this.viewModelEvents.UserFeedback -= new EventHandler<UserFeedbackEventArgs>(this.ViewModelEvents_UserFeedback);
            this.viewModelEvents.OpenDialog -= this.ViewModelEvents_OpenDialog;
            this.viewModelEvents.CursorLoading -= this.ViewModelEvents_CursorLoading;

            this.mainWindowViewModel.UnsubscribeEvents();
        }

        /// <summary>
        /// Handles unexpected exceptions from the views (GUI thread) and main application.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void Application_DispatcherUnhandledException(object sender, System.Windows.Threading.DispatcherUnhandledExceptionEventArgs e)
        {
            try
            {
                this.viewModelEvents.OnUserFeedback(sender, new UserFeedbackUnexpectedErrorEventArgs(e.Exception));
                e.Handled = true;
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
        }

        /// <summary>
        /// Handles ViewModel exceptions
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_HandleError(object sender, ErrorHandlerEventArgs e)
        {
            if (e is UnexpectedErrorHandlerEventArgs)
            {
                this.viewModelEvents.OnUserFeedback(sender, new UserFeedbackUnexpectedErrorEventArgs(((UnexpectedErrorHandlerEventArgs)e).Exception));
            }
            else if (e is ExpectedErrorHandlerEventArgs)
            {
                this.viewModelEvents.OnUserFeedback(sender, new UserFeedbackErrorEventArgs(((ExpectedErrorHandlerEventArgs)e).ErrorText));
            }
            else
            {
                throw new NotImplementedException("Unknown ViewModelErrorHandlerEventArgs; " + e.GetType().Name);
            }
        }

        /// <summary>
        /// Handles UserFeedbacks
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_UserFeedback(object sender, UserFeedbackEventArgs e)
        {
            e.Result = MessageBox.Show(this.mainWindow, e.Message, e.Caption, e.Button, e.Image);
        }

        /// <summary>
        /// Cursor events shows the user that the application is running in the background.
        /// Example if the user saved a new selection item.
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="Events.EventArgs.Cursor.CursorLoadingEventArgs"/> instance containing the event data.</param>
        private void ViewModelEvents_CursorLoading(object sender, Events.EventArgs.Cursor.CursorLoadingEventArgs e)
        {
           if (e.IsCursorLoading)
            {
                System.Windows.Input.Mouse.OverrideCursor = System.Windows.Input.Cursors.Wait;
            }
           else
            {
                System.Windows.Input.Mouse.OverrideCursor = System.Windows.Input.Cursors.Arrow;
            }
        }

        /// <summary>
        /// Checks the database version.
        /// </summary>
        private void CheckDatabaseVersion()
        {
            //no database version saved in database
            int databaseVersionInDatabase = this.model.DatabaseConnectionSettingsDatabaseAccessManager.GetDatabaseVersion();
            if (databaseVersionInDatabase == 0)
                this.model.DatabaseConnectionSettingsDatabaseAccessManager.AddConfigData(this.model.DatabaseConnectionSettingsDatabaseAccessManager.DatabaseVersionString, VersionManagement.Properties.Settings.Default.DatabaseVersion);

            //check if frontend version is actually -> frontend version must the same version number or higher than the database version
            if (VersionManagement.Properties.Settings.Default.FirmwareDatabaseVersion - databaseVersionInDatabase < 0)
            {
                this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("Update the Firmware Database programm. Please contact Andreas Stucki (STUA)"));
                Environment.Exit(0);
            }
        }
    }
}
