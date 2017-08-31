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
#include "GenerateReport.h"
#include "HardwareEnums.h"
#include "HelpFunctions.h"
#include "SharedVariable.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
//interne Funtkionen
static void ConnectDatabase(void);
static SINT32 GetTestIdFromTestInformation(char *pName);
static SINT32 GetTestVersionID(SINT32 TestId, SINT32 TestVersion);
static void ShowError(void);

//Testinformationen in Datenbank schreiben
BOOL8 WriteTestInformationInDatabase(char *pName, char *pDescription, char *pCreationDate, char *pCreationAuthor);
BOOL8 WriteTestVersionInDatabase(SINT16 TestVersion, char *pModification, char *pDurating, char *pModulName, SINT16 ExternalIsolationValveOption, char *pName, SINT32 *pTestVersionId);

//Test Hardware Anforderungen definieren
BOOL8 WriteTestVersionValveSerieTable(SINT32 TestVersionId, SINT32 EnumValue);
BOOL8 WriteTestVersionControllerTypeTable(SINT32 TestVersionId, SINT32 EnumValue);
BOOL8 WriteTestVersionInterfaceTypeTable(SINT32 TestVersionId, SINT32 EnumValue);
BOOL8 WriteTestVersionSingleOptionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionTestHardwareTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionControllerHardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionInterfaceHardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionControllerAssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionInterfaceAssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule1HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule2HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule3HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule4HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule1AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule2AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule3AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule4AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule1TypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule2TypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule3TypeTable(SINT32 TestVersionId, SINT16 EnumValue);
BOOL8 WriteTestVersionModule4TypeTable(SINT32 TestVersionId, SINT16 EnumValue);

//Test Hardware Type Definition
static SINT32 WriteModulNameTable(char *pModulName); 
static void WriteValveSerieTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum);
static void WriteControllerTypeTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum);
static void WriteInterfaceTypeTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum);
static void WriteSingleOptionTypeTable(SINT16 EnumIndex, char *pEnum);
static void WriteOptionTypeTable(SINT16 EnumValue, char *pEnum);
static void WriteTestHardwareTypeTable(SINT16 EnumIndex, char *pEnum);
static void WriteControllerHardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteInterfaceHardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteControllerAssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteInterfaceAssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule1HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule2HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule3HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule4HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule1AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule2AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule3AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule4AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule1TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule2TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule3TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);
static void WriteModule4TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum);

//Liefert Test Hardware Types ID zurück
static SINT32 GetValveSerieId(SINT16 EnumIndex);
static SINT32 GetControllerTypeId(SINT16 EnumIndex);
static SINT32 GetInterfaceTypeId(SINT16 EnumIndex);
static SINT32 GetOptionTypeId(SINT16 EnumIndex);
static SINT32 GetTestHardwareTypeId(SINT16 EnumIndex);
static SINT32 GetControllerHardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetInterfaceHardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetControllerAssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetInterfaceAssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetModule1HardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetModule2HardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetModule3HardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetModule4HardwareVersionTypeId(SINT16 EnumIndex);
static SINT32 GetModule1AssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetModule2AssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetModule3AssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetModule4AssemblyVariantTypeId(SINT16 EnumIndex);
static SINT32 GetModule1TypeId(SINT16 EnumIndex);
static SINT32 GetModule2TypeId(SINT16 EnumIndex);
static SINT32 GetModule3TypeId(SINT16 EnumIndex);
static SINT32 GetModule4TypeId(SINT16 EnumIndex);

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
char ConnectionString[200] = "DRIVER={SQL Server};Server=CHVL0281\\SOFTWARE_DATABAS;Database=SoftwareVersionsDatabase;Trusted_Connection=yes;";


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




//Testinformationen in der Datenbank überprüfen
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Überprüft, ob die Testinformationen noch aktuell in der Datenbank sind 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_CheckAllTestInformation(void)
{
	BOOL8 ErrorDatabase = FALSE;
	SINT32 TestVersionId; 
	
	//Legt den TestInformation Eintrag an, wenn er nicht schon vorhanden ist
	ErrorDatabase = WriteTestInformationInDatabase(ReceiveTestName, ReceiveTestDescription, ReceiveCreationTest, ReceiveCreationAuthor);
	if(ErrorDatabase == TRUE)	return;
	//Legt den TestVersion Eintrag an, wenn dieser noch nicht existiert
	WriteTestVersionInDatabase(TestVersion, ReceiveTestModification, ReceiveTestDurating, ReceiveTestModulName, TestExternalIsolationValveOption, ReceiveTestName, &TestVersionId);
	if(ErrorDatabase == TRUE || TestVersionId == -1)	return;
		//Schreibt alle Hardware Anforderungen des Tests in die Datenbank, wenn diese noch nicht existieren 
	WriteTestVersionValveSerieTable(TestVersionId, TestValveSerie);
	WriteTestVersionControllerTypeTable(TestVersionId, TestController);
	WriteTestVersionInterfaceTypeTable(TestVersionId, TestInterface);
	WriteTestVersionSingleOptionTypeTable(TestVersionId, TestOption);
	WriteTestVersionTestHardwareTypeTable(TestVersionId, TestHardware);
	WriteTestVersionControllerHardwareVersionTypeTable(TestVersionId, TestControllerHardwareVersion);
	WriteTestVersionInterfaceHardwareVersionTypeTable(TestVersionId, TestInterfaceHardwareVersion);
	WriteTestVersionControllerAssemblyVariantTypeTable(TestVersionId, TestControllerAssemblyVariant);
	WriteTestVersionInterfaceAssemblyVariantTypeTable(TestVersionId, TestInterfaceAssemblyVariant);
	WriteTestVersionModule1HardwareVersionTypeTable(TestVersionId, TestModule1HardwareVersion);
	WriteTestVersionModule2HardwareVersionTypeTable(TestVersionId, TestModule2HardwareVersion);
	WriteTestVersionModule3HardwareVersionTypeTable(TestVersionId, TestModule3HardwareVersion);
	WriteTestVersionModule4HardwareVersionTypeTable(TestVersionId, TestModule4HardwareVersion);
	WriteTestVersionModule1AssemblyVariantTypeTable(TestVersionId, TestModule1AssemblyVariant);
	WriteTestVersionModule2AssemblyVariantTypeTable(TestVersionId, TestModule2AssemblyVariant);
	WriteTestVersionModule3AssemblyVariantTypeTable(TestVersionId, TestModule3AssemblyVariant);
	WriteTestVersionModule4AssemblyVariantTypeTable(TestVersionId, TestModule4AssemblyVariant);
	WriteTestVersionModule1TypeTable(TestVersionId, TestModule1);
	WriteTestVersionModule2TypeTable(TestVersionId, TestModule2);
	WriteTestVersionModule3TypeTable(TestVersionId, TestModule3);
	WriteTestVersionModule4TypeTable(TestVersionId, TestModule4);
}



