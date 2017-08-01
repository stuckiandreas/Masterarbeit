using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.TextFormatting;
using MasterDetailViewExample.Model;

namespace MasterDetailViewExample.ViewModel
{
    public class WorkspaceViewModel
    {
        private readonly OrderListDatabaseAccessManager orderListDatabaseAccessManager;

        private List<Customer> Customers;

        public List<OrderList> OrderList { get; private set; }

        public ObservableCollection<CustomerType> CustomerList { get; private set; }

        private string flinty = "flinty";

        public string Flinty
        {
            get { return flinty; }
            set { flinty = value;  }
        }

        public WorkspaceViewModel(OrderListDatabaseAccessManager orderListDatabaseAccessManager)
        {
            this.orderListDatabaseAccessManager = orderListDatabaseAccessManager;

            Customers = orderListDatabaseAccessManager.GetCustomerList();
            OrderList = orderListDatabaseAccessManager.GetOrderList();

            for (int i = 0; i < Customers.Count; i++)
            {
                if (CustomerList == null)   CustomerList = new ObservableCollection<CustomerType>();
                CustomerList.Add(new CustomerType(Customers[i].Surname, Customers[i].Lastname));
            }
        }
    }
}
