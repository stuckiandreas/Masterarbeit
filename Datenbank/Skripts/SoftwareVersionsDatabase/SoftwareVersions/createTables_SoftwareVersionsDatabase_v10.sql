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
CREATE TABLE ConfigurationParameterFile(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(20) NOT NULL,
	ActivityState SMALLINT NOT NULL);
	
CREATE TABLE DriveParameterFile(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(20) NOT NULL,
	ActivityState SMALLINT NOT NULL);
	

	
	
/*TABLE to save test information (for every test a single entrie)*/
CREATE TABLE TestInformation(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL,
	Description VARCHAR(400) NOT NULL,
	CreationDate DATE,
	CreationAuthor VARCHAR(30));
	
CREATE TABLE ModulName(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Enum VARCHAR(40) NOT NULL);

	
/*For each test is possible that he has multiple versions*/
CREATE TABLE TestVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Number SMALLINT NOT NULL,
	Modification VARCHAR(400),
	Durating TIME,
	ModulName_Id int references ModulName(Id),
	ExternalIsolationValveOption SMALLINT NOT NULL,
	TestInformation_Id int references TestInformation(Id));

/*TABLES to save specific information that can be inserted to the TestVersion table*/
CREATE TABLE Module1Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));

CREATE TABLE Module2Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));

CREATE TABLE Module3Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));

CREATE TABLE Module4Type(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module1HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module2HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module3HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module4HardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module1AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module2AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE Module3AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));

CREATE TABLE Module4AssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE ControllerHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE InterfaceHardwareVersion(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE ControllerAssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE InterfaceAssemblyVariant(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(100));
	
CREATE TABLE OptionType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue SMALLINT NOT NULL,
	Enum VARCHAR(40));
	
CREATE TABLE ControllerType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue INT NOT NULL,
	Enum VARCHAR(40));
	
CREATE TABLE InterfaceType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue INT NOT NULL,
	Enum VARCHAR(40));
	
CREATE TABLE ValveSerie(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	EnumValue INT NOT NULL,
	Enum VARCHAR(40));
	
CREATE TABLE TestHardwareType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	EnumIndex SMALLINT NOT NULL,
	Enum VARCHAR(60));

/*TABLES with FOREIGN KEY of the TestVersion TABLE*/
CREATE TABLE TestVersion2Module1Type(
	TestVersion_Id int references TestVersion(Id),
	Module1Type_Id int references Module1Type(Id)
	PRIMARY KEY (TestVersion_Id, Module1Type_Id));
	
CREATE TABLE TestVersion2Module2Type(
	TestVersion_Id int references TestVersion(Id),
	Module2Type_Id int references Module2Type(Id)
	PRIMARY KEY (TestVersion_Id, Module2Type_Id));
	
CREATE TABLE TestVersion2Module3Type(
	TestVersion_Id int references TestVersion(Id),
	Module3Type_Id int references Module3Type(Id)
	PRIMARY KEY (TestVersion_Id, Module3Type_Id));
	
CREATE TABLE TestVersion2Module4Type(
	TestVersion_Id int references TestVersion(Id),
	Module4Type_Id int references Module4Type(Id)
	PRIMARY KEY (TestVersion_Id, Module4Type_Id));
	
CREATE TABLE TestVersion2Module1HardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	Module1HardwareVersion_Id int references Module1HardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, Module1HardwareVersion_Id));
	
CREATE TABLE TestVersion2Module2HardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	Module2HardwareVersion_Id int references Module2HardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, Module2HardwareVersion_Id));

CREATE TABLE TestVersion2Module3HardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	Module3HardwareVersion_Id int references Module3HardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, Module3HardwareVersion_Id));

CREATE TABLE TestVersion2Module4HardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	Module4HardwareVersion_Id int references Module4HardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, Module4HardwareVersion_Id));
	
CREATE TABLE TestVersion2Module1AssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	Module1AssemblyVariant_Id int references Module1AssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, Module1AssemblyVariant_Id));

