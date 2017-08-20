//-----------------------------------------------------------------------
// <copyright file="ErrorDatabaseDataGridView.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Views
{
    using System;
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.Xpf.Printing;

    /// <summary>
    /// Interaction logic for ErrorDatabaseDataGridView.xaml
    /// </summary>
    public partial class ErrorDatabaseDataGridView : UserControl
    {
        private string settingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings";
        private string dataGridControlSettingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings\\errorDataGirdControlSettings.xml";
        private string etic2Path = @"c:\\Test\ETIC2\Reports\";
        private string etic2PdfName = "ErrorTestResult.pdf";

        public ErrorDatabaseDataGridView()
        {
            InitializeComponent();
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows print view to select the active printer (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void Print(object sender, RoutedEventArgs e)
        {
            var link = new PrintableControlLink(ErrorView);
            link.Landscape = true;
            link.CreateDocument(true);
            link.Print();
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows a pdf version of the test results view (in landscape format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">instance containing the event data</param>
        private void PDF(object sender, RoutedEventArgs e)
        {
            if (System.IO.Path.IsPathRooted(this.etic2Path))
            {
                string fullPdfPath = this.etic2Path + this.etic2PdfName;
                var link = new PrintableControlLink(ErrorView);
                link.Landscape = true;
                link.CreateDocument(true);
                link.ExportToPdf(fullPdfPath);
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
