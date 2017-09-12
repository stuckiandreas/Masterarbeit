//-----------------------------------------------------------------------
// <copyright file="VersionManagementException.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2015 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Exceptions
{
    using System;

    /// <summary>
    /// Exception used for this application to throw further. If such an exception is cached,
    /// it is an expected error and the details are already logged where the error occurred.
    /// </summary>
    public class VersionManagementException : ApplicationException
    {
        public VersionManagementException(string message)
            : base(message)
        {
        }

        public VersionManagementException(string message, Exception innerException)
            : base(message, innerException)
        {
        }
    }
}
