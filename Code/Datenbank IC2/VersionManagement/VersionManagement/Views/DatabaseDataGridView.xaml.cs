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
    }
}
