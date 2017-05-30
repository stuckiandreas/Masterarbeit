/*Firmware Database with old structure and DriveParameterFiles & ConfigurationFiles table*/
CREATE TABLE SoftwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Software VARCHAR(100) NOT NULL,
	Base VARCHAR(100),
	System VARCHAR(100),
	Customer VARCHAR(100),
	Author VARCHAR(100),
	Date DATETIME,
	PssNumber INT,
	Status INT,
	PssStatus INT,
	Description VARCHAR(500),
	PrivateInfosBugs VARCHAR(500),
	AdditionalInformation VARCHAR(500));

/*TABLES with FOREIGN KEY of the SoftwareVersions TABLE*/
CREATE TABLE Properties(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PropertyType VARCHAR(100),
	Description VARCHAR(100),
	SoftwareVersionsId int references SoftwareVersions(Id));
CREATE TABLE Softwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Software VARCHAR(100),
	System VARCHAR(100),
	SoftwareVersionsId int references SoftwareVersions(Id));
CREATE TABLE Documents(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Document VARCHAR(100),
	DocumentType VARCHAR (100),
	SoftwareVersionsId int references SoftwareVersions(Id));

/*TABLES to save specific information that can be inserted to the main table*/
CREATE TABLE BaseSoftwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	BaseSoftware VARCHAR(100));
CREATE TABLE Systems(
	Id int IDENTITY(0,1) PRIMARY KEY,
	System VARCHAR(100));
CREATE TABLE Customers(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Customer VARCHAR(100));
CREATE TABLE Authors(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Author VARCHAR(100));
CREATE TABLE DocumentTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	DocumentType VARCHAR(100));
CREATE TABLE PropertyTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PropertyType VARCHAR(100),
	PropertyDescription VARCHAR(100));
CREATE TABLE SystemSoftwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Software VARCHAR(100),
	SystemsId int references Systems(Id));
CREATE TABLE ConfigData(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ConfigString VARCHAR(100),
	ConfigNumber INT);
CREATE TABLE Users(
	Id int IDENTITY(0,1) PRIMARY KEY,
	UserName VARCHAR(100),
	UserRight INT);
	
/*TABLES for additional Information (not used in SoftfwareVersionsDatabase)*/
CREATE TABLE ConfigurationFiles(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ConfigurationFile VARCHAR(20) NOT NULL);
	
CREATE TABLE DriveParameterFiles(
	Id int IDENTITY(0,1) PRIMARY KEY,
	DriveParameterFile VARCHAR(20) NOT NULL);
	

	
	
/*TABLES to save test information* (not included in master work)*/ 
/*TABLE to save test information (for every test a single entrie)*/
CREATE TABLE TestInformation(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL,
	Description VARCHAR(400) NOT NULL,
	CreationDate DATE,
	CreationAuthor VARCHAR(30));
	
CREATE TABLE ModulNames(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL);

	
/*For each test is possible that he has multiple versions*/
CREATE TABLE TestVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Modification VARCHAR(400),
	Durating TIME,
	ModulNames_Id int references ModulNames(Id),
	ExternalIsolationValveOption BIT NOT NULL,
	TestInformation_Id int references TestInformation(Id));

/*TABLES to save specific information that can be inserted to the TestVersion table*/
CREATE TABLE Module1Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));

CREATE TABLE Module2Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));

CREATE TABLE Module3Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));

CREATE TABLE Module4Types(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module1HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module2HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module3HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module4HardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module1AssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module2AssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE Module3AssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));

CREATE TABLE Module4AssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE ControllerHardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE InterfaceHardwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE ControllerAssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE InterfaceAssemblyVariants(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(100));
	
CREATE TABLE OptionTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(40));
	
CREATE TABLE ControllerTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(40));
	
CREATE TABLE InterfaceTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(40));
	
CREATE TABLE ValveSeries(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(40));
	
CREATE TABLE TestHardwareTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Name VARCHAR(60));

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
	
CREATE TABLE Module1AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module1AssemblyVariants_Id int references Module1AssemblyVariants(Id));

CREATE TABLE Module2AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module2AssemblyVariants_Id int references Module2AssemblyVariants(Id));
	
CREATE TABLE Module3AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module3AssemblyVariants_Id int references Module3AssemblyVariants(Id));
	
