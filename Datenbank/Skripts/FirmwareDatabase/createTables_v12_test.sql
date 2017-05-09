/*TABLES to save specific information that can be inserted to the main table*/
CREATE TABLE BaseSoftwares(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100));
CREATE TABLE Customers(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100));
CREATE TABLE Authors(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(40),
	Surname VARCHAR(40));
CREATE TABLE PropertyTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL,
	Description VARCHAR(100));
CREATE TABLE Systems(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);
CREATE TABLE DocumentTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL);	

/*Save a single software entrie*/
CREATE TABLE SoftwareVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL,
	BaseSoftwares_Id int references BaseSoftwares(Id),
	Customers_Id int references Customers(Id),
	Authors_Id int references Authors(Id),
	Date DATETIME NOT NULL,
	PssNumber INT,
	Status INT,
	PssStatus INT,
	Description VARCHAR(500),
	PrivateInfosBugs VARCHAR(500),
	AdditionalInformation VARCHAR(500));
	
/*TABLES with FOREIGN KEY of the SoftwareVersions TABLE*/
CREATE TABLE SoftwareVersions_System(
	Id int IDENTITY(0,1) PRIMARY KEY,
	SoftwareVersions_Id int references SoftwareVersions(Id),
	Systems_Id int references Systems(Id));
CREATE TABLE Properties(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PropertyTypes_Id int references PropertyTypes(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));
CREATE TABLE Compatibilities(
	Id int IDENTITY(0,1) PRIMARY KEY,
	SoftwareVersions_System_Id int references SoftwareVersions_System(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));
CREATE TABLE Documents(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Path VARCHAR(200) NOT NULL,
	DocumentTypes_Id int references DocumentTypes(Id),
	SoftwareVersions_Id int references SoftwareVersions(Id));
	
/*TABLES to save environment Information*/
CREATE TABLE EnvironmentVersions(
	Id int IDENTITY(0,1) PRIMARY KEY,
	Name VARCHAR(100) NOT NULL,
	Version VARCHAR(10) NOT NULL);
CREATE TABLE UserRightTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	UserRightType VARCHAR(40) NOT NULL);
CREATE TABLE Users(
	Id int IDENTITY(0,1) PRIMARY KEY,
	UserName VARCHAR(100) NOT NULL,
	UserRightTypes_Id int references UserRightTypes(Id));



