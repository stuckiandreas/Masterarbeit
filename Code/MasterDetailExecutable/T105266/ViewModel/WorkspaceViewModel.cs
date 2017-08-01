using DXSample.Utils;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;

namespace T105266.ViewModel
{
    public class WorkspaceViewModel
    {
        public IList Items { get; private set; }

        public WorkspaceViewModel()
        {
            Items = new ObservableCollection<League>();

            for (int i = 0; i < RandomStringHelper.Random.Next(5, 10); i++)
                Items.Add(new League());
        }

    }
}
