//-----------------------------------------------------------------------
// <copyright file="ErrorHandlerEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Events.EventArgs.Error
{
    /// <summary>
    /// Basis class for all ViewModelErrorHandler EventArgs
    /// </summary>
    public abstract class ErrorHandlerEventArgs : System.EventArgs
    {
        /// <summary>
        /// Flag if the someone handled this error
        /// </summary>
        private bool isHandled;

        public ErrorHandlerEventArgs()
        {
        }

        public bool IsHandled
        {
            get
            {
                return this.isHandled;
            }

            set
            {
                this.isHandled = value;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "IsHandled; " + this.IsHandled.ToString();
        }
    }
}
