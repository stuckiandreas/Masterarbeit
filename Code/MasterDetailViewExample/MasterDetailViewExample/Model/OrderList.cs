namespace MasterDetailViewExample.Model
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("OrderList")]
    public partial class OrderList
    {
        public int Id { get; set; }

        [Required]
        [StringLength(100)]
        public string Name { get; set; }

        public int Value { get; set; }

        public int Price { get; set; }

        public int? Customer_Id { get; set; }

        public virtual Customer Customer { get; set; }
    }
}
