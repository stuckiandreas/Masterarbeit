/*TABLES to save specific information that can be inserted to the main table*/
CREATE TABLE BaseSoftwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	BaseSoftware VARCHAR(100));
CREATE TABLE Customers(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Customer VARCHAR(100));
CREATE TABLE Authors(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Author VARCHAR(100));
CREATE TABLE PropertyTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PropertyType VARCHAR(100) NOT NULL,
	Description VARCHAR(100));
CREATE TABLE Systems(
	Id int IDENTITY(0,1) PRIMARY KEY,
	System VARCHAR(100) NOT NULL);
CREATE TABLE SystemSoftwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Software VARCHAR(100),
	System_Id int references Systems(Id));
CREATE TABLE DocumentTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	DocumentType VARCHAR(100) NOT NULL);	
CREATE TABLE ConfigData(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ConfigString VARCHAR(100),
	ConfigNumber INT);
CREATE TABLE Users(
	Id int IDENTITY(0,1) PRIMARY KEY,
	UserName VARCHAR(100) NOT NULL,
	UserRight INT);
	
/*TABLES with FOREIGN KEY of the SoftwareVersions TABLE*/
CREATE TABLE Properties(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PropertyTypes_Id int references PropertyTypes(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));
CREATE TABLE Softwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	SystemSoftwares_Id int references SystemSoftwares(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));
CREATE TABLE Documents(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Document VARCHAR(100) NOT NULL,
	DocumentType_Id int references DocumentTypes(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));

/*Save a single software entrie*/
CREATE TABLE SoftwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	SystemSoftwares_Id int references SystemSoftwares(Id),
	BaseSoftwares_Id int references BaseSoftwares(Id),
	Customer_Id int references Customers(Id),
	Author_Id int references Authors(Id),
	Date DATETIME NOT NULL,
	PssNumber INT,
	Status INT,
	PssStatus INT,
	Description VARCHAR(500),
	PrivateInfosBugs VARCHAR(500),
	AdditionalInformation VARCHAR(500));


