/*TABLE for Saving Buglist Infos*/
CREATE TABLE Buglist(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureTypeId int references FailureTypes(Id),							
	ControllerTypesId int references ControllerTypes(Id),					
	HardwareIdentificationLevel1Id int references HardwareIdentificationLevels1(Id),			
	HardwareIdentificationLevel2Id int references HardwareIdentificationLevels2(Id),			
	Bug	VARCHAR(200) NOT NULL,						
	Comment VARCHAR(200),
	PrioritiesId int references Priorities(Id),
	DateFound DATETIME,
	DateFixed DATETIME);
	
/*TABLES to save specific information that can be inserted to the Buglist table*/
/*repeatable misbehaviour, unaccountable behaviour*/
CREATE TABLE FailureTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureType VARCHAR(100));	
	
/*IC2H1, IC2H2, IC2H3*/
CREATE TABLE ControllerTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	ControllerType VARCHAR(40));
	
/*Low, Medium, High*/
CREATE TABLE Priorities(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PriorityType VARCHAR(40));
	
/*General,Interface, Option, Valve*/
CREATE TABLE HardwareIdentificationLevels1(
	Id int IDENTITY(0,1) PRIMARY KEY,
	HardwareIdentificationLevel1Type VARCHAR(100));

/*Interface: RS232/EtherCAT, DeviceNET, Logic; Option: Cluster, PFO; Valve: SFV, FaceSeal, R62;*/
CREATE TABLE HardwareIdentificationLevels2(
	Id int IDENTITY(0,1) PRIMARY KEY,
	HardwareIdentificationLevel2Type VARCHAR(100));