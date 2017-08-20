//-----------------------------------------------------------------------
// <copyright file="ExpectedErrorHandlerEventArgs.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace ETIC2.Events.EventArgs.Error
{
    /// <summary>
    /// Describes and catch the expected error
    /// </summary>
    /// <seealso cref="VersionManagement.Events.EventArgs.Error.ErrorHandlerEventArgs" />
    public class ExpectedErrorHandlerEventArgs : ErrorHandlerEventArgs
    {
        private string errorText;

        public ExpectedErrorHandlerEventArgs(string currentErrorText)
        {
            this.errorText = currentErrorText;
        }

        public string ErrorText
        {
            get
            {
                return this.errorText;
            }
        }

        public override string ToString()
        {
            return base.ToString() + "; " +
                "ErrorText; " + this.errorText;
        }
    }
}
