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
#include <NIDAQmx.h>
#include <userint.h>
#include <utility.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <toolbox.h>

//Problem with the PI Constant from the toolbox.h Libary
#undef PI;
//muss vor AvaiableListbox.h definiert werden
#include "Definelist.h"
#include "AvaiableListbox.h"
#include "Database.h"
#include "DefineStructFileService.h"
#include "GenerateFilterList.h"
#include "GenerateStructureTests.h"
#include "HelpFunctions.h"
#include "InfoListbox.h"
#include "ParameterProcessing.h"
#include "Report.h"
#include "SelectFilesFromFilter.h"
#include "SelectListbox.h"
#include "SurfaceTest.h" 
#include "SharedVariable.h"
#include "CollectionTest.h"
#include "Typedefinitions.h"
#include "UsbLabView.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void DiscardInformation(void);
static void UpdateConfigurationParameterFileRingList(void);
static void UpdateDriveParameterFileRingList(void);
static void WriteInitialStateFirmwareRingLists(void);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
BOOL8 TestAreRunning = 0, IsShowAllDriveParameterFilesSelected = FALSE, IsShowAllConfigurationParameterFilesSelected = FALSE, TerminateByError;
UINT8 TempFreeOperation = 0;
UINT16 CountTests = 0, EntriesFiles = 0, CountTestCollections = 0;
int  STpanel, TCpanel, RPpanel, MenuBarHandle, MenuItemID = 0, AdressMemory, i = 0, StrlenTestFiles = 0, StrlenTestFilesTot = 500, ReportTextfileOpen = 0, StatusUserInterface, StatusUserInterfaceRP, SelectedFile = 0, IndexTestCollection = 0, SelectedFilter = 0, CountSelectedFiles = 0, MaxIndex, StartIndexDragEvent = 0, ThreadFunctionId1 = 0, ThreadFunctionId2 = 0, ExecutionButtonValue = 0, IterationTestCollectionMaxUser, VisibleReport, ResponsValueUser; 
static int CVICALLBACK ThreadFunction1 (void *functionData), CVICALLBACK ThreadFunction2 (void *functionData);
FLOAT DuratingTestCollectionMaxUser = 0;
char *pAdressInfoTest, *pFileList, *CompareAdress, *pDataInfoTest, *pTestsCollectionList, *pTextInfoTest, **ppArrayElementExeListInfo, **ppArrayTestCollectionList, **ppArrayInterfaceCollectionList, **ppArrayFileList, *pAdressInfoTestForValveSerie, *pAdressInfoTestForControllerType, *pAdressInfoTestForOption, *pAdressInfoTestForCluster, *pAdressInfoTestForValue;
char FilePath[MAX_PATHNAME_LEN], FileName[MAX_PATHNAME_LEN], SingleTest[MAX_PATHNAME_LEN], ReportFile[MAX_PATHNAME_LEN], DescriptionTestConnection[DESCRIPTION_TEST_CONNECTION_FILE + 1], PathTTIC2Folder[MAX_PATHNAME_LEN], PathTestFolder[MAX_PATHNAME_LEN];
FILE *pReportFile;
//Ein leeres Strukturelement um einen besteheneden Eintrag zu löschen
struct test_type *pTests, EmptyTestStructure = {0,};
struct testSelected_type *pSelectedTests, *pTemp = NULL;
struct testAvaiable_type *pAvaiableTests;
struct testHardware_type *pHardwareVentil;
struct testCollection_type *pTestCollections;

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : main Funktion der Testoberfläche
//  Input           : argc 		gibt Anzahl Argumente an
//									:	*argv[] Text der einzelnen Einträge
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
int main(int argc, char *argv[])
{
	//Variable Oberfläche
	if (InitCVIRTE (0, argv, 0) == 0) 
		 return -1;
	if ((STpanel = LoadPanel (0, "SurfaceTest.uir", STPANEL)) < 0)
		 return -1;
	
	SurfaceTest_Init();
	
	StatusUserInterface = RunUserInterface();
	DiscardPanel(STpanel);
	CloseCVIRTE ();
	
	//Abschlissung Umgebungsvariablen
	CNVFinish ();
	
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Führt die Initialisierung durch
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SurfaceTest_Init(void)
{
	HelpFunctions_SetTimeOffset();
	HelpFunctions_GetTTIC2Path(PathTTIC2Folder);
	HelpFunctions_GetTestsPath(PathTestFolder);
	//Umgebungsvariable anlegen
	SharedVariable_InitSharedVariable();
	EntriesFiles = GenerateStructureTests_WriteExeFiles(PathTestFolder);
	//Die Anzahl Tests können sich später verändern
	CountTests = EntriesFiles;
	GenerateStructureTests_GenerateTestsStructure();
	//Hauptfenster einblenden
	DisplayPanel(STpanel);
	AvaiableListbox_WriteTestsInAvaiableTests(EntriesFiles);
+++
	Database_WriteTestCollectionsStructure();
	CollectionTest_WriteTestCollectionInTextBox();
	Database_WriteTestCollectionNameInRingList();
	GenerateFilterList_FillFilterLists();
	GenerateFilterList_WriteModulFilterInTextBox();
	AvaiableListbox_ShowAllTests();
	ChangeHardware_SaveHardwareInformationVentil();
	//Die Auswahlfelder im InitialStateFirmware Bereich mit den Datenbank Werten füllen
	WriteInitialStateFirmwareRingLists();
---
}
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Aktuallisiert die Test Collection Selektionsansicht
//  Input           : TestCollectionId:	Id der Test Kollektion 
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SurfaceTest_UpdateTestCollectionRingList(SINT32 TestCollectionId)
{
	BOOL8 FindTestCollectionEntry = FALSE;
	UINT16 TestCollectionIndex;
	int CountTestCollectionEntrys;
	
	GetNumListItems(STpanel, STPANEL_RING_ISV_TEST_COL, &CountTestCollectionEntrys); 
	if(CountTestCollectionEntrys >= 0)
	for(TestCollectionIndex = 0; TestCollectionIndex < CountTestCollectionEntrys; TestCollectionIndex++)
	{
		//Erster Eintrag ist immer leer
		if(TestCollectionIndex > 0 && pTestCollections[TestCollectionIndex-1].TestCollectionId == TestCollectionId)	
		{
			//Hat immer noch einen leeren Eintrag in der Auswahlliste
			SetCtrlIndex(STpanel, STPANEL_RING_ISV_TEST_COL, TestCollectionIndex);
			FindTestCollectionEntry = TRUE;
			break;
		}
	}
	//Wenn der Eintrag nicht gefunden werden kann soll der User Informiert werden
	if(FindTestCollectionEntry == FALSE && CountTestCollectionEntrys > 0)	MessagePopup("Test Collection ID", "Test Collection ID not find in Database!");
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Kollektion Informationen des ausgewählten Eintrages
//  Input           : 
//  Output *Pointer : pName:					Name der Test Kollektion
//										pDescription:		Beschreibung der Test Kollektion
//         *return  : ID der Test Kollektion 
//******************************************************************************************************************************
SINT32 Surface_GetSelectedTestCollectionInformation(char *pName, char *pDescription)
{
	SINT32 TestCollectionId = -1;
	int NumberSelectedIndex;
	
	GetCtrlIndex(STpanel, STPANEL_RING_ISV_TEST_COL, &NumberSelectedIndex);
	GetValueFromIndex(STpanel, STPANEL_RING_ISV_TEST_COL, NumberSelectedIndex, pName);
	
	if(NumberSelectedIndex >= 1)	
	{
		//Wenn alle Test Kollektion ausgewählt ist, so soll das Datum  und der Aktiv Status noch aus dem Namen entfernt werden
		if(IsShowAllTestCollectionSelected == TRUE) HelpFunctions_GetTestCollectionNameWithoutDateAndStateInfo(pName, NumberSelectedIndex-1);
		TestCollectionId = Database_GetTestCollectionId(pName, pTestCollections[NumberSelectedIndex-1]);
	}
	
	//Nun soll die Beschreibung der Test Collection aus der Datenbank geholt werden
	if(TestCollectionId >= 0)	Database_GetTestCollectionTable(TestCollectionId, pName, pDescription);
	return TestCollectionId;
}








//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den Speicher wider frei von den dynamisch erzeugten Variablen
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
static void DiscardInformation(void)
{
	SharedVariable_EndSharedVariable();
	
	//Datenbank Verbindung trennen
	Database_DisconnectDatabase();
	
	//Network Variable wider freigeben
	CNVFinish();

	//gibt den Speicher wieder frei
	if(pFileList != NULL)																			free(pFileList);
	if(pDataInfoTest != NULL)																	free(pDataInfoTest);
	if(ppArrayElementExeListInfo != NULL)											free(ppArrayElementExeListInfo);
	if(pTestsCollectionList != NULL)													free(pTestsCollectionList);
	if(ppArrayTestCollectionList != NULL)											free(ppArrayTestCollectionList);
	if(pTextInfoTest != NULL)																	free(pTextInfoTest);
	if(pAdressInfoTestForValveSerie != NULL)									free(pAdressInfoTestForValveSerie);
	if(pAdressInfoTestForControllerType != NULL)							free(pAdressInfoTestForControllerType);
	if(pAdressInfoTestForOption != NULL)											free(pAdressInfoTestForOption);
	if(pAdressInfoTestForCluster != NULL)											free(pAdressInfoTestForCluster);
	if(pAdressInfoTestForValue != NULL)												free(pAdressInfoTestForValue);
	if(pTests != NULL)																				free(pTests);
	if(pSelectedTests != NULL)																free(pSelectedTests);
	if(pModulName != NULL)																		free(pModulName);
	if(pInterfaceName != NULL)																free(pInterfaceName);
	if(pValveSerieName != NULL)																free(pValveSerieName);
	if(pControllerTypeName != NULL)														free(pControllerTypeName);
	if(pOptionName != NULL)																		free(pOptionName);
	if(pClusterName != NULL)																	free(pClusterName);
	if(ppArrayFileList != NULL && ResponsValueUser != FALSE)	free(ppArrayFileList);
	if(ppArrayModulList != NULL)															free(ppArrayModulList);
	if(ppArrayInterfaceList != NULL)													free(ppArrayInterfaceList);
	if(ppArrayValveSerieList != NULL)													free(ppArrayValveSerieList);
	if(ppArrayControllerTypeList != NULL)											free(ppArrayControllerTypeList);
	if(ppArrayOptionList != NULL)															free(ppArrayOptionList);
	if(ppArrayClusterList != NULL)														free(ppArrayClusterList);
	if(pTemp != NULL && TempFreeOperation == 0)								free(pTemp);
	if(pExeList != NULL)																			free(pExeList);
	if(pHardwareVentil != NULL)																free(pHardwareVentil);
	if(pStringList != NULL)																		free(pStringList);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt die InitialStateFirmware Ring Elemente mit den in der Datenbank abgespeicherten Werten
//  Input           : 
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
static void WriteInitialStateFirmwareRingLists(void)
{	 
	BOOL8 FindInterfaceEntry = FALSE;
	UINT8 MotionControllerFirmwareState, InterfaceFirmwareState;
	int CountFirmwareEntrys, i, Result;
	char ValveFirmwareVersion[TESTINFORMATION_NAME_LENGTH], MotionControllerFirmware[TESTINFORMATION_NAME_LENGTH], InterfaceFirmware[TESTINFORMATION_NAME_LENGTH];
	char RingElementString[TESTINFORMATION_NAME_LENGTH];
	
	//Initial State Valve Elemente mit Daten aus der Datenbank füllen
	Database_WriteValveFirmwareInRingList();
	Database_WriteMotionControllerFirmwareInRingList();
	Database_WriteInterfaceFirmwareInRingList();
	Database_WriteDriveParameterFileInRingList();
	Database_WriteConfigurationFileInRingList();
	
	//Aktuelle Firmware Informationen aus dem Ventil auslesen, so kann das aktuelle Selektionselement dementsprechend angepasst werden
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.ValveFirmwareVersion, ValveFirmwareVersion, ""); 
	ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.MotionController1FirmwareVersion.ID, 0, &MotionControllerFirmwareState, DT_UINT8, PARAMETER_GET_STATE);
	if(MotionControllerFirmwareState == STATE_ACTIVE)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.MotionController1FirmwareVersion, MotionControllerFirmware, ""); 
	else	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.Valve.ParameterAxis1.SpecificDriveParameter._100AManufacturerSoftwareVersion, MotionControllerFirmware, ""); 
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceType, "");
	ParameterProcessing_SendParameter(Parameter.System.Identification.Firmware.InterfaceFirmwareVersion.ID, 0, &InterfaceFirmwareState, DT_UINT8, PARAMETER_GET_STATE);
	if(InterfaceFirmwareState == STATE_ACTIVE)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Firmware.InterfaceFirmwareVersion, InterfaceFirmware, ""); 
	else if(Parameter.System.Identification.Hardware.InterfaceType.Value == IT_EtherCAT)	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.InterfaceEtherCAT.SlaveFirmware, InterfaceFirmware, "");
	else InterfaceFirmware[0] = 0;
	
	//Valve Firmware
	GetNumListItems(STpanel, STPANEL_RING_ISV_FIRMWARE, &CountFirmwareEntrys); 
	for(i = 0; i < CountFirmwareEntrys; i++)
	{
		Result = GetValueFromIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, i, RingElementString); 
		if(strcmp(ValveFirmwareVersion, RingElementString) == 0)	
		{
			SetCtrlIndex(STpanel, STPANEL_RING_ISV_FIRMWARE, i); 
			break;
		}
	}
	
	//Motion Controller Firmware
	GetNumListItems(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, &CountFirmwareEntrys); 
	for(i = 0; i < CountFirmwareEntrys; i++)
	{
		Result = GetValueFromIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, i, RingElementString); 
		if(strcmp(MotionControllerFirmware, RingElementString) == 0)	
		{
			SetCtrlIndex(STpanel, STPANEL_RING_ISV_MC_FIRMWARE, i); 
			break;
		}
	}
	
	//Interface Firmware
	//wenn das Ventil nicht über ein Feldbus Interface verfügt
	if(InterfaceFirmwareState != STATE_ACTIVE)	SetCtrlIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, 0);
	else
	{
		GetNumListItems(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, &CountFirmwareEntrys); 
		for(i = 0; i < CountFirmwareEntrys; i++)
		{
			Result = GetValueFromIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, i, RingElementString); 
			if(strcmp(InterfaceFirmware, RingElementString) == 0)	
			{
				SetCtrlIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, i);
				FindInterfaceEntry = TRUE;
				break;
			}
		}
		//Wenn der Eintrag nicht gefunden werden kann soll undef auf der Oberfläche stehen
		if(FindInterfaceEntry == FALSE)	SetCtrlIndex(STpanel, STPANEL_RING_ISV_IF_FIRMWARE, 0);
	} 
	
	//Drive Parameter File
	UpdateDriveParameterFileRingList();
	
	//Configuration Parameter File
	UpdateConfigurationParameterFileRingList();
	
	
	//Den Port schliessen 
	HelpFunctions_CloseUSBCommunication();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Aktuallisiert die Drive Parameter Files Selektionsansicht
