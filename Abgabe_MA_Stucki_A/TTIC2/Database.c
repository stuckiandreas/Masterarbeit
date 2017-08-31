+++
//******************************************************************************************************************************
//  C-File
//******************************************************************************************************************************
//                                                               __    __     _______
//  VAT Vacuumvalves AG                                          \ \  / / /\ |__   __|
//  Control Valve Group                                           \ \/ / /  \   | |
//  CH-9469 Haag (Switzerland)                                     \  / / /\ \  | |
//                                                                  \/ /_/  \_\ |_|
//  Copyright(c) VAT Vacuumvalves AG / Switzerland
//
//******************************************************************************************************************************

//******************************************************************************************************************************
// Includes
//******************************************************************************************************************************
#include <ansi_c.h>
#include <cvinetv.h>
#include <stdint.h>
#include <stdio.h>
//Because of name conflicts, Windows headers must be included before utility.h and formatio.h.
#include <windows.h>
#include "cvi_db.h"
#include <formatio.h>
#include <utility.h>
#include "toolbox.h"
#undef GetSystemTime 

//Problem with the PI Constant from the toolbox.h Libary
#undef PI;
#include "Database.h"
#include "DefineEnums.h"
#include "Definelist.h"
#include "DefineStructFileService.h"
#include "HelpFunctions.h"
#include "SharedVariable.h"
#include "SelectListbox.h"
#include "SurfaceTest.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//interne Funtkionen
static void ShowError(void);

