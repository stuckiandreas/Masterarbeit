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
#include <cvinetv.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
//Because of name conflicts, Windows headers must be included before utility.h and formatio.h.
#include <windows.h>
#include <rs232.h>
#include <NIDAQmx.h>
#include <utility.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <toolbox.h>
#undef PI;
#undef GetSystemTime

#include "CPX400DP_SupplyValve.h"
#include "DAQTask_PC_IC2H1_DI.h" 
#include "DAQTask_PC_IC2H1_DO.h"
#include "DAQTask_PC_IC2H2_DI.h" 
#include "DAQTask_PC_IC2H2_DO.h"
#include "DAQTask_PC_IC2H2_AI.h" 
#include "DAQTask_PC_IC2H2_AO.h"
#include "Database.h"
#include "DefineEnums.h"
#include "Definelist.h"
#include "DefineStructFileService.h"
#include "GenerateStructureTests.h"
#include "HelpFunctions.h"
#include "InterfaceCommunication.h"
#include "ParameterProcessing.h"
#include "PlatformConfig.h"
#include "PC_Defines.h"
#include "PC_IC2H1_HelpFunctions.h"
#include "PC_IC2H2_HelpFunctions.h"
#include "ReadWriteDIO_PC_IC2H1.h"
#include "ReadWriteDIO_PC_IC2H2.h"
#include "ReadWriteAIO_PC_IC2H2.h"
#include "Report.h"
#include "SelectListbox.h"
#include "SharedVariable.h" 
#include "SurfaceTest.h"
#include "Typedefinitions.h"
#include "UsbLabView.h" 

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void CVICALLBACK DataReturnValueCallback (void * handle, CNVData data,void * callbackData);
static void CVICALLBACK StatusCallback (void * handle, CNVConnectionStatus status, int error, void * callbackData);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
BOOL8 PowerConnectorHardwareAvaiable = FALSE, RestoreCommand = FALSE;
UINT8 NumberOfSlaves = 0, TimeOffset;
UINT16 FirstCalling = 0, IndexExe = 0;
char *pExeList = NULL, GlobalVariableTestOptions[TEST_OPTION_LENGTH], PathNameUsbLogFile[MAX_PATHNAME_LEN];
FLOAT HomingProcessTime = 60.0f;

//******************************************************************************************************************************
//  Description     : sucht in einem angebenen Pfad rekursiv nach Exe Files und listet sie in einem String welcher mit ; getrennt wird
//  Input           : Path:						Pfad in welchem Rekursiv nach Exe Datein gesucht werden sollte
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
int HelpFunctions_RecursiveFileCount(char *Path)
{
	int numDir = 0, ReturnVal = 0, i = 0, ignore = 0, StrlenCurrentFile, IndexExeFileString, DirectionFolder = 5, StrlenCurrentFileWithRoot, IndexCurrentFileWithRoot;
	static int StrlenExeEntriesStringMalloc, IndexCurrentFileString = 0;
	long tempNumFiles; 
	double StartSearchExeFiles, DiffSearchExeFiles;
	char CurrentFile[MAX_PATHNAME_LEN], dirNamesSaved[MAX_NUM_DIR][MAX_PATHNAME_LEN], CurrentFileWithRoot[MAX_PATHNAME_LEN] = ""; 

	StartSearchExeFiles = Timer();
	
	if(strlen(Path) > 0)
	{
		//malloc ein Liste von Einträgen erstellen und Adresse übergeben, nur einmal initialisieren
		if(FirstCalling == 0)	
		{
			pExeList = (char *)malloc(sizeof(char) * NUMBER_CHAR);
			StrlenExeEntriesStringMalloc = NUMBER_CHAR;
			pExeList[0] = 0;
		}
		FirstCalling++;
	
		MakePathname (Path, "*.*", CurrentFile);
		ReturnVal = GetFirstFile (CurrentFile, 1, 1, 1, 1, 1, 1, CurrentFile);
		if (ReturnVal == -1)
			return ReturnVal;
		while(ReturnVal == 0)
		{
			//Limit einfügen, um nicht endlos nach exe Files zu suchen
			if((DiffSearchExeFiles = Timer() - StartSearchExeFiles) > MAX_TIME_SEARCH_EXE_FILES)	return 0;
			MakePathname (Path, CurrentFile, CurrentFile);
			if(GetFileAttrs(CurrentFile, &ignore, &ignore, &ignore, &ignore) == 1)
			{
				if(numDir >= MAX_NUM_DIR-1)
					return -4; 
				strcpy(dirNamesSaved[numDir++], CurrentFile);
			}
			else
			{
				if(strstr(CurrentFile, EXE_FILE))
				{
					HelpFunctions_StringReplace(".exe", "", CurrentFile);
					
					//Jetzt nach dem Ordner durchsuchen -> wiessen in welchem Ordner die exe abgelegt ist
					if(strstr(CurrentFile, IC2_FOLDER))	DirectionFolder = 0;
					else if(strstr(CurrentFile, IC2H1_FOLDER))	DirectionFolder = 1;
					else if(strstr(CurrentFile, IC2H2_FOLDER))	DirectionFolder = 2;
					else if(strstr(CurrentFile, IC2H3_FOLDER))	DirectionFolder = 3;
					else if(strstr(CurrentFile, IC2H4_FOLDER))	DirectionFolder = 4;
					else 																				DirectionFolder = 5;

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
					
										//Ordner als erstes abspeichern
					switch(DirectionFolder)
					{
						case 0:	strcpy(CurrentFileWithRoot, "IC2\\");		break;
						case 1:	strcpy(CurrentFileWithRoot, "IC2H1\\");	break;
						case 2:	strcpy(CurrentFileWithRoot, "IC2H2\\");	break;
						case 3:	strcpy(CurrentFileWithRoot, "IC2H3\\");	break;
						case 4:	strcpy(CurrentFileWithRoot, "IC2H4\\");	break;
						default:																				break;
					}
					
										IndexCurrentFileWithRoot = strlen(CurrentFileWithRoot);
					
					//gefundener exe Name mit dem Ordnernamen abspeichern
					for(IndexExeFileString = IndexCurrentFileString + 1; IndexExeFileString < StrlenCurrentFile; IndexExeFileString++)
					{
						CurrentFileWithRoot[IndexCurrentFileWithRoot] = CurrentFile[IndexExeFileString];
						IndexCurrentFileWithRoot++;
						//sauberer Abschluss des Strings
						if(IndexExeFileString == (StrlenCurrentFile - 1))	CurrentFileWithRoot[IndexCurrentFileWithRoot] = 0;
					}
					
					StrlenCurrentFileWithRoot = strlen(CurrentFileWithRoot);

					
					//gefundene exe File abspeichern, wenn sie nicht bereits vorhanden ist
					if(strstr(pExeList, CurrentFile) == NULL)
					{
						for(IndexExeFileString = 0; IndexExeFileString < StrlenCurrentFileWithRoot; IndexExeFileString++)
						{
							pExeList[IndexExe] = CurrentFileWithRoot[IndexExeFileString];
							IndexExe++;
						}	//end for Schleife um den Namen abzuspeichern
						//nacher ein ; Zeichen einfügen um eine Trennung später zu haben
						pExeList[IndexExe] = 0x3B;
						IndexExe++;
						//0 Abschluss, wird beim letzten Durchgang nicht überschriben
						pExeList[IndexExe] = 0;
					}
					//Wenn alle Einträge aufgebraucht um 5 weitere dynamisch vergrössern
					if((StrlenExeEntriesStringMalloc - IndexExe) < 200)	
					{
						StrlenExeEntriesStringMalloc = StrlenExeEntriesStringMalloc + NUMBER_CHAR;
						pExeList = (char *) realloc(pExeList, sizeof(char) * StrlenExeEntriesStringMalloc);
					}
				}	//end if Abfrage ob exe Datei
			}	//end else Bedingung ob File
			ReturnVal = GetNextFile(CurrentFile);
		}
		for(i=0;i<numDir;i++)
		{
			tempNumFiles = 0;
			if(ReturnVal == HelpFunctions_RecursiveFileCount(dirNamesSaved[i]) <- 1)
				return ReturnVal;
		}
		if (ReturnVal == -1)
			ReturnVal = 0; //Der Return Value der Funktion ist -1
	}
	else	ReturnVal = 0;
	
	return ReturnVal;
}

