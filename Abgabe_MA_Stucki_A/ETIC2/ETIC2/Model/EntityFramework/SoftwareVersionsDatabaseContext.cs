namespace ETIC2.Model.EntityFramework
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

        public virtual DbSet<AbortType> AbortType { get; set; }
        public virtual DbSet<Buglist> Buglist { get; set; }
        public virtual DbSet<ConfigData> ConfigData { get; set; }
        public virtual DbSet<ConfigurationParameterFile> ConfigurationParameterFile { get; set; }
        public virtual DbSet<ControllerAssemblyVariant> ControllerAssemblyVariant { get; set; }
        public virtual DbSet<ControllerHardwareVersion> ControllerHardwareVersion { get; set; }
        public virtual DbSet<ControllerType> ControllerType { get; set; }
        public virtual DbSet<DriveParameterFile> DriveParameterFile { get; set; }
        public virtual DbSet<FailureType> FailureType { get; set; }
        public virtual DbSet<HardwareIdentificationLevel1> HardwareIdentificationLevel1 { get; set; }
        public virtual DbSet<HardwareIdentificationLevel2> HardwareIdentificationLevel2 { get; set; }
        public virtual DbSet<InitialStateFirmware> InitialStateFirmware { get; set; }
        public virtual DbSet<InterfaceAssemblyVariant> InterfaceAssemblyVariant { get; set; }
        public virtual DbSet<InterfaceHardwareVersion> InterfaceHardwareVersion { get; set; }
        public virtual DbSet<InterfaceType> InterfaceType { get; set; }
        public virtual DbSet<Module1AssemblyVariant> Module1AssemblyVariant { get; set; }
        public virtual DbSet<Module1HardwareVersion> Module1HardwareVersion { get; set; }
        public virtual DbSet<Module1Type> Module1Type { get; set; }
        public virtual DbSet<Module2AssemblyVariant> Module2AssemblyVariant { get; set; }
        public virtual DbSet<Module2HardwareVersion> Module2HardwareVersion { get; set; }
        public virtual DbSet<Module2Type> Module2Type { get; set; }
        public virtual DbSet<Module3AssemblyVariant> Module3AssemblyVariant { get; set; }
        public virtual DbSet<Module3HardwareVersion> Module3HardwareVersion { get; set; }
        public virtual DbSet<Module3Type> Module3Type { get; set; }
        public virtual DbSet<Module4AssemblyVariant> Module4AssemblyVariant { get; set; }
        public virtual DbSet<Module4HardwareVersion> Module4HardwareVersion { get; set; }
        public virtual DbSet<Module4Type> Module4Type { get; set; }
        public virtual DbSet<OptionType> OptionType { get; set; }
        public virtual DbSet<Priority> Priority { get; set; }
        public virtual DbSet<ResultType> ResultType { get; set; }
        public virtual DbSet<SoftwareVersions> SoftwareVersions { get; set; }
        public virtual DbSet<StatusType> StatusType { get; set; }
        public virtual DbSet<TestCollection> TestCollection { get; set; }
        public virtual DbSet<TestCollectionResult> TestCollectionResult { get; set; }
        public virtual DbSet<TestErrorMessage> TestErrorMessage { get; set; }
        public virtual DbSet<TestInformation> TestInformation { get; set; }
        public virtual DbSet<TestResult> TestResult { get; set; }
        public virtual DbSet<TestVersion> TestVersion { get; set; }
        public virtual DbSet<ValveHardware> ValveHardware { get; set; }
        public virtual DbSet<ValveSerie> ValveSerie { get; set; }

        protected override void OnModelCreating(DbModelBuilder modelBuilder)
        {
            modelBuilder.Entity<AbortType>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<AbortType>()
                .HasMany(e => e.TestCollectionResult)
                .WithOptional(e => e.AbortType)
                .HasForeignKey(e => e.AbortType_Id);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.Bug)
                .IsUnicode(false);

            modelBuilder.Entity<Buglist>()
                .Property(e => e.Comment)
                .IsUnicode(false);

            modelBuilder.Entity<ConfigData>()
                .Property(e => e.ConfigString)
                .IsUnicode(false);

            modelBuilder.Entity<ConfigurationParameterFile>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<ConfigurationParameterFile>()
                .HasMany(e => e.InitialStateFirmware)
                .WithOptional(e => e.ConfigurationParameterFile)
                .HasForeignKey(e => e.ConfigurationParameterFile_ID);

            modelBuilder.Entity<ControllerAssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<ControllerAssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.ControllerAssemblyVariant)
                .HasForeignKey(e => e.ControllerAssemblyVariant_Id);

            modelBuilder.Entity<ControllerHardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<ControllerHardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.ControllerHardwareVersion)
                .HasForeignKey(e => e.ControllerHardwareVersion_Id);

            modelBuilder.Entity<ControllerType>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<ControllerType>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.ControllerType)
                .HasForeignKey(e => e.ControllerType_Id);

            modelBuilder.Entity<ControllerType>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.ControllerType)
                .HasForeignKey(e => e.ControllerType_Id);

            modelBuilder.Entity<DriveParameterFile>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<DriveParameterFile>()
                .HasMany(e => e.InitialStateFirmware)
                .WithOptional(e => e.DriveParameterFile)
                .HasForeignKey(e => e.DriveParameterFile_ID);

            modelBuilder.Entity<FailureType>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<FailureType>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.FailureType)
                .HasForeignKey(e => e.FailureType_Id);

            modelBuilder.Entity<HardwareIdentificationLevel1>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<HardwareIdentificationLevel1>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.HardwareIdentificationLevel1)
                .HasForeignKey(e => e.HardwareIdentificationLevel1_Id);

            modelBuilder.Entity<HardwareIdentificationLevel2>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<HardwareIdentificationLevel2>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.HardwareIdentificationLevel2)
                .HasForeignKey(e => e.HardwareIdentificationLevel2_Id);

            modelBuilder.Entity<InitialStateFirmware>()
                .HasMany(e => e.TestCollectionResult)
                .WithOptional(e => e.InitialStateFirmware)
                .HasForeignKey(e => e.InitialStateFirmware_Id);

            modelBuilder.Entity<InterfaceAssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<InterfaceAssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.InterfaceAssemblyVariant)
                .HasForeignKey(e => e.InterfaceAssemblyVariant_Id);

            modelBuilder.Entity<InterfaceHardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<InterfaceHardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.InterfaceHardwareVersion)
                .HasForeignKey(e => e.InterfaceHardwareVersion_Id);

            modelBuilder.Entity<InterfaceType>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<InterfaceType>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.InterfaceType)
                .HasForeignKey(e => e.InterfaceType_Id);

            modelBuilder.Entity<Module1AssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module1AssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module1AssemblyVariant)
                .HasForeignKey(e => e.Module1AssemblyVariant_Id);

            modelBuilder.Entity<Module1HardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module1HardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module1HardwareVersion)
                .HasForeignKey(e => e.Module1HardwareVersion_Id);

            modelBuilder.Entity<Module1Type>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module1Type>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module1Type)
                .HasForeignKey(e => e.Module1Type_Id);

            modelBuilder.Entity<Module2AssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module2AssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module2AssemblyVariant)
                .HasForeignKey(e => e.Module2AssemblyVariant_Id);

            modelBuilder.Entity<Module2HardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module2HardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module2HardwareVersion)
                .HasForeignKey(e => e.Module2HardwareVersion_Id);

            modelBuilder.Entity<Module2Type>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module2Type>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module2Type)
                .HasForeignKey(e => e.Module2Type_Id);

            modelBuilder.Entity<Module3AssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module3AssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module3AssemblyVariant)
                .HasForeignKey(e => e.Module3AssemblyVariant_Id);

            modelBuilder.Entity<Module3HardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module3HardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module3HardwareVersion)
                .HasForeignKey(e => e.Module3HardwareVersion_Id);

            modelBuilder.Entity<Module3Type>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module3Type>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module3Type)
                .HasForeignKey(e => e.Module3Type_Id);

            modelBuilder.Entity<Module4AssemblyVariant>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module4AssemblyVariant>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module4AssemblyVariant)
                .HasForeignKey(e => e.Module4AssemblyVariant_Id);

            modelBuilder.Entity<Module4HardwareVersion>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module4HardwareVersion>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module4HardwareVersion)
                .HasForeignKey(e => e.Module4HardwareVersion_Id);

            modelBuilder.Entity<Module4Type>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<Module4Type>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.Module4Type)
                .HasForeignKey(e => e.Module4Type_Id);

            modelBuilder.Entity<OptionType>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<OptionType>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.OptionType)
                .HasForeignKey(e => e.OptionType_Id);

            modelBuilder.Entity<Priority>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<Priority>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.Priority)
                .HasForeignKey(e => e.Priority_Id);

            modelBuilder.Entity<ResultType>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<ResultType>()
                .HasMany(e => e.TestResult)
                .WithOptional(e => e.ResultType)
                .HasForeignKey(e => e.ResultType_Id);

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
                .WithOptional(e => e.SoftwareVersions)
                .HasForeignKey(e => e.SoftwareVersions_Id_Firmware);

            modelBuilder.Entity<SoftwareVersions>()
                .HasMany(e => e.InitialStateFirmware1)
                .WithOptional(e => e.SoftwareVersions1)
                .HasForeignKey(e => e.SoftwareVersions_Id_MotionController);

            modelBuilder.Entity<SoftwareVersions>()
                .HasMany(e => e.InitialStateFirmware2)
                .WithOptional(e => e.SoftwareVersions2)
                .HasForeignKey(e => e.SoftwareVersions_Id_Interface);

            modelBuilder.Entity<StatusType>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<StatusType>()
                .HasMany(e => e.Buglist)
                .WithOptional(e => e.StatusType)
                .HasForeignKey(e => e.StatusType_Id);

            modelBuilder.Entity<TestCollection>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<TestCollection>()
                .Property(e => e.Description)
                .IsUnicode(false);

            modelBuilder.Entity<TestCollection>()
                .HasMany(e => e.InitialStateFirmware)
                .WithOptional(e => e.TestCollection)
                .HasForeignKey(e => e.TestCollection_Id);

            modelBuilder.Entity<TestCollectionResult>()
                .Property(e => e.UserName)
                .IsUnicode(false);

            modelBuilder.Entity<TestCollectionResult>()
                .HasMany(e => e.TestResult)
                .WithOptional(e => e.TestCollectionResult)
                .HasForeignKey(e => e.TestCollectionResult_Id);

            modelBuilder.Entity<TestErrorMessage>()
                .Property(e => e.Description)
                .IsUnicode(false);

            modelBuilder.Entity<TestInformation>()
                .Property(e => e.Name)
                .IsUnicode(false);

            modelBuilder.Entity<TestInformation>()
                .Property(e => e.Description)
                .IsUnicode(false);

            modelBuilder.Entity<TestInformation>()
                .Property(e => e.CreationAuthor)
                .IsUnicode(false);

            modelBuilder.Entity<TestInformation>()
                .HasMany(e => e.TestVersion)
                .WithOptional(e => e.TestInformation)
                .HasForeignKey(e => e.TestInformation_Id);

            modelBuilder.Entity<TestResult>()
                .HasMany(e => e.TestErrorMessage)
                .WithOptional(e => e.TestResult)
                .HasForeignKey(e => e.TestResult_Id);

            modelBuilder.Entity<TestVersion>()
                .Property(e => e.Modification)
                .IsUnicode(false);

            modelBuilder.Entity<TestVersion>()
                .HasMany(e => e.TestResult)
                .WithOptional(e => e.TestVersion)
                .HasForeignKey(e => e.TestVersion_Id);

            modelBuilder.Entity<ValveHardware>()
                .HasMany(e => e.TestCollectionResult)
                .WithOptional(e => e.ValveHardware)
                .HasForeignKey(e => e.ValveHardware_Id);

            modelBuilder.Entity<ValveSerie>()
                .Property(e => e.Enum)
                .IsUnicode(false);

            modelBuilder.Entity<ValveSerie>()
                .HasMany(e => e.ValveHardware)
                .WithOptional(e => e.ValveSerie)
                .HasForeignKey(e => e.ValveSerie_Id);
        }
    }
}
