namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("TestResult")]
    public partial class TestResult
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public TestResult()
        {
            TestErrorMessage = new HashSet<TestErrorMessage>();
        }

        public int Id { get; set; }

        public DateTime StartTime { get; set; }

        public DateTime EndTime { get; set; }

        [Column("TestResult")]
        public short TestResult1 { get; set; }

        public int? TestCollectionResult_Id { get; set; }

        public int? TestVersion_Id { get; set; }

        public virtual TestCollectionResult TestCollectionResult { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<TestErrorMessage> TestErrorMessage { get; set; }

        public virtual TestVersion TestVersion { get; set; }
    }
}
