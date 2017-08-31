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
#include <NIDAQmx.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <utility.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <toolbox.h>
#undef PI;

#include "ChangeHardware.h"
#include "Database.h"
#include "DefineEnums.h"
#include "Definelist.h"
#include "DefineStructFileService.h"
#include "HelpFunctions.h"
#include "InterfaceCommunication.h"
#include "ParameterProcessing.h"
#include "PC_Defines.h"
#include "PC_IC2H1_HelpFunctions.h"
#include "PC_IC2H2_HelpFunctions.h"
#include "PlatformConfig.h"
#include "UsbLabView.h" 

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
SINT8 CheckOptionValve(SINT16 OptionValue);
SINT16 ReadTestHardware(void);
char* ReadOptionHardware(UINT16 OptionType);
UINT8 RequestAvaiableInterfaceCommunicationTestSequence(void);
UINT16 RequestDiffrentHardwareForTestSequence(void);
static void WriteDiffrentHardwareOnChangeHardwarePanel(void);
void WriteNotAvaiableInterfaceCommunicationForTestSequence(void);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
BOOL8 LoadChangeHardware = FALSE, ChangeHardwarePanelCloseStatus = FALSE, LedStateChangeHardware = FALSE, ResultNeedfulHardware[DIFFERENT_HARDWARE_CRITERIA];
UINT8 CheckedInterfaceCommunication = 0;
int CHpanel = 0, ThreadFunctionId3 = 0, ThreadFunctionId4 = 0, ThreadFunctionId5 = 0;
static int CVICALLBACK ThreadFunction3 (void *functionData), CVICALLBACK ThreadFunction4 (void *functionData), CVICALLBACK ThreadFunction5 (void *functionData);
UINT8 Error, UserPushButton = HARDWARE_OK, InterfaceDiffrentCommunicationEntries = 0, ResultAvaiableInterfaceCommunication = 0;
char ValveSeriesString[10], ControllerTypeString[10], InterfaceTypeString[10], OptionTypeString[10], DiffrentInterfaceCommunicationList[INTERFACE_DIFFRENT_COMMUNICATION][INTERFACENAMES_LENGTH], CheckedInterfaceCommunicationList[INTERFACE_DIFFRENT_COMMUNICATION][INTERFACENAMES_LENGTH], TestHardware[TESTHARDWARE_LENGTH];;

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liest die Hardware Informationen ein und Speichert Sie in einer Struktur ab 
//  Input           : 
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
void ChangeHardware_SaveHardwareInformationVentil(void)
{
	SINT16 ValveSeries;
	UINT16 ControllerType, InterfaceType, OptionType;
	
	//alloziert eine Struktur vom Typ test_type, inwelchem die Indormationen des Ventil abgespeichert wird
	pHardwareVentil = malloc(sizeof(struct testHardware_type) * 1);
	//Ventil Enum Wert abspeichern
	ParameterProcessing_SendParameter(Parameter.System.Identification.Configuration.ValveSeries.ID, 0, &ValveSeries, Parameter.System.Identification.Configuration.ValveSeries.DataTyp, PARAMETER_GET);
	pHardwareVentil->TestValve = ValveSeries;
	//Controller Enum Wert abspeichern
	ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.ControllerType.ID, 0, &ControllerType, Parameter.System.Identification.Hardware.ControllerType.DataTyp, PARAMETER_GET);
	pHardwareVentil->TestController = (SINT16)ControllerType;
	//Interface Enum Wert abspeichern
	ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.InterfaceType.ID, 0, &InterfaceType, Parameter.System.Identification.Hardware.InterfaceType.DataTyp, PARAMETER_GET);
	pHardwareVentil->TestInterface = (SINT16)InterfaceType;
	//Optionen Enum Wert abspeichern
	ParameterProcessing_SendParameter(Parameter.System.Identification.Hardware.OptionType.ID, 0, &OptionType, Parameter.System.Identification.Hardware.OptionType.DataTyp, PARAMETER_GET);
	pHardwareVentil->TestOption = (SINT16)OptionType;
	//pHardwareVentil->pTestOption = ReadOptionHardware(OptionType);
	//TestHardware abspeichern (Funktion liefert ein Zeichenzeiger zurück
	pHardwareVentil->TestNIHardware = ReadTestHardware();
	
	//Den Port schliessen 
	HelpFunctions_CloseUSBCommunication();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beurteilt ob der Test mit der angeschlossenen Hardware ausgeführt werden kann 
