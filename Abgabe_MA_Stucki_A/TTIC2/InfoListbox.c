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
#include <userint.h>
#include <utility.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <toolbox.h>

#include "Database.h"
#include "Definelist.h"
#include "GenerateFilterList.h"
#include "HelpFunctions.h" 
#include "InfoListbox.h"
#include "SurfaceTest.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Info des ausgwählten Files in das Info Textfeld
//  Input           : pListbox:										Gibt an in welcher Listbox der User sich befindet
//									: SelectedFile:								Gibt den Index des markierten Tests an
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_WriteInfoTestOnScreen(char *pListbox, int SelectedFile)
{
	UINT8 ModificationTextAvaiable = 0;
	SINT16 TestVersion = 0;
	SINT32 TestVersionId = -1;
	int i, CountAvaiableTests = 0, CountTests = 0, IndexAvaiable = 0;
	char TestName[TESTINFORMATION_NAME_LENGTH], InfoLineString[TESTINFOMATION_DESCRIPTION_LENGTH+10], TestDescription[TESTINFOMATION_DESCRIPTION_LENGTH];
	char TestCreationDate[DATE_TYPE_LENGTH], TestCreationAuthor[TESTINFOMATION_CREATIONAUTHOR_LENGTH], TestModification[TESTVERSION_MODIFICATION_LENGTH], TestDurating[TESTVERSION_DURATING_LENGTH]; 
	char TestModul[MODULNAMES_NAME_LENGTH], *pInterface = NULL, *pValveSerie = NULL, *pController = NULL, *pOption = NULL, *pTestHardware = NULL;
	
	//muss hier die Anzahl möglicher Tests erhalten
	CountTests = EntriesFiles;
	if(CountTests > 0)
	{
		//errChk braucht ein Label in welchem er hineinspringt bei einem Fehler
		if (strcmp(pListbox, "Avaiable") == 0)	
		{
			//Abfragen wieviele Test sind in der Test Collection Fenster
			GetCtrlIndex(STpanel, STPANEL_TREE_AVAIABLETESTS, &SelectedFile);
			//Wenn ich die Info darstellen möchte, brauche ich den Index des ausgewählten Kollektionstests von der Sparte mit den vorhandenen Tests
			//brauche die Anzahl aller möglichen Test zum alle Namen miteinander zu vergleichen können
			if(SelectedFile >= 0)
			{
				for(i = 0; i < EntriesFiles ; i++)
				{
					//EXE Namen überprüfen um den richtigen Eintrag zu erwischen
					if(pAvaiableTests[i].Visible == 1)
					{
						IndexAvaiable++;
						if((IndexAvaiable - 1) == SelectedFile)	
						{
							TestVersionId = pTests[i].TestVersionId;
							break;
						}
					}
				}  //end for EntriesFiles
			}	//end if selected files
		}
		if (strcmp(pListbox, "Selected") == 0)	
		{
			//Abfragen wieviele Test sind in der Test Collection Fenster
			GetCtrlIndex(STpanel, STPANEL_TREE_TESTCOLLECTION, &SelectedFile);
			GetNumListItems(STpanel, STPANEL_TREE_AVAIABLETESTS, &CountAvaiableTests);
			//Wenn ich die Info darstellen möchte, brauche ich den Index des ausgewählten Kollektionstests von der Sparte mit den vorhandenen Tests
			//brauche die Anzahl aller möglichen Test zum alle Namen mitienander zu vergleichen können
			if(SelectedFile >= 0)
			{
				for(i = 0; i < CountTests ; i++)
				{
					//EXE Namen überprüfen um den richtigen Eintrag zu erwischen
					if(!(pSelectedTests == NULL) && !(pSelectedTests[0].pTests->TestName == NULL) && strlen(pSelectedTests[0].pTests->TestName) != 0 && strcmp(pTests[i].TestName, pSelectedTests[SelectedFile].pTests->TestName) == 0)
					{
						 TestVersionId = pTests[i].TestVersionId;
						 break;
					}
				}  //end for EntriesFiles
			}	//end if selected files
		} //end if selected listbox								
	
		//Nun die Informationen aus der Datenbank heraus holen 
		//(Dazu zählen Name, Beschreibung, Datum, Autor, Version, Modification, Dauer, Modul, Interface, ValveSerie, Controller, Option, TestHardware)
		Database_GetTestInformation(TestVersionId, TestName, TestDescription, TestCreationDate, TestCreationAuthor);
		sprintf(InfoLineString, "Testname: %s", TestName); 
		if(strlen(TestName) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, 0, InfoLineString, 0);
		sprintf(InfoLineString, "Beschreibung: %s", TestDescription);
		if(strlen(TestDescription) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, 1, InfoLineString, 0);
		sprintf(InfoLineString, "Erstellungsdatum: %s", TestCreationDate);
		if(strlen(TestCreationDate) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, 2, InfoLineString, 0);
		sprintf(InfoLineString, "Erstellt durch: %s", TestCreationAuthor);
		if(strlen(TestCreationAuthor) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, 3, InfoLineString, 0);
		Database_GetTestVersionInformation(TestVersionId, &TestVersion, TestModification, TestDurating, TestModul);
		sprintf(InfoLineString, "Testversion: %d", TestVersion);
		if(strlen(TestCreationAuthor) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, 4, InfoLineString, 0);
		if(TestVersion > 1)	
		{
			sprintf(InfoLineString, "Modifikation: %s", TestModification);
			if(strlen(TestModification) != 0)	
			{
				InsertListItem (STpanel, STPANEL_TREE_INFO, 5, InfoLineString, 0);
				ModificationTextAvaiable = 1;
			}
		}
		sprintf(InfoLineString, "Zeitdauer: %s", TestDurating);
		if(strlen(TestDurating) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (5+ModificationTextAvaiable), InfoLineString, 0);
		sprintf(InfoLineString, "Modul: %s", TestModul);
		if(TestVersionId != -1)
		{
			if(strlen(TestModul) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (6+ModificationTextAvaiable), InfoLineString, 0);
			pInterface = Database_GetInterfaceTestHardware(TestVersionId);
			sprintf(InfoLineString, "Interface: %s", pInterface);
			if(strlen(pInterface) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (7+ModificationTextAvaiable), InfoLineString, 0);
			pValveSerie = Database_GetValveSerieTestHardware(TestVersionId);
			sprintf(InfoLineString, "Ventilreihe: %s", pValveSerie);
			if(strlen(pValveSerie) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (8+ModificationTextAvaiable), InfoLineString, 0);
			pController = Database_GetControllerTypeTestHardware(TestVersionId);
			sprintf(InfoLineString, "Controller: %s", pController);
			if(strlen(pController) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (9+ModificationTextAvaiable), InfoLineString, 0);
			pOption = Database_GetOptionTypeTestHardware(TestVersionId);
			sprintf(InfoLineString, "Option: %s", pOption);
			if(strlen(pOption) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (10+ModificationTextAvaiable), InfoLineString, 0);
			pTestHardware = Database_GetTestHardware(TestVersionId);
			sprintf(InfoLineString, "Test Hardware: %s", pTestHardware);
			if(strlen(pTestHardware) != 0)	InsertListItem (STpanel, STPANEL_TREE_INFO, (11+ModificationTextAvaiable), InfoLineString, 0);
		}
	}	
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten Test Kollektion in das Info Textfeld
//  Input           : TestCollectionId:	Id der Test Kollection
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationTestCollection(SINT32 TestCollectionId)
{
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	Database_GetTestCollectionTable(TestCollectionId, TestCollectionName, TestCollectionDescription);
	
	if(TestCollectionId >= 0)
	{
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, TestCollectionName, 0);
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, TestCollectionDescription, 0);
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests beinhaltet die Test Collection:", 0);
	
		Database_WriteTestsFromTestCollectionInInfoListbox(TestCollectionId);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten Modul Kollektion in das Info Textfeld
//  Input           : ppArrayModulList: Gibt alle Tests an, welche zu den jeweiligen Modulen eingeteilt wurden
//									: pTests:						Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationModulFilter(char **ppArrayModulList, struct test_type *pTests)
{
	char SelectedModul[MAX_PATHNAME_LEN];
	int NumberSelectedModul, i;
	SelectedModul[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedModul);
	strcat(SelectedModul, pModulList[NumberSelectedModul].ModulName);
	
	//löscht den Inhalt der Info Textbox
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		
	InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden vom Modul unterstützt:", 0);
	
	//write the exe files of the selected modul in the info textbox
	for(i = 0; i < EntriesFiles; i++)
	{
		if(strcmp(SelectedModul, Database_GetModulName(pTests[i].TestVersionId)) == 0)	InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten Interface Kollektion in das Info Textfeld
//  Input           : ppArrayInterfaceList: Gibt alle Tests an, welche zu den jeweiligen Interfaces eingeteilt wurden
//									: pTests:								Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationInterfaceFilter(char **ppArrayInterfaceList, struct test_type *pTests)
{
	SINT32 InterfaceTypesId = -1, InterfaceTypesAllId = -1;
	char SelectedInterface[MAX_PATHNAME_LEN];
	int NumberSelectedFile, i;
	SelectedInterface[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedFile);
	//nur wenn auch Interfaces vorhanden sind anzeigen
	if(NumberSelectedFile >= 0)	
	{	
		strcat(SelectedInterface, pInterfaceList[NumberSelectedFile].InterfaceName);
		InterfaceTypesId = Database_GetInterfaceTypeIdWithEnum(SelectedInterface);
		InterfaceTypesAllId = Database_GetInterfaceTypeIdWithEnum("All");
		
		//löscht den Inhalt der Info Textbox
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden vom Interface unterstützt:", 0);
		
		//write the exe files of the selected interface in the info textbox 
		for(i = 0; i < EntriesFiles; i++)
		{	
			if(Database_IsTestInterfaceCompatibleWithTestHardware(pTests[i].TestVersionId, InterfaceTypesId, InterfaceTypesAllId) == TRUE) InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten ValveSeries Kollektion in das Info Textfeld
//  Input           : ppArrayValveSerieList: Gibt alle Tests an, welche zu den jeweiligen Ventil eingeteilt wurden
//									: pTests:									Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationValveSerieFilter(char **ppArrayValveSerieList, struct test_type *pTests)
{
	SINT32 ValveSerieTypesId = -1, ValveSerieTypesAllId = -1;
	char SelectedValveSerie[MAX_PATHNAME_LEN];
	int NumberSelectedFile, i;
	SelectedValveSerie[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedFile);
	//nur wenn auch Interfaces vorhanden sind anzeigen
	if(NumberSelectedFile >= 0)	
	{
		strcat(SelectedValveSerie, pValveSerieList[NumberSelectedFile].ValveSerieName);
		ValveSerieTypesId = Database_GetValveSerieIdWithEnum(SelectedValveSerie);
		ValveSerieTypesAllId = Database_GetValveSerieIdWithEnum("All");
		
		//löscht den Inhalt der Info Textbox
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden von der Ventilhardware unterstützt:", 0);
		
		//write the exe files of the selected valve series in the info textbox 
		for(i = 0; i < EntriesFiles; i++)
		{
			if(Database_IsTestValveSerieCompatibleWithTestHardware(pTests[i].TestVersionId, ValveSerieTypesId, ValveSerieTypesAllId) == TRUE) InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten ControllerType Kollektion in das Info Textfeld
//  Input           : ppArrayControllerTypeList:	Gibt alle Tests an, welche zu den jeweiligen Controller eingeteilt wurden
//									: pTests:											Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationControllerTypeFilter(char **ppArrayControllerTypeList, struct test_type *pTests)
{
	SINT32 ControllerTypesId = -1, ControllerTypesAllId = -1;
	int NumberSelectedFile, i;
	char SelectedControllerType[MAX_PATHNAME_LEN];
	
	SelectedControllerType[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedFile);
	//nur wenn auch Interfaces vorhanden sind anzeigen
	if(NumberSelectedFile >= 0)	
	{
		strcat(SelectedControllerType, pControllerTypeList[NumberSelectedFile].ControllerTypeName);
		ControllerTypesId = Database_GetControllerTypeIdWithEnum(SelectedControllerType);
		ControllerTypesAllId = Database_GetControllerTypeIdWithEnum("All");
		
		//löscht den Inhalt der Info Textbox
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden vom Controller Type unterstützt:", 0);
		
		//write the exe files of the selected controller in the info textbox 
		for(i = 0; i < EntriesFiles; i++)
		{
			if(Database_IsTestControllerCompatibleWithTestHardware(pTests[i].TestVersionId, ControllerTypesId, ControllerTypesAllId) == TRUE) InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten Option Kollektion in das Info Textfeld
//  Input           : ppArrayOptionList:	Gibt alle Tests an, welche zu den jeweiligen Option eingeteilt wurden
//									: pTests:							Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationOptionFilter(char **ppArrayOptionList, struct test_type *pTests)
{
	SINT32 OptionTypesId = -1, OptionTypesAllId = -1;
	char SelectedOption[MAX_PATHNAME_LEN];
	int NumberSelectedFile, i;
	SelectedOption[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedFile);
	//nur wenn auch Interfaces vorhanden sind anzeigen
	if(NumberSelectedFile >= 0)	
	{
		strcat(SelectedOption, pOptionList[NumberSelectedFile].OptionName);
		OptionTypesId = Database_GetSingleOptionTypeIdWithEnum(SelectedOption);
		OptionTypesAllId = Database_GetSingleOptionTypeIdWithEnum("All");
		
		//löscht den Inhalt der Info Textbox
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden von der Option unterstützt:", 0);
		
		//write the exe files of the selected modul in the info textbox 
		for(i = 0; i < EntriesFiles; i++)
		{
			if(Database_IsTestOptionCompatibleWithTestHardware(pTests[i].TestVersionId, OptionTypesId, OptionTypesAllId) == TRUE) InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt die Informationen des aktuell ausgewählten NI Hardware Kollektion in das Info Textfeld
//  Input           : ppArrayNIHardwareList:	Gibt alle Tests an, welche zu den jeweiligen NI Hardware eingeteilt wurden
//									: pTests:							Struktur in welcher alle Informationen über die Hardware der einzelnen Tests abgespeichert ist
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void InfoListbox_ShowsInformationNIHardwareFilter(char **ppArrayNIHardwareList, struct test_type *pTests)
{
	SINT32 TestHardwareTypesId = -1, TestHardwareTypesAllId = -1; 
	char SelectedNIHardware[MAX_PATHNAME_LEN];
	int NumberSelectedFile, i;
	SelectedNIHardware[0] = 0;
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedFile);
	//nur wenn auch Interfaces vorhanden sind anzeigen
	if(NumberSelectedFile >= 0)	
	{
		strcat(SelectedNIHardware, pNIHardwareTypeList[NumberSelectedFile].NIHardwareTypeName);
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum(SelectedNIHardware);
		TestHardwareTypesAllId = Database_GetNIHardwareTypeIdWithEnum("All");
	
		//löscht den Inhalt der Info Textbox
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		
		InsertListItem(STpanel, STPANEL_TREE_INFO, -1, "Folgende Tests werden von der Test Hardware unterstützt:", 0);
		
		//write the exe files of the selected modul in the info textbox 
		for(i = 0; i < EntriesFiles; i++)
		{
			if(Database_IsTestNIHardwareCompatibleWithTestHardware(pTests[i].TestVersionId, TestHardwareTypesId, TestHardwareTypesAllId) == TRUE) InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
		}
	}
}
---







