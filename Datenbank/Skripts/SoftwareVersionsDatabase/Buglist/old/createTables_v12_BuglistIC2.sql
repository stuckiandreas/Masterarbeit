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
	PropertyDescription VARCHAR(100),
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
	
/*TABLE for Saving Buglist Infos IC2*/
CREATE TABLE Buglist(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureType VARCHAR(100),							/*repeatable misbehaviour, unaccountable behaviour*/
	ControllerGenertion VARCHAR(100),					/*IC2H1, IC2H2, IC2H3*/
	HardwareIdentificationLevel1 VARCHAR(100),			/*General,Interface, Option, Valve*/
	HardwareIdentificationLevel2 VARCHAR(100),			/*Interface: RS232/EtherCAT, DeviceNET, Logic; Option: Cluster, PFO; Valve: SFV, FaceSeal, R62;*/
	Bug	VARCHAR(200),						
	Comment VARCHAR(200),
	Priority INT,
	DateFound DATETIME,
	DateFixed DATETIME);
	