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
#include <stdio.h>
#include <utility.h>
#include <userint.h>

#include "Definelist.h"
#include "HelpFunctions.h"
#include "ParameterProcessing.h"
#include "Typedefinitions.h" 
#include "UsbLabView.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
UINT16 FirstCalling = 0, IndexExe = 0;
char StartPathName[MAX_PATHNAME_LEN], **ppArrayFileList, *pExeList = NULL, TestDirection[MAX_PATHNAME_LEN], ProjectDirection[MAX_PATHNAME_LEN], ParametertreeStructBuildDirection[MAX_PATHNAME_LEN], TTIC2Direction[MAX_PATHNAME_LEN], ControllerHardware[CONTROLLER_HARDWARE_LENGTH], GlobalVariableTestOptions[TEST_OPTION_LENGTH], TestOptionsString[MAX_OPTION_LENGTH];

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den aktuellen Projekt Pfadnamen an  
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetProjectPathName(void)
{
	BOOL8 ResultUserWantEditTestPath = TRUE, ResultUserWantEditTemplateTestPath = TRUE, ResultUserWantEditTTIC2Path = TRUE;
	char *pPathName, ProjectDirectionTemp[MAX_PATHNAME_LEN], UserTTIC2Path[MAX_PATHNAME_LEN], *pSourceInTestDirection, *pSourceInTTIC2Direction;
	int StatusDirectionTestPath = 0, StatusDirectionTemplateTestPath = 0, StatusDirectionTTIC2Path = 0;
	
	ProjectDirectionTemp[0] = 0;
	GetFullPathFromProject ("", ProjectDirection);
	strcat(ProjectDirectionTemp, ProjectDirection);
	pPathName = strstr(ProjectDirectionTemp, "Test");
	//An der Adresse des Suchwortes den String abschliessen
	*pPathName = 0;
	strcpy(StartPathName, ProjectDirectionTemp);
	
	do
	{
		//Pfad vom User vorgeben, in welchem nach Tests gesucht wird, welche die aktuelle Software erhalten
		//Achtung DirSelectPopupEx ist für Windows Vista und DirSelectPopup für Windows 7
		StatusDirectionTestPath = DirSelectPopup ("C:\\Test", "Choose the test direction, in which the software will updated", 1, 1, TestDirection);
		if(StatusDirectionTestPath == DIRECTORY_NOT_SELECTED)	ResultUserWantEditTestPath = (BOOL8)ConfirmPopup("Required test path", "Please select the test folder");
	} while(ResultUserWantEditTestPath == TRUE && StatusDirectionTestPath == DIRECTORY_NOT_SELECTED);

	//Nach Source im TestDirection String suchen und wenn nicht vorhanden anfügen
	pSourceInTestDirection = strstr(TestDirection, "Source");
	if(pSourceInTestDirection == NULL)	strcat(TestDirection, "\\Source");
	
	//Je nach Controller Hardware die Tests aktuallisieren
	//HelpFunctions_GetControllerHardwareString(ControllerHardware);
	strcpy(ControllerHardware, "IC2");
	//strcat(TestDirection, "\\");
	//strcat(TestDirection, ControllerHardware);
	
	do
	{
		//Pfad in welchem sich das ParametertreeStructBuild Programm sich befindet		
		StatusDirectionTemplateTestPath = DirSelectPopup ("C:\\Test\\TestUpdate", "Choose the ParametertreeStructBuild programm direction(default Test/TestUpdate) direction", 1, 1, ParametertreeStructBuildDirection);
		if(StatusDirectionTemplateTestPath == DIRECTORY_NOT_SELECTED)	ResultUserWantEditTemplateTestPath = (BOOL8)ConfirmPopup("Required ParametertreeStructBuild path", "Please select the ParametertreeStructBuild folder");
	} while(ResultUserWantEditTemplateTestPath == TRUE && StatusDirectionTemplateTestPath == DIRECTORY_NOT_SELECTED);
	
	do
	{
		//Pfad in welchem sich die Testoberfläche sich befindet	
		StatusDirectionTTIC2Path = DirSelectPopup ("C:\\TTIC2", "Choose the TTIC2(Test Tool IC2) direction", 1, 1, UserTTIC2Path);
		if(StatusDirectionTTIC2Path == DIRECTORY_NOT_SELECTED)	ResultUserWantEditTTIC2Path = (BOOL8)ConfirmPopup("Required TTIC2 path", "Please select the TTIC2(Test Tool IC2) folder");
	} while(ResultUserWantEditTTIC2Path == TRUE && StatusDirectionTTIC2Path == DIRECTORY_NOT_SELECTED);
	
	
	//Nach Source in TTIC2 Direction String suchen und wenn nicht vorhanden anfügen
	pSourceInTTIC2Direction = strstr(UserTTIC2Path, "Source");
	if(pSourceInTTIC2Direction == NULL)	strcat(UserTTIC2Path, "\\Source\\");
	else																strcat(UserTTIC2Path, "\\");
	strcpy(TTIC2Direction, UserTTIC2Path);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Controller Hardware als String zurück
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetControllerHardwareString(char *pHardwareVersion)
{
	UINT16 ControllerType;
	
	ParameterProcessing_SendParameter(0x0F100401, 0, &ControllerType, 3, 11);
	//HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "");
	switch(ControllerType)
	{
		case Test_IC2H1:	strcat(pHardwareVersion, "IC2H1");	break;
		case Test_IC2H2:	strcat(pHardwareVersion, "IC2H2"); 	break;
		case Test_IC2H3:	strcat(pHardwareVersion, "IC2H3"); 	break;
		case Test_IC2H4:	strcat(pHardwareVersion, "IC2H4"); 	break;
		case Test_IC2H5:	strcat(pHardwareVersion, "IC2H5"); 	break;
		default:					strcat(pHardwareVersion, "");				break;
	}
	
	//USB Kommmunikation wider frei geben (Wird anschliessend zum auslesen der Parameter gebraucht)
	UsbDisconnect(&UsbReferece);
	UsbConnected = 0;
}

+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Controller Hardware den Enum Wert zurück
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
UINT16 HelpFunctions_GetControllerHardwareValue(void)
{
	UINT16 ControllerType;
	
	ParameterProcessing_SendParameter(0x0F100401, 0, &ControllerType, 3, 11);

	//USB Kommmunikation wider frei geben (Wird anschliessend zum auslesen der Parameter gebraucht)
	UsbDisconnect(&UsbReferece);
	UsbConnected = 0;
	
	return ControllerType;
}
---
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Controller Hardware den Enum Wert als String zurück
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetControllerHardware(char *pControllerTypeEnum)
{
	UINT16 ControllerType;
	
	ParameterProcessing_SendParameter(0x0F100401, 0, &ControllerType, 3, 11);
	
	switch(ControllerType)
	{
		case Test_IC2H1:	strcpy(pControllerTypeEnum, "1");		break;
		case Test_IC2H2:	strcpy(pControllerTypeEnum, "2"); 	break;
		case Test_IC2H3:	strcpy(pControllerTypeEnum, "3"); 	break;
		case Test_IC2H4:	strcpy(pControllerTypeEnum, "4"); 	break;
		case Test_IC2H5:	strcpy(pControllerTypeEnum, "5"); 	break;
		default:					strcpy(pControllerTypeEnum, "");		break;
	}
		
	
	//USB Kommmunikation wider frei geben (Wird anschliessend zum auslesen der Parameter gebraucht)
	UsbDisconnect(&UsbReferece);
	UsbConnected = 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den Firmware Code zurück
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetFirmwareCodeString(char *pFirmwareCode)
{
	UINT16 ControllerType;
	
	ParameterProcessing_SendParameter(0x0F100401, 0, &ControllerType, 3, 11);
	//HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "");
	switch(ControllerType)
	{
		case Test_IC2H1:	strcat(pFirmwareCode, "F01");		break;
		case Test_IC2H2:	strcat(pFirmwareCode, "F02"); 	break;
		case Test_IC2H3:	strcat(pFirmwareCode, "F01"); 	break;
		case Test_IC2H4:	strcat(pFirmwareCode, "F03"); 	break;
		default:					strcat(pFirmwareCode, "");			break;
	}
	
	//USB Kommmunikation wider frei geben (Wird anschliessend zum auslesen der Parameter gebraucht)
	UsbDisconnect(&UsbReferece);
	UsbConnected = 0;

}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die vorhandenen exe Files in eine Struktur ab und stellt sie in der linken Liste(verfügbare Tests) dar
//  Input           : pTestPathNames: Gibt den Pfad an in welchem die Tests gesucht werden
//  Output *Pointer : -
//         *return  : gibt die Anzahl gefundenen Exe Files an 
//******************************************************************************************************************************
UINT16 HelpFunctions_WriteExeFiles(char *pTestPathNames)
{
	int EntriesLengthMalloc = NUMBERENTRIES, CountExeFiles, StrlenExeList, IndexExeString, StartAdressExeList = 0, IndexSingleExeName = 0, EntriesFiles = 0, IndexCompareExeNameList = 0, DuplicateExeName = 0, LengthPathFileList = 0;
	char SingleExeName[MAX_PATHNAME_LEN];
	LengthPathFileList++;
	
	//Darf nicht schon initialisiert sein
	if(ppArrayFileList != NULL) free(ppArrayFileList);
	ppArrayFileList = (char **) malloc(sizeof(char*) * NUMBERENTRIES);
	SingleExeName[0] = 0;
	
	//Durchsucht den Ordner rekursiv nach allen exe Files
	if(strlen(pTestPathNames) > 0)	
	{
		HelpFunctions_RecursiveFileCount(pTestPathNames,&CountExeFiles);
		//Um beim nächsten Aufruf wider ein neue dynamische Exe Liste erstellen zu können
		FirstCalling = 0;
		IndexExe = 0;
		//Auswertung der ExeList
		StrlenExeList = strlen(pExeList); 
	
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
				//TemplateTest wird nicht mehr kompiliert, da in unterschiedlichen Ordner diese Abgelegt ist
				if(strstr(SingleExeName, "TTIC2") == NULL && strstr(SingleExeName, "ParametertreeStructBuild") == NULL && strstr(SingleExeName, "TestUpdateFirmware") == NULL && strstr(SingleExeName, "sample_dbg.manifest") == NULL && strstr(SingleExeName, "GenerateDiagnosticFile") == NULL && strstr(SingleExeName, "FoE_Labview") == NULL && strstr(SingleExeName, "ReadWrite_PDO_SDO_Labview") == NULL && strstr(SingleExeName, "PDO_SDO_Synchronization") == NULL && strstr(SingleExeName, "InstallSharedVariable") == NULL && strstr(SingleExeName, "PositionSequenz_Labview") == NULL && strstr(SingleExeName, "ControlModeSequenz_Labview") == NULL && strstr(SingleExeName, "TemplateTest") == NULL)
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
				EntriesLengthMalloc = EntriesLengthMalloc + NUMBERENTRIES;
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
//  Description     : Legt die Globale TestOptions Variable an
//										Index 0: Info File Funktion im Test ausführen, Index 1: Keine Pass Command im Report, Index 2: Die Parameter werte werden nach dem Test nicht mehr zurück gesetzt 
//										Index 3: Ob die Kollektion abgebrochen werden soll bei einem Fehler, Index 4: Um die TestVersion in die CurrentTestsVersion Tabelle zu schreiben
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_InitGlobalVariableTestOptions(void)
{
	//TestOptions initialisieren
	strcpy(TestOptionsString, "0000");
	//TestOptions Globale Variable initiallisieren
	strcpy(GlobalVariableTestOptions, "TESTOPTIONS=");
	//TestOptions initialisieren
	strcat(GlobalVariableTestOptions, TestOptionsString);
	//Ändert die Globale Variable
	_putenv(GlobalVariableTestOptions);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den aktuellen Wert in die TestOptions Global Variable
//										Index 0: Info File Funktion im Test ausführen, Index 1: Keine Pass Command im Report, Index 2: Die Parameter werte werden nach dem Test nicht mehr zurück gesetzt
//										Index 3: Ob die Kollektion abgebrochen werden soll bei einem Fehler, Index 4: Um die TestVersion in die CurrentTestsVersion Tabelle zu schreiben
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WriteGlobalVariableTestOptions(UINT8 TestOptionIndex, char *pTestOptionValue)
{
	switch(TestOptionIndex)
	{
		case INFO_OPTION_INDEX:
		{
			//Um den TestOptionsString zu eliminieren
			GlobalVariableTestOptions[OFFSET_TEST_OPTION_NAME] = 0;
			//Schreibt den neuen Info Option Wert
			TestOptionsString[INFO_OPTION_INDEX] = pTestOptionValue[0];
			strcat(GlobalVariableTestOptions, TestOptionsString);
			//Ändert die Globale Variable
			_putenv(GlobalVariableTestOptions);
			break;
		}
		case PASS_COMMAND_OPTION_INDEX:
		{
			//Um den TestOptionsString zu eliminieren
			GlobalVariableTestOptions[OFFSET_TEST_OPTION_NAME] = 0;
			//Schreibt den neuen Pass Command Option Wert
			TestOptionsString[PASS_COMMAND_OPTION_INDEX] = pTestOptionValue[0];
			strcat(GlobalVariableTestOptions, TestOptionsString);
			//Ändert die Globale Variable
			_putenv(GlobalVariableTestOptions);
			break;
		}
		case TERMINATE_BY_ERROR_OPTION_INDEX:
		{
			//Um den TestOptionsString zu eliminieren
			GlobalVariableTestOptions[OFFSET_TEST_OPTION_NAME] = 0;
			//Schreibt den neuen ParameterSettingBack Option Wert
			TestOptionsString[TERMINATE_BY_ERROR_OPTION_INDEX] = pTestOptionValue[0];
			strcat(GlobalVariableTestOptions, TestOptionsString);
			//Ändert die Globale Variable
			_putenv(GlobalVariableTestOptions);
			break;
		}
		case TEST_VERSION_OPTION_INDEX:
		{
			//Um den TestOptionsString zu eliminieren
			GlobalVariableTestOptions[OFFSET_TEST_OPTION_NAME] = 0;
			//Schreibt den neuen ParameterSettingBack Option Wert
			TestOptionsString[TEST_VERSION_OPTION_INDEX] = pTestOptionValue[0];
			strcat(GlobalVariableTestOptions, TestOptionsString);
			//Ändert die Globale Variable
			_putenv(GlobalVariableTestOptions);
			break;
		}
		default:	break;
	}
}
---
//******************************************************************************************************************************
//  Description     : sucht in einem angebenen Pfad rekursiv nach Exe Files und listet sie in einem String welcher mit ; getrennt wird
//  Input           : Path:						Pfad in welchem Rekursiv nach Exe Datein gesucht werden sollte
//  Output *Pointer : NumberOfFiles:	Gibt die Anzahl gefundenen Exe Files zurück	
//         *return  :  
//******************************************************************************************************************************
int HelpFunctions_RecursiveFileCount(char *Path, long *NumberOfFiles)
{
	char CurrentFile[MAX_PATHNAME_LEN], dirNamesSaved[MAX_NUM_DIR][MAX_PATHNAME_LEN];
	int numDir = 0, ReturnVal = 0, i = 0, ignore = 0, StrlenCurrentFile, IndexExeFileString;
	static int IndexCurrentFileString = 0, StrlenExeEntries = NUMBERCHAR;
	long tempNumFiles;

	if(strlen(Path) > 0)
	{
		//malloc ein Liste von Einträgen erstellen und Adresse übergeben, nur einmal initialisieren
		if(FirstCalling == 0)	pExeList = (char *)malloc(sizeof(char) * NUMBERCHAR);
		FirstCalling++;
	
		MakePathname (Path, "*.*", CurrentFile);
		ReturnVal = GetFirstFile (CurrentFile, 1, 1, 1, 1, 1, 1, CurrentFile);
		if (ReturnVal == -1)
			return ReturnVal;
		while(ReturnVal == 0)
		{
			MakePathname (Path, CurrentFile, CurrentFile);
			if(GetFileAttrs(CurrentFile, &ignore, &ignore, &ignore, &ignore) == 1)
			{
				if(numDir >= MAX_NUM_DIR-1)
					return -4; 
				strcpy(dirNamesSaved[numDir++], CurrentFile);
			}
			else
			{
				if(strstr(CurrentFile, EXEFILE))
				{
					(*NumberOfFiles)++;
					HelpFunctions_StringReplace(".exe", "", CurrentFile);
					
					StrlenCurrentFile = strlen(CurrentFile);
					for(IndexCurrentFileString = StrlenCurrentFile; IndexCurrentFileString > 0; IndexCurrentFileString--)
					{
						//auf / Zeichen suchen
						if (CurrentFile[IndexCurrentFileString] == 0x5C)
						{
								CurrentFile[IndexCurrentFileString] = 0;
								break;
						} //end if suche /
					} //end for Schleife um String zu durchsuchen

					//gefundene exe File abspeichern
					for(IndexExeFileString = IndexCurrentFileString + 1; IndexExeFileString < StrlenCurrentFile; IndexExeFileString++)
					{
						pExeList[IndexExe] = CurrentFile[IndexExeFileString];
						IndexExe++;
					}	//end for Schleife um den Namen abzuspeichern*/
					
					
					//nacher ein ; Zeichen einfügen um eine Trennung später zu haben
					pExeList[IndexExe] = 0x3B;
					IndexExe++;
					//0 Abschluss, wird beim letzten Durchgang nicht überschriben
					pExeList[IndexExe] = 0;
					//Wenn alle Einträge aufgebrauct um 5 weitere dynamisch vergrössern
					if((StrlenExeEntries - IndexExe) < 200)	
					{
						StrlenExeEntries = StrlenExeEntries + NUMBERCHAR;
						pExeList = (char *) realloc(pExeList, sizeof(char) * StrlenExeEntries);
					}
				}	//end if Abfrage ob exe Datei
			}	//end else Bedingung ob File
			ReturnVal = GetNextFile(CurrentFile);
		}
		for(i=0;i<numDir;i++)
		{
			tempNumFiles = 0;
			if(ReturnVal == HelpFunctions_RecursiveFileCount(dirNamesSaved[i], &tempNumFiles) <- 1)
				return ReturnVal;
			(*NumberOfFiles)+=tempNumFiles;
		}
		if (ReturnVal == -1)
			ReturnVal = 0; //It is normal for the return value to be -1
	}
	else	ReturnVal = 0;
	
	return ReturnVal;
}

//******************************************************************************************************************************
//  Description     : Funktion mit welcher ein String manipuliert werden kann (Kann sagen was in einem String ersetzt werden sollte)
//  Input           : search:			Was sollte im String gesucht werden
//									: replace			Mit was sollte der String ersetzt werden
//  Output *Pointer : string			Gibt den manipulirten String zurück	
//         *return  :  
//******************************************************************************************************************************
char *HelpFunctions_StringReplace(char *search, char *replace, char *string) 
{
	char *tempString, *searchStart;
	int len=0;

	// preuefe ob Such-String vorhanden ist
	searchStart = strstr(string, search);
	if(searchStart == NULL) return string;

	// Speicher reservieren
	tempString = (char*) malloc((strlen(string)+1) * sizeof(char));
	if(tempString == NULL) return NULL;

	// temporaere Kopie anlegen
	strcpy(tempString, string);

	// ersten Abschnitt in String setzen
	len = searchStart - string;
	string[len] = '\0';

	// zweiten Abschnitt anhaengen
	strcat(string, replace);

	// dritten Abschnitt anhaengen
	len += strlen(search);
	strcat(string, (char*)tempString+len);

	// Speicher freigeben
	free(tempString);
	
	return string;
}

//******************************************************************************************************************************
//  Description     : Liefert den String nach dem Suchstring zurück
//  Input           : search:			Was sollte im String gesucht werden
//  Output *Pointer : string			Gibt den String nach dem Suchstring zurück	
//         *return  :  
//******************************************************************************************************************************
char *HelpFunctions_StringAfterSubstring(char *string, char *search) 
{
	char *tempString, *searchStart;
	int len=0;

	// preuefe ob Such-String vorhanden ist
	searchStart = strstr(string, search);
	if(searchStart == NULL) return NULL;

	// Speicher reservieren
	tempString = (char*) malloc((strlen(string)+1) * sizeof(char));
	if(tempString == NULL) return NULL;

	// temporaere Kopie anlegen
	strcpy(tempString, string);

	//String nach dem Suchstring anhaengen
	len = strlen(string) - strlen(searchStart) + strlen(search);
	strcpy(string, (char*)tempString+len);

	// Speicher freigeben
	free(tempString);
	
	return string;
}


