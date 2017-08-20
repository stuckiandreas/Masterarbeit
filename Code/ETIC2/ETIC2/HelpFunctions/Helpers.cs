//-----------------------------------------------------------------------
// <copyright file="Helpers.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.HelpFunctions
{
    using System.IO;

    /// <summary>
    /// Helper class for etic2 application. To check if File is already in use
    /// </summary>
    public static class Helpers
    {
        /// <summary>
        /// Check if File is already in use.
        /// </summary>
        /// <param name="filePath">The file path.</param>
        /// <returns>
        ///   <c>true</c> if [is file in use] [the specified file path]; otherwise, <c>false</c>.
        /// </returns>
        static bool IsFileInUse(string filePath)
        {
            using (FileStream fileStream = File.Open(filePath, FileMode.Open, FileAccess.ReadWrite, FileShare.None))
            {
                if (fileStream != null)
                {
                    fileStream.Close();
                    return false;
                }
            }
            return true;
        }
    }
}
