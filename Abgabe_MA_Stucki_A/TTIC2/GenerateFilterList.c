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
#include "SurfaceTest.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
void WriteSortListInTextBox(char **ppStringList, int StringListFiles);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
char *pModulName, *pInterfaceName, *pValveSerieName, *pControllerTypeName, *pOptionName, *pClusterName, *pStringList;
char **ppArrayModulList, **ppArrayInterfaceList, **ppArrayValveSerieList, **ppArrayControllerTypeList, **ppArrayOptionList, **ppArrayClusterList, **ppArrayNIHardwareList;

UINT16 CountModuls, CountInterfaces, CountValveSeries, CountControllerTypes, CountOptions, CountNIHardwareTypes;
struct testModul_type *pModulList;
struct testInterface_type *pInterfaceList;
struct testValveSerie_type *pValveSerieList;
struct testControllerType_type *pControllerTypeList;
struct testOption_type *pOptionList;
struct testNIHardwareType_type *pNIHardwareTypeList;
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Füllt alle Filter Lists mit den Werten aus der Datenbank 
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_FillFilterLists(void)
{
	//Modul
	//Zuerst die Anzahlt der Moduls heraus lesen
	CountModuls = Database_GetCountModul();
	pModulList = malloc(sizeof(struct testModul_type) * CountModuls);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteModulStructure(pModulList);
	
	//Interface
	CountInterfaces = Database_GetCountInterface();
	pInterfaceList = malloc(sizeof(struct testInterface_type) * CountInterfaces);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteInterfaceStructure(pInterfaceList);
	
	//ValveSerie
	CountValveSeries = Database_GetCountValveSerie();
	pValveSerieList = malloc(sizeof(struct testValveSerie_type) * CountValveSeries);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteValveSerieStructure(pValveSerieList);
	
	//ControllerType
	CountControllerTypes = Database_GetCountControllerType();
	pControllerTypeList = malloc(sizeof(struct testControllerType_type) * CountControllerTypes);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteControllerTypeStructure(pControllerTypeList);
	
	//Option
	CountOptions = Database_GetCountOption();
	pOptionList = malloc(sizeof(struct testInterface_type) * CountOptions);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteOptionStructure(pOptionList);
	
	//NIHardwareType
	CountNIHardwareTypes = Database_GetCountNIHardwareType();
	pNIHardwareTypeList = malloc(sizeof(struct testNIHardwareType_type) * CountNIHardwareTypes);
	//Nun sollen die Werte in die Modul Liste eingetragen werden
	Database_WriteNIHardwareTypeStructure(pNIHardwareTypeList);
}
---
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt alle Modul Types in das Selektionsauswahlfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteModulFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountModuls; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pModulList[i].ModulName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt alle gefundenen Interfaces(aus der Info Funktion der jeweiligen Tests) in das InterfaceCollection Textfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteInterfaceFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountInterfaces; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pInterfaceList[i].InterfaceName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt alle gefundenen Valve Series(aus der Info Funktion der jeweiligen Tests) in das ValveSeriesCollection Textfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteValveSerieFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountValveSeries; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pValveSerieList[i].ValveSerieName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt alle gefundenen Controller Type(aus der Info Funktion der jeweiligen Tests) in das ControllerTypeCollection Textfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteControllerTypeFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountControllerTypes; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pControllerTypeList[i].ControllerTypeName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt alle gefundenen Option(aus der Info Funktion der jeweiligen Tests) in das OptionCollection Textfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteOptionFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountOptions; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pOptionList[i].OptionName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : schreibt alle gefundenen NI Hardware(aus der Info Funktion der jeweiligen Tests) in das NIHardwareCollection Textfeld
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
void GenerateFilterList_WriteNIHardwareFilterInTextBox(void)
{
	UINT16 i;
	
	//Löscht das Info Feld
	//o: fürd den Startindex, -1: alle Elemente werden gelöscht
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Löscht das Selektionsfeld
	DeleteListItem(STpanel, STPANEL_LISTBOX_SELECTION, 0, -1);
	
	//Listet die einzelnen Module auf
	for(i = 0; i < CountNIHardwareTypes; i++)	InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, pNIHardwareTypeList[i].NIHardwareTypeName, 0);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Sortiert die Liste nach dem ABC und schreibt sie ins Selection Feld
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void WriteSortListInTextBox(char **ppStringList, int StringListFiles)
{
	//Abbruch Bedingung
	if(StringListFiles == 0)	return;	
	
	char TempString[STRING_CHAR], StringList[StringListFiles][STRING_CHAR], *pFoundDelimiterChar, DelimiterChar = 0x3B;
	int i,j, StrlenElementList, StrlenMallocList = 100, StrlenListForDelimiterChar;
	
	for(i = 0; i < StringListFiles; i++)
	{
		strcpy(StringList[i], ppStringList[i]);
	}
	
	
	for (i = 0; i < StringListFiles - 1 ; i++)
	{
		for (j = i + 1; j < StringListFiles; j++)
		{
 			if (strcmp(StringList[i], StringList[j]) > 0)
			{
				strcpy(TempString, StringList[i]);
				strcpy(StringList[i], StringList[j]);
				strcpy(StringList[j], TempString);
			}
		}
	}
	//sortierte Liste in Selection Feld schreiben
	for(i = 0; i < StringListFiles; i++)
	{
		InsertListItem (STpanel, STPANEL_LISTBOX_SELECTION, -1, StringList[i], 0);
	}
	
	pStringList = (char *) malloc(sizeof(char) * STRING_CHAR);
	pStringList[0] = 0;
	
	for(i = 0; i < StringListFiles; i++)
	{
		StrlenElementList = strlen(pStringList);
		while(StrlenMallocList - StrlenElementList < 90)
		{
			StrlenMallocList = StrlenMallocList + STRING_CHAR;
			pStringList = realloc(pStringList, StrlenMallocList);
		}
		strcat(pStringList, StringList[i]);
		StrlenListForDelimiterChar = strlen(pStringList);
		if(i != (StringListFiles - 1))	strcat((pStringList + StrlenListForDelimiterChar), ";");
	}
	
	//um die einzelnen List Einträge zu separrieren
	for(i = 0; i < StringListFiles; i++)
	{
		if (i == 0)	ppStringList[0] = pStringList;
		//wenn kein ; vorliegt, so ist kein Option Eintrag vorhanden
		else
		{
			//Nach ; Zeichen suchen
			pFoundDelimiterChar = strchr(ppStringList[i - 1], DelimiterChar);
			*pFoundDelimiterChar = 0;
			while(pFoundDelimiterChar != NULL)
			{
				ppStringList[i] = pFoundDelimiterChar + 1; 

				//Nach ; Zeichen suchen
				pFoundDelimiterChar = strchr(pStringList, DelimiterChar);
			}
		}
	}
}