//  Input           : 
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
static void UpdateDriveParameterFileRingList(void)
{
	BOOL8 FindDriveParameterEntry = FALSE;
	UINT16 i;
	int CountDriveParameterEntrys, Result;
	char DriveParameterFile[DRIVE_PARAMETER_FILE_NAME_LENGTH], RingElementString[TESTINFORMATION_NAME_LENGTH];
	
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Configuration.DriveParametersID, DriveParameterFile, ""); 
	if(strcmp(DriveParameterFile, FILE_NOT_AVAIABLE) == 0)	SetCtrlIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, 0);
	else
	{
		GetNumListItems(STpanel, STPANEL_RING_ISV_DRIVE_PAR, &CountDriveParameterEntrys); 
		for(i = 0; i < CountDriveParameterEntrys; i++)
		{
			Result = GetValueFromIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, i, RingElementString); 
			if(strcmp(DriveParameterFile, RingElementString) == 0)	
			{
				SetCtrlIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, i);
				FindDriveParameterEntry = TRUE;
				break;
			}
		}
		//Wenn der Eintrag nicht gefunden werden kann soll der User Informiert werden
		if(FindDriveParameterEntry == FALSE)	MessagePopup("Drive Parameter ID", "Drive Parameter File ID not find in Database!");
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Aktuallisiert die Configuration Parameter Files Selektionsansicht
//  Input           : 
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
static void UpdateConfigurationParameterFileRingList(void)
{
	BOOL8 FindConfigurationParameterEntry = FALSE;
	UINT16 i;
	int CountConfigurationParameterEntrys, Result;
	char ConfigurationParameterFile[CONFIGURATION_FILE_NAME_LENGTH], RingElementString[TESTINFORMATION_NAME_LENGTH];
	
	HelpFunctions_GetCommandString((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Configuration.ConfigurationParametersID, ConfigurationParameterFile, ""); 
	if(strcmp(ConfigurationParameterFile, FILE_NOT_AVAIABLE) == 0)	SetCtrlIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, 0);
	else
	{
		GetNumListItems(STpanel, STPANEL_RING_ISV_CONFIG_PAR, &CountConfigurationParameterEntrys); 
		for(i = 0; i < CountConfigurationParameterEntrys; i++)
		{
			Result = GetValueFromIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, i, RingElementString); 
			if(strcmp(ConfigurationParameterFile, RingElementString) == 0)	
			{
				SetCtrlIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, i);
				FindConfigurationParameterEntry = TRUE;
				break;
			}
		}
		//Wenn der Eintrag nicht gefunden werden kann soll der User Informiert werden
		if(FindConfigurationParameterEntry == FALSE)	MessagePopup("Configuration Parameter ID", "Configuration Parameter File ID not find in Database!");
	}
}
---









