using DXSample.Utils;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace T105266.ViewModel
{
    public class Team
    {
        string _Name;
        public string Name
        {
            get { return _Name; }
            set { this._Name = value; }
        }
        public ObservableCollection<Player> Players { get; private set; }

        public Team()
        {
            Name = RandomStringHelper.GetRandomString();

            Players = new ObservableCollection<Player>();
            for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
                Players.Add(new Player());
        }
    }
}
