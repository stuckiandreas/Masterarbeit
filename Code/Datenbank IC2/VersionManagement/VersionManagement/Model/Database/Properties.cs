namespace VersionManagement.Model.Database
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Properties
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string PropertyType { get; set; }

        [StringLength(100)]
        public string Description { get; set; }

        public int? SoftwareVersionsId { get; set; }

        public virtual SoftwareVersions SoftwareVersions { get; set; }
    }
}