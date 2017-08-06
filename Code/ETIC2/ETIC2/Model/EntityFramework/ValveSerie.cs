namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("ValveSerie")]
    public partial class ValveSerie
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public ValveSerie()
        {
            ValveHardware = new HashSet<ValveHardware>();
        }

        public int Id { get; set; }

        public short EnumIndex { get; set; }

        public int EnumValue { get; set; }

        [StringLength(40)]
        public string Enum { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<ValveHardware> ValveHardware { get; set; }
    }
}
