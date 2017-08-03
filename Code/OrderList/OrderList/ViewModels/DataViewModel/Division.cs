using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrderList.ViewModels.DataViewModel
{
    public class Division
    {
        string _Name;
        public string Name
        {
            get { return _Name; }
            set { this._Name = value; }
        }
        public ObservableCollection<Team> Teams { get; private set; }

        public Division()
        {
            Name = RandomStringHelper.GetRandomString();

            Teams = new ObservableCollection<Team>();
            for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
                Teams.Add(new Team());
        }
    }
}
