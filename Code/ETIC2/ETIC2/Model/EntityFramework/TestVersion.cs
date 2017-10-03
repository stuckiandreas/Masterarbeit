namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("TestVersion")]
    public partial class TestVersion
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public TestVersion()
        {
            TestResult = new HashSet<TestResult>();
        }

        public int Id { get; set; }

        public short Number { get; set; }

        [StringLength(400)]
        public string Modification { get; set; }

        public int? TimeInSecond { get; set; }

        public int? ModulName_Id { get; set; }

        public short ExternalIsolationValveOption { get; set; }

        public int? TestInformation_Id { get; set; }

        public virtual TestInformation TestInformation { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<TestResult> TestResult { get; set; }
    }
}
