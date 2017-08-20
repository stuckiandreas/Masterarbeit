namespace ETIC2.Model.EntityFramework
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    [Table("ValveHardware")]
    public partial class ValveHardware
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public ValveHardware()
        {
            TestCollectionResult = new HashSet<TestCollectionResult>();
        }

        public int Id { get; set; }

        public int? ValveSerie_Id { get; set; }

        public int? InterfaceType_Id { get; set; }

        public int? ControllerType_Id { get; set; }

        public int? OptionType_Id { get; set; }

        public short ExternalIsolationValveOption { get; set; }

        public int? ControllerHardwareVersion_Id { get; set; }

        public int? InterfaceHardwareVersion_Id { get; set; }

        public int? ControllerAssemblyVariant_Id { get; set; }

        public int? InterfaceAssemblyVariant_Id { get; set; }

        public int? Module1Type_Id { get; set; }

        public int? Module1HardwareVersion_Id { get; set; }

        public int? Module1AssemblyVariant_Id { get; set; }

        public int? Module2Type_Id { get; set; }

        public int? Module2HardwareVersion_Id { get; set; }

        public int? Module2AssemblyVariant_Id { get; set; }

        public int? Module3Type_Id { get; set; }

        public int? Module3HardwareVersion_Id { get; set; }

        public int? Module3AssemblyVariant_Id { get; set; }

        public int? Module4Type_Id { get; set; }

        public int? Module4HardwareVersion_Id { get; set; }

        public int? Module4AssemblyVariant_Id { get; set; }

        public virtual ControllerAssemblyVariant ControllerAssemblyVariant { get; set; }

        public virtual ControllerHardwareVersion ControllerHardwareVersion { get; set; }

        public virtual ControllerType ControllerType { get; set; }

        public virtual InterfaceAssemblyVariant InterfaceAssemblyVariant { get; set; }

        public virtual InterfaceHardwareVersion InterfaceHardwareVersion { get; set; }

        public virtual InterfaceType InterfaceType { get; set; }

        public virtual Module1AssemblyVariant Module1AssemblyVariant { get; set; }

        public virtual Module1HardwareVersion Module1HardwareVersion { get; set; }

        public virtual Module1Type Module1Type { get; set; }

        public virtual Module2AssemblyVariant Module2AssemblyVariant { get; set; }

        public virtual Module2HardwareVersion Module2HardwareVersion { get; set; }

        public virtual Module2Type Module2Type { get; set; }

        public virtual Module3AssemblyVariant Module3AssemblyVariant { get; set; }

        public virtual Module3HardwareVersion Module3HardwareVersion { get; set; }

        public virtual Module3Type Module3Type { get; set; }

        public virtual Module4AssemblyVariant Module4AssemblyVariant { get; set; }

        public virtual Module4HardwareVersion Module4HardwareVersion { get; set; }

        public virtual Module4Type Module4Type { get; set; }

        public virtual OptionType OptionType { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<TestCollectionResult> TestCollectionResult { get; set; }

        public virtual ValveSerie ValveSerie { get; set; }
    }
}
