using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Text;

namespace DXSample.Utils {
    public class RandomStringHelper {
        static Random rnd = new Random();
        public static Random Random { get { return rnd; } }

        public static string GetRandomString(int min = 6, int max = 20) {
            StringBuilder strb = new StringBuilder();
            strb.Append((char)rnd.Next(0x41, 0x5A));

            int length = rnd.Next(min, max);
            for (int i = 0; i < length - 1; i++)
                strb.Append((char)rnd.Next(0x61, 0x7A));

            return strb.ToString();
        }
    }
}

namespace DXSample {

    using DXSample.Utils;
    using T105266.ViewModel;
    public class MainViewModel {

        public WorkspaceViewModel workspaceViewModel;

        //public IList Items { get; private set; }

        //public MainViewModel()
        //{
        //    Items = new ObservableCollection<League>();

        //    for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
        //        Items.Add(new League());
        //}

        public MainViewModel()
        {
            workspaceViewModel = new WorkspaceViewModel();
        }

    }


    //public class League {
    //    string _Name;
    //    string _Info;

    //    public string Name
    //    {
    //        get { return _Name; }
    //        set { this._Name = value; }
    //    }

    //    public string Info { 
    //        get { return _Info; }
    //        set { this._Info = value; }
    //    }
    //    public ObservableCollection<Division> Divisions { get; private set; }

    //    public League() {
    //        Name = RandomStringHelper.GetRandomString();
    //        Info = "Test";

    //        Divisions = new ObservableCollection<Division>();
    //        for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
    //            Divisions.Add(new Division());
    //    }

    //}

    //public class Division  {
    //    string _Name;
    //    public string Name {
    //        get { return _Name; }
    //        set { this._Name = value; }
    //    }
    //    public ObservableCollection<Team> Teams { get; private set; }

    //    public Division() {
    //        Name = RandomStringHelper.GetRandomString();

    //        Teams = new ObservableCollection<Team>();
    //        for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
    //            Teams.Add(new Team());
    //    }
    //}

    //public class Team  {
    //    string _Name;
    //    public string Name {
    //        get { return _Name; }
    //        set { this._Name = value; }
    //    }
    //    public ObservableCollection<Player> Players { get; private set; }

    //    public Team() {
    //        Name = RandomStringHelper.GetRandomString();

    //        Players = new ObservableCollection<Player>();
    //        for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
    //            Players.Add(new Player());
    //    }
    //}

    //public class Player {
    //    string _Name;
    //    public string Name {
    //        get { return _Name; }
    //        set { this._Name = value; }
    //    }

    //    public Player() {
    //        Name = RandomStringHelper.GetRandomString();
    //    }
    //}


}

