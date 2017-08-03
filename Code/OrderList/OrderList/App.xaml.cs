using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace OrderList
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private MainWindowViewModel mainWindowViewModel;

        private MainWindow mainWindow;

        public App()
        {

        }

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            mainWindowViewModel = new MainWindowViewModel();
            mainWindow = new MainWindow();
            mainWindow.DataContext = mainWindowViewModel;
            mainWindow.Show();
        }
    }
}