//Ventil Hardware
static SINT32 GetValveSerieId(SINT32 EnumValue);
static SINT32 GetInterfaceTypeId(SINT32 EnumValue);
static SINT32 GetControllerTypeId(SINT32 EnumValue);
static SINT32 GetOptionTypeId(SINT16 EnumValue);
static SINT32 GetControllerHardwareVersionId(SINT16 EnumValue);
static SINT32 GetInterfaceHardwareVersionId(SINT16 EnumValue);
static SINT32 GetControllerAssemblyVariantId(SINT16 EnumValue);
static SINT32 GetInterfaceAssemblyVariantId(SINT16 EnumValue); 
static SINT32 GetModule1TypeId(SINT16 EnumValue); 
static SINT32 GetModule1HardwareVersionId(SINT16 EnumValue); 
static SINT32 GetModule1AssemblyVariantId(SINT16 EnumValue); 
static SINT32 GetModule2TypeId(SINT16 EnumValue); 
static SINT32 GetModule2HardwareVersionId(SINT16 EnumValue); 
static SINT32 GetModule2AssemblyVariantId(SINT16 EnumValue); 
static SINT32 GetModule3TypeId(SINT16 EnumValue); 
static SINT32 GetModule3HardwareVersionId(SINT16 EnumValue); 
static SINT32 GetModule3AssemblyVariantId(SINT16 EnumValue); 
static SINT32 GetModule4TypeId(SINT16 EnumValue); 
static SINT32 GetModule4HardwareVersionId(SINT16 EnumValue); 
static SINT32 GetModule4AssemblyVariantId(SINT16 EnumValue); 

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
int hdbc = 0;       /* Handle to database connection    */
int hmap = 0;       /* Handle to map                    */
int hstmt = 0;      /* Handle to SQL statement          */
int resCode;        /* Result code                      */
char *pTableName;   /* table name                       */
BOOL8 ErrorState = FALSE, IsDatabaseConnectionOpen = FALSE;
//Verbindung zur Datenbank im Betrieb mit char string[200] = "DRIVER={ODBC Driver 11 for SQL Server};Server=chva0012;Database=SoftwareVersionsDatabase;Trusted_Connection=yes;";
//char ConnectionString[200] = "DRIVER={SQL Server};Server=CHVL0281\\SOFTWARE_DATABAS;Database=SoftwareVersionsDatabase;Trusted_Connection=yes;";
char ConnectionString[200] = "DRIVER={SQL Server};Server=DESKTOP-N4UJ39P\\ANDREAS_DATABASE;Database=SoftwareVersionsDatabase;Trusted_Connection=yes;"; 


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Trennt die Verbindung zur Datenbank 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_DisconnectDatabase(void)
{
	if(IsDatabaseConnectionOpen == TRUE)	
	{
		resCode = DBDisconnect (hdbc);
		if (resCode != DB_SUCCESS) ShowError();
		IsDatabaseConnectionOpen = FALSE;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Baut eine Verbindung zur Datenbank auf 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_ConnectDatabase(void)
{
	if(IsDatabaseConnectionOpen == FALSE)	
	{
		hdbc = DBConnect (ConnectionString);
		if(hdbc >= 0)	IsDatabaseConnectionOpen = TRUE;
		else	
		{
			MessagePopup("Error Database", "Not able to connect with database");
			printf("Error: %d", hdbc);
		}
	}
}












//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die TestVersionID zurück (Wenn nicht gefunden -1)
//  Input           : TestID:				ID des Tests
//									: TestVersion:	Index der Test Version
//  Output *Pointer : -
//         *return  : TestVersionID
//******************************************************************************************************************************
SINT32 Database_GetTestVersionID(SINT32 TestId, SINT32 TestVersion)
{
	SINT16 TestVersionNumber;
	SINT32 TestIdDatabase, TestVersionId;
	long TestVersionNumberStat, TestInformationIdStat, TestVersionIdStat;
	
	//TestID heraus finden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionId, &TestVersionIdStat);
		resCode = DBBindColShort(hstmt, 2, &TestVersionNumber, &TestVersionNumberStat);
		resCode = DBBindColInt(hstmt, 7, &TestIdDatabase, &TestInformationIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestIdDatabase == TestId)
			{
				if(TestVersionNumber == TestVersion)	
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					return TestVersionId;
				}
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test ID zurück, welche mit Hilfe mit dem Testnamen gesucht wird. 
//									: Wird keine Übereinstimmung gefunden wird -1 zurück gegeben 
//  Input           : pTestName: Name des Tests
//  Output *Pointer : -
//         *return  : Liefert die TestId
//******************************************************************************************************************************
SINT32 Database_GetTestIdFromTestInformation(char *pName)
{
	BOOL8 FindTestId = FALSE;
	UINT16 NameLength = TESTINFORMATION_NAME_LENGTH;
	SINT32 TestId = -1;
	long TestIdStat,NameStat;
	char TestName[TESTINFORMATION_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestId, &TestIdStat);
		resCode = DBBindColChar (hstmt, 2, NameLength, TestName, &NameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			//schauen ob der Test schon in der Datenbank hinterlegt ist
			if (StrICmpWithoutSurrWhiteSpace(TestName, pName) == 0)	
			{
				FindTestId = TRUE;
				break;
			}
		}
		
		//hier hat er es nicht gefunden
		if (FindTestId == FALSE)	
		{
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
			return TestId = -1;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return TestId;
}






//Firmware Informationen beschreiben
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Eintrag in die Drive Parameter File Liste 
//  Input           : pDriveParameterFile: Drive Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteDriveParameterFile(char *pDriveParameterFile)
{
	SINT16 ActivityStateDatabase = ACTIVITY_STATE_ACTIVE;
	int DriveParameterFileLength;
	long DriveParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, DriveParameterFileStat, ActivityStateStat;
	char DriveParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH], DriveParameterNameWithoutIndex[DRIVE_PARAMETER_FILE_NAME_LENGTH-1];
	
	//Kontrollieren, ob der Eintrag schon vorhanden ist
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM DriveParameterFile"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, DriveParameterNameLength, DriveParameterName, &DriveParameterFileStat, "");
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(DriveParameterName, pDriveParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Keinen Eintrag vorhanden, so soll dieser erstellt werden
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "Name", DriveParameterNameLength, DriveParameterName, &DriveParameterFileStat, "");
	resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateDatabase, &ActivityStateStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "DriveParameterFile");
	if (resCode != DB_SUCCESS) ShowError();
	
	//Wenn der neue Eintrag nicht Index A hat, so soll der alte Eintrag auf inaktiv gesetzt werden
	if(strstr(pDriveParameterFile, "A") == NULL)
	{
		strcpy(DriveParameterNameWithoutIndex, pDriveParameterFile);
		DriveParameterFileLength = strlen(pDriveParameterFile);
		//Index abschneiden
		DriveParameterNameWithoutIndex[DriveParameterFileLength-2] = 0;
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strstr(DriveParameterName, DriveParameterNameWithoutIndex) != 0 && ActivityStateDatabase == ACTIVITY_STATE_ACTIVE)
			{
				ActivityStateDatabase = ACTIVITY_STATE_NOT_ACTIVE;
				if (resCode != DB_SUCCESS) ShowError();
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
		}
	}
	
	//Neuer Eintrag erstellen
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	strcpy(DriveParameterName, pDriveParameterFile);
	ActivityStateDatabase = ACTIVITY_STATE_ACTIVE;
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Löscht, wenn der Eintrag existiert, in der Drive Parameter File Liste 
//										(ausser das File ist schon in einem Grundzustand abgespeichert)
//  Input           : pDriveParameterFile: Drive Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_DeleteDriveParameterFile(char *pDriveParameterFile)
{
	BOOL8 FindDriveParameterFileInInitialStateFirmware = FALSE;
	SINT16 ActivityStateDatabase;
	SINT32 DriveParameterFileId, DriveParameterFileDatabaseId;
	long DriveParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, DriveParameterFileStat, ActivityStateStat;
	char DriveParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	//Brauche zuerst die Drive Parameter File ID
	DriveParameterFileId = Database_GetDriveParameterFileId(pDriveParameterFile);
	
	//Darf nicht als Grundzustand abgespeichert sein (Wenn nicht -> wird gelöscht, Wenn schon wird ActivityState auf FALSE gesetzt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InitialStateFirmware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 6, &DriveParameterFileDatabaseId, &DriveParameterFileStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(DriveParameterFileDatabaseId == DriveParameterFileId)	FindDriveParameterFileInInitialStateFirmware = TRUE;
			break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
  hmap = DBBeginMap (hdbc);
  if (hmap <= 0) ShowError();
  resCode = DBMapColumnToChar (hmap, "Name", DriveParameterNameLength, DriveParameterName, &DriveParameterFileStat, "");
	resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateDatabase, &ActivityStateStat); 
  if (hmap <= 0) ShowError();
  hstmt = DBActivateMap (hmap, "DriveParameterFile");
  if (hmap <= 0) ShowError();
	
	//Wenn nicht -> löschen
	if(FindDriveParameterFileInInitialStateFirmware == FALSE)
	{
	  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden (Nur wenn er nicht in der Testresultaten eingetragen ist)
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (strcmp(DriveParameterName, pDriveParameterFile) == 0)	resCode = DBDeleteRecord(hstmt);
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
	//Sonst soll der Activity State auf False gesetzt werden
	else
	{
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (strcmp(DriveParameterName, pDriveParameterFile) == 0)
			{
				ActivityStateDatabase = ACTIVITY_STATE_NOT_ACTIVE;
				if (resCode != DB_SUCCESS) ShowError();
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Drive Parameter File Id zurück 
//  Input           : pDriveParameterFile: Drive Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
SINT32 Database_GetDriveParameterFileId(char *pDriveParameterFile)
{
	SINT32 DriveParameterId;
	long DriveParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, DriveParameterFileStat, DriveParameterStat;
	char DriveParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM DriveParameterFile"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &DriveParameterId, &DriveParameterStat);
		resCode = DBBindColChar (hstmt, 2, DriveParameterNameLength, DriveParameterName, &DriveParameterFileStat, "");
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(DriveParameterName, pDriveParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return DriveParameterId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Activity State des Drive Parameter Files zurück 
//  Input           : pDriveParameterFile: Drive Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
SINT16 Database_GetDriveParameterFileActive(char *pDriveParameterFile)
{
	SINT16 ActivityState;
	long DriveParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, DriveParameterFileStat, ActivityStateStat;
	char DriveParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM DriveParameterFile"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, DriveParameterNameLength, DriveParameterName, &DriveParameterFileStat, "");
		resCode = DBBindColShort  (hstmt, 3, &ActivityState, &ActivityStateStat);
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(DriveParameterName, pDriveParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ActivityState;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Activity State des Configuration Parameter Files zurück 
//  Input           : pConfigurationParameterFile: Configuration Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
SINT16 Database_GetConfigurationParameterFileActive(char *pConfigurationParameterFile)
{
	SINT16 ActivityState;
	long ConfigurationParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, ConfigurationParameterFileStat, ActivityStateStat;
	char ConfigurationParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, ConfigurationParameterNameLength, ConfigurationParameterName, &ConfigurationParameterFileStat, "");
		resCode = DBBindColShort  (hstmt, 3, &ActivityState, &ActivityStateStat);
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(ConfigurationParameterName, pConfigurationParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ActivityState;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Firmware Id basiert auf deren Namen 
//  Input           : pFirmwareName: Firmware Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
SINT32 Database_GetFirmwareId(char *pFirmwareName)
{
	SINT32 SoftwareId;
	long SoftwareNameLength = SOFTWARE_NAME_LENGTH, SoftwareNameStat, SoftwareIdStat, SystemNameLength = SYSTEM_NAME_LENGTH, SystemNameStat;
	char SoftwareName[SOFTWARE_NAME_LENGTH], SystemName[SYSTEM_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &SoftwareId, &SoftwareIdStat);
		resCode = DBBindColChar (hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColChar(hstmt, 4, SystemNameLength, SystemName, &SystemNameStat, "");
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(SystemName, "Motion Controller") == 0)	
			{
				//Zuerst den Namen Axis wegschneiden
				if(strstr(SoftwareName, "Axis1") != NULL)	HelpFunctions_StringReplace(" Axis1", "", SoftwareName);
				if(strstr(SoftwareName, "Axis2") != NULL)	HelpFunctions_StringReplace(" Axis2", "", SoftwareName);
				if (StrICmpWithoutSurrWhiteSpace(SoftwareName, pFirmwareName) == 0)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					return SoftwareId;
				}
			}
			else
			{
				if (StrICmpWithoutSurrWhiteSpace(SoftwareName, pFirmwareName) == 0)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					return SoftwareId;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Configuration Parameter File Id zurück 
//  Input           : pConfigurationParameterFile: Configuration Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
SINT32 Database_GetConfigurationParameterFileId(char *pConfigurationParameterFile)
{
	SINT32 ConfigurationParameterId;
	long ConfigurationParameterNameLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, ConfigurationParameterFileStat, ConfigurationParameterStat;
	char ConfigurationParameterName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ConfigurationParameterFile"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ConfigurationParameterId, &ConfigurationParameterStat);
		resCode = DBBindColChar (hstmt, 2, ConfigurationParameterNameLength, ConfigurationParameterName, &ConfigurationParameterFileStat, "");
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(ConfigurationParameterName, pConfigurationParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ConfigurationParameterId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Eintrag in die Configuration Parameter File Liste 
//  Input           : pConfigurationParameterFile: Configuration Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteConfigurationParameterFile(char *pConfigurationParameterFile)
{
	SINT16 ActivityStateDatabase;
	int ConfigurationParameterFileLength;
	long ConfigurationParameterNameLength = CONFIGURATION_FILE_NAME_LENGTH, ConfigurationParameterFileStat, ActivityStateStat;
	char ConfigurationParameterName[CONFIGURATION_FILE_NAME_LENGTH], ConfigurationParameterNameWithoutIndex[CONFIGURATION_FILE_NAME_LENGTH-1];
	
	//Kontrollieren, ob der Eintrag schon vorhanden ist
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ConfigurationParameterFile"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, ConfigurationParameterNameLength, ConfigurationParameterName, &ConfigurationParameterFileStat, "");
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(ConfigurationParameterName, pConfigurationParameterFile) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Keinen Eintrag vorhanden, so soll dieser erstellt werden
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "Name", ConfigurationParameterNameLength, ConfigurationParameterName, &ConfigurationParameterFileStat, "");
	resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateDatabase, &ActivityStateStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ConfigurationParameterFile");
	if (resCode != DB_SUCCESS) ShowError();
	
	//Wenn der neue Eintrag nicht Index A hat, so soll der alte Eintrag auf inaktiv gesetzt werden
	if(strstr(pConfigurationParameterFile, "A") == NULL)
	{
		strcpy(ConfigurationParameterNameWithoutIndex, pConfigurationParameterFile);
		ConfigurationParameterFileLength = strlen(pConfigurationParameterFile);
		//Index abschneiden
		ConfigurationParameterNameWithoutIndex[ConfigurationParameterFileLength-2] = 0;
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strstr(ConfigurationParameterName, ConfigurationParameterNameWithoutIndex) != 0 && ActivityStateDatabase == ACTIVITY_STATE_ACTIVE)
			{
				ActivityStateDatabase = ACTIVITY_STATE_NOT_ACTIVE;
				if (resCode != DB_SUCCESS) ShowError();
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
		}
	}
	
	//Neuer Eintrag erstellen
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	strcpy(ConfigurationParameterName, pConfigurationParameterFile);
	ActivityStateDatabase = ACTIVITY_STATE_ACTIVE;
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Löscht, wenn der Eintrag existiert, in der Configuration Parameter File Liste 
//										(ausser das File ist schon in einem Grundzustand abgespeichert)
//  Input           : pConfigurationParameterFile: Configuration Parameter File Name
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_DeleteConfigurationParameterFile(char *pConfigurationParameterFile)
{
	BOOL8 FindConfigurationParameterFileInitialStateFirmware = FALSE; 
	SINT16 ActivityStateDatabase;
	SINT32 ConfigurationParameterFileId, ConfigurationParameterFileDatabaseId;
	long ConfigurationParameterNameLength = CONFIGURATION_FILE_NAME_LENGTH, ConfigurationParameterFileStat, ActivityStateStat;
	char ConfigurationParameterName[CONFIGURATION_FILE_NAME_LENGTH];
	
	//Brauche zuerst die Configuration Parameter File ID
	ConfigurationParameterFileId = Database_GetConfigurationParameterFileId(pConfigurationParameterFile);
	
	//Darf nicht als Grundzustand abgespeichert sein (Wenn nicht -> wird gelöscht, Wenn schon wird ActivityState auf FALSE gesetzt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InitialStateFirmware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 7, &ConfigurationParameterFileDatabaseId, &ConfigurationParameterFileStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(ConfigurationParameterFileDatabaseId == pConfigurationParameterFile)	FindConfigurationParameterFileInitialStateFirmware = TRUE;
			break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
  hmap = DBBeginMap (hdbc);
  if (hmap <= 0) ShowError();
  resCode = DBMapColumnToChar (hmap, "Name", ConfigurationParameterNameLength, ConfigurationParameterName, &ConfigurationParameterFileStat, "");
	resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateDatabase, &ActivityStateStat);
  if (hmap <= 0) ShowError();
  hstmt = DBActivateMap (hmap, "ConfigurationParameterFile");
  if (hmap <= 0) ShowError();

	//Wenn nicht -> löschen
	if(FindConfigurationParameterFileInitialStateFirmware == FALSE)
	{
	  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden (Nur wenn er nicht in der Testresultaten eingetragen ist)
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (strcmp(ConfigurationParameterName, pConfigurationParameterFile) == 0)	resCode = DBDeleteRecord(hstmt);
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
	//Sonst soll der Activity State auf False gesetzt werden
	else
	{
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (strcmp(ConfigurationParameterName, pConfigurationParameterFile) == 0)
			{
				ActivityStateDatabase = ACTIVITY_STATE_NOT_ACTIVE;
				if (resCode != DB_SUCCESS) ShowError();
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die PSS Nummer zurück anhand des Firmware Namen 
//  Input           : pFirmwareName: Name der Firmware
//  Output *Pointer : -
//         *return  : PSS Nummer der Firmware
//******************************************************************************************************************************
int Database_GetPSSNumberFromFirmware(char *pFirmwareName)
{
	int PSSNumber;
	long SoftwareNameLength = SOFTWARE_NAME_LENGTH, SoftwareNameStat, PSSNumberStat;
	char SoftwareName[SOFTWARE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColInt  (hstmt, 8, &PSSNumber, &PSSNumberStat);
		
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(SoftwareName, pFirmwareName) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return PSSNumber;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die PSS Nummern zurück, welche unter dem Motion Controller Namen in der Datenbank hinterlegt sind
//										Mehrere Einträge, da mehrere Achsen möglich sind
//  Input           : pMotionControllerName: Name der Motion Controller Firmware
//  Output *Pointer : pPSSNumberList; Alle hinterlegten Motion Controller Firmware Achsen mit der PSS Nummer
//         *return  : -
//******************************************************************************************************************************
void Database_GetPSSNumberFromMotionControllerFirmware(char *pMotionControllerName, int *pPSSNumberList)
{
	int PSSNumber;
	long SoftwareNameLength = SOFTWARE_NAME_LENGTH, SoftwareNameStat, PSSNumberStat;
	char SoftwareName[SOFTWARE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions"); 
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColInt  (hstmt, 8, &PSSNumber, &PSSNumberStat);
		
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strstr(SoftwareName, pMotionControllerName) != NULL)
			{
				//Schauen welche Achse hier definiert wird
				if(strstr(SoftwareName, "Axis1") != NULL)				pPSSNumberList[0] = PSSNumber;
				else if(strstr(SoftwareName, "Axis2") != NULL)	pPSSNumberList[1] = PSSNumber;
				else if(strstr(SoftwareName, "Axis3") != NULL)	pPSSNumberList[2] = PSSNumber;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}







//Test Kollektion
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Test Kollektionen zurück. 
//										Mit der Option ob alle oder nur die aktiven Test Kollektionen gezählt werden sollen 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Test Kollektionen
//******************************************************************************************************************************
UINT16 Database_GetCountTestCollections(void)
{
	SINT16 ActivityStateDatabase = -1;
	SINT32 TestCollectionId = -1;
	long TestCollectionIdStat, ActivityStateStat;
	
	//Auf 0 setzen, wenn der Counter wieder neu ausgelesen werden soll
	CountTestCollections = 0;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestCollectionId, &TestCollectionIdStat);
		resCode = DBBindColShort  (hstmt, 5, &ActivityStateDatabase, &ActivityStateStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			//Nur aktive Test Kollektionen zählen
			if(IsShowAllTestCollectionSelected == FALSE && ActivityStateDatabase == ACTIVITY_STATE_ACTIVE)	CountTestCollections++;
			else if(IsShowAllTestCollectionSelected == TRUE)																								CountTestCollections++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountTestCollections; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Activity State der Test Kollektion zurück 
//  Input           : TestCollectionId: Id der Test Kollektion
//  Output *Pointer : -
//         *return  : Activity State der Test Kollektion
//******************************************************************************************************************************
SINT16 Database_GetActivityState(SINT32 TestCollectionId)
{
	SINT16 ActivityState = 0;
	SINT32 TestCollectionDatabaseId = -1;																																																																							 
	long TestCollectionIdStat; 
	long ActivityStateStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestCollectionDatabaseId, &TestCollectionIdStat);
		resCode = DBBindColShort  (hstmt, 5, &ActivityState, &ActivityStateStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	break;	
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ActivityState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Test Kollektion Struktur mit den Kollektionsnamen sowie der Id 
//  Input           : -
//  Output *Pointer : pTestCollections: Test Kollektion Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestCollectionsStructure(void)
{
	UINT16 TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH, IndexTestCollection = 0;
	SINT16 ActivityStateDatabase = -1;
	SINT32 TestCollectionId = -1;
	long TestCollectionIdStat, TestCollectionNameStat, ActivityStateStat, CreationDateLength = DATE_TYPE_LENGTH, CreationDateStat;
	char TestCollectionName[TEST_COLLECTION_NAME_LENGTH], CretionDateString[DATE_TYPE_LENGTH];
	
	Database_ConnectDatabase();
	
	//Zuerst die Anzahl Test Kollektionen auslesen
	Database_GetCountTestCollections();
	
	//Wenn keine Test Kollektionen eingetragen sind, so müssen auch keine ausgelesen werden
	if(CountTestCollections > 0)
	{
		if(pTestCollections != NULL)	free(pTestCollections);
		pTestCollections = malloc(sizeof(struct testCollection_type) * CountTestCollections);
	
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
		if (hstmt > 0)
		{
			resCode = DBBindColInt  (hstmt, 1, &TestCollectionId, &TestCollectionIdStat);
			resCode = DBBindColChar (hstmt, 2, TestCollectionNameLength, TestCollectionName, &TestCollectionNameStat, "");
			//braucht dieses Format -> sonst kann der Wert nicht ausgelesen werden
			resCode = DBBindColChar (hstmt, 4, CreationDateLength, CretionDateString, &CreationDateStat, "yyyy-mm-dd");
			resCode = DBBindColShort  (hstmt, 5, &ActivityStateDatabase, &ActivityStateStat);
		
			if (resCode != DB_SUCCESS) ShowError();
			while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
			{
				//Nur aktive Test Kollektionen in Liste aufnehmen
				if(IsShowAllTestCollectionSelected == FALSE && ActivityStateDatabase == ACTIVITY_STATE_ACTIVE)	
				{
					pTestCollections[IndexTestCollection].TestCollectionId = TestCollectionId;
					strcpy(pTestCollections[IndexTestCollection].TestCollectionName, TestCollectionName);
					strcpy(pTestCollections[IndexTestCollection].CreationDate, CretionDateString);
					pTestCollections[IndexTestCollection].ActivityState = ActivityStateDatabase;
					IndexTestCollection++;
				}
				else if(IsShowAllTestCollectionSelected == TRUE)
				{
					pTestCollections[IndexTestCollection].TestCollectionId = TestCollectionId;
					strcpy(pTestCollections[IndexTestCollection].TestCollectionName, TestCollectionName);
					strcpy(pTestCollections[IndexTestCollection].CreationDate, CretionDateString);
					pTestCollections[IndexTestCollection].ActivityState = ActivityStateDatabase;
					IndexTestCollection++;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Es wird eine neue Test Kolelktion erstellt. Ist der alte Eintrag in einem Resultat vorhanden, so wird dieser nicht gelöscht sonder der Activity State auf 0 gesetzt
//  Input           : pName:				Name der Test Collection
//										pDescription:	Beschreibung der Test Collection
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestCollectionTable(char *pName, char *pDescription)
{
	UINT16 TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH, TestCollectionDescriptionLength = TEST_COLLECTION_DESCR_LENGTH;
	SINT16 ActivityStateValue, k;
	SINT32 TestCollectionId, TestId;
	int CountSelectedTests;
	long TestCollectionNameStat, TestCollectionDescriptionStat, ActivityStateStat, CreationDateLength = DATE_TYPE_LENGTH, CreationDateStat;
	char TestCollectionName[TEST_COLLECTION_NAME_LENGTH], TestCollectionDescription[TEST_COLLECTION_DESCR_LENGTH], CretionDateString[DATE_TYPE_LENGTH];
	
	//Datum anfragen
	SYSTEMTIME str_t;  
	GetSystemTime(&str_t);
	sprintf(CretionDateString, "%d%02d%02d", str_t.wYear, str_t.wMonth, str_t.wDay); 
	
	//Wenn es einen alten aktiven Eintrag gibt mit dem gleichen Namen, so soll dieser gelöscht werden
	TestCollectionId = Database_GetActiveTestCollectionId(pName);
	if(TestCollectionId != -1)	Database_DeleteTestCollectionEntry(TestCollectionId);
	
	//Neuer Eintrag in Datenbank schreiben
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "Name", TestCollectionNameLength, TestCollectionName, &TestCollectionNameStat, "");
	resCode = DBMapColumnToChar (hmap, "Description", TestCollectionDescriptionLength, TestCollectionDescription, &TestCollectionDescriptionStat, "");
	resCode = DBMapColumnToChar (hmap, "CreationDate", CreationDateLength, CretionDateString, &CreationDateStat, "YYYYMMDD");
	resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateValue, &ActivityStateStat);
	
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestCollection");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	strcpy(TestCollectionName, pName);
	strcpy(TestCollectionDescription, pDescription);
	strcpy(CretionDateString, CretionDateString); 
	ActivityStateValue = ACTIVITY_STATE_ACTIVE;
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
	
	//Brauche die TestCollectionId des erzeugten Eintrages
	TestCollectionId = Database_GetActiveTestCollectionId(TestCollectionName);
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &CountSelectedTests);
	
	//Weiter müssen noch alle ausgewählten Tests in der Verknüpfungstabelle eingetragen werden
	for (k = 0; k < CountSelectedTests; k ++)
	{
		TestId = Database_GetTestIdFromTestInformation(pSelectedTests[k].pTests->TestName);
		Database_WriteTestCollectionTestInformationTable(TestId, TestCollectionId);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Collection Id zurück, wenn es sich um eine nicht aktive Test Kollektion handelt
//  Input           : pName:						Name der Test Collection
//										TestCollection:		Struktur Eintrag der Test Kollektion
//  Output *Pointer : -
//         *return  : Test Collection Id
//******************************************************************************************************************************
SINT32 Database_GetTestCollectionId(char *pName, struct testCollection_type TestCollection)
{
	SINT16 ActivityStateValue;
	SINT32 TestCollectionId = -1;
	long TestCollectionNameStat, TestCollectionIdStat, TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH, CreationDateLength = DATE_TYPE_LENGTH; 
	long CreationDateStringStat, ActivityStateStat;
	char TestCollectionName[TEST_COLLECTION_NAME_LENGTH], CreationDate[DATE_TYPE_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestCollectionId, &TestCollectionIdStat);
		resCode = DBBindColChar (hstmt, 2, TestCollectionNameLength, TestCollectionName, &TestCollectionNameStat, "");
		resCode = DBBindColChar (hstmt, 4, CreationDateLength, CreationDate, &CreationDateStringStat, "");
		resCode = DBBindColShort (hstmt, 5, &ActivityStateValue, &ActivityStateStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(pName, TestCollectionName) == 0 && strcmp(TestCollection.CreationDate, CreationDate) == 0 && ActivityStateValue == TestCollection.ActivityState)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return TestCollectionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Collection Id zurück, wenn es sich um eine aktive Test Kollektion handelt
//  Input           : pName:				Name der Test Collection
//  Output *Pointer : -
//         *return  : Test Collection Id
//******************************************************************************************************************************
SINT32 Database_GetActiveTestCollectionId(char *pName)
{
	SINT16 ActivityStateValue;
	SINT32 TestCollectionId = -1;
	long TestCollectionNameStat, TestCollectionIdStat, TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH, CreationDateLength = DATE_TYPE_LENGTH, CreationDateStringStat,ActivityStateStat;
	char TestCollectionName[TEST_COLLECTION_NAME_LENGTH], CreationDate[DATE_TYPE_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestCollectionId, &TestCollectionIdStat);
		resCode = DBBindColChar (hstmt, 2, TestCollectionNameLength, TestCollectionName, &TestCollectionNameStat, "");
		resCode = DBBindColChar (hstmt, 4, CreationDateLength, CreationDate, &CreationDateStringStat, "");
		resCode = DBBindColShort (hstmt, 5, &ActivityStateValue, &ActivityStateStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(pName, TestCollectionName) == 0 && ActivityStateValue == ACTIVITY_STATE_ACTIVE)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return TestCollectionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt eine neue Verknüpfung einer Test Collection mit der Test Information Id
//  Input           : TestInformationId:	Id des Tests
//										TestCollectionId:		Id der Test Collection
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestCollectionTestInformationTable(SINT32 TestInformationId, SINT32 TestCollectionId)
{
	SINT32 TestInformationDatabaseId, TestCollectionDatabaseId;
	long TestInformationIdStat, TestCollecitonIdStat;
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToInt (hmap, "TestInformation_Id", &TestInformationDatabaseId, &TestInformationIdStat);
	resCode = DBMapColumnToInt (hmap, "TestCollection_Id", &TestCollectionDatabaseId, &TestCollecitonIdStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestCollection2TestInformation");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	TestInformationDatabaseId = TestInformationId;
	TestCollectionDatabaseId = TestCollectionId;
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert mit Hilfe der Test Collection Id deren Name und Beschreibung 
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : pName:						Name der Test Collection
//										pDescription:			Beschreibung der Test Collection
//         *return  : -
//******************************************************************************************************************************
void Database_GetTestCollectionTable(SINT32 TestCollectionId, char* pName, char* pDescription)
{
	SINT32 TestCollectionDatabaseId = -1;																																																																							 
	long TestCollectionNameStat, TestCollectionIdStat, TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH; 
	long TestCollectionDescriptionLength = TEST_COLLECTION_DESCR_LENGTH, TestCollectionDescriptionStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestCollectionDatabaseId, &TestCollectionIdStat);
		resCode = DBBindColChar (hstmt, 2, TestCollectionNameLength, pName, &TestCollectionNameStat, "");
		resCode = DBBindColChar (hstmt, 3, TestCollectionDescriptionLength, pDescription, &TestCollectionDescriptionStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	break;	
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt alle Tests, welche mit der Test Colelction verbunden sin, in das Info Textfile
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestsFromTestCollectionInInfoListbox(SINT32 TestCollectionId)
{
	UINT16 CountTestsInTestCollection = 0, IndexTestsInTestCollectionList = 0;
	SINT32 TestInformationDatabaseId, TestCollectionDatabaseId;
	long TestInformationIdStat, TestCollecitonIdStat;
	char TestName[TESTINFORMATION_NAME_LENGTH];
	
	//1 Durchgang die Anzahl Tests in der Test Collection heraus finden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
		resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	CountTestsInTestCollection++;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//2 Durchlauf alle Test Information Id in eine Liste schreiben
	if(CountTestsInTestCollection != 0)
	{
		SINT32 TestInformationListId[CountTestsInTestCollection];
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
			resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
			if (resCode != DB_SUCCESS) ShowError();
			while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
			{
				if(TestCollectionDatabaseId == TestCollectionId)	
				{
					TestInformationListId[IndexTestsInTestCollectionList] = TestInformationDatabaseId;
					IndexTestsInTestCollectionList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
		
		//3 Durchgang soll nun der Test Name heraus geholt werden und dieser ins Info Feld geschrieben werden
		for(IndexTestsInTestCollectionList = 0; IndexTestsInTestCollectionList < CountTestsInTestCollection; IndexTestsInTestCollectionList++)
		{
			Database_GetTestName(TestInformationListId[IndexTestsInTestCollectionList], TestName);
			InsertListItem(STpanel, STPANEL_TREE_INFO, -1, TestName, 0);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt alle Tests, welche mit der Test Colelction verbunden sin, in das Selected Listbox
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestsInSelectedListboxFromTestCollection(SINT32 TestCollectionId)
{
	UINT16 CountTestsInTestCollection = 0, IndexTestsInTestCollectionList = 0, i;
	SINT32 TestInformationDatabaseId, TestCollectionDatabaseId;
	long TestInformationIdStat, TestCollecitonIdStat;
	char TestName[TESTINFORMATION_NAME_LENGTH];
	
	//1 Durchgang die Anzahl Tests in der Test Collection heraus finden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
		resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	CountTestsInTestCollection++;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//2 Durchlauf alle Test Information Id in eine Liste schreiben
	if(CountTestsInTestCollection != 0)
	{
		SINT32 TestInformationListId[CountTestsInTestCollection];
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
			resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
			if (resCode != DB_SUCCESS) ShowError();
			while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
			{
				if(TestCollectionDatabaseId == TestCollectionId)	
				{
					TestInformationListId[IndexTestsInTestCollectionList] = TestInformationDatabaseId;
					IndexTestsInTestCollectionList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
		
		//3 Durchgang soll nun der Test Name heraus geholt werden und dieser im Selected Listbox angezeigt werden
		for(IndexTestsInTestCollectionList = 0; IndexTestsInTestCollectionList < CountTestsInTestCollection; IndexTestsInTestCollectionList++)
		{
			Database_GetTestName(TestInformationListId[IndexTestsInTestCollectionList], TestName);
			for(i = 0; i < CountTests; i++)
			{
				if (strcmp(pTests[i].TestName, TestName) == 0)
				{
					pSelectedTests[IndexTestsInTestCollectionList].pTests = &pTests[i];
					pSelectedTests[IndexTestsInTestCollectionList].pTests->SelectTestSelected = 0;
					InsertListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexTestsInTestCollectionList, pSelectedTests[IndexTestsInTestCollectionList].pTests->TestName, 0);
					CountSelectedFiles++;
					break;
				}
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Löscht den Test Kollektion Eintrag (Dabei werden alle Verknüpfungen mitgelöscht)
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_DeleteTestCollectionEntry(SINT32 TestCollectionId)
{
	BOOL8 FindTestCollectionInInitialStateFirmware = FALSE;
	SINT32 TestInformationDatabaseId, TestCollectionDatabaseId; 
	long TestInformationIdStat, TestCollecitonIdStat;
	
	//Darf nicht als Grundzustand abgespeichert sein (Wenn nicht -> wird gelöscht, Wenn schon wird ActivityState auf FALSE gesetzt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InitialStateFirmware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 8, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	
			{
				FindTestCollectionInInitialStateFirmware = TRUE;
				break;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	if(FindTestCollectionInInitialStateFirmware == FALSE)
	{
		//Zuerst alle Verknüpfungen TestCollection TestInformation löschen
	  hmap = DBBeginMap (hdbc);
	  if (hmap <= 0) ShowError();
		resCode = DBMapColumnToInt (hmap, "TestInformation_Id", &TestInformationDatabaseId, &TestInformationIdStat);
		resCode = DBMapColumnToInt (hmap, "TestCollection_Id", &TestCollectionDatabaseId, &TestCollecitonIdStat);
	  if (hmap <= 0) ShowError();

	  hstmt = DBActivateMap (hmap, "TestCollection2TestInformation");
	  if (hmap <= 0) ShowError();

	  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (TestCollectionDatabaseId == TestCollectionId)	resCode = DBDeleteRecord(hstmt);
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	
		//Anschliessend kann die Test Collection Eintrag entfernt werden
	  hmap = DBBeginMap (hdbc);
	  if (hmap <= 0) ShowError();
		resCode = DBMapColumnToInt (hmap, "Id", &TestCollectionDatabaseId, &TestCollecitonIdStat);
	  if (hmap <= 0) ShowError();

	  hstmt = DBActivateMap (hmap, "TestCollection");
	  if (hmap <= 0) ShowError();

	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (TestCollectionDatabaseId == TestCollectionId)	
			{
				resCode = DBDeleteRecord(hstmt);
				break;
			}
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
	//Darf nicht gelöscht werden -> ActivityState auf 0 setzen
	else	Database_WriteActivityStateFalseTestCollection(TestCollectionId);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Prüft ob sich die ausgewählte Test Kollektion mit der in der Datenbank eingetragene Test Kollektion unterscheidet
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : -
//         *return  : Resultat der Überprüfung
//******************************************************************************************************************************
BOOL8 Database_CheckIfTestCollectionHasChanged(SINT32 TestCollectionId)
{
	BOOL8 TestCollectionHasChanged = FALSE, FoundTestInTestCollection = FALSE;
	UINT16 CountTestsInTestCollection = 0, IndexTestsInTestCollectionList = 0, IndexTestInSelectedList;
	SINT32 TestInformationDatabaseId, TestCollectionDatabaseId;
	long TestInformationIdStat, TestCollecitonIdStat;
	char TestName[TESTINFORMATION_NAME_LENGTH];
	
	//1 Durchgang die Anzahl Tests in der Test Collection heraus finden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
		resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	CountTestsInTestCollection++;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Vergleichen ob die Anzahl Elemte in der Test Collection sowie in der abgespeicherten Version übereinstimmen
	if(CountSelectedFiles != CountTestsInTestCollection) return TRUE;
	
	//2 Durchlauf alle Test Information Id in eine Liste schreiben
	if(CountTestsInTestCollection != 0)
	{
		SINT32 TestInformationListId[CountTestsInTestCollection];
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection2TestInformation");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestInformationDatabaseId, &TestInformationIdStat);
			resCode = DBBindColInt (hstmt, 2, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		
			if (resCode != DB_SUCCESS) ShowError();
			while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
			{
				if(TestCollectionDatabaseId == TestCollectionId)	
				{
					TestInformationListId[IndexTestsInTestCollectionList] = TestInformationDatabaseId;
					IndexTestsInTestCollectionList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
		
		//3 Durchgang vergleicht alle Test Kollektionen in der Datenbank mit den ausgewählten Tests in der Testoberfläche
		//Äussere forSchleife mit Einträgen der Test welche sich in der rechten Spalte sich befinden
		//Die Reihenfolge der Auflistung der Tests spilt keine Rolle
		for(IndexTestInSelectedList = 0; IndexTestInSelectedList < CountTestsInTestCollection; IndexTestInSelectedList++)
		{
			for(IndexTestsInTestCollectionList = 0; IndexTestsInTestCollectionList < CountTestsInTestCollection; IndexTestsInTestCollectionList++)
			{
				Database_GetTestName(TestInformationListId[IndexTestsInTestCollectionList], TestName);
				if(strcmp(TestName, pSelectedTests[IndexTestInSelectedList].pTests->TestName) == 0) 
				{
					FoundTestInTestCollection = TRUE;
					break;
				}
			}
			if(FoundTestInTestCollection == FALSE)
			{
				 TestCollectionHasChanged = TRUE;
				 break;
			}
			else
			{
				FoundTestInTestCollection = FALSE;
				IndexTestInSelectedList++;
			}
		}
	}
	return TestCollectionHasChanged;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beschreibt den ActivityState der ausgewählten Test Kollektion auf den Wert False
//  Input           : TestCollectionId: Id der Test Collection 
//  Output *Pointer : -
//         *return  : Resultat der Überprüfung
//******************************************************************************************************************************
void Database_WriteActivityStateFalseTestCollection(SINT32 TestCollectionId)
{
	BOOL8 FindEntry = FALSE;
	SINT16 ActivityStateDatabase = -1, ActivityStateRead = 0;
	SINT32 TestCollectionDatabaseId = -1;
	long TestCollecitonIdStat, ActivityStateStat, TestCollectionNameLength = TEST_COLLECTION_NAME_LENGTH, TestCollectionNameStat, CreationDateLength = DATE_TYPE_LENGTH;
	long CreationDateStat;
	char TestCollectionName[TEST_COLLECTION_NAME_LENGTH] = "", CreationDate[DATE_TYPE_LENGTH] = "", TestCollectionNameDatabase[TEST_COLLECTION_NAME_LENGTH] = "";
	
	//Zuerst mit Hilfe der Id den Namen, Creation Date und Activity State auslesen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestCollectionDatabaseId, &TestCollecitonIdStat);
		resCode = DBBindColChar (hstmt, 2, TestCollectionNameLength, TestCollectionName, &TestCollectionNameStat, "");
		resCode = DBBindColChar (hstmt, 4, CreationDateLength, CreationDate, &CreationDateStat, "yyyy-mm-dd");
		resCode = DBBindColShort  (hstmt, 5, &ActivityStateRead, &ActivityStateStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	
			{
				FindEntry = TRUE;
				break;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//Nur wenn der Activity State auf True steht
	if(FindEntry == TRUE && ActivityStateRead == ACTIVITY_STATE_ACTIVE)
	{
	  hmap = DBBeginMap (hdbc);
	  if (hmap <= 0) ShowError();
		resCode = DBMapColumnToChar (hmap, "Name", TestCollectionNameLength, TestCollectionNameDatabase, &TestCollectionNameStat, "");
		resCode = DBMapColumnToShort (hmap, "ActivityState", &ActivityStateDatabase, &ActivityStateStat);
	  if (hmap <= 0) ShowError();
	  hstmt = DBActivateMap (hmap, "TestCollection");
	  if (hmap <= 0) ShowError();
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
	  	if (strcmp(TestCollectionNameDatabase, TestCollectionName) == 0 && ActivityStateDatabase == ActivityStateRead)	
			{
				ActivityStateDatabase = ACTIVITY_STATE_NOT_ACTIVE;
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				break;
			}
	  }
	  resCode = DBDeactivateMap (hmap);
	  if (hmap <= 0) ShowError();
	}
	else if(FindEntry == FALSE)	MessagePopup("Database Error","Test collection id not found in database");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt das Datum der Test Kollektion zurück
//  Input           : TestCollectionId: 						Id der Test Collection 
//  Output *Pointer : pTestCollectionCreationDate:	Datum von der Erstellung der Test Kollektion
//         *return  : Resultat der Überprüfung
//******************************************************************************************************************************
void Database_GetDateFromTestCollectionTable(SINT32 TestCollectionId, char *pTestCollectionCreationDate)
{
	SINT32 TestCollectionDatabaseId = -1;
	long TestCollectionIdStat, CreationDateLength = DATE_TYPE_LENGTH, CreationDateStringStat;
	char CreationDate[DATE_TYPE_LENGTH] = "";
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollection");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestCollectionDatabaseId, &TestCollectionIdStat);
		resCode = DBBindColChar (hstmt, 4, CreationDateLength, CreationDate, &CreationDateStringStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestCollectionDatabaseId == TestCollectionId)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	strcpy(pTestCollectionCreationDate, " ");
	strcat(pTestCollectionCreationDate, CreationDate);
}










//Filter List
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Moduls zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Moduls
//******************************************************************************************************************************
UINT16 Database_GetCountModul(void)
{
	UINT16 CountModuls = 0;
	SINT32 ModulId = -1;
	long ModulIdStat, ModulNameLength = MODUL_NAME_LENGTH, ModulNameStat;
	char ModulName[MODUL_NAME_LENGTH];
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ModulId, &ModulIdStat);
		resCode = DBBindColChar (hstmt, 2, ModulNameLength, ModulName, &ModulNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ModulName, "Unknown") != 0 && strcmp(ModulName, "Not_Avaiable") != 0)	CountModuls++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountModuls; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Interfaces zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Interfaces
//******************************************************************************************************************************
UINT16 Database_GetCountInterface(void)
{
	UINT16 CountInterfaces = 0;
	SINT32 InterfaceId = -1;
	long InterfaceIdStat, InterfaceNameLength = INTERFACE_NAME_LENGTH, InterfaceNameStat;
	char InterfaceName[INTERFACE_NAME_LENGTH];
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceId, &InterfaceIdStat);
		resCode = DBBindColChar (hstmt, 4, InterfaceNameLength, InterfaceName, &InterfaceNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(InterfaceName, "Unknown") != 0 && strcmp(InterfaceName, "Not_Avaiable") != 0)	CountInterfaces++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountInterfaces; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Ventilreihen zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Ventilreihen
//******************************************************************************************************************************
UINT16 Database_GetCountValveSerie(void)
{
	UINT16 CountValveSeries = 0;
	SINT32 ValveSerieId = -1;
	long ValveSerieIdStat,ValveSerieNameLength = VALVE_SERIE_NAME_LENGTH, ValveSerieNameStat;
	char ValveSerieName[VALVE_SERIE_NAME_LENGTH]; 
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ValveSerieId, &ValveSerieIdStat);
		resCode = DBBindColChar (hstmt, 4, ValveSerieNameLength, ValveSerieName, &ValveSerieNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ValveSerieName, "Unknown") != 0 && strcmp(ValveSerieName, "Not_Avaiable") != 0)	CountValveSeries++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountValveSeries; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Controller Typen zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Controller Typen
//******************************************************************************************************************************
UINT16 Database_GetCountControllerType(void)
{
	UINT16 CountControllerTypes = 0;
	SINT32 ControllerTypeId = -1;
	long ControllerTypeIdStat, ControllerTypeNameStat, ControllerTypeNameLength = CONTROLLER_TYPE_NAME_LENGTH;
	char ControllerTypeName[CONTROLLER_TYPE_NAME_LENGTH];
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerTypeId, &ControllerTypeIdStat);
		resCode = DBBindColChar (hstmt, 4, ControllerTypeNameLength, ControllerTypeName, &ControllerTypeNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ControllerTypeName, "Unknown") != 0 && strcmp(ControllerTypeName, "Not_Avaiable") != 0)	CountControllerTypes++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountControllerTypes; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter Optionen zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl Optionen
//******************************************************************************************************************************
UINT16 Database_GetCountOption(void)
{
	UINT16 CountOptions = 0;
	SINT32 OptionId = -1;
	long OptionIdStat, OptionNameLength = OPTION_NAME_LENGTH, OptionNameStat;
	char OptionName[OPTION_NAME_LENGTH];
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &OptionId, &OptionIdStat);
		resCode = DBBindColChar (hstmt, 3, OptionNameLength, OptionName, &OptionNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(OptionName, "Unknown") != 0 && strcmp(OptionName, "Not_Avaiable") != 0)	CountOptions++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountOptions; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Anzahl abgespeicherter NI Hardware Typen zurück. 
//  Input           : -
//  Output *Pointer : -
//         *return  : Anzahl NI Hardware Typen
//******************************************************************************************************************************
UINT16 Database_GetCountNIHardwareType(void)
{
	UINT16 CountNIHardwareTypes = 0;
	SINT32 NIHardwareTypeId = -1;
	long NIHardwareTypeIdStat, NIHardwareTypeNameLength = NI_HARDWARE_NAME_LENGTH, NIHardwareTypeNameStat;
	char NIHardwareTypeName[NI_HARDWARE_NAME_LENGTH];
	
	Database_ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &NIHardwareTypeId, &NIHardwareTypeIdStat);
		resCode = DBBindColChar (hstmt, 3, NIHardwareTypeNameLength, NIHardwareTypeName, &NIHardwareTypeNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(NIHardwareTypeName, "Unknown") != 0 && strcmp(NIHardwareTypeName, "Not_Avaiable") != 0)	CountNIHardwareTypes++;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	return CountNIHardwareTypes; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Modul Struktur mit den Modulnamen 
//  Input           : -
//  Output *Pointer : pModuls: Modul Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteModulStructure(struct testModul_type *pModuls)
{
	UINT16 CountModuls = 0, ModulNameLength = MODUL_NAME_LENGTH;
	long ModulNameStat;
	char ModulName[MODUL_NAME_LENGTH];
	
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, ModulNameLength, ModulName, &ModulNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ModulName, "Unknown") != 0 && strcmp(ModulName, "Not_Avaiable") != 0)	
			{
				strcpy(pModuls[CountModuls].ModulName, ModulName);
				CountModuls++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Interface Struktur mit den Interfacenamen 
//  Input           : -
//  Output *Pointer : pInterfaces: Interface Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteInterfaceStructure(struct testInterface_type *pInterfaces)
{
	UINT16 CountInterfaces = 0, InterfaceNameLength = INTERFACE_NAME_LENGTH;
	long InterfaceNameStat;
	char InterfaceName[INTERFACE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 4, InterfaceNameLength, InterfaceName, &InterfaceNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(InterfaceName, "Unknown") != 0 && strcmp(InterfaceName, "Not_Avaiable") != 0)	
			{
				strcpy(pInterfaces[CountInterfaces].InterfaceName, InterfaceName);
				CountInterfaces++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Valve Series Struktur mit den Ventilreihennamen 
//  Input           : -
//  Output *Pointer : pValveSeries: Ventilreihe Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteValveSerieStructure(struct testValveSerie_type *pValveSeries)
{
	UINT16 CountValveSeries = 0, ValveSerieNameLength = VALVE_SERIE_NAME_LENGTH;
	long ValveSerieNameStat;
	char ValveSerieName[VALVE_SERIE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 4, ValveSerieNameLength, ValveSerieName, &ValveSerieNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ValveSerieName, "Unknown") != 0 && strcmp(ValveSerieName, "Not_Avaiable") != 0)	
			{
				strcpy(pValveSeries[CountValveSeries].ValveSerieName, ValveSerieName);
				CountValveSeries++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Controller Type Struktur mit den Controllernamen 
//  Input           : -
//  Output *Pointer : pControllerTypes: Controller Type Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteControllerTypeStructure(struct testControllerType_type *pControllerTypes) 
{
	UINT16 CountControllerTypes = 0, ControllerTypeNameLength = CONTROLLER_TYPE_NAME_LENGTH;
	long ControllerTypeNameStat;
	char ControllerTypeName[CONTROLLER_TYPE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 4, ControllerTypeNameLength, ControllerTypeName, &ControllerTypeNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(ControllerTypeName, "Unknown") != 0 && strcmp(ControllerTypeName, "Not_Avaiable") != 0)	
			{
				strcpy(pControllerTypes[CountControllerTypes].ControllerTypeName, ControllerTypeName);
				CountControllerTypes++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Option Struktur mit den Optionnamen 
//  Input           : -
//  Output *Pointer : pOptions: Option Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteOptionStructure(struct testOption_type *pOptions) 
{
	UINT16 CountOptions = 0, OptionNameLength = OPTION_NAME_LENGTH;
	long OptionNameStat;
	char OptionName[OPTION_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 3, OptionNameLength, OptionName, &OptionNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(OptionName, "Unknown") != 0 && strcmp(OptionName, "Not_Avaiable") != 0)	
			{
				strcpy(pOptions[CountOptions].OptionName, OptionName);
				CountOptions++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die NI Hardware Type Struktur mit den NIHardwarenamen 
//  Input           : -
//  Output *Pointer : pNIHardwares: NI Hardware Struktur
//         *return  : -
//******************************************************************************************************************************
void Database_WriteNIHardwareTypeStructure(struct testNIHardwareType_type *pNIHardwareTypes)
{
	UINT16 CountNIHardwareTypes = 0, NIHardwareTypeNameLength = MODUL_NAME_LENGTH;
	long NIHardwareTypeNameStat;
	char NIHardwareTypeName[MODUL_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 3, NIHardwareTypeNameLength, NIHardwareTypeName, &NIHardwareTypeNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(strcmp(NIHardwareTypeName, "Unknown") != 0 && strcmp(NIHardwareTypeName, "Not_Avaiable") != 0)	
			{
				strcpy(pNIHardwareTypes[CountNIHardwareTypes].NIHardwareTypeName, NIHardwareTypeName);
				CountNIHardwareTypes++;
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
}







//Info Feld
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Informationen
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pTestName: 				Testname
//										pTestDescription:			Beschreibung des Testes
//										pTestCreationDate:		Datum als der Test erstellt wurde 
//										pTestCreationAuthor:	Autor des Tests
//         *return  : 
//******************************************************************************************************************************
void Database_GetTestInformation(SINT32 TestVersionId, char *pTestName, char *pTestDescription, char *pTestCreationDate, char *pTestCreationAuthor)
{
	SINT32 TestVersionDatabaseId = -1, TestInformationDatabaseId, TestInformationId = -1;
	long TestVersionDatabaseIdIdStat, TestInformationIdStat, TestInformationDatabaseIdStat, TestNameStat, TestNameLength = TESTINFORMATION_NAME_LENGTH;
	long TestDescriptionLength = TESTINFOMATION_DESCRIPTION_LENGTH, TestDescriptionStat, TestCreationDateLength = DATE_TYPE_LENGTH, TestCreationDateStat;
	long TestCreationAuthorLength = TESTINFOMATION_CREATIONAUTHOR_LENGTH, TestCreationAuthorStat;
	
	//Zuerst muss die TestInformation Id heraus gefunden werden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestVersionDatabaseId, &TestVersionDatabaseIdIdStat);
		resCode = DBBindColInt  (hstmt, 7, &TestInformationId, &TestInformationIdStat);	
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			 if(TestVersionId == TestVersionDatabaseId)	break;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	//Nun mit Hilfe des TestInformationId den Testnamen zurück geben
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestInformationDatabaseId, &TestInformationDatabaseIdStat);
		resCode = DBBindColChar (hstmt, 2, TestNameLength, pTestName, &TestNameStat, "");
		resCode = DBBindColChar (hstmt, 3, TestDescriptionLength, pTestDescription, &TestDescriptionStat, "");
		resCode = DBBindColChar (hstmt, 4, TestCreationDateLength, pTestCreationDate, &TestCreationDateStat, "");
		resCode = DBBindColChar (hstmt, 5, TestCreationAuthorLength, pTestCreationAuthor, &TestCreationAuthorStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			 if(TestInformationId == TestInformationDatabaseId)	break;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Name
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pTestName: 				Testname
//         *return  : 
//******************************************************************************************************************************
void Database_GetTestName(SINT32 TestInformationId, char *pTestName)
{
	SINT32 TestInformationDatabaseId;
	long TestNameStat, TestNameLength = TESTINFORMATION_NAME_LENGTH, TestInformationDatabaseIdStat;
	
	//Nun mit Hilfe des TestInformationId den Testnamen zurück geben
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestInformationDatabaseId, &TestInformationDatabaseIdStat);
		resCode = DBBindColChar (hstmt, 2, TestNameLength, pTestName, &TestNameStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			 if(TestInformationId == TestInformationDatabaseId)	break;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Version Information
//  Input           : TestVersionId: 			Id der Test Version
//  Output *Pointer : pTestVersion: 			Test Version
//										pTestModification:	Beschreibung der Veränderung zur letzter Testversion
//										pTestDurating:			Dauer des Tests 
//										pTestModul:					Modul des Tests
//         *return  : 
//******************************************************************************************************************************
void Database_GetTestVersionInformation(SINT32 TestVersionId, SINT16 *pTestVersion, char *pTestModification, char *pTestDurating, char *pTestModul)
{
	SINT32 TestVersionDatabaseId, TestModulNameId = -1, TestModulNameDatabaseId;
	long TestVersionStat, TestVersionDatabaseIdIdStat, TestModificationLength = TESTVERSION_MODIFICATION_LENGTH, TestModificationStat, TestDuratingLength = TESTVERSION_DURATING_LENGTH;
	long TestDuratingStat, TestModulNameIdStat, TestModulNameLength = MODULNAMES_NAME_LENGTH, TestModulNameEnumStat;
	
	//Zuerst die Informationen aus der Test Version Tabelle auslesen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestVersionDatabaseId, &TestVersionDatabaseIdIdStat);
		resCode = DBBindColShort  (hstmt, 2, pTestVersion, &TestVersionStat);	
		resCode = DBBindColChar (hstmt, 3, TestModificationLength, pTestModification, &TestModificationStat, "");
		resCode = DBBindColChar (hstmt, 4, TestDuratingLength, pTestDurating, &TestDuratingStat, "hh:mm:ss");
		resCode = DBBindColInt (hstmt, 5, &TestModulNameId, &TestModulNameIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(TestVersionId == TestVersionDatabaseId)	break;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	
	//ModulName mit Hilfe der ModulNameId
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestModulNameDatabaseId, &TestModulNameIdStat);
		resCode = DBBindColChar (hstmt, 2, TestModulNameLength, pTestModul, &TestModulNameEnumStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			 if(TestModulNameId == TestModulNameDatabaseId)	break;
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert das geforderte Interface für die Ventil Hardware (es können auch mehrere Werte definiert werden)
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pInterface: 		Interface der Ventil Hardware Anforderung
//         *return  : 
//******************************************************************************************************************************
char* Database_GetInterfaceTestHardware(SINT32 TestVersionId)
{
	UINT8 CountInterfaceEntrys = 0, IndexList = 0;
	SINT32 TestVersionDatabaseId, InterfaceTypeId = -1, InterfaceTypeDatabaseId = -1, InterfaceTypesIndex;
	long TestVersionIdStat, InterfaceTypeIdStat, InterfaceNameLength = INTERFACETYPES_NAME_LENGTH, InterfaceNameStat;
	char InterfaceName[INTERFACETYPES_NAME_LENGTH], *pInterface = NULL;
	
	//Zuerst den InterfaceTypesId aus der Link Tabelle holen
	//1 Durchgang um Anzahl Interface Einträge heraus zu finden 
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt (hstmt, 2, &InterfaceTypeDatabaseId, &InterfaceTypeIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestVersionId == TestVersionDatabaseId)	
			{
				InterfaceTypeId = InterfaceTypeDatabaseId;
				CountInterfaceEntrys++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//2 Durchgang um die ID in die Liste eintragen zu können (bei mehreren Einträgen)
	if(CountInterfaceEntrys == 0)	return pInterface;
	SINT32 InterfaceIdList[CountInterfaceEntrys];
	if(CountInterfaceEntrys > 1)
	{
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
			resCode = DBBindColInt (hstmt, 2, &InterfaceTypeId, &InterfaceTypeIdStat);
		
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
			 	if(TestVersionId == TestVersionDatabaseId)
				{
					InterfaceIdList[IndexList]	= InterfaceTypeId;
					IndexList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	if(CountInterfaceEntrys > 0)
	{
		//Nun kann die Liste durchgegangen werden und die Interface Werte in den String eingefügt werden
		pInterface = malloc(sizeof(char)*CountInterfaceEntrys*INTERFACETYPES_NAME_LENGTH);
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 2, &InterfaceTypesIndex, &TestVersionIdStat);
			resCode = DBBindColChar (hstmt, 4, InterfaceNameLength, InterfaceName, &InterfaceNameStat, "");
			IndexList = 0;
			
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
				if(CountInterfaceEntrys > 1)
				{
				 	if(InterfaceTypesIndex == InterfaceIdList[IndexList])
					{
						if(IndexList != 0)	
						{
							strcat(pInterface, ", ");
							strcat(pInterface, InterfaceName);
						}
						else	strcpy(pInterface, InterfaceName);
						IndexList++;
						if(IndexList == CountInterfaceEntrys) break;
					}
				}
				else
				{
					if(InterfaceTypesIndex == InterfaceTypeId)
					{
						strcpy(pInterface, InterfaceName);
						break;
					}
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	return pInterface;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert das geforderte Ventilreihe für die Ventil Hardware (es können auch mehrere Werte definiert werden)
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pInterface: 		Ventilreihe der Ventil Hardware Anforderung
//         *return  : 
//******************************************************************************************************************************
char* Database_GetValveSerieTestHardware(SINT32 TestVersionId)
{
	UINT8 CountValveSerieEntrys = 0, IndexList = 0;
	SINT32 TestVersionDatabaseId, ValveSerieTypeId = -1, ValveSerieTypeDatabaseId = -1, ValveSerieTypesIndex;
	long TestVersionIdStat, ValveSerieTypeIdStat, ValveSerieNameLength = VALVESERIES_NAME_LENGTH, ValveSerieNameStat;
	char ValveSerieName[VALVESERIES_NAME_LENGTH], *pValveSerie = NULL;
	
	//Zuerst den ValveSerieTypesId aus der Link Tabelle holen
	//1 Durchgang um Anzahl ValveSerie Einträge heraus zu finden 
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt (hstmt, 2, &ValveSerieTypeDatabaseId, &ValveSerieTypeIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestVersionId == TestVersionDatabaseId)	
			{
				ValveSerieTypeId = ValveSerieTypeDatabaseId;
				CountValveSerieEntrys++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//2 Durchgang um die ID in die Liste eintragen zu können (bei mehreren Einträgen)
	SINT32 ValveSerieIdList[CountValveSerieEntrys];
	if(CountValveSerieEntrys > 1)
	{
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ValveSerieType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
			resCode = DBBindColInt (hstmt, 2, &ValveSerieTypeId, &ValveSerieTypeIdStat);
		
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
			 	if(TestVersionId == TestVersionDatabaseId)
				{
					ValveSerieIdList[IndexList]	= ValveSerieTypeId;
					IndexList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	if(CountValveSerieEntrys > 0)
	{
		//Nun kann die Liste durchgegangen werden und die ValveSerie Werte in den String eingefügt werden
		pValveSerie = malloc(sizeof(char)*CountValveSerieEntrys*VALVESERIES_NAME_LENGTH);
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 2, &ValveSerieTypesIndex, &TestVersionIdStat);
			resCode = DBBindColChar (hstmt, 4, ValveSerieNameLength, ValveSerieName, &ValveSerieNameStat, "");
			IndexList = 0;
			
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
				if(CountValveSerieEntrys > 1)
				{
				 	if(ValveSerieTypesIndex == ValveSerieIdList[IndexList])
					{
						if(IndexList != 0)	
						{
							strcat(pValveSerie, ValveSerieName);
							strcat(pValveSerie, ", ");
						}
						else	strcpy(pValveSerie, ValveSerieName);
						IndexList++;
						if(IndexList == CountValveSerieEntrys) break;
					}
				}
				else
				{
					if(ValveSerieTypesIndex == ValveSerieTypeId)
					{
						strcpy(pValveSerie, ValveSerieName);
						break;
					}
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	return pValveSerie;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert das geforderte Controller für die Ventil Hardware (es können auch mehrere Werte definiert werden)
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pInterface: 		Controller der Ventil Hardware Anforderung
//         *return  : 
//******************************************************************************************************************************
char* Database_GetControllerTypeTestHardware(SINT32 TestVersionId)
{
	UINT8 CountControllerTypeEntrys = 0, IndexList = 0;
	SINT32 TestVersionDatabaseId, ControllerTypeId = -1, ControllerTypeDatabaseId = -1, ControllerTypeTypesIndex;
	long TestVersionIdStat, ControllerTypeIdStat, ControllerTypeNameLength = CONTROLLERTYPES_NAME_LENGTH, ControllerTypeNameStat;
	char ControllerTypeName[CONTROLLERTYPES_NAME_LENGTH], *pControllerType = NULL;
	
	//Zuerst den ControllerTypeTypesId aus der Link Tabelle holen
	//1 Durchgang um Anzahl ControllerType Einträge heraus zu finden 
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt (hstmt, 2, &ControllerTypeDatabaseId, &ControllerTypeIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestVersionId == TestVersionDatabaseId)	
			{
				ControllerTypeId = ControllerTypeDatabaseId;
				CountControllerTypeEntrys++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//2 Durchgang um die ID in die Liste eintragen zu können (bei mehreren Einträgen)
	SINT32 ControllerTypeIdList[CountControllerTypeEntrys];
	if(CountControllerTypeEntrys > 1)
	{
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
			resCode = DBBindColInt (hstmt, 2, &ControllerTypeId, &ControllerTypeIdStat);
		
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
			 	if(TestVersionId == TestVersionDatabaseId)
				{
					ControllerTypeIdList[IndexList]	= ControllerTypeId;
					IndexList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	if(CountControllerTypeEntrys > 0)
	{
		//Nun kann die Liste durchgegangen werden und die ControllerType Werte in den String eingefügt werden
		pControllerType = malloc(sizeof(char)*CountControllerTypeEntrys*CONTROLLERTYPES_NAME_LENGTH);
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 2, &ControllerTypeTypesIndex, &TestVersionIdStat);
			resCode = DBBindColChar (hstmt, 4, ControllerTypeNameLength, ControllerTypeName, &ControllerTypeNameStat, "");
			IndexList = 0;
			
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
				if(CountControllerTypeEntrys > 1)
				{
				 	if(ControllerTypeTypesIndex == ControllerTypeIdList[IndexList])
					{
						if(IndexList != 0)	
						{
							strcat(pControllerType, ", ");
							strcat(pControllerType, ControllerTypeName);
						}
						else	strcpy(pControllerType, ControllerTypeName);
						IndexList++;
						if(IndexList == CountControllerTypeEntrys) break;
					}
				}
				else
				{
					if(ControllerTypeTypesIndex == ControllerTypeId)
					{
						strcpy(pControllerType, ControllerTypeName);
						break;
					}
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	return pControllerType;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert das geforderte Option für die Ventil Hardware (es können auch mehrere Werte definiert werden)
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pInterface: 		Option der Ventil Hardware Anforderung
//         *return  : 
//******************************************************************************************************************************
char* Database_GetOptionTypeTestHardware(SINT32 TestVersionId)
{
	UINT8 CountOptionEntrys = 0, IndexList = 0;
	SINT32 TestVersionDatabaseId, OptionTypeId = -1, OptionTypeDatabaseId = -1, OptionTypesIndex;
	long TestVersionIdStat, OptionTypeIdStat, OptionNameLength = OPTIONTYPES_NAME_LENGTH, OptionNameStat;
	char OptionName[OPTIONTYPES_NAME_LENGTH], *pOption = NULL;
	
	//Zuerst den OptionTypesId aus der Link Tabelle holen
	//1 Durchgang um Anzahl Option Einträge heraus zu finden 
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt (hstmt, 2, &OptionTypeDatabaseId, &OptionTypeIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestVersionId == TestVersionDatabaseId)	
			{
				OptionTypeId = OptionTypeDatabaseId;
				CountOptionEntrys++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//2 Durchgang um die ID in die Liste eintragen zu können (bei mehreren Einträgen)
	SINT32 OptionIdList[CountOptionEntrys];
	if(CountOptionEntrys > 1)
	{
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
			resCode = DBBindColInt (hstmt, 2, &OptionTypeId, &OptionTypeIdStat);
		
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
			 	if(TestVersionId == TestVersionDatabaseId)
				{
					OptionIdList[IndexList]	= OptionTypeId;
					IndexList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	if(CountOptionEntrys > 0)
	{
		//Nun kann die Liste durchgegangen werden und die Option Werte in den String eingefügt werden
		pOption = malloc(sizeof(char)*CountOptionEntrys*OPTIONTYPES_NAME_LENGTH);
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM SingleOptionType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 2, &OptionTypesIndex, &TestVersionIdStat);
			resCode = DBBindColChar (hstmt, 3, OptionNameLength, OptionName, &OptionNameStat, "");
			IndexList = 0;
			
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
				if(CountOptionEntrys > 1)
				{
				 	if(OptionTypesIndex == OptionIdList[IndexList])
					{
						if(IndexList != 0)	
						{
							strcat(pOption, ", ");
							strcat(pOption, OptionName);
						}
						else	strcpy(pOption, OptionName);
						IndexList++;
						if(IndexList == CountOptionEntrys) break;
					}
				}
				else
				{
					if(OptionTypesIndex == OptionTypeId)
					{
						strcpy(pOption, OptionName);
						break;
					}
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	return pOption;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert das geforderte Test Hardware für die Ventil Hardware (es können auch mehrere Werte definiert werden)
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : pInterface: 		Test Hardware der Ventil Hardware Anforderung
//         *return  : 
//******************************************************************************************************************************
char* Database_GetTestHardware(SINT32 TestVersionId)
{
	UINT8 CountTestHardwareEntrys = 0, IndexList = 0;
	SINT32 TestVersionDatabaseId, TestHardwareTypeId = -1, TestHardwareTypeDatabaseId = -1, TestHardwareTypesIndex;
	long TestVersionIdStat, TestHardwareTypeIdStat, TestHardwareNameLength = TESTHARDWARETYPES_NAME_LENGTH, TestHardwareNameStat;
	char TestHardwareName[TESTHARDWARETYPES_NAME_LENGTH], *pTestHardware = NULL;
	
	//Zuerst den TestHardwareTypesId aus der Link Tabelle holen
	//1 Durchgang um Anzahl TestHardware Einträge heraus zu finden 
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt (hstmt, 2, &TestHardwareTypeDatabaseId, &TestHardwareTypeIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(TestVersionId == TestVersionDatabaseId)	
			{
				TestHardwareTypeId = TestHardwareTypeDatabaseId;
				CountTestHardwareEntrys++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//2 Durchgang um die ID in die Liste eintragen zu können (bei mehreren Einträgen)
	SINT32 TestHardwareIdList[CountTestHardwareEntrys];
	if(CountTestHardwareEntrys > 1)
	{
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2TestHardwareType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
			resCode = DBBindColInt (hstmt, 2, &TestHardwareTypeId, &TestHardwareTypeIdStat);
		
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
			 	if(TestVersionId == TestVersionDatabaseId)
				{
					TestHardwareIdList[IndexList]	= TestHardwareTypeId;
					IndexList++;
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	if(CountTestHardwareEntrys > 0)
	{
		//Nun kann die Liste durchgegangen werden und die TestHardware Werte in den String eingefügt werden
		pTestHardware = malloc(sizeof(char)*CountTestHardwareEntrys*TESTHARDWARETYPES_NAME_LENGTH);
		
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestHardwareType");
		if (hstmt > 0)
		{
			resCode = DBBindColInt (hstmt, 2, &TestHardwareTypesIndex, &TestVersionIdStat);
			resCode = DBBindColChar (hstmt, 3, TestHardwareNameLength, TestHardwareName, &TestHardwareNameStat, "");
			IndexList = 0;
			
		 	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		 	{
				if(CountTestHardwareEntrys > 1)
				{
				 	if(TestHardwareTypesIndex == TestHardwareIdList[IndexList])
					{
						if(IndexList != 0)	
						{
							strcat(pTestHardware, ", ");
							strcat(pTestHardware, TestHardwareName);
						}
						else	strcpy(pTestHardware, TestHardwareName);
						IndexList++;
						if(IndexList == CountTestHardwareEntrys) break;
					}
				}
				else
				{
					if(TestHardwareTypesIndex == TestHardwareTypeId)
					{
						strcpy(pTestHardware, TestHardwareName);
						break;
					}
				}
			}
			resCode = DBDeactivateSQL (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	return pTestHardware;
}





//Selection Feld
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Modul Namen zurück
//  Input           : TestVersionId: 	Id der Test Version
//  Output *Pointer : -
//         *return  : Name des Moduls
//******************************************************************************************************************************
char* Database_GetModulName(SINT32 TestVersionId)
{
	SINT32 TestVersionDatabaseId = -1, ModulNameId = -1, ModulNameDatabaseId = -1;
	long TestVersionIdStat, ModulNameIdStat, ModulNameLength = MODULNAMES_NAME_LENGTH, ModulNameStat;
	char ModulName[MODULNAMES_NAME_LENGTH];
	
	//Zuerst die ModulNameId heraus holen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 5, &ModulNameId, &ModulNameIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Jetzt mit der Id den ModulNamen zurück geben
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ModulNameDatabaseId, &ModulNameIdStat);
		resCode = DBBindColChar(hstmt, 2, ModulNameLength, ModulName, &ModulNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ModulNameDatabaseId == ModulNameId)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ModulName;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Interface Types Id zurück
//  Input           : pInterface: 	Interface Name
//  Output *Pointer : -
//         *return  : Id des Interfaces Type
//******************************************************************************************************************************
SINT32 Database_GetInterfaceTypeIdWithEnum(char *pInterface)
{
	SINT32 InterfaceTypesId = -1;
	long InterfaceNameLength = INTERFACETYPES_NAME_LENGTH, InterfaceTypesIdStat, InterfaceNameStat;
	char InterfaceNameDatabase[INTERFACETYPES_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &InterfaceTypesId, &InterfaceTypesIdStat);
		resCode = DBBindColChar (hstmt, 4, InterfaceNameLength, InterfaceNameDatabase, &InterfaceNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(InterfaceNameDatabase, pInterface) == 0)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return InterfaceTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Interface Types Id zurück
//  Input           : InterfaceEnumValue: 	Interface Enum Wert
//  Output *Pointer : -
//         *return  : Id des Interfaces Type
//******************************************************************************************************************************
SINT32 Database_GetInterfaceTypeIdWithEnumValue(SINT32 InterfaceEnumValue)
{
	SINT32 InterfaceTypesId = -1, InterfaceEnumValueDatabase;
	long InterfaceTypesIdStat, InterfaceEnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &InterfaceTypesId, &InterfaceTypesIdStat);
		resCode = DBBindColInt(hstmt, 3, &InterfaceEnumValueDatabase, &InterfaceEnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (InterfaceEnumValueDatabase == InterfaceEnumValue)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return InterfaceTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Valve Serie Types Id zurück
//  Input           : pValveSerie: 	Valve Serie Name
//  Output *Pointer : -
//         *return  : Id des Valve Serie Type
//******************************************************************************************************************************
SINT32 Database_GetValveSerieIdWithEnum(char *pValveSerie)
{
	SINT32 ValveSerieTypesId = -1;
	long ValveSerieNameLength = VALVESERIES_NAME_LENGTH, ValveSerieTypesIdStat, ValveSerieNameStat;
	char ValveSerieNameDatabase[VALVESERIES_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ValveSerieTypesId, &ValveSerieTypesIdStat);
		resCode = DBBindColChar (hstmt, 4, ValveSerieNameLength, ValveSerieNameDatabase, &ValveSerieNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(ValveSerieNameDatabase, pValveSerie) == 0)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ValveSerieTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Valve Serie Types Id zurück
//  Input           : ValveSerieEnumValue: 	Valve Serie Enum Wert
//  Output *Pointer : -
//         *return  : Id des Valve Serie Type
//******************************************************************************************************************************
SINT32 Database_GetValveSerieIdWithEnumValue(SINT16 ValveSerieEnumValue)
{
	SINT16 ValveSerieEnumValueDatabase;
	SINT32 ValveSerieTypesId = -1;
	long ValveSerieTypesIdStat, ValveSerieEnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ValveSerieTypesId, &ValveSerieTypesIdStat);
		resCode = DBBindColShort(hstmt, 3, &ValveSerieEnumValueDatabase, &ValveSerieEnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ValveSerieEnumValueDatabase == ValveSerieEnumValue)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ValveSerieTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Controller Types Id zurück
//  Input           : pController: 	Controller Name
//  Output *Pointer : -
//         *return  : Id des Controller Type
//******************************************************************************************************************************
SINT32 Database_GetControllerTypeIdWithEnum(char *pController)
{
	SINT32 ControllerTypesId = -1;
	long ControllerNameLength = CONTROLLERTYPES_NAME_LENGTH, ControllerTypesIdStat, ControllerNameStat;
	char ControllerNameDatabase[CONTROLLERTYPES_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ControllerTypesId, &ControllerTypesIdStat);
		resCode = DBBindColChar (hstmt, 4, ControllerNameLength, ControllerNameDatabase, &ControllerNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(ControllerNameDatabase, pController) == 0)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ControllerTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Controller Types Id zurück
//  Input           : ControllerEnumValue: 	Controller Enum Wert
//  Output *Pointer : -
//         *return  : Id des Controller Type
//******************************************************************************************************************************
SINT32 Database_GetControllerTypeIdWithEnumValue(SINT32 ControllerEnumValue)
{
	SINT32 ControllerTypesId = -1, ControllerEnumValueDatabase;
	long ControllerTypesIdStat, ControllerEnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ControllerTypesId, &ControllerTypesIdStat);
		resCode = DBBindColInt(hstmt, 3, &ControllerEnumValueDatabase, &ControllerEnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ControllerEnumValueDatabase == ControllerEnumValue)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return ControllerTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Option Types Id zurück
//  Input           : pOption: 	Option Name
//  Output *Pointer : -
//         *return  : Id des Option Type
//******************************************************************************************************************************
SINT32 Database_GetSingleOptionTypeIdWithEnum(char *pOption)
{
	SINT32 OptionTypesId = -1;
	long OptionNameLength = OPTIONTYPES_NAME_LENGTH, OptionTypesIdStat, OptionNameStat;
	char OptionNameDatabase[OPTIONTYPES_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &OptionTypesId, &OptionTypesIdStat);
		resCode = DBBindColChar (hstmt, 3, OptionNameLength, OptionNameDatabase, &OptionNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(OptionNameDatabase, pOption) == 0)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return OptionTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert eine Liste der Option Types Id zurück, je nach Option Valve Value
//  Input           : OptionValveValue: 	Option Wert
//  Output *Pointer : pCountOptionEntrys: Anzahl Einträge in die Liste
//         *return  : Liste der Option Types Id
//******************************************************************************************************************************
SINT32* Database_GetOptionTypeIdWithValveValue(SINT16 OptionValveValue, UINT8 *pCountOptionEntrys)
{
	SINT32 *pOptionTypesIdList = NULL, OptionTypesId;
	
	//Zuerst muss heraus gefunden werden, wieviel Einträge die Option Valve Value unterstützt
	if(OptionValveValue == CH_SPS || OptionValveValue == CH_PFO || OptionValveValue == CH_CLUSTER || OptionValveValue == CH_SPFO) *pCountOptionEntrys = 1;
	else if(OptionValveValue == CH_SPS_PFO || OptionValveValue == CH_SPS_CLUSTER || OptionValveValue == CH_PFO_CLUSTER|| OptionValveValue == CH_SPS_SPFO || OptionValveValue == CH_SPFO_CLUSTER)	*pCountOptionEntrys = 2;
	else if(OptionValveValue == CH_SPS_PFO_CLUSTER || OptionValveValue == CH_SPS_SPFO_CLUSTER) *pCountOptionEntrys = 3;
	
	//Bei keinem Eintrag soll die Funtkion verlassen werden
	if(*pCountOptionEntrys == 0)	return pOptionTypesIdList;
	else
	{
		pOptionTypesIdList = malloc(sizeof(SINT32)*(*pCountOptionEntrys));
		//Nun sollen die Werte eingetragen werden
		switch(OptionValveValue)
		{
			case CH_SPS:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				break;
			}
			case CH_PFO:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("PFO");
				pOptionTypesIdList[0] = OptionTypesId;
				break;
			}
			case CH_CLUSTER:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[0] = OptionTypesId;
				break;
			}
			case CH_SPS_PFO:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("PFO");
				pOptionTypesIdList[1] = OptionTypesId;
				break;
			}
			case CH_SPS_CLUSTER:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[1] = OptionTypesId;
				break;
			}
			case CH_PFO_CLUSTER:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("PFO");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[1] = OptionTypesId;
				break;
			}
			case CH_SPS_PFO_CLUSTER:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("PFO");
				pOptionTypesIdList[1] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[2] = OptionTypesId;
				break;
			}
			case CH_SPFO:
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("sPFO");
				pOptionTypesIdList[0] = OptionTypesId;
				break;
			}
			case CH_SPS_SPFO:	
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("sPFO");
				pOptionTypesIdList[1] = OptionTypesId;
				break;
			}
			case CH_SPFO_CLUSTER:
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("sPFO");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[1] = OptionTypesId;
				break;
			}
			case CH_SPS_SPFO_CLUSTER:
			{
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("SPS");
				pOptionTypesIdList[0] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("sPFO");
				pOptionTypesIdList[1] = OptionTypesId;
				OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");
				pOptionTypesIdList[2] = OptionTypesId;
				break;
			}
		}
	}
	return pOptionTypesIdList;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert eine Liste der Test Hardware Types Id zurück, je nach Test Hardware Value
//  Input           : pNIHardware: 	NI Hardware Bitwert
//  Output *Pointer : pCountTestHardwareEntrys: Anzahl Einträge in der Liste
//         *return  : Liste der Test Hardware Types Id
//******************************************************************************************************************************
SINT32* Database_GetTestHardwareTypeIdWithValue(BOOL8 *pNIHardware, UINT8 *pCountTestHardwareEntrys)
{
	UINT8 i, IndexTestHardwareTypesList = 0;
	SINT32 *pTestHardwareTypesIdList = NULL, TestHardwareTypesId;
	
	*pCountTestHardwareEntrys = 0;
	for(i = 0; i < 16; i++)	if(pNIHardware[i] == TRUE) (*pCountTestHardwareEntrys)++;
	
	pTestHardwareTypesIdList = malloc(sizeof(SINT32)*(*pCountTestHardwareEntrys));
	
	if(pNIHardware[NIH_POWER_CONNECTOR] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("NI Power Connector");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	if(pNIHardware[NIH_INTERFACE_RS232] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("Interface RS232/RS485");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	if(pNIHardware[NIH_INTERFACE_LOGIC] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("Logic");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	if(pNIHardware[NIH_INTERFACE_DEVICENET] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("DeviceNet");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	if(pNIHardware[NIH_INTERFACE_ETHERCAT] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("EtherCAT");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	
	if(pNIHardware[NIH_ANALOG_SENSOR] == TRUE)
	{
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum("NI Analog Sensor");
		pTestHardwareTypesIdList[IndexTestHardwareTypesList] = TestHardwareTypesId;
		IndexTestHardwareTypesList++;
	}
	return pTestHardwareTypesIdList;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Hardware Types Id zurück
//  Input           : pNIHardware: 	Test Hardware Name
//  Output *Pointer : -
//         *return  : Id des Test Hardware Type
//******************************************************************************************************************************
SINT32 Database_GetNIHardwareTypeIdWithEnum(char *pNIHardware)
{
	SINT32 TestHardwareTypesId = -1;
	long TestHardwareNameLength = TESTHARDWARETYPES_NAME_LENGTH, TestHardwareTypesIdStat, TestHardwareNameStat;
	char TestHardwareNameDatabase[TESTHARDWARETYPES_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestHardwareTypesId, &TestHardwareTypesIdStat);
		resCode = DBBindColChar (hstmt, 3, TestHardwareNameLength, TestHardwareNameDatabase, &TestHardwareNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (strcmp(TestHardwareNameDatabase, pNIHardware) == 0)	break;
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return TestHardwareTypesId;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob das geforderte Interface vom Test unterstützt wird
//  Input           : TestVersionId: 				Id der Test Version
//										InterfaceTypesId:			Id des Interface Types
//										InterfaceTypesAllId		Id des Interface Types All
//  Output *Pointer : -
//         *return  : Resultat ob das vom Test definierte Interface auch von der Ventil Hardware unterstützt wird
//******************************************************************************************************************************
BOOL8 Database_IsTestInterfaceCompatibleWithTestHardware(SINT32 TestVersionId, SINT32 InterfaceTypesId, SINT32 InterfaceTypesAllId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, InterfaceTypesDatabaseId = -1;
	long InterfaceTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der InterfaceType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &InterfaceTypesDatabaseId, &InterfaceTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(InterfaceTypesDatabaseId == InterfaceTypesId || InterfaceTypesDatabaseId == InterfaceTypesAllId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob das geforderte ValveSerie vom Test unterstützt wird
//  Input           : TestVersionId: 				Id der Test Version
//										ValveSerieTypesId:		Id des ValveSerie Types
//										ValveSerieTypesAllId:	Id des ValveSerie Types All
//  Output *Pointer : -
//         *return  : Resultat ob das vom Test definierte ValveSerie auch von der Ventil Hardware unterstützt wird
//******************************************************************************************************************************
BOOL8 Database_IsTestValveSerieCompatibleWithTestHardware(SINT32 TestVersionId, SINT32 ValveSeriesId, SINT32 ValveSeriesAllId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, ValveSerieTypesDatabaseId = -1;
	long ValveSerieTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der ValveSerieType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ValveSerieType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &ValveSerieTypesDatabaseId, &ValveSerieTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(ValveSerieTypesDatabaseId == ValveSeriesId || ValveSerieTypesDatabaseId == ValveSeriesAllId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob das geforderte Controller vom Test unterstützt wird
//  Input           : TestVersionId: 				Id der Test Version
//										ControllerTypesId:			Id des Controller Types
//										ControllerTypesAllId		Id des Controller Types All
//  Output *Pointer : -
//         *return  : Resultat ob das vom Test definierte Controller auch von der Ventil Hardware unterstützt wird
//******************************************************************************************************************************
BOOL8 Database_IsTestControllerCompatibleWithTestHardware(SINT32 TestVersionId, SINT32 ControllerTypesId, SINT32 ControllerTypesAllId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, ControllerTypesDatabaseId = -1;
	long ControllerTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der ControllerType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &ControllerTypesDatabaseId, &ControllerTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(ControllerTypesDatabaseId == ControllerTypesId || ControllerTypesDatabaseId == ControllerTypesAllId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob das geforderte Option vom Test unterstützt wird
//  Input           : TestVersionId: 				Id der Test Version
//										OptionTypesId:			Id des Option Types
//										OptionTypesAllId		Id des Option Types All
//  Output *Pointer : -
//         *return  : Resultat ob das vom Test definierte Option auch von der Ventil Hardware unterstützt wird
//******************************************************************************************************************************
BOOL8 Database_IsTestOptionCompatibleWithTestHardware(SINT32 TestVersionId, SINT32 OptionTypesId, SINT32 OptionTypesAllId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, OptionTypesDatabaseId = -1;
	long OptionTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der OptionType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &OptionTypesDatabaseId, &OptionTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(OptionTypesDatabaseId == OptionTypesId || OptionTypesDatabaseId == OptionTypesAllId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob das geforderte TestHardware vom Test unterstützt wird
//  Input           : TestVersionId: 				Id der Test Version
//										TestHardwareTypesId:			Id des TestHardware Types
//										TestHardwareTypesAllId		Id des TestHardware Types All
//  Output *Pointer : -
//         *return  : Resultat ob das vom Test definierte TestHardware auch angeschlossen ist
//******************************************************************************************************************************
BOOL8 Database_IsTestNIHardwareCompatibleWithTestHardware(SINT32 TestVersionId, SINT32 TestHardwareTypesId, SINT32 TestHardwareTypesAllId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, TestHardwareTypesDatabaseId = -1;
	long TestHardwareTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der TestHardwareType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &TestHardwareTypesDatabaseId, &TestHardwareTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(TestHardwareTypesDatabaseId == TestHardwareTypesId || TestHardwareTypesDatabaseId == TestHardwareTypesAllId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}





//Hardware Type gebraucht
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die Test Hardware die geforderte Valve Serie vom Test unterstützt
//  Input           : TestVersionId: 				Id der Test Version
//										ValveSerieEnumValue:	Enum Wert Valve Serie
//  Output *Pointer : -
//         *return  : Resultat ob die Test Hardware die geforderte Valve Serie unterstützt
//******************************************************************************************************************************
BOOL8 Database_HasTestHardwareValveSerieNeededFromTest(SINT32 TestVersionId, SINT16 ValveSerieEnumValue)
{
	//Zuerst braucht man die ID des Enum Types
	BOOL8 Result = FALSE;
	SINT16 EnumValueAll = -1;
	SINT32 TestVersionDatabaseId, ValveSeriesDatabaseId = -1, ValveSeriesId = -1, ValveSeriesAllId = -1;
	long ValveSeriesIdStat, TestVersionIdStat;
	
	//Zuerst den Valve Series Id heraus lesen
	ValveSeriesId = Database_GetValveSerieIdWithEnumValue(ValveSerieEnumValue);
	ValveSeriesAllId = Database_GetValveSerieIdWithEnumValue(EnumValueAll);
	
	//Jetzt soll geschaut werden, ob der Valve Serie Eintrag vom Test unterstützt wird (Ausnahme der All Eintrag -> Bedingung erfüllt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &ValveSeriesDatabaseId, &ValveSeriesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				//Entweder den Eintrag All oder ansonsten muss der gleiche Eintrag sein, wie das vom Ventil ausgelesene
				if(ValveSeriesDatabaseId == ValveSeriesAllId)	
				{
					Result = TRUE;
					break;
				}
				if(ValveSeriesDatabaseId == ValveSeriesId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die Test Hardware die geforderte Controller Type vom Test unterstützt
//  Input           : TestVersionId: 				Id der Test Version
//										ControllerEnumValue:	Enum Wert Controller
//  Output *Pointer : -
//         *return  : Resultat ob die Test Hardware die geforderte Controller unterstützt
//******************************************************************************************************************************
BOOL8 Database_HasTestHardwareControllerNeededFromTest(SINT32 TestVersionId, SINT32 ControllerEnumValue)
{
	//Zuerst braucht man die ID des Enum Types
	BOOL8 Result = FALSE;
	SINT16 EnumValueAll = -1;
	SINT32 TestVersionDatabaseId, ControllerTypesDatabaseId = -1, ControllerTypesId = -1, ControllerTypesAllId = -1;
	long ControllerTypesIdStat, TestVersionIdStat;
	
	//Zuerst den Valve Series Id heraus lesen
	ControllerTypesId = Database_GetControllerTypeIdWithEnumValue(ControllerEnumValue);
	ControllerTypesAllId = Database_GetControllerTypeIdWithEnumValue(EnumValueAll);
	
	//Jetzt soll geschaut werden, ob der Valve Serie Eintrag vom Test unterstützt wird (Ausnahme der All Eintrag -> Bedingung erfüllt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &ControllerTypesDatabaseId, &ControllerTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				//Entweder den Eintrag All oder ansonsten muss der gleiche Eintrag sein, wie das vom Ventil ausgelesene
				if(ControllerTypesDatabaseId == ControllerTypesAllId)
				{
					Result = TRUE;
					break;
				}
				if(ControllerTypesDatabaseId == ControllerTypesId)	
				{
					Result = TRUE;
					break;
				}
			}
		}
		
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die Test Hardware die geforderte Interface Type vom Test unterstützt
//  Input           : TestVersionId: 				Id der Test Version
//										InterfaceEnumValue:		Enum Wert Controller
//  Output *Pointer : -
//         *return  : Resultat ob die Test Hardware die geforderte Interface unterstützt
//******************************************************************************************************************************
BOOL8 Database_HasTestHardwareInterfaceNeededFromTest(SINT32 TestVersionId, SINT32 InterfaceEnumValue)
{
	//Zuerst braucht man die ID des Enum Types
	BOOL8 Result = FALSE;
	SINT16 EnumValueAll = -1;
	SINT32 TestVersionDatabaseId, InterfaceTypesDatabaseId = -1, InterfaceTypesId = -1, InterfaceTypesAllId = -1;
	long InterfaceTypesIdStat, TestVersionIdStat;
	
	//Zuerst den Interface Id heraus lesen
	InterfaceTypesId = Database_GetInterfaceTypeIdWithEnumValue(InterfaceEnumValue);
	InterfaceTypesAllId = Database_GetInterfaceTypeIdWithEnumValue(EnumValueAll);
	
	//Jetzt soll geschaut werden, ob der Valve Serie Eintrag vom Test unterstützt wird (Ausnahme der All Eintrag -> Bedingung erfüllt)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &InterfaceTypesDatabaseId, &InterfaceTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				//Entweder den Eintrag All oder ansonsten muss der gleiche Eintrag sein, wie das vom Ventil ausgelesene
				if(InterfaceTypesDatabaseId == InterfaceTypesAllId)	
				{
					Result = TRUE;
					break;
				}
				if(InterfaceTypesDatabaseId == InterfaceTypesId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die Test Hardware die geforderte Option Type vom Test unterstützt
//  Input           : TestVersionId: 	Id der Test Version
//										OptionValue:		Option Wert des Ventils
//  Output *Pointer : -
//         *return  : Resultat ob die Test Hardware die geforderte Option unterstützt
//******************************************************************************************************************************
BOOL8 Database_HasTestHardwareOptionNeededFromTest(SINT32 TestVersionId, SINT16 OptionValue)
{
	BOOL8 Result = TRUE, Access = FALSE;
	UINT8 CountOptionEntrys = 0, i;
	SINT32 TestVersionDatabaseId, OptionTypesDatabaseId = -1, OptionTypesAllId = -1, *pOptionTypesId = NULL;
	long OptionTypesIdStat, TestVersionIdStat;
	
	//Zuerst den OptionValue in alle unterstützten OptionTypesId Liste umwandeln
	pOptionTypesId = Database_GetOptionTypeIdWithValveValue(OptionValue, &CountOptionEntrys);
	
	//Nun soll überprüft werden, ob alle benötigten Optionen vom Tests auch unterstützt werden
	OptionTypesAllId = Database_GetSingleOptionTypeIdWithEnum("All");
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &OptionTypesDatabaseId, &OptionTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				//Entweder den Eintrag All oder ansonsten muss der gleiche Eintrag sein, wie das vom Ventil ausgelesene
				if(OptionTypesDatabaseId == OptionTypesAllId)	break;
				for(i = 0; i < CountOptionEntrys; i++)
				{
					if(OptionTypesDatabaseId == pOptionTypesId[i]) Access = TRUE;
				}
				if(Access == FALSE)
				{
					Result = FALSE;
					break;
				}
				else	Access = FALSE;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ist die NI Hardware verfügbar, welche vom Test verlangt wird
//  Input           : TestVersionId: 			Id der Test Version
//										TestHardwareValue:	Wert der NI Hardware
//  Output *Pointer : -
//         *return  : Resultat ob die NI Hardware verfügbar ist, welche der Test fordert
//******************************************************************************************************************************
BOOL8 Database_IsNIHardwareAvaiableFromTest(SINT32 TestVersionId, SINT16 TestHardwareValue)
{
	BOOL8 Access = FALSE, Result = TRUE;
	UINT8 CountTestHardwareEntrys, i;
	BOOL8 NIHardware[16];
	SINT32 *pTestHardwareTypesIdList = NULL, TestHardwareTypesAllId = -1, TestVersionDatabaseId = -1, TestHardwareTypesDatabaseId = -1;
	long TestVersionIdStat, TestHardwareTypesIdStat;
	
	HelpFunctions_GetEnumIndex16BitArray(TestHardwareValue, NIHardware);
	
	//Zuerst den TestHardwareValue in alle unterstützten TestHardwareTypesId Liste umwandeln
	pTestHardwareTypesIdList = Database_GetTestHardwareTypeIdWithValue(NIHardware, &CountTestHardwareEntrys);
	
	//Nun soll überprüft werden, ob alle benötigte NI Hardware vorhanden ist, welche der Test vorgibt
	TestHardwareTypesAllId = Database_GetNIHardwareTypeIdWithEnum("All");
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2TestHardwareType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &TestHardwareTypesDatabaseId, &TestHardwareTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				//Entweder den Eintrag All oder ansonsten muss der gleiche Eintrag sein, wie das vom Ventil ausgelesene
				if(TestHardwareTypesDatabaseId == TestHardwareTypesAllId)	break;
				for(i = 0; i < CountTestHardwareEntrys; i++)
				{
					if(TestHardwareTypesDatabaseId == pTestHardwareTypesIdList[i]) Access = TRUE;
				}
				if(Access == FALSE)
				{
					Result = FALSE;
					break;
				}
				else	Access = FALSE;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob der Test die angeforderte Option braucht
//  Input           : TestVersionId: 				Id der Test Version
//										OptionTypesId:				Id des Option Types
//  Output *Pointer : -
//         *return  : Resultat ob die Option im Test gebraucht wird
//******************************************************************************************************************************
BOOL8 Database_IsOptionNeededFromTest(SINT32 TestVersionId, SINT32 OptionTypesId)
{
	BOOL8 Result = FALSE;
	SINT32 TestVersionDatabaseId, OptionTypesDatabaseId = -1;
	long OptionTypesIdStat, TestVersionIdStat;
	
	//Jetzt soll geschaut werden, ob der OptionType Eintrag mit der Test Version verknüpft wurde
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestVersionDatabaseId, &TestVersionIdStat);
		resCode = DBBindColInt(hstmt, 2, &OptionTypesDatabaseId, &OptionTypesIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestVersionDatabaseId == TestVersionId)	
			{
				if(OptionTypesDatabaseId == OptionTypesId)
				{
					Result = TRUE;
					break;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return Result;
}







//Initial State Ring List füllen
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Valve Firmware Selektionsfeld mit den Firmware Einträgen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteValveFirmwareInRingList(void)
{
	UINT8 Access = 0;
	UINT16 ValveFirmwareRingIndex = 0;
	long SystemNameStat, SoftwareNameStat, SoftwareNameLength = SOFTWARE_NAME_LENGTH, SystemNameLength = SYSTEM_NAME_LENGTH;
	char SoftwareName[SOFTWARE_NAME_LENGTH], SystemName[SYSTEM_NAME_LENGTH];

	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_FIRMWARE, ATTR_DATA_TYPE, VAL_STRING); 
	//trunk als einen Eintrag definieren
	if(Access == 0)
	{
		InsertListItem(STpanel, STPANEL_RING_ISV_FIRMWARE, 0, TRUNK_SOFTWARE_NAME, TRUNK_SOFTWARE_NAME);
		ValveFirmwareRingIndex++;
		Access++;
	}
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions ORDER BY Software");
	if (hstmt > 0)
	{
		resCode = DBBindColChar(hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColChar(hstmt, 4, SystemNameLength, SystemName, &SystemNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			//es muss sich um eine Ventil Firmware handeln
			if (strcmp(SystemName, "IC2") == 0)	
			{
				InsertListItem(STpanel, STPANEL_RING_ISV_FIRMWARE, ValveFirmwareRingIndex, SoftwareName, SoftwareName);
				ValveFirmwareRingIndex++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Motion Controller Firmware Selektionsfeld mit den Firmware Einträgen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteMotionControllerFirmwareInRingList(void)
{
	UINT8 Access = 0;
	UINT16 MotionControllerFirmwareRingIndex = 0;
	long SystemNameStat, SoftwareNameStat, SoftwareNameLength = SOFTWARE_NAME_LENGTH, SystemNameLength = SYSTEM_NAME_LENGTH;
	char SoftwareName[SOFTWARE_NAME_LENGTH], SystemName[SYSTEM_NAME_LENGTH], LastSoftwareName[SOFTWARE_NAME_LENGTH] = "";
	
	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, ATTR_DATA_TYPE, VAL_STRING);
	//Wenn der Eintrag nicht in der Datenbank vorhanden ist -> undef
	if(Access == 0)
	{
		InsertListItem(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, 0, UNDEFINED_SOFTWARE_NAME, UNDEFINED_SOFTWARE_NAME);
		MotionControllerFirmwareRingIndex++;
		Access++;
	}
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions ORDER BY Software");
	if (hstmt > 0)
	{
		resCode = DBBindColChar(hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColChar(hstmt, 4, SystemNameLength, SystemName, &SystemNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			//es muss sich um eine Ventil Firmware handeln
			if (strcmp(SystemName, "Motion Controller") == 0)	
			{
				//Zuerst den Namen Axis wegschneiden
				if(strstr(SoftwareName, "Axis1") != NULL)	HelpFunctions_StringReplace(" Axis1", "", SoftwareName);
				if(strstr(SoftwareName, "Axis2") != NULL)	HelpFunctions_StringReplace(" Axis2", "", SoftwareName);
				if(strcmp(LastSoftwareName, SoftwareName) != 0)	
				{
					InsertListItem(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, MotionControllerFirmwareRingIndex, SoftwareName, SoftwareName);
					strcpy(LastSoftwareName, SoftwareName);
					MotionControllerFirmwareRingIndex++;
				}
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Interface Firmware Selektionsfeld mit den Firmware Einträgen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteInterfaceFirmwareInRingList(void)
{
	UINT8 Access = 0;
	UINT16 InterfaceFirmwareRingIndex = 0;
	long SystemNameStat, SoftwareNameStat, SoftwareNameLength = SOFTWARE_NAME_LENGTH, SystemNameLength = SYSTEM_NAME_LENGTH;
	char SoftwareName[SOFTWARE_NAME_LENGTH], SystemName[SYSTEM_NAME_LENGTH];
	
	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, ATTR_DATA_TYPE, VAL_STRING);
	//- leeren Eintrag definieren + undef wenn das Ventil ein Feldbus Interface hat, welches nicht eingetragen ist
	if(Access == 0)
	{
		InsertListItem(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, 0, UNDEFINED_SOFTWARE_NAME, UNDEFINED_SOFTWARE_NAME);
		InterfaceFirmwareRingIndex++;
		Access++;
	}
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM SoftwareVersions ORDER BY Software");
	if (hstmt > 0)
	{
		resCode = DBBindColChar(hstmt, 2, SoftwareNameLength, SoftwareName, &SoftwareNameStat, "");
		resCode = DBBindColChar(hstmt, 4, SystemNameLength, SystemName, &SystemNameStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			//es muss sich um eine Ventil Firmware handeln
			if (strstr(SystemName, "netX") != NULL)	
			{
				InsertListItem(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, InterfaceFirmwareRingIndex, SoftwareName, SoftwareName);
				InterfaceFirmwareRingIndex++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Drive Parameter File Selektionsfeld mit den Drive Parameter Einträgen, wenn dieser aktiv ist
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteDriveParameterFileInRingList()
{
	UINT8 Access = 0;
	UINT16 DriveParameterFileRingIndex = 0;
	SINT16 ActivityState;
	long DriveParameterFileNameStat, DriveParameterFileLength = DRIVE_PARAMETER_FILE_NAME_LENGTH, ActivityStateStat;
	char DriveParameterFileName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_DRIVE_PAR, ATTR_DATA_TYPE, VAL_STRING);
	//- leeren Eintrag definieren
	if(Access == 0)
	{
		DeleteListItem(STpanel, STPANEL_RING_ISV_DRIVE_PAR, 0, -1);
		InsertListItem(STpanel, STPANEL_RING_ISV_DRIVE_PAR, 0, FILE_NOT_AVAIABLE, FILE_NOT_AVAIABLE);
		DriveParameterFileRingIndex++;
		Access++;
	}
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM DriveParameterFile ORDER BY Name");
	if (hstmt > 0)
	{
		resCode = DBBindColChar(hstmt, 2, DriveParameterFileLength, DriveParameterFileName, &DriveParameterFileNameStat, "");
		resCode = DBBindColShort  (hstmt, 3, &ActivityState, &ActivityStateStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if((ActivityState == ACTIVITY_STATE_ACTIVE || IsShowAllDriveParameterFilesSelected == TRUE) && (strcmp(DriveParameterFileName, FILE_NOT_AVAIABLE) != 0))
			{
				InsertListItem(STpanel, STPANEL_RING_ISV_DRIVE_PAR, DriveParameterFileRingIndex, DriveParameterFileName, DriveParameterFileName);
				DriveParameterFileRingIndex++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Configuration File Selektionsfeld mit den Configuration Einträgen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteConfigurationFileInRingList(void)
{
	UINT8 Access = 0;
	UINT16 ConfigurationFileRingIndex = 0;
	SINT16 ActivityState;
	long ConfigurationFileNameStat, ConfigurationFileLength = CONFIGURATION_FILE_NAME_LENGTH, ActivityStateStat;
	char ConfigurationFileName[CONFIGURATION_FILE_NAME_LENGTH];
	
	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_CONFIG_PAR, ATTR_DATA_TYPE, VAL_STRING);
	//- leeren Eintrag definieren
	if(Access == 0)
	{
		DeleteListItem(STpanel, STPANEL_RING_ISV_CONFIG_PAR, 0, -1);
		InsertListItem(STpanel, STPANEL_RING_ISV_CONFIG_PAR, 0, FILE_NOT_AVAIABLE, FILE_NOT_AVAIABLE);
		ConfigurationFileRingIndex++;
		Access++;
	}
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ConfigurationParameterFile ORDER BY Name");
	if (hstmt > 0)
	{
		resCode = DBBindColChar(hstmt, 2, ConfigurationFileLength, ConfigurationFileName, &ConfigurationFileNameStat, "");
		resCode = DBBindColShort  (hstmt, 3, &ActivityState, &ActivityStateStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if((ActivityState == ACTIVITY_STATE_ACTIVE || IsShowAllConfigurationParameterFilesSelected == TRUE) && (strcmp(ConfigurationFileName, FILE_NOT_AVAIABLE) != 0))
			{
				InsertListItem(STpanel, STPANEL_RING_ISV_CONFIG_PAR, ConfigurationFileRingIndex, ConfigurationFileName, ConfigurationFileName);
				ConfigurationFileRingIndex++;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die Test Collection Selektionsfeld mit den Test Collection Einträgen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestCollectionNameInRingList(void)
{
	UINT8 Access = 0;
	UINT16 i;
	char TestCollectionNameWithDate[TESTCOLLECTION_NAME_LENGTH+DATE_TYPE_LENGTH];
	
	Database_ConnectDatabase();	
	
	//löscht alle Elemente in der Testkollektion
	ClearListCtrl(STpanel, STPANEL_RING_ISV_TEST_COL);
	
	SetCtrlAttribute(STpanel, STPANEL_RING_ISV_TEST_COL, ATTR_DATA_TYPE, VAL_STRING);
	//"" leeren Eintrag definieren
	if(Access == 0)
	{
		DeleteListItem(STpanel, STPANEL_RING_ISV_TEST_COL, 0, -1);
		InsertListItem(STpanel, STPANEL_RING_ISV_TEST_COL, -1, "", "");
		Access++;
	}
	
	if(CountTestCollections > 0)
	{
		for(i = 0; i < CountTestCollections; i++)
		{
			//Datum noch zusätzlich dem Test Kollektion Namen schreiben
			if(IsShowAllTestCollectionSelected == TRUE)
			{
				//Datum aus Test Kollektion auslesen
				sprintf(TestCollectionNameWithDate, "%s ", pTestCollections[i].TestCollectionName);
				strcat(TestCollectionNameWithDate, pTestCollections[i].CreationDate);
				if(pTestCollections[i].ActivityState == ACTIVITY_STATE_NOT_ACTIVE)	strcat(TestCollectionNameWithDate, " Not Active");
				else																																strcat(TestCollectionNameWithDate, " Active");
				InsertListItem (STpanel, STPANEL_RING_ISV_TEST_COL, -1, TestCollectionNameWithDate, TestCollectionNameWithDate);
			}
			//-1 bedeutet ans Ende anfügen
			//Sollen nur Test angezeigt werden, bei welchem der Activity State auf TRUE steht
			else if(pTestCollections[i].ActivityState == ACTIVITY_STATE_ACTIVE)	InsertListItem (STpanel, STPANEL_RING_ISV_TEST_COL, -1, pTestCollections[i].TestCollectionName, pTestCollections[i].TestCollectionName);
		}
	}
}








//Testresultate
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt einen InitialStateFirmware Eintrag, wenn dieser noch nicht existiert
//  Input           : -
//  Output *Pointer : -
//         *return  : Gibt die Id des erstellten InitialStateFirmware Eintrag zurück
//******************************************************************************************************************************
SINT32 Database_WriteInitialStateFirmwareTable(char *pValveFirmwareReleaseTime)
{
	SINT32 InitialStateValveId, SoftwareVersionsIdFirmware, SoftwareVersionsIdMotionController, SoftwareVersionsIdInterface, DriveParameterFileId; 
	SINT32 ConfigurationParameterFileId, TestCollectionId, SoftwareVersionsDatabaseIdFirmware, SoftwareVersionsDatabaseIdMotionController;
	SINT32 SoftwareVersionsDatabaseIdInterface, DriveParameterFileDatabaseId, ConfigurationParameterFileDatabaseId, TestCollectionDatabaseId;
	int NumberSelectedFile;
	long InitialStateValveIdStat, SoftwareVersionsDatabaseIdFirmwareStat, FirmwareReleaseTimeLength = DATE_TIME_TYPE_LENGTH, FirmwareReleaseTimeDatabaseStat;
	long SoftwareVersionsDatabaseIdMotionControllerStat, SoftwareVersionsDatabaseIdInterfaceStat, DriveParameterFileDatabaseIdStat, ConfigurationParameterFileDatabaseIdStat;
	long TestCollectionDatabaseIdStat;
	char ValveFirmwareVersion[TESTINFORMATION_NAME_LENGTH], MotionControllerFirmware[TESTINFORMATION_NAME_LENGTH], InterfaceFirmware[TESTINFORMATION_NAME_LENGTH]; 
	char DriveParameterFile[DRIVE_PARAMETER_FILE_NAME_LENGTH], ConfigurationParameterFile[CONFIGURATION_FILE_NAME_LENGTH], TestCollectionName[TESTCOLLECTION_NAME_LENGTH];
	char FirmwareReleaseTimeDatabase[DATE_TIME_TYPE_LENGTH];
	
	//Valve Firmware Id mit Angabe des Namens
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, NumberSelectedFile, ValveFirmwareVersion);
	SoftwareVersionsIdFirmware = Database_GetFirmwareId(ValveFirmwareVersion);
	
	//Motion Controller Firmware Id mit Angabe des Namens 
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, NumberSelectedFile, MotionControllerFirmware);
	SoftwareVersionsIdMotionController = Database_GetFirmwareId(MotionControllerFirmware);
	
	//Interface Firmware Id mit Angabe des Namens 
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, NumberSelectedFile, InterfaceFirmware);
	SoftwareVersionsIdInterface = Database_GetFirmwareId(InterfaceFirmware);
	
	//Drive Parameter File Id mit Angabe des Namens
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, NumberSelectedFile, DriveParameterFile);
	DriveParameterFileId = Database_GetDriveParameterFileId(DriveParameterFile);
	
	//Configuration Parameter File Id mit Angabe des Namens
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, NumberSelectedFile, ConfigurationParameterFile);
	ConfigurationParameterFileId = Database_GetConfigurationParameterFileId(ConfigurationParameterFile);
	
	//Test Collection Id mit Angabe des Namens 
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_TEST_COL, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_TEST_COL, NumberSelectedFile, TestCollectionName);
	//Wenn alle Test Collection angezeigt werden, so muss aus dem Namen das Datum sowie der Activity State Wert entfernt werden
	if(IsShowAllTestCollectionSelected == TRUE)	 HelpFunctions_GetTestCollectionNameWithoutDateAndStateInfo(TestCollectionName, NumberSelectedFile-1);
	TestCollectionId = Database_GetTestCollectionId(TestCollectionName, pTestCollections[NumberSelectedFile-1]);
	
	//Prüfen ob es den Eintrag schon gibt
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InitialStateFirmware");
	if (hstmt > 0)
	{										
		resCode = DBBindColInt(hstmt, 1, &InitialStateValveId, &InitialStateValveIdStat);
		resCode = DBBindColInt(hstmt, 2, &SoftwareVersionsDatabaseIdFirmware, &SoftwareVersionsDatabaseIdFirmwareStat);
		resCode = DBBindColChar (hstmt, 3, FirmwareReleaseTimeLength, FirmwareReleaseTimeDatabase, &FirmwareReleaseTimeDatabaseStat, "yyyy-mm-dd hh:mm:ss");
		resCode = DBBindColInt(hstmt, 4, &SoftwareVersionsDatabaseIdMotionController, &SoftwareVersionsDatabaseIdMotionControllerStat);
		resCode = DBBindColInt(hstmt, 5, &SoftwareVersionsDatabaseIdInterface, &SoftwareVersionsDatabaseIdInterfaceStat);
		resCode = DBBindColInt(hstmt, 6, &DriveParameterFileDatabaseId, &DriveParameterFileDatabaseIdStat);
		resCode = DBBindColInt(hstmt, 7, &ConfigurationParameterFileDatabaseId, &ConfigurationParameterFileDatabaseIdStat);
		resCode = DBBindColInt(hstmt, 8, &TestCollectionDatabaseId, &TestCollectionDatabaseIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (SoftwareVersionsDatabaseIdFirmware == SoftwareVersionsIdFirmware && strcmp(FirmwareReleaseTimeDatabase, pValveFirmwareReleaseTime) == 0 && SoftwareVersionsDatabaseIdMotionController == SoftwareVersionsIdMotionController && SoftwareVersionsDatabaseIdInterface == SoftwareVersionsIdInterface && DriveParameterFileDatabaseId == DriveParameterFileId && ConfigurationParameterFileDatabaseId == ConfigurationParameterFileId && TestCollectionDatabaseId == TestCollectionId)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InitialStateValveId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Kein Eintrag gefunden -> neuer erstellen
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToInt (hmap, "SoftwareVersions_Id_Firmware", &SoftwareVersionsDatabaseIdFirmware, &SoftwareVersionsDatabaseIdFirmwareStat);
	resCode = DBMapColumnToChar (hmap, "ValveFirmwareReleaseTime", FirmwareReleaseTimeLength, FirmwareReleaseTimeDatabase, &FirmwareReleaseTimeDatabaseStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToInt (hmap, "SoftwareVersions_Id_MotionController", &SoftwareVersionsDatabaseIdMotionController, &SoftwareVersionsDatabaseIdMotionControllerStat);
	resCode = DBMapColumnToInt (hmap, "SoftwareVersions_Id_Interface", &SoftwareVersionsDatabaseIdInterface, &SoftwareVersionsDatabaseIdInterfaceStat);
	resCode = DBMapColumnToInt (hmap, "DriveParameterFile_ID", &DriveParameterFileDatabaseId, &DriveParameterFileDatabaseIdStat);
	resCode = DBMapColumnToInt (hmap, "ConfigurationParameterFile_ID", &ConfigurationParameterFileDatabaseId, &ConfigurationParameterFileDatabaseIdStat);
	resCode = DBMapColumnToInt (hmap, "TestCollection_Id", &TestCollectionDatabaseId, &TestCollectionDatabaseIdStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "InitialStateFirmware");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	SoftwareVersionsDatabaseIdFirmware = SoftwareVersionsIdFirmware;
	strcpy(FirmwareReleaseTimeDatabase, pValveFirmwareReleaseTime);
	SoftwareVersionsDatabaseIdMotionController = SoftwareVersionsIdMotionController; 
	SoftwareVersionsDatabaseIdInterface = SoftwareVersionsIdInterface; 
	DriveParameterFileDatabaseId = DriveParameterFileId; 
	ConfigurationParameterFileDatabaseId = ConfigurationParameterFileId; 
	TestCollectionDatabaseId = TestCollectionId; 
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
	
	//Neu erzeugten Eintrag herauslesen, um ID zu bekommen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InitialStateFirmware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &InitialStateValveId, &InitialStateValveIdStat);
		resCode = DBBindColInt(hstmt, 2, &SoftwareVersionsDatabaseIdFirmware, &SoftwareVersionsDatabaseIdFirmwareStat);
		resCode = DBBindColChar (hstmt, 3, FirmwareReleaseTimeLength, FirmwareReleaseTimeDatabase, &FirmwareReleaseTimeDatabaseStat, "yyyy-mm-dd hh:mm:ss");
		resCode = DBBindColInt(hstmt, 4, &SoftwareVersionsDatabaseIdMotionController, &SoftwareVersionsDatabaseIdMotionControllerStat);
		resCode = DBBindColInt(hstmt, 5, &SoftwareVersionsDatabaseIdInterface, &SoftwareVersionsDatabaseIdInterfaceStat);
		resCode = DBBindColInt(hstmt, 6, &DriveParameterFileDatabaseId, &DriveParameterFileDatabaseIdStat);
		resCode = DBBindColInt(hstmt, 7, &ConfigurationParameterFileDatabaseId, &ConfigurationParameterFileDatabaseIdStat);
		resCode = DBBindColInt(hstmt, 8, &TestCollectionDatabaseId, &TestCollectionDatabaseIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (SoftwareVersionsDatabaseIdFirmware == SoftwareVersionsIdFirmware && strcmp(FirmwareReleaseTimeDatabase, pValveFirmwareReleaseTime) == 0 && SoftwareVersionsDatabaseIdMotionController == SoftwareVersionsIdMotionController && SoftwareVersionsDatabaseIdInterface == SoftwareVersionsIdInterface && DriveParameterFileDatabaseId == DriveParameterFileId && ConfigurationParameterFileDatabaseId == ConfigurationParameterFileId && TestCollectionDatabaseId == TestCollectionId)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InitialStateValveId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	MessagePopup("Database Error","Error to write InitialStateFirmware entry");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt einen ValveHardware Eintrag, wenn dieser noch nicht existiert
//  Input           : -
//  Output *Pointer : -
//         *return  : Gibt die Id des erstellten InitialStateFirmware Eintrag zurück
//******************************************************************************************************************************
SINT32 Database_WriteValveHardwareTable(void)
{
#ifdef PINLIFTER
	UINT8 Module1TypeState, Module2TypeState, Module3TypeState, Module4TypeState;
#endif
	SINT16 OptionTypeValue, ExternalIsolationValve, ControllerHardwareVersionValue, InterfaceHardwareVersionValue, ControllerAssemblyVariantValue;
	SINT16 InterfaceAssemblyVariantValue, Module1TypeValue = 0, Module1HardwareVersionValue = 0, Module1AssemblyVariantValue = 0;
	SINT16 Module2TypeValue = 0, Module2HardwareVersionValue = 0, Module2AssemblyVariantValue = 0, Module3TypeValue = 0, Module3HardwareVersionValue = 0; 
	SINT16 Module3AssemblyVariantValue = 0, Module4TypeValue = 0, Module4HardwareVersionValue = 0, Module4AssemblyVariantValue = 0, ExternalIsolationValveDatabase;
	SINT32 ValveSerieId, ValveSerieValue, ValveHardwareId, InterfaceTypeId, InterfaceTypeValue, ControllerTypeValue, ControllerTypeId, OptionTypeId;
	SINT32 ControllerHardwareVersionId, InterfaceHardwareVersionId, ControllerAssemblyVariantId, InterfaceAssemblyVariantId, Module1TypeId, Module1HardwareVersionId;
	SINT32 Module1AssemblyVariantId, Module2TypeId, Module2HardwareVersionId, Module2AssemblyVariantId, Module3TypeId, Module3HardwareVersionId; 
	SINT32 Module3AssemblyVariantId, Module4TypeId, Module4HardwareVersionId, Module4AssemblyVariantId, ValveSerieDatabaseId, InterfaceTypeDatabaseId;
	SINT32 ControllerTypeDatabaseId, OptionTypeDatabaseId, ControllerHardwareVersionDatabaseId, InterfaceHardwareVersionDatabaseId, ControllerAssemblyVariantDatabaseId;
	SINT32 InterfaceAssemblyVariantDatabaseId, Module1TypeDatabaseId, Module1HardwareVersionDatabaseId, Module1AssemblyVariantDatabaseId, Module2TypeDatabaseId;
	SINT32 Module2HardwareVersionDatabaseId, Module2AssemblyVariantDatabaseId, Module3TypeDatabaseId, Module3HardwareVersionDatabaseId, Module3AssemblyVariantDatabaseId;
	SINT32 Module4TypeDatabaseId, Module4HardwareVersionDatabaseId, Module4AssemblyVariantDatabaseId;
	long ValveSerieIdStat, InterfaceTypeIdStat, ControllerTypeIdStat, OptionTypeIdStat, ExternalIsolationValveStat, ControllerHardwareVersionIdStat;
	long InterfaceHardwareVersionIdStat, ControllerAssemblyVariantIdStat, InterfaceAssemblyVariantIdStat, Module1TypeIdStat, Module1HardwareVersionIdStat;
	long Module1AssemblyVariantIdStat, Module2TypeIdStat, Module2HardwareVersionIdStat, Module2AssemblyVariantIdStat, Module3TypeIdStat, Module3HardwareVersionIdStat;
	long Module3AssemblyVariantIdStat, Module4TypeIdStat, Module4HardwareVersionIdStat, Module4AssemblyVariantIdStat, ValveHardwareIdStat;
	
	//Zuerst die Ventil Hardware auslesen
	//Valve Serie
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Configuration.ValveSeries, "");
	ValveSerieValue = (SINT32)Parameter.System.Identification.Configuration.ValveSeries.Value;
	ValveSerieId = GetValveSerieId(ValveSerieValue);
	
	//Interface Type
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceType, "");
	InterfaceTypeValue = (SINT32)Parameter.System.Identification.Hardware.InterfaceType.Value;
	InterfaceTypeId = GetInterfaceTypeId(InterfaceTypeValue);
	
	//Controller Type
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "");
	ControllerTypeValue = (SINT32)Parameter.System.Identification.Hardware.ControllerType.Value;
	ControllerTypeId = GetControllerTypeId(ControllerTypeValue);
	
	//Option Type
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.OptionType, "");
	OptionTypeValue = (SINT16)Parameter.System.Identification.Hardware.OptionType.Value;
	OptionTypeId = GetOptionTypeId(OptionTypeValue);
	
	//External Isolation Valve
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ExternalIsolationValveOption, "");
	ExternalIsolationValve = (SINT16)Parameter.System.Identification.Hardware.ExternalIsolationValveOption.Value;
	
	//ControllerHardwareVersion
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerHardwareVersion, "");
	ControllerHardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.ControllerHardwareVersion.Value;
	ControllerHardwareVersionId = GetControllerHardwareVersionId(ControllerHardwareVersionValue);
	
	//InterfaceHardwareVersion
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceHardwareVersion, "");
	InterfaceHardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.InterfaceHardwareVersion.Value;
	InterfaceHardwareVersionId = GetInterfaceHardwareVersionId(InterfaceHardwareVersionValue);
	
	//ControllerAssemblyVariant
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerAssemblyVariant, "");
	ControllerAssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.ControllerAssemblyVariant.Value;
	ControllerAssemblyVariantId = GetControllerAssemblyVariantId(ControllerAssemblyVariantValue);
	
	//InterfaceAssemblyVariant
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceAssemblyVariant, "");
	InterfaceAssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.InterfaceAssemblyVariant.Value;
	InterfaceAssemblyVariantId = GetInterfaceAssemblyVariantId(InterfaceAssemblyVariantValue);
	
	//Bei den Modul Informationen zuerst den Type abfragen ob er aktiv ist und zusätzlich muss das Enum nicht auf Not_Avaiable gesetzt sein
	//Wenn Hardware nicht vorhanden wird der Wert Not_Avaiable genommen, dieser Wert wird am Anfang als Standard gesetzt (gilt für alle 4 Module)
	//Gibt es momentan nur für Pinlifter -> nur hier werden die Parameter unterstützt
#ifdef PINLIFTER 
	//Module 1
	ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.Module1Type.ID, 0, &Module1TypeState, DT_UINT8, PARAMETER_GET_STATE);
	if(Module1TypeState == STATE_ACTIVE)
	{
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module1Type, "");
		if(Parameter.System.Identification.Hardware.Module1Type.Value != VH_NOT_AVAIABLE)
		{
			//Module 1 Type
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module1Type "");
			Module1TypeValue = (SINT16)Parameter.System.Identification.Hardware.Module1Type.Value;
			//Module 1 Hardware Version
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module1HardwareVersion "");
			Module1HardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.Module1HardwareVersion.Value;
			//Module 1 Assembly Variant
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module1AssemblyVariant "");
			Module1AssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.Module1AssemblyVariant.Value;
		}
		//Module 2
		ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.Module2Type.ID, 0, &Module2TypeState, DT_UINT8, PARAMETER_GET_STATE);
		if(Module2TypeState == STATE_ACTIVE)
		{
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module2Type, "");
			if(Parameter.System.Identification.Hardware.Module2Type.Value != VH_NOT_AVAIABLE)
			{
				//Module 2 Type
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module2Type "");
				Module2TypeValue = (SINT16)Parameter.System.Identification.Hardware.Module2Type.Value;
				//Module 2 Hardware Version
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module2HardwareVersion "");
				Module2HardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.Module2HardwareVersion.Value;
				//Module 2 Assembly Variant
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module2AssemblyVariant "");
				Module2AssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.Module2AssemblyVariant.Value;
			}
		}
		//Module 3
		ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.Module3Type.ID, 0, &Module3TypeState, DT_UINT8, PARAMETER_GET_STATE);
		if(Module3TypeState == STATE_ACTIVE)
		{
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module3Type, "");
			if(Parameter.System.Identification.Hardware.Module3Type.Value != VH_NOT_AVAIABLE)
			{
				//Module 3 Type
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module3Type "");
				Module3TypeValue = (SINT16)Parameter.System.Identification.Hardware.Module3Type.Value;
				//Module 3 Hardware Version
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module3HardwareVersion "");
				Module3HardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.Module3HardwareVersion.Value;
				//Module 3 Assembly Variant
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module3AssemblyVariant "");
				Module3AssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.Module3AssemblyVariant.Value;
			}
		}
		//Module 4
		ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.Module4Type.ID, 0, &Module4TypeState, DT_UINT8, PARAMETER_GET_STATE);
		if(Module4TypeState == STATE_ACTIVE)
		{
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module4Type, "");
			if(Parameter.System.Identification.Hardware.Module4Type.Value != VH_NOT_AVAIABLE)
			{
				//Module 4 Type
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module4Type "");
				Module4TypeValue = (SINT16)Parameter.System.Identification.Hardware.Module4Type.Value;
				//Module 4 Hardware Version
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module4HardwareVersion "");
				Module4HardwareVersionValue = (SINT16)Parameter.System.Identification.Hardware.Module4HardwareVersion.Value;
				//Module 4 Assembly Variant
				HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.Module4AssemblyVariant "");
				Module4AssemblyVariantValue = (SINT16)Parameter.System.Identification.Hardware.Module4AssemblyVariant.Value;
			}
		}
	}
#endif
	//Abfrage der Id -> da nicht sicher, ob die Module vorhanden sind
	Module1TypeId = GetModule1TypeId(Module1TypeValue);
	Module1HardwareVersionId = GetModule1HardwareVersionId(Module1HardwareVersionValue);
	Module1AssemblyVariantId = GetModule1AssemblyVariantId(Module1AssemblyVariantValue);
	Module2TypeId = GetModule2TypeId(Module2TypeValue);
	Module2HardwareVersionId = GetModule2HardwareVersionId(Module2HardwareVersionValue);
	Module2AssemblyVariantId = GetModule2AssemblyVariantId(Module2AssemblyVariantValue);
	Module3TypeId = GetModule3TypeId(Module3TypeValue);
	Module3HardwareVersionId = GetModule3HardwareVersionId(Module3HardwareVersionValue);
	Module3AssemblyVariantId = GetModule3AssemblyVariantId(Module3AssemblyVariantValue);
	Module4TypeId = GetModule4TypeId(Module4TypeValue);
	Module4HardwareVersionId = GetModule4HardwareVersionId(Module4HardwareVersionValue);
	Module4AssemblyVariantId = GetModule4AssemblyVariantId(Module4AssemblyVariantValue);
	
	//Eintrag in der Datenbank suchen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveHardware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ValveHardwareId, &ValveHardwareIdStat);
		resCode = DBBindColInt(hstmt, 2, &ValveSerieDatabaseId, &ValveSerieIdStat);
		resCode = DBBindColInt(hstmt, 3, &InterfaceTypeDatabaseId, &InterfaceTypeIdStat);
		resCode = DBBindColInt(hstmt, 4, &ControllerTypeDatabaseId, &ControllerTypeIdStat);
		resCode = DBBindColInt(hstmt, 5, &OptionTypeDatabaseId, &OptionTypeIdStat);
		resCode = DBBindColShort(hstmt, 6, &ExternalIsolationValveDatabase, &ExternalIsolationValveStat);
		resCode = DBBindColInt(hstmt, 7, &ControllerHardwareVersionDatabaseId, &ControllerHardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 8, &InterfaceHardwareVersionDatabaseId, &InterfaceHardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 9, &ControllerAssemblyVariantDatabaseId, &ControllerAssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 10, &InterfaceAssemblyVariantDatabaseId, &InterfaceAssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 11, &Module1TypeDatabaseId, &Module1TypeIdStat);
		resCode = DBBindColInt(hstmt, 12, &Module1HardwareVersionDatabaseId, &Module1HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 13, &Module1AssemblyVariantDatabaseId, &Module1AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 14, &Module2TypeDatabaseId, &Module2TypeIdStat);
		resCode = DBBindColInt(hstmt, 15, &Module2HardwareVersionDatabaseId, &Module2HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 16, &Module2AssemblyVariantDatabaseId, &Module2AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 17, &Module3TypeDatabaseId, &Module3TypeIdStat);
		resCode = DBBindColInt(hstmt, 18, &Module3HardwareVersionDatabaseId, &Module3HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 19, &Module3AssemblyVariantDatabaseId, &Module3AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 20, &Module4TypeDatabaseId, &Module4TypeIdStat);
		resCode = DBBindColInt(hstmt, 21, &Module4HardwareVersionDatabaseId, &Module4HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 22, &Module4AssemblyVariantDatabaseId, &Module4AssemblyVariantIdStat);
	
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ValveSerieDatabaseId == ValveSerieId && InterfaceTypeDatabaseId == InterfaceTypeId && ControllerTypeDatabaseId == ControllerTypeId && OptionTypeDatabaseId == OptionTypeId && ExternalIsolationValveDatabase == ExternalIsolationValve && ControllerHardwareVersionDatabaseId == ControllerHardwareVersionId && InterfaceHardwareVersionDatabaseId == InterfaceHardwareVersionId && ControllerAssemblyVariantDatabaseId == ControllerAssemblyVariantId && InterfaceAssemblyVariantDatabaseId == InterfaceAssemblyVariantId 
					&& Module1TypeDatabaseId == Module1TypeId && Module1HardwareVersionDatabaseId == Module1HardwareVersionId && Module1AssemblyVariantDatabaseId == Module1AssemblyVariantId	&& Module2TypeDatabaseId == Module2TypeId && Module2HardwareVersionDatabaseId == Module2HardwareVersionId && Module2AssemblyVariantDatabaseId == Module2AssemblyVariantId	&& Module3TypeDatabaseId == Module3TypeId && Module3HardwareVersionDatabaseId == Module3HardwareVersionId && Module3AssemblyVariantDatabaseId == Module3AssemblyVariantId	&& Module4TypeDatabaseId == Module4TypeId && Module4HardwareVersionDatabaseId == Module4HardwareVersionId && Module4AssemblyVariantDatabaseId == Module4AssemblyVariantId)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ValveHardwareId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Wenn kein Eintrag gefunden -> soll ein neuer erzeugt werden
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToInt (hmap, "ValveSerie_Id", &ValveSerieDatabaseId, &ValveSerieIdStat);
	resCode = DBMapColumnToInt (hmap, "InterfaceType_Id", &InterfaceTypeDatabaseId, &InterfaceTypeIdStat);
	resCode = DBMapColumnToInt (hmap, "ControllerType_Id", &ControllerTypeDatabaseId, &ControllerTypeIdStat);
	resCode = DBMapColumnToInt (hmap, "OptionType_Id", &OptionTypeDatabaseId, &OptionTypeIdStat);
	resCode = DBMapColumnToShort (hmap, "ExternalIsolationValveOption", &ExternalIsolationValveDatabase, &ExternalIsolationValveStat);
	resCode = DBMapColumnToInt (hmap, "ControllerHardwareVersion_Id", &ControllerHardwareVersionDatabaseId, &ControllerHardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "InterfaceHardwareVersion_Id", &InterfaceHardwareVersionDatabaseId, &InterfaceHardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "ControllerAssemblyVariant_Id", &ControllerAssemblyVariantDatabaseId, &ControllerAssemblyVariantIdStat);
	resCode = DBMapColumnToInt (hmap, "InterfaceAssemblyVariant_Id", &InterfaceAssemblyVariantDatabaseId, &InterfaceAssemblyVariantIdStat);
	resCode = DBMapColumnToInt (hmap, "Module1Type_Id", &Module1TypeDatabaseId, &Module1TypeIdStat);
	resCode = DBMapColumnToInt (hmap, "Module1HardwareVersion_Id", &Module1HardwareVersionDatabaseId, &Module1HardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "Module1AssemblyVariant_Id",  &Module1AssemblyVariantDatabaseId, &Module1AssemblyVariantIdStat);
	resCode = DBMapColumnToInt (hmap, "Module2Type_Id", &Module2TypeDatabaseId, &Module2TypeIdStat);
	resCode = DBMapColumnToInt (hmap, "Module2HardwareVersion_Id", &Module2HardwareVersionDatabaseId, &Module2HardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "Module2AssemblyVariant_Id",  &Module2AssemblyVariantDatabaseId, &Module2AssemblyVariantIdStat);
	resCode = DBMapColumnToInt (hmap, "Module3Type_Id", &Module3TypeDatabaseId, &Module3TypeIdStat);
	resCode = DBMapColumnToInt (hmap, "Module3HardwareVersion_Id", &Module3HardwareVersionDatabaseId, &Module3HardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "Module3AssemblyVariant_Id",  &Module3AssemblyVariantDatabaseId, &Module3AssemblyVariantIdStat);
	resCode = DBMapColumnToInt (hmap, "Module4Type_Id", &Module4TypeDatabaseId, &Module4TypeIdStat);
	resCode = DBMapColumnToInt (hmap, "Module4HardwareVersion_Id", &Module4HardwareVersionDatabaseId, &Module4HardwareVersionIdStat);
	resCode = DBMapColumnToInt (hmap, "Module4AssemblyVariant_Id",  &Module4AssemblyVariantDatabaseId, &Module4AssemblyVariantIdStat);
	
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ValveHardware");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	ValveSerieDatabaseId = ValveSerieId;
	InterfaceTypeDatabaseId = InterfaceTypeId; 
	ControllerTypeDatabaseId = ControllerTypeId; 
	OptionTypeDatabaseId = OptionTypeId; 
	ExternalIsolationValveDatabase = ExternalIsolationValve; 
	ControllerHardwareVersionDatabaseId = ControllerHardwareVersionId; 
	InterfaceHardwareVersionDatabaseId = InterfaceHardwareVersionId;
	ControllerAssemblyVariantDatabaseId = ControllerAssemblyVariantId; 
	InterfaceAssemblyVariantDatabaseId = InterfaceAssemblyVariantId; 
	Module1TypeDatabaseId = Module1TypeId; 
	Module1HardwareVersionDatabaseId = Module1HardwareVersionId; 
	Module1AssemblyVariantDatabaseId = Module1AssemblyVariantId; 
	Module2TypeDatabaseId = Module2TypeId; 
	Module2HardwareVersionDatabaseId = Module2HardwareVersionId; 
	Module2AssemblyVariantDatabaseId = Module2AssemblyVariantId; 
	Module3TypeDatabaseId = Module3TypeId; 
	Module3HardwareVersionDatabaseId = Module3HardwareVersionId; 
	Module3AssemblyVariantDatabaseId = Module3AssemblyVariantId; 
	Module4TypeDatabaseId = Module4TypeId; 
	Module4HardwareVersionDatabaseId = Module4HardwareVersionId; 
	Module4AssemblyVariantDatabaseId = Module4AssemblyVariantId; 
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
	
	//Brauche die Id des vorher erzeugten Eintrages
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveHardware");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &ValveHardwareId, &ValveHardwareIdStat);
		resCode = DBBindColInt(hstmt, 2, &ValveSerieDatabaseId, &ValveSerieIdStat);
		resCode = DBBindColInt(hstmt, 3, &InterfaceTypeDatabaseId, &InterfaceTypeIdStat);
		resCode = DBBindColInt(hstmt, 4, &ControllerTypeDatabaseId, &ControllerTypeIdStat);
		resCode = DBBindColInt(hstmt, 5, &OptionTypeDatabaseId, &OptionTypeIdStat);
		resCode = DBBindColShort(hstmt, 6, &ExternalIsolationValveDatabase, &ExternalIsolationValveStat);
		resCode = DBBindColInt(hstmt, 7, &ControllerHardwareVersionDatabaseId, &ControllerHardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 8, &InterfaceHardwareVersionDatabaseId, &InterfaceHardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 9, &ControllerAssemblyVariantDatabaseId, &ControllerAssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 10, &InterfaceAssemblyVariantDatabaseId, &InterfaceAssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 11, &Module1TypeDatabaseId, &Module1TypeIdStat);
		resCode = DBBindColInt(hstmt, 12, &Module1HardwareVersionDatabaseId, &Module1HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 13, &Module1AssemblyVariantDatabaseId, &Module1AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 14, &Module2TypeDatabaseId, &Module2TypeIdStat);
		resCode = DBBindColInt(hstmt, 15, &Module2HardwareVersionDatabaseId, &Module2HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 16, &Module2AssemblyVariantDatabaseId, &Module2AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 17, &Module3TypeDatabaseId, &Module3TypeIdStat);
		resCode = DBBindColInt(hstmt, 18, &Module3HardwareVersionDatabaseId, &Module3HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 19, &Module3AssemblyVariantDatabaseId, &Module3AssemblyVariantIdStat);
		resCode = DBBindColInt(hstmt, 20, &Module4TypeDatabaseId, &Module4TypeIdStat);
		resCode = DBBindColInt(hstmt, 21, &Module4HardwareVersionDatabaseId, &Module4HardwareVersionIdStat);
		resCode = DBBindColInt(hstmt, 22, &Module4AssemblyVariantDatabaseId, &Module4AssemblyVariantIdStat);
	
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ValveSerieDatabaseId == ValveSerieId && InterfaceTypeDatabaseId == InterfaceTypeId && ControllerTypeDatabaseId == ControllerTypeId && OptionTypeDatabaseId == OptionTypeId && ExternalIsolationValveDatabase == ExternalIsolationValve && ControllerHardwareVersionDatabaseId == ControllerHardwareVersionId && InterfaceHardwareVersionDatabaseId == InterfaceHardwareVersionId && ControllerAssemblyVariantDatabaseId == ControllerAssemblyVariantId && InterfaceAssemblyVariantDatabaseId == InterfaceAssemblyVariantId 
					&& Module1TypeDatabaseId == Module1TypeId && Module1HardwareVersionDatabaseId == Module1HardwareVersionId && Module1AssemblyVariantDatabaseId == Module1AssemblyVariantId	&& Module2TypeDatabaseId == Module2TypeId && Module2HardwareVersionDatabaseId == Module2HardwareVersionId && Module2AssemblyVariantDatabaseId == Module2AssemblyVariantId	&& Module3TypeDatabaseId == Module3TypeId && Module3HardwareVersionDatabaseId == Module3HardwareVersionId && Module3AssemblyVariantDatabaseId == Module3AssemblyVariantId	&& Module4TypeDatabaseId == Module4TypeId && Module4HardwareVersionDatabaseId == Module4HardwareVersionId && Module4AssemblyVariantDatabaseId == Module4AssemblyVariantId)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ValveHardwareId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	return -1; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt einen TestCollectionResult Eintrag
//  Input           : ValveHardwareId: Id eines ValveHardware Eintrages
//  Output *Pointer : -
//         *return  : Gibt die ID des erzeugten Eintrages zurück
//******************************************************************************************************************************
SINT32 Database_CreateTestCollectionResultEntry(SINT32 ValveHardwareId, SINT32 InitialStateValveId)
{
	SINT16 CountErrorTest = 0;
	SINT32 TestCollectionResultId, AbortTypeId;
	long ExecutionDateStringLength = DATE_TIME_TYPE_LENGTH, ExecutionDateStringStat, UserNameLength = USER_LENGTH, UserNameStat, ValveHardwareIdStat;
	long InitialStateValveIdState, TestCollectionResultIdStat, AbortTypeIdStat, CountErrorTestStat;
	char ExecutionDateTimeStringDatabase[DATE_TIME_TYPE_LENGTH], ExecutionDateTimeString[DATE_TIME_TYPE_LENGTH];
	
	//Aktuelles Datum&Zeit auslesen
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);
	sprintf(ExecutionDateTimeString,"%04d-%02d-%02d %02d:%02d:%02d", str_t.wYear, str_t.wMonth, str_t.wDay, str_t.wHour+TimeOffset, str_t.wMinute, str_t.wSecond);
	//Problem das Systemtime 24h nimmt und Datenbank mit 00h den Eintrag schreibt -> hier schon konventieren
	if(ExecutionDateTimeString[11] == '2' && ExecutionDateTimeString[12] == '4')
	{
		ExecutionDateTimeString[11] = '0';
		ExecutionDateTimeString[12] = '0';
	}
	
	//User abfragen -> default STUA
	if(strlen(User) == 0)	strcpy(User, "STUA");
	
	//Abort Type aus Datenbank holen
	AbortTypeId = Database_GetAbortTypeId("None");
	
	//Eintrag erstellen
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "ExecutionDateTime", ExecutionDateStringLength, ExecutionDateTimeString, &ExecutionDateStringStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToChar (hmap, "UserName", UserNameLength, User, &UserNameStat, "");
	resCode = DBMapColumnToInt (hmap, "AbortType_Id", &AbortTypeId, &AbortTypeIdStat);
	resCode = DBMapColumnToShort (hmap, "CountErrorTest", &CountErrorTest, &CountErrorTestStat);
	resCode = DBMapColumnToInt (hmap, "ValveHardware_Id", &ValveHardwareId, &ValveHardwareIdStat);
	resCode = DBMapColumnToInt (hmap, "InitialStateFirmware_Id", &InitialStateValveId, &InitialStateValveIdState);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestCollectionResult");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
	
	//Neu erzeugten Eintrag herauslesen, um ID zu bekommen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollectionResult");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestCollectionResultId, &TestCollectionResultIdStat);
		resCode = DBBindColChar (hstmt, 2, ExecutionDateStringLength, ExecutionDateTimeStringDatabase, &ExecutionDateStringStat, "yyyy-mm-dd hh:mm:ss");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strcmp(ExecutionDateTimeStringDatabase, ExecutionDateTimeString) == 0)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return TestCollectionResultId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die AbortTypeId zurück mit Hilfe dessen Namens
//  Input           : pAbortTypeName: Name des Abort Types
//  Output *Pointer : -
//         *return  : AbortTypeId
//******************************************************************************************************************************
SINT32 Database_GetAbortTypeId(char *pAbortTypeName)
{
	SINT32 AbortTypeId;
	long AbortTypeIdStat, AbortTypeNameLength = ABORT_TYPE_NAME_LENGTH, AbortTypeNameDatabaseStat;
	char AbortTypeNameDatabase[ABORT_TYPE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM AbortType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &AbortTypeId, &AbortTypeIdStat);
		resCode = DBBindColChar (hstmt, 2, AbortTypeNameLength, AbortTypeNameDatabase, &AbortTypeNameDatabaseStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strcmp(AbortTypeNameDatabase, pAbortTypeName) == 0)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return AbortTypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Abort type name not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die ResultTypeId zurück mit Hilfe dessen Namens
//  Input           : pResultTypeName: Name des Result Types
//  Output *Pointer : -
//         *return  : ResultTypeId
//******************************************************************************************************************************
SINT32 Database_GetResultTypeId(char *pResultTypeName)
{
	SINT32 ResultTypeId;
	long ResultTypeIdStat, ResultTypeNameLength = RESULT_TYPE_NAME_LENGTH, ResultTypeNameDatabaseStat;
	char ResultTypeNameDatabase[RESULT_TYPE_NAME_LENGTH];
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ResultType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ResultTypeId, &ResultTypeIdStat);
		resCode = DBBindColChar (hstmt, 2, ResultTypeNameLength, ResultTypeNameDatabase, &ResultTypeNameDatabaseStat, "");
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strcmp(ResultTypeNameDatabase, pResultTypeName) == 0)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ResultTypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Result type name not found in database");
	return -1;
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt einen TestResult Eintrag
//  Input           : TestCollectionResultId: Id des TestCollectionResult Eintrages
//										TestVersionId:					Id der TestVersion
//  Output *Pointer : -
//         *return  : Gibt die ID des erzeugten Eintrages zurück
//******************************************************************************************************************************
SINT32 Database_CreateTestResultEntry(SINT32 TestCollectionResultId, SINT32 TestVersionId, char *pStartTime)
{
	SINT32 TestResultDatabaseId, TestCollectionResultDatabaseId, TestVersionDatabaseId, ResultTypeId;
	long StartTimeStringLength = DATE_TIME_TYPE_LENGTH, StartTimeStringStat, TestCollectionResultIdStat, TestVersionIdStat, TestResultDatabaseIdStat, TestResultIdStat;
	long EndTimeStringLength = DATE_TIME_TYPE_LENGTH, EndTimeStringStat;
	char StartTimeStringDatabase[DATE_TIME_TYPE_LENGTH], EndTimeStringDatabase[DATE_TIME_TYPE_LENGTH];
	
	//Aktuelle Zeit auslesen
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);
	sprintf(pStartTime,"%04d-%02d-%02d %02d:%02d:%02d", str_t.wYear, str_t.wMonth, str_t.wDay, str_t.wHour+TimeOffset, str_t.wMinute, str_t.wSecond);
	//Problem das Systemtime 24h nimmt und Datenbank mit 00h den Eintrag schreibt -> hier schon konventieren
	if(pStartTime[11] == '2' && pStartTime[12] == '4')
	{
		pStartTime[11] = '0';
		pStartTime[12] = '0';
	}

	//Muss ein Wert in der Datenbank stehen, um ihn später updaten zu können
	strcpy(EndTimeStringDatabase, pStartTime);
	
	//Result Type aus Datenbank holen
	ResultTypeId = Database_GetResultTypeId("Successful");
	
	//Eintrag erstellen -> muss zuerst das TestResultat auf fehlerlos setzten, um den Eintrag erstellen zu können
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "StartTime", StartTimeStringLength, pStartTime, &StartTimeStringStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToChar (hmap, "EndTime", EndTimeStringLength, EndTimeStringDatabase, &EndTimeStringStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToInt (hmap, "ResultType_Id", &ResultTypeId, &TestResultIdStat);
	resCode = DBMapColumnToInt (hmap, "TestCollectionResult_Id", &TestCollectionResultId, &TestCollectionResultIdStat);
	resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionId, &TestVersionIdStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestResult");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();

	//Neu erzeugten Eintrag herauslesen, um ID zu bekommen
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestResult");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestResultDatabaseId, &TestResultDatabaseIdStat);
		resCode = DBBindColChar (hstmt, 2, StartTimeStringLength, StartTimeStringDatabase, &StartTimeStringStat, "yyyy-mm-dd hh:mm:ss");
		resCode = DBBindColInt(hstmt, 5, &TestCollectionResultDatabaseId, &TestCollectionResultIdStat);
		resCode = DBBindColInt(hstmt, 6, &TestVersionDatabaseId, &TestVersionIdStat);
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(strcmp(StartTimeStringDatabase, pStartTime) == 0 && TestCollectionResultDatabaseId == TestCollectionResultId && TestVersionDatabaseId == TestVersionId)	 
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return TestResultDatabaseId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt das Resultat und die Endzeit in den TestResultEintrag
//  Input           : TestResultId: Id des TestResult Eintrages, welcher beschriebeb werden soll
//										TestResult:		Ergebnis des Testes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestResultTable(char *pStartTime, SINT32 TestCollectionResultId, UINT8 TestResult)
{
	BOOL8 FoundEntry = FALSE;
	SINT32 TestResultIdDatabase, ResultTypeId = -1;
	long EndTimeStringLength = DATE_TIME_TYPE_LENGTH, EndTimeStringStat, TestResultIdStat;
	long StartTimeStringLength = DATE_TIME_TYPE_LENGTH, StartTimeStringStat;
	char EndTimeString[DATE_TIME_TYPE_LENGTH], EndTimeStringDatabase[DATE_TIME_TYPE_LENGTH], StartTimeStringDatabase[DATE_TIME_TYPE_LENGTH];
	
	//Aktuelle Zeit auslesen
	SYSTEMTIME str_t;
	GetSystemTime(&str_t);
	sprintf(EndTimeString, "%04d-%02d-%02d %02d:%02d:%02d", str_t.wYear, str_t.wMonth, str_t.wDay, str_t.wHour+TimeOffset, str_t.wMinute, str_t.wSecond);
	//Problem das Systemtime 24h nimmt und Datenbank mit 00h den Eintrag schreibt -> hier schon konventieren
	if(EndTimeString[11] == '2' && EndTimeString[12] == '4')
	{
		EndTimeString[11] = '0';
		EndTimeString[12] = '0';
	}
	
	switch(TestResult)
	{
		case SUCCESFUL:			ResultTypeId = Database_GetResultTypeId("Successful"); 			break; 
		case USERCANCELED:	ResultTypeId = Database_GetResultTypeId("User Cancelled"); 	break; 
		case ERRORTEST:			ResultTypeId = Database_GetResultTypeId("Error Test"); 			break; 
		default:																																				break;
	}
	
	//Eintrag ändern mit der Endzeit und dem Resultat
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "StartTime", StartTimeStringLength, StartTimeStringDatabase, &StartTimeStringStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToChar (hmap, "EndTime", EndTimeStringLength, EndTimeStringDatabase, &EndTimeStringStat, "yyyy-mm-dd hh:mm:ss");
	resCode = DBMapColumnToInt (hmap, "ResultType_Id", &TestResultIdDatabase, &TestResultIdStat);
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestResult");
	if (resCode != DB_SUCCESS) ShowError();
  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
	{
		//Kann nicht die Id überprüfen gehen, da diese später nicht in die Datenbank upgedatet werden kann
    if(strcmp(StartTimeStringDatabase, pStartTime) == 0)
		{
			FoundEntry = TRUE;
			break;
		}
	}
	if(FoundEntry == TRUE)
	{
		strcpy(EndTimeStringDatabase, EndTimeString);
		TestResultIdDatabase = ResultTypeId;
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	else	MessagePopup("Database Error","Result entry not found in database to write test result");
	
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt die Error Messages in die Datenbank
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestErrorMessageTable(SINT32 TestResultId)
{
	UINT8 IndexErrorMessages;
	long ErrorMessageNameLength = ERROR_MESSAGE_NAME_LENGTH,ErrorMessageNameStat, TestResultIdStat;
	char ErrorMessageName[ERROR_MESSAGE_NAME_LENGTH];
	
	for(IndexErrorMessages = 0; IndexErrorMessages < TestErrorMessageIndex; IndexErrorMessages++)
	{
		hmap = DBBeginMap (hdbc);
		if (hmap <= 0) ShowError();
		resCode = DBMapColumnToChar (hmap, "Description", ErrorMessageNameLength, ErrorMessageName, &ErrorMessageNameStat, "");
		resCode = DBMapColumnToInt (hmap, "TestResult_Id", &TestResultId, &TestResultIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		hstmt = DBActivateMap (hmap, "TestErrorMessage");
		if (resCode != DB_SUCCESS) ShowError();
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
		strcpy(ErrorMessageName, TestErrorMessageList[IndexErrorMessages]);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
		resCode = DBDeactivateMap (hmap);
		if (resCode != DB_SUCCESS) ShowError();
	}
} 

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt die Anzahl fehlerhaften Tests und der Abbruch Grund in die TestCollectionResult Tabelle
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_WriteTestCollectionResultTable(SINT32 TestCollectionResultId, SINT32 AbortTypeId, SINT16 CountErrorTests)
{
	BOOL8 FindEntry = FALSE;
	SINT16 CountErrorTestsDatabase;
	SINT32 TestCollectionResultDatabaseId, AbortTypeDatabaseId;
	long TestCollectionResultDatabaseIdStat, ExecutionDateTimeLength = DATE_TIME_TYPE_LENGTH, ExecutionDateTimeStringStat, AbortTypeIdStat, CountErrorTestsStat;
	char ExecutionDateTimeDatabaseString[DATE_TIME_TYPE_LENGTH] = "", ExecutionDateTimeString[DATE_TIME_TYPE_LENGTH] = "";
	
	//Zuerst die ExecutionDateTime aus dem TestCollectionResult Eintrag holen -> mit diesem Eintrag können später Werte abgeändert werden
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestCollectionResult");
	if (hstmt > 0)
	{
		resCode = DBBindColInt(hstmt, 1, &TestCollectionResultDatabaseId, &TestCollectionResultDatabaseIdStat);
		resCode = DBBindColChar (hstmt, 2, ExecutionDateTimeLength, ExecutionDateTimeString, &ExecutionDateTimeStringStat, "yyyy-mm-dd hh:mm:ss");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(TestCollectionResultDatabaseId == TestCollectionResultId)	 
			{
				FindEntry = TRUE;
				break;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	if(FindEntry == TRUE)
	{
		hmap = DBBeginMap (hdbc);
		if (hmap <= 0) ShowError();
		resCode = DBMapColumnToChar (hmap, "ExecutionDateTime", ExecutionDateTimeLength, ExecutionDateTimeDatabaseString, &ExecutionDateTimeStringStat, "yyyy-mm-dd hh:mm:ss");
		resCode = DBMapColumnToInt (hmap, "AbortType_Id", &AbortTypeDatabaseId, &AbortTypeIdStat);
		resCode = DBMapColumnToShort (hmap, "CountErrorTest", &CountErrorTestsDatabase, &CountErrorTestsStat);
		if (resCode != DB_SUCCESS) ShowError();
		hstmt = DBActivateMap (hmap, "TestCollectionResult");
		if (resCode != DB_SUCCESS) ShowError();
	  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
			//Kann nicht die Id überprüfen gehen, da diese später nicht in die Datenbank upgedatet werden kann
	    if(strcmp(ExecutionDateTimeDatabaseString, ExecutionDateTimeString) == 0) break;
		}
		AbortTypeDatabaseId = AbortTypeId;
		CountErrorTestsDatabase = CountErrorTests;
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
		resCode = DBDeactivateMap (hmap);
		if (resCode != DB_SUCCESS) ShowError();
	}
	else	MessagePopup("Database Error","TestCollectionResult entry not found in database to write test collection result");
}








//interne Funtkionen
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wenn ein Fehler mit der Datenbank auftrit, wird eine Fehlermeldung ausgegeben
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void ShowError(void)
{
	MessagePopup("Database Error",DBErrorMessage());
	ErrorState = TRUE;
}

//Valve Hardware
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Valve Serie Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : ValveSerieId
//******************************************************************************************************************************
static SINT32 GetValveSerieId(SINT32 EnumValue)
{
	SINT32 ValveSerieId, EnumValueDatabase;
	long ValveSerieStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ValveSerieId, &ValveSerieStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ValveSerieId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Valve Serie value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die InterfaceType Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : InterfaceTypeId
//******************************************************************************************************************************
static SINT32 GetInterfaceTypeId(SINT32 EnumValue)
{
	SINT32 InterfaceTypeId, EnumValueDatabase;
	long InterfaceTypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceTypeId, &InterfaceTypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceTypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","InterfaceType value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die ControllerType Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : ControllerTypeId
//******************************************************************************************************************************
static SINT32 GetControllerTypeId(SINT32 EnumValue)
{
	SINT32 ControllerTypeId, EnumValueDatabase;
	long ControllerTypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerTypeId, &ControllerTypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerTypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","ControllerType value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die OptionType Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : ControllerTypeId
//******************************************************************************************************************************
static SINT32 GetOptionTypeId(SINT16 EnumValue)
{
	SINT32 OptionTypeId, EnumValueDatabase;
	long OptionTypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM OptionType");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &OptionTypeId, &OptionTypeStat);
		resCode = DBBindColInt  (hstmt, 2, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return OptionTypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","OptionType value not found in database");
	return -1;
} 

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die ControllerHardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : ControllerHardwareVersionId
//******************************************************************************************************************************
static SINT32 GetControllerHardwareVersionId(SINT16 EnumValue)
{
	SINT32 ControllerHardwareVersionId, EnumValueDatabase;
	long ControllerHardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerHardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerHardwareVersionId, &ControllerHardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerHardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","ControllerHardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die InterfaceHardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : InterfaceHardwareVersionId
//******************************************************************************************************************************
static SINT32 GetInterfaceHardwareVersionId(SINT16 EnumValue)
{
	SINT32 InterfaceHardwareVersionId, EnumValueDatabase;
	long InterfaceHardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceHardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceHardwareVersionId, &InterfaceHardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceHardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","InterfaceHardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die ControllerAssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : ControllerAssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetControllerAssemblyVariantId(SINT16 EnumValue)
{
	SINT32 ControllerAssemblyVariantId, EnumValueDatabase;
	long ControllerAssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerAssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerAssemblyVariantId, &ControllerAssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerAssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","ControllerAssemblyVariant value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die InterfaceAssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : InterfaceAssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetInterfaceAssemblyVariantId(SINT16 EnumValue)
{
	SINT32 InterfaceAssemblyVariantId, EnumValueDatabase;
	long InterfaceAssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceAssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceAssemblyVariantId, &InterfaceAssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceAssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","InterfaceAssemblyVariant value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module1Type Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module1TypeId
//******************************************************************************************************************************
static SINT32 GetModule1TypeId(SINT16 EnumValue) 
{
	SINT32 Module1TypeId, EnumValueDatabase;
	long Module1TypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1Type");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1TypeId, &Module1TypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1TypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module1Type value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module1HardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module1HardwareVersionId
//******************************************************************************************************************************
static SINT32 GetModule1HardwareVersionId(SINT16 EnumValue) 
{
	SINT32 Module1HardwareVersionId, EnumValueDatabase;
	long Module1HardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1HardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1HardwareVersionId, &Module1HardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1HardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module1HardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module1AssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module1AssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetModule1AssemblyVariantId(SINT16 EnumValue)
{
	SINT32 Module1AssemblyVariantId, EnumValueDatabase;
	long Module1AssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1AssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1AssemblyVariantId, &Module1AssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1AssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module1AssemblyVariant value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module2Type Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module2TypeId
//******************************************************************************************************************************
static SINT32 GetModule2TypeId(SINT16 EnumValue) 
{
	SINT32 Module2TypeId, EnumValueDatabase;
	long Module2TypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2Type");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2TypeId, &Module2TypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2TypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module2Type value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module2HardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module2HardwareVersionId
//******************************************************************************************************************************
static SINT32 GetModule2HardwareVersionId(SINT16 EnumValue) 
{
	SINT32 Module2HardwareVersionId, EnumValueDatabase;
	long Module2HardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2HardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2HardwareVersionId, &Module2HardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2HardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module2HardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module2AssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module2AssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetModule2AssemblyVariantId(SINT16 EnumValue)
{
	SINT32 Module2AssemblyVariantId, EnumValueDatabase;
	long Module2AssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2AssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2AssemblyVariantId, &Module2AssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2AssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module2AssemblyVariant value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module3Type Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module3TypeId
//******************************************************************************************************************************
static SINT32 GetModule3TypeId(SINT16 EnumValue) 
{
	SINT32 Module3TypeId, EnumValueDatabase;
	long Module3TypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3Type");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3TypeId, &Module3TypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3TypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module3Type value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module3HardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module3HardwareVersionId
//******************************************************************************************************************************
static SINT32 GetModule3HardwareVersionId(SINT16 EnumValue) 
{
	SINT32 Module3HardwareVersionId, EnumValueDatabase;
	long Module3HardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3HardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3HardwareVersionId, &Module3HardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3HardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module3HardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module3AssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module3AssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetModule3AssemblyVariantId(SINT16 EnumValue)
{
	SINT32 Module3AssemblyVariantId, EnumValueDatabase;
	long Module3AssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3AssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3AssemblyVariantId, &Module3AssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3AssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module3AssemblyVariant value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module4Type Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module4TypeId
//******************************************************************************************************************************
static SINT32 GetModule4TypeId(SINT16 EnumValue) 
{
	SINT32 Module4TypeId, EnumValueDatabase;
	long Module4TypeStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4Type");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4TypeId, &Module4TypeStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4TypeId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module4Type value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module4HardwareVersion Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module4HardwareVersionId
//******************************************************************************************************************************
static SINT32 GetModule4HardwareVersionId(SINT16 EnumValue) 
{
	SINT32 Module4HardwareVersionId, EnumValueDatabase;
	long Module4HardwareVersionStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4HardwareVersion");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4HardwareVersionId, &Module4HardwareVersionStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4HardwareVersionId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module4HardwareVersion value not found in database");
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Unter Angabe des EnumValue wird die Module4AssemblyVariant Id zurück gegeben
//  Input           : EnumIndex: Index des Enums
//  Output *Pointer : -
//         *return  : Module4AssemblyVariantId
//******************************************************************************************************************************
static SINT32 GetModule4AssemblyVariantId(SINT16 EnumValue)
{
	SINT32 Module4AssemblyVariantId, EnumValueDatabase;
	long Module4AssemblyVariantStat, EnumValueStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4AssemblyVariant");
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4AssemblyVariantId, &Module4AssemblyVariantStat);
		resCode = DBBindColInt  (hstmt, 3, &EnumValueDatabase, &EnumValueStat);
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if(EnumValueDatabase == EnumValue)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4AssemblyVariantId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	MessagePopup("Database Error","Module4AssemblyVariant value not found in database");
	return -1;
}
---




