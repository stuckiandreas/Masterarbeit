using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MasterDetailViewExample.Model;

namespace MasterDetailViewExample.ViewModel
{
    public class WorkspaceViewModel
    {
        private readonly OrderListDatabaseAccessManager orderListDatabaseAccessManager;

        public List<Customer> Customers { get; private set; }

        public List<OrderList> OrderList { get; private set; }

        public WorkspaceViewModel(OrderListDatabaseAccessManager orderListDatabaseAccessManager)
        {
            this.orderListDatabaseAccessManager = orderListDatabaseAccessManager;

            Customers = orderListDatabaseAccessManager.GetCustomerList();
            OrderList = orderListDatabaseAccessManager.GetOrderList();
        }
    }
}
