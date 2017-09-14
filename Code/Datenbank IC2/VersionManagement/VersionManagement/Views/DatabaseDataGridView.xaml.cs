//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridView.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace VersionManagement.Views
{
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.Xpf.Printing;

    /// <summary>
    /// Interaction logic for DatabaseDataGridView
    /// </summary>
    public partial class DatabaseDataGridView : UserControl
    {
        /// <summary>
        /// The settings firmware database path
        /// </summary>
        private string settingsFirmwareDatabasePath = @"C:\\Program Files (x86)\\VAT\\Firmware Database\\Settings";
        private string dataGridControlSettingsFirmwareDatabasePath = @"C:\\Program Files (x86)\\VAT\\Firmware Database\\Settings\\dataGirdControlSettings.xml";
        private string FirmwareDatabasePath = @"\\Hq.vat\chvat\metpd\Development\Software\Firmware Database\\Reports\";
        private string FirmwareDatabasePdfName = "FirmwareDatabase.pdf";

        /// <summary>
        /// Initializes a new instance of the <see cref="DatabaseDataGridView"/> class.
        /// </summary>
        public DatabaseDataGridView()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void GridControl_Loaded(object sender, RoutedEventArgs e)
        {
            if (File.Exists(this.dataGridControlSettingsFirmwareDatabasePath))
                ((GridControl)sender).RestoreLayoutFromXml(this.dataGridControlSettingsFirmwareDatabasePath);
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender">The source of the event.</param>
        /// <param name="e">The <see cref="EventArgs"/> instance containing the event data.</param>
        private void GridControl_LostFocus(object sender, RoutedEventArgs e)
        {
            //first to check if the path exist -> create if not
            if (!System.IO.File.Exists(this.settingsFirmwareDatabasePath)) System.IO.Directory.CreateDirectory(this.settingsFirmwareDatabasePath);

            ((GridControl)sender).SaveLayoutToXml(this.dataGridControlSettingsFirmwareDatabasePath);
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
                var link = new PrintableControlLink(FirmwareDatabase);
                link.Landscape = true;
                link.CreateDocument(true);
                link.Print();
            }
            catch
            {
                MessageBox.Show("firmware database", "error print", MessageBoxButton.OK, MessageBoxImage.Error);
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
            //path not exist -> create directory
            if (!Directory.Exists(this.FirmwareDatabasePath))
            {
                System.IO.Directory.CreateDirectory(this.FirmwareDatabasePath);
                if (!Directory.Exists(this.FirmwareDatabasePath))
                {
                    MessageBox.Show("Directory not exist", "error export firmware datatbase pdf", MessageBoxButton.OK, MessageBoxImage.Error);
                    return;
                }
            }

            string fullPdfPath = this.FirmwareDatabasePath + this.FirmwareDatabasePdfName;
            try
            {
                var link = new PrintableControlLink(FirmwareDatabase);
                link.Landscape = true;
                link.CreateDocument(true);
                link.ExportToPdf(fullPdfPath);
            }
            catch
            {
                //check if File is already in use
                bool fileInUse = HelpFunctions.Helpers.IsFileInUse(fullPdfPath);

                // inform user, that the file is not possible to open
                if (fileInUse == true)
                    MessageBox.Show("File is already in use", "error export firmware pdf", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
