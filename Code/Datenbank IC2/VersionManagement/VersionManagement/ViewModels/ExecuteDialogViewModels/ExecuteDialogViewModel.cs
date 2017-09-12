//-----------------------------------------------------------------------
// <copyright file="ExecuteDialogViewModel.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ViewModels.ExecuteDialogViewModels
{
    using System;
    using System.Threading.Tasks;
    using System.Windows;
    using System.Windows.Input;
    using ActionCommands;
    using Events;
    using Events.EventArgs.Cursor;
    using Events.EventArgs.Error;

    /// <summary>
    /// ViewModel to execute an Dialog. The view included an Button. This class is basis of the Save and DeleteExecuteDialogViewModel.
    /// The execute method is async. For this reason the GUI is not frozen. The user will be informed by a wait cursor.
    /// </summary>
    /// <seealso cref="VersionManagement.ViewModels.BasisViewModel" />
    public abstract class ExecuteDialogViewModel : BasisViewModel
    {
        private string buttonText;

        private ViewModelEvents viewModelEvents;

        public ExecuteDialogViewModel(ViewModelEvents viewModelEvents, string buttonText)
            : base(viewModelEvents)
        {
            this.viewModelEvents = viewModelEvents;
            this.buttonText = buttonText;
            this.ExecuteCommand = new ActionCommand(this.OnExecuteCommand);
        }

        public string ButtonText
        {
            get
            {
                return this.buttonText;
            }

            set
            {
                this.buttonText = value;
                this.OnPropertyChanged("ButtonText");
            }
        }

        public ICommand ExecuteCommand
        {
            get;
            set;
        }

        public abstract void Execute();

        private async void OnExecuteCommand(object parameter)
        {
            try
            {
                this.viewModelEvents.OnCursorLoading(this, new CursorLoadingEventArgs(true));

                var slowTask = Task.Factory.StartNew(() => this.Execute());
                await slowTask;
            }
            catch (Exception ex)
            {
                this.viewModelEvents.OnHandleError(this, new UnexpectedErrorHandlerEventArgs(ex));
            }
            finally
            {
                this.viewModelEvents.OnCursorLoading(this, new CursorLoadingEventArgs(false));
                Application.Current.Windows[1].Close();
            }
        } 
    }
}
