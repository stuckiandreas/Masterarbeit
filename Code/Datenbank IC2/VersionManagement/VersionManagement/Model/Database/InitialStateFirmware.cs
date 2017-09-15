namespace VersionManagement.Model.Database
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("InitialStateFirmware")]
    public partial class InitialStateFirmware
    {
        public int Id { get; set; }

        public int SoftwareVersions_Id_Firmware { get; set; }

        public DateTime ValveFirmwareReleaseTime { get; set; }

        public int SoftwareVersions_Id_MotionController { get; set; }

        public int SoftwareVersions_Id_Interface { get; set; }

        public int DriveParameterFile_ID { get; set; }

        public int ConfigurationParameterFile_ID { get; set; }

        public int TestCollection_Id { get; set; }

        public virtual SoftwareVersions SoftwareVersions { get; set; }

        public virtual SoftwareVersions SoftwareVersions1 { get; set; }

        public virtual SoftwareVersions SoftwareVersions2 { get; set; }
    }
}
