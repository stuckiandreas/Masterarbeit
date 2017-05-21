/*TABLES to save specific information that can be inserted to the Buglist table*/
/*repeatable misbehaviour, unaccountable behaviour*/
CREATE TABLE FailureTypes(
	Id int IDENTITY(0,1) PRIMARY KEY,
	FailureType VARCHAR(100) NOT NULL);	
	
/*IC2H1, IC2H2, IC2H3* ControllerTypes is defined in TestInformation/
	
/*Low, Medium, High*/
CREATE TABLE Priorities(
	Id int IDENTITY(0,1) PRIMARY KEY,
	PriorityType VARCHAR(40) NOT NULL);
	
/*General,Interface, Option, Valve*/
CREATE TABLE HardwareIdentificationLevels1(
	Id int IDENTITY(0,1) PRIMARY KEY,
	HardwareIdentificationLevel1Type VARCHAR(100) NOT NULL);

/*Interface: RS232/EtherCAT, DeviceNET, Logic; Option: Cluster, PFO; Valve: SFV, FaceSeal, R62;*/
CREATE TABLE HardwareIdentificationLevels2(
	Id int IDENTITY(0,1) PRIMARY KEY,
	HardwareIdentificationLevel2Type VARCHAR(100) NOT NULL);
	
	
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
	
