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

#include "Database.h"
#include "Definelist.h"
#include "CollectionTest.h"
#include "HelpFunctions.h"
#include "InfoListbox.h"
#include "SelectListbox.h"
#include "SurfaceTest.h"
#include "Typedefinitions.h" 

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
BOOL8 IsShowAllTestCollectionSelected = FALSE;
char FileNameSaveTestCollectionAfterSaveDirection[MAX_PATHNAME_LEN];

//Panel Callback Functions
//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert das Test Kollektion Fenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK MenuTestCollection(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			QuitUserInterface(0);
		break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Wird ausgeführt, wenn der Benutzer nach New Test Collection auf den Save Knopf drückt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK SaveTestCollectionInformation (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	SINT32 TestCollectionId;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH] = "";
+++
	switch (event)
	{
		case EVENT_COMMIT:
		{
			CollectionTest_NewTestCollectionEntryInDatabase(TestCollectionName);
			TestCollectionId = Database_GetActiveTestCollectionId(TestCollectionName);
			SetActivePanel(TCpanel);
			QuitUserInterface(0);
			Database_WriteTestCollectionsStructure();
			CollectionTest_WriteTestCollectionInTextBox();
			Database_WriteTestCollectionNameInRingList();
			if(strlen(TestCollectionName) > 0)	SurfaceTest_UpdateTestCollectionRingList(TestCollectionId);
			break;
		}
	}
---
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : schliesst das Test Collection Fenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK CloseMenuTestCollection (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		{
			SetActivePanel(TCpanel);
			QuitUserInterface(0);
			break;
		}
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback Menu
//******************************************************************************************************************************
//  Description     : Drück der Benutzer den Cancel Button, so soll das Fenster ohne Änderungen geschlossen werden
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK CancelTestCollectionInformation (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		{
			SetActivePanel(TCpanel);
			QuitUserInterface(0);
			break;
		}
	}
	return 0;
}