CREATE TABLE TestVersion2Module2AssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	Module2AssemblyVariant_Id int references Module2AssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, Module2AssemblyVariant_Id));
	
CREATE TABLE TestVersion2Module3AssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	Module3AssemblyVariant_Id int references Module3AssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, Module3AssemblyVariant_Id));
	
CREATE TABLE TestVersion2Module4AssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	Module4AssemblyVariant_Id int references Module4AssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, Module4AssemblyVariant_Id));
	
CREATE TABLE TestVersion2ControllerHardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	ControllerHardwareVersion_Id int references ControllerHardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, ControllerHardwareVersion_Id));
	
CREATE TABLE TestVersion2InterfaceHardwareVersion(
	TestVersion_Id int references TestVersion(Id),
	InterfaceHardwareVersion_Id int references InterfaceHardwareVersion(Id)
	PRIMARY KEY (TestVersion_Id, InterfaceHardwareVersion_Id));
	
CREATE TABLE TestVersion2ControllerAssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	ControllerAssemblyVariant_Id int references ControllerAssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, ControllerAssemblyVariant_Id));
	
CREATE TABLE TestVersion2InterfaceAssemblyVariant(
	TestVersion_Id int references TestVersion(Id),
	InterfaceAssemblyVariant_Id int references InterfaceAssemblyVariant(Id)
	PRIMARY KEY (TestVersion_Id, InterfaceAssemblyVariant_Id));
	
CREATE TABLE TestVersion2OptionType(
	TestVersion_Id int references TestVersion(Id),
	OptionType_Id int references OptionType(Id)
	PRIMARY KEY (TestVersion_Id, OptionType_Id));
	
CREATE TABLE TestVersion2ControllerType(
	TestVersion_Id int references TestVersion(Id),
	ControllerType_Id int references ControllerType(Id)
	PRIMARY KEY (TestVersion_Id, ControllerType_Id));
	
CREATE TABLE TestVersion2InterfaceType(
	TestVersion_Id int references TestVersion(Id),
	InterfaceType_Id int references InterfaceType(Id)
	PRIMARY KEY (TestVersion_Id, InterfaceType_Id));
	
CREATE TABLE TestVersion2ValveSerie(
	TestVersion_Id int references TestVersion(Id),
	ValveSerie_Id int references ValveSerie(Id)
	PRIMARY KEY (TestVersion_Id, ValveSerie_Id));
	
CREATE TABLE TestVersion2TestHardwareType(
	TestVersion_Id int references TestVersion(Id),
	TestHardwareType_Id int references TestHardwareType(Id)
	PRIMARY KEY (TestVersion_Id, TestHardwareType_Id));



/*TABLES to save test results*/
/*First run TestInformation script*/
/*TABLE for all test collections*/
CREATE TABLE TestCollection(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL,
	Description VARCHAR(400),
	CreationDate DATE NOT NULL,
	ActivityState SMALLINT NOT NULL);	
	
/*TABLE for saving test results*/
CREATE TABLE InitialStateValve(
	Id int IDENTITY(0,1) PRIMARY KEY,
	SoftwareVersions_Id_Firmware int references SoftwareVersions(Id),							
	ValveFirmwareReleaseTime DATETIME NOT NULL,					
	SoftwareVersions_Id_MotionController int references SoftwareVersions(Id),			
	SoftwareVersions_Id_Interface int references SoftwareVersions(Id),			
	DriveParameterFile_ID int references DriveParameterFile(Id),						
	ConfigurationParameterFile_ID int references ConfigurationParameterFile(Id),
	TestCollection_Id int references TestCollection(Id));
	
