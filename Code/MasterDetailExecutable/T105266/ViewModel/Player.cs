using DXSample.Utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace T105266.ViewModel
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
