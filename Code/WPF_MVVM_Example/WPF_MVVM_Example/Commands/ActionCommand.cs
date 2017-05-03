using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace WPF_MVVM_Example.Commands
{
    class ActionCommand : ICommand
    {
        private readonly Action<object> executeHandler;
        private readonly Func<object, bool> canExecuteHandler;


        public ActionCommand(Action<object> execute)
        {
            if (execute == null)
                throw new ArgumentNullException("Execute cannot be null");

            this.executeHandler = execute;
        }

        public ActionCommand(Action<object> execute, Func<object, bool> canExecute)
            : this(execute)
        {
            this.canExecuteHandler = canExecute;
        }

        public void Execute(object parameter)
        {
            this.executeHandler(parameter);
        }


        public bool CanExecute(object parameter)
        {
            if (this.canExecuteHandler == null)
                return true;

            return this.canExecuteHandler(parameter);
        }


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
}
