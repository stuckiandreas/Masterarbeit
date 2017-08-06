namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("TestCollectionResult")]
    public partial class TestCollectionResult
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public TestCollectionResult()
        {
            TestResult = new HashSet<TestResult>();
        }

        public int Id { get; set; }

        public DateTime? ExecutionDateTime { get; set; }

        [StringLength(30)]
        public string UserName { get; set; }

        public int? AbortType_Id { get; set; }

        public short? CountErrorTest { get; set; }

        public int? ValveHardware_Id { get; set; }

        public int? InitialStateFirmware_Id { get; set; }

        public virtual AbortType AbortType { get; set; }

        public virtual InitialStateFirmware InitialStateFirmware { get; set; }

        public virtual ValveHardware ValveHardware { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<TestResult> TestResult { get; set; }
    }
}