//******************************************************************************************************************************
//  Description     : Gibt die Anzahl gefundenen exe Files zurück
//  Input           : -
//  Output *Pointer : 
//         *return  : Anzahl gefundene exe Files 
//******************************************************************************************************************************
int HelpFunctions_GetCountExeFiles(void)
{
	int StrlenExeList, IndexExeList, CountExeList = 0;
	
	StrlenExeList = strlen(pExeList);
	for(IndexExeList = 0; IndexExeList < StrlenExeList; IndexExeList++)
	{
		//auf Trennzeichen überprüfen
		if(pExeList[IndexExeList] == 0x3B)	CountExeList++;
	}
	return CountExeList;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Enum Wert in einen BOOL Array um (1 bedeutet, das dieser Eintrag unterstützt wird)
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetEnumIndex32BitArray(SINT32 EnumValue, BOOL8 *pEnumIndexArray)
{
	int i;
	
	for (i = 31; i >= 0; i--)	pEnumIndexArray[i] = ((EnumValue >> i) & 0x1);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Enum Wert in einen BOOL Array um (1 bedeutet, das dieser Eintrag unterstützt wird)
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetEnumIndex16BitArray(SINT16 EnumValue, BOOL8 *pEnumIndexArray)
{
	int i;
	
	for (i = 15; i >= 0; i--)	pEnumIndexArray[i] = ((EnumValue >> i) & 0x1);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Portnummer als Zahl zurück 
//  Input           : pPortNumberString:	Portnummer als String
//  Output *Pointer : -
//         *return  : Portnummer
//******************************************************************************************************************************
SINT8 HelpFunctions_GetPortNumber(char *pPortNumberString)
{
	UINT8 StrlenPortNumber, IndexPortNumber;
	char SubString[3], *pSubString;
	
	pSubString = strstr(pPortNumberString, "COM");
	if(pSubString == NULL)	return -1;
	else
	{
		StrlenPortNumber = (UINT8)strlen(pPortNumberString) - 3;
		for(IndexPortNumber = 0; IndexPortNumber < StrlenPortNumber; IndexPortNumber++)	SubString[IndexPortNumber] = pPortNumberString[3 + IndexPortNumber];
		SubString[StrlenPortNumber] = 0;
		return (SINT8)atoi(SubString);
	}
}

//******************************************************************************************************************************
//  Description     : Löscht alle Diagnostic Files im Result Ordner drin
//  Input           : -
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
void HelpFunctions_DeleteAllDiagnosticFiles()
{
	int numDir = 0, ReturnVal = 0, ignore = 0;
	double StartSearchDiagnosticFiles, DiffSearchDiagnosticFiles;
	char CurrentFile[MAX_PATHNAME_LEN], dirNamesSaved[MAX_NUM_DIR][MAX_PATHNAME_LEN], DiagnosticFilePath[MAX_PATHNAME_LEN]; 
	
	//Diagnostic Pfad zusammen setzen
	strcpy(DiagnosticFilePath, PathTTIC2Folder);
	strcat(DiagnosticFilePath, RESULT_FOLDER);
	
	StartSearchDiagnosticFiles = Timer();
	
	if(strlen(DiagnosticFilePath) > 0)
	{
		FirstCalling++;
	
		MakePathname (DiagnosticFilePath, "*.*", CurrentFile);
		ReturnVal = GetFirstFile (CurrentFile, 1, 1, 1, 1, 1, 1, CurrentFile);
		if (ReturnVal == -1)
			return;
		while(ReturnVal == 0)
		{
			//Limit einfügen, um nicht endlos nach Diagnostic Files zu suchen
			if((DiffSearchDiagnosticFiles = Timer() - StartSearchDiagnosticFiles) > MAX_TIME_SEARCH_EXE_FILES)	return;
			MakePathname (DiagnosticFilePath, CurrentFile, CurrentFile);
			if(GetFileAttrs(CurrentFile, &ignore, &ignore, &ignore, &ignore) == 1)
			{
				if(numDir >= MAX_NUM_DIR-1)
					return; 
				strcpy(dirNamesSaved[numDir++], CurrentFile);
			}
			else
			{
				if(strstr(CurrentFile, DIAGNOSTIC_FILE))
				{
					//Diagnostic File löschen
					remove(CurrentFile);
				}	//end if Abfrage ob Diagnostic Datei
			}	//end else Bedingung ob File
			ReturnVal = GetNextFile(CurrentFile);
		}
	}
}

//******************************************************************************************************************************
//  Description     : Funktion mit welcher ein String manipuliert werden kann (Kann sagen was in einem String ersetzt werden sollte)
//  Input           : search:			Was sollte im String gesucht werden
//									: replace			Mit was sollte der String ersetzt werden
//  Output *Pointer : string			Gibt den manipulirten String zurück	
//         *return  :  
//******************************************************************************************************************************
char* HelpFunctions_StringReplace(char *search, char *replace, char *string) 
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
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den aktuellen Projekt Pfadnamen an  
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetTTIC2Path(char *pStartPathTTIC2)
{
	char ProjectDirection[MAX_PATHNAME_LEN], *pPathName;
	
	GetFullPathFromProject ("", ProjectDirection);
	//Achtung Problem, je nachdem ob das Projekt oder die Exe ausgeführt wird
	pPathName = strstr(ProjectDirection, "Source");
	if(pPathName == NULL)	pPathName = strstr(ProjectDirection, "exe");
	//An der Adresse des Suchwortes den String abschliessen
	if(pPathName != NULL)	*pPathName = 0;
	strcpy(pStartPathTTIC2, ProjectDirection);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Der Anwender wählt den Ordner aus, aus welchem die Tests gesucht werden  
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetTestsPath(char *pTestPathName)
{
	BOOL8 ResultUserWantEditTestPath = TRUE;
	int StatusFile;
	
	do
	{
		//Abfrage nach dem Pfad
		StatusFile = DirSelectPopup ("C:\\Test", "Choose the direction of the tests", 1, 1, pTestPathName);

		if(StatusFile == DIRECTORY_NOT_SELECTED)	ResultUserWantEditTestPath = (BOOL8)ConfirmPopup("Required test path", "Please select the test folder");
	} while(ResultUserWantEditTestPath == TRUE && StatusFile == DIRECTORY_NOT_SELECTED);
+++
	if(ResultUserWantEditTestPath == FALSE)	
	{
		Database_DisconnectDatabase();
		exit(USER_NOT_WANT_EDIT_TESTPATH);
	}
---
	//Die einzelnen Test befinden sich im Tesordner im Unterordner Source
	if(strstr(pTestPathName, "Source") == NULL)	strcat(pTestPathName, "\\Source");
	//Noch ein Blackslash einfügen
	strcat(pTestPathName, BETWENCHAR);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den Test Colelction Name ohne Datum und Activity State zurück 
//  Input           : pTestName:						Test Kollektionsnamen
//										TestCollectionIndex:	Index des Test Kollektionsliste
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetTestCollectionNameWithoutDateAndStateInfo(char *pTestCollectionName, int TestCollectionIndex)
{
	char StringReplace[DATE_TYPE_LENGTH+1] = "";
	
	sprintf(StringReplace, " %s", pTestCollections[TestCollectionIndex].CreationDate);
	HelpFunctions_StringReplace(StringReplace, "", pTestCollectionName);
	if(pTestCollections[TestCollectionIndex].ActivityState == ACTIVITY_STATE_NOT_ACTIVE)	HelpFunctions_StringReplace(" Not Active", "", pTestCollectionName);
	else																																									HelpFunctions_StringReplace(" Active", "", pTestCollectionName);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Hardware Information des Controllers auf dem Ventil zurück 
//  Input           : 
//  Output *Pointer : -
//         *return  : Controller Type
//******************************************************************************************************************************
UINT16 HelpFunctions_GetValveControllerType(void)
{
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "");
	return(Parameter.System.Identification.Hardware.ControllerType.Value);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Sendet den Befehl an das Ventil und wertet sogleich die Antwort aus
//  Input           : ParameterID      Ist die BefehlsID
//                  : ParameterIndex   Array index (bei Parametertypen ungleich Array immer 0)					  
//                  : DataType         Datentyp vom Übergabewert pValue
//                  : *pValue          Wert, welcher in den Buffer geschrieben werden soll
//  Output *Pointer : *pValue					 Bei Get Befehlen steht der Wert darin
//         *return  : -
//******************************************************************************************************************************
UINT8 HelpFunctions_SendCommand(UINT32 ParameterID, UINT8 ParameterIndex, void *pValue, UINT8 DataType, UINT8 Action, char *pCommandMessage)
{
	int StrlenCommandMessage;
	char CompareString [11] = "error free", Command[REPORT_LENGTH], ReportMessage[REPORT_LENGTH];
	
	strcpy(Command, ParameterProcessing_SendParameter(ParameterID, ParameterIndex, pValue, DataType, Action));
	StrlenCommandMessage = strlen(pCommandMessage);
	
	if(strcmp(Command, CompareString) != 0 && (strlen(pCommandMessage) != 0)) 
	{
		sprintf(ReportMessage, "Fail Communication: %s", pCommandMessage);
		Report_AddText(ReportMessage);
		Error = 1;
	}
	else if(StrlenCommandMessage > 0)	Report_AddText(pCommandMessage);
	
	return Error;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Neustart des Ventils 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_RestartVentil(void)
{
	UINT64 StartUpCount = 0;
	double DiffRestart, StartRestart;
	
	//Abfrage StartUpCounter
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Statistics.StartUpCounter, "");
	StartUpCount = Parameter.System.Statistics.StartUpCounter.Value;
	//Ventil restart
	HelpFunctions_SendCommand(Parameter.System.Services.RestartController.ID, 0, &Parameter.System.Services.RestartController.MaxValue, Parameter.System.Services.RestartController.DataTyp, PARAMETER_SET, "Ventil neu gestartet");
	StartUpCount++;
	//Startzeit auf Null setzen
	StartRestart = Timer();
	//difftime Funktion welche den zeitlichen Unterschied liefert
	while((StartUpCount > Parameter.System.Statistics.StartUpCounter.Value) && (DiffRestart = Timer() - StartRestart) < 20)
	{
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Statistics.StartUpCounter, "");
	}
	if(StartUpCount != Parameter.System.Statistics.StartUpCounter.Value)
	{
		//Auf Oberfläche schreiben
		Report_AddText("Ventil Neustart fehlgeschlagen");
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Stellt die User oder Factory Settings wieder zurück
//  Input           : RestoreSetting: Entweder User oder Factory
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_RestoreParameters(UINT8 RestoreSetting)
{
	FLOAT TargetPosition = 0.0f;
	double DiffControlMode, StartControlMode;
	
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, "Abfrage Control Mode");
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "Abfrage Controller Type");
	if((Parameter.System.ControlMode.Value == CM_CLOSE || Parameter.System.ControlMode.Value == CM_INIT) && Parameter.System.Identification.Hardware.ControllerType.Value != CT_IC2H2)
	{
		//Wenn er im Close Ist muss er auf Position 0 fahren um einen Homing durchzuführen
		HelpFunctions_SetCommandFloat((struct ParameterType_GENERAL_INFORMATION *) &Parameter.PositionControl.TargetPosition, TargetPosition, "");
		HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode.Enum.Position, "");
		//Kontrolle ob in Position Mode gewechselt wurde
		StartControlMode = Timer();
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, ""); 
		while(Parameter.System.ControlMode.Value != CM_POSITION && (DiffControlMode = Timer() - StartControlMode) < REACHED_CONTROLMODE_MAX_TIME)	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, "");
		if(Parameter.System.ControlMode.Value != CM_POSITION)	Report_AddText("Fail: Control Mode Position nicht erreicht");
	}
	
	//Füt Position Only Tests muss der Acess Mode auf Local stehen
	if(Parameter.System.Identification.Hardware.ControllerType.Value == CT_IC2H2)
	{
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, "");
		if(Parameter.System.AccessMode.Value != AM_LOCAL)	HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode.Enum.Local, "");
	}
	
	//Anfangszustand laden
	RestoreCommand = TRUE;
	if(RestoreSetting == USER_RESTORE_PARAMETERS)					HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.StoreRestoreSettings.RestoreUserParameters, TRUE, "Der Restore User Parameter wurde auf True gesetzt");
	else if(RestoreSetting == FACTORY_RESTORE_PARAMETERS) HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.StoreRestoreSettings.RestoreFactoryParameters, TRUE, "Der Restore Factory Parameter wurde auf True gesetzt");
	RestoreCommand = FALSE;
	
	HelpFunctions_WaitForCommunicationAfterRestart();
	
	//Abfrage des Homing Start Condition
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.Homing.StartCondition, "");
	
	HelpFunctions_WaitHomingProcedur();	

	HelpFunctions_LocalMode();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Speichert die User oder Factory Parameter ab. Es wird geschaut, dass alle LearnBanken mit Daten versehen sind
