namespace VersionManagement.Model
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("ConfigData")]
    public partial class ConfigData
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string ConfigString { get; set; }

        public int? ConfigNumber { get; set; }
    }
}
