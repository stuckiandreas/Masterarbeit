namespace ETIC2.Model.EntityFramework
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

        public int? FailureType_Id { get; set; }

        public int? StatusType_Id { get; set; }

        public int? ControllerType_Id { get; set; }

        public int? HardwareIdentificationLevel1_Id { get; set; }

        public int? HardwareIdentificationLevel2_Id { get; set; }

        [Required]
        [StringLength(200)]
        public string Bug { get; set; }

        [StringLength(200)]
        public string Comment { get; set; }

        public int? Priority_Id { get; set; }

        public DateTime DateFound { get; set; }

        public DateTime? DateFixed { get; set; }

        public virtual ControllerType ControllerType { get; set; }

        public virtual FailureType FailureType { get; set; }

        public virtual HardwareIdentificationLevel1 HardwareIdentificationLevel1 { get; set; }

        public virtual HardwareIdentificationLevel2 HardwareIdentificationLevel2 { get; set; }

        public virtual Priority Priority { get; set; }

        public virtual StatusType StatusType { get; set; }
    }
}
