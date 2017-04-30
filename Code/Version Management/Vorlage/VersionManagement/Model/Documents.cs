namespace VersionManagement.Model
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Documents
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string Document { get; set; }

        [StringLength(100)]
        public string DocumentType { get; set; }

        public int? SoftwareVersionsId { get; set; }

        public virtual SoftwareVersions SoftwareVersions { get; set; }
    }
}
