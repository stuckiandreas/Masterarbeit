namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("ControllerAssemblyVariant")]
    public partial class ControllerAssemblyVariant
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public ControllerAssemblyVariant()
        {
            ValveHardware = new HashSet<ValveHardware>();
        }

        public int Id { get; set; }

        public short EnumIndex { get; set; }

        public short EnumValue { get; set; }

        [StringLength(100)]
        public string Enum { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<ValveHardware> ValveHardware { get; set; }
    }
}
