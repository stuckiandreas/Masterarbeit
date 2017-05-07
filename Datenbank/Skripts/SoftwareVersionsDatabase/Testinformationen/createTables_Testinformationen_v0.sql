/*TABLES with FOREIGN KEY of the TestVersion TABLE*/
CREATE TABLE Module1Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module1TypesId int references Module1Types(Id));
	
CREATE TABLE Module2Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module2TypesId int references Module2Types(Id));
	
CREATE TABLE Module3Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module3TypesId int references Module3Types(Id));
	
CREATE TABLE Module4Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module4TypesId int references Module4Types(Id));
	
CREATE TABLE Module1HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module1HardwareVersionsId int references Module1HardwareVersions(Id));
	
CREATE TABLE Module2HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module2HardwareVersionsId int references Module2HardwareVersions(Id));

CREATE TABLE Module3HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module3HardwareVersionsId int references Module3HardwareVersions(Id));

CREATE TABLE Module4HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module4HardwareVersionsId int references Module4HardwareVersions(Id));
	
CREATE TABLE Module1AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module1AssemblyVersionsId int references Module1AssemblyVersions(Id));

CREATE TABLE Module2AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module2AssemblyVersionsId int references Module2AssemblyVersions(Id));
	
CREATE TABLE Module3AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module3AssemblyVersionsId int references Module3AssemblyVersions(Id));
	
CREATE TABLE Module4AssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	Module4AssemblyVersionsId int references Module4AssemblyVersions(Id));
	
CREATE TABLE ControllerHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	ControllerHardwareVersionsId int references ControllerHardwareVersions(Id));
	
CREATE TABLE InterfaceHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	InterfaceHardwareVersionsId int references InterfaceHardwareVersions(Id));
	
CREATE TABLE ControllerAssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	ControllerAssemblyVersionsId int references ControllerAssemblyVersions(Id));
	
CREATE TABLE InterfaceAssemblyVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	InterfaceAssemblyVersionsId int references InterfaceAssemblyVersions(Id));
	
CREATE TABLE OptionType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	OptionTypesId int references OptionTypes(Id));
	
CREATE TABLE ControllerType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	ControllerTypesId int references ControllerTypes(Id));
	
CREATE TABLE InterfaceType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	InterfaceTypesId int references InterfaceTypes(Id));
	
CREATE TABLE ValveSerie(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	ValveSeriesId int references ValveSeries(Id));
	
CREATE TABLE TestHardwareType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersionId int references TestVersion(Id),
	TestHardwareTypesId int references TestHardwareTypes(Id));

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