//Test Hardware Types überprüfen
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Überprüft alle Hardware Types, ob sie noch aktuell in der Datenbank sind 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void Database_CheckAllHardwareTypes(void)
{
	BOOL8 TestHardwareDefinitionLine = FALSE, GetEnumString;
	static UINT8 Access = 1;
	SINT16 EnumValue, EnumIndex;
	SINT32 EnumValue32Bit;
	int FileHandle;
	char HardwareEnumSourceFilePath[MAX_PATHNAME_LEN], TextLineFromTxt[MAX_PATHNAME_LEN], HardwareTypeString[MAX_PATHNAME_LEN], EnumString[MAX_PATHNAME_LEN];
	
	strcpy(HardwareEnumSourceFilePath, TestDirection);
	strcat(HardwareEnumSourceFilePath, HARDWAREENUM_SOURCE_FILE_PATH);
	
	//Zuerst muss die TestHardwareDefintion Struktur ausgelesen werden
	FileHandle = OpenFile(HardwareEnumSourceFilePath, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
	
	//bekomme Zeile für Zeile, -2 bedeutet Ende des Files
	while(ReadLine(FileHandle, TextLineFromTxt, MAX_PATHNAME_LEN-1) != -2)
	{
		//kommt } ist Struktur fertig definiert
		if(TestHardwareDefinitionLine == TRUE)
		{
			if(strstr(TextLineFromTxt, "};") != NULL)	TestHardwareDefinitionLine = FALSE;
			//hier wird die Struktur bearbeitet
			else
			{
				//Rausfinden welches Hardware Typ definiert wird
				//Modul
				if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Modul.") != NULL)
				{
					 strcpy(HardwareTypeString, TextLineFromTxt);
					 GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					 if(GetEnumString == TRUE)	
					 {
						 strcpy(EnumString, HardwareTypeString);
						 WriteModulNameTable(EnumString);
					 }
				}
				//ValveSerie
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".ValveSerie.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValueInt32(HardwareTypeString, &EnumValue32Bit);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteValveSerieTable(EnumIndex, EnumValue32Bit, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Controller
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Controller.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValueInt32(HardwareTypeString, &EnumValue32Bit);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteControllerTypeTable(EnumIndex, EnumValue32Bit, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Interface
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Interface.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValueInt32(HardwareTypeString, &EnumValue32Bit);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteInterfaceTypeTable(EnumIndex, EnumValue32Bit, EnumString);
						Access = 0;
					}
					Access++;
				}
				//SingleOption
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".SingleOption.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex und Name definiert worden sein 
					if(Access == 2)
					{
						WriteSingleOptionTypeTable(EnumIndex, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Option
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Option.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumValue und Name definiert worden sein 
					if(Access == 2)
					{
						WriteOptionTypeTable(EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//TestHardware
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".TestHardware.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 2)
					{
						WriteTestHardwareTypeTable(EnumIndex, EnumString);
						Access = 0;
					}
					Access++;
				}
				//ControllerHardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".ControllerHardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteControllerHardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//InterfaceHardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".InterfaceHardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteInterfaceHardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module1HardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module1HardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule1HardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module2HardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module2HardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule2HardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module3HardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module3HardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule3HardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module4HardwareVersion
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module4HardwareVersion.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule4HardwareVersionTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//ControllerAssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".ControllerAssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteControllerAssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//InterfaceAssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".InterfaceAssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteInterfaceAssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module1AssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module1AssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule1AssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module2AssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module2AssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule2AssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module3AssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module3AssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule3AssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module4AssemblyVariant
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module4AssemblyVariant.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule4AssemblyVariantTypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module1
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module1.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule1TypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module2
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module2.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule2TypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module3
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module3.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule3TypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
				//Module4
				else if(HelpFunctions_StringAfterSubstring(TextLineFromTxt, ".Module4.") != NULL)
				{
					strcpy(HardwareTypeString, TextLineFromTxt);
					HardwareEnums_GetEnumIndex(HardwareTypeString, &EnumIndex);
					HardwareEnums_GetEnumValue(HardwareTypeString, &EnumValue);
					GetEnumString = HardwareEnums_GetEnumString(HardwareTypeString);
					if(GetEnumString == TRUE) strcpy(EnumString, HardwareTypeString);
					//Zuerst müssen EnumIndex, EnumValue und Name definiert worden sein 
					if(Access == 3)
					{
						WriteModule4TypeTable(EnumIndex, EnumValue, EnumString);
						Access = 0;
					}
					Access++;
				}
			}
		}
		//warten bis die TestHardwareDefintion Struktur anfangt
		if(strstr(TextLineFromTxt, "struct HardwareDefinition TestHardwareDefinition") != NULL)	TestHardwareDefinitionLine = TRUE;
	}

}






//Testinformationen in Datenbank schreiben 
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den Test in die Datenbank, wenn dieser noch nicht existiert
//										Wenn ein Eintrag noch nicht vorhanden ist, wird dieser erstellt. Besteht er bereits, wird dieser auf Gültigkeit überprüft
//  Input           : pName: 						Testname
//									:	pDescription:			Beschreibung des Tests
//									: pCreationDate:		Datum an welchem der Test erstellt wurde
//									: pCreationAuthor:	Name des Test Autor
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestInformationInDatabase(char *pName, char *pDescription, char *pCreationDate, char *pCreationAuthor)
{		
	BOOL8 ChangeEntry = FALSE, FindTestNameEntry = FALSE, i;
	UINT16 NameLength = TESTINFORMATION_NAME_LENGTH, DescriptionLengt = TESTINFOMATION_DESCRIPTION_LENGTH, CreationDateLength = DATE_TYPE_LENGTH, CreationAuthorLength = TESTINFOMATION_CREATIONAUTHOR_LENGTH;
	char TestName[TESTINFORMATION_NAME_LENGTH], TestDescription[TESTINFOMATION_DESCRIPTION_LENGTH], CretionDateString[DATE_TYPE_LENGTH], CreationAuthor[TESTINFOMATION_CREATIONAUTHOR_LENGTH];  
	long NameStat, DescriptionStat, CreationDateStat, CreationAuthorStat;
	
	ConnectDatabase();
	
	//zuerst nach dem Namen in der Datenbank suchen (muss SQL statement nehmen mit yyyy-mm-dd statement)
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestInformation");
	if (hstmt > 0)
	{
		resCode = DBBindColChar (hstmt, 2, NameLength, TestName, &NameStat, "");
		resCode = DBBindColChar (hstmt, 3, DescriptionLengt, TestDescription, &DescriptionStat, "");
		resCode = DBBindColChar (hstmt, 4, CreationDateLength, CretionDateString, &CreationDateStat, "yyyy-mm-dd");
		resCode = DBBindColChar (hstmt, 5, CreationAuthorLength, CreationAuthor, &CreationAuthorStat, "");
		
		if (resCode != DB_SUCCESS) ShowError();
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)	
		{
			if(StrICmpWithoutSurrWhiteSpace(TestName, pName) == 0)	
			{
				FindTestNameEntry = TRUE;
				break;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}

	//Struktur erzeugen, um später entweder einen Eintrag ändern oder neu erzeugen zu können
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError(); 
	resCode = DBMapColumnToChar (hmap, "Name", NameLength, TestName, &NameStat, ""); 
	resCode = DBMapColumnToChar (hmap, "Description", DescriptionLengt, TestDescription, &DescriptionStat, "");
	resCode = DBMapColumnToChar (hmap, "CreationDate", CreationDateLength, CretionDateString, &CreationDateStat, "YYYYMMDD");
	resCode = DBMapColumnToChar (hmap, "CreationAuthor", CreationAuthorLength, CreationAuthor, &CreationAuthorStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestInformation");
	if (resCode != DB_SUCCESS) ShowError();
	
	//hier hat er es nicht gefunden
	if (FindTestNameEntry == FALSE)
	{
		//neuer Eintrag erstellen
		resCode = DBCreateRecord (hstmt);
  	if (resCode != DB_SUCCESS) ShowError(); 
		strcpy(TestName, pName);
		strcpy(TestDescription, pDescription);
		strcpy(CretionDateString, pCreationDate);
		strcpy(CreationAuthor, pCreationAuthor);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	//schauen ob sich der Eintrag verändert hat
	else
	{
		//CreationDateString soll vom yyyy-mm-dd ins YYYYMMDD Format umgewandelt werden
		for(i = 0; i < 2; i++) HelpFunctions_StringReplace("-", "", CretionDateString);
		if(StrICmpWithoutSurrWhiteSpace(TestDescription, pDescription) != 0)		
		{
			strcpy(TestDescription, pDescription);
			ChangeEntry = TRUE;
		}
		if(StrICmpWithoutSurrWhiteSpace(CretionDateString, pCreationDate) != 0)	
		{
			strcpy(CretionDateString, pCreationDate);
			ChangeEntry = TRUE;
		}
		if(StrICmpWithoutSurrWhiteSpace(CreationAuthor, pCreationAuthor) != 0)	
		{
			strcpy(CreationAuthor, pCreationAuthor);
			ChangeEntry = TRUE;
		}
		if(ChangeEntry == TRUE)	
		{
			resCode = DBPutRecord (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
		}
	}
	
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
	return ErrorState; 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den Test in die Datenbank, wenn dieser noch nicht existiert
//										Wenn ein Eintrag noch nicht vorhanden ist, wird dieser erstellt. Besteht er bereits, wird dieser auf Gültigkeit überprüft
//  Input           : Number:												Version des Tests
//									:	pModification:								Beschreibung der Modifikation am Test
//									: pDurating:										Laufzeit des Tests
//									: ModulNameTest:								Name des Test Moduls
//									: ExternalIsolationValveOption:	Gibt an ob eine externe Ventil Option vorhanden ist
//									: pName: 						Testname
//  Output *Pointer : pTestVersionId: Gibt die Id des TestVersion zurück
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionInDatabase(SINT16 TestVersion, char *pModification, char *pDurating, char *pModulName, SINT16 ExternalIsolationValveOption, char *pName, SINT32 *pTestVersionId)
{	
	BOOL8 ChangeEntry = FALSE, FindTestVersionEntry = FALSE;
	UINT16 ModificationLength = TESTVERSION_MODIFICATION_LENGTH, DuratingLength = TESTVERSION_DURATING_LENGTH;
	SINT16 TestVersionNumber, ExternalIsolationValveOptionDatabase = 0;
	SINT32 TestId = 0, ModulNameId = 0, TestIdDatabase, ModulNamesId, TestVersionId;
	long TestVersionNumberStat, ModificationStat, DuratingStat, TestInformationIdStat, ModulNamesIdStat, ExternalIsolationValveOptionStat, TestVersionIdStat;
	char Modification[TESTVERSION_MODIFICATION_LENGTH], Durating[TESTVERSION_DURATING_LENGTH];
	
	ConnectDatabase();
	//wenn kein TestVersion Eintrag gefunden oder angelegt werden konnte wird -1 zurück gegeben
	*pTestVersionId = -1;
	
	//TestID Information abfragen//
	//Zuerst die Id des Tests holen -> wenn nicht in Datenbank kann abgebrochen werden
	TestId = GetTestIdFromTestInformation(pName);
	
	//hier hat er es nicht gefunden
	if (TestId == -1)	MessagePopup("Database Errror", "Test ist nicht in der Datenbank eingetragen um eine Version anlegen zu können");
	else
	{
		/*ModulName Eintrag erstellen/aktuallisieren*/
		ModulNameId = WriteModulNameTable(pModulName);
		
		/*Test Version Eintrag erstellen/aktualisieren*/
		//jetzt soll nach bereits vorhandenen Test Versionen gesucht werden
		hmap = DBBeginMap (hdbc);
		if (hmap <= 0) ShowError();
		resCode = DBMapColumnToShort (hmap, "Number", &TestVersionNumber, &TestVersionNumberStat); 
		resCode = DBMapColumnToChar (hmap, "Modification", ModificationLength, Modification, &ModificationStat, "");
		resCode = DBMapColumnToChar (hmap, "Durating", DuratingLength, Durating, &DuratingStat, "hh:mm:ss");
		resCode = DBMapColumnToInt (hmap, "ModulName_Id", &ModulNamesId, &ModulNamesIdStat);
		resCode = DBMapColumnToShort (hmap, "ExternalIsolationValveOption", &ExternalIsolationValveOptionDatabase, &ExternalIsolationValveOptionStat);
		
		resCode = DBMapColumnToInt (hmap, "TestInformation_Id", &TestIdDatabase, &TestInformationIdStat);
		if (resCode != DB_SUCCESS) ShowError();
		hstmt = DBActivateMap (hmap, "TestVersion");
		if (resCode != DB_SUCCESS) ShowError(); 
		
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
		{
      if (TestIdDatabase == TestId)
			{
				//Eintrag gibt es schon in der Datenbank (dieser Eintrag anpassen, wenn dieser sich verändert hat)
				if(TestVersionNumber == TestVersion)
				{
					FindTestVersionEntry = TRUE;
					if(StrICmpWithoutSurrWhiteSpace(Modification, pModification) != 0)							
					{
						strcpy(Modification, pModification);
						ChangeEntry = TRUE;
					}
					if(strstr(Durating, pDurating) == NULL)
					{
						strcpy(Durating, pDurating);
						ChangeEntry = TRUE;
					}
					if(ModulNamesId != ModulNameId)																									
					{
						ModulNamesId = ModulNameId;
						ChangeEntry = TRUE;
					}
					if(ExternalIsolationValveOptionDatabase != ExternalIsolationValveOption)	
					{
						ExternalIsolationValveOptionDatabase = ExternalIsolationValveOption;
						ChangeEntry = TRUE;
					}
					if(ChangeEntry == TRUE)
					{
						resCode = DBPutRecord (hstmt);
						if (resCode != DB_SUCCESS) ShowError();
						resCode = DBDeactivateMap (hmap);
						if (resCode != DB_SUCCESS) ShowError();
						*pTestVersionId = GetTestVersionID(TestId, TestVersion);
						return ErrorState;
					}
					else
					{
						resCode = DBDeactivateMap (hmap);
						if (resCode != DB_SUCCESS) ShowError();
						break;
					}
				}
			}
    }
		if (FindTestVersionEntry == FALSE) 
		{
			//Keinen Versions Eintrag gefunden -> neu erstellen
			resCode = DBCreateRecord (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
			TestVersionNumber = TestVersion;
			strcpy(Modification, pModification);
			strcpy(Durating, pDurating);
			ModulNamesId = ModulNameId;
			ExternalIsolationValveOptionDatabase = ExternalIsolationValveOption;
			TestIdDatabase = TestId;
			resCode = DBPutRecord (hstmt);
			if (resCode != DB_SUCCESS) ShowError();
			resCode = DBDeactivateMap (hmap);
			if (resCode != DB_SUCCESS) ShowError();
		}
		//Test Version finden um die Id zurück geben zu können
		hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion");
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
					*pTestVersionId = TestVersionId;
					break;
				}
			}
		}
	}
	resCode = DBDeactivateSQL (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	return ErrorState; 
}



//Test Hardware Anforderungen definieren
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionValveSerieTable(SINT32 TestVersionId, SINT32 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_32BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 ValveSerieId, TestVersionIdDatabase, ValveSeriesIdDatabase;
	long TestVersionIdStat, ValveSeriesIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex32BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_32BIT; EnumIndex++)
	{
		//Wenn die Valve Serie unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Valve Serie Id zurück
			ValveSerieId = GetValveSerieId(EnumIndex);
			if(ValveSerieId < 0)	
			{
				MessagePopup("Database Errror", "Valve Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ValveSerie");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &ValveSeriesIdDatabase, &ValveSeriesIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(ValveSeriesIdDatabase == ValveSerieId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				if(FoundEntry == FALSE)
				{
					//keinen Verlinkung -> Eintrag erstellen
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "ValveSerie_Id", &ValveSeriesIdDatabase, &ValveSeriesIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2ValveSerie");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					ValveSeriesIdDatabase = ValveSerieId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Valve Serie Id zurück
			ValveSerieId = GetValveSerieId(EnumIndex);
			if(ValveSerieId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "ValveSerie_Id", &ValveSeriesIdDatabase, &ValveSeriesIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2ValveSerie");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && ValveSeriesIdDatabase == ValveSerieId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionControllerTypeTable(SINT32 TestVersionId, SINT32 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_32BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 ControllerId, TestVersionIdDatabase, ControllerTypeIdDatabase;
	long TestVersionIdStat, ControllerTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex32BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_32BIT; EnumIndex++)
	{
		//Wenn die Controller unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Controller Id zurück
			ControllerId = GetControllerTypeId(EnumIndex);
			if(ControllerId < 0)	
			{
				MessagePopup("Database Errror", "Controller Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerType");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &ControllerTypeIdDatabase, &ControllerTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(ControllerTypeIdDatabase == ControllerId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "ControllerType_Id", &ControllerTypeIdDatabase, &ControllerTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2ControllerType");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					ControllerTypeIdDatabase = ControllerId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Controller Id zurück
			ControllerId = GetControllerTypeId(EnumIndex);
			if(ControllerId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "ControllerType_Id", &ControllerTypeIdDatabase, &ControllerTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2ControllerType");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && ControllerTypeIdDatabase == ControllerId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
} 

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionInterfaceTypeTable(SINT32 TestVersionId, SINT32 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_32BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 InterfaceId, TestVersionIdDatabase, InterfaceTypeIdDatabase;
	long TestVersionIdStat, InterfaceTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex32BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_32BIT; EnumIndex++)
	{
		//Wenn die InterfaceType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die InterfaceType Id zurück
			InterfaceId = GetInterfaceTypeId(EnumIndex);
			if(InterfaceId < 0)	
			{
				MessagePopup("Database Errror", "InterfaceType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceType");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &InterfaceTypeIdDatabase, &InterfaceTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(InterfaceTypeIdDatabase == InterfaceId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "InterfaceType_Id", &InterfaceTypeIdDatabase, &InterfaceTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2InterfaceType");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					InterfaceTypeIdDatabase = InterfaceId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die InterfaceType Id zurück
			InterfaceId = GetInterfaceTypeId(EnumIndex);
			if(InterfaceId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "InterfaceType_Id", &InterfaceTypeIdDatabase, &InterfaceTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2InterfaceType");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && InterfaceTypeIdDatabase == InterfaceId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionSingleOptionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 OptionId, TestVersionIdDatabase, OptionTypeIdDatabase;
	long TestVersionIdStat, OptionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die OptionType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die OptionType Id zurück
			OptionId = GetOptionTypeId(EnumIndex);
			if(OptionId < 0)	
			{
				MessagePopup("Database Errror", "SingleOptionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2SingleOptionType");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &OptionTypeIdDatabase, &OptionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(OptionTypeIdDatabase == OptionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "SingleOptionType_Id", &OptionTypeIdDatabase, &OptionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2SingleOptionType");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					OptionTypeIdDatabase = OptionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die OptionType Id zurück
			OptionId = GetOptionTypeId(EnumIndex);
			if(OptionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "SingleOptionType_Id", &OptionTypeIdDatabase, &OptionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2SingleOptionType");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && OptionTypeIdDatabase == OptionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionTestHardwareTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 TestHardwareId, TestVersionIdDatabase, TestHardwareTypeIdDatabase;
	long TestVersionIdStat, TestHardwareTypeIdStat;
	
	ConnectDatabase();

	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Test Hardware Type unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die TestHardwareType Id zurück
			TestHardwareId = GetTestHardwareTypeId(EnumIndex);
			if(TestHardwareId < 0)	
			{
				MessagePopup("Database Errror", "TestHardwareType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2TestHardwareType");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &TestHardwareTypeIdDatabase, &TestHardwareTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(TestHardwareTypeIdDatabase == TestHardwareId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "TestHardwareType_Id", &TestHardwareTypeIdDatabase, &TestHardwareTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2TestHardwareType");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					TestHardwareTypeIdDatabase = TestHardwareId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die TestHardwareType Id zurück
			TestHardwareId = GetTestHardwareTypeId(EnumIndex);
			if(TestHardwareId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "TestHardwareType_Id", &TestHardwareTypeIdDatabase, &TestHardwareTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2TestHardwareType");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && TestHardwareTypeIdDatabase == TestHardwareId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionControllerHardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 ControllerHardwareVersionId, TestVersionIdDatabase, ControllerHardwareVersionTypeIdDatabase;
	long TestVersionIdStat, ControllerHardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die ControllerHardwareVersionType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die ControllerHardwareVersionType Id zurück
			ControllerHardwareVersionId = GetControllerHardwareVersionTypeId(EnumIndex);
			if(ControllerHardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "ControllerHardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerHardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &ControllerHardwareVersionTypeIdDatabase, &ControllerHardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(ControllerHardwareVersionTypeIdDatabase == ControllerHardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "ControllerHardwareVersion_Id", &ControllerHardwareVersionTypeIdDatabase, &ControllerHardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2ControllerHardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					ControllerHardwareVersionTypeIdDatabase = ControllerHardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die ControllerHardwareVersionType Id zurück
			ControllerHardwareVersionId = GetControllerHardwareVersionTypeId(EnumIndex);
			if(ControllerHardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "ControllerHardwareVersion_Id", &ControllerHardwareVersionTypeIdDatabase, &ControllerHardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2ControllerHardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && ControllerHardwareVersionTypeIdDatabase == ControllerHardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionInterfaceHardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 InterfaceHardwareVersionId, TestVersionIdDatabase, InterfaceHardwareVersionTypeIdDatabase;
	long TestVersionIdStat, InterfaceHardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die InterfaceHardwareVersionType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die InterfaceHardwareVersionType Id zurück
			InterfaceHardwareVersionId = GetInterfaceHardwareVersionTypeId(EnumIndex);
			if(InterfaceHardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "InterfaceHardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceHardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &InterfaceHardwareVersionTypeIdDatabase, &InterfaceHardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(InterfaceHardwareVersionTypeIdDatabase == InterfaceHardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "InterfaceHardwareVersion_Id", &InterfaceHardwareVersionTypeIdDatabase, &InterfaceHardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2InterfaceHardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					InterfaceHardwareVersionTypeIdDatabase = InterfaceHardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die InterfaceHardwareVersionType Id zurück
			InterfaceHardwareVersionId = GetInterfaceHardwareVersionTypeId(EnumIndex);
			if(InterfaceHardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "InterfaceHardwareVersion_Id", &InterfaceHardwareVersionTypeIdDatabase, &InterfaceHardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2InterfaceHardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && InterfaceHardwareVersionTypeIdDatabase == InterfaceHardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionControllerAssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 ControllerAssemblyVariantId, TestVersionIdDatabase, ControllerAssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, ControllerAssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die ControllerAssemblyVariantType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die ControllerAssemblyVariantType Id zurück
			ControllerAssemblyVariantId = GetControllerAssemblyVariantTypeId(EnumIndex);
			if(ControllerAssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "ControllerAssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2ControllerAssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &ControllerAssemblyVariantTypeIdDatabase, &ControllerAssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(ControllerAssemblyVariantTypeIdDatabase == ControllerAssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "ControllerAssemblyVariant_Id", &ControllerAssemblyVariantTypeIdDatabase, &ControllerAssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2ControllerAssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					ControllerAssemblyVariantTypeIdDatabase = ControllerAssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die ControllerAssemblyVariantType Id zurück
			ControllerAssemblyVariantId = GetControllerAssemblyVariantTypeId(EnumIndex);
			if(ControllerAssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "ControllerAssemblyVariant_Id", &ControllerAssemblyVariantTypeIdDatabase, &ControllerAssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2ControllerAssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && ControllerAssemblyVariantTypeIdDatabase == ControllerAssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionInterfaceAssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 InterfaceAssemblyVariantId, TestVersionIdDatabase, InterfaceAssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, InterfaceAssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die InterfaceAssemblyVariant unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die InterfaceAssemblyVariant Id zurück
			InterfaceAssemblyVariantId = GetInterfaceAssemblyVariantTypeId(EnumIndex);
			if(InterfaceAssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "InterfaceAssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2InterfaceAssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &InterfaceAssemblyVariantTypeIdDatabase, &InterfaceAssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(InterfaceAssemblyVariantTypeIdDatabase == InterfaceAssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "InterfaceAssemblyVariant_Id", &InterfaceAssemblyVariantTypeIdDatabase, &InterfaceAssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2InterfaceAssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					InterfaceAssemblyVariantTypeIdDatabase = InterfaceAssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die InterfaceAssemblyVariant Id zurück
			InterfaceAssemblyVariantId = GetInterfaceAssemblyVariantTypeId(EnumIndex);
			if(InterfaceAssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "InterfaceAssemblyVariant_Id", &InterfaceAssemblyVariantTypeIdDatabase, &InterfaceAssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2InterfaceAssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && InterfaceAssemblyVariantTypeIdDatabase == InterfaceAssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule1HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module1HardwareVersionId, TestVersionIdDatabase, Module1HardwareVersionTypeIdDatabase;
	long TestVersionIdStat, Module1HardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module1HardwareVersion unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module1HardwareVersion Id zurück
			Module1HardwareVersionId = GetModule1HardwareVersionTypeId(EnumIndex);
			if(Module1HardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "Module1HardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module1HardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module1HardwareVersionTypeIdDatabase, &Module1HardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module1HardwareVersionTypeIdDatabase == Module1HardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module1HardwareVersion_Id", &Module1HardwareVersionTypeIdDatabase, &Module1HardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module1HardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module1HardwareVersionTypeIdDatabase = Module1HardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module1HardwareVersion Id zurück
			Module1HardwareVersionId = GetModule1HardwareVersionTypeId(EnumIndex);
			if(Module1HardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module1HardwareVersion_Id", &Module1HardwareVersionTypeIdDatabase, &Module1HardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module1HardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module1HardwareVersionTypeIdDatabase == Module1HardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule2HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module2HardwareVersionId, TestVersionIdDatabase, Module2HardwareVersionTypeIdDatabase;
	long TestVersionIdStat, Module2HardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module2HardwareVersion unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module2HardwareVersion Id zurück
			Module2HardwareVersionId = GetModule2HardwareVersionTypeId(EnumIndex);
			if(Module2HardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "Module2HardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}

			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module2HardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module2HardwareVersionTypeIdDatabase, &Module2HardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module2HardwareVersionTypeIdDatabase == Module2HardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module2HardwareVersion_Id", &Module2HardwareVersionTypeIdDatabase, &Module2HardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module2HardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module2HardwareVersionTypeIdDatabase = Module2HardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module2HardwareVersion Id zurück
			Module2HardwareVersionId = GetModule2HardwareVersionTypeId(EnumIndex);
			if(Module2HardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module2HardwareVersion_Id", &Module2HardwareVersionTypeIdDatabase, &Module2HardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module2HardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module2HardwareVersionTypeIdDatabase == Module2HardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule3HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module3HardwareVersionId, TestVersionIdDatabase, Module3HardwareVersionTypeIdDatabase;
	long TestVersionIdStat, Module3HardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module3HardwareVersionType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module3HardwareVersionType Id zurück
			Module3HardwareVersionId = GetModule3HardwareVersionTypeId(EnumIndex);
			if(Module3HardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "Module3HardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module3HardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module3HardwareVersionTypeIdDatabase, &Module3HardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module3HardwareVersionTypeIdDatabase == Module3HardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module3HardwareVersion_Id", &Module3HardwareVersionTypeIdDatabase, &Module3HardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module3HardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module3HardwareVersionTypeIdDatabase = Module3HardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module3HardwareVersionType Id zurück
			Module3HardwareVersionId = GetModule3HardwareVersionTypeId(EnumIndex);
			if(Module3HardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module3HardwareVersion_Id", &Module3HardwareVersionTypeIdDatabase, &Module3HardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module3HardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module3HardwareVersionTypeIdDatabase == Module3HardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule4HardwareVersionTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module4HardwareVersionId, TestVersionIdDatabase, Module4HardwareVersionTypeIdDatabase;
	long TestVersionIdStat, Module4HardwareVersionTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module4HardwareVersionType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module4HardwareVersionType Id zurück
			Module4HardwareVersionId = GetModule4HardwareVersionTypeId(EnumIndex);
			if(Module4HardwareVersionId < 0)	
			{
				MessagePopup("Database Errror", "Module4HardwareVersionType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module4HardwareVersion");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module4HardwareVersionTypeIdDatabase, &Module4HardwareVersionTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module4HardwareVersionTypeIdDatabase == Module4HardwareVersionId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module4HardwareVersion_Id", &Module4HardwareVersionTypeIdDatabase, &Module4HardwareVersionTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module4HardwareVersion");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module4HardwareVersionTypeIdDatabase = Module4HardwareVersionId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module4HardwareVersionType Id zurück
			Module4HardwareVersionId = GetModule4HardwareVersionTypeId(EnumIndex);
			if(Module4HardwareVersionId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module4HardwareVersion_Id", &Module4HardwareVersionTypeIdDatabase, &Module4HardwareVersionTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module4HardwareVersion");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module4HardwareVersionTypeIdDatabase == Module4HardwareVersionId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule1AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module1AssemblyVariantId, TestVersionIdDatabase, Module1AssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, Module1AssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module1AssemblyVariantType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module1AssemblyVariantType Id zurück
			Module1AssemblyVariantId = GetModule1AssemblyVariantTypeId(EnumIndex);
			if(Module1AssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "Module1AssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}

			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module1AssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module1AssemblyVariantTypeIdDatabase, &Module1AssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module1AssemblyVariantTypeIdDatabase == Module1AssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module1AssemblyVariant_Id", &Module1AssemblyVariantTypeIdDatabase, &Module1AssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module1AssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module1AssemblyVariantTypeIdDatabase = Module1AssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module1AssemblyVariantType Id zurück
			Module1AssemblyVariantId = GetModule1AssemblyVariantTypeId(EnumIndex);
			if(Module1AssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module1AssemblyVariant_Id", &Module1AssemblyVariantTypeIdDatabase, &Module1AssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module1AssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module1AssemblyVariantTypeIdDatabase == Module1AssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule2AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module2AssemblyVariantId, TestVersionIdDatabase, Module2AssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, Module2AssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module2AssemblyVariantType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module2AssemblyVariantType Id zurück
			Module2AssemblyVariantId = GetModule2AssemblyVariantTypeId(EnumIndex);
			if(Module2AssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "Module2AssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module2AssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module2AssemblyVariantTypeIdDatabase, &Module2AssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module2AssemblyVariantTypeIdDatabase == Module2AssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module2AssemblyVariant_Id", &Module2AssemblyVariantTypeIdDatabase, &Module2AssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module2AssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module2AssemblyVariantTypeIdDatabase = Module2AssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module2AssemblyVariantType Id zurück
			Module2AssemblyVariantId = GetModule2AssemblyVariantTypeId(EnumIndex);
			if(Module2AssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module2AssemblyVariant_Id", &Module2AssemblyVariantTypeIdDatabase, &Module2AssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module2AssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module2AssemblyVariantTypeIdDatabase == Module2AssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule3AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module3AssemblyVariantId, TestVersionIdDatabase, Module3AssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, Module3AssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module3AssemblyVariantType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module3AssemblyVariantType Id zurück
			Module3AssemblyVariantId = GetModule3AssemblyVariantTypeId(EnumIndex);
			if(Module3AssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "Module3AssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module3AssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module3AssemblyVariantTypeIdDatabase, &Module3AssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module3AssemblyVariantTypeIdDatabase == Module3AssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module3AssemblyVariant_Id", &Module3AssemblyVariantTypeIdDatabase, &Module3AssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module3AssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module3AssemblyVariantTypeIdDatabase = Module3AssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module3AssemblyVariantType Id zurück
			Module3AssemblyVariantId = GetModule3AssemblyVariantTypeId(EnumIndex);
			if(Module3AssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module3AssemblyVariant_Id", &Module3AssemblyVariantTypeIdDatabase, &Module3AssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module3AssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module3AssemblyVariantTypeIdDatabase == Module3AssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule4AssemblyVariantTypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module4AssemblyVariantId, TestVersionIdDatabase, Module4AssemblyVariantTypeIdDatabase;
	long TestVersionIdStat, Module4AssemblyVariantTypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module4AssemblyVariantType unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module4AssemblyVariantType Id zurück
			Module4AssemblyVariantId = GetModule4AssemblyVariantTypeId(EnumIndex);
			if(Module4AssemblyVariantId < 0)	
			{
				MessagePopup("Database Errror", "Module4AssemblyVariantType Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module4AssemblyVariant");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module4AssemblyVariantTypeIdDatabase, &Module4AssemblyVariantTypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module4AssemblyVariantTypeIdDatabase == Module4AssemblyVariantId)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module4AssemblyVariant_Id", &Module4AssemblyVariantTypeIdDatabase, &Module4AssemblyVariantTypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module4AssemblyVariant");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module4AssemblyVariantTypeIdDatabase = Module4AssemblyVariantId;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
				if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module4AssemblyVariantType Id zurück
			Module4AssemblyVariantId = GetModule4AssemblyVariantTypeId(EnumIndex);
			if(Module4AssemblyVariantId >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module4AssemblyVariant_Id", &Module4AssemblyVariantTypeIdDatabase, &Module4AssemblyVariantTypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module4AssemblyVariant");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module4AssemblyVariantTypeIdDatabase == Module4AssemblyVariantId)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule1TypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module1Id, TestVersionIdDatabase, Module1TypeIdDatabase;
	long TestVersionIdStat, Module1TypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module1Type unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module1Type Id zurück
			Module1Id = GetModule1TypeId(EnumIndex);
			if(Module1Id < 0)	
			{
				MessagePopup("Database Errror", "Module1Type Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module1Type");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module1TypeIdDatabase, &Module1TypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module1TypeIdDatabase == Module1Id)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module1Type_Id", &Module1TypeIdDatabase, &Module1TypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module1Type");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module1TypeIdDatabase = Module1Id;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module1Type Id zurück
			Module1Id = GetModule1TypeId(EnumIndex);
			if(Module1Id >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module1Type_Id", &Module1TypeIdDatabase, &Module1TypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module1Type");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module1TypeIdDatabase == Module1Id)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule2TypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module2Id, TestVersionIdDatabase, Module2TypeIdDatabase;
	long TestVersionIdStat, Module2TypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module2Type unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module2Type Id zurück
			Module2Id = GetModule2TypeId(EnumIndex);
			if(Module2Id < 0)	
			{
				MessagePopup("Database Errror", "Module2Type Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module2Type");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module2TypeIdDatabase, &Module2TypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module2TypeIdDatabase == Module2Id)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module2Type_Id", &Module2TypeIdDatabase, &Module2TypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module2Type");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module2TypeIdDatabase = Module2Id;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module2Type Id zurück
			Module2Id = GetModule2TypeId(EnumIndex);
			if(Module2Id >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module2Type_Id", &Module2TypeIdDatabase, &Module2TypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module2Type");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module2TypeIdDatabase == Module2Id)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule3TypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module3Id, TestVersionIdDatabase, Module3TypeIdDatabase;
	long TestVersionIdStat, Module3TypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module3Type unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module3Type Id zurück
			Module3Id = GetModule3TypeId(EnumIndex);
			if(Module3Id < 0)	
			{
				MessagePopup("Database Errror", "Module3Type Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module3Type");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module3TypeIdDatabase, &Module3TypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module3TypeIdDatabase == Module3Id)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module3Type_Id", &Module3TypeIdDatabase, &Module3TypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module3Type");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module3TypeIdDatabase = Module3Id;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module3Type Id zurück
			Module3Id = GetModule3TypeId(EnumIndex);
			if(Module3Id >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module3Type_Id", &Module3TypeIdDatabase, &Module3TypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module3Type");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module3TypeIdDatabase == Module3Id)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert die generellen Test Hardware Anforderungen 
//										Wenn der Type noch nicht definiert ist, wird einer Erstellt oder angepasst
//  Input           : TestVersionId:	Id der Test Version
//									:	EnumIndex:			Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Ob ein Fehler aufgetreten ist
//******************************************************************************************************************************
BOOL8 WriteTestVersionModule4TypeTable(SINT32 TestVersionId, SINT16 EnumValue)
{
	BOOL8 EnumIndexArray[ENUM_INDEX_LENGTH_16BIT], FoundEntry = FALSE;
	SINT16 EnumIndex;
	SINT32 Module4Id, TestVersionIdDatabase, Module4TypeIdDatabase;
	long TestVersionIdStat, Module4TypeIdStat;
	
	ConnectDatabase();
	
	GetEnumIndex16BitArray(EnumValue, EnumIndexArray);
	
	for(EnumIndex = 0; EnumIndex < ENUM_INDEX_LENGTH_16BIT; EnumIndex++)
	{
		//Wenn die Module4Type unterstützt wird
		if(EnumIndexArray[EnumIndex] == TRUE)
		{
			//Liefert die Module4Type Id zurück
			Module4Id = GetModule4TypeId(EnumIndex);
			if(Module4Id < 0)	
			{
				MessagePopup("Database Errror", "Module4Type Enum Index konnte nicht in der Datenbank gefunden werden");
				return ErrorState;
			}
	
			//Type mit TestVersion verknüpfen (wenn nicht vorhanden)
			hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestVersion2Module4Type");
			if (hstmt > 0)
			{
				resCode = DBBindColInt(hstmt, 1, &TestVersionIdDatabase, &TestVersionIdStat);
				resCode = DBBindColInt(hstmt, 2, &Module4TypeIdDatabase, &Module4TypeIdStat);
				if (resCode != DB_SUCCESS) ShowError();
				while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
				{
					if (TestVersionIdDatabase == TestVersionId)
					{
						if(Module4TypeIdDatabase == Module4Id)
						{
							//Eintrag schon vorhanden -> Abbrechen
							resCode = DBDeactivateSQL (hstmt);
							if (resCode != DB_SUCCESS) ShowError();
							FoundEntry = TRUE;
							break;
						}
					}
				}
				//keinen Verlinkung -> Eintrag erstellen
				if(FoundEntry == FALSE)
				{
					resCode = DBDeactivateSQL (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					hmap = DBBeginMap (hdbc);
					if (hmap <= 0) ShowError();
					resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
					resCode = DBMapColumnToInt (hmap, "Module4Type_Id", &Module4TypeIdDatabase, &Module4TypeIdStat); 
					if (resCode != DB_SUCCESS) ShowError();
					hstmt = DBActivateMap (hmap, "TestVersion2Module4Type");
					if (resCode != DB_SUCCESS) ShowError(); 
					resCode = DBCreateRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError(); 
					TestVersionIdDatabase = TestVersionId;
					Module4TypeIdDatabase = Module4Id;
					resCode = DBPutRecord (hstmt);
					if (resCode != DB_SUCCESS) ShowError();
					resCode = DBDeactivateMap (hmap);
					if (resCode != DB_SUCCESS) ShowError();
				} 
				else FoundEntry = FALSE;	
			}
			if(resCode != DB_SUCCESS)	break;
		}
		//Dieser Eintrag darf nicht in der Datenbank vorhanden sein
		else
		{
			//Liefert die Module4Type Id zurück
			Module4Id = GetModule4TypeId(EnumIndex);
			if(Module4Id >= 0)	
			{
				hmap = DBBeginMap (hdbc);
			  if (hmap <= 0) ShowError();
				resCode = DBMapColumnToInt (hmap, "TestVersion_Id", &TestVersionIdDatabase, &TestVersionIdStat); 
				resCode = DBMapColumnToInt (hmap, "Module4Type_Id", &Module4TypeIdDatabase, &Module4TypeIdStat); 
			  if (hmap <= 0) ShowError();
			  hstmt = DBActivateMap (hmap, "TestVersion2Module4Type");
			  if (hmap <= 0) ShowError();

			  //Wenn der Eintrag gefunden werden kann, soll dieser gelöscht werden 
			  while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS) 
				{
			  	if (TestVersionIdDatabase == TestVersionId && Module4TypeIdDatabase == Module4TypeIdDatabase)	 
					{
						resCode = DBDeleteRecord(hstmt);
						break;
					}
			  }
			  resCode = DBDeactivateMap (hmap);
			  if (hmap <= 0) ShowError();
			}
		}
	}
	return ErrorState;
}














/*interne Funktionen*/
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Baut eine Verbindung zur Datenbank auf 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void ConnectDatabase(void)
{
	if(IsDatabaseConnectionOpen == FALSE)	
	{
		hdbc = DBConnect (ConnectionString);
		if(hdbc >= 0)	IsDatabaseConnectionOpen = TRUE;
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
static SINT32 GetTestVersionID(SINT32 TestId, SINT32 TestVersion)
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
static SINT32 GetTestIdFromTestInformation(char *pName)
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

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen ModulNames Eintrag, wenn dieser nicht exisitiert
//  Input           : ModulNameTest: Modul Name des Tests
//  Output *Pointer : -
//         *return  : Gibt die ModulNameId zurück
//******************************************************************************************************************************
static SINT32 WriteModulNameTable(char *pModulName)
{
	UINT16 ModulNameLength = MODULNAMES_NAME_LENGTH;
	SINT32 ModulNameId = -1;
	long ModulNameIdStat, ModulNameStat;
	char ModulName[MODULNAMES_NAME_LENGTH];
	
	ConnectDatabase();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ModulNameId, &ModulNameIdStat);
		resCode = DBBindColChar (hstmt, 2, ModulNameLength, ModulName, &ModulNameStat, "");
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(ModulName, pModulName) == 0)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ModulNameId;
			}
		}
		resCode = DBDeactivateSQL (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	
	//Keinen Eintrag vorhanden, so soll dieser erstellt werden
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToChar (hmap, "Enum", ModulNameLength, ModulName, &ModulNameStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ModulName");
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBCreateRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError(); 
	strcpy(ModulName, pModulName);
	resCode = DBPutRecord (hstmt);
	if (resCode != DB_SUCCESS) ShowError();
	resCode = DBDeactivateMap (hmap);
	if (resCode != DB_SUCCESS) ShowError();
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ModulName"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ModulNameId, &ModulNameIdStat);
		resCode = DBBindColChar (hstmt, 2, ModulNameLength, ModulName, &ModulNameStat, "");
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (StrICmpWithoutSurrWhiteSpace(ModulName, pModulName) == 0)	
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ModulNameId;
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
//  Description     : Schreibt einen neuen ValveSerie Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteValveSerieTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum)
{	
	BOOL8 FindEntry = FALSE;
	UINT16 ValveSeriesEnumLength = VALVESERIES_VALVESERIEENUM_LENGTH;
	SINT16 ValveSerieEnumIndex;
	SINT32 ValveSerieEnumValue;
	long ValveSerieEnumIndexStat, ValveSerieEnumStat, ValveSerieEnumValueStat;
	char ValveSerieEnum[VALVESERIES_VALVESERIEENUM_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &ValveSerieEnumIndex, &ValveSerieEnumIndexStat); 
	resCode = DBMapColumnToInt (hmap, "EnumValue", &ValveSerieEnumValue, &ValveSerieEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", ValveSeriesEnumLength, ValveSerieEnum, &ValveSerieEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ValveSerie");
	if (resCode != DB_SUCCESS) ShowError();
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == ValveSerieEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(ValveSerieEnum, pEnum) != 0 || EnumValue != ValveSerieEnumValue)
			{
				strcpy(ValveSerieEnum, pEnum);
				ValveSerieEnumValue = EnumValue; 
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		ValveSerieEnumIndex = EnumIndex;
		ValveSerieEnumValue = EnumValue;
		strcpy(ValveSerieEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen ControllerType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteControllerTypeTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 ControllerTypesEnumLength = CONTROLLERTYPES_NAME_LENGTH;
	SINT16 ControllerTypesEnumIndex;
	SINT32 ControllerTypesEnumValue;
	long ControllerTypesEnumStat, ControllerTypesEnumValueStat, ControllerTypesEnumIndexStat;
	char ControllerTypesEnum[CONTROLLERTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &ControllerTypesEnumIndex, &ControllerTypesEnumIndexStat); 
	resCode = DBMapColumnToInt (hmap, "EnumValue", &ControllerTypesEnumValue, &ControllerTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", ControllerTypesEnumLength, ControllerTypesEnum, &ControllerTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ControllerType");
	if (resCode != DB_SUCCESS) ShowError();
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == ControllerTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(ControllerTypesEnum, pEnum) != 0 || EnumValue != ControllerTypesEnumValue)
			{
				strcpy(ControllerTypesEnum, pEnum);
				ControllerTypesEnumValue = EnumValue; 
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		ControllerTypesEnumIndex = EnumIndex;
		ControllerTypesEnumValue = EnumValue;
		strcpy(ControllerTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen InterfaceType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteInterfaceTypeTable(SINT16 EnumIndex, SINT32 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 InterfaceTypesEnumLength = INTERFACETYPES_NAME_LENGTH;
	SINT16 InterfaceTypesEnumIndex;
	SINT32 InterfaceTypesEnumValue;
	long InterfaceTypesNumberStat, InterfaceTypesEnumStat, InterfaceTypesEnumValueStat;
	char InterfaceTypesEnum[INTERFACETYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &InterfaceTypesEnumIndex, &InterfaceTypesNumberStat);
	resCode = DBMapColumnToInt (hmap, "EnumValue", &InterfaceTypesEnumValue, &InterfaceTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", InterfaceTypesEnumLength, InterfaceTypesEnum, &InterfaceTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "InterfaceType");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == InterfaceTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(InterfaceTypesEnum, pEnum) != 0 || EnumValue != InterfaceTypesEnumValue)
			{
				strcpy(InterfaceTypesEnum, pEnum);
				InterfaceTypesEnumValue = EnumValue;
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		InterfaceTypesEnumIndex = EnumIndex;
		InterfaceTypesEnumValue = EnumValue;
		strcpy(InterfaceTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen SingleOptionType Eintrag, wenn dieser nicht exisitiert (nur eine Option pro Wert)
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteSingleOptionTypeTable(SINT16 EnumIndex, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 OptionTypesEnumLength = OPTIONTYPES_NAME_LENGTH;
	SINT16 OptionTypesNumberEnumIndex;
	long OptionTypesEnumIndexStat, OptionTypesEnumStat;
	char OptionTypesEnum[OPTIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &OptionTypesNumberEnumIndex, &OptionTypesEnumIndexStat);
	resCode = DBMapColumnToChar (hmap, "Enum", OptionTypesEnumLength, OptionTypesEnum, &OptionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "SingleOptionType");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == OptionTypesNumberEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(OptionTypesEnum, pEnum) != 0)
			{
				strcpy(OptionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		OptionTypesNumberEnumIndex = EnumIndex;
		strcpy(OptionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen OptionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteOptionTypeTable(SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 OptionTypesEnumLength = OPTIONTYPES_NAME_LENGTH;
	SINT16 OptionTypesEnumValue;
	long OptionTypesEnumStat, OptionTypesEnumValueStat;
	char OptionTypesEnum[OPTIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumValue", &OptionTypesEnumValue, &OptionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", OptionTypesEnumLength, OptionTypesEnum, &OptionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "OptionType");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumValue == OptionTypesEnumValue)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(OptionTypesEnum, pEnum) != 0)
			{
				strcpy(OptionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		OptionTypesEnumValue = EnumValue;
		strcpy(OptionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen TestHardwareType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteTestHardwareTypeTable(SINT16 EnumIndex, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 TestHardwareTypesEnumLength = OPTIONTYPES_NAME_LENGTH;
	SINT16 TestHardwareTypesEnumIndex;
	long TestHardwareTypesEnumIndexStat, TestHardwareTypesEnumStat;
	char TestHardwareTypesEnum[OPTIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &TestHardwareTypesEnumIndex, &TestHardwareTypesEnumIndexStat);
	resCode = DBMapColumnToChar (hmap, "Enum", TestHardwareTypesEnumLength, TestHardwareTypesEnum, &TestHardwareTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "TestHardwareType");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == TestHardwareTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(TestHardwareTypesEnum, pEnum) != 0)
			{
				strcpy(TestHardwareTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		TestHardwareTypesEnumIndex = EnumIndex;
		strcpy(TestHardwareTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen ControllerHardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteControllerHardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 ControllerHardwareVersionTypesEnumLength = CONTROLLERHARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 ControllerHardwareVersionTypesEnumIndex, ControllerHardwareVersionTypesEnumValue;
	long ControllerHardwareVersionTypesEnumIndexStat, ControllerHardwareVersionTypesEnumStat, ControllerHardwareVersionTypesEnumValueStat;
	char ControllerHardwareVersionTypesEnum[CONTROLLERHARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &ControllerHardwareVersionTypesEnumIndex, &ControllerHardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &ControllerHardwareVersionTypesEnumValue, &ControllerHardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", ControllerHardwareVersionTypesEnumLength, ControllerHardwareVersionTypesEnum, &ControllerHardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ControllerHardwareVersion");
	if (resCode != DB_SUCCESS) ShowError();
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == ControllerHardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(ControllerHardwareVersionTypesEnum, pEnum) != 0 || EnumValue != ControllerHardwareVersionTypesEnumValue)
			{
				ControllerHardwareVersionTypesEnumValue = EnumValue;
				strcpy(ControllerHardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		ControllerHardwareVersionTypesEnumIndex = EnumIndex;
		ControllerHardwareVersionTypesEnumValue = EnumValue;
		strcpy(ControllerHardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen InterfaceHardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteInterfaceHardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 InterfaceHardwareVersionTypesEnumLength = INTERAFCEHARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 InterfaceHardwareVersionTypesEnumIndex, InterfaceHardwareVersionTypesEnumValue;
	long InterfaceHardwareVersionTypesEnumIndexStat, InterfaceHardwareVersionTypesEnumStat, InterfaceHardwareVersionTypesEnumValueStat;
	char InterfaceHardwareVersionTypesEnum[INTERAFCEHARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &InterfaceHardwareVersionTypesEnumIndex, &InterfaceHardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &InterfaceHardwareVersionTypesEnumValue, &InterfaceHardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", InterfaceHardwareVersionTypesEnumLength, InterfaceHardwareVersionTypesEnum, &InterfaceHardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "InterfaceHardwareVersion");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == InterfaceHardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(InterfaceHardwareVersionTypesEnum, pEnum) != 0 || EnumValue != InterfaceHardwareVersionTypesEnumValue)
			{
				InterfaceHardwareVersionTypesEnumValue = EnumValue;
				strcpy(InterfaceHardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		InterfaceHardwareVersionTypesEnumIndex = EnumIndex;
		InterfaceHardwareVersionTypesEnumValue = EnumValue;
		strcpy(InterfaceHardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen ControllerAssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteControllerAssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 ControllerAssemblyVariantTypesEnumLength = CONTROLLERASSEMBLYVARIANTSTYPES_NAME_LENGTH;
	SINT16 ControllerAssemblyVariantTypesEnumIndex, ControllerAssemblyVariantTypesEnumValue;
	long ControllerAssemblyVariantTypesEnumIndexStat, ControllerAssemblyVariantTypesEnumStat, ControllerAssemblyVariantTypesEnumValueStat;
	char ControllerAssemblyVariantTypesEnum[CONTROLLERASSEMBLYVARIANTSTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &ControllerAssemblyVariantTypesEnumIndex, &ControllerAssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &ControllerAssemblyVariantTypesEnumValue, &ControllerAssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", ControllerAssemblyVariantTypesEnumLength, ControllerAssemblyVariantTypesEnum, &ControllerAssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "ControllerAssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == ControllerAssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(ControllerAssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != ControllerAssemblyVariantTypesEnumValue)
			{
				ControllerAssemblyVariantTypesEnumValue = EnumValue;
				strcpy(ControllerAssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		ControllerAssemblyVariantTypesEnumIndex = EnumIndex;
		ControllerAssemblyVariantTypesEnumValue = EnumValue;
		strcpy(ControllerAssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen InterfaceAssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteInterfaceAssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 InterfaceAssemblyVariantTypesEnumLength = INTERFACEASSEMBLYVARIANTSTYPES_NAME_LENGTH;
	SINT16 InterfaceAssemblyVariantTypesEnumIndex, InterfaceAssemblyVariantTypesEnumValue;
	long InterfaceAssemblyVariantTypesEnumIndexStat, InterfaceAssemblyVariantTypesEnumStat, InterfaceAssemblyVariantTypesEnumValueStat;
	char InterfaceAssemblyVariantTypesEnum[INTERFACEASSEMBLYVARIANTSTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &InterfaceAssemblyVariantTypesEnumIndex, &InterfaceAssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &InterfaceAssemblyVariantTypesEnumValue, &InterfaceAssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", InterfaceAssemblyVariantTypesEnumLength, InterfaceAssemblyVariantTypesEnum, &InterfaceAssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "InterfaceAssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == InterfaceAssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(InterfaceAssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != InterfaceAssemblyVariantTypesEnumValue)
			{
				InterfaceAssemblyVariantTypesEnumValue = EnumValue;
				strcpy(InterfaceAssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		InterfaceAssemblyVariantTypesEnumIndex = EnumIndex;
		InterfaceAssemblyVariantTypesEnumValue = EnumValue;
		strcpy(InterfaceAssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module1HardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule1HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module1HardwareVersionTypesEnumLength = MODULE1HARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 Module1HardwareVersionTypesEnumIndex, Module1HardwareVersionTypesEnumValue;
	long Module1HardwareVersionTypesEnumIndexStat, Module1HardwareVersionTypesEnumStat, Module1HardwareVersionTypesEnumValueStat;
	char Module1HardwareVersionTypesEnum[MODULE1HARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module1HardwareVersionTypesEnumIndex, &Module1HardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module1HardwareVersionTypesEnumValue, &Module1HardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module1HardwareVersionTypesEnumLength, Module1HardwareVersionTypesEnum, &Module1HardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module1HardwareVersion");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module1HardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module1HardwareVersionTypesEnum, pEnum) != 0 || EnumValue != Module1HardwareVersionTypesEnumValue)
			{
				Module1HardwareVersionTypesEnumValue = EnumValue;
				strcpy(Module1HardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module1HardwareVersionTypesEnumIndex = EnumIndex;
		Module1HardwareVersionTypesEnumValue = EnumValue;
		strcpy(Module1HardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module2HardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule2HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module2HardwareVersionTypesEnumLength = MODULE2HARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 Module2HardwareVersionTypesEnumIndex, Module2HardwareVersionTypesEnumValue;
	long Module2HardwareVersionTypesEnumIndexStat, Module2HardwareVersionTypesEnumStat, Module2HardwareVersionTypesEnumValueStat;
	char Module2HardwareVersionTypesEnum[MODULE2HARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module2HardwareVersionTypesEnumIndex, &Module2HardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module2HardwareVersionTypesEnumValue, &Module2HardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module2HardwareVersionTypesEnumLength, Module2HardwareVersionTypesEnum, &Module2HardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module2HardwareVersion");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module2HardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module2HardwareVersionTypesEnum, pEnum) != 0 || EnumValue != Module2HardwareVersionTypesEnumValue)
			{
				Module2HardwareVersionTypesEnumValue = EnumValue;
				strcpy(Module2HardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module2HardwareVersionTypesEnumIndex = EnumIndex;
		Module2HardwareVersionTypesEnumValue = EnumValue;
		strcpy(Module2HardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module3HardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule3HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module3HardwareVersionTypesEnumLength = MODULE3HARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 Module3HardwareVersionTypesEnumIndex, Module3HardwareVersionTypesEnumValue;
	long Module3HardwareVersionTypesEnumIndexStat, Module3HardwareVersionTypesEnumStat, Module3HardwareVersionTypesEnumValueStat;
	char Module3HardwareVersionTypesEnum[MODULE3HARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module3HardwareVersionTypesEnumIndex, &Module3HardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module3HardwareVersionTypesEnumValue, &Module3HardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module3HardwareVersionTypesEnumLength, Module3HardwareVersionTypesEnum, &Module3HardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module3HardwareVersion");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module3HardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module3HardwareVersionTypesEnum, pEnum) != 0 || EnumValue != Module3HardwareVersionTypesEnumValue)
			{
				Module3HardwareVersionTypesEnumValue = EnumValue;
				strcpy(Module3HardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module3HardwareVersionTypesEnumIndex = EnumIndex;
		Module3HardwareVersionTypesEnumValue = EnumValue;
		strcpy(Module3HardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module4HardwareVersionType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule4HardwareVersionTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module4HardwareVersionTypesEnumLength = MODULE4HARDWAREVERSIONTYPES_NAME_LENGTH;
	SINT16 Module4HardwareVersionTypesEnumIndex, Module4HardwareVersionTypesEnumValue;
	long Module4HardwareVersionTypesEnumIndexStat, Module4HardwareVersionTypesEnumStat, Module4HardwareVersionTypesEnumValueStat;
	char Module4HardwareVersionTypesEnum[MODULE4HARDWAREVERSIONTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module4HardwareVersionTypesEnumIndex, &Module4HardwareVersionTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module4HardwareVersionTypesEnumValue, &Module4HardwareVersionTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module4HardwareVersionTypesEnumLength, Module4HardwareVersionTypesEnum, &Module4HardwareVersionTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module4HardwareVersion");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module4HardwareVersionTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module4HardwareVersionTypesEnum, pEnum) != 0 || EnumValue != Module4HardwareVersionTypesEnumValue)
			{
				Module4HardwareVersionTypesEnumValue = EnumValue;
				strcpy(Module4HardwareVersionTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module4HardwareVersionTypesEnumIndex = EnumIndex;
		Module4HardwareVersionTypesEnumValue = EnumValue;
		strcpy(Module4HardwareVersionTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module1AssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule1AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module1AssemblyVariantTypesEnumLength = MODULE1ASSEMBLYVARIANTTYPES_NAME_LENGTH;
	SINT16 Module1AssemblyVariantTypesEnumIndex, Module1AssemblyVariantTypesEnumValue;
	long Module1AssemblyVariantTypesEnumIndexStat, Module1AssemblyVariantTypesEnumStat, Module1AssemblyVariantTypesEnumValueStat;
	char Module1AssemblyVariantTypesEnum[MODULE1ASSEMBLYVARIANTTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module1AssemblyVariantTypesEnumIndex, &Module1AssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module1AssemblyVariantTypesEnumValue, &Module1AssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module1AssemblyVariantTypesEnumLength, Module1AssemblyVariantTypesEnum, &Module1AssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module1AssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module1AssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module1AssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != Module1AssemblyVariantTypesEnumValue)
			{
				Module1AssemblyVariantTypesEnumValue = EnumValue;
				strcpy(Module1AssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module1AssemblyVariantTypesEnumIndex = EnumIndex;
		Module1AssemblyVariantTypesEnumValue = EnumValue;
		strcpy(Module1AssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module2AssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule2AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module2AssemblyVariantTypesEnumLength = MODULE2ASSEMBLYVARIANTTYPES_NAME_LENGTH;
	SINT16 Module2AssemblyVariantTypesEnumIndex, Module2AssemblyVariantTypesEnumValue;
	long Module2AssemblyVariantTypesEnumIndexStat, Module2AssemblyVariantTypesEnumStat, Module2AssemblyVariantTypesEnumValueStat;
	char Module2AssemblyVariantTypesEnum[MODULE2ASSEMBLYVARIANTTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module2AssemblyVariantTypesEnumIndex, &Module2AssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module2AssemblyVariantTypesEnumValue, &Module2AssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module2AssemblyVariantTypesEnumLength, Module2AssemblyVariantTypesEnum, &Module2AssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module2AssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module2AssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module2AssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != Module2AssemblyVariantTypesEnumValue)
			{
				Module2AssemblyVariantTypesEnumValue = EnumValue;
				strcpy(Module2AssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module2AssemblyVariantTypesEnumIndex = EnumIndex;
		Module2AssemblyVariantTypesEnumValue = EnumValue;
		strcpy(Module2AssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module3AssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule3AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module3AssemblyVariantTypesEnumLength = MODULE3ASSEMBLYVARIANTTYPES_NAME_LENGTH;
	SINT16 Module3AssemblyVariantTypesEnumIndex, Module3AssemblyVariantTypesEnumValue;
	long Module3AssemblyVariantTypesEnumIndexStat, Module3AssemblyVariantTypesEnumStat, Module3AssemblyVariantTypesEnumValueStat;
	char Module3AssemblyVariantTypesEnum[MODULE3ASSEMBLYVARIANTTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module3AssemblyVariantTypesEnumIndex, &Module3AssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module3AssemblyVariantTypesEnumValue, &Module3AssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module3AssemblyVariantTypesEnumLength, Module3AssemblyVariantTypesEnum, &Module3AssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module3AssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module3AssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module3AssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != Module3AssemblyVariantTypesEnumValue)
			{
				Module3AssemblyVariantTypesEnumValue = EnumValue;
				strcpy(Module3AssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module3AssemblyVariantTypesEnumIndex = EnumIndex;
		Module3AssemblyVariantTypesEnumValue = EnumValue;
		strcpy(Module3AssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module4AssemblyVariantType Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule4AssemblyVariantTypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module4AssemblyVariantTypesEnumLength = MODULE4ASSEMBLYVARIANTTYPES_NAME_LENGTH;
	SINT16 Module4AssemblyVariantTypesEnumIndex, Module4AssemblyVariantTypesEnumValue;
	long Module4AssemblyVariantTypesEnumIndexStat, Module4AssemblyVariantTypesEnumStat, Module4AssemblyVariantTypesEnumValueStat;
	char Module4AssemblyVariantTypesEnum[MODULE4ASSEMBLYVARIANTTYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module4AssemblyVariantTypesEnumIndex, &Module4AssemblyVariantTypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module4AssemblyVariantTypesEnumValue, &Module4AssemblyVariantTypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module4AssemblyVariantTypesEnumLength, Module4AssemblyVariantTypesEnum, &Module4AssemblyVariantTypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module4AssemblyVariant");
	if (resCode != DB_SUCCESS) ShowError(); 
	
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module4AssemblyVariantTypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module4AssemblyVariantTypesEnum, pEnum) != 0 || EnumValue != Module4AssemblyVariantTypesEnumValue)
			{
				Module4AssemblyVariantTypesEnumValue = EnumValue;
				strcpy(Module4AssemblyVariantTypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module4AssemblyVariantTypesEnumIndex = EnumIndex;
		Module4AssemblyVariantTypesEnumValue = EnumValue;
		strcpy(Module4AssemblyVariantTypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module1Type Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule1TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module1TypesEnumLength = MODULE1TYPES_NAME_LENGTH;
	SINT16 Module1TypesEnumIndex, Module1TypesEnumValue;
	long Module1TypesEnumIndexStat, Module1TypesEnumStat, Module1TypesEnumValueStat;
	char Module1TypesEnum[MODULE1TYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module1TypesEnumIndex, &Module1TypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module1TypesEnumValue, &Module1TypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module1TypesEnumLength, Module1TypesEnum, &Module1TypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module1Type");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module1TypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module1TypesEnum, pEnum) != 0 || EnumValue != Module1TypesEnumValue)
			{
				Module1TypesEnumValue = EnumValue;
				strcpy(Module1TypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module1TypesEnumIndex = EnumIndex;
		Module1TypesEnumValue = EnumValue;
		strcpy(Module1TypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module2Type Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule2TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module2TypesEnumLength = MODULE2TYPES_NAME_LENGTH;
	SINT16 Module2TypesEnumIndex, Module2TypesEnumValue;
	long Module2TypesEnumIndexStat, Module2TypesEnumStat, Module2TypesEnumValueStat;
	char Module2TypesEnum[MODULE2TYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module2TypesEnumIndex, &Module2TypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module2TypesEnumValue, &Module2TypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module2TypesEnumLength, Module2TypesEnum, &Module2TypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module2Type");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module2TypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module2TypesEnum, pEnum) != 0 || EnumValue != Module2TypesEnumValue)
			{
				Module2TypesEnumValue = EnumValue;
				strcpy(Module2TypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module2TypesEnumIndex = EnumIndex;
		Module2TypesEnumValue = EnumValue;
		strcpy(Module2TypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module3Type Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule3TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module3TypesEnumLength = MODULE3TYPES_NAME_LENGTH;
	SINT16 Module3TypesEnumIndex, Module3TypesEnumValue;
	long Module3TypesEnumIndexStat, Module3TypesEnumStat, Module3TypesEnumValueStat;
	char Module3TypesEnum[MODULE3TYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module3TypesEnumIndex, &Module3TypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module3TypesEnumValue, &Module3TypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module3TypesEnumLength, Module3TypesEnum, &Module3TypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module3Type");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module3TypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module3TypesEnum, pEnum) != 0 || EnumValue != Module3TypesEnumValue)
			{
				Module3TypesEnumValue = EnumValue;
				strcpy(Module3TypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module3TypesEnumIndex = EnumIndex;
		Module3TypesEnumValue = EnumValue;
		strcpy(Module3TypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt einen neuen Module4Type Eintrag, wenn dieser nicht exisitiert
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//										EnumValue: Wert des Enums im Test Hardware Struktur
//										pEnum: Beschriebung des Enum Wertes
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void WriteModule4TypeTable(SINT16 EnumIndex, SINT16 EnumValue, char *pEnum)
{
	BOOL8 FindEntry = FALSE;
	UINT16 Module4TypesEnumLength = MODULE4TYPES_NAME_LENGTH;
	SINT16 Module4TypesEnumIndex, Module4TypesEnumValue;
	long Module4TypesEnumIndexStat, Module4TypesEnumStat, Module4TypesEnumValueStat;
	char Module4TypesEnum[MODULE4TYPES_NAME_LENGTH];
	
	hmap = DBBeginMap (hdbc);
	if (hmap <= 0) ShowError();
	resCode = DBMapColumnToShort (hmap, "EnumIndex", &Module4TypesEnumIndex, &Module4TypesEnumIndexStat);
	resCode = DBMapColumnToShort (hmap, "EnumValue", &Module4TypesEnumValue, &Module4TypesEnumValueStat);
	resCode = DBMapColumnToChar (hmap, "Enum", Module4TypesEnumLength, Module4TypesEnum, &Module4TypesEnumStat, "");
	if (resCode != DB_SUCCESS) ShowError();
	hstmt = DBActivateMap (hmap, "Module4Type");
	if (resCode != DB_SUCCESS) ShowError(); 
	while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
	{
		if (EnumIndex == Module4TypesEnumIndex)
		{
			//prüfen, ob sich das Enum verändert hat
			if (StrICmpWithoutSurrWhiteSpace(Module4TypesEnum, pEnum) != 0 || EnumValue != Module4TypesEnumValue)
			{
				Module4TypesEnumValue = EnumValue;
				strcpy(Module4TypesEnum, pEnum);
				resCode = DBPutRecord (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
			}
			FindEntry = TRUE;
			break;
		}
	}
	if(FindEntry == FALSE)
	{
		//Keinen Eintrag vorhanden, so soll dieser erstellt werden
		resCode = DBCreateRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError(); 
		Module4TypesEnumIndex = EnumIndex;
		Module4TypesEnumValue = EnumValue;
		strcpy(Module4TypesEnum, pEnum);
		resCode = DBPutRecord (hstmt);
		if (resCode != DB_SUCCESS) ShowError();
	}
	resCode = DBDeactivateMap (hmap);
  if (resCode != DB_SUCCESS) ShowError();
}





//Liefert Test Hardware Types ID zurück
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Fragt mit Hilfe des Enum Index die ValveSeriesId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die ValveSeriesId zurück
//******************************************************************************************************************************
static SINT32 GetValveSerieId(SINT16 EnumIndex)
{
	SINT16 ValveSerieEnumIndex;
	SINT32 ValveSerieId = -1;
	long ValveSerieIdStat, ValveSerieEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ValveSerie"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ValveSerieId, &ValveSerieIdStat);
		resCode = DBBindColShort (hstmt, 2, &ValveSerieEnumIndex, &ValveSerieEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ValveSerieEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ValveSerieId;
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
//  Description     : Fragt mit Hilfe des Enum Index die ControllerTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die ControllerTypeId zurück
//******************************************************************************************************************************
static SINT32 GetControllerTypeId(SINT16 EnumIndex)
{
	SINT16 ControllerTypeEnumIndex;
	SINT32 ControllerTypeId = -1;
	long ControllerTypeIdStat, ControllerTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerType"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerTypeId, &ControllerTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &ControllerTypeEnumIndex, &ControllerTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ControllerTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die InterfaceTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die InterfaceTypeId zurück
//******************************************************************************************************************************
static SINT32 GetInterfaceTypeId(SINT16 EnumIndex)
{
	SINT16 InterfaceTypeEnumIndex;
	SINT32 InterfaceTypeId = -1;
	long InterfaceTypeIdStat, InterfaceTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceType"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceTypeId, &InterfaceTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &InterfaceTypeEnumIndex, &InterfaceTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (InterfaceTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die OptionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die OptionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetOptionTypeId(SINT16 EnumIndex)
{
	SINT16 OptionTypeEnumIndex;
	SINT32 OptionTypeId = -1;
	long OptionTypeIdStat, OptionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM OptionType"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &OptionTypeId, &OptionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &OptionTypeEnumIndex, &OptionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (OptionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return OptionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die TestHardwareTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die TestHardwareTypeId zurück
//******************************************************************************************************************************
static SINT32 GetTestHardwareTypeId(SINT16 EnumIndex)
{
	SINT16 TestHardwareTypeEnumIndex;
	SINT32 TestHardwareTypeId = -1;
	long TestHardwareTypeIdStat, TestHardwareTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM TestHardwareType"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &TestHardwareTypeId, &TestHardwareTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &TestHardwareTypeEnumIndex, &TestHardwareTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (TestHardwareTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return TestHardwareTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die ControllerHardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die ControllerHardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetControllerHardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 ControllerHardwareVersionTypeEnumIndex;
	SINT32 ControllerHardwareVersionTypeId = -1;
	long ControllerHardwareVersionTypeIdStat, ControllerHardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerHardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerHardwareVersionTypeId, &ControllerHardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &ControllerHardwareVersionTypeEnumIndex, &ControllerHardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ControllerHardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerHardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die InterfaceHardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die InterfaceHardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetInterfaceHardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 InterfaceHardwareVersionTypeEnumIndex;
	SINT32 InterfaceHardwareVersionTypeId = -1;
	long InterfaceHardwareVersionTypeIdStat, InterfaceHardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceHardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceHardwareVersionTypeId, &InterfaceHardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &InterfaceHardwareVersionTypeEnumIndex, &InterfaceHardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (InterfaceHardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceHardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die ControllerAssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die ControllerAssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetControllerAssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 ControllerAssemblyVariantTypeEnumIndex;
	SINT32 ControllerAssemblyVariantTypeId = -1;
	long ControllerAssemblyVariantTypeIdStat, ControllerAssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM ControllerAssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &ControllerAssemblyVariantTypeId, &ControllerAssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &ControllerAssemblyVariantTypeEnumIndex, &ControllerAssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (ControllerAssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return ControllerAssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die InterfaceAssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die InterfaceAssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetInterfaceAssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 InterfaceAssemblyVariantTypeEnumIndex;
	SINT32 InterfaceAssemblyVariantTypeId = -1;
	long InterfaceAssemblyVariantTypeIdStat, InterfaceAssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM InterfaceAssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &InterfaceAssemblyVariantTypeId, &InterfaceAssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &InterfaceAssemblyVariantTypeEnumIndex, &InterfaceAssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (InterfaceAssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return InterfaceAssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module1HardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module1HardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule1HardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 Module1HardwareVersionTypeEnumIndex;
	SINT32 Module1HardwareVersionTypeId = -1;
	long Module1HardwareVersionTypeIdStat, Module1HardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1HardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1HardwareVersionTypeId, &Module1HardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module1HardwareVersionTypeEnumIndex, &Module1HardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module1HardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1HardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module2HardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module2HardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule2HardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 Module2HardwareVersionTypeEnumIndex;
	SINT32 Module2HardwareVersionTypeId = -1;
	long Module2HardwareVersionTypeIdStat, Module2HardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2HardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2HardwareVersionTypeId, &Module2HardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module2HardwareVersionTypeEnumIndex, &Module2HardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module2HardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2HardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module3HardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module3HardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule3HardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 Module3HardwareVersionTypeEnumIndex;
	SINT32 Module3HardwareVersionTypeId = -1;
	long Module3HardwareVersionTypeIdStat, Module3HardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3HardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3HardwareVersionTypeId, &Module3HardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module3HardwareVersionTypeEnumIndex, &Module3HardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module3HardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3HardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module4HardwareVersionTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module4HardwareVersionTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule4HardwareVersionTypeId(SINT16 EnumIndex)
{
	SINT16 Module4HardwareVersionTypeEnumIndex;
	SINT32 Module4HardwareVersionTypeId = -1;
	long Module4HardwareVersionTypeIdStat, Module4HardwareVersionTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4HardwareVersion"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4HardwareVersionTypeId, &Module4HardwareVersionTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module4HardwareVersionTypeEnumIndex, &Module4HardwareVersionTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module4HardwareVersionTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4HardwareVersionTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module1AssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module1AssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule1AssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 Module1AssemblyVariantTypeEnumIndex;
	SINT32 Module1AssemblyVariantTypeId = -1;
	long Module1AssemblyVariantTypeIdStat, Module1AssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1AssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1AssemblyVariantTypeId, &Module1AssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module1AssemblyVariantTypeEnumIndex, &Module1AssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module1AssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1AssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module2AssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module2AssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule2AssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 Module2AssemblyVariantTypeEnumIndex;
	SINT32 Module2AssemblyVariantTypeId = -1;
	long Module2AssemblyVariantTypeIdStat, Module2AssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2AssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2AssemblyVariantTypeId, &Module2AssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module2AssemblyVariantTypeEnumIndex, &Module2AssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module2AssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2AssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module3AssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module3AssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule3AssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 Module3AssemblyVariantTypeEnumIndex;
	SINT32 Module3AssemblyVariantTypeId = -1;
	long Module3AssemblyVariantTypeIdStat, Module3AssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3AssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3AssemblyVariantTypeId, &Module3AssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module3AssemblyVariantTypeEnumIndex, &Module3AssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module3AssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3AssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module4AssemblyVariantTypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module4AssemblyVariantTypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule4AssemblyVariantTypeId(SINT16 EnumIndex)
{
	SINT16 Module4AssemblyVariantTypeEnumIndex;
	SINT32 Module4AssemblyVariantTypeId = -1;
	long Module4AssemblyVariantTypeIdStat, Module4AssemblyVariantTypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4AssemblyVariant"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4AssemblyVariantTypeId, &Module4AssemblyVariantTypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module4AssemblyVariantTypeEnumIndex, &Module4AssemblyVariantTypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module4AssemblyVariantTypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4AssemblyVariantTypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module1TypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module1TypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule1TypeId(SINT16 EnumIndex)
{
	SINT16 Module1TypeEnumIndex;
	SINT32 Module1TypeId = -1;
	long Module1TypeIdStat, Module1TypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module1Type"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module1TypeId, &Module1TypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module1TypeEnumIndex, &Module1TypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module1TypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module1TypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module2TypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module2TypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule2TypeId(SINT16 EnumIndex)
{
	SINT16 Module2TypeEnumIndex;
	SINT32 Module2TypeId = -1;
	long Module2TypeIdStat, Module2TypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module2Type"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module2TypeId, &Module2TypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module2TypeEnumIndex, &Module2TypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module2TypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module2TypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module3TypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module3TypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule3TypeId(SINT16 EnumIndex)
{
	SINT16 Module3TypeEnumIndex;
	SINT32 Module3TypeId = -1;
	long Module3TypeIdStat, Module3TypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module3Type"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module3TypeId, &Module3TypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module3TypeEnumIndex, &Module3TypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module3TypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module3TypeId;
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
//  Description     : Fragt mit Hilfe des Enum Index die Module4TypeId ab
//  Input           : EnumIndex: Index des Enums im Test Hardware Struktur
//  Output *Pointer : -
//         *return  : Gibt die Module4TypeId zurück
//******************************************************************************************************************************
static SINT32 GetModule4TypeId(SINT16 EnumIndex)
{
	SINT16 Module4TypeEnumIndex;
	SINT32 Module4TypeId = -1;
	long Module4TypeIdStat, Module4TypeEnumIndexStat;
	
	hstmt = DBActivateSQL (hdbc, "SELECT * FROM Module4Type"); 
	if (hstmt > 0)
	{
		resCode = DBBindColInt  (hstmt, 1, &Module4TypeId, &Module4TypeIdStat);
		resCode = DBBindColShort (hstmt, 2, &Module4TypeEnumIndex, &Module4TypeEnumIndexStat);
		//Nun soll seine Id heraus gelesen werden
		while ((resCode = DBFetchNext (hstmt)) == DB_SUCCESS)
		{
			if (Module4TypeEnumIndex == EnumIndex)
			{
				resCode = DBDeactivateSQL (hstmt);
				if (resCode != DB_SUCCESS) ShowError();
				return Module4TypeId;
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
---