//Test Kollektion rechts Klick Optionen
//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Ermöglicht dem Benutzer eine neue Test Kollektion zu erstellen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK NewTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	int StatusUserInterfaceTC;
	TCpanel = LoadPanel (0, "CollectionTest.uir", TCPANEL);
	DisplayPanel (TCpanel);
	StatusUserInterfaceTC = RunUserInterface();
	DiscardPanel (TCpanel);
}
+++
//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Speichert die neue Test Kollektion ab mit den alten Kollektionsinformationen. (Name und Beschreibung bleibt gleich) 
//										Er wird nur abgespeichert, wenn sich an der Test zusammenstellung was geändert hat
//										Dabei wird der alte Eintrag überschrieben, wenn er noch nicht in einem Resultat vorkommt. 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK SaveTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	BOOL8 ChangeTestCollectionEntries = FALSE;
	SINT32 TestCollectionId;
	int ResponseMenu;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	//Zuerst prüfen ob eine Test Kollektion ausgewählt ist
	ResponseMenu = ConfirmPopup("Rewrite test collection entry", "Are you sure to rewrite selected test collection entry? (test collection in ring menu)");
	if(ResponseMenu == TRUE)	
	{
		TestCollectionId = Surface_GetSelectedTestCollectionInformation(TestCollectionName, TestCollectionDescription);
		if(TestCollectionId < 0) ConfirmPopup("Rewrite test collection entry", "No avaiable test collection are selected in ring menu");
		else
		{
			//Prüfen ob sich an der Test Zusammenstellung sich was verändert hat (Liste der Selktierten Tests mit Einträge in der Datenbank für die Test Kollektion)
			ChangeTestCollectionEntries = Database_CheckIfTestCollectionHasChanged(TestCollectionId);
			if(ChangeTestCollectionEntries == FALSE)	MessagePopup("Rewrite test collection entry", "Test collection has no changes");
			//Nun sollen die die neuen Verlinkungen in der TestCollection_TestInformation geschrieben werden
			else
			{
				Database_DeleteTestCollectionEntry(TestCollectionId);
				Database_WriteTestCollectionTable(TestCollectionName, TestCollectionDescription);
				Database_WriteTestCollectionsStructure();
				Database_WriteTestCollectionNameInRingList();
				SurfaceTest_UpdateTestCollectionRingList(TestCollectionId);
				CollectionTest_WriteTestCollectionInTextBox();
				TestCollectionId = Database_GetActiveTestCollectionId(TestCollectionName);
				CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Das aktuelle ausgewählte Test Kollektion im Auswahlfeld wird gelöscht, wenn er noch nicht in einem Resultat vorkommt
//										Ansonsten wird das ActivityState auf FALSE gesetzt und normalerweise auf der Oberfläche nicht mehr angezeigt
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK DeleteTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	SINT32 TestCollectionId;
	int ResponseMenu;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	ResponseMenu = ConfirmPopup("Delete test collection entry", "Are you sure to delete selected test collection entry?");
	TestCollectionId = CollectionTest_GetTestCollectionInformationFromSelectedEntry(TestCollectionName, TestCollectionDescription);
	if(TestCollectionId >= 0 && ResponseMenu == YES)
	{
		Database_DeleteTestCollectionEntry(TestCollectionId);
		ClearListCtrl(STpanel, STPANEL_TREE_INFO);
		InfoListbox_ShowsInformationTestCollection(TestCollectionId);
		CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
		Database_WriteTestCollectionsStructure();
		Database_WriteTestCollectionNameInRingList();
		CollectionTest_WriteTestCollectionInTextBox();
	}
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Zeigt alle Test Kollektionen im Menu an. Auch die bei welchem der ActivityState auf FALSE steht
//										Zudem wird der Name noch mit dem Datum erweitert, um die einzelnen Kollektionen unterscheiden zu können
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK ShowAllTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	SINT32 TestCollectionId;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	IsShowAllTestCollectionSelected = TRUE; 
	Database_WriteTestCollectionsStructure();
	Database_WriteTestCollectionNameInRingList();
	CollectionTest_WriteTestCollectionInTextBox();
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	//Kollektion vom ausgewählten Ring Menu nehmen
	TestCollectionId = Surface_GetSelectedTestCollectionInformation(TestCollectionName, TestCollectionDescription);
	InfoListbox_ShowsInformationTestCollection(TestCollectionId);
	CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Zeigt nur die aktiven Test Kollektionen im Menu an. 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CVICALLBACK ShowActiveTestCollection(int menubar, int menuItem, void *callbackData, int panel)
{
	SINT32 TestCollectionId;
	char TestCollectionName[TESTCOLLECTION_NAME_LENGTH], TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH];
	
	IsShowAllTestCollectionSelected = FALSE;
	Database_WriteTestCollectionsStructure();
	Database_WriteTestCollectionNameInRingList();
	CollectionTest_WriteTestCollectionInTextBox();
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	//Kollektion vom ausgewählten Ring Menu nehmen  
	TestCollectionId = Surface_GetSelectedTestCollectionInformation(TestCollectionName, TestCollectionDescription);
	InfoListbox_ShowsInformationTestCollection(TestCollectionId);
	CollectionTest_SelectFilesFromTestCollectionInformation(TestCollectionId);
}
---






//own defined Functions witch starts in the controlling Callback Functions
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt die vorhandenen Test Kollektionen in das Test Collection Textfeld 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CollectionTest_WriteTestCollectionInTextBox(void)
{
	UINT16 i;
	char TestCollectionNameWithDate[TESTCOLLECTION_NAME_LENGTH+DATE_TYPE_LENGTH];
	
	//löscht alle Elemente in der Testkollektion
	ClearListCtrl(STpanel, STPANEL_LISTBOX_TC);
	//löscht das Info Textfeld
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	
	if(CountTestCollections > 0)
	{
		SetCtrlAttribute(STpanel, STPANEL_LISTBOX_TC, ATTR_DATA_TYPE, VAL_STRING);
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
				InsertListItem (STpanel, STPANEL_LISTBOX_TC, -1, TestCollectionNameWithDate, TestCollectionNameWithDate);
			}
			//-1 bedeutet ans Ende anfügen
			//Sollen nur Test angezeigt werden, bei welchem der Activity State auf TRUE steht
			else if(pTestCollections[i].ActivityState == ACTIVITY_STATE_ACTIVE)	InsertListItem (STpanel, STPANEL_LISTBOX_TC, -1, pTestCollections[i].TestCollectionName, pTestCollections[i].TestCollectionName);
		}
	}
}
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Die aufgelisteten Tests, welche in der Test Kollektion definiert worden sind, 
//										werden in der rechten Liste (ausführbare Tests) markiert 
//  Input           : TestCollectionId: Id der Test ausgewählten Test Kollektiion
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void CollectionTest_SelectFilesFromTestCollectionInformation(SINT32 TestCollectionId)
{
	SelectListbox_ClearSelectListBox(EmptyTestStructure);
	
	//Alle Tests markieren, welche in der Test Kollektion definiert wurden
	Database_WriteTestsInSelectedListboxFromTestCollection(TestCollectionId);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt einen neuen Kollektion Eintrag in der Datenbank 
//  Input           : 
//  Output *Pointer : pName: Liefert den Test Kollektionsnamen zurück
//         *return  : - 
//******************************************************************************************************************************
void CollectionTest_NewTestCollectionEntryInDatabase(char *pName)
{
	int CountSelectedTests;
	char TestCollectionDescription[TESTCOLLECTION_DESCRIPTION_LENGTH] = "";
	
	//Name der Test Kollektion
	GetCtrlVal(TCpanel, TCPANEL_TB_NAME_TESTCOL, pName);
	if(strlen(pName) == 0) MessagePopup("Save test collection", "Pleae define a test collection name");
	else
	{
		//Beschreibung der Test Kollektion
		GetCtrlVal(TCpanel, TCPANEL_TB_DESC_TESTCOL, TestCollectionDescription);
	
		//Selektionierte Files in die Verlinkungstabelle eintragen (TestCollection_TestInformation)
		GetNumListItems(STpanel, STPANEL_TREE_TESTCOLLECTION, &CountSelectedTests);
		if(CountSelectedTests == 0) MessagePopup("Save test collection", "No test in test collection view");
		else
		{
			//Neue Test Kollektion in der Datenbank erstellen
			Database_WriteTestCollectionTable(pName, TestCollectionDescription);
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert die Test Kollektion Informationen zu dem selektioniertem Eintrag im Test Collection Fenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
SINT32 CollectionTest_GetTestCollectionInformationFromSelectedEntry(char *pName, char *pDescription)
{
	SINT32 TestCollectionId = -1;
	int NumberSelectedFile;
	char StringReplace[DATE_TYPE_LENGTH+1] = "";
	
	//Test Collection Name von der Oberfläche auslesen
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_TC, &NumberSelectedFile);		
	GetValueFromIndex(STpanel, STPANEL_LISTBOX_TC, NumberSelectedFile, pName);
	
	//verhindert Absturz, wenn keine Test Kollektion definiert sind
	if(NumberSelectedFile >= 0)
	{
		//Wenn alle Test Kollektion ausgewählt ist, so soll das Datum und der Active String noch aus dem Namen entfernt werden
		if(IsShowAllTestCollectionSelected == TRUE && pTestCollections != NULL)
		{
			sprintf(StringReplace, " %s", pTestCollections[NumberSelectedFile].CreationDate);
			HelpFunctions_StringReplace(StringReplace, "", pName);
			if(pTestCollections[NumberSelectedFile].ActivityState == ACTIVITY_STATE_NOT_ACTIVE)	HelpFunctions_StringReplace(" Not Active", "", pName);
			else																																								HelpFunctions_StringReplace(" Active", "", pName);
		}
		TestCollectionId = pTestCollections[NumberSelectedFile].TestCollectionId;
		
		//Nun soll die Beschreibung der Test Collection aus der Datenbank geholt werden
		Database_GetTestCollectionTable(TestCollectionId, pName, pDescription);
	}
	
	return TestCollectionId;
}
---








