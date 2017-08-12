//-----------------------------------------------------------------------
// <copyright file="App.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2
{
    using System;
    using System.Windows;
    using Events;
    using Events.EventArgs.Dialog;
    using Events.EventArgs.Error;
    using Events.EventArgs.UserFeedback;
    using Model.Application;
    
    /// <summary>
    /// Interaction logic for <c>App.xaml</c>
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Events which are available in all view models.
        /// </summary>
        private ViewModelEvents viewModelEvents;

        /// <summary>
        /// On the main window is the data grid with the test results. 
        /// </summary>
        private MainWindow mainWindow;

        /// <summary>
        /// Main window in which all view models are defined. The base of this view models is the workspace view model. 
        /// Here the DatabaseDataGridView are generated 
        /// </summary>
        private MainWindowViewModel mainWindowViewModel;

        /// <summary>
        /// Gets the test result items directly from the database with entity framework.
        /// </summary>
        private ETIC2Model etic2Model;

        public App()
        {
        }

        protected override void OnStartup(StartupEventArgs e)
        {
            try
            {
                base.OnStartup(e);

                //Create GUI
                this.viewModelEvents = new ViewModelEvents();
                this.InitMainWindow();
                this.SubscribeEvents();
                this.Init();
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
            this.etic2Model = new ETIC2Model();
            this.etic2Model.BuildDatabaseContext();
            
            this.mainWindowViewModel = new MainWindowViewModel(this.viewModelEvents, this.etic2Model);
            this.mainWindow = new MainWindow();
            this.mainWindow.DataContext = this.mainWindowViewModel;
            this.mainWindow.Show();
        }

        /// <summary>
        /// Check if the database is available. If result is true, the main window will load.
        /// </summary>
        private void Init()
        {
            if (!this.etic2Model.InitialStateFirmwareDatabaseAccessManager.IsDatabaseAvailable())
            {
                string errorText = "Database is not available! \nDatabaseName: " + this.etic2Model.InitialStateFirmwareDatabaseAccessManager.DatabaseName;
                this.viewModelEvents.OnHandleError(this, new ExpectedErrorHandlerEventArgs(errorText));
            }
            else
            {
                this.CheckDatabaseVersion();
                this.mainWindowViewModel.Init();
            }
        }

        private void SubscribeEvents()
        {
            this.viewModelEvents.HandleError += new EventHandler<ErrorHandlerEventArgs>(this.ViewModelEvents_HandleError);
            this.viewModelEvents.UserFeedback += new EventHandler<UserFeedbackEventArgs>(this.ViewModelEvents_UserFeedback);
            this.viewModelEvents.OpenDialog += this.ViewModelEvents_OpenDialog;
            this.viewModelEvents.CursorLoading += this.ViewModelEvents_CursorLoading;

            this.mainWindowViewModel.SubscribeEvents();
        }

        private void UnsubscribeEvents()
        {
            this.viewModelEvents.HandleError -= new EventHandler<ErrorHandlerEventArgs>(this.ViewModelEvents_HandleError);
            this.viewModelEvents.UserFeedback -= new EventHandler<UserFeedbackEventArgs>(this.ViewModelEvents_UserFeedback);
            this.viewModelEvents.OpenDialog -= this.ViewModelEvents_OpenDialog;
            this.viewModelEvents.CursorLoading -= this.ViewModelEvents_CursorLoading;

            this.mainWindowViewModel.UnsubscribeEvents();
        }

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
        /// Opens the dialog window. To set the database settings.
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

        private void ViewModelEvents_UserFeedback(object sender, UserFeedbackEventArgs e)
        {
            e.Result = MessageBox.Show(this.mainWindow, e.Message, e.Caption, e.Button, e.Image);
        }

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

        private void CheckDatabaseVersion()
        {
            //no database version saved in database
            int databaseVersionInDatabase = this.etic2Model.DatabaseConnectionSettingsDatabaseAccessManager.GetDatabaseVersion();
            if (databaseVersionInDatabase == 0)
                this.etic2Model.DatabaseConnectionSettingsDatabaseAccessManager.AddConfigData(this.etic2Model.DatabaseConnectionSettingsDatabaseAccessManager.DatabaseVersionString, ETIC2.Properties.Settings.Default.DatabaseVersion);

            //check if frontend version is actually -> frontend version must the same version number or higher than the database version
            if (ETIC2.Properties.Settings.Default.ETIC2Version - databaseVersionInDatabase < 0)
            {
                this.viewModelEvents.OnUserFeedback(this, new UserFeedbackErrorEventArgs("Update the Firmware Database programm. Please contact Andreas Stucki (STUA)"));
                Environment.Exit(0);
            }
        }
    }
}
