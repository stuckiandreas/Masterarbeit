namespace VersionManagement.Model.Database
{
    using System;
    using System.Collections.Generic;
    using System.ComponentModel.DataAnnotations;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Data.Entity.Spatial;

    public partial class SoftwareVersions
    {
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2214:DoNotCallOverridableMethodsInConstructors")]
        public SoftwareVersions()
        {
            Documents = new HashSet<Documents>();
            Properties = new HashSet<Properties>();
            Softwares = new HashSet<Softwares>();
        }

        public int Id { get; set; }

        [Required]
        [StringLength(100)]
        public string Software { get; set; }

        [StringLength(100)]
        public string Base { get; set; }

        [StringLength(100)]
        public string System { get; set; }

        [StringLength(100)]
        public string Customer { get; set; }

        [StringLength(100)]
        public string Author { get; set; }

        public DateTime? Date { get; set; }

        public int? PssNumber { get; set; }

        public int? Status { get; set; }

        public int? PssStatus { get; set; }

        [StringLength(500)]
        public string Description { get; set; }

        [StringLength(500)]
        public string PrivateInfosBugs { get; set; }

        [StringLength(500)]
        public string AdditionalInformation { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Documents> Documents { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Properties> Properties { get; set; }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2227:CollectionPropertiesShouldBeReadOnly")]
        public virtual ICollection<Softwares> Softwares { get; set; }
    }
}
