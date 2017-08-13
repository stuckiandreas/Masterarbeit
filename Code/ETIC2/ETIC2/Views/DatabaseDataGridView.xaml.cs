//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridView.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Views
{
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.XtraPrinting;

    /// <summary>
    /// Interaction logic for DatabaseDataGridView.xaml
    /// </summary>
    public partial class DatabaseDataGridView : UserControl
    {
        private string settingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings";
        private string dataGridControlSettingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings\\dataGirdControlSettings.xml";
        private string ETIC2PdfPath = @"c:\\Test\ETIC2\Reports\";
        private string ETIC2PdfName = "TestResult.pdf";

        public DatabaseDataGridView()
        {
            InitializeComponent();
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// Shows a print preview (direct print is not for advantage -> portrait format)
        /// </summary>
        /// <param name="sender">The sender.</param>
        /// <param name="e">The <see cref="RoutedEventArgs"/> instance containing the event data.</param>
        private void Print(object sender, RoutedEventArgs e)
        {
            view.ShowPrintPreview(this);
        }

        private void Export(object sender, RoutedEventArgs e)
        {
            if (Path.IsPathRooted(ETIC2PdfPath))
            {
                ETIC2PdfPath += ETIC2PdfName;
                view.ExportToPdf(ETIC2PdfPath);
            }
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GridControl_Loaded(object sender, RoutedEventArgs e)
        {
            if (File.Exists(dataGridControlSettingsETIC2Path))
                ((GridControl)sender).RestoreLayoutFromXml(dataGridControlSettingsETIC2Path);
        }

        /// <summary>
        /// This code has to be in code behind because it manages view information
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GridControl_LostFocus(object sender, RoutedEventArgs e)
        {
            //first to check if the path exist -> create if not
            if (!System.IO.File.Exists(settingsETIC2Path)) System.IO.Directory.CreateDirectory(settingsETIC2Path);

            ((GridControl)sender).SaveLayoutToXml(dataGridControlSettingsETIC2Path);
        }
    }
}
