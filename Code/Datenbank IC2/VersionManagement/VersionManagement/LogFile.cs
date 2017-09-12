//-----------------------------------------------------------------------
// <copyright file="LogFile.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement
{
    using System;
    using System.IO;

    public static class LogFile
    {
        /// <summary>
        /// Deletes a file.
        /// </summary>
        public static void Delete()
        {
            try
            {
                File.Delete(@"C:\\Programme (x86)\\VAT\\Firmware Database\\Settings\\ReportFile.txt");
            }
            catch
            {
            }
        }

        /// <summary>
        /// Writes the specified string to logfile.
        /// </summary>
        /// <param name="stringToLogfile">The string to logfile.</param>
        public static void Write(string stringToLogfile)
        {
            string settingsFirmwareDatabasePath = @"C:\\Program Files (x86)\\VAT\\Firmware Database\\Settings";
            string logFileFirmwareDatabasePath = @"C:\\Program Files (x86)\\VAT\\Firmware Database\\Settings\\ReportFile.txt";

            //first to check if the path exist -> create if not
            if (!System.IO.File.Exists(settingsFirmwareDatabasePath))
                System.IO.Directory.CreateDirectory(settingsFirmwareDatabasePath);           
            TextWriter tw = File.AppendText(logFileFirmwareDatabasePath);

            // write a line of text to the file
            tw.WriteLine(Environment.NewLine);
            tw.WriteLine(DateTime.Now + " " + stringToLogfile);
            tw.Flush();
            tw.Close();
        }
    }
}
