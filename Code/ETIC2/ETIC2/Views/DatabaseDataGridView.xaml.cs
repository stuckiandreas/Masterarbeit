//-----------------------------------------------------------------------
// <copyright file="DatabaseDataGridView.xaml.cs" company="VAT Vakuumventile AG">
//     Copyright (c) 2017 . All rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace ETIC2.Views
{
    using System.IO;
    using System.Windows;
    using System.Windows.Controls;
    using DevExpress.Xpf.Grid;
    using DevExpress.Data.Filtering;

    /// <summary>
    /// Interaction logic for DatabaseDataGridView.xaml
    /// </summary>
    public partial class DatabaseDataGridView : UserControl
    {
        private string settingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings";
        private string dataGridControlSettingsETIC2Path = @"C:\\Program Files (x86)\\VAT\\ETIC2\\Settings\\dataGirdControlSettings.xml";

        public DatabaseDataGridView()
        {
            InitializeComponent();
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
