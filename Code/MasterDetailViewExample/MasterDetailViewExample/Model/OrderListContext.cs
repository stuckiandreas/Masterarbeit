namespace MasterDetailViewExample.Model
{
    using System;
    using System.Data.Entity;
    using System.ComponentModel.DataAnnotations.Schema;
    using System.Linq;

    public partial class OrderListContext : DbContext
    {
        public OrderListContext(string connection)
            : base(connection)
        {
        }

        public virtual DbSet<Customer> Customer { get; set; }
        public virtual DbSet<OrderList> OrderList { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Customer>()
                .Property(e => e.Surname)
                .IsUnicode(false);

            modelBuilder.Entity<Customer>()
                .Property(e => e.Lastname)
                .IsUnicode(false);

            modelBuilder.Entity<Customer>()
                .HasMany(e => e.OrderList)
                .WithOptional(e => e.Customer)
                .HasForeignKey(e => e.Customer_Id);

            modelBuilder.Entity<OrderList>()
                .Property(e => e.Name)
                .IsUnicode(false);
        }
    }
}
