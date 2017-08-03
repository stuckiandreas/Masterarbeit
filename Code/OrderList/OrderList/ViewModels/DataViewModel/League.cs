using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrderList.ViewModels.DataViewModel
{
    public class League
    {
        string _Name;
        string _Info;

        public string Name
        {
            get { return _Name; }
            set { this._Name = value; }
        }

        public string Info
        {
            get { return _Info; }
            set { this._Info = value; }
        }
        public ObservableCollection<Division> Divisions { get; private set; }

        public League()
        {
            Name = RandomStringHelper.GetRandomString();
            Info = "Test";

            Divisions = new ObservableCollection<Division>();
            for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
                Divisions.Add(new Division());
        }
    }
}
