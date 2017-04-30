namespace VersionManagement.Model
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class Customers
    {
        public int Id { get; set; }

        [StringLength(100)]
        public string Customer { get; set; }
    }
}
