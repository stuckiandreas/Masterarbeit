namespace VersionManagement.Model.Database
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class PropertyTypes
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string PropertyType { get; set; }

        [StringLength(100)]
        public string PropertyDescription { get; set; }
    }
}
