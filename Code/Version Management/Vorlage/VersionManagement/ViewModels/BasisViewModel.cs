using System;
using System.ComponentModel;
using System.Windows;
using System.Windows.Input;
using VersionManagement.Model;
// Only references to the Model are allowed. References to Views are not allowed

namespace VersionManagement.ViewModels
{
    /// <summary>
    /// BasisViewModel for all ViewModels.
    /// Contains all the stuff that belongs to every ViewModel:
    /// - Logger,
    /// - Events helper class including events that are used in several view models (e.g. ErrorHandler, UserFeedbackHandler
    /// - PropertyChanged realization,
    /// - Virtual methods for SubscribeEvents, UnsubscribeEvents and Refresh
    /// </summary>
    public abstract class BasisViewModel : INotifyPropertyChanged
    {
        protected ViewModelEvents events;

        public BasisViewModel(ViewModelEvents currentEvents)
        {
            events = currentEvents;
        }

        /// <summary>
        /// Property Changed Event for Bindings
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;
        /// <summary>
        /// Property Changed Event helper method
        /// </summary>
        /// <param name="propertyName">Name of the changed Property</param>
        protected void OnPropertyChanged(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        public virtual void Refresh()
        {

        }

        public virtual void SubscribeEvents()
        {

        }

        public virtual void UnsubscribeEvents()
        {

        }

        public void OnHandleError(object sender, ErrorHandlerEventArgs e)
        {
            events.OnHandleError(sender, e);
        }

        public void OnUserFeedback(object sender, UserFeedbackEventArgs e)
        {
            events.OnUserFeedback(sender, e);
        }
    }

    /// <summary>
    /// WPF Infrastructure for Commands.
    /// </summary>
    public class ActionCommand : ICommand
    {
        private readonly Action<object> executeHandler;
        private readonly Func<object, bool> canExecuteHandler;

        /// <summary>
        /// Create Action Command from type ICommand
        /// </summary>
        /// <param name="execute">Method which is called on Action Command</param>
        public ActionCommand(Action<object> execute)
        {
            if (execute == null)
                throw new ArgumentNullException("Execute cannot be null");

            executeHandler = execute;
        }

        /// <summary>
        /// Create Action Command from type ICommand
        /// </summary>
        /// <param name="execute">Method which is called on Action Command</param>
        /// <param name="canExecute">Check method if the command can be executed</param>
        public ActionCommand(Action<object> execute, Func<object, bool> canExecute)
            : this(execute)
        {
            canExecuteHandler = canExecute;
        }

        /// <summary>
        /// Execute methode for ICommands
        /// </summary>
        /// <param name="parameter">optional parameter</param>
        public void Execute(object parameter)
        {
            executeHandler(parameter);
        }

        /// <summary>
        /// Check if execute-method can execute
        /// </summary>
        /// <param name="parameter">optional parameter</param>
        /// <returns>returns true if xecute-method can execute, else false</returns>
        public bool CanExecute(object parameter)
        {
            if (canExecuteHandler == null)
                return true;

            return canExecuteHandler(parameter);
        }

        /// <summary>
        /// Event handler for executed method
        /// </summary>
        public event EventHandler CanExecuteChanged
        {
            add
            {
                CommandManager.RequerySuggested += value;
            }
            remove
            {
                CommandManager.RequerySuggested -= value;
            }
        }
    }

    /// <summary>
    /// Helper class to encapsulate events that should be available in all ViewModels.
    /// Because always the same instance of events has to be used, the reference of this helper class must be provided each time a ViewModel is instanciated.
    /// The helper class is needed because events may not be used as a parameter for reference assignment.
    /// </summary>
    public class ViewModelEvents
    {
        public event EventHandler<ErrorHandlerEventArgs> HandleError;
        public event EventHandler<UserFeedbackEventArgs> UserFeedback;

        public void OnHandleError(object sender, ErrorHandlerEventArgs errorEventArgs)
        {
            if (HandleError != null)
                HandleError(sender, errorEventArgs);
        }