CREATE TABLE Module4AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	Module4AssemblyVariants_Id int references Module4AssemblyVariants(Id));
	
CREATE TABLE ControllerHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ControllerHardwareVersions_Id int references ControllerHardwareVersions(Id));
	
CREATE TABLE InterfaceHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	InterfaceHardwareVersions_Id int references InterfaceHardwareVersions(Id));
	
CREATE TABLE ControllerAssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	ControllerAssemblyVariants_Id int references ControllerAssemblyVariants(Id));
	
CREATE TABLE InterfaceAssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id),
	InterfaceAssemblyVariants_Id int references InterfaceAssemblyVariants(Id));
	
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



/*TABLES to save test results*/
/*First run TestInformation script*/
/*TABLE for all test collections*/
CREATE TABLE TestCollection(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(45) NOT NULL,
	Description VARCHAR(400));	
	
/*TABLE for saving test results*/
CREATE TABLE InitialStateValve(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ValveFirmwareID INT NOT NULL,							
	ValveFirmwareReleaseTime DATETIME NOT NULL,					
	MotionControllerFirmwareVersion VARCHAR(100) NOT NULL,			
	InterfaceFirmwareVersion VARCHAR(100) NOT NULL,			
	DriveParameterID VARCHAR(20) NOT NULL,						
	ConfigurationParameterID VARCHAR(20) NOT NULL,
	TestCollection_Id int references TestCollection(Id));
	
/*TABLE for binding tests with a test collection*/
CREATE TABLE TestCollection_TestInformation(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(45) NOT NULL,
	TestInfomation_Id int references TestInformation(Id),
	TestCollection_Id int references TestCollection(Id));
	
/* TABLE which defined the abort reason (by error, user cancelled)*/
CREATE TABLE AbortTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(30) NOT NULL);
	
/*TABLE witch save the information of the completed test collection*/
CREATE TABLE TestCollectionResult(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ExecutionDate DATE,
	UserName VARCHAR(30),
	AbortTypes_Id int references AbortTypes(Id),
	CountErrorTests SMALLINT,
	InitialStateValve_Id int references InitialStateValve(Id)); 

/*TABLE witch save the information of the completed test*/
CREATE TABLE TestResult(
	Id int IDENTITY(0,1) PRIMARY KEY,
	StartTime DATETIME,
	EndTime DATETIME,
	TestResult SMALLINT NOT NULL,
	TestCollectionResult_Id int references TestCollectionResult(Id),
	TestVersion_Id int references TestVersion(Id));
	
/*TABLE witch save the information of the test error message*/
CREATE TABLE TestErrorMessage(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(400),
	TestResult_Id int references TestResult(Id));
	
/*TABLE witch will override by starting the TTIC2 application (binding test and the used version)*/
CREATE TABLE CurrentTestsVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	TestVersion_Id int references TestVersion(Id));


	
/*TABLES to save firmware bugs (not included master work)*/	
/*TABLES to save specific information that can be inserted to the Buglist table*/
/*repeatable misbehaviour, unaccountable behaviour*/
CREATE TABLE FailureTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);	
	
/*IC2H1, IC2H2, IC2H3* ControllerTypes is defined in TestInformation*/
	
/*Low, Medium, High*/
CREATE TABLE Priorities(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL);
	
/*General,Interface, Option, Valve*/
CREATE TABLE HardwareIdentificationLevels1(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);

/*Interface: RS232/EtherCAT, DeviceNET, Logic; Option: Cluster, PFO; Valve: SFV, FaceSeal, R62;*/
CREATE TABLE HardwareIdentificationLevels2(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);
	
	
/*TABLE for Saving Buglist Infos*/
CREATE TABLE Buglist(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureType_Id int references FailureTypes(Id),							
	ControllerTypes_Id int references ControllerTypes(Id),					
	HardwareIdentificationLevel1_Id int references HardwareIdentificationLevels1(Id),			
	HardwareIdentificationLevel2_Id int references HardwareIdentificationLevels2(Id),			
	Bug	VARCHAR(200) NOT NULL,						
	Comment VARCHAR(200),
	Priorities_Id int references Priorities(Id),
	DateFound DATETIME NOT NULL,
	DateFixed DATETIME);
	
