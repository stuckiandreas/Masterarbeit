//-----------------------------------------------------------------------
// <copyright file="ActionCommand.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.ActionCommands
{
    using System;
    using System.Windows.Input;

    /// <summary>
    /// WPF Infrastructure for Commands.
    /// </summary>
    public class ActionCommand : ICommand
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

        public void Execute(object parameter)
        {
            this.executeHandler(parameter);
        }

        /// <summary>
        /// Check if the action is workable
        /// </summary>
        /// <param name="parameter">optional parameter</param>
        /// <returns>returns true if execute-method can execute, else false</returns>
        public bool CanExecute(object parameter)
        {
            if (this.canExecuteHandler == null)
                return true;

            return this.canExecuteHandler(parameter);
        }
    }
}
