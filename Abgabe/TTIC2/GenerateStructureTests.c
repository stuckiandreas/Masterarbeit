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
#include <cvirte.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility.h>
#include <string.h>
#include <time.h>
#include <toolbox.h>
#include <userint.h>
#undef PI;

#include "Database.h"
#include "Definelist.h"
#include "DefineStructFileService.h"
#include "GenerateStructureTests.h"
#include "HelpFunctions.h"
#include "LoadTestInformation.h"
#include "SharedVariable.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
UINT16 LoadTestIndex;
void WrongDirectionOfTestPath(void);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : List die Test Version aus dem Test im Testordner. Anschliessend wird die Teststruktur gefüllt 
//  Input           : -
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void GenerateStructureTests_GenerateTestsStructure(void)
{
	char ErrorMessage[MAX_PATHNAME_LEN];
	
	HelpFunctions_InitGlobalVariableTestOptions();
	
	//wenn keine exe files gefunden wurden
	if(EntriesFiles == 0)
	{
		WrongDirectionOfTestPath();
		sprintf(ErrorMessage, "Found no exe Files");
		MessagePopup ("", ErrorMessage);
	}
	
	//Strukturen erstellen um später die Tests auflisten zu können
	pTests = malloc(sizeof(struct test_type) * EntriesFiles);
	pSelectedTests = malloc(sizeof(struct testSelected_type) * EntriesFiles);
	pAvaiableTests = malloc(sizeof(struct testAvaiable_type) * EntriesFiles);

	//Hier soll ein Menu aufgehen und zeigen, wie der Fortschritt aussieht
	//List die Test Version Information 
	LoadTestInformation_Load();

}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die vorhandenen exe Files in eine Struktur ab und stellt sie in der linken Liste(verfügbare Tests) dar
//  Input           : pTestPathNames: Gibt den Pfad an in welchem die Tests gesucht werden
//  Output *Pointer : -
//         *return  : gibt die Anzahl gefundenen Exe Files an 
//******************************************************************************************************************************
UINT16 GenerateStructureTests_WriteExeFiles(char *pTestPathNames)
{
	int EntriesLengthMalloc = NUMBER_ENTRIES, CountExeFiles, StrlenExeList, IndexExeString, StartAdressExeList = 0, IndexSingleExeName = 0, EntriesFiles = 0, IndexCompareExeNameList = 0, DuplicateExeName = 0, LengthPathFileList = 0;
	char SingleExeName[MAX_PATHNAME_LEN], TestPathNameIC2[MAX_PATHNAME_LEN] = "", TestPathNameControllerType[MAX_PATHNAME_LEN] = "";
	LengthPathFileList++;
	
	ppArrayFileList = (char **) malloc(sizeof(char*) * NUMBER_ENTRIES);
	SingleExeName[0] = 0;
	
	//Durchsucht den Ordner rekursiv nach allen exe Files
	if(strlen(pTestPathNames) > 0)	
	{
		//Es werden alle Tests ausgelesen (werden später nach Hardware nicht angezeigt)
		strcpy(TestPathNameIC2, pTestPathNames);
		strcpy(TestPathNameControllerType, pTestPathNames);
		HelpFunctions_RecursiveFileCount(TestPathNameIC2); 
		HelpFunctions_RecursiveFileCount(TestPathNameControllerType);
		CountExeFiles = HelpFunctions_GetCountExeFiles();
		if(CountExeFiles == 0)	return 0;
		//Um beim nächsten Aufruf wider ein neue dynamische Exe Liste erstellen zu können
		FirstCalling = 0;
		IndexExe = 0;
		//Auswertung der ExeList
		StrlenExeList = strlen(pExeList);
		if(StrlenExeList == 0)	return 0;
	
		//String nach ; durchsuchen und die einzelnen Exe Namen abspeichern
		for(IndexExeString = 0; IndexExeString < StrlenExeList; IndexExeString++)
		{
			SingleExeName[IndexSingleExeName] = pExeList[IndexExeString];
			IndexSingleExeName++;
			//auf ; überprüfen
			if (pExeList[IndexExeString] == 0x3B)	
			{
				pExeList[IndexExeString] = 0;
				SingleExeName[IndexSingleExeName - 1] = 0;
				IndexSingleExeName = 0;
				//wenn nicht das Hauptprogramm oder die Hilfsfunktion für die generierung der Parameterliste oder den Hilfstest
				if(strstr(SingleExeName, "TTIC2") == NULL && strstr(SingleExeName, "ParametertreeStructBuild") == NULL && strstr(SingleExeName, "TemplateTest") == NULL && strstr(SingleExeName, "TestUpdateFirmware") == NULL && strstr(SingleExeName, "sample_dbg.manifest") == NULL && strstr(SingleExeName, "GenerateDiagnosticFile") == NULL && strstr(SingleExeName, "ReadWrite_PDO_SDO_Labview") == NULL && strstr(SingleExeName, "PDO_SDO_Synchronization") == NULL && strstr(SingleExeName, "InstallSharedVariable") == NULL && strstr(SingleExeName, "FoE_Labview") == NULL && strstr(SingleExeName, "PositionSequenz_Labview") == NULL && strstr(SingleExeName, "ControlModeSequenz_Labview") == NULL)
				{
					//wenn noch kein Exe Eintrag
					if(EntriesFiles == 0)
					{
						ppArrayFileList[EntriesFiles] = &pExeList[StartAdressExeList];
						EntriesFiles++;
						SingleExeName[0] = 0;
					}	//end if überprüfung kein Eintrag
					//sind schon Einträge vorhanden muss überprüft werden ob der Exe Namen schon vertreten ist
					else 
					{
						for(IndexCompareExeNameList = 0; IndexCompareExeNameList < EntriesFiles; IndexCompareExeNameList++)
						{
							if(strcmp(ppArrayFileList[IndexCompareExeNameList], SingleExeName) == 0)	DuplicateExeName = 1;
						} //end for Vergleich ob exe Namen schon in Liste
						if(DuplicateExeName == 0)
						{
							ppArrayFileList[EntriesFiles] = &pExeList[StartAdressExeList];
							EntriesFiles++;
							SingleExeName[0] = 0;
						}	//end if Action wenn der Exe Namen noch nicht in der Liste war
					}	//end else nicht der erste Eintrag
					DuplicateExeName = 0;
				}	//end if ob der Exe Name des Haupt-/Hilfsprogramm ist
				StartAdressExeList = IndexExeString + 1;
			}	//end if überprüfung ob ein ; Zeichen vorhanden ist
			//Exe Liste dynamisch vergrössern
			if(EntriesLengthMalloc - EntriesFiles < 2)
			{
				EntriesLengthMalloc = EntriesLengthMalloc + NUMBER_ENTRIES;
				ppArrayFileList = (char **) realloc(ppArrayFileList, sizeof(char*) * EntriesLengthMalloc);
			}	//end if dynamisch die Exe Liste vergrössern
		} //end for String mit Exe Namen
	}	//end if Pfadnamen vorhanden
	else	EntriesFiles = 0;	
	return (UINT16)EntriesFiles;
}

