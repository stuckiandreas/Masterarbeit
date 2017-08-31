namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("TestErrorMessage")]
    public partial class TestErrorMessage
    {
        public int Id { get; set; }

        [StringLength(400)]
        public string Description { get; set; }

        public int? TestResult_Id { get; set; }

        public virtual TestResult TestResult { get; set; }
    }
}