/*TABLE which save the valve hardware*/
CREATE TABLE ValveHardware(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ValveSerie_Id int references ValveSerie(Id),
	InterfaceType_Id int references InterfaceType(Id),
	ControllerType_Id int references ControllerType(Id),
	OptionType_Id int references OptionType(Id),
	ExternalIsolationValveOption BIT NOT NULL,
	ControllerHardwareVersion_Id int references ControllerHardwareVersion(Id),
	InterfaceHardwareVersion_Id int references InterfaceHardwareVersion(Id),
	ControllerAssemblyVariant_Id int references ControllerAssemblyVariant(Id),
	InterfaceAssemblyVariant_Id int references InterfaceAssemblyVariant(Id),
	Module1Type_Id int references Module1Type(Id),
	Module1HardwareVersion_Id int references Module1HardwareVersion(Id),
	Module1AssemblyVariant_Id int references Module1AssemblyVariant(Id),
	Module2Type_Id int references Module2Type(Id),
	Module2HardwareVersion_Id int references Module2HardwareVersion(Id),
	Module2AssemblyVariant_Id int references Module2AssemblyVariant(Id),
	Module3Type_Id int references Module3Type(Id),
	Module3HardwareVersion_Id int references Module3HardwareVersion(Id),
	Module3AssemblyVariant_Id int references Module3AssemblyVariant(Id),
	Module4Type_Id int references Module4Type(Id),
	Module4HardwareVersion_Id int references Module4HardwareVersion(Id),
	Module4AssemblyVariant_Id int references Module4AssemblyVariant(Id));
	
/*TABLE for binding tests with a test collection*/
CREATE TABLE TestCollection2TestInformation(
	TestInformation_Id int references TestInformation(Id),
	TestCollection_Id int references TestCollection(Id)
	PRIMARY KEY (TestInformation_Id, TestCollection_Id));
	
/* TABLE which defined the abort reason (by error, user cancelled)*/
CREATE TABLE AbortType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(30) NOT NULL);
	
/*TABLE which save the information of the completed test collection*/
CREATE TABLE TestCollectionResult(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ExecutionDate DATE,
	UserName VARCHAR(30),
	AbortType_Id int references AbortType(Id),
	CountErrorTest SMALLINT,
	ValveHardware_Id int references ValveHardware(Id),
	InitialStateValve_Id int references InitialStateValve(Id)); 

/*TABLE which save the information of the completed test*/
CREATE TABLE TestResult(
	Id int IDENTITY(0,1) PRIMARY KEY,
	StartTime DATETIME,
	EndTime DATETIME,
	TestResult SMALLINT NOT NULL,
	TestCollectionResult_Id int references TestCollectionResult(Id),
	TestVersion_Id int references TestVersion(Id));
	
/*TABLE which save the information of the test error message*/
CREATE TABLE TestErrorMessage(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(400),
	TestResult_Id int references TestResult(Id));


	
/*TABLES to save firmware bugs (not included master work)*/	
/*TABLES to save specific information that can be inserted to the Buglist table*/
/*repeatable misbehaviour, unaccountable behaviour*/
CREATE TABLE FailureType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);	
	
/*open, solved, no solution*/
CREATE TABLE StatusType(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);
	
/*IC2H1, IC2H2, IC2H3* ControllerTypes is defined in TestInformation*/
	
/*Low, Medium, High*/
CREATE TABLE Priority(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40) NOT NULL);
	
/*General,Interface, Option, Valve*/
CREATE TABLE HardwareIdentificationLevel1(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);

/*Interface: RS232/EtherCAT, DeviceNET, Logic; Option: Cluster, PFO; Valve: SFV, FaceSeal, R62;*/
CREATE TABLE HardwareIdentificationLevel2(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);
	
	
/*TABLE for Saving Buglist Infos*/
CREATE TABLE Buglist(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureType_Id int references FailureType(Id),	
	StatusType_Id int references StatusType(Id),
	ControllerType_Id int references ControllerType(Id),					
	HardwareIdentificationLevel1_Id int references HardwareIdentificationLevel1(Id),			
	HardwareIdentificationLevel2_Id int references HardwareIdentificationLevel2(Id),			
	Bug	VARCHAR(200) NOT NULL,						
	Comment VARCHAR(200),
	Priority_Id int references Priority(Id),
	DateFound DATETIME NOT NULL,
	DateFixed DATETIME);
	
