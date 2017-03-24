using System;
using System.Linq;
using System.ComponentModel;
using BuglistIC2.Data;
using DevExpress.Data.ODataLinq.Helpers;
using DevExpress.Mvvm.DataAnnotations;
using DevExpress.Mvvm;
using DevExpress.Utils.OAuth;

namespace BuglistIC2.ViewModels
{
    [POCOViewModel]
    public class BugViewModel
    {
        private BuglistInfo buglist;

        public virtual object Parameter { get; set; }

        protected void OnParameterChanged()
        {
            int id = (int)Parameter;
            using (var ctx = new BuglistContext())
            {
                var result = (from buglist in ctx.Buglist
                              where buglist.Id == id
                              select new BuglistInfo()
                              {
                                  BuglistId = buglist.Id,
                                  FailureType = buglist.FailureType,
                                  ControllerGeneration = buglist.ControllerGenertion,
                                  HardwareIdentificationLevel1 = buglist.HardwareIdentificationLevel1,
                                  HardwareIdentificationLevel2 = buglist.HardwareIdentificationLevel2,
                                  Bug = buglist.Bug,
                                  Comment = buglist.Comment,
                                  Priority = buglist.Priority ?? -1,
                                  DateFound = buglist.DateFound ?? DateTime.Today,
                                  DateFixed = buglist.DateFixed ?? DateTime.Today
                              }).FirstOrDefault();
                Load(result);
            }
        }

        private void Load(BuglistInfo buglist)
        {
            this.buglist = buglist;
            this.BuglistId = BuglistId;
            this.FailureType = FailureType;
            this.ControllerGeneration = ControllerGeneration;
            this.HardwareIdentificationLevel1 = HardwareIdentificationLevel1;
            this.HardwareIdentificationLevel2 = HardwareIdentificationLevel2;
            this.Bug = Bug;
            this.Comment = Comment;
            this.Priority = Priority;
            this.DateFound = DateFound;
            this.DateFixed = DateFixed;
        }

        public int BuglistId { get; set; }
        public virtual string FailureType { get; set; }
        public virtual string ControllerGeneration { get; set; }
        public virtual string HardwareIdentificationLevel1 { get; set; }
        public virtual string HardwareIdentificationLevel2 { get; set; }
        public virtual string Bug { get; set; }
        public virtual string Comment { get; set; }
        public virtual int Priority { get; set; }
        public virtual DateTime DateFound { get; set; }
        public virtual DateTime DateFixed { get; set; }
    }
}