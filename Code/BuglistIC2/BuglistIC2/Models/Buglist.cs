namespace BuglistIC2
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("Buglist")]
    public partial class Buglist
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string FailureType { get; set; }

        [StringLength(100)]
        public string ControllerGenertion { get; set; }

        [StringLength(100)]
        public string HardwareIdentificationLevel1 { get; set; }

        [StringLength(100)]
        public string HardwareIdentificationLevel2 { get; set; }

        [StringLength(200)]
        public string Bug { get; set; }

        [StringLength(200)]
        public string Comment { get; set; }

        public int? Priority { get; set; }

        public DateTime? DateFound { get; set; }

        public DateTime? DateFixed { get; set; }
    }
}
