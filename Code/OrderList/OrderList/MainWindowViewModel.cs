using OrderList.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrderList
{
    public class MainWindowViewModel
    {
        private WorkspaceViewModel workspaceViewModel;

        public MainWindowViewModel()
        {
            workspaceViewModel = new WorkspaceViewModel();
        }

        public WorkspaceViewModel WorkspaceViewModel
        {
            get
            {
                return workspaceViewModel;
            }
        }
    }
}