//  Input           : StoreSetting: Entweder User oder Factory Parameter
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_StoreParameters(UINT8 StoreSetting)
{
	double StartCommunication, DiffCommunication;
	
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.ControllerType, "Abfrage Controller Type");
	//Service Interface Kommunikation muss auf Hex konstanter Länge umgestellt werden 
	HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Service.StringformatInteger, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Service.StringformatInteger.Enum.Hexadecimal_Constant_Length, "Service Stringformat Integer auf Hexadezimal konstanter Länge eingestellt");
	HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Service.StringformatFloatingPoint, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Service.StringformatFloatingPoint.Enum.Hexadecimal, "Service Stringformat Floating Point auf IEE754 Hexadezimal konstanter Länge eingestellt");

	//Interface Kommunikation muss auf Hex konstanter Länge umgestellt werden (Kontrole ob ein Interface vorhanden ist)
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceType, "Abfrage Hardware Interface");
	if(Parameter.System.Identification.Hardware.InterfaceType.Value != IT_NOVALUE && Parameter.System.Identification.Hardware.InterfaceType.Value < IT_ONBOARD)
	{
		HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Interface.StringformatInteger, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Interface.StringformatInteger.Enum.Hexadecimal_Constant_Length, "RS232 Interface Stringformat Integer auf Hexadezimal konstanter Länge eingestellt");
		HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Interface.StringformatFloatingPoint, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.Parameter.Channels.Interface.StringformatFloatingPoint.Enum.Hexadecimal, "RS232 Interface Stringformat Floating Point auf IEE754 Hexadezimal konstanter Länge eingestellt");
	} 
	
	
	//Homing Start Condition muss auf Standard stehen um bei einem Restore ein Homing ausgelöst wird
	HelpFunctions_SetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.Homing.StartCondition, (struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.Homing.StartCondition.Enum.Standard, "");
	
	
	if(Parameter.System.Identification.Hardware.ControllerType.Value != CT_IC2H2)
	{
		//Manuell alle Learn Daten füllen
		HelpFunctions_SetAllLearnDataManualy();
		//um die vielen Daten verarbeiten zu können
		Delay(1);
	}
	
	RestoreCommand = TRUE;
	if(StoreSetting == USER_STORE_PARAMETERS)					HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.StoreRestoreSettings.StoreUserParameters, TRUE, "Der Store User Parameter wurde auf True gesetzt");
	else if(StoreSetting == FACTORY_STORE_PARAMETERS) HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.StoreRestoreSettings.StoreFactoryParameters, TRUE, "Der Store Factory Parameter wurde auf True gesetzt");
	RestoreCommand = FALSE;
	
	//warten bis er wieder die Kommunikation aufnehmen kann
	StartCommunication = Timer();
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Diagnostics.StateInformation.TaskisRunning, "");
	while(Parameter.Diagnostics.StateInformation.TaskisRunning.Value != TRUE && (DiffCommunication = Timer() - StartCommunication < STORE_DELAY))	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Diagnostics.StateInformation.TaskisRunning, "");
	if(Parameter.Diagnostics.StateInformation.TaskisRunning.Value != TRUE)	Report_AddText("Das Ventil hat zulange um eine Kommunikastion nach einem Store Befehl aufzubauen");	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaltet in Local Mode um
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_LocalMode(void)
{
	//Acess Mode auf Local schalten
	HelpFunctions_SendCommand(Parameter.System.AccessMode.ID, 0, &Parameter.System.AccessMode.Enum.Local, Parameter.System.AccessMode.DataTyp, PARAMETER_SET, "Acess Mode auf Local geschaltet");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaltet in Remote Mode um
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_RemoteMode(void)
{
	//Acess Mode auf Remote schalten
	HelpFunctions_SendCommand(Parameter.System.AccessMode.ID, 0, &Parameter.System.AccessMode.Enum.Remote, Parameter.System.AccessMode.DataTyp, PARAMETER_SET, "Acess Mode auf Remote geschaltet");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Öffnet das Ventil
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_OpenVentil(void)
{
	HelpFunctions_SendCommand(Parameter.System.ControlMode.ID, 0, &Parameter.System.ControlMode.Enum.Open, Parameter.System.ControlMode.DataTyp, PARAMETER_SET, "Ventil geöffnet");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schliesst das Ventil
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_CloseVentil(void)
{
	HelpFunctions_SendCommand(Parameter.System.ControlMode.ID, 0, &Parameter.System.ControlMode.Enum.Close, Parameter.System.ControlMode.DataTyp, PARAMETER_SET, "Ventil geschlossen");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schliesst die USB Kommunikation
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_CloseUSBCommunication(void)
{
	UsbDisconnect(&UsbReferece, &LogReference);
	UsbConnected = 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet ein Set Befehl aus
//  Input           : *pParameterStruct		Adresse des Parameters
//									: *pParameterStructEnum	Adresse des Enums des selektionierten Parameters
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommand(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, struct ParameterType_GENERAL_INFORMATION *pParameterStructEnum, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, pParameterStructEnum, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet ein Set Befehl vom Typ Bolean welcher mit direktem Wertübergabe erfolgt
//  Input           : *pParameterStruct		Addresse des Parameters
//									: *pValueValve				Wert welcher gesetzt wird in BOOL8 Format
//									:	Channel: 						Definiert den Kanal über welchen Kommuniziert werden soll
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommandBool(struct ParameterType_GENERAL_INFORMATION *pParameterStruct,BOOL8 ValueValve, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, &ValueValve, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet ein Set Befehl vom Typ UINT8 welcher mit direktem Wertübergabe erfolgt
//  Input           : *pParameterStruct		Addresse des Parameters
//									: *pValueValve				Wert welcher gesetzt wird in UINT8 Format
//									:	Channel: 						Definiert den Kanal über welchen Kommuniziert werden soll
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommandUint8(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, UINT8 ValueValve, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, &ValueValve, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet ein Set Befehl vom Typ UINT32 welcher mit direktem Wertübergabe erfolgt
//  Input           : *pParameterStruct		Addresse des Parameters
//									: *pValueValve				Wert welcher gesetzt wird in UINT32 Format
//									:	Channel: 						Definiert den Kanal über welchen Kommuniziert werden soll
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommandUint32(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, UINT32 ValueValve, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, &ValueValve, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Führt ein Set Befehl vom Typ Float welcher mit direktem Wertübergabe erfolgt aus
//  Input           : *pParameterStruct		Addresse des Parameters
//									: *pValueValve				Wert welcher gesetzt wird in UINT32 Format 
//									:	Channel: 						Definiert den Kanal über welchen Kommuniziert werden soll
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommandFloat(struct ParameterType_GENERAL_INFORMATION *pParameterStruct,FLOAT Value, char *pCommandMessage)
{
	UINT32 *pValue;

	pValue = (UINT32 *) &Value;
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, pValue, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet ein Set Befehl aus, welcher der Wert als Addresse übergeben wird  
//  Input           : *pParameterStruct		Addresse des Parameters
//									: Value								Wert welcher gesetzt wird
//									: IndexParameter			Index des Parameteres
//									:	Channel: 						Definiert den Kanal über welchen Kommuniziert werden soll
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetCommandWithoutEnumValueIndex(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, void *pValue, UINT8 IndexParameter, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, IndexParameter, pValue, pParameterStruct->DataTyp, PARAMETER_SET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet den Get Befehl aus
//  Input           : *pParameterStruct		Adresse des Parameters
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetCommand(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, &pParameterStruct->pValue, pParameterStruct->DataTyp, PARAMETER_GET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet den Get Befehl aus (Es wird ein Index des Parameters abgefragt)
//  Input           : *pParameterStruct		Addresse des Parameters
//									:	IndexParameter:			Index des Parameters 
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetCommandIndex(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, UINT8 IndexParameter, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, IndexParameter, &pParameterStruct->pValue, pParameterStruct->DataTyp, PARAMETER_GET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet den Get Befehl aus (Es wird ein String Information abgefragt)
//  Input           : *pParameterStruct		Addresse des Parameters
//									:	pReceiveString:			Antwortstring des Ventils 
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetCommandString(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, char *pReceiveString, char *pCommandMessage)
{
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, pReceiveString, pParameterStruct->DataTyp, PARAMETER_GET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wertet den Get Befehl aus (Es wird ein String Information abgefragt, welcher bei einem Leerzeichen nicht abgebrochen wird)
//  Input           : *pParameterStruct		Addresse des Parameters
//									:	pReceiveString:			Antwortstring des Ventils 
//									: *pCommandMessage		String welcher in Report geschrieben wird 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_GetCommandStringWithSpaceChar(struct ParameterType_GENERAL_INFORMATION *pParameterStruct, char *pReceiveString, char *pCommandMessage)
{
	//DT_GET_NAME bricht bei einem Leerzeichen nicht ab
	HelpFunctions_SendCommand(pParameterStruct->ID, 0, pReceiveString, DT_GET_DATE, PARAMETER_GET, pCommandMessage);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den Computer Name zurück
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_GetComputerName(char *pComputerName)
{
	unsigned char *buffer;
	unsigned int size;

	// Assuming we do not know the length of string representing the computer name
	// Make the first call to RegReadString to determine the size.
	RegReadString (REGKEY_HKLM, "System\\CurrentControlSet\\Control\\ComputerName\\ComputerName", 
				   "ComputerName",NULL, 0, &size);

	// Allocating appropriate amount of memory
	buffer = (unsigned char*) malloc(size * sizeof(unsigned char));

	if (buffer != NULL) 
	{
		//Make the second call to RegReadString to fetch the Computer Name
		RegReadString (REGKEY_HKLM, "System\\CurrentControlSet\\Control\\ComputerName\\ComputerName",
		              "ComputerName", buffer, size, &size);
	
		strcpy(pComputerName, (char *)buffer);
		
		//Deallocate the memory
		free (buffer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Fragt die Umgebungsvariablen ab ob ein Fehler im Test aufgetreten ist
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
UINT8 HelpFunctions_AnalysisTest(void)
{
	//Auswertung Test
	if(strcmp(TestReturnValue, "UserCanceled") == 0)	return USERCANCELED;
	if(strcmp(TestReturnValue, "ErrorTest") == 0)			return ERRORTEST;
	return SUCCESFUL;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die vom User definiertes Zeitlimit noch nicht abgelaufen ist
//									: Wenn ja wird der TestStatus auf Cancel gesetzt
//  Input           : DurationTestCollection: Aktuelle Zeitdauer in welcher die TestKollektion ausgeführt worden ist
//  Output *Pointer : -
//         *return  : Gibt das Resultat retour ob die Zeit noch nicht abgelaufen ist
//******************************************************************************************************************************
UINT8 HelpFunctions_AnalyzeDuratingTestCollection(double StartDurationTestCollection)
{
	double ActualDuratingTestCollectionTime, StopDuratingTestCollection;
	
	ActualDuratingTestCollectionTime = (StopDuratingTestCollection = Timer() - StartDurationTestCollection);
	//Überprüfung mit dem User definierten Test Laufzeit, wenn die Zeit abgelaufen ist wird der TestStatus auf Cancel gesetzt
	if(ActualDuratingTestCollectionTime > DuratingTestCollectionMaxUser)	
	{
		return 1;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erzeugt einen globalen String mit dem Speicherort des vom Labview Erzeugten USB Textfile Protokoll
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_TextfileUSBComLogfileDirection(void)
{
	//USB Kommunikation raportieren 
	UINT16 StrlenTextfile, StrlenDirectionUsbLogFile, StrlenBetweenChar;
	StrlenBetweenChar = strlen(BETWEEN_CHAR);
	StrlenDirectionUsbLogFile = strlen(USB_COM_LOGFILE);
	StrlenTextfile = strlen(TEXT_FILE);
	char UsbTextFile[StrlenBetweenChar+StrlenDirectionUsbLogFile+StrlenTextfile+1];
	UsbTextFile[0] = 0;
	PathNameUsbLogFile[0] = 0;
	
	strcat(UsbTextFile, USB_COM_LOGFILE);
	strcat(UsbTextFile, TEXT_FILE);
	strcat(PathNameUsbLogFile, PathTTIC2Folder);
	strcat(PathNameUsbLogFile, UsbTextFile);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Setzt den Zeitoffset, da je nach Sommer- oder Winterzeit dieser sich unterscheidet
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetTimeOffset(void)
{
	BOOL SummerTime = FALSE;
	//für Zeitraportierung
	SYSTEMTIME str_t;
	
	GetSystemTime(&str_t);
	//Abfrage ob man sich alkuell in der Sommerzeit sich befindet
	if(str_t.wMonth >= MONTH_MARCH && str_t.wMonth <= MONTH_OCTOBER)
	{
		SummerTime = TRUE;
		//Beginn Grenze der Sommerzeit
		if(str_t.wMonth == MONTH_MARCH)
		{
			switch(str_t.wYear)
			{
				case 2015:	if(str_t.wDay < 29 && (str_t.wDay == 29 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2016:	if(str_t.wDay < 27 && (str_t.wDay == 27 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2017:	if(str_t.wDay < 26 && (str_t.wDay == 26 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2018:	if(str_t.wDay < 25 && (str_t.wDay == 25 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2019:	if(str_t.wDay < 31 && (str_t.wDay == 31 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2020:	if(str_t.wDay < 29 && (str_t.wDay == 29 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2021:	if(str_t.wDay < 28 && (str_t.wDay == 28 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2022:	if(str_t.wDay < 27 && (str_t.wDay == 27 && str_t.wHour < 2))	SummerTime = FALSE;
				case 2023:	if(str_t.wDay < 26 && (str_t.wDay == 26 && str_t.wHour < 2))	SummerTime = FALSE;
			}
		}
		if(str_t.wMonth == MONTH_OCTOBER)
		{
			switch(str_t.wYear)
			{
				case 2015:	if(str_t.wDay > 25 && (str_t.wDay == 25 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2016:	if(str_t.wDay > 30 && (str_t.wDay == 30 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2017:	if(str_t.wDay > 29 && (str_t.wDay == 29 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2018:	if(str_t.wDay > 28 && (str_t.wDay == 28 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2019:	if(str_t.wDay > 27 && (str_t.wDay == 27 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2020:	if(str_t.wDay > 25 && (str_t.wDay == 25 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2021:	if(str_t.wDay > 31 && (str_t.wDay == 31 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2022:	if(str_t.wDay > 30 && (str_t.wDay == 30 && str_t.wHour < 3))	SummerTime = FALSE;
				case 2023:	if(str_t.wDay > 29 && (str_t.wDay == 29 && str_t.wHour < 3))	SummerTime = FALSE;
			}
		}
	}
	if(SummerTime == TRUE)	TimeOffset = 2;
	else										TimeOffset = 1;
}

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
	//Initiallisiert die Zustände der User Optionen
	HelpFunctions_WriteTerminateByErrorOption();
	HelpFunctions_WritePassCommandOption();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wartet den Homing Prozedur ab und kontrolliert ob diese durchgeführt wurde 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WaitHomingProcedur(void)
{
	double StartWaitHoming, DiffWaitHoming, StartHomingStatus, DiffHomingStatus;
	
	//warten bis er in den Control Mode kommt
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, "");
	StartWaitHoming = Timer();
	while(Parameter.System.ControlMode.Value != CM_HOMING)
	{
		if((DiffWaitHoming = Timer() - StartWaitHoming) > TIME_START_HOMING_PROCEDURE)	
		{
			Report_AddText("Fail: Nicht in den Homing Control Mode gewechselt");
			break;
		}
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.ControlMode, "");
	}
	
	//schauen ob der Homing auch erfolgreich ausgeführt wurde
	StartHomingStatus = Timer();
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.Homing.Status, "");
	while(Parameter.Valve.Homing.Status.Value != HS_COMPLETED_SUCESSFULLY)
	{
		//Additional Delay für den DefaultConfig Befehl
		if((DiffHomingStatus = Timer() - StartHomingStatus) > HomingProcessTime)	
		{
			Report_AddText("Fail: Homing Prozess konnte nicht abgeschlossen werden");
			break;
		}
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.Homing.Status, "");
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wartet bis mit dem Ventil nach einem Restart wieder kommuniziert werden kann 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WaitForCommunicationAfterRestart(void)
{
	double StartCommunication, DiffCommunication;
	
	//warten bis Kommunikation mit dem Ventil verloren geht
	Delay(LOST_COMMUNICATION_AFTER_RESTART);
	
	//warten bis die Kommunikation mit dem Ventil wieder möglich ist, (Bei einem normalen Restart werden 8s mind gemessen)
	StartCommunication = Timer();
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Diagnostics.StateInformation.TaskisRunning, "");
	while(Parameter.Diagnostics.StateInformation.TaskisRunning.Value != TRUE && (DiffCommunication = Timer() - StartCommunication) < SET_DEFAULT_DELAY)	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Diagnostics.StateInformation.TaskisRunning, "");
	if(Parameter.Diagnostics.StateInformation.TaskisRunning.Value != TRUE)	
	{
		Report_AddText("Fail: Das Ventil hat zulange um eine Kommunikastion nach einem Restart aufzubauen");
		//Auswertungsfunktion
		while(Parameter.Diagnostics.StateInformation.TaskisRunning.Value != TRUE && (DiffCommunication = Timer() - StartCommunication) < 10)
		{
			Delay(0.1);
			HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Diagnostics.StateInformation.TaskisRunning, "");
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Setzt die Terminate By Error Einstellung 
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WriteTerminateByErrorOption(void)
{
	int TerminateByErrorInt;
	
	GetCtrlVal(STpanel, STPANEL_BT_TERMINATECONDITION, &TerminateByErrorInt);
	TerminateByError = (BOOL8)TerminateByErrorInt;
	if(TerminateByError == FALSE)			HelpFunctions_WriteGlobalVariableTestOptions(TERMINATE_BY_ERROR_OPTION_INDEX, "0");
	else if(TerminateByError == TRUE)	HelpFunctions_WriteGlobalVariableTestOptions(TERMINATE_BY_ERROR_OPTION_INDEX, "1");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Setzt die PassCommand Einstellung ab
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WritePassCommandOption(void)
{
	int ValuePassCommandButton;
	GetCtrlVal(STpanel, STPANEL_PASSCOMMANDBUTTON, &ValuePassCommandButton);
	//Global Variable TestOptions setzen, wenn der User dieses Feld ausgewählt hat
	if(ValuePassCommandButton == 0)	HelpFunctions_WriteGlobalVariableTestOptions(PASS_COMMAND_OPTION_INDEX, "0");
	else if(ValuePassCommandButton == 1)	HelpFunctions_WriteGlobalVariableTestOptions(PASS_COMMAND_OPTION_INDEX, "1");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den aktuellen Wert in die TestOptions Global Variable
//										Index 0: Info File Funktion im Test ausführen, Index 1: Keine Pass Command im Report
//										Index 2: Ob die Kollektion abgebrochen werden soll bei einem Fehler, 
//										Index 3: Um die TestVersion in die CurrentTestsVersion Tabelle zu schreiben
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

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den User Namen in die Globale Variable(wird im Test für die Zertifizierung gebraucht)
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_WriteUserName(void)
{
	UINT8 StrlenUserNameString = 0;
	char GlobalVariableUserName[USERNAME_GLOBALVARIABLE];
	
	StrlenUserNameString = (UINT8)strlen(User);
	if(StrlenUserNameString != 0)	
	{	
		strcpy(GlobalVariableUserName, "USERNAME=");
		strcat(GlobalVariableUserName, User);
		//Ändert die Globale Variable
		_putenv(GlobalVariableUserName);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt, wenn ein Cluster Test ausgwählt ist, die Anzahl Slaves in die Globale Variable(um nicht später den User nicht danach zu fragen)
//  Input           :
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SaveNumberOfSlaves(void)
{
	UINT16 IndexTestList;
	SINT32 OptionTypesId = -1;
	char NumberOfSlavesString[NUMBER_OF_SLAVES_LENGTH], GlobalVariableNumberOfSlaves[USERNAME_GLOBALVARIABLE];
+++
	OptionTypesId = Database_GetSingleOptionTypeIdWithEnum("Cluster");

	//Zuerst überprüfen ob ein Cluster Test vorliegt
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
	{
		if(Database_IsOptionNeededFromTest(pSelectedTests[IndexTestList].pTests->TestVersionId, OptionTypesId) == TRUE)	
		{
			PromptPopup("Cluster System", "Enter number of Slaves(Master included)", NumberOfSlavesString, NUMBER_OF_SLAVES_LENGTH - 1);
			strcpy(GlobalVariableNumberOfSlaves, "NUMBEROFSLAVES=");
			strcat(GlobalVariableNumberOfSlaves, NumberOfSlavesString);
			_putenv(GlobalVariableNumberOfSlaves);
			break;
		}
	}
}
---







//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den ValveSeries String in den Parameterzahlenwert um, um anschlissend mit der Hardware vergleichen zu könenn
//  Input           : *ValveSeriesString		String des ValveSeries
//  Output *Pointer : -
//         *return  : Den ValveSeries Zahlenwert
//******************************************************************************************************************************
SINT16 HelpFunctions_TranslateValveSeriesStringToValue(char *ValveSeriesString)
{
	//Test_R61_2-> 7 Zeichen 1
	switch(ValveSeriesString[6])
	{
		case '1':	switch(ValveSeriesString[8]) 
		{
			case '2': return 612;
			case '5': return 615;
			default : return -1;
		}
		case '5': switch(ValveSeriesString[8])
		{
			case '0':	return 650;
			case '1': return 651;
			case '2': return 652;
			case '5': return 655;
			default	: return -1;
		}
	}
	return -1;								
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Controller String in den Parameterzahlenwert um, um anschlissend mit der Hardware vergleichen zu könenn
//  Input           : *ControllerString		String des Controllers
//  Output *Pointer : -
//         *return  : Den Controller Zahlenwert
//******************************************************************************************************************************
SINT32 HelpFunctions_TranslateControllerStringToValue(char *ControllerString)
{
	//Test_IC2H1
	switch(ControllerString[5])
	{
		case 'I':	switch(ControllerString[9])
		{
			case '1':	
			{
				if(ControllerString[10] == '3')	return 4;
				else														return 1;
			}
			case '2':	return 2;
			case '3': return 3;
		}
		case 'U':	return 65535;
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Interface String in den Parameterzahlenwert um, um anschlissend mit der Hardware vergleichen zu könenn
//  Input           : *InterfaceString		String des Interface Parameters
//  Output *Pointer : -
//         *return  : Den Interface Zahlenwert
//******************************************************************************************************************************
SINT32 HelpFunctions_TranslateInterfaceStringToValue(char *InterfaceString)
{	 
	switch(InterfaceString[5])
	{
		case 'R':	return	1;
		case 'E':	return 	2;
		case 'D':	return 	3;
		case 'O': return  4;
		case 'L': return  5;
		case 'N': return 	65534;
		case 'U':	return  65535;
	}
	return -1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Option String in den Parameterzahlenwert um, um anschlissend mit der Hardware vergleichen zu könenn
//  Input           : *OptionString		String des Option Parameters
//  Output *Pointer : -
//         *return  : Den Option Zahlenwert
//******************************************************************************************************************************
SINT16 HelpFunctions_TranslateOptionStringToValue(char *OptionString)
{
	UINT16 Strlen_OptionString = 0;
	Strlen_OptionString = (UINT16)strlen(OptionString);
	//Test_Cluster+sPFO+SPS
	switch(OptionString[5])
	{
		case 'C':																		return 	3;
		case 's':	if(OptionString[10] == 'C')				return	10;
							else															return 	8;
		case 'S':	
		{
			if(Strlen_OptionString == 8)							return	1;
			else
			{
				switch(OptionString[9])
				{
					case 'P':															
					{
						if(Strlen_OptionString == 12)				return 	4;
						else																return 	7;
					}
					case 'C':															return 	5;
					case 's':															
					{
						if(OptionString[14] == 'C')					return 	11;
						else																return 	9;
					}
				}
			}
		}
		case 'P':
		{
			if(Strlen_OptionString == 8)							return 	2;
			else																			return 	6;
		}
		case 'N':																		return	0;
		case 'U':																		return 	255;
		default:																		return 	-1;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die NI Hardware vorhnanden ist um den Power Stecker speisen zu können. Wenn ja dann wird die digitalen IO gespiesen.
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_InitPowerConnector(void)
{
	BOOL8 PowerSupplyAvaiable; 
	
#if (VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<1) && VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<3)) && ((TEST_HARDWARE_CODE == 1) || (TEST_HARDWARE_CODE == 0))
	
	//Einspeisegerät initialisieren -> muss Eingesteckt sein, ansosnten kann die NI Hardware nicht verwendet werden
	PowerSupplyAvaiable = SupplyValve_InitPowerSupply();
	
	//DI initiallisieren für den Power Stecker
	if(PowerSupplyAvaiable == TRUE)	PowerConnectorHardwareAvaiable = PC_IC2H1_HelpFunctions_PowerConnectorHardwareAvaiable();

	if(PowerConnectorHardwareAvaiable)
	{
		CreateDAQTask_PC_IC2H1_DO(&TaskHandlePowerConnectorIC2H1DO);
		CreateDAQTask_PC_IC2H1_DI(&TaskHandlePowerConnectorIC2H1DI);
		ReadWriteDIO_PC_IC2H1_InitDILogicIO(TaskHandlePowerConnectorIC2H1DI);
	}
#endif
#if (VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<2)) && ((TEST_HARDWARE_CODE == 2) || (TEST_HARDWARE_CODE == 0)) 
	PowerSupplyAvaiable = SupplyValve_InitPowerSupply();
	
	//DI initiallisieren für den Power Stecker
	if(PowerSupplyAvaiable == TRUE)	PowerConnectorHardwareAvaiable = PC_IC2H2_HelpFunctions_PowerConnectorHardwareAvaiable();

	if(PCHardwareIC2H2Avaiable)
	{
		CreateDAQTask_PC_IC2H2_DO(&TaskHandlePowerConnectorIC2H2DO);
		CreateDAQTask_PC_IC2H2_DI(&TaskHandlePowerConnectorIC2H2DI);
		ReadWriteDIO_PC_IC2H2_InitDILogicIO(TaskHandlePowerConnectorIC2H2DI);
	}
#endif
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wenn die Power Connector Hardware vorhanden ist, wird die Speisung wieder geschlossen
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_TerminatePowerConnector(void)
{	
	//Power Supply Port schliessen, wenn dieser geöffnet ist
	if(strcmp(ComPortSupply, "") != 0)	CloseVisa(ComPortSupply);
	
#if (VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<1) && VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<3)) && ((TEST_HARDWARE_CODE == 1) || (TEST_HARDWARE_CODE == 0))
	BOOL8 PCHardwareIC2H1Avaiable = FALSE;
	
	PCHardwareIC2H1Avaiable = PC_IC2H1_HelpFunctions_PowerConnectorHardwareAvaiable();

	if(PCHardwareIC2H1Avaiable == TRUE)
	{
		//Grundzustand für die DI herstellen und die Tasks beenden
		ReadWriteDIO_PC_IC2H1_EndDILogicIO(TaskHandlePowerConnectorIC2H1DI);
		DAQmxClearTask(TaskHandlePowerConnectorIC2H1DO);
	}
#endif

#if (VATFIRMWARE_COMPATIBLECONTROLLERHW & (1<<2)) && ((TEST_HARDWARE_CODE == 2) || (TEST_HARDWARE_CODE == 0)) 
	BOOL8 PCHardwareIC2H2Avaiable = FALSE;

	PCHardwareIC2H2Avaiable = PC_IC2H2_HelpFunctions_PowerConnectorHardwareAvaiable();

	if(PCHardwareIC2H2Avaiable == TRUE)
	{
		//Grundzustand für die AI und DI herstellen und die Tasks beenden
		ReadWriteDIO_PC_IC2H2_EndDILogicIO(TaskHandlePowerConnectorIC2H2DI);
		DAQmxClearTask(TaskHandlePowerConnectorIC2H2DO);
	}
#endif
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Es werden alle Learnbanken mit fixen Learndaten gefüllt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void HelpFunctions_SetAllLearnDataManualy(void)
{
#ifndef PLATFORMCONFIG_PRESSURECONTROL_NA
	UINT8 DataTableIndex;
	UINT32 LearnBankData[INDEX_DATA_TABLE_MAX] = {1, 					1074330355, 6, 					4059077840, 1080460041, 1078840758, 1077221475, 1075602192, 1073982909, 1070985428,
																							 	1067746863, 1063663377, 1059207921, 1056913741, 1053268928, 1048730978, 1043922117, 1041506397, 1040298535, 1036830838,
																								1031820457, 1025579151, 1022021982, 1018801023, 1017011603, 1015937951, 1015222181, 1013991264, 1013275494, 1012559724,
																								1012187524, 1011843954, 1011614904, 1011414494, 1011271344, 1011128193, 0,					0,					0,					0,
																								0, 					0,					0,					0,					0,					0,					0,					0,					0,					0,
																								0, 					0,					0,					0,					0,					0,					0,					0,					0,					0,
																								0, 					0,					0,					0,					0,					1008981770, 1017370378, 1022739087, 1025758986, 1028443341,
																								1031127695, 1032805417, 1034147594, 1035489772, 1036831949, 1039516303, 1041194025, 1042536202, 1043878380, 1045220557,
																								1048576000, 1050253722, 1051931443, 1053609165, 1055286886, 1056964608, 1057803469, 1058642330, 1059481190, 1060320051,
																								1061158912, 1061997773, 1062836634, 1063675494, 1064514355, 1065353216, 3212836864, 3212836864, 3212836864, 3212836864,
																								3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864,
																								3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864, 3212836864,
																								3212836864, 3212836864, 3212836864, 3212836864, 1077321750};
	
	//Fixe Learndaten in alle Banken schreiben
	for(DataTableIndex = 0; DataTableIndex < INDEX_DATA_TABLE_MAX; DataTableIndex++)
	{
		//Daten von Learnbank1 übertragen
		HelpFunctions_SetCommandWithoutEnumValueIndex((struct ParameterType_GENERAL_INFORMATION *) &Parameter.PressureControl.AdaptiveLearn.LearnBank1.Data, &LearnBankData[DataTableIndex], DataTableIndex, "");
		HelpFunctions_SetCommandWithoutEnumValueIndex((struct ParameterType_GENERAL_INFORMATION *) &Parameter.PressureControl.AdaptiveLearn.LearnBank2.Data, &LearnBankData[DataTableIndex], DataTableIndex, "");
		HelpFunctions_SetCommandWithoutEnumValueIndex((struct ParameterType_GENERAL_INFORMATION *) &Parameter.PressureControl.AdaptiveLearn.LearnBank3.Data, &LearnBankData[DataTableIndex], DataTableIndex, "");
		HelpFunctions_SetCommandWithoutEnumValueIndex((struct ParameterType_GENERAL_INFORMATION *) &Parameter.PressureControl.AdaptiveLearn.LearnBank4.Data, &LearnBankData[DataTableIndex], DataTableIndex, "");
	}
#endif
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt eine Mitteilung auf das Report Fenster(Vor und nach der Anweisung hat es eine Leerzeile).
//  Input           : pMessage:	Mitteilung, welche auf das Report Fenster geschrieben wird
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void HelpFunctions_WriteMessageOnReportPanel(char *pMessage, UINT8 StatusMessage)
{
	int NumberListItemsReport;
	
	InsertListItem (RPpanel, RPPANEL_TREE_REPORT, -1, "", 0);
	GetNumListItems(RPpanel, RPPANEL_TREE_REPORT, &NumberListItemsReport);
	InsertListItem (RPpanel, RPPANEL_TREE_REPORT, NumberListItemsReport, pMessage, 0);
	switch(StatusMessage)
	{
		case MESSAGE_DEFAULT:	SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT, NumberListItemsReport, 0, ATTR_LABEL_BGCOLOR, VAL_BLACK);	break;
		case MESSAGE_PASS:		SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT, NumberListItemsReport, 0, ATTR_LABEL_BGCOLOR, VAL_GREEN);	break;
		case MESSAGE_WARNING:	SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT, NumberListItemsReport, 0, ATTR_LABEL_BGCOLOR, 0xFFA500);		break;
		case MESSAGE_ERROR:		SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT, NumberListItemsReport, 0, ATTR_LABEL_BGCOLOR, VAL_RED);		break;
		default:																																																														break;
	}
	InsertListItem (RPpanel, RPPANEL_TREE_REPORT, -1, "", 0);
	GetNumListItems(RPpanel, RPPANEL_TREE_REPORT, &NumberListItemsReport);
	
	SetCtrlAttribute (RPpanel, RPPANEL_TREE_REPORT, ATTR_CTRL_INDEX, NumberListItemsReport-1);
}








