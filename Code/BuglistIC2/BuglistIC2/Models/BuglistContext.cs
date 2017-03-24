namespace BuglistIC2
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class BuglistContext : DbContext
    {
        public BuglistContext()
            : base("name=BuglistContext")
        {
        }

        public virtual DbSet<Buglist> Buglist { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Buglist>()
                .Property(e => e.FailureType)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.ControllerGenertion)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.HardwareIdentificationLevel1)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.HardwareIdentificationLevel2)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.Bug)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.Comment)
                .IsUnicode(false);
        }
    }
}