//Panel Callback Functions
//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert das Hauptfenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int  CVICALLBACK MainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	int Error;
	switch (event)
	{
		case EVENT_CLOSE:
		{
			//kann nur geschlossen werden, wenn das Report Fenster geschlossen ist
			if(ReportHandleActive) 	GetPanelAttribute (RPpanel, ATTR_VISIBLE, &VisibleReport);
			else										VisibleReport = 0;
			if(VisibleReport)	MessagePopup("Not able to close TTIC2", "Please Close first the Report Panel");
			else
			{
				SetActivePanel(STpanel);
				
				Error = QuitUserInterface(0);
				DiscardInformation();
				break;
			}
		}
		default:
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert die Collection Liste
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK TestCollectionListbox (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	SINT32 TestCollectionId;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	//to save the information of the mouse
	Point CoordinatesMouse;
	CoordinatesMouse = MakePoint (eventData2, eventData1);
	switch (event)
	{
			case EVENT_COMMIT:
				break;
			case EVENT_RIGHT_CLICK_UP:
			{
				MenuBarHandle = LoadMenuBar (0, "SurfaceTest.uir", MENUBAR_TC);
				MenuItemID = RunPopupMenu(MenuBarHandle, MENUBAR_TC_TESTCOLLECTION, STpanel, CoordinatesMouse.y,CoordinatesMouse.x, 0, 0, 0, 0);
				break;
			}
			case EVENT_LEFT_CLICK_UP:
			{
				ClearListCtrl(STpanel, STPANEL_TREE_INFO); 
				TestCollectionId = CollectionTest_GetTestCollectionInformationFromSelectedEntry(TestCollectionName, TestCollectionDescription);
				InfoListbox_ShowsInformationTestCollection(TestCollectionId);
				break;
			}
			case EVENT_LEFT_DOUBLE_CLICK:
			{
				ClearListCtrl(STpanel, STPANEL_TREE_INFO);
				TestCollectionId = CollectionTest_GetTestCollectionInformationFromSelectedEntry(TestCollectionName, TestCollectionDescription);
				InfoListbox_ShowsInformationTestCollection(TestCollectionId);
				CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
				SurfaceTest_UpdateTestCollectionRingList(TestCollectionId);
				break;
			}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Auswahl Listbox welche die verfügbaren Tests filtert
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK SelectionListbox (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
		{
			switch(SelectedFilter)
			{
				case MODUL_SELECTION:
				{
					SelectFilesFromFilter_SelectFilesFromModulFilterInformation(pTests);
					break;
				}
				case INTERFACE_SELECTION:
				{
					SelectFilesFromFilter_SelectFilesFromInterfaceFilterInformation(pTests);
					break;	
				}
				case VALVESERIES_SELECTION:
				{
					SelectFilesFromFilter_SelectFilesFromValveSerieFilterInformation(pTests);
					break;
				}
				case CONTROLLERTYPE_SELECTION:
				{
					SelectFilesFromFilter_SelectFilesFromControllerTypeFilterInformation(pTests);
					break;
				}
				case OPTION_SELECTION:
				{
					SelectFilesFromFilter_SelectFilesFromOptionFilterInformation(pTests);
					break;
				}
				case NIHARDWARE_SELECTION:		
				{
					SelectFilesFromFilter_SelectFilesFromNIHardwareFilterInformation(pTests);
					break;
				}
			}
		}
		case EVENT_LEFT_CLICK_UP:
		{
			switch(SelectedFilter)
			{
				case MODUL_SELECTION:
				{
					InfoListbox_ShowsInformationModulFilter(ppArrayModulList, pTests);
					break;
				}
				case INTERFACE_SELECTION:
				{
					InfoListbox_ShowsInformationInterfaceFilter(ppArrayInterfaceList, pTests);
					break;
				}
				case VALVESERIES_SELECTION:
				{
					InfoListbox_ShowsInformationValveSerieFilter(ppArrayValveSerieList, pTests);
					break;
				}
				case CONTROLLERTYPE_SELECTION:
				{
					InfoListbox_ShowsInformationControllerTypeFilter(ppArrayControllerTypeList, pTests);
					break;
				}
				case OPTION_SELECTION:
				{
					InfoListbox_ShowsInformationOptionFilter(ppArrayOptionList, pTests);
					break;
				}
				case NIHARDWARE_SELECTION:
				{
					InfoListbox_ShowsInformationNIHardwareFilter(ppArrayNIHardwareList, pTests);
					break;
				}
			}
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert die linke Liste in welchem die möglichen Test angezeigt werden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK AvaiableTests (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	int MenuBarHandle, MenuItemID = 0;
	//to save the information of the mouse
	Point CoordinatesMouse;
	CoordinatesMouse = MakePoint (eventData2, eventData1);
	switch (event)
	{
		case EVENT_COMMIT:
			break;
		case EVENT_LEFT_CLICK_UP:
			DeleteListItem (STpanel, STPANEL_TREE_INFO, 0, -1);
			if(EntriesFiles != 0)	InfoListbox_WriteInfoTestOnScreen("Avaiable", SelectedFile);
			break;
		case EVENT_RIGHT_CLICK_UP:
			MenuBarHandle = LoadMenuBar (0, "SurfaceTest.uir", MENUBAR_AT);
			MenuItemID = RunPopupMenu(MenuBarHandle, MENUBAR_AT_AVAIABLETESTS, STpanel, CoordinatesMouse.y,CoordinatesMouse.x, 0, 0, 0, 0);
			break;
		case EVENT_LEFT_DOUBLE_CLICK:
			AvaiableListbox_MoveAvaiableFile(pTests);
			break;
		case EVENT_SELECTION_CHANGE:
			AvaiableListbox_SelectTestAvaiable(eventData1, eventData2);
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert das rechte Liste in welchem die ausgewählten Test aufgelisten werden, welche Bereit für den Test sind
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK SelectedTests (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int MenuBarHandle, MenuItemID = 0;
	//to save the information of the mouse
	Point CoordinatesMouse;
	CoordinatesMouse = MakePoint (eventData2, eventData1);
	int Relation, Position;
	switch (event)
	{
		case EVENT_COMMIT:
			break;
		case EVENT_LEFT_CLICK_UP:
			DeleteListItem (STpanel, STPANEL_TREE_INFO, 0, -1);
			InfoListbox_WriteInfoTestOnScreen("Selected", SelectedFile);
			break;
		case EVENT_RIGHT_CLICK_UP:
			MenuBarHandle = LoadMenuBar (0, "SurfaceTest.uir", MENUBAR_ST);
			MenuItemID = RunPopupMenu(MenuBarHandle, MENUBAR_ST_SELECTEDTESTS, STpanel, CoordinatesMouse.y,CoordinatesMouse.x, 0, 0, 0, 0);
			break;
		case EVENT_DRAG:
			Relation = eventData2;
			StartIndexDragEvent = Relation;
			break;	
		case EVENT_DROPPED:
			Position = eventData2;
			SelectListbox_ActualSelectFileListAfterDragDrop(TempFreeOperation, StartIndexDragEvent, Position, pTemp);
			break;
		case EVENT_SELECTION_CHANGE:
			SelectListbox_SelectTestCollection(eventData1, eventData2);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert den Run Knopf
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK RunExeFiles (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	BOOL8 ChangeTestCollectionEntries = FALSE;
	SINT32 TestCollectionId;
	int NumberSelectedFile;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH] = "", StringReplace[DATE_TYPE_LENGTH+1] = "";;
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			//Nur ausführen, wenn eine Test Kollektion ausgewählt ist
			GetCtrlIndex(STpanel, STPANEL_RING_ISV_TEST_COL, &NumberSelectedFile);		
			GetValueFromIndex(STpanel, STPANEL_RING_ISV_TEST_COL, NumberSelectedFile, TestCollectionName);
			if(strlen(TestCollectionName) == 0) MessagePopup("Run Test Collection Error","No selected test collection in my test collection menu");
+++			
			//Prüfen ob die Test Kollektion mit den ausgewählten Tests übereinstimmt
			else
			{
				//Wenn alle Test Kollektion ausgewählt ist, so soll das Datum und der Aktiv Status noch aus dem Namen entfernt werden
				if(IsShowAllTestCollectionSelected == TRUE)
				{
					sprintf(StringReplace, " %s", pTestCollections[NumberSelectedFile-1].CreationDate);
					HelpFunctions_StringReplace(StringReplace, "", TestCollectionName);
					if(pTestCollections[NumberSelectedFile-1].ActivityState == ACTIVITY_STATE_NOT_ACTIVE)	HelpFunctions_StringReplace(" Not Active", "", TestCollectionName);
					else																																									HelpFunctions_StringReplace(" Active", "", TestCollectionName);
				}
				TestCollectionId = Database_GetTestCollectionId(TestCollectionName, pTestCollections[NumberSelectedFile-1]);
				ChangeTestCollectionEntries = Database_CheckIfTestCollectionHasChanged(TestCollectionId);
---			
				if(ChangeTestCollectionEntries == FALSE)
				{
					//um mehrer Aufgaben gleichzeiotig zu absolvieren (Report Oberfläche und Ablauf Exe Files)
					CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThreadFunction1, NULL, &ThreadFunctionId1);
					CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThreadFunction2, NULL, &ThreadFunctionId2);
	
					//Warten bis die thread Funktionen abgeschlossen sind
					CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionId1, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
					CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionId2, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
				
					break;
				}
				else MessagePopup("Run Test Collection Error","My test collection has diffrent test defined as in test collection view. Solution: Save the selected test collection in my test collection");
			}
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert was im ersten Thread ausgeführt werden soll 
//									  (So kann nebenbei die Testexe noch ausgeführt werden)
//  Input           : 
//  Output *Pointer : -
//         *return  : Sucessful
//******************************************************************************************************************************
static int CVICALLBACK ThreadFunction1 (void *functionData)
{
	RPpanel = LoadPanel (0, "Report.uir", RPPANEL);
	ReportHandleActive = 1;
	Report_InitTestCollectionTree();
	StatusUserInterfaceRP = RunUserInterface();
	
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Definiert was im zweiten Thread ausgeführt werden soll 
//									  (So kann nebenbei noch die Oberfläche für den Report ausgeführt werden)
//  Input           : 
//  Output *Pointer : -
//         *return  : Sucessful
//******************************************************************************************************************************
static int CVICALLBACK ThreadFunction2 (void *functionData)
{
	TestAreRunning = TRUE;
	
	//öffnet die Ausführungsfunktion für die Exen
	ChangeHardware_SetExecutableToZero();
	SelectListbox_RunExeFilesFunction(PathTestFolder);
	
	TestAreRunning = FALSE;
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Auswahlfeld des Collection Liste
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK SelectedFilterRing (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int Sucess;
	switch (event)
	{
		case EVENT_COMMIT:
			break;
		case EVENT_VAL_CHANGED: 
		{
			Sucess = GetCtrlIndex(STpanel, STPANEL_RING_SC, &SelectedFilter);
			switch(SelectedFilter)
			{
				case MODUL_SELECTION:						GenerateFilterList_WriteModulFilterInTextBox();						break;
				case INTERFACE_SELECTION:				GenerateFilterList_WriteInterfaceFilterInTextBox();				break;
				case VALVESERIES_SELECTION:			GenerateFilterList_WriteValveSerieFilterInTextBox();			break;
				case CONTROLLERTYPE_SELECTION:	GenerateFilterList_WriteControllerTypeFilterInTextBox();	break;
				case OPTION_SELECTION:					GenerateFilterList_WriteOptionFilterInTextBox();					break;
				case NIHARDWARE_SELECTION:			GenerateFilterList_WriteNIHardwareFilterInTextBox();			break;
				default:																																									break;
			}
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Pfeil Button nach rechts, welcher die ausgewählten Test von den möglichen zu den ausgewählten Tests verschiebt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK SelectFileButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			AvaiableListbox_MoveAvaiableFile(pTests);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Pfeil Button nach links, welcher die markierten Tests aus der ausgewählten Liste entfernt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK DeselectFileButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SelectListbox_RemoveSelectFile(EmptyTestStructure);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Pfeil Button nach oben, um den Test früher ausführen zu können
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK pushUpButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SelectListbox_DecrementIndexSelectedFile(pTemp);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Pfeil Button nach unten, um den Test später ausführen zu können
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK pushDownButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SelectListbox_IncrementIndexSelectFile(MaxIndex, pTemp);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Button um alle verfügbaren Tests anzeigen zu lassen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK ShowAllAvaiableTests (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			AvaiableListbox_ShowAllTests();
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Button um alle ausführbaren Tests anzeigen zu lassen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK ShowExecutableAvaiableTests (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			AvaiableListbox_ShowExecutableTests();
			break;
	}
	return 0;
}







//Callback Functions for Menus
//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : sucht nach bestimmten Dateien (File Extension Box) welche im angegebenen Ordner (Path to search) sich befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK FindTestsListboxAvaiable(int menubar, int menuItem, void *callbackData, int panel)
{
	//zuerst alles entfernen aus dem Speicher
	DiscardInformation();
	//alles von vorne beginnen
	SurfaceTest_Init();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : markiert alle Files welche sich in der linken Liste (vorhandenen Test) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK SelectAllTestsListboxAvaiable(int menubar, int menuItem, void *callbackData, int panel)
{
	AvaiableListbox_SelectAllTestsAvaiable();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : unmarkiert alle Files welche sich in der linken Liste (vorhandenen Test) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK DeselectAllTestsListboxAvaiable(int menubar, int menuItem, void *callbackData, int panel)
{
	AvaiableListbox_DeselectAllTestsAvaiable();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : wenn der User Hilfe in der linken Liste(verfügbare Tests) auswählt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK HelpListboxAvaiable(int menubar, int menuItem, void *callbackData, int panel)
{
	MessagePopup("Help", "Select all test, which you want to test. You have many option, which select the files automatecly. Under Test/Modul/Interface Collection Textfield, you have the possibilities for select all files in the selected modul/interface or your own test collection. After your click the right dart, you will see the test in the selected File Listbox");
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : löscht alle aufgelisteten Test welche sich in der linken Liste(verfügbare Tests)
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK ClearTestsListboxAvaiable(int menubar, int menuItem, void *callbackData, int panel)
{
	AvaiableListbox_ClearAvaiableTestsListbox();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : wenn im Textfeld Test Collection Deselect All Files ausgewählt wird 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK ClearListboxSelected(int menubar, int menuItem, void *callbackData, int panel)
{
	SelectListbox_ClearSelectListBox(EmptyTestStructure);
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : markiert alle Tests welche sich in der rechten Liste (Test Collection) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK SelectAllTestsListboxSelect(int menubar, int menuItem, void *callbackData, int panel)
{
	SelectListbox_SelectAllTestsSelect();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : unmarkiert alle Tests welche sich in der rechten Liste (ausführbare Tests) befinden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK DeselectAllTestsListboxSelect(int menubar, int menuItem, void *callbackData, int panel)
{
	SelectListbox_DeselectAllTestsSelect();
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : stellt die Anzahl Durchläufe der ausgewählten Tests ein
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK IterationAllSelectedTests(int menubar, int menuItem, void *callbackData, int panel)
{
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : stellt die zeitliche Länge der ausgewählten Tests ein
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK LengthOfTimeAllSelectedTests(int menubar, int menuItem, void *callbackData, int panel)
{
}

//******************************************************************************************************************************
//  Procedure/Callback  Menu
//******************************************************************************************************************************
//  Description     : wenn der User das Help aus dem Menu von der rechten Liste auswählt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK HelpListboxSelected(int menubar, int menuItem, void *callbackData, int panel)
{
	MessagePopup("Help", "All the exe files, which are listed in the textfield, will be running after push the run button");
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : löscht die Test Zusammenstellung, wenn im im Menu TestCollection das Feld Clear ausgewählt wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK ClearTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Wird der Pass Command RadioButton ausgewählt, so wird keine Pass Command Mitteilungen im Report angezeigt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK PassCommandOption (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
		{
			HelpFunctions_WritePassCommandOption();		
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Der User wählt wie viel mal die TestKollektion ausgeführt wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK IterationCollection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal(STpanel, STPANEL_ITERATIONCOLLECTION, &IterationTestCollectionMaxUser);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Der User wählt wie lange die TestKollektion ausgeführt wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK DuratingTestCollection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal(STpanel, STPANEL_NUMERIC_DURATINGTC, &DuratingTestCollectionMaxUser);
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Der User wählt wie viel mal die TestKollektion ausgeführt wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK TerminateConditionTestCollection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int TerminateByErrorInt;
	
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			HelpFunctions_WriteTerminateByErrorOption();
			
			GetCtrlVal(STpanel, STPANEL_BT_TERMINATECONDITION, &TerminateByErrorInt);
			TerminateByError = (BOOL8)TerminateByErrorInt;
			if(TerminateByError == 0)				HelpFunctions_WriteGlobalVariableTestOptions(TERMINATE_BY_ERROR_OPTION_INDEX, "0");
			else if(TerminateByError == 1)	HelpFunctions_WriteGlobalVariableTestOptions(TERMINATE_BY_ERROR_OPTION_INDEX, "1");
			
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Auswahl ob die Zeitdauer oder die Anzahl Widerholungen definiert werden soll durch den User
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK ExecutionTB (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			GetCtrlVal(STpanel, STPANEL_TB_EXECUTION, &ExecutionButtonValue);
			SelectListbox_ExecutionTB(ExecutionButtonValue);
			break;
	}
	return 0;
}
+++
//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : User kann ein neue Drive Parameter File Eintrag erstellen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK NewDriveParameterFileEntry (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ResultUser;
	char DriveParameterFileName[DRIVE_PARAMETER_FILE_NAME_LENGTH+1];
	size_t ResonseLength = (sizeof(char) * DRIVE_PARAMETER_FILE_NAME_LENGTH);
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			//User muss eine neue Drive Parameter File Name angeben
			ResultUser = PromptPopup("Add Drive Parameter File Entry", "Please write drive parameter id with index", DriveParameterFileName, ResonseLength);
			if(ResultUser != VAL_USER_CANCEL)
			{
				Database_WriteDriveParameterFile(DriveParameterFileName);
				//Jetzt das Auswahlfeld updaten
				Database_WriteDriveParameterFileInRingList();
				UpdateDriveParameterFileRingList();
				HelpFunctions_CloseUSBCommunication();
			}
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : User kann den selektierten Drive Parameter File Eintrag löschen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK DeleteDriveParameterFileEntry (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ResponseMenu, SelectedIndex;
	char DriveParameterFileName[DRIVE_PARAMETER_FILE_NAME_LENGTH];
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			ResponseMenu = ConfirmPopup("Delete drive parameter file entry", "Are you sure to delete actual selected drive parameter file entry?");
			GetCtrlIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, &SelectedIndex);
			GetValueFromIndex(STpanel, STPANEL_RING_ISV_DRIVE_PAR, SelectedIndex, DriveParameterFileName);
			if(ResponseMenu == YES)	Database_DeleteDriveParameterFile(DriveParameterFileName);
			//Jetzt das Auswahlfeld updaten
			Database_WriteDriveParameterFileInRingList();
			UpdateDriveParameterFileRingList();
			HelpFunctions_CloseUSBCommunication();
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Es werden alle Drive Parameter Files angezeigt (Auch die inaktiven)
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK ShowDriveParameterFiles (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ShowDriveParameterFileValue;
	switch (event)
	{
		case EVENT_COMMIT:
		{
			GetCtrlVal(STpanel, STPANEL_BUTTON_SHOW_DRIVE_PAR, &ShowDriveParameterFileValue);
			if(ShowDriveParameterFileValue == SHOW_ALL)	IsShowAllDriveParameterFilesSelected = TRUE;
			else																				IsShowAllDriveParameterFilesSelected = FALSE;
			Database_WriteDriveParameterFileInRingList();
			break;
		}
	}
	return 0;
}


//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : User kann ein neuer Configuration Parameter File Eintrag erstellen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK NewConfigurationParameterFileEntry (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ResultUser;
	char ConfigurationParameterFileName[CONFIGURATION_FILE_NAME_LENGTH+1];
	size_t ResonseLength = (sizeof(char) * CONFIGURATION_FILE_NAME_LENGTH);
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			//User muss eine neue Configuraton Parameter File Name angeben
			ResultUser = PromptPopup("Add Configuration Parameter File Entry", "Please write configuration parameter id with index", ConfigurationParameterFileName, ResonseLength);
			Database_WriteConfigurationParameterFile(ConfigurationParameterFileName);
			//Jetzt das Auswahlfeld updaten
			Database_WriteConfigurationFileInRingList();
			UpdateConfigurationParameterFileRingList();
			HelpFunctions_CloseUSBCommunication();
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : User kann den selektierten Configuration Parameter File Eintrag löschen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK DeleteConfigurationParameterFileEntry (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ResponseMenu, SelectedIndex;
	char ConfigurationParameterFileName[CONFIGURATION_FILE_NAME_LENGTH];
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			ResponseMenu = ConfirmPopup("Delete configuration parameter file entry", "Are you sure to delete actual configuration drive parameter file entry?");
			GetCtrlIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, &SelectedIndex);
			GetValueFromIndex(STpanel, STPANEL_RING_ISV_CONFIG_PAR, SelectedIndex, ConfigurationParameterFileName);
			if(ResponseMenu == YES)	Database_DeleteConfigurationParameterFile(ConfigurationParameterFileName);
			//Jetzt das Auswahlfeld updaten
			Database_WriteConfigurationFileInRingList();
			UpdateConfigurationParameterFileRingList();
			HelpFunctions_CloseUSBCommunication();
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Es werden alle Configuration Parameter Files angezeigt (Auch die inaktiven)
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK ShowConfigurationParameterFiles (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	int ShowConfigurationParameterFileValue;
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			GetCtrlVal(STpanel, STPANEL_BUTTON_SHOW_CONF_PARM, &ShowConfigurationParameterFileValue);
			if(ShowConfigurationParameterFileValue == SHOW_ALL)	IsShowAllConfigurationParameterFilesSelected = TRUE;
			else																								IsShowAllConfigurationParameterFilesSelected = FALSE;
			Database_WriteConfigurationFileInRingList();
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Wählt der Benutzer eine Test Collection aus, so sollen die Test der Kollektion in der Selektionsfeld angezeigt werden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK InitialStateValve_TestCollection_Ring (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	SINT32 TestCollectionId;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	switch (event)
	{
		case EVENT_COMMIT:
		{
			ClearListCtrl(STpanel, STPANEL_TREE_INFO);
			TestCollectionId = Surface_GetSelectedTestCollectionInformation(TestCollectionName, TestCollectionDescription);
			InfoListbox_ShowsInformationTestCollection(TestCollectionId);
			CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
			break;
		}
	}
	return 0;
}
---