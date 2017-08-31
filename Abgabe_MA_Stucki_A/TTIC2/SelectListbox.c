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
#include <rs232.h>
#include <cvirte.h>
#include <userint.h>
//um Konflikte zu vermeiden, muss die windows Libary vor der utility definiert werden
#include <windows.h>
#include <formatio.h>
#include <utility.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <toolbox.h>
//um die Funktion von windows.h benützen zu können
#undef GetSystemTime
#undef PI;

#include "Database.h"
#include "Definelist.h"
#include "DefineStructFileService.h"
#include "HelpFunctions.h"
#include "InterfaceCommunication.h"
#include "Report.h"
#include "ParameterProcessing.h"
#include "PlatformConfig.h"
#include "SelectListbox.h"
#include "SurfaceTest.h"
#include "SharedVariable.h"
#include "Typedefinitions.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void BuildTestResultEntrys(void);
static void CheckIfDriveAndConfigurationFileAreActual(void);
static void CheckIfFirmwareAreActual(void);
static void CheckOneTimeValveHardware(UINT8 ChangeHardwareSetting);
static void CheckMultipleTimeValveHardware(UINT8 ChangeHardwareSetting);
static void InitializeReportPanel(void);
static void InitTestCollectionEnvironment(char *pTestPathNames);
void TranslateDateTimeInDatabaseFormat(char *pFirmwareReleaseTime);
static void RunTestExe(struct test_type *pTestType);
static void SetInitialStateFirmware(void);
static void UserNotWantChangeHardwareWriteOnReportTree(void);
static void WriteTestResultInReport(void);
static void WriteTestCollectionResult(void);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
BOOL8 NeedDiffrentHardware = FALSE;
UINT8 UserWantChangeHardware = 0, UserCommitHardwareChange = 0, IsInterfaceCommunicationAvaiable = 0, TestResult = SUCCESFUL, DuratingTestCollectionStatus = 0;
SINT16 CountErrorTests;
SINT32 TestCollectionResultId;
int LedState = 0, IndexTestList, FirstReport = 0;
double StartTestCollectionTime;
char User[USER_LENGTH], FileName[MAX_PATHNAME_LEN]="";

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : führt die markierten Files in der rechten Liste(ausführbare Tests) aus 
//  									und schreibt die Informationen vom Test in das Report Fenster
//  Input           : pTestPathNames:	Gibt den Startpfad an, wo die Test abgelegt sind
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_RunExeFilesFunction(char *pTestPathNames)
{
	//Überprüfung ob die benötigte Hardware vorhanden ist
	BOOL8 ExecutedAllTests = 0, FixedTestsOrder;
	UINT8 TestNeedDifferentHardware = 0, IterationTestCollection = 0;
	UINT16 IterationTestCollectionUser = 0;
	int StateSequencerButton = 0;
	//nur einmal initialisieren
	static int WriteReportOnScreen = 0;
	
	//Diagnose File + Anzahl Slaves + Umgebungsvariable + NI Hardware + Hardware Check + Report File + Globale Variable + Anzahl fehlerhafte Tests auf 0 + Test Result auf Successfull setzen
	InitTestCollectionEnvironment(pTestPathNames);
	
	//Lädt die benötigten Firmwares sowie werden die Ventilparameter gesetzt
	if(UserCommitHardwareChange != CANCEL_BUTTON && UserCommitHardwareChange != USER_PUSH_NO_BUTTON)	
	{
		SetInitialStateFirmware();
	
		//InitialStateFirmware + ValveHardware + TestCollectionResult Eintrag in Datenbank erstellen
		BuildTestResultEntrys();
	
		//Speisung wieder freigeben, für die einzelnen Tests
		HelpFunctions_TerminatePowerConnector();
		//neue Zeile im Report und auf der Reportoberfläche zu erzeugen. Es braucht ein Leerzeichen, da bei Kontrolle des Leerzeichen auf dem Reportoberfläche -2 gerechnet wird
		Report_AddText(" \n");
		HelpFunctions_CloseUSBCommunication();
		
		//Überprüfen ob die Test Collection angelegt werden konnte in der Datenbank
		if(TestCollectionResultId >= 0)
		{
			//setzen wenn der User keine Anzahl Iterationen ungleich 1 ausgewählt hat
			if(IterationTestCollectionMaxUser == 0)	IterationTestCollectionMaxUser = 1; 
			//Der User kann entweder die Anzahl durchläufe der TestKollektion oder deren Dauer festlegen
			if(ExecutionButtonValue == DURATING)	IterationTestCollectionMaxUser = 2;
			//Um die Testkollektion mehrfach ausführen zu können
			for(IterationTestCollectionUser = 0; IterationTestCollectionUser < IterationTestCollectionMaxUser; IterationTestCollectionUser++)
			{
				//Wenn die Zeit den Abruch der TestKollektion bestimmt, sollte die Anzahl der Durchläufe immer wider zurück gesetzt werden
				if(ExecutionButtonValue == DURATING && IterationTestCollectionUser != 0) IterationTestCollectionUser = 0;
				//reseten um den Durchgang zu widerholen, ausser wenn der User den Test abgebrochen hat
				if(TestResult != USERCANCELED)	ExecutedAllTests = 0;
				//Alle Tests wider den Status ausgeführt aufheben für den nächsten Durchgang
				for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)	pSelectedTests[IndexTestList].Executed = 0;	
				//ExecutedAllTests Variable wird erst 0 wenn alle Tests durchgeführt worden sind
				while(ExecutedAllTests != 1)
				{
					for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
					{
						//Reihenfolge der Tests je nach Sequencer Button(wenn nicht markiert wird je nach Hardware ausgeführt, wenn markiert wird der Reihe nach die Tests ausgeführt
						GetCtrlVal (STpanel, STPANEL_SEQUENCER_BUTTON, &StateSequencerButton);
						FixedTestsOrder = (BOOL8)StateSequencerButton;
			
						//Wenn der Testablauf nicht fix ist (Zuerst alle Tests, die möglich sind)
						if(FixedTestsOrder == FALSE)
						{
							//Wenn der Test noch nicht ausgeführt worden ist, wird die exe mit system aufgerufen  
							if (strlen(pSelectedTests[IndexTestList].pTests->TestName) != 0 && pSelectedTests[IndexTestList].Executed == 0)	
							{
								CheckHardwareState = TRUE;
								//Muss nur kontrolliert werden, wenn bei der ersten Überprüfung nicht alles in Ordnung war und der Benutzer trotzdem die Kollektion starten möchte
								if(UserCommitHardwareChange == OK_BUTTON)
								{
									NeedDiffrentHardware = ChangeHardware_NeedfulHardwareForTest(pSelectedTests, IndexTestList);
									IsInterfaceCommunicationAvaiable = ChangeHardware_AvaiableInterfaceCommunication(pSelectedTests, IndexTestList);
								}
						
								if(NeedDiffrentHardware == FALSE && IsInterfaceCommunicationAvaiable == ERROR_FREE)	
								{
									CheckHardwareState = FALSE;
									//Wenn der User die Zeitdauer der TestKollektion bestimmt, wird hier geschaut, ob das Zeitlimit noch nicht abgelaufen ist
									if(DuratingTestCollectionMaxUser > 0)	DuratingTestCollectionStatus = HelpFunctions_AnalyzeDuratingTestCollection(StartTestCollectionTime);
									if(ExecutionButtonValue == DURATING && DuratingTestCollectionStatus == TIMEEXPIRED)	TestResult = USERCANCELED;
									if(TestResult == SUCCESFUL || (TestResult == ERRORTEST && TerminateByError == FALSE))	RunTestExe(pSelectedTests[IndexTestList].pTests);
								}
								else if(IterationTestCollection != 0)
								{
									if(UserCommitHardwareChange != HARDWARE_OK)	CheckMultipleTimeValveHardware(CH_SETTING_DESCRIBE_DIFFRENT_HW);
									CheckHardwareState = FALSE;
									//wird nicht ausgeführt, wenn der User die Hardware nicht wechseln möchte
									if(NeedDiffrentHardware == FALSE && IsInterfaceCommunicationAvaiable == ERROR_FREE)
									{
										//Zeitlimite ausgewählt für den Ausführungsmodus, so soll jetzt geprüft werden, ob dieses Limit noch nicht erreicht wurde
										if(DuratingTestCollectionMaxUser > 0)	DuratingTestCollectionStatus = HelpFunctions_AnalyzeDuratingTestCollection(StartTestCollectionTime);
										if(ExecutionButtonValue == DURATING && DuratingTestCollectionStatus == TIMEEXPIRED)	TestResult = USERCANCELED;
										if(TestResult == SUCCESFUL || (TestResult == ERRORTEST && TerminateByError == FALSE))
										{
											//um beim nächsten Test nicht gleich wider die Hardware wechseln zu müsssen sondern einen ganzen Durchlauf macht 
											IterationTestCollection = 0;
											TestNeedDifferentHardware = 0;
											RunTestExe(pSelectedTests[IndexTestList].pTests);
										}
									}
									//Hardware wird nicht gewechselt, so dass der Test ausgeführt werden kann
									else	UserNotWantChangeHardwareWriteOnReportTree();
								}
								//Der Test wurde noch gar nicht gestartet 
								else	TestNeedDifferentHardware = 1;
							}
							//Keine Tests mehr in der auszuführenden Liste, wenn in dieser Zeit die Anforderung an die Hardware immer überein stimmte, kann die Routine verlassen werden
							if(IndexTestList == CountSelectedFiles - 1)
							{
								IterationTestCollection++;
								if(TestNeedDifferentHardware == 0 || UserWantChangeHardware == USER_NOT_WANT)	ExecutedAllTests = 1;
							}
						}
						//Verwendung als Sequenzer, d.h. die Reihenfolge wird nicht geändert
						else
						{
							if (strlen(pSelectedTests[IndexTestList].pTests->TestName))	
							{
								CheckHardwareState = TRUE;
								//Muss nur kontrolliert werden, wenn bei der ersten Überprüfung nicht alles in Ordnung war und der Benutzer trotzdem die Collection starten möchte
								if(UserCommitHardwareChange == OK_BUTTON)
								{
									NeedDiffrentHardware = ChangeHardware_NeedfulHardwareForTest(pSelectedTests, IndexTestList);
									IsInterfaceCommunicationAvaiable = ChangeHardware_AvaiableInterfaceCommunication(pSelectedTests, IndexTestList);
								}
								if(NeedDiffrentHardware == FALSE && IsInterfaceCommunicationAvaiable == ERROR_FREE)	
								{
									CheckHardwareState = FALSE;
									//Wenn der User die Zeitdauer der TestKollektion bestimmen möchte, muss bevor der einzelne Test ausgeführt wird geschaut werden ob das Limit noch nicht überschritten wurde
									if(DuratingTestCollectionMaxUser > 0)	DuratingTestCollectionStatus = HelpFunctions_AnalyzeDuratingTestCollection(StartTestCollectionTime);
									if(ExecutionButtonValue == DURATING && DuratingTestCollectionStatus == TIMEEXPIRED)	TestResult = USERCANCELED;
									if(TestResult == SUCCESFUL || (TestResult == ERRORTEST && TerminateByError == FALSE))	RunTestExe(pSelectedTests[IndexTestList].pTests);
								}
								//hier gleich User anfordern die Hardware zu wechseln
								else	
								{
									if(UserCommitHardwareChange != HARDWARE_OK)	CheckMultipleTimeValveHardware(CH_SETTING_DESCRIBE_DIFFRENT_HW);
									CheckHardwareState = FALSE;
									//wird nicht ausgeführt, wenn der User die Hardware nicht wechseln möchte
									if(NeedDiffrentHardware == FALSE && IsInterfaceCommunicationAvaiable == ERROR_FREE)	RunTestExe(pSelectedTests[IndexTestList].pTests);
									//um nichts in das Report Fenster zu schreiben, da der Test nicht ausgeführt wird
									else	UserNotWantChangeHardwareWriteOnReportTree();						
								}
							}
							//Keine Tests mehr in der auszuführenden Liste, wenn in dieser Zeit die Anforderung an die Hardware immer überein stimmte, kann die Routine verlassen werden
							if(IndexTestList == CountSelectedFiles - 1)	ExecutedAllTests = 1;
						}
						WriteReportOnScreen = 0;
						FirstReport = 1;
			
						//Wenn der Test vom User beendet worden ist oder der Test einen Fehler hatte(ausser User will die Kollektion nicht unterbrechen) wird der Status ExecutedAllTests gesetzt und die Ausführeung der Kollektion beendet
						if((TestResult == USERCANCELED) || (TestResult == ERRORTEST && TerminateByError == TRUE))
						{
							ExecutedAllTests = 1;
							IterationTestCollectionUser = (UINT16)(IterationTestCollectionMaxUser - 1);
							break;
						}
				
					}	//end for all list elements
				}	//end while Durchgang bis alle Tests erledigt sind
			
				//Abort + CountErrorTests in TestCollectionResult eintragen
				WriteTestCollectionResult();
			}//User hat die Hardware nicht wechseln wollen
		}//Kontrolle, das ein TestResultatCollection Eintrag in der Datenbank erstellt worden ist
		else	Report_AddText("\nKonnte kein Test Collection Resultat Eintrag in der Datenbank erstellt werden");	

		//Report Oberfläche LED ausschalten (In Process) 
		InProcessState = FALSE;
	
		WriteTestResultInReport();
	}
}	//end function

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : löscht den Aray Eintrag des Files welches in der rechten Liste(auszuführende Tests) ausgewählt ist 
//  Input           : EmptyTestStructure:	Ein leerer Stuktur Eintrag um die Informationen zu ersetzen
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_RemoveSelectFile(struct test_type EmptyTestStructure)
{
	int IndexTestList, k;
	
	//wissen wie viele Tests bereits in der Selektionierten Liste sind
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &CountSelectedFiles); 
	
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
	{
		if(pSelectedTests[IndexTestList].pTests->SelectTestSelected == 1)
		{
			DeleteListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexTestList, 1);
			//after the remove entry, the follow entry has to move up
			for(k = IndexTestList; k < CountSelectedFiles - 1; k++)
			{
				pSelectedTests[k] = pSelectedTests[k+1];
			}
			//the last listing to remove
			if(k == CountSelectedFiles - 1) 
			{
				pSelectedTests[k].pTests = &EmptyTestStructure;
				CountSelectedFiles--;
				IndexTestList--;
			}
		}	//end if checked
	}	//end for CountSelectedFiles
}	//end function

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : der Index des markierten File in der rechten Liste(auszuführende Tests) wird um eins verkleinert
//										sowie der nächst obere Index um eins erhöht wird
//  Input           : EmptyTestStructure:	Ein leerer Stuktur Eintrag um die Informationen zu ersetzen
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_DecrementIndexSelectedFile(struct testSelected_type *pTemp)
{
	int IndexSelectedFile = 0;
	GetCtrlIndex(STpanel, STPANEL_TREE_TESTCOLLECTION, &IndexSelectedFile);
	pTemp = (struct testSelected_type *)malloc(sizeof(struct testSelected_type) * 1);
	
	//ist das erste Element ausgewählt, so ist es nicht möglich den Index zu verkleinern
	if(IndexSelectedFile != 0)	
	{
		//eins tifere Index wird gelöscht 
		DeleteListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile - 1, 2);
		//die Informationen vom markirten Test werden im eins kleineren Index abgespeichert
		pTemp->pTests = pSelectedTests[IndexSelectedFile - 1].pTests;
		pSelectedTests[IndexSelectedFile - 1] = pSelectedTests[IndexSelectedFile];
		pSelectedTests[IndexSelectedFile].pTests = pTemp->pTests;
		InsertListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile - 1, pSelectedTests[IndexSelectedFile - 1].pTests->TestName, 0);
		InsertListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile, pSelectedTests[IndexSelectedFile].pTests->TestName, 0);
		SetCtrlIndex(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile - 1);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : der Index des markierten File in der rechten Liste(auszuführende Tests) wird um eins vergrösert