+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Fragt alle Tests nach ihrer Test Version aus 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateStructureTests_WriteTestStructure(void)
{
	SINT32 TestId, TestVersionId = -1;
	char TestExeFile[MAX_PATHNAME_LEN], TestInfoFileExe[MAX_PATHNAME_LEN];
	
	//Bit 3 setzen für Test Version in CurrentTestsVersion Tabelle zu schreiben -> passiert für alle Tests
	HelpFunctions_WriteGlobalVariableTestOptions(TEST_VERSION_OPTION_INDEX, "1");
	
	Database_ConnectDatabase();

	for (LoadTestIndex = 0; LoadTestIndex < EntriesFiles; LoadTestIndex++)
	{	 
		//Den Testnamen eintragen
		strcpy(pTests[LoadTestIndex].TestName, ppArrayFileList[LoadTestIndex]);
		strcpy(TestExeFile, PathTestFolder);
		//Einmal um den Ordnernamen zu öffnen
		strcat(TestExeFile, pTests[LoadTestIndex].TestName);
		strcat(TestExeFile, BETWENCHAR);
		strcpy(TestInfoFileExe, TestExeFile);
		strcat(TestExeFile, pTests[LoadTestIndex].TestName);
		strcat(TestExeFile, ".exe");
		system(TestExeFile);
		
		//Nun soll die TestVersionId abgespeichert werden -> zuerst Id aus Datenbank holen
		TestId = Database_GetTestIdFromTestInformation(pTests[LoadTestIndex].TestName);
		if(TestId >= 0)	TestVersionId = Database_GetTestVersionID(TestId, TestVersion);
		else	MessagePopup("Error Database", "Test not found in database");
		if(TestVersionId >= 0)	pTests[LoadTestIndex].TestVersionId = TestVersionId;
		else	MessagePopup("Error Database", "Test version not found in database");
	}
	
	//>Anschliessend wieder das Bit zurück setzen
	HelpFunctions_WriteGlobalVariableTestOptions(TEST_VERSION_OPTION_INDEX, "0");
}
---

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die dem User die Meldung, das der Testpfad nicht existiert. Fordert ihn auf einen existierenden Pfad einzugeben und kontrolliert diesen
//  Output *Pointer : pDataInfoTest:							Info File Informationen der Tests
//         *return  : Liste der gefundenen Exe Files im angegebenen Test Pfad 
//******************************************************************************************************************************
void WrongDirectionOfTestPath(void)
{
	BOOL8 ResultUserWantEditTestPath = TRUE;
	
	ResultUserWantEditTestPath = (BOOL8)ConfirmPopup("Wrong Direction", "Found no test file. Please select an avaiable test folder");
	//alle alten Daten löschen, welche mit dem falschen Pfad ausgelesen wurden
	free(ppArrayFileList);
	EntriesFiles = 0;
	
	//wider von neuem ein Test Pfad suchen
	HelpFunctions_GetTestsPath(PathTestFolder);
	
	EntriesFiles = GenerateStructureTests_WriteExeFiles(PathTestFolder);
	//Die Anzahl Tests können sich später verändern
	CountTests = EntriesFiles;
	GenerateStructureTests_GenerateTestsStructure();
}









