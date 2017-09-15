namespace VersionManagement.Model.Database
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class SoftwareVersionsDatabaseContext : DbContext
    {
        public SoftwareVersionsDatabaseContext(string connection)
            : base(connection)
        {
        }

        public virtual DbSet<Authors> Authors { get; set; }
        public virtual DbSet<BaseSoftwares> BaseSoftwares { get; set; }
        public virtual DbSet<ConfigData> ConfigData { get; set; }
        public virtual DbSet<Customers> Customers { get; set; }
        public virtual DbSet<Documents> Documents { get; set; }
        public virtual DbSet<DocumentTypes> DocumentTypes { get; set; }
        public virtual DbSet<InitialStateFirmware> InitialStateFirmware { get; set; }
        public virtual DbSet<Properties> Properties { get; set; }
        public virtual DbSet<PropertyTypes> PropertyTypes { get; set; }
        public virtual DbSet<Softwares> Softwares { get; set; }
        public virtual DbSet<SoftwareVersions> SoftwareVersions { get; set; }
        public virtual DbSet<Systems> Systems { get; set; }
        public virtual DbSet<SystemSoftwares> SystemSoftwares { get; set; }
        public virtual DbSet<Users> Users { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Authors>()
                .Property(e => e.Author)
                .IsUnicode(false);

            modelBuilder.Entity<BaseSoftwares>()
                .Property(e => e.BaseSoftware)
                .IsUnicode(false);

            modelBuilder.Entity<ConfigData>()
                .Property(e => e.ConfigString)
                .IsUnicode(false);

            modelBuilder.Entity<Customers>()
                .Property(e => e.Customer)
                .IsUnicode(false);

            modelBuilder.Entity<Documents>()
                .Property(e => e.Document)
                .IsUnicode(false);

            modelBuilder.Entity<Documents>()
                .Property(e => e.DocumentType)
                .IsUnicode(false);

            modelBuilder.Entity<DocumentTypes>()
                .Property(e => e.DocumentType)
                .IsUnicode(false);

            modelBuilder.Entity<Properties>()
                .Property(e => e.PropertyType)
                .IsUnicode(false);

            modelBuilder.Entity<Properties>()
                .Property(e => e.Description)
                .IsUnicode(false);

            modelBuilder.Entity<PropertyTypes>()
                .Property(e => e.PropertyType)
                .IsUnicode(false);

            modelBuilder.Entity<PropertyTypes>()
                .Property(e => e.PropertyDescription)
                .IsUnicode(false);

            modelBuilder.Entity<Softwares>()
                .Property(e => e.Software)
                .IsUnicode(false);

            modelBuilder.Entity<Softwares>()
                .Property(e => e.System)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.Software)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.Base)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.System)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.Customer)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.Author)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.Description)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.PrivateInfosBugs)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .Property(e => e.AdditionalInformation)
                .IsUnicode(false);

            modelBuilder.Entity<SoftwareVersions>()
                .HasMany(e => e.InitialStateFirmware)
                .WithRequired(e => e.SoftwareVersions)
                .HasForeignKey(e => e.SoftwareVersions_Id_Firmware)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<SoftwareVersions>()
                .HasMany(e => e.InitialStateFirmware1)
                .WithRequired(e => e.SoftwareVersions1)
                .HasForeignKey(e => e.SoftwareVersions_Id_MotionController)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<SoftwareVersions>()
                .HasMany(e => e.InitialStateFirmware2)
                .WithRequired(e => e.SoftwareVersions2)
                .HasForeignKey(e => e.SoftwareVersions_Id_Interface)
                .WillCascadeOnDelete(false);

            modelBuilder.Entity<Systems>()
                .Property(e => e.System)
                .IsUnicode(false);

            modelBuilder.Entity<SystemSoftwares>()
                .Property(e => e.Software)
                .IsUnicode(false);

            modelBuilder.Entity<Users>()
                .Property(e => e.UserName)
                .IsUnicode(false);
        }
    }
}
