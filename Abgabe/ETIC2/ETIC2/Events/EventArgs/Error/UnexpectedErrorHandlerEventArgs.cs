//-----------------------------------------------------------------------
// <copyright file="UnexpectedErrorHandlerEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.Error
{
    using System;

    /// <summary>
    /// Describes and catch the unexpected error
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.Error.ErrorHandlerEventArgs" />
    public class UnexpectedErrorHandlerEventArgs : ErrorHandlerEventArgs
    {
        private Exception exception;

        public UnexpectedErrorHandlerEventArgs(Exception currentException)
        {
            this.exception = currentException;
        }

        public Exception Exception
        {
            get
            {
                return this.exception;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "Exception; " + this.Exception.StackTrace.ToString();
        }
    }
}
