using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MasterDetailViewExample.ViewModel
{
    public class CustomerType
    {
        public string Surname { get; set; }

        public string Lastname { get; set; }

        public CustomerType(string Surname, string Lastname)
        {
            this.Surname = Surname;
            this.Lastname = Lastname;
        }
    }
}