//										sowie der nächst untere Index um eins verkleinert wird
//  Input           : EmptyTestStructure:		Ein leerer Stuktur Eintrag um die Informationen zu ersetzen
//									:	MaxIndex:	Anzahl vorhandene Test welche in der Testzusammenstellungsfenster zu sehen sind
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_IncrementIndexSelectFile(int MaxIndex, struct testSelected_type *pTemp)
{
	int IndexSelectedFile = 0;
	GetCtrlIndex(STpanel, STPANEL_TREE_TESTCOLLECTION, &IndexSelectedFile);
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &MaxIndex);
	pTemp = (struct testSelected_type *)malloc(sizeof(struct testSelected_type) * 1);
	
	//if the first item is selected, so it isn't possible to decrement the index
	if(IndexSelectedFile != MaxIndex - 1)	
	{
		//to remove the one lower and the actual File in the textFile (2,remove the next 2 files) 
		DeleteListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile, 2);
		//the information of the selected file to save it in the nextr lower index
		pTemp->pTests = pSelectedTests[IndexSelectedFile + 1].pTests;
		pSelectedTests[IndexSelectedFile + 1] = pSelectedTests[IndexSelectedFile];
		pSelectedTests[IndexSelectedFile].pTests = pTemp->pTests;
		InsertListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile, pSelectedTests[IndexSelectedFile ].pTests->TestName, 0);
		InsertListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile + 1, pSelectedTests[IndexSelectedFile + 1].pTests->TestName, 0);
		SetCtrlIndex(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexSelectedFile + 1);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : aktuallisiert die Arrayliste der auszuführenden Tests nach einem Drag&Drop Event
