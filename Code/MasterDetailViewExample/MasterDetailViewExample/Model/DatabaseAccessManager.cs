using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MasterDetailViewExample.Model
{
    public abstract class DatabaseAccessManager
    {
        protected OrderListContext OrderListContext;

        public DatabaseAccessManager()
        {
            
        }

        public void ConnectDatabase()
        {
            string ConnectionString;
            
            // Initialize the connection string builder for the
            // underlying provider.
            SqlConnectionStringBuilder sqlBuilder = new SqlConnectionStringBuilder();

            // Set the properties for the data source.
            sqlBuilder.DataSource = "DESKTOP-N4UJ39P\\ANDREAS_DATABASE";
            sqlBuilder.InitialCatalog = "OrderList";
            sqlBuilder.IntegratedSecurity = true;
            sqlBuilder.MultipleActiveResultSets = true;

            ConnectionString = sqlBuilder.ToString();

            this.OrderListContext = new OrderListContext(ConnectionString);
        }
    }
}
