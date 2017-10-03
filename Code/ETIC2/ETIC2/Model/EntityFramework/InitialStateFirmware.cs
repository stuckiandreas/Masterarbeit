namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("InitialStateFirmware")]
    public partial class InitialStateFirmware
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public InitialStateFirmware()
        {
            TestCollectionResult = new HashSet<TestCollectionResult>();
        }

        public int Id { get; set; }

        public int SoftwareVersions_Id_Firmware { get; set; }

        public DateTime ValveFirmwareReleaseTime { get; set; }

        public int SoftwareVersions_Id_MotionController { get; set; }

        public int SoftwareVersions_Id_Interface { get; set; }

        public int DriveParameterFile_ID { get; set; }

        public int ConfigurationParameterFile_ID { get; set; }

        public int TestCollection_Id { get; set; }

        public virtual ConfigurationParameterFile ConfigurationParameterFile { get; set; }

        public virtual DriveParameterFile DriveParameterFile { get; set; }

        public virtual SoftwareVersions SoftwareVersions { get; set; }

        public virtual SoftwareVersions SoftwareVersions1 { get; set; }

        public virtual SoftwareVersions SoftwareVersions2 { get; set; }

        public virtual TestCollection TestCollection { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<TestCollectionResult> TestCollectionResult { get; set; }
    }
}