//  Input           : TempFreeOperation:	Gibt an ob der erszeugte leere Struktur Eintrag schon gelöscht worden ist
//									: StartIndex:					Gibt an wo der Test positioniert war vor dem Drag and Drop des Users
//									:	IndexAfterDropEvent:Gibt an wo der Test positioniert ist nach dem Drag and Drop des Users
//									: EmptyTestStructure:		Ein leerer Stuktur Eintrag um die Informationen zu ersetzen
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_ActualSelectFileListAfterDragDrop(UINT8 TempFreeOperation, int StartIndex, int IndexAfterDropEvent, struct testSelected_type *pTemp)
{
	int IndexShift;
	pTemp = (struct testSelected_type *)malloc(sizeof(struct testSelected_type));
	if(StartIndex > IndexAfterDropEvent)
	{
		pTemp->pTests = pSelectedTests[StartIndex].pTests;
		for(IndexShift = StartIndex - 1; IndexShift >= IndexAfterDropEvent; IndexShift--)
		{ 
			pSelectedTests[IndexShift + 1] = pSelectedTests[IndexShift];
		}
		pSelectedTests[IndexAfterDropEvent].pTests = pTemp->pTests;
	}
	else
	{
		pTemp->pTests = pSelectedTests[StartIndex].pTests;
		for(IndexShift = StartIndex; IndexShift <= IndexAfterDropEvent; IndexShift++)
		{ 
			pSelectedTests[IndexShift] = pSelectedTests[IndexShift + 1];
		}
		pSelectedTests[IndexAfterDropEvent].pTests = pTemp->pTests;
	}
	free(pTemp);
	TempFreeOperation = 1;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : ändert in der auszuführenden Test Struktur(enthält alle Informationen zu den Test), den Status selected 
//  Input           : statusSelected: 	0: für nicht angewählt, 1: für angewählt
//										index:						Index welcher den Status gewechselt hat
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_SelectTestCollection(int statusSelected, int index)
{
	pSelectedTests[index].pTests->SelectTestSelected = statusSelected;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : löscht alle aufgelisteten Test Kollektionen 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_ClearSelectListBox(struct test_type EmptyTestStructure)
{
	int IndexTestList, MaxIndex;
	
	//wieviele Elemente sind in der Test Kollektion Textfeld
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &MaxIndex);
	//initiallisiert alle Einträge mit 0 Werte
	for(IndexTestList = MaxIndex - 1; IndexTestList >= 0; IndexTestList--)
	{
		DeleteListItem(STpanel, STPANEL_TREE_TESTCOLLECTION, IndexTestList, 1);
		pSelectedTests[IndexTestList].pTests = &EmptyTestStructure;
	}
	//it has no Files left
	CountSelectedFiles = 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : markiert alle Tests welche sich in der rechten Liste (Test Collection) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_SelectAllTestsSelect(void)
{
	int k, m = 0, CountSelectedTests;
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &CountSelectedTests);
	for(k = 0; k < CountSelectedTests; k++)
	{
		//ob ein Strukturelement vorhanden ist
		if (pSelectedTests[k].pTests != NULL)
		{
			SetTreeItemAttribute (STpanel, STPANEL_TREE_TESTCOLLECTION, m, ATTR_SELECTED, 1);
			pSelectedTests[k].pTests->SelectTestSelected = 1;
			m++;
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : unmarkiert alle Tests welche sich in der rechten Liste (ausführbare Tests) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_DeselectAllTestsSelect(void)
{
	int k, CountSelectedTests;
	GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &CountSelectedTests);
	for(k = 0; k < CountSelectedTests; k++)
	{
		//ob ein Strukturelement vorhanden ist
		if (pSelectedTests[k].pTests != NULL)
		{
			SetTreeItemAttribute (STpanel, STPANEL_TREE_TESTCOLLECTION, k, ATTR_SELECTED, 0);
			pSelectedTests[k].pTests->SelectTestSelected = 0;
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Zeigt die Ausgewählte Option der Testausführung an(Zeitdauer oder Anzahl Widerholungen)
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectListbox_ExecutionTB(int ExecutionButtonValue)
{
	int Visible = 1, Hidden = 0;
	
	if(ExecutionButtonValue == ITERATION)
	{
		SetCtrlAttribute(STpanel, STPANEL_ITERATIONCOLLECTION, ATTR_VISIBLE, Visible);
		SetCtrlAttribute(STpanel, STPANEL_NUMERIC_DURATINGTC, ATTR_VISIBLE, Hidden);
	}
		
	if(ExecutionButtonValue == DURATING)
	{
		SetCtrlAttribute(STpanel, STPANEL_NUMERIC_DURATINGTC, ATTR_VISIBLE, Visible);
		SetCtrlAttribute(STpanel, STPANEL_ITERATIONCOLLECTION, ATTR_VISIBLE, Hidden);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Blendet das Report Fenster ein und setzt das Is Running LED auf Grün
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void InitializeReportPanel(void)
{
	if(InProcessState == FALSE)	InProcessState = TRUE; 
	
	HidePanel(STpanel);
	DisplayPanel (RPpanel);
}
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die Firmwaren zum definierten Grundzustand passt. Ansonsten wird ein Firmware download durchgeführt.
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CheckIfFirmwareAreActual(void)
{
	BOOL8 DoNotOperateValue;
	UINT8 MotionControllerAxis1State, MotionControllerAxis2State, MotionControllerAxis3State, InterfaceFirmwareState, MotionControllerFirmwareState;
	UINT32 AutoSetupCode = 1454057407;
	int NumberSelectedFile, ResultFirmwareUpdate, PSSNumber, PSSNumberListMotionController[3] = {-1, -1, -1}, FileExist;
	ssize_t size;
	char ValveFirmwareVersionValve[TESTINFORMATION_NAME_LENGTH], MotionControllerFirmwareValve[TESTINFORMATION_NAME_LENGTH], InterfaceFirmwareValve[TESTINFORMATION_NAME_LENGTH];
	char RingElementString[TESTINFORMATION_NAME_LENGTH], UpdateToolCommand[MAX_PATHNAME_LEN] = "", StartPathNameTTIC2[MAX_PATHNAME_LEN], PSSNumberString[8], FilePath[MAX_PATHNAME_LEN];
	
	//Aktuelle Firmwaren auf dem Ventil
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.ValveFirmwareVersion, ValveFirmwareVersionValve, ""); 
	ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.MotionController1FirmwareVersion.ID, 0, &MotionControllerFirmwareState, DT_UINT8, PARAMETER_GET_STATE);
	if(MotionControllerFirmwareState == STATE_ACTIVE)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.MotionController1FirmwareVersion, MotionControllerFirmwareValve, ""); 
	else	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.ParameterAxis1.SpecificDriveParameter._100AManufacturerSoftwareVersion, MotionControllerFirmwareValve, ""); 
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceType, "");
	ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.InterfaceFirmwareVersion.ID, 0, &InterfaceFirmwareState, DT_UINT8, PARAMETER_GET_STATE);
	if(InterfaceFirmwareState == STATE_ACTIVE)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.InterfaceFirmwareVersion, InterfaceFirmwareValve, ""); 
	else if(Parameter.System.Identification.Hardware.InterfaceType.Value == IT_EtherCAT)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.InterfaceEtherCAT.SlaveFirmware, InterfaceFirmwareValve, "");
	else InterfaceFirmwareValve[0] = 0;
	HelpFunctions_CloseUSBCommunication();
	
	//Valve Firmware
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, NumberSelectedFile, RingElementString);
	//Entweder ist die gleiche Firmware definiert oder in beiden Namen kommt trunk vor
	if((strcmp(ValveFirmwareVersionValve, RingElementString) == 0) || ((strcmp(RingElementString, TRUNK_SOFTWARE_NAME) == 0) && strstr(ValveFirmwareVersionValve, TRUNK_SOFTWARE_NAME) != NULL))	return;
	//trunk kann vom Programm nicht selber upgedatet werden
	else if(strcmp(RingElementString, TRUNK_SOFTWARE_NAME) == 0)
	{
		MessagePopup("Update valve firmware", "trunk version not possible to download automaticly");
		HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
		strcpy(UpdateToolCommand, StartPathNameTTIC2);
		strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
		strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
		system(UpdateToolCommand);
	}
	//Wenn nicht gleich -> Firmware update
	else
	{
		//Brauche die PSS Nummer -> Aus Datenbank mit dem Firmware Namen
		PSSNumber = Database_GetPSSNumberFromFirmware(RingElementString);
		if(PSSNumber == -1)
		{
			MessagePopup("Update valve firmware", "No PSS Number found");	
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			system(UpdateToolCommand);
		}
		else
		{
			sprintf(PSSNumberString, "%d", PSSNumber);
			//Zuerst schauen ob das File existiert
			strcpy(FilePath, PRODUCTION_FIRMWARE_ACTUAL_PATH);
			strcat(FilePath, PSSNumberString);
			strcat(FilePath, VAT_FIRMWARE);
			FileExist = GetFileInfo(FilePath, &size);
			//alte Firmwares haben noch die .bin Endung
			if(FileExist == FILE_NOT_EXIST)
			{
				strcpy(FilePath, PRODUCTION_FIRMWARE_ACTUAL_PATH);
				strcat(FilePath, PSSNumberString);
				strcat(FilePath, OLD_VALVE_FIRMWARE);
				FileExist = GetFileInfo(FilePath, &size);
			}
			//könte auch im ersetzt Ordner sein -> noch zu definieren
			if(FileExist == FILE_NOT_EXIST)	
			{
				strcpy(FilePath, PRODUCTION_FIRMWARE_REPLACED_PATH);
				strcat(FilePath, PSSNumberString);
				strcat(FilePath, VAT_FIRMWARE);
				FileExist = GetFileInfo(FilePath, &size);
			}
			if(FileExist == FILE_NOT_EXIST)	
			{
				strcpy(FilePath, PRODUCTION_FIRMWARE_REPLACED_PATH);
				strcat(FilePath, PSSNumberString);
				strcat(FilePath, OLD_VALVE_FIRMWARE);
				FileExist = GetFileInfo(FilePath, &size);
			}
			if(FileExist == FILE_NOT_EXIST)	MessagePopup("Update valve firmware", "Firmware file not found");
			else
			{
				//Nach der exe kommt als Argument der Ort der Firmware
				HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
				strcpy(UpdateToolCommand, StartPathNameTTIC2);
				strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
				strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
				strcat(UpdateToolCommand, FilePath);
				ResultFirmwareUpdate = system(UpdateToolCommand);
			}
		}
	}
	
	//Motion Controller Firmware (Hier ist es wichtig die Anzahl Motor Achsen zu wissen) -> über State Argument
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, NumberSelectedFile, RingElementString);
	if(strcmp(RingElementString, MotionControllerFirmwareValve) != 0)
	{
		Database_GetPSSNumberFromMotionControllerFirmware(RingElementString, PSSNumberListMotionController);
		//Vor dem Update muss der Do Not Operate gesetzt werden
		DoNotOperateValue = TRUE;
		HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.DoNotOperate, DoNotOperateValue, "Do Not Operate Service auf True gesetzt");		
		//Ventil abfragen, wie viele Achsen, das unterstützt werden
		ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.MotionController1FirmwareVersion.ID, 0, &MotionControllerAxis1State, DT_UINT8, PARAMETER_GET_STATE);
		if(MotionControllerAxis1State == STATE_ACTIVE)
		{
			sprintf(PSSNumberString, "%d", PSSNumberListMotionController[0]);
			//Nach der exe kommt als Argument der Ort der Firmware
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			strcat(UpdateToolCommand, PRODUCTION_FIRMWARE_ACTUAL_PATH);
			strcat(UpdateToolCommand, PSSNumberString);
			strcat(UpdateToolCommand, MOTIONCONTROLLER_AXIS1);
			ResultFirmwareUpdate = system(UpdateToolCommand);
		}
		ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.MotionController2FirmwareVersion.ID, 0, &MotionControllerAxis2State, DT_UINT8, PARAMETER_GET_STATE);
		if(MotionControllerAxis2State == STATE_ACTIVE)
		{
			sprintf(PSSNumberString, "%d", PSSNumberListMotionController[1]);
			//Nach der exe kommt als Argument der Ort der Firmware
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			strcat(UpdateToolCommand, PRODUCTION_FIRMWARE_ACTUAL_PATH);
			strcat(UpdateToolCommand, PSSNumberString);
			strcat(UpdateToolCommand, MOTIONCONTROLLER_AXIS2);
			ResultFirmwareUpdate = system(UpdateToolCommand);
		}
		ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.MotionController3FirmwareVersion.ID, 0, &MotionControllerAxis3State, DT_UINT8, PARAMETER_GET_STATE);
		if(MotionControllerAxis3State == STATE_ACTIVE)
		{
			sprintf(PSSNumberString, "%d", PSSNumberListMotionController[2]);
			//Nach der exe kommt als Argument der Ort der Firmware
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			strcat(UpdateToolCommand, PRODUCTION_FIRMWARE_ACTUAL_PATH);
			strcat(UpdateToolCommand, PSSNumberString);
			strcat(UpdateToolCommand, MOTIONCONTROLLER_AXIS3);
			ResultFirmwareUpdate = system(UpdateToolCommand);
		}
		//Nach dem Update soll der AutoSetup Code gesetzt werden und der Do Not Operate Service ausgeschaltet werden
		if(MotionControllerAxis1State == STATE_ACTIVE)	HelpFunctions_SetCommandUint32((struct ParameterType_GENERAL_INFORMATION *) &Parameter.DriveUnit.ParameterAxis1.DriveEEPROMData.AutosetupCode, AutoSetupCode, "Autosetup Code Achse 1 gesetzt");		
