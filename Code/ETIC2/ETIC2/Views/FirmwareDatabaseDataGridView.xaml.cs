//-----------------------------------------------------------------------
// <copyright file="FirmwareDatabaseDataGridView.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Views
{
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.Xpf.Printing;
    using HelpFunctions;

    /// <summary>
    /// Interaction logic for FirmwareDatabaseDataGridView.xaml
    /// </summary>
    public partial class FirmwareDatabaseDataGridView : UserControl
    {
        private string settingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings";
        private string dataGridControlSettingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings\\firmwareDataGirdControlSettings.xml";
        private string etic2Path = @"c:\\ETIC2\Reports\";
        private string etic2PdfName = "FirmwareTestResult.pdf";

        public FirmwareDatabaseDataGridView()
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
                var link = new PrintableControlLink(FirmwareView);
                link.Landscape = true;
                link.CreateDocument(true);
                link.Print();
            }
            catch
            {
                MessageBox.Show("firmware view", "error print", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows a pdf version of the test results view (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void PDF(object sender, RoutedEventArgs e)
        {
            //file not exist -> create directory
            if (!File.Exists(this.etic2Path))
            {
                System.IO.Directory.CreateDirectory(this.etic2Path);
                if (File.Exists(this.etic2Path))
                {
                    MessageBox.Show("Directory not exist", "error export firmware pdf", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }

            string fullPdfPath = this.etic2Path + this.etic2PdfName;
            try
            {
                var link = new PrintableControlLink(FirmwareView);
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
                    MessageBox.Show("File is already in use", "error export firmware pdf", MessageBoxButton.OK, MessageBoxImage.Error);
            }            
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">describe sender</param>
        /// <param name="e">instance containing the event data</param>
        private void GridControl_Loaded(object sender, RoutedEventArgs e)
        {
            if (File.Exists(this.dataGridControlSettingsETIC2Path))
                ((GridControl)sender).RestoreLayoutFromXml(this.dataGridControlSettingsETIC2Path);
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">escribe sender</param>
        /// <param name="e">instance containing the event data</param>
        private void GridControl_LostFocus(object sender, RoutedEventArgs e)
        {
            //first to check if the path exist -> create if not
            if (!System.IO.File.Exists(this.settingsETIC2Path)) System.IO.Directory.CreateDirectory(this.settingsETIC2Path);

            ((GridControl)sender).SaveLayoutToXml(this.dataGridControlSettingsETIC2Path);
        }
    }
}
