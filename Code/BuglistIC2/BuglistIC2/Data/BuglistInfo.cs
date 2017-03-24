using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.RightsManagement;
using System.Text;
using System.Threading.Tasks;

namespace BuglistIC2.Data
{
    public class BuglistInfo
    {
        public BuglistInfo()
        {
        }
       
        public int BuglistId { get; set; }
        public string FailureType { get; set; }
        public string ControllerGeneration { get; set; }
        public string HardwareIdentificationLevel1 { get; set; }
        public string HardwareIdentificationLevel2 { get; set; }
        public string Bug { get; set; }
        public string Comment { get; set; }
        public int Priority { get; set; }
        public DateTime DateFound { get; set; }
        public DateTime DateFixed { get; set; }
    }
}
