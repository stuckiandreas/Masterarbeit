using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using BuglistIC2.Data;
using DevExpress.Mvvm.DataAnnotations;
using DevExpress.Mvvm;
using DevExpress.Mvvm.POCO;
using DevExpress.Mvvm.UI;

namespace BuglistIC2.ViewModels
{
    [POCOViewModel]
    public class BuglistViewModel
    {
        public List<BuglistInfo> Buglist
        {
            get;
            /* We only want to set this through the ViewModel code */
            protected set;
        }

        public virtual bool IsLoading
        {
            get;
            protected set;
        }

        protected BuglistViewModel()
        {
            Messenger.Default.Register<RequerySuggestedMessage>(this, OnRequerySuggested);
        }

        public static BuglistViewModel Create()
        {
            return ViewModelSource.Create(() => new BuglistViewModel());
        }

        private class RequerySuggestedMessage
        {
        }

        void OnRequerySuggested(RequerySuggestedMessage message)
        {
            LoadBuglist();
        }


        [ServiceProperty(SearchMode = ServiceSearchMode.PreferParents)]
        protected virtual IDispatcherService DispatcherService { get { return null; } }

        public Task LoadBuglist()
        {
            IsLoading = true;
            return Task.Factory.StartNew((state) =>
            {
                using (var ctx = new BuglistContext())
                {
                    // Apply the DTO pattern
                    var result = from buglist in ctx.Buglist
                                 select new BuglistInfo()
                                 {
                                     FailureType = buglist.FailureType,
                                     ControllerGeneration = buglist.ControllerGenertion,
                                     HardwareIdentificationLevel1 = buglist.HardwareIdentificationLevel1,
                                     HardwareIdentificationLevel2 = buglist.HardwareIdentificationLevel2,
                                     Bug = buglist.Bug,
                                     Comment = buglist.Comment,
                                     Priority = buglist.Priority ?? -1,
                                     DateFound = buglist.DateFound ?? DateTime.Today,
                                     DateFixed = buglist.DateFixed ?? DateTime.Today
                                 };
                    Buglist = new List<BuglistInfo>(result.ToList());
                }
                 ((IDispatcherService)state).BeginInvoke(() =>
                 {
                     this.RaisePropertyChanged(x => x.Buglist); // Notify the UI
                     IsLoading = false;
                 });

            }, DispatcherService);
        }

    }
}