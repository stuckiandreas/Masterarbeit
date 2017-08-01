using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using MasterDetailViewExample.Model;
using MasterDetailViewExample.ViewModel;

namespace MasterDetailViewExample
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        private OrderListDatabaseAccessManager orderListDatabaseAccessManager;

        private WorkspaceViewModel workspaceViewModel;

        public App()
        {
            orderListDatabaseAccessManager = new OrderListDatabaseAccessManager();
            workspaceViewModel = new WorkspaceViewModel(orderListDatabaseAccessManager);
        }
    }
}