        public void OnUserFeedback(object sender, UserFeedbackEventArgs userFeedbackEventArgs)
        {
            if (UserFeedback != null)
                UserFeedback(sender, userFeedbackEventArgs);
        }
    }

    /// <summary>
    /// Basis class for all ViewModelErrorHandler EventArgs
    /// </summary>
    public abstract class ErrorHandlerEventArgs : EventArgs
    {
        /// <summary>
        /// Flag if the someone handled this error
        /// </summary>
        private bool isHandled;

        public ErrorHandlerEventArgs()
        {

        }

        /// <summary>
        /// Get/set the ishandled Flag, to check if someone handled the ErrorEvent
        /// </summary>
        public bool IsHandled
        {
            get
            {
                return isHandled;
            }
            set
            {
                isHandled = value;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "IsHandled; " + IsHandled.ToString();
        }
    }

    public class UnexpectedErrorHandlerEventArgs : ErrorHandlerEventArgs
    {
        private Exception exception;

        public UnexpectedErrorHandlerEventArgs(Exception currentException)
        {
            exception = currentException;
        }

        public Exception Exception
        {
            get
            {
                return exception;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "Exception; " + Exception.ToString();
        }
    }

    public class ExpectedErrorHandlerEventArgs : ErrorHandlerEventArgs
    {
        private string errorText;

        public ExpectedErrorHandlerEventArgs(string currentErrorText)
        {
            errorText = currentErrorText;
        }

        public string ErrorText
        {
            get
            {
                return errorText;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "ErrorText; " + errorText;
        }
    }

    /// <summary>
    /// Basis class for all UserFeedback EventArgs
    /// These class is a ViewModel for MessageBoxes
    /// </summary>
    public abstract class UserFeedbackEventArgs : EventArgs
    {
        private string caption;
        private string message;
        private MessageBoxButton button;
        private MessageBoxImage image;
        private MessageBoxResult result = MessageBoxResult.None;

        public UserFeedbackEventArgs()
        {

        }

        public UserFeedbackEventArgs(string currentCaption, string currentMessage, MessageBoxButton currentButton, MessageBoxImage currentImage)
            : base()
        {
            caption = currentCaption;
            message = currentMessage;
            button = currentButton;
            image = currentImage;
        }

        public string Caption
        {
            get
            {
                return caption;
            }
            protected set
            {
                caption = value;
            }
        }

        public string Message
        {
            get
            {
                return message;
            }
            protected set
            {
                message = value;
            }
        }

        public MessageBoxButton Button
        {
            get
            {
                return button;
            }
            protected set
            {
                button = value;
            }
        }

        public MessageBoxImage Image
        {
            get
            {
                return image;
            }
            protected set
            {
                image = value;
            }
        }

        public MessageBoxResult Result
        {
            get
            {
                return result;
            }
            set
            {
                result = value;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "Caption; " + caption + "; " +
                "Message; " + message;
        }
    }

    public class UserFeedbackUnexpectedErrorEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackUnexpectedErrorEventArgs(Exception exception)
        {
            base.Caption = "Unexpected Error";
            base.Message = exception.ToString();
            base.Image = MessageBoxImage.Error;
            base.Button = MessageBoxButton.OK;
        }
    }

    public class UserFeedbackErrorEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackErrorEventArgs(string errorMessage)
        {
            base.Caption = "Error";
            base.Message = errorMessage;
            base.Image = MessageBoxImage.Error;
            base.Button = MessageBoxButton.OK;
        }
    }

    public class UserFeedbackInfoEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackInfoEventArgs(string infoMessage)
        {
            base.Caption = "Information";
            base.Message = infoMessage;
            base.Image = MessageBoxImage.Information;
            base.Button = MessageBoxButton.OK;
        }
    }

    public class UserFeedbackQuestionEventArgs : UserFeedbackEventArgs
    {
        public UserFeedbackQuestionEventArgs(string questionMessage)
        {
            base.Caption = "Confirmation";
            base.Message = questionMessage;
            base.Image = MessageBoxImage.Question;
            base.Button = MessageBoxButton.YesNo;
        }
    }
}
