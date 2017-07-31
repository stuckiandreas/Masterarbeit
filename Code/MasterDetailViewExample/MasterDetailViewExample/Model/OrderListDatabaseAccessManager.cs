using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MasterDetailViewExample.Model
{
    public class OrderListDatabaseAccessManager : DatabaseAccessManager
    {
        public OrderListDatabaseAccessManager()
        {
            
        }

        public List<Customer> GetCustomerList()
        {
            return OrderListContext.Customer.ToList();
        }

        public List<OrderList> GetOrderList()
        {
            return OrderListContext.OrderList.ToList();
        } 
    }
}
