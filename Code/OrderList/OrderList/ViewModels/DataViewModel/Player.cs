using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrderList.ViewModels.DataViewModel
{
    public class Player
    {
        string _Name;
        public string Name
        {
            get { return _Name; }
            set { this._Name = value; }
        }

        public Player()
        {
            Name = RandomStringHelper.GetRandomString();
        }
    }
}