#ifndef PLATFORMCONFIG_DRIVEAXIS2_NA		
		if(MotionControllerAxis2State == STATE_ACTIVE)	HelpFunctions_SetCommandUint32((struct ParameterType_GENERAL_INFORMATION *) &Parameter.DriveUnit.ParameterAxis2.DriveEEPROMData.AutosetupCode, AutoSetupCode, "Autosetup Code Achse 2 gesetzt");		
#endif
#ifndef PLATFORMCONFIG_DRIVEAXIS3_NA
		if(MotionControllerAxis3State == STATE_ACTIVE)	HelpFunctions_SetCommandUint32((struct ParameterType_GENERAL_INFORMATION *) &Parameter.DriveUnit.ParameterAxis3.DriveEEPROMData.AutosetupCode, AutoSetupCode, "Autosetup Code Achse 3 gesetzt");		
#endif
		DoNotOperateValue = FALSE;
		HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.DoNotOperate, DoNotOperateValue, "Do Not Operate Service auf False gesetzt");		
	}
	
	//Interface Firmware (nur wenn es sich beim Interface um ein DeviceNet oder EtherCAT sich handelt)
	if(Parameter.System.Identification.Hardware.InterfaceType.Value == IT_EtherCAT || Parameter.System.Identification.Hardware.InterfaceType.Value == IT_DeviceNet)
	{	
		GetCtrlIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, &NumberSelectedFile);
		GetValueFromIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, NumberSelectedFile, RingElementString);
		if(strcmp(RingElementString, InterfaceFirmwareValve) != 0)
		{
			//Interface Firmware muss upgedatet werden
			PSSNumber = Database_GetPSSNumberFromFirmware(RingElementString);
			if(PSSNumber == -1)
			{
				MessagePopup("Update valve firmware", "No PSS Number found");	
				HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
				strcpy(UpdateToolCommand, StartPathNameTTIC2);
				strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
				strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
				system(UpdateToolCommand);
			}
			else
			{
				sprintf(PSSNumberString, "%d", PSSNumber);
				//Nach der exe kommt als Argument der Ort der Firmware
				HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
				strcpy(UpdateToolCommand, StartPathNameTTIC2);
				strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
				strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
				strcat(UpdateToolCommand, PRODUCTION_FIRMWARE_ACTUAL_PATH);
				strcat(UpdateToolCommand, PSSNumberString);
				strcat(UpdateToolCommand, INTERFACE_FIRMWARE);
				ResultFirmwareUpdate = system(UpdateToolCommand);
			}
		}
	}	//nur wenn es sich um ein EtherCAT oder DeviceNet Interface handelt 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schaut ob die definierten Drive + Configuration Files auf das Ventil geladen wurden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CheckIfDriveAndConfigurationFileAreActual(void)
{
	SINT16 ActivityState;
	int NumberSelectedFile, ResultFirmwareUpdate, FileExist;
	ssize_t size;
	char RingElementString[TESTINFORMATION_NAME_LENGTH], DriveParameterFileString[DRIVE_PARAMETER_FILE_NAME_LENGTH], ConfigurationParameterFileString[CONFIGURATION_FILE_NAME_LENGTH];
	char UpdateToolCommand[MAX_PATHNAME_LEN], StartPathNameTTIC2[MAX_PATHNAME_LEN], FilePath[MAX_PATHNAME_LEN];
	
	//Drive Parameter File (muss vor dem Configuration File ausgeführt werden)
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, NumberSelectedFile, RingElementString);
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Configuration.DriveParametersID, DriveParameterFileString, ""); 
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Configuration.ConfigurationParametersID, ConfigurationParameterFileString, ""); 
	HelpFunctions_CloseUSBCommunication();
	
	//Entweder hat das Ventil ein Drive File definiert, welches aber nicht mehr gebraucht wird oder noch kein Drive File ausgeführt
	//Kein Drive File auf dem Ventil aber der Grundzustand verlangt ein File
	if(strcmp(DriveParameterFileString, FILE_NOT_AVAIABLE) == 0 && strcmp(RingElementString, FILE_NOT_AVAIABLE) != 0)
	{
		//Drive File auf dem Ventil updaten
		//Wichtig zu Wissen ob es sich um ein aktives File handelt, da der Speicherort des Files sich unterscheidet
		//Prüfung ob File vorhanden ist
		ActivityState = Database_GetDriveParameterFileActive(RingElementString);
		if(ActivityState == ACTIVITY_STATE_ACTIVE)	strcpy(FilePath, PRODUCTION_DRIVEPAR_ACTUAL_PATH);
		else																				strcpy(FilePath, PRODUCTION_DRIVEPAR_REPLACED_PATH);
		strcat(FilePath, RingElementString);
		strcat(FilePath, DRIVEPARAMETER_FILE);
		FileExist = GetFileInfo(FilePath, &size);
		if(FileExist == FILE_NOT_EXIST)	MessagePopup("Update drive parameter file", "Drive parameter file not found");
		else
		{
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			strcat(UpdateToolCommand, FilePath);
			ResultFirmwareUpdate = system(UpdateToolCommand);
		}
	}
	
	//Drive File auf dem Ventil aber der Grundzustand verlangt keines
	else if(strcmp(DriveParameterFileString, FILE_NOT_AVAIABLE) != 0 && strcmp(RingElementString, FILE_NOT_AVAIABLE) == 0)	HelpFunctions_SetCommandBool((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Services.SetDriveFactoryDefaultParameters, TRUE, "Set Drive Factory Default Parameter auf True gesetzt");		
	
	//Configuration File Update
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, &NumberSelectedFile);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, NumberSelectedFile, RingElementString);
	
	//Entweder hat das Ventil ein Configuration File definiert, welches aber nicht mehr gebraucht wird oder noch kein Configuration File ausgeführt
	//Kein Configuration File auf dem Ventil aber der Grundzustand verlangt ein File
	if(strcmp(ConfigurationParameterFileString, FILE_NOT_AVAIABLE) == 0 && strcmp(RingElementString, FILE_NOT_AVAIABLE) != 0)
	{
		//Configuration File auf dem Ventil updaten
		//Wichtig zu Wissen ob es sich um ein aktives File handelt, da der Speicherort des Files sich unterscheidet
		//Prüfung ob File vorhanden ist
		ActivityState = Database_GetConfigurationParameterFileActive(RingElementString);
		if(ActivityState == ACTIVITY_STATE_ACTIVE)	strcpy(FilePath, PRODUCTION_CONFPAR_ACTUAL_PATH);
		else																				strcpy(FilePath, PRODUCTION_CONFPAR_REPLACED_PATH);
		strcat(FilePath, RingElementString);
		strcat(FilePath, CONFIGURATIONPARAMETER_FILE);
		FileExist = GetFileInfo(FilePath, &size);
		if(FileExist == FILE_NOT_EXIST)	MessagePopup("Update configuration parameter file", "Configuration parameter file not found");
		else
		{
			HelpFunctions_GetTTIC2Path(StartPathNameTTIC2);
			strcpy(UpdateToolCommand, StartPathNameTTIC2);
			strcat(UpdateToolCommand, UPDATETOOL_FILE_NAME);
			strcat(UpdateToolCommand, UPDATETOOL_EXE_NAME);
			strcat(UpdateToolCommand, FilePath);
			ResultFirmwareUpdate = system(UpdateToolCommand);
		}
	}
}
---
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Prüft ob die Hardware mit der Testanforderungen übereinstimmt sowie die Interface Kommunikation vorhanden ist
//										Dies wird nur einmal durchgeführt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CheckOneTimeValveHardware(UINT8 ChangeHardwareSetting)
{
	UserCommitHardwareChange = ChangeHardware_RequestDiffrentHardwareForTestSequence(ChangeHardwareSetting);
	if(UserCommitHardwareChange != HARDWARE_OK && UserCommitHardwareChange != OK_BUTTON)
	{	 
		switch(UserCommitHardwareChange)
		{
			case CANCEL_BUTTON:				HelpFunctions_WriteMessageOnReportPanel("Der User wollte die Test Kollektion nicht ausführen, da nicht alle Tests die nötige Hardware besitzen oder die Interface Kommunikation nicht möglich ist", MESSAGE_WARNING);	break;
			case USER_PUSH_NO_BUTTON: HelpFunctions_WriteMessageOnReportPanel("Der User hat keine Entscheidung getroffen, ob er die Hardware tauschen möchte sowie dafür sorgt, dass die Interface Kommunikation möglich ist", MESSAGE_WARNING);						break;
			default:																																																																																																																				break;
		}
		CheckHardwareState = FALSE;
		return;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Prüft ob die Hardware mit der Testanforderungen übereinstimmt sowie die Interface Kommunikation vorhanden ist
//										Dies wird mehrmals ausgeführt, bis entweder die Hardware für den Test kompatibel ist oder der User die Abfrage abgebrochen hat
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CheckMultipleTimeValveHardware(UINT8 ChangeHardwareSetting)
{
	do
	{
		CheckOneTimeValveHardware(ChangeHardwareSetting);
		if(UserCommitHardwareChange == CANCEL_BUTTON || UserCommitHardwareChange == USER_PUSH_NO_BUTTON)	UserWantChangeHardware = USER_NOT_WANT;
		//wird nicht ausgeführt, wenn der User die Hardware nicht wechseln möchte
		if(UserWantChangeHardware != USER_NOT_WANT)
		{
			//wider Auslesung der verwendetetn Hardware und Überprüfung ob sie mit den Anforderungen übereinstimmt
			ChangeHardware_SaveHardwareInformationVentil();
			NeedDiffrentHardware = ChangeHardware_NeedfulHardwareForTest(pSelectedTests, IndexTestList);
			IsInterfaceCommunicationAvaiable = ChangeHardware_AvaiableInterfaceCommunication(pSelectedTests, IndexTestList);
		}
	}	while((NeedDiffrentHardware != FALSE || IsInterfaceCommunicationAvaiable != ERROR_FREE) && UserWantChangeHardware == USER_WANT);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Führt das Exe File aus. Dabei wird im Report noch das File grün markiert, welches gerade ausgeführt wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void RunTestExe(struct test_type *pTestType)
{
	UINT8 i;
	int ResultExe;
	SINT32 TestResultId;
	char FileNameExe[MAX_PATHNAME_LEN], StartTime[DATE_TIME_TYPE_LENGTH];
+++
	//Alle Test Error Messages löschen
	if(TestErrorMessageIndex > 0)	for(i = 0; i < TestErrorMessageIndex; i++)	strcpy(TestErrorMessageList[TestErrorMessageIndex], "");
	TestErrorMessageIndex = 0;
	
	//Test Resultat Eintrag erstellen (Start Time, TestCollectionResult_Id, TestVersion_Id)
	TestResultId = Database_CreateTestResultEntry(TestCollectionResultId, pTestType->TestVersionId, StartTime);
---	
	//Balken im Report Test Collection Feld
	SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT_TC, IndexTestList, 0, ATTR_LABEL_BGCOLOR, VAL_YELLOW);

	//Report Oberfläche LED(Grün) einschalten (In Process)
	InitializeReportPanel();
	
	//ans Ende des Report springen
	fseek(pReportFile, 0, SEEK_END); 
	//zusammenfügen des Exe Namen mit dem Pfad 
	FileNameExe[0] = 0;
	strcat(FileNameExe, PathTestFolder); 
	strcat(FileNameExe, pTestType->TestName);
	strcat(FileNameExe, BETWENCHAR);
	strcat(FileNameExe, pTestType->TestName);
	strcat(FileNameExe, EXE_FILE);
	
	ResultExe = system(FileNameExe);
	if(ResultExe < 0)	MessagePopup("Run test", "Error by run test");
	//Auswertung des Tests
	TestResult = HelpFunctions_AnalysisTest();
	
	if(TestResult == SUCCESFUL)					SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT_TC, IndexTestList, 0, ATTR_LABEL_BGCOLOR, VAL_GREEN);
	else if(TestResult == USERCANCELED)	SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT_TC, IndexTestList, 0, ATTR_LABEL_BGCOLOR, VAL_DK_YELLOW);
	else																SetTreeCellAttribute(RPpanel, RPPANEL_TREE_REPORT_TC, IndexTestList, 0, ATTR_LABEL_BGCOLOR, VAL_RED);
	
	//Test in Selected Liste als erledigt kennzeichnen
	pSelectedTests[IndexTestList].Executed = 1;
+++	
	//Resultat im Test Result eintragen
	Database_WriteTestResultTable(StartTime, TestCollectionResultId, TestResult);
	//Bei einem Fehler die letzten drei Fehlermeldungen abspeichern
	if(TestResult == ERRORTEST)
	{
		CountErrorTests++;
		if(TestErrorMessageIndex > 0)	Database_WriteTestErrorMessageTable(TestResultId);	
	}
}
---
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Führt alle allgemeinen Tätigkeiten vor Ablauf der einzelnen Tests aus
//										Dazu zählen: Diagnose File + Anzahl Slaves + Umgebungsvariable + NI Hardware + Hardware Check + Report File + Globale Variable
//  Input           : pTestPathNames:	Gibt den Startpfad an, wo die Test abgelegt sind
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void InitTestCollectionEnvironment(char *pTestPathNames)
{
	char *pSearchSourceString;
	
	//wird schon vor der for Initialisierung in Change Hardware Modul gebraucht
	IndexTestList = 0;
	
	//für Zeitraportierung
	SYSTEMTIME ReportTime;
	GetSystemTime(&ReportTime);
	
	//Löscht alle Diagnostic Files im Result Ordner
	HelpFunctions_DeleteAllDiagnosticFiles();
	
	//Wenn ein Cluster Test ausgewählt ist, die Anzahl Slaves abspeichern
	HelpFunctions_SaveNumberOfSlaves();
	
	//Running in Umgebungsvariable TestStatus schreiben um später ein Abbruck triggern zu können
	SharedVariable_SendSharedVariableTestStatus("Running");
	
	//Wenn NI Hardware vorhanden ist sollte diese initialisiert werden
	HelpFunctions_InitPowerConnector();
	
	//Alle Tests überprüfen, ob die Test mit der aktuellen Hardware durchgeführt werden kann. Weiter wird die Kommunikation auf dem Interface kontrolliert 
	CheckOneTimeValveHardware(CH_SETTING_LIST_TESTS);

	GetCtrlVal (STpanel, STPANEL_USER, User); 
	sprintf(FileName, "Report_%s_%02d.%02d.%d_%02dh%02dm%02ds.txt", User, ReportTime.wDay, ReportTime.wMonth, ReportTime.wYear, ReportTime.wHour+TimeOffset, ReportTime.wMinute, ReportTime.wSecond);
	//immer den letzten Report löschen um keine Anhäufungen zu haben
	if (FirstReport != 0 && ReportTextfileOpen == 1)	
	{
		fclose(pReportFile);
		remove(ReportFile);
	}
	ReportFile[0] = 0;
	strcpy(ReportFile, pTestPathNames);
	pSearchSourceString = strstr(ReportFile, "Source");
	if(pSearchSourceString != NULL)	*pSearchSourceString = 0;
	strcat(ReportFile, REPORT_DIRECTION);
	strcat(ReportFile, FileName);
	//w+ um das File zu löschen, wenn der User es schliesst
	pReportFile = fopen(ReportFile, "w+");
	ReportTextfileOpen = 1;
	
	StartTestCollectionTime = Timer();
	
	//User Name in Globale Variable abspeichern
	HelpFunctions_WriteUserName();
+++
	//Anzahl fehlerhafte Tests zurück setzen
	CountErrorTests = 0;
	
	//Bevor die Testcollection ausgefüht wird soll kein Fehler vorliegen
	TestResult = SUCCESFUL;
---
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Lädt den Grundzustand der Firmware sowie setzt die Ventil Parameter zurück und lädt Drive + Config File
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void SetInitialStateFirmware(void)
{
	//Firmwaren laden, wenn sie nicht übereinstimmen
	CheckIfFirmwareAreActual();
	
	//Zuerst den Factory Zustand herstellen. So kann Anschliessend der Store Befehl ausgeführt werden. Nachdem ein Test ausgeführt wurde wird dann der Restore User Settings benützt.
	HelpFunctions_LocalMode();
	HelpFunctions_RestoreParameters(FACTORY_RESTORE_PARAMETERS);
	
	//Drive + Config Files laden
	CheckIfDriveAndConfigurationFileAreActual();
	
	HelpFunctions_StoreParameters(USER_STORE_PARAMETERS);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Der User möchte die Hardware nicht wechseln. Schreibt die geforderte Hardware in das Report Fenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void UserNotWantChangeHardwareWriteOnReportTree(void)
{
 	char NoHardwareChangeMessage[100], NoAvaiableInterfaceCommunicationMessage[100];
	
	if(NeedDiffrentHardware == TRUE)
	{
		strcpy(NoHardwareChangeMessage, "User will die Hardware nicht wechseln, Test:");
		strcat(NoHardwareChangeMessage, pSelectedTests[IndexTestList].pTests->TestName);
		HelpFunctions_WriteMessageOnReportPanel(NoHardwareChangeMessage, MESSAGE_WARNING);
		pSelectedTests[IndexTestList].Executed = 1;
	}
	if(IsInterfaceCommunicationAvaiable != ERROR_FREE)
	{
		strcpy(NoAvaiableInterfaceCommunicationMessage, "User möchte die Interface Kommunikation nicht herstellen, interface:");
		switch(IsInterfaceCommunicationAvaiable)
		{
			case FAIL_RS232_COMMUNICATION:			strcat(NoAvaiableInterfaceCommunicationMessage, "RS232/RS485");	break;
			case FAIL_DEVICENET_COMMUNICATION:	strcat(NoAvaiableInterfaceCommunicationMessage, "DeviceNet");		break;
			case FAIL_ETHERCAT_COMMUNICATION:		strcat(NoAvaiableInterfaceCommunicationMessage, "EtherCAT");		break;
			default:														strcat(NoAvaiableInterfaceCommunicationMessage, "not defined");	break;
		}
		HelpFunctions_WriteMessageOnReportPanel(NoAvaiableInterfaceCommunicationMessage, MESSAGE_WARNING);
		pSelectedTests[IndexTestList].Executed = 1;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt das Test Resultat in den Report
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void WriteTestResultInReport(void)
{
	//nur Auswertung des Abbruchs Anzeigen, wenn die Report Oberfläche noch offen ist
	if(ReportTextfileOpen == 1)
	{
		switch(TestResult)
		{
			case SUCCESFUL:			Report_AddText("\nDie Testkollektion ist ausgeführt worden");														break;
			case USERCANCELED: 	
			{
				if(DuratingTestCollectionStatus == TIMEEXPIRED) Report_AddText("\nDie Zeitdauer der Testkollektion ist abgelaufen");
				else																						Report_AddText("\nDie Testkollektion wurde durch den User abgebrochen");								
				break;
			}
			case ERRORTEST:			
			{
				if(TerminateByError == TRUE)	Report_AddText("\nDie Testkollektion ist aufgrund eines Fehlers unterbrochen worden");
				else													Report_AddText("\nDer User wollte trotz eines gefundenen Fehlers die Testkollektion ausgeführt haben");
				break;
			}
			default:						Report_AddText("\nUnbekannter TestStatus");																							break;
		}
	}	//end Überprüfung ob der Report Panel noch existiert
}
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : InitialStateFirmware + ValveHardware + TestCollectionResult Eintrag in Datenbank erstellen  
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void BuildTestResultEntrys(void)
{
	SINT32 InitialStateValveId, ValveHardwareId;
	char FirmwareReleaseTime[DATE_TIME_TYPE_LENGTH];
	
	//Firmware Release Time abfragen 
	TranslateDateTimeInDatabaseFormat(FirmwareReleaseTime);
	
	//Initial State Firmware Eintrag erstellen, wenn noch keiner exisitiert
	InitialStateValveId = Database_WriteInitialStateFirmwareTable(FirmwareReleaseTime);
	
	if(InitialStateValveId >= 0)
	{
		//Valve Hardware in der Datenbank hinterlegen
		ValveHardwareId = Database_WriteValveHardwareTable();
	
		//TestCollectionResult Eintrag erstellen
		TestCollectionResultId = Database_CreateTestCollectionResultEntry(ValveHardwareId, InitialStateValveId);
	}
	else TestCollectionResultId	= -1;
	
	//Den Port schliessen 
	HelpFunctions_CloseUSBCommunication();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt den AbortType und CountErrorTests Wert in die TestCollectionResult Tabelle  
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void WriteTestCollectionResult(void)
{
	SINT32 AbortTypeId;
	
	if(TestResult == USERCANCELED)																AbortTypeId = Database_GetAbortTypeId("User");
	else if(TestResult == ERRORTEST && TerminateByError == TRUE)	AbortTypeId = Database_GetAbortTypeId("First Error");
	else																													AbortTypeId = Database_GetAbortTypeId("None");
	
	Database_WriteTestCollectionResultTable(TestCollectionResultId, AbortTypeId, CountErrorTests);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt das Datum&Zeiform von der C Libary in das Datenbank Format um
//  Input           : 
//  Output *Pointer : pFirmwareReleaseTime: Datum&Zeitformat im Datenbank Format
//         *return  :  
//******************************************************************************************************************************
void TranslateDateTimeInDatabaseFormat(char *pFirmwareReleaseTime)
{
	UINT8 i;
	char ValveFirmwareReleaseDate[FIRMWARE_RELEASE_DATE_LENGTH], ValveFirmwareReleaseTime[FIRMWARE_RELEASE_TIME_LENGTH];
	char TempYearString[YEAR_STRING_LENGTH], TempMonthString[MONTH_STRING_LENGTH], TempDayString[DAY_STRING_LENGTH];
	
	//Zuerst das Release Zeit auf dem Ventil abfragen
	HelpFunctions_GetCommandStringWithSpaceChar((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.ValveFirmwareReleaseDate, ValveFirmwareReleaseDate, ""); 
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.ValveFirmwareCompileTime, ValveFirmwareReleaseTime, ""); 
	
	//Format umwandeln
	//Jahr
	for(i = 0; i < YEAR_STRING_LENGTH-1; i++)	TempYearString[i] = ValveFirmwareReleaseDate[i+7];
	TempYearString[YEAR_STRING_LENGTH-1] = 0;
	
	strcpy(pFirmwareReleaseTime, TempYearString);
	strcat(pFirmwareReleaseTime, "-");
	
	//Monat
	switch(ValveFirmwareReleaseDate[0])
	{
		case 'J':
		{
			if(ValveFirmwareReleaseDate[1] == 'a')			strcpy(TempMonthString, "01");
			else if(ValveFirmwareReleaseDate[2] == 'n') strcpy(TempMonthString, "06");
			else																				strcpy(TempMonthString, "07");
			break;
		}
		case 'F':	strcpy(TempMonthString, "02");	break;
		case 'M':	
		{
			if(ValveFirmwareReleaseDate[1] == 'a')			strcpy(TempMonthString, "03");
			else																				strcpy(TempMonthString, "05");
			break;
		}
		case 'A':
		{
			if(ValveFirmwareReleaseDate[1] == 'p')			strcpy(TempMonthString, "04");
			else																				strcpy(TempMonthString, "08");
			break;
		}
		case 'S':	strcpy(TempMonthString, "09");	break;
		case 'O':	strcpy(TempMonthString, "10");	break;
		case 'N':	strcpy(TempMonthString, "11");	break;
		case 'D':	strcpy(TempMonthString, "12");	break;
		default:	strcpy(TempMonthString, "");		break;
	}
	
	strcat(pFirmwareReleaseTime, TempMonthString);
	strcat(pFirmwareReleaseTime, "-");
	
	//Tag
	for(i = 0; i < DAY_STRING_LENGTH-1; i++)	
	{
		if(ValveFirmwareReleaseDate[i+4] == ' ')	TempDayString[i] = '0';	
		else																			TempDayString[i] = ValveFirmwareReleaseDate[i+4];
	}
	TempDayString[DAY_STRING_LENGTH-1] = 0;
	
	strcat(pFirmwareReleaseTime, TempDayString);
	strcat(pFirmwareReleaseTime, " ");
	
	//Zeit noch anfügen
	strcat(pFirmwareReleaseTime, ValveFirmwareReleaseTime);
}
---