//  Input           : pSelectedTests:		Tests Struktur, in welchem die auszuführeneden Tests angegeben sind
//									:	IndexSelectTest:	Gibt den Index bekannt vom auszuführenden Test in der Liste
//  Output *Pointer : 
//         *return  : Gibt das Resultat zurück, ob die angeschlossene Hardware ausreicht 
//******************************************************************************************************************************
BOOL8 ChangeHardware_NeedfulHardwareForTest(struct testSelected_type *pSelectedTests, int IndexSelectTest)
{
	BOOL8 NeedDiffrentHardware = FALSE;
	UINT8 i;
	
	//Überprüfen was der Test für ein Ventil benötigt
+++
	//Überprüfung ob das Ventil überein stimmt
	if(Database_HasTestHardwareValveSerieNeededFromTest(pSelectedTests[IndexSelectTest].pTests->TestVersionId, pHardwareVentil->TestValve) == FALSE)	ResultNeedfulHardware[DHC_VALVE_SERIE] = TRUE;
	else																																																																							ResultNeedfulHardware[DHC_VALVE_SERIE] = FALSE;
	
	//Überprüfung ob der benötigte Controller vorhanden ist
	if(Database_HasTestHardwareControllerNeededFromTest(pSelectedTests[IndexSelectTest].pTests->TestVersionId, pHardwareVentil->TestController) == FALSE)	ResultNeedfulHardware[DHC_CONTROLLER_TYPE] = TRUE;
	else																																																																									ResultNeedfulHardware[DHC_CONTROLLER_TYPE] = FALSE;
	
	//Überprüfung ob das benötigte Interface vorhanden ist
	if(Database_HasTestHardwareInterfaceNeededFromTest(pSelectedTests[IndexSelectTest].pTests->TestVersionId, pHardwareVentil->TestInterface) == FALSE) 	ResultNeedfulHardware[DHC_INTERFACE] = TRUE;
	else																																																																									ResultNeedfulHardware[DHC_INTERFACE] = FALSE;

	//Überprüfung ob die benötigten Optionen vorhanden sind
	if(Database_HasTestHardwareOptionNeededFromTest(pSelectedTests[IndexSelectTest].pTests->TestVersionId, pHardwareVentil->TestOption) == FALSE) 	ResultNeedfulHardware[DHC_OPTION] = TRUE;
	else																																																																						ResultNeedfulHardware[DHC_OPTION] = FALSE;
	
	//Überprüfung ob die benötigte NI Hardware vorhanden sind
	if(Database_IsNIHardwareAvaiableFromTest(pSelectedTests[IndexSelectTest].pTests->TestVersionId, pHardwareVentil->TestNIHardware) == FALSE)	ResultNeedfulHardware[DHC_TEST_HARDWARE] = TRUE;
	else																																																																				ResultNeedfulHardware[DHC_TEST_HARDWARE] = FALSE;
---	
	for(i = 0; i < DIFFERENT_HARDWARE_CRITERIA; i++)
	{
		if(ResultNeedfulHardware[i] == TRUE)	
		{
			NeedDiffrentHardware = TRUE;
			break;
		}
	}
	
	return NeedDiffrentHardware;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beurteilt ob der Test mit der angeschlossenen Hardware ausgeführt werden kann 
//  Input           : pSelectedTests:		Tests Struktur, in welchem die auszuführeneden Tests angegeben sind
//									:	IndexSelectTest:	Gibt den Index bekannt vom auszuführenden Test in der Liste
//  Output *Pointer : 
//         *return  : Gibt das Resultat zurück, ob die angeschlossene Hardware ausreicht 
//******************************************************************************************************************************
BOOL8 ChangeHardware_IsTestExecutableCauseHardware(SINT32 TestVersionId)
{
	static UINT8 Access = 0;
	
	//Zuerst muss noch die Test Hardware ausgelesen werden
	if(Access == 0)
	{
		ChangeHardware_SaveHardwareInformationVentil();
		Access++;
	}
	
	//Überprüfen was der Test für ein Ventil benötigt
+++	
	//Überprüfung ob das Ventil überein stimmt
	if(Database_HasTestHardwareValveSerieNeededFromTest(TestVersionId, pHardwareVentil->TestValve) != TRUE)				return FALSE;	
	
	//Überprüfung ob der benötigte Controller vorhanden ist
	if(Database_HasTestHardwareControllerNeededFromTest(TestVersionId, pHardwareVentil->TestController) != TRUE)	return FALSE;
	
	//Überprüfung ob das benötigte Interface vorhanden ist
	if(Database_HasTestHardwareInterfaceNeededFromTest(TestVersionId, pHardwareVentil->TestInterface) != TRUE) 		return FALSE;

	//Überprüfung ob die benötigten Optionen vorhanden sind
	if(Database_HasTestHardwareOptionNeededFromTest(TestVersionId, pHardwareVentil->TestOption) != TRUE) 					return FALSE;	
	
	//Überprüfung ob die benötigte NI Hardware vorhanden sind
	if(Database_IsNIHardwareAvaiableFromTest(TestVersionId, pHardwareVentil->TestNIHardware) != TRUE)							return FALSE;
---	
	return TRUE;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beurteilt ob mit dem benötigten Interface kommuniziert werden kann
//  Input           : pSelectedTests:		Tests Struktur, in welchem die auszuführeneden Tests angegeben sind
//									:	IndexSelectTest:	Gibt den Index bekannt vom auszuführenden Test in der Liste
//  Output *Pointer : 
//         *return  : Gibt das Resultat zurück, ob das Interface kommunikationsfähig ist
//******************************************************************************************************************************
UINT8	ChangeHardware_AvaiableInterfaceCommunication(struct testSelected_type *pSelectedTests, int IndexSelectTest)
{
	BOOL8 InterfaceCommunicationChecked = FALSE;
	BOOL8 DeleteListEntry = FALSE;
	UINT8 InterfaceDiffrentCommunicationIndex, InterfaceCommunicationIndex;
	char *pTestInterfaceEnum = NULL;
	
	//Nur überprüfen bei Interfaces, welches eine Kommunikationsschnittstelle verfügen
	pTestInterfaceEnum = Database_GetInterfaceTestHardware(pSelectedTests[IndexSelectTest].pTests->TestVersionId);
	if(strcmp(pTestInterfaceEnum, "RS232/RS485") == 0 || strcmp(pTestInterfaceEnum, "EtherCAT") == 0 || strcmp(pTestInterfaceEnum, "DeviceNet") == 0)
	{
		//zuerst schauen, ob die Kommunikation noch nicht überprüft wurde
		for(InterfaceCommunicationIndex = 0; InterfaceCommunicationIndex < CheckedInterfaceCommunication; InterfaceCommunicationIndex++)
		{
			//bereits überprüft
			if(strcmp(CheckedInterfaceCommunicationList[InterfaceCommunicationIndex], pTestInterfaceEnum) == 0) InterfaceCommunicationChecked = TRUE;
		}
		if(InterfaceCommunicationChecked == FALSE)
		{
			//noch keine überprüfung
			ResultAvaiableInterfaceCommunication =  InterfaceCommunication_CheckInterfaceCommunication(pTestInterfaceEnum);
			strcpy(CheckedInterfaceCommunicationList[CheckedInterfaceCommunication], pTestInterfaceEnum);
			CheckedInterfaceCommunication++;
		}
		
		//wenn kein Fehler vorliegt, darf das Interface nicht in der DiffrentInterfaceCommunicationList stehen
		if(ResultAvaiableInterfaceCommunication == ERRORLESS)
		{
			for(InterfaceDiffrentCommunicationIndex = 0; InterfaceDiffrentCommunicationIndex < InterfaceDiffrentCommunicationEntries; InterfaceDiffrentCommunicationIndex++)
			{
				if(strcmp(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex], pTestInterfaceEnum) == 0)	
				{
					DeleteListEntry = TRUE;
					break;
				}
			}
			if(DeleteListEntry == TRUE)
			{
				//löscht den Eintrag
				DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex][0] = 0;
				//muss Liste nachtragen
				for(InterfaceDiffrentCommunicationIndex = 0; InterfaceDiffrentCommunicationIndex < InterfaceDiffrentCommunicationEntries; InterfaceDiffrentCommunicationIndex++)
				{
					if(strlen(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex]) == 0)
					{
						//wenn es nicht der letzte Eintrag in der Liste ist, soll der nächste in das leere Feld geschrieben werden
						if(InterfaceDiffrentCommunicationIndex + 1 < InterfaceDiffrentCommunicationEntries)
						{
							strcpy(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex], DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex + 1]);
							DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex + 1][0] = 0;
						}
						else	InterfaceDiffrentCommunicationEntries--;
					}
				}
			}
		}
		//Nur überprüfung durchführen, wenn die Interface Kommunikationsüberprüfung noch nicht durchgeführt wurde
		for(InterfaceDiffrentCommunicationIndex = 0; InterfaceDiffrentCommunicationIndex < InterfaceDiffrentCommunicationEntries; InterfaceDiffrentCommunicationIndex++)
		{
			if(strcmp(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationIndex], pTestInterfaceEnum) == 0)
			{
				if(strcmp(pTestInterfaceEnum, "RS232/RS485") == 0) 		return	FAIL_RS232_COMMUNICATION;
				else if(strcmp(pTestInterfaceEnum, "DeviceNet") == 0)	return	FAIL_DEVICENET_COMMUNICATION;
				else																									return	FAIL_ETHERCAT_COMMUNICATION;
			}
		}
		if(ResultAvaiableInterfaceCommunication == FAIL_RS232_COMMUNICATION)	
		{
			strcpy(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationEntries], "RS232/RS485");
			InterfaceDiffrentCommunicationEntries++;
		}
		else if(ResultAvaiableInterfaceCommunication == FAIL_DEVICENET_COMMUNICATION)
		{
			strcpy(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationEntries], "DeviceNet");
			InterfaceDiffrentCommunicationEntries++;
		}
		else if(ResultAvaiableInterfaceCommunication == FAIL_ETHERCAT_COMMUNICATION)
		{
			strcpy(DiffrentInterfaceCommunicationList[InterfaceDiffrentCommunicationEntries], "EtherCAT");
			InterfaceDiffrentCommunicationEntries++;
		}
	} 
	return ResultAvaiableInterfaceCommunication;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den User bescheid, was der Test für eine Hardware braucht(nur der Aspekt welcher unterschiedlich ist)  
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void ChangeHardware_RequestUserHardwareChange(void)
{
	UserPushButton = USER_PUSH_NO_BUTTON;
+++	
	//ValveSeries
	if(ResultNeedfulHardware[DHC_VALVE_SERIE] == TRUE)
	{
			SetCtrlAttribute(CHpanel, CHPANEL_TEXTBOX_CH_DH, ATTR_LABEL_TEXT, "Needful Valve");
			SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_DH, Database_GetValveSerieTestHardware(pSelectedTests[IndexTestList].pTests->TestVersionId));
	}
	//Controller
	if(ResultNeedfulHardware[DHC_CONTROLLER_TYPE] == TRUE)
	{
		SetCtrlAttribute(CHpanel, CHPANEL_TEXTBOX_CH_DH, ATTR_LABEL_TEXT, "Needful Controller");
		SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_DH, Database_GetControllerTypeTestHardware(pSelectedTests[IndexTestList].pTests->TestVersionId));
	}
	//Interface
	if(ResultNeedfulHardware[DHC_INTERFACE] == TRUE)
	{
		SetCtrlAttribute(CHpanel, CHPANEL_TEXTBOX_CH_DH, ATTR_LABEL_TEXT, "Needful Interface");
		SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_DH, Database_GetInterfaceTestHardware(pSelectedTests[IndexTestList].pTests->TestVersionId));
	}
	//Option
	if(ResultNeedfulHardware[DHC_OPTION] == TRUE)
	{
		SetCtrlAttribute(CHpanel, CHPANEL_TEXTBOX_CH_DH, ATTR_LABEL_TEXT, "Needful Option");
		SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_DH, Database_GetOptionTypeTestHardware(pSelectedTests[IndexTestList].pTests->TestVersionId));
	}
	//NI Hardware
	if(ResultNeedfulHardware[DHC_TEST_HARDWARE] == TRUE)
	{
		SetCtrlAttribute(CHpanel, CHPANEL_TEXTBOX_CH_DH, ATTR_LABEL_TEXT, "NI Hardware required");
		SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_DH, Database_GetTestHardware(pSelectedTests[IndexTestList].pTests->TestVersionId));
	}
---
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt den User bescheid, was der Test für eine Hardware braucht(nur der Aspekt welcher unterschiedlich ist)  
//  Input           : ResultAvaiableInterfaceCommunication:	gibt das betroffene Interface an, mit welchem nicht Komuniziert werden kann
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void ChangeHardware_RequestUserLookInterfaceCommunication(void)
{
	switch(ResultAvaiableInterfaceCommunication)
	{
		case 1:	SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_RI, "RS232");			break;
		case 2:	SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_RI, "DeviceNet");	break;
		case 3:	SetCtrlVal(CHpanel, CHPANEL_TEXTBOX_CH_RI, "EtherCAT");		break;
		default:																													break;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Setzt den Status des auszuführenden Tests auf 0  
//  Input           : 
//  Output *Pointer : 
//         *return  : 
//******************************************************************************************************************************
void ChangeHardware_SetExecutableToZero(void)
{
	int i;
	for(i = 0; i < CountSelectedFiles; i++)
	{
		pSelectedTests[i].Executed = 0;
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Test, welche andere Hardware benötigen auf der Oberfläche aus  
//  Input           : 
//  Output *Pointer : 
//         *return  : 
//******************************************************************************************************************************
void WriteDiffrentHardwareForTestSequence(void)
{
	BOOL8 NeedDiffrentHardware = FALSE;
	UINT16 IndexTestList = 0, IndexTestWithDiffrentHardware = 0;
	
	//Ventil Hardware auslesen 
	ChangeHardware_SaveHardwareInformationVentil();
	
	//Zuerst Textfeld leeren
	DeleteTextBoxLines (CHpanel, CHPANEL_TEXTBOX_CH_DH, 0, -1);
	
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
	{
		NeedDiffrentHardware = ChangeHardware_NeedfulHardwareForTest(pSelectedTests, IndexTestList);
		if(NeedDiffrentHardware == TRUE)	
		{
			InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_DH, IndexTestWithDiffrentHardware, pSelectedTests[IndexTestList].pTests->TestName);
			IndexTestWithDiffrentHardware++;
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Interfaces aus, auf welchem die Kommunikation fehl schlägt 
//  Input           : 
//  Output *Pointer : 
//         *return  : 
//******************************************************************************************************************************
void WriteNotAvaiableInterfaceCommunicationForTestSequence(void)
{
	UINT8 ResultAvaiableInterfaceCommunication = 0;
	UINT16 IndexTestList = 0, IndexNotAvaiableInterfaceCommunication;
	
	//Auswertung
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)	
	{
		ResultAvaiableInterfaceCommunication = ChangeHardware_AvaiableInterfaceCommunication(pSelectedTests, IndexTestList);
		IsInterfaceCommunicationAvaiable = ResultAvaiableInterfaceCommunication;
	}
	//Zuerst Ausgabefenster leeren
	DeleteTextBoxLines (CHpanel, CHPANEL_TEXTBOX_CH_RI, 0, -1);
	
	//Ausgabe auf dem ChangeHardware Fenster
	for(IndexNotAvaiableInterfaceCommunication = 0; IndexNotAvaiableInterfaceCommunication < InterfaceDiffrentCommunicationEntries; IndexNotAvaiableInterfaceCommunication++)	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_RI, IndexNotAvaiableInterfaceCommunication, DiffrentInterfaceCommunicationList[IndexNotAvaiableInterfaceCommunication]);
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Gibt die Anzahl Test an, welche unterschiedliche Hardware Anforderungen aufweisen  
//  Input           : 
//  Output *Pointer : 
//         *return  : Anzahl Test mit unterschiedlicher Hardware wie die aktuelle Testhardware
//******************************************************************************************************************************
UINT16 RequestDiffrentHardwareForTestSequence(void)
{
	BOOL8 NeedDiffrentHardware = FALSE;
	UINT16 IndexTestList = 0, IndexTestWithDiffrentHardware = 0;
	
	//Ventil Hardware auslesen 
	ChangeHardware_SaveHardwareInformationVentil(); 
	
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
	{
		NeedDiffrentHardware = ChangeHardware_NeedfulHardwareForTest(pSelectedTests, IndexTestList);
		if(NeedDiffrentHardware != 0)	IndexTestWithDiffrentHardware++;
	}
	return IndexTestWithDiffrentHardware;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wird überprft, ob eine Kommunikation auf dem jeweiligen Interface möglich ist
//  Input           :
//  Output *Pointer : 
//         *return  : Anzahl Interfaces, auf welchem die Kommunikation fail schlägt
//******************************************************************************************************************************
UINT8 RequestAvaiableInterfaceCommunicationTestSequence(void)
{
	UINT8 IndexTestList = 0, ResultAvaiableInterfaceCommunication = 0, IndexInterfaceWithNotAvaiableCommunication = 0;
	
	for(IndexTestList = 0; IndexTestList < CountSelectedFiles; IndexTestList++)
	{
		//Überprüft, ob die Interface Kommunikation möglich ist
		ResultAvaiableInterfaceCommunication = ChangeHardware_AvaiableInterfaceCommunication(pSelectedTests, IndexTestList);
		if(ResultAvaiableInterfaceCommunication != 0)	IndexInterfaceWithNotAvaiableCommunication++;	
	}
	return IndexInterfaceWithNotAvaiableCommunication;
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liest die NI Hardware aus, welche Option mit der Hardware unterstützt werden kann  
//  Input           : 
//  Output *Pointer : 
//         *return  : String der OptionHardware Information
//******************************************************************************************************************************
char* ReadOptionHardware(UINT16 OptionType)
{		
	BOOL8 PowerSupplyAvaiable = FALSE;
	switch(OptionType)
	{
		case CH_PFO:
		case CH_SPS_PFO:
		case CH_PFO_CLUSTER:
		case CH_SPS_PFO_CLUSTER:
		case CH_SPFO:
		case CH_SPS_SPFO:
		case CH_SPFO_CLUSTER:
		case CH_SPS_SPFO_CLUSTER:
		//überprüfen, ob das Speisegerät angeschlossen ist
		PowerSupplyAvaiable = SupplyValve_IsPowerSupplyAvaiable();
		if(PowerSupplyAvaiable == TRUE) 
		{
			sprintf(OptionTypeString,"%d", OptionType);
			return OptionTypeString;
		}
		//wenn nicht, fällt die PFO Option weg
		else
		{
			switch(OptionType)
			{
				case CH_PFO:
				{
					sprintf(OptionTypeString,"%d", CH_NOT_AVAIABLE);
					return OptionTypeString;
				}
				case CH_SPS_PFO:
				{
					sprintf(OptionTypeString,"%d", CH_SPS);
					return OptionTypeString;
				}
				case CH_PFO_CLUSTER:
				{
					sprintf(OptionTypeString,"%d", CH_CLUSTER);
					return OptionTypeString;
				}
				case CH_SPS_PFO_CLUSTER:
				{
					sprintf(OptionTypeString,"%d", CH_SPS_CLUSTER);
					return OptionTypeString;
				}
				case CH_SPFO:
				{
					sprintf(OptionTypeString,"%d", CH_NOT_AVAIABLE);
					return OptionTypeString;
				}
				case CH_SPS_SPFO:
				{
					sprintf(OptionTypeString,"%d", CH_SPS);
					return OptionTypeString;
				}
				case CH_SPFO_CLUSTER:
				{
					sprintf(OptionTypeString,"%d", CH_CLUSTER);
					return OptionTypeString;
				}
				case CH_SPS_SPFO_CLUSTER:
				{
					sprintf(OptionTypeString,"%d", CH_SPS_CLUSTER);
					return OptionTypeString;
				}
			}
		}
		default: 
		{
			sprintf(OptionTypeString,"%d", OptionType);
			return OptionTypeString;
		}
	}
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liest die NI Hardware aus, welche Hardware verfügbar ist 
//										Index0: All, Index1: NI Power Connector, Index2: Interface RS232/RS485, Index3: Logic, 
//										Index4: DeviceNet, Index5: EtherCAT, Index6: NI Analog Sensor
//  Input           : 
//  Output *Pointer : 
//         *return  : Bitwert der TestHardware Information
//******************************************************************************************************************************
SINT16 ReadTestHardware(void)
{
	SINT16 NIHardwareValue = 0;
	
	//Power Connector
	//momentan wird überprüft, ob die Power Connection Option aktiv ist oder nicht
#if IC2H13_CONTROLLER && ((TEST_HARDWARE_CODE == 1) || (TEST_HARDWARE_CODE == 0))
	if(PC_IC2H1_HelpFunctions_PowerConnectorHardwareAvaiable() == TRUE)	NIHardwareValue = NIHardwareValue+2;
#endif
#if IC2H2_CONTROLLER && ((TEST_HARDWARE_CODE == 2) || (TEST_HARDWARE_CODE == 0))
	if(PC_IC2H2_HelpFunctions_PowerConnectorHardwareAvaiable() == TRUE)	NIHardwareValue = NIHardwareValue+2;
#endif
	
	//Interface RS232
#ifndef IC2H2_CONTROLLER && ((TEST_HARDWARE_CODE == 2) || (TEST_HARDWARE_CODE == 0))
	if(InterfaceCommunication_RS232HardwareAvaiable() == 0) NIHardwareValue = NIHardwareValue+4;
#endif	
	
	//Interface Logik
	if(InterfaceCommunication_LogicHardwareAvaiable() == 0)	NIHardwareValue = NIHardwareValue+8;
	
	//Interface DeviceNET
	if(InterfaceCommunication_DeviceNetHardwareAvaiable() == 0)	NIHardwareValue = NIHardwareValue+16;																												
		
	//Interface EtherCAT (hat jeder Computer)
	NIHardwareValue = NIHardwareValue+32;
	
	//Analog Sensor
	if(InterfaceCommunication_AnalogSensorHardwareAvaiable() == 0)	NIHardwareValue = NIHardwareValue+64;
	
	return NIHardwareValue;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Kontrolliert der Test die angeforderte Option von der NI Hardware zur Verfügung gestellt wird
//  Input           : OptionValue: Option, welche der Test braucht
//  Output *Pointer : 
//         *return  : Ergebnis
//******************************************************************************************************************************
SINT8 CheckOptionValve(SINT16 OptionValue)
{
	SINT16 OptionHardware;
	
	OptionHardware = pHardwareVentil->TestOption;
	
	switch(OptionValue)
	{
		case CH_PFO:					if(OptionHardware == CH_PFO || OptionHardware == CH_SPS_PFO || OptionHardware == CH_PFO_CLUSTER || OptionHardware == CH_SPS_PFO_CLUSTER)																																												return 0;	break;	
		case CH_CLUSTER:			if(OptionHardware == CH_CLUSTER || OptionHardware == CH_SPS_CLUSTER || OptionHardware == CH_PFO_CLUSTER || OptionHardware == CH_SPS_PFO_CLUSTER || OptionHardware == CH_SPFO_CLUSTER || OptionHardware == CH_SPS_SPFO_CLUSTER)	return 0; break;
		case CH_PFO_CLUSTER:	if(OptionHardware == CH_PFO_CLUSTER || OptionHardware == CH_SPS_PFO_CLUSTER)																																																																										return 0; break;
		case CH_SPFO:					if(OptionHardware == CH_SPFO || OptionHardware == CH_SPS_SPFO || OptionHardware == CH_SPFO_CLUSTER || OptionHardware == CH_SPS_SPFO_CLUSTER)																																										return 0; break;
		case CH_SPFO_CLUSTER:	if(OptionHardware == CH_SPFO_CLUSTER || OptionHardware == CH_SPS_SPFO_CLUSTER)																																																																									return 0; break;
		default:																																																																																																																							return 0;
	}
	
	return -1;
}






//Callback Function
//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : steuert das Change Hardware Fenster
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int  CVICALLBACK Chance_Hardware_Panel(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
		{
			UserPushButton = CANCEL_BUTTON;
			ChangeHardwarePanelCloseStatus = TRUE;
		}
		default:	break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Listet alle Tests auf, welche nicht mit der aktuellen Hardware durchgeführt werden können. 
//										Listet alle Interfaces auf, mit welchen nicht Kommuniziert werden können
//  Input           : 
//  Output *Pointer : 
//         *return  : 
//******************************************************************************************************************************
UINT8 ChangeHardware_RequestDiffrentHardwareForTestSequence(UINT8 Setting)
{
	//um das Change Hardware Panel nach dem zweiten Durchlauf der Funktion noch offen zu behalten
	ChangeHardwarePanelCloseStatus = FALSE;
	
	//Multi Threading um die Oberfläche sowie die Auswertung vornehmen zu können
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThreadFunction3, NULL, &ThreadFunctionId3);
	if(Setting == CH_SETTING_LIST_TESTS)						CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThreadFunction4, NULL, &ThreadFunctionId4);
	if(Setting == CH_SETTING_DESCRIBE_DIFFRENT_HW)	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE, ThreadFunction5, NULL, &ThreadFunctionId5);
	
	//Warten bis die thread Funktionen abgeschlossen sind
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionId3, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	if(Setting == CH_SETTING_LIST_TESTS)						CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionId4, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	if(Setting == CH_SETTING_DESCRIBE_DIFFRENT_HW)	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE, ThreadFunctionId5, OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	
	return UserPushButton;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : In diesem Task wird die Change Hardware Oberfläche ausgeführt
//  Input           : 
//  Output *Pointer : -
//         *return  : Sucessful
//******************************************************************************************************************************
static int CVICALLBACK ThreadFunction3 (void *functionData)
{
	int StatusUserInterfaceCH = 0; 

	//Oberfläche laden
	CHpanel = LoadPanel (0, "ChangeHardware.uir", CHPANEL);
	HidePanel(RPpanel);
	HidePanel(STpanel);
	DisplayPanel(CHpanel);
	LoadChangeHardware = TRUE;
	StatusUserInterfaceCH = RunUserInterface();
	DiscardPanel(CHpanel);
	LoadChangeHardware = FALSE;
	
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Change Hardware Oberfläche, wenn die Hardware Voraussetzung nicht gegeben sind oder keine Kommunikation auf dem Interface möglich ist
//  Input           : 
//  Output *Pointer : -
//         *return  : Sucessful
//******************************************************************************************************************************
static int CVICALLBACK ThreadFunction4 (void *functionData)
{
	UINT8 InterfaceCommunicationIndex;
	double StartUserDecision, DiffUserDecision;
	
	WriteDiffrentHardwareOnChangeHardwarePanel();
	if(NeedDiffrentHardware == TRUE  || IsInterfaceCommunicationAvaiable != ERROR_FREE)
	{
		StartUserDecision = Timer();
		while(UserPushButton != OK_BUTTON && UserPushButton != CANCEL_BUTTON && (DiffUserDecision = Timer() - StartUserDecision < WAIT_USER_FOR_DECISION_TIME))	Delay(1);
		if(UserPushButton != OK_BUTTON && UserPushButton != CANCEL_BUTTON) 
		{
			UserPushButton	= USER_PUSH_NO_BUTTON;
			ChangeHardwarePanelCloseStatus = TRUE;
		}
	}
	else	
	{
		UserPushButton = HARDWARE_OK;
		ChangeHardwarePanelCloseStatus = TRUE;
	}
	//Überprüfte Interface Liste wieder löschen um beim nächsten Durchlauf wieder von neuem zu prüfen
	for(InterfaceCommunicationIndex = 0; InterfaceCommunicationIndex < CheckedInterfaceCommunication; InterfaceCommunicationIndex++)	CheckedInterfaceCommunicationList[InterfaceCommunicationIndex][0] = 0;
	CheckedInterfaceCommunication = 0;
	ResultAvaiableInterfaceCommunication = 0;
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Change Hardware Oberfläche, wenn die Hardware Voraussetzung nicht gegeben sind oder keine Kommunikation auf dem Interface möglich ist
//									: (Bei der Hardware Voraussetzung wird explixit erwähnt was beim aktuellen Test nicht stimmt)
//  Input           : 
//  Output *Pointer : -
//         *return  : Sucessful
//******************************************************************************************************************************
static int CVICALLBACK ThreadFunction5 (void *functionData)
{
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 0, "Please follow the change hardware instruction.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 1, "Ok button causes, that the hardware will tested.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 2, "Cancel button causes, that the test collection will continue.");
	
	while(LoadChangeHardware != TRUE)	Delay(1);
	ChangeHardware_RequestUserHardwareChange();
	ChangeHardware_RequestUserLookInterfaceCommunication();
	//Wenn der User die Hardware wechseln muss, so muss er die Option haben dies zu verweigern
	SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_CANCEL, ATTR_VISIBLE, 1);
	SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_OK, ATTR_VISIBLE, 1);
	
	return 0;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt bei falcher Hardware Voraussetzung, die benötigte Hardware in das Change Hardware Panel.
//									:	Weiter wird auch die Kommunikation auf dem Interface überprüft
//  Input           : 
//  Output *Pointer : -
//         *return  : Lieferr das Ergebnis der Hardware Auswertung zurück
//******************************************************************************************************************************
static void WriteDiffrentHardwareOnChangeHardwarePanel(void)
{
	UINT16 CountTestDiffrentHardware = 0, CountInterfaceWithNotAvaiableCommunication = 0;
	
	while(LoadChangeHardware != TRUE)	Delay(1);
	
	//Infos zur Oberfläche dem User geben
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 0, "First all test will verified, if the hardware fits.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 1, "If not, all hardware dismatch tests will be listed.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 2, "Ok button causes, the test collection will start.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 3, "Attention that the User has change the Hardware later during the test procedure.");
	InsertTextBoxLine(CHpanel, CHPANEL_TEXTBOX_CH_INFO, 4, "Cancel button causes, the test collection will not started.");
	
	//Change Hardware Oberfläche Request Hardware LED einschalten
	SetCtrlAttribute(CHpanel, CHPANEL_REQUEST_LED, ATTR_ON_COLOR, VAL_YELLOW); 
	LedStateChangeHardware = TRUE;
	SetCtrlVal(CHpanel, CHPANEL_REQUEST_LED, 1);
	
	//Auswertung Hardware
	CountTestDiffrentHardware = RequestDiffrentHardwareForTestSequence();
	CountInterfaceWithNotAvaiableCommunication = RequestAvaiableInterfaceCommunicationTestSequence();

	if((CountTestDiffrentHardware > 0) || (CountInterfaceWithNotAvaiableCommunication > 0))
	{
		if(CountTestDiffrentHardware > 0)										
		{
			WriteDiffrentHardwareForTestSequence();
			NeedDiffrentHardware = TRUE;
		}
		if(CountInterfaceWithNotAvaiableCommunication > 0)	WriteNotAvaiableInterfaceCommunicationForTestSequence();
		
		//Change Hardware Oberfläche Request Hardware LED ausschalten
		SetCtrlVal(CHpanel, CHPANEL_REQUEST_LED, 0);
		LedStateChangeHardware = FALSE;
		//Buttons nach der Auswertung wieder einblenden
		SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_CANCEL, ATTR_VISIBLE, 1);
		SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_OK, ATTR_VISIBLE, 1);
	}
	else	ChangeHardwarePanelCloseStatus = TRUE;
}


//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Bestätigt, dass der User einverstanden ist, dass im Ablauf der Test Kollektion die Hardware gewechselt werden muss
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK OkButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		{
			UserPushButton = OK_BUTTON;
			ChangeHardwarePanelCloseStatus = TRUE;
			UserWantChangeHardware = TRUE;
		}
		break;
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
int CVICALLBACK CancelButton (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			{
				UserPushButton = CANCEL_BUTTON;
				ChangeHardwarePanelCloseStatus = TRUE;
				UserWantChangeHardware = FALSE;
			}
			break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Kontrolliert jede Sekunde den Change Hardware Windows Close Status. Solbald dieser auf True gesetzt wird, wird das Fenster geschlossen
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK TimerCHWindow (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
		{
			if(ChangeHardwarePanelCloseStatus == TRUE)
			{
				SetActivePanel(CHpanel);
				//Beim schliessen der Oberfläche sollen die Buttons nicht mehr eingeblendet werden
				SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_CANCEL, ATTR_VISIBLE, 0);
				SetCtrlAttribute(CHpanel, CHPANEL_COMMANDBUTTON_OK, ATTR_VISIBLE, 0);
				//muss das Fenster aus einer Callback Fenster verlassen
				QuitUserInterface(0);
				DisplayPanel(RPpanel);
			}
		}
		break;
	}
	return 0;
}

//******************************************************************************************************************************
//  Procedure/Callback
//******************************************************************************************************************************
//  Description     : Wechslet jede halbe Sekunde den Zustand des Request Hardware LED's. Jedoch nur, wenn die Hardware auch wirklich ausgewertet wird
//  Input           : 
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
int CVICALLBACK TimerLedCHFlashing (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			int LedState;
			
			if(LedStateChangeHardware == TRUE)
			{
				GetCtrlVal(CHpanel, CHPANEL_REQUEST_LED, &LedState);
				LedState ^= 1;
				SetCtrlVal(CHpanel, CHPANEL_REQUEST_LED, LedState);
			}
			break;
	}
	return 0;
}
