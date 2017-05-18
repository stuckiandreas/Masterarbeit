/*TABLE to save test information (for every test a single entrie)*/
CREATE TABLE TestInformation(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL,
	CreationDate DATE,
	CreationAuthor VARCHAR(30));
	
CREATE TABLE ModulNames(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ModulName VARCHAR(40) NOT NULL);

	
/*For each test is possible that he has multiple versions*/
CREATE TABLE TestVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Index UNSIGNED SMALLINT NOT NULL,
	Description VARCHAR(400),
	Durating TIME,
	ModulNames_Id int references ModulNames(Id),
	ExternalIsolationValveOption BIT NOT NULL,
	TestInformation_Id int references TestInformation(Id));

/*TABLES to save specific information that can be inserted to the TestVersion table*/
CREATE TABLE Module1Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module1Type UNSIGNED SMALLINT NOT NULL,
	Module1TypeEnum VARCHAR(100));

CREATE TABLE Module2Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module2Type UNSIGNED SMALLINT NOT NULL,
	Module2TypeEnum VARCHAR(100));

CREATE TABLE Module3Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module3Type UNSIGNED SMALLINT NOT NULL,
	Module3TypeEnum VARCHAR(100));

CREATE TABLE Module4Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module4Type UNSIGNED SMALLINT NOT NULL,
	Module4TypeEnum VARCHAR(100));
	
CREATE TABLE Module1HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module1HardwareVersion UNSIGNED SMALLINT NOT NULL,
	Module1HardwareVersionEnum VARCHAR(100));
	
CREATE TABLE Module2HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module2HardwareVersion UNSIGNED SMALLINT NOT NULL,
	Module2HardwareVersionEnum VARCHAR(100));
	
CREATE TABLE Module3HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module3HardwareVersion UNSIGNED SMALLINT NOT NULL,
	Module3HardwareVersionEnum VARCHAR(100));
	
CREATE TABLE Module4HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module4HardwareVersion UNSIGNED SMALLINT NOT NULL,
	Module4HardwareVersionEnum VARCHAR(100));
	
CREATE TABLE Module1AssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module1AssemblyVersion UNSIGNED SMALLINT NOT NULL,
	Module1AssemblyVersionEnum VARCHAR(100));
	
CREATE TABLE Module2AssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module2AssemblyVersion UNSIGNED SMALLINT NOT NULL,
	Module2AssemblyVersionEnum VARCHAR(100));
	
CREATE TABLE Module3AssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module3AssemblyVersion UNSIGNED SMALLINT NOT NULL,
	Module3AssemblyVersionEnum VARCHAR(100));

CREATE TABLE Module4AssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Module4AssemblyVersion UNSIGNED SMALLINT NOT NULL,
	Module4AssemblyVersionEnum VARCHAR(100));
	
CREATE TABLE ControllerHardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ControllerHardwareVersion UNSIGNED SMALLINT NOT NULL,
	ControllerHardwareVersionEnum VARCHAR(100));
	
CREATE TABLE InterfaceHardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	InterfaceHardwareVersion UNSIGNED SMALLINT NOT NULL,
	InterfaceHardwareVersionEnum VARCHAR(100));
	
CREATE TABLE ControllerAssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ControllerAssemblyVersion UNSIGNED SMALLINT NOT NULL,
	ControllerAssemblyVersionEnum VARCHAR(100));
	
CREATE TABLE InterfaceAssemblyVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	InterfaceAssemblyVersion UNSIGNED SMALLINT NOT NULL,
	InterfaceAssemblyVersionEnum VARCHAR(100));
	
CREATE TABLE OptionTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	OptionType UNSIGNED SMALLINT NOT NULL,
	OptionTypeEnum VARCHAR(40));
	
CREATE TABLE ControllerTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ControllerType UNSIGNED SMALLINT NOT NULL,
	ControllerTypeEnum VARCHAR(40));
	
CREATE TABLE InterfaceTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	InterfaceType UNSIGNED SMALLINT NOT NULL,
	InterfaceTypeEnum VARCHAR(40));
	
CREATE TABLE ValveSeries(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ValveSerie UNSIGNED SMALLINT NOT NULL,
	ValveSerieEnum VARCHAR(40));
	
CREATE TABLE TestHardwareTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestHardwareType UNSIGNED SMALLINT NOT NULL,
	TestHardwareTypeEnum VARCHAR(60));

/*TABLES with FOREIGN KEY of the TestVersion TABLE*/
CREATE TABLE Module1Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module1Types_Id int references Module1Types(Id));
	
CREATE TABLE Module2Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module2Types_Id int references Module2Types(Id));
	
CREATE TABLE Module3Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module3Types_Id int references Module3Types(Id));
	
CREATE TABLE Module4Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module4Types_Id int references Module4Types(Id));
	
CREATE TABLE Module1HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module1HardwareVersions_Id int references Module1HardwareVersions(Id));
	
CREATE TABLE Module2HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module2HardwareVersions_Id int references Module2HardwareVersions(Id));

CREATE TABLE Module3HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module3HardwareVersions_Id int references Module3HardwareVersions(Id));

CREATE TABLE Module4HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module4HardwareVersions_Id int references Module4HardwareVersions(Id));
	
CREATE TABLE Module1AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module1AssemblyVersions_Id int references Module1AssemblyVersions(Id));

CREATE TABLE Module2AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module2AssemblyVersions_Id int references Module2AssemblyVersions(Id));
	
CREATE TABLE Module3AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module3AssemblyVersions_Id int references Module3AssemblyVersions(Id));
	
CREATE TABLE Module4AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module4AssemblyVersions_Id int references Module4AssemblyVersions(Id));
	
CREATE TABLE ControllerHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ControllerHardwareVersions_Id int references ControllerHardwareVersions(Id));
	
CREATE TABLE InterfaceHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	InterfaceHardwareVersions_Id int references InterfaceHardwareVersions(Id));
	
CREATE TABLE ControllerAssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ControllerAssemblyVersions_Id int references ControllerAssemblyVersions(Id));
	
CREATE TABLE InterfaceAssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	InterfaceAssemblyVersions_Id int references InterfaceAssemblyVersions(Id));
	
CREATE TABLE OptionType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	OptionTypes_Id int references OptionTypes(Id));
	
CREATE TABLE ControllerType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ControllerTypes_Id int references ControllerTypes(Id));
	
CREATE TABLE InterfaceType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	InterfaceTypes_Id int references InterfaceTypes(Id));
	
CREATE TABLE ValveSerie(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ValveSeries_Id int references ValveSeries(Id));
	
CREATE TABLE TestHardwareType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	TestHardwareTypes_Id int references TestHardwareTypes(Id));

