//-----------------------------------------------------------------------
// <copyright file="BuglistDatabaseDataGridView.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Views.BuglistViews
{
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.Xpf.Printing;
    using HelpFunctions;

    /// <summary>
    /// Interaction logic for BuglistDatabaseDataGridView.xaml
    /// </summary>
    public partial class BuglistDatabaseDataGridView : UserControl
    {
        /// <summary>
        /// The settings buglist database path
        /// </summary>
        private string settingsBuglistPath = @"C:\ETIC2\Settings";
        private string dataGridControlSettingsBuglistPath = @"C:\ETIC2\Settings\buglistDataGirdControlSettings.xml";
        private string buglistPath = @"c:\ETIC2\Reports\";
        private string buglistPdfName = "Buglist.pdf";
        private string buglistCsvName = "Buglist.csv";

        public BuglistDatabaseDataGridView()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows print view to select the active printer (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void Print(object sender, RoutedEventArgs e)
        {
            try
            {
                var link = new PrintableControlLink(Buglist);
                link.Landscape = true;
                link.CreateDocument(true);
                link.Print();
            }
            catch
            {
                MessageBox.Show("buglist view", "error print", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows a pdf version of the buglist view (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void PDF(object sender, RoutedEventArgs e)
        {
            //path not exist -> create directory
            if (!Directory.Exists(this.buglistPath))
            {
                System.IO.Directory.CreateDirectory(this.buglistPath);
                if (!Directory.Exists(this.buglistPath))
                {
                    MessageBox.Show("Directory not exist", "error export buglist pdf", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }

            string fullPdfPath = this.buglistPath + this.buglistPdfName;
            try
            {
                var link = new PrintableControlLink(Buglist);
                link.Landscape = true;
                link.CreateDocument(true);
                link.ExportToPdf(fullPdfPath);
            }
            catch
            {
                //check if File is already in use
                bool fileInUse = Helpers.IsFileInUse(fullPdfPath);

                // inform user, that the file is not possible to open
                if (fileInUse == true)
                    MessageBox.Show("File is already in use", "error export buglist pdf", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">describe sender</param>
        /// <param name="e">instance containing the event data</param>
        private void GridControl_Loaded(object sender, RoutedEventArgs e)
        {
            if (File.Exists(this.dataGridControlSettingsBuglistPath))
                ((GridControl)sender).RestoreLayoutFromXml(this.dataGridControlSettingsBuglistPath);
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">escribe sender</param>
        /// <param name="e">instance containing the event data</param>
        private void GridControl_LostFocus(object sender, RoutedEventArgs e)
        {
            //first to check if the path exist -> create if not
            if (!Directory.Exists(this.settingsBuglistPath)) System.IO.Directory.CreateDirectory(this.settingsBuglistPath);
            if (!Directory.Exists(this.settingsBuglistPath))
            {
                MessageBox.Show("Directory not exist", "error save grid control settings", MessageBoxButton.OK, MessageBoxImage.Error);
                return;
            }

            ((GridControl)sender).SaveLayoutToXml(this.dataGridControlSettingsBuglistPath);
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows a csv version of the buglist view (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void CSV(object sender, RoutedEventArgs e)
        {
            //path not exist -> create directory
            if (!Directory.Exists(this.buglistPath))
            {
                System.IO.Directory.CreateDirectory(this.buglistPath);
                if (!Directory.Exists(this.buglistPath))
                {
                    MessageBox.Show("Directory not exist", "error export buglist csv", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }

            string fullPdfPath = this.buglistPath + this.buglistCsvName;
            try
            {
                var link = new PrintableControlLink(Buglist);
                link.Landscape = true;
                link.CreateDocument(true);
                link.ExportToCsv(fullPdfPath);
            }
            catch
            {
                //check if File is already in use
                bool fileInUse = HelpFunctions.Helpers.IsFileInUse(fullPdfPath);

                // inform user, that the file is not possible to open
                if (fileInUse == true)
                    MessageBox.Show("File is already in use", "error export buglist csv", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
