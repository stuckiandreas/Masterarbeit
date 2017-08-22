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
#include "SelectFilesFromFilter.h"
#include "SelectListbox.h"
#include "SurfaceTest.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
+++
//eigene Funktionen
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der Modul Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) markiert
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromModulFilterInformation(struct test_type *pTests)
{
	int NumberSelectedModul;
	UINT16 k = 0,i;
	char CompareString[MODUL_NAME_LENGTH], AllModulList[MODUL_NAME_LENGTH];
	CompareString[0] = 0;
	//löscht die Einträge in der Infosequenz
	DeleteListItem(STpanel, STPANEL_TREE_INFO, 0, -1);
	//löscht die Einträge in der Auswahl der Tests
	DeleteListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, 0, -1);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedModul);
	strcat(CompareString, pModulList[NumberSelectedModul].ModulName);
	strcpy(AllModulList, "All");
	//browse the structur about the same modulname, if yes than notice the exe file
	for(i = 0; i < EntriesFiles; i++)
	{
		//Nur sichtbare Test filtern
		if(pAvaiableTests[i].Visible == 1)
		{
			pTests[i].SelectTestAvaiable = 0;
			//if the structelement has the same modulname
			if(strcmp(CompareString, Database_GetModulName(pTests[i].TestVersionId)) == 0)
			{
				InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
				//in selected Test Liste die zum gewählten Modul gehören selected auf 1 setzen
				pAvaiableTests[i].Visible = 1;
				SetTreeItemAttribute (STpanel, STPANEL_TREE_AVAIABLETESTS, k, ATTR_SELECTED, 0);
				InsertListItem(STpanel, STPANEL_TREE_INFO, -1, pTests[i].TestName, 0);
				k++;
			}
			else	pAvaiableTests[i].Visible = 0;
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der Interface Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) aufgelistet
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromInterfaceFilterInformation(struct test_type *pTests)
{
	SINT32 InterfaceTypesId = -1, InterfaceTypesAllId;
	int NumberSelectedInterface, i, k = 0;
	char SelectedInterface[INTERFACE_NAME_LENGTH];
	SelectedInterface[0] = 0;
	//deselect all Files first
	DeleteListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, 0, -1);
	//removed the information in the info listbox
	DeleteListItem(STpanel, STPANEL_TREE_INFO, 0, -1);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedInterface);
	//nur wenn auch Interfaces in der Liste stehen
	if(NumberSelectedInterface != -1)	
	{
		strcat(SelectedInterface, pInterfaceList[NumberSelectedInterface].InterfaceName);
		InterfaceTypesId = Database_GetInterfaceTypeIdWithEnum(SelectedInterface);	 
		InterfaceTypesAllId = Database_GetInterfaceTypeIdWithEnum("All");
		
		//browse the structur about the same modulname, if yes than notice the exe file
		for(i = 0; i < EntriesFiles; i++)
		{
			//Nur sichtbare Test filtern
			if(pAvaiableTests[i].Visible == 1)
			{
				pTests[i].SelectTestAvaiable = 0;
				//Vergleicht auf den selben Interface Namen
				if(Database_IsTestInterfaceCompatibleWithTestHardware(pTests[i].TestVersionId, InterfaceTypesId, InterfaceTypesAllId) == TRUE)		
				{
					InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
					ppArrayFileList[k] = pTests[i].TestName;
					//den Test anzeigen auf der auszuwählenden Liste
					pAvaiableTests[i].Visible = 1;
					k++;
				}
				else	pAvaiableTests[i].Visible = 0;
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der Valve Series Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) aufgelistet
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromValveSerieFilterInformation(struct test_type *pTests)
{
	UINT16 i, k = 0;
	SINT32 ValveSerieTypesId = -1, ValveSerieTypesAllId = -1;
	int NumberSelectedValveSerie;
	char SelectedValveSerie[VALVE_SERIE_NAME_LENGTH];
	SelectedValveSerie[0] = 0;
	//löscht alle aufgelisteten Test in der linken Liste
	ClearListCtrl(STpanel, STPANEL_TREE_AVAIABLETESTS);
	//löscht den Info Textfeld Inhalt
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedValveSerie);
	//nur wenn auch Valve Series in der Liste stehen
	if(NumberSelectedValveSerie != -1)	
	{
		strcat(SelectedValveSerie, pValveSerieList[NumberSelectedValveSerie].ValveSerieName);
		ValveSerieTypesId = Database_GetValveSerieIdWithEnum(SelectedValveSerie);	 
		ValveSerieTypesAllId = Database_GetValveSerieIdWithEnum("All");
		
		//browse the structur about the same modulname, if yes than notice the exe file
		for(i = 0; i < EntriesFiles; i++)
		{
			//Nur sichtbare Test filtern
			if(pAvaiableTests[i].Visible == 1)
			{
				pTests[i].SelectTestAvaiable = 0;
				//Vergleicht auf den selben ValveSerie Namen
				if(Database_IsTestValveSerieCompatibleWithTestHardware(pTests[i].TestVersionId, ValveSerieTypesId, ValveSerieTypesAllId) == TRUE)		
				{
					InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
					ppArrayFileList[k] = pTests[i].TestName;
					//den Test anzeigen auf der auszuwählenden Liste
					pAvaiableTests[i].Visible = 1;
					k++;
				}
				else	pAvaiableTests[i].Visible = 0;
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der Controller Type Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) aufgelistet
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromControllerTypeFilterInformation(struct test_type *pTests)
{
	UINT16 i, k = 0;
	SINT32 ControllerTypesId = -1, ControllerTypesAllId = -1;
	int NumberSelectedController;
	char SelectedController[CONTROLLER_TYPE_NAME_LENGTH];
	SelectedController[0] = 0;
	//löscht alle aufgelisteten Test in der linken Liste
	ClearListCtrl(STpanel, STPANEL_TREE_AVAIABLETESTS);
	//löscht den Info Textfeld Inhalt
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedController);
	//nur wenn auch Controller in der Liste stehen
	if(NumberSelectedController != -1)	
	{
		strcat(SelectedController, pControllerTypeList[NumberSelectedController].ControllerTypeName);
		ControllerTypesId = Database_GetControllerTypeIdWithEnum(SelectedController);	 
		ControllerTypesAllId = Database_GetControllerTypeIdWithEnum("All");
		
		//browse the structur about the same modulname, if yes than notice the exe file
		for(i = 0; i < EntriesFiles; i++)
		{
			//Nur sichtbare Test filtern
			if(pAvaiableTests[i].Visible == 1)
			{
				pTests[i].SelectTestAvaiable = 0;
				//Vergleicht auf den selben Controller Namen
				if(Database_IsTestControllerCompatibleWithTestHardware(pTests[i].TestVersionId, ControllerTypesId, ControllerTypesAllId) == TRUE)		
				{
					InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
					ppArrayFileList[k] = pTests[i].TestName;
					//den Test anzeigen auf der auszuwählenden Liste
					pAvaiableTests[i].Visible = 1;
					k++;
				}
				else	pAvaiableTests[i].Visible = 0;
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der Option Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) aufgelistet
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromOptionFilterInformation(struct test_type *pTests)
{
	UINT16 i, k = 0;
	SINT32 OptionTypesId = -1, OptionTypesAllId = -1;
	int NumberSelectedOption;
	char SelectedOption[OPTION_NAME_LENGTH];
	SelectedOption[0] = 0;
	//löscht alle aufgelisteten Test in der linken Liste
	ClearListCtrl(STpanel, STPANEL_TREE_AVAIABLETESTS);
	//löscht den Info Textfeld Inhalt
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedOption);
	//nur wenn auch Option in der Liste stehen
	if(NumberSelectedOption != -1)	
	{
		strcat(SelectedOption, pOptionList[NumberSelectedOption].OptionName);
		OptionTypesId = Database_GetSingleOptionTypeIdWithEnum(SelectedOption);	 
		OptionTypesAllId = Database_GetSingleOptionTypeIdWithEnum("All");
		
		//browse the structur about the same modulname, if yes than notice the exe file
		for(i = 0; i < EntriesFiles; i++)
		{
			//Nur sichtbare Test filtern
			if(pAvaiableTests[i].Visible == 1)
			{
				pTests[i].SelectTestAvaiable = 0;
				//Vergleicht auf den selben Option Namen
				if(Database_IsTestOptionCompatibleWithTestHardware(pTests[i].TestVersionId, OptionTypesId, OptionTypesAllId) == TRUE)		
				{
					InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
					ppArrayFileList[k] = pTests[i].TestName;
					//den Test anzeigen auf der auszuwählenden Liste
					pAvaiableTests[i].Visible = 1;
					k++;
				}
				else	pAvaiableTests[i].Visible = 0;
			}
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : die aufgelisteten Tests, welche in der NI Hardware Kollektion definiert worden sind, 
//										werden in der linken Liste (verfügbare Tests) aufgelistet
//  Input           : pTests: Übergabe der Struktur in welchem die Informationen der Tests steht
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void SelectFilesFromFilter_SelectFilesFromNIHardwareFilterInformation(struct test_type *pTests)
{
	UINT16 i, k = 0;
	SINT32 TestHardwareTypesId = -1, TestHardwareTypesAllId = -1;
	int NumberSelectedTestHardware;
	char SelectedTestHardware[OPTION_NAME_LENGTH];
	SelectedTestHardware[0] = 0;
	//löscht alle aufgelisteten Test in der linken Liste
	ClearListCtrl(STpanel, STPANEL_TREE_AVAIABLETESTS);
	//löscht den Info Textfeld Inhalt
	ClearListCtrl(STpanel, STPANEL_TREE_INFO);
	GetCtrlIndex(STpanel, STPANEL_LISTBOX_SELECTION, &NumberSelectedTestHardware);
	//nur wenn auch TestHardware in der Liste stehen
	if(NumberSelectedTestHardware != -1)	
	{
		strcat(SelectedTestHardware, pNIHardwareTypeList[NumberSelectedTestHardware].NIHardwareTypeName);
		TestHardwareTypesId = Database_GetNIHardwareTypeIdWithEnum(SelectedTestHardware);	 
		TestHardwareTypesAllId = Database_GetNIHardwareTypeIdWithEnum("All");
		
		//browse the structur about the same modulname, if yes than notice the exe file
		for(i = 0; i < EntriesFiles; i++)
		{
			//Nur sichtbare Test filtern
			if(pAvaiableTests[i].Visible == 1)
			{
				pTests[i].SelectTestAvaiable = 0;
				//Vergleicht auf den selben TestHardware Namen
				if(Database_IsTestNIHardwareCompatibleWithTestHardware(pTests[i].TestVersionId, TestHardwareTypesId, TestHardwareTypesAllId) == TRUE)		
				{
					InsertListItem(STpanel, STPANEL_TREE_AVAIABLETESTS, k, pTests[i].TestName, 0);
					ppArrayFileList[k] = pTests[i].TestName;
					//den Test anzeigen auf der auszuwählenden Liste
					pAvaiableTests[i].Visible = 1;
					k++;
				}
				else	pAvaiableTests[i].Visible = 0;
			}
		}
	}
}
---



