+++
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
#include <cvinetv.h>
#include <stdint.h>
#include <stdio.h>

#include "HardwareEnums.h"
#include "HelpFunctions.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************

/*Hardware Types Definitionen  (Die Reihenfolge von )*/ 
struct HardwareDefinition TestHardwareDefinition = {
.Modul.General.pEnum = "General",
.Modul.System.pEnum = "System",
.Modul.Valve.pEnum = "Valve",
.Modul.Position_Control.pEnum = "Position Control",
.Modul.Pressure_Control.pEnum = "Pressure Control",
.Modul.Pressure_Sensor.pEnum = "Pressure Sensor",
.Modul.Interface_RS232_RS485.pEnum = "Interface RS232/RS485",
.Modul.Interface_DeviceNet.pEnum = "Interface DeviceNet",
.Modul.Interface_EtherCAT.pEnum = "Interface EtherCAT",
.Modul.Interface_Logic.pEnum = "Interface Logic",
.Modul.Interface_Service.pEnum = "Interface Service",
.Modul.Power_Connector_IO.pEnum = "Power Connector IO",
.Modul.Cluster.pEnum = "Cluster",
.Modul.Power_Fail_Option.pEnum = "Power Fail Option",
.Modul.NVMemory.pEnum = "NVMemory",
.Modul.Drive_Unit.pEnum = "Drive Unit",
.Modul.Parameter.pEnum = "Parameter",
.ValveSerie.All.EnumIndex = 0,
.ValveSerie.All.EnumValue = -1,
.ValveSerie.All.pEnum = "All",
.ValveSerie.Unknown.EnumIndex = 1,
.ValveSerie.Unknown.EnumValue = 0,
.ValveSerie.Unknown.pEnum = "Unknown",
.ValveSerie.R163.EnumIndex = 2,
.ValveSerie.R163.EnumValue = 163,
.ValveSerie.R163.pEnum = "16.3",
.ValveSerie.R590.EnumIndex = 3,
.ValveSerie.R590.EnumValue = 590,
.ValveSerie.R590.pEnum = "59.0",
.ValveSerie.R612.EnumIndex = 4,
.ValveSerie.R612.EnumValue = 612,
.ValveSerie.R612.pEnum = "61.2",
.ValveSerie.R613.EnumIndex = 5,
.ValveSerie.R613.EnumValue = 613,
.ValveSerie.R613.pEnum = "61.3",
.ValveSerie.R615.EnumIndex = 6,
.ValveSerie.R615.EnumValue = 615,
.ValveSerie.R615.pEnum = "61.5",
.ValveSerie.R616.EnumIndex = 7,
.ValveSerie.R616.EnumValue = 616,
.ValveSerie.R616.pEnum = "61.6",
.ValveSerie.R620.EnumIndex = 8,
.ValveSerie.R620.EnumValue = 620,
.ValveSerie.R620.pEnum = "62.0",
.ValveSerie.R642.EnumIndex = 9,
.ValveSerie.R642.EnumValue = 642,
.ValveSerie.R642.pEnum = "64.2",
.ValveSerie.R650.EnumIndex = 10,
.ValveSerie.R650.EnumValue = 650,
.ValveSerie.R650.pEnum = "65.0",
.ValveSerie.R651.EnumIndex = 11,
.ValveSerie.R651.EnumValue = 651,
.ValveSerie.R651.pEnum = "65.1",
.ValveSerie.R652.EnumIndex = 12,
.ValveSerie.R652.EnumValue = 652,
.ValveSerie.R652.pEnum = "65.2",
.ValveSerie.R653.EnumIndex = 13,
.ValveSerie.R653.EnumValue = 653,
.ValveSerie.R653.pEnum = "65.3",
.ValveSerie.R655.EnumIndex = 14,
.ValveSerie.R655.EnumValue = 655,
.ValveSerie.R655.pEnum = "65.5",
.ValveSerie.R670.EnumIndex = 15,
.ValveSerie.R670.EnumValue = 670,
.ValveSerie.R670.pEnum = "67.0",
.ValveSerie.R940.EnumIndex = 16,
.ValveSerie.R940.EnumValue = 940,
.ValveSerie.R940.pEnum = "94.0",
.ValveSerie.R950.EnumIndex = 17,
.ValveSerie.R950.EnumValue = 950,
.ValveSerie.R950.pEnum = "95.0",
.ValveSerie.R951.EnumIndex = 18,
.ValveSerie.R951.EnumValue = 951,
.ValveSerie.R951.pEnum = "95.1",
.ValveSerie.R952.EnumIndex = 19,
.ValveSerie.R952.EnumValue = 952,
.ValveSerie.R952.pEnum = "95.2",
.ValveSerie.R980.EnumIndex = 20,
.ValveSerie.R980.EnumValue = 980,
.ValveSerie.R980.pEnum = "98.0",
.Controller.All.EnumIndex = 0,
.Controller.All.EnumValue = -1,
.Controller.All.pEnum = "All",	
.Controller.Unknown.EnumIndex = 1,
.Controller.Unknown.EnumValue = 65535,
.Controller.Unknown.pEnum = "Unknown",
.Controller.IC2H1.EnumIndex = 2,
.Controller.IC2H1.EnumValue = 1,
.Controller.IC2H1.pEnum = "IC2H1",
.Controller.IC2H2.EnumIndex = 3,
.Controller.IC2H2.EnumValue = 2,
.Controller.IC2H2.pEnum = "IC2H2",
.Controller.IC2H3.EnumIndex = 4,
.Controller.IC2H3.EnumValue = 3,
.Controller.IC2H3.pEnum = "IC2H3",
.Controller.IC2H4.EnumIndex = 5,
.Controller.IC2H4.EnumValue = 4,
.Controller.IC2H4.pEnum = "IC2H4",
.Controller.IC2H5.EnumIndex = 6,
.Controller.IC2H5.EnumValue = 5,
.Controller.IC2H5.pEnum = "IC2H5",
.Interface.All.EnumIndex = 0,
.Interface.All.EnumValue = -1,
.Interface.All.pEnum = "All",
.Interface.Unknown.EnumIndex = 1,
.Interface.Unknown.EnumValue = 65535,
.Interface.Unknown.pEnum = "Unknown",
.Interface.Not_Avaiable.EnumIndex = 2,
.Interface.Not_Avaiable.EnumValue = 65534,
.Interface.Not_Avaiable.pEnum = "n.a.",
.Interface.RS232_RS485.EnumIndex = 3,
.Interface.RS232_RS485.EnumValue = 1,
.Interface.RS232_RS485.pEnum = "RS232/RS485",
.Interface.EtherCAT.EnumIndex = 4,
.Interface.EtherCAT.EnumValue = 2,
.Interface.EtherCAT.pEnum = "EtherCAT",
.Interface.DeviceNet.EnumIndex = 5,
.Interface.DeviceNet.EnumValue = 3,
.Interface.DeviceNet.pEnum = "DeviceNet",
.Interface.Onboard.EnumIndex = 6,
.Interface.Onboard.EnumValue = 4,
.Interface.Onboard.pEnum = "Onboard",
.Interface.Logic.EnumIndex = 7,
.Interface.Logic.EnumValue = 5,
.Interface.Logic.pEnum = "Logic",
.Interface.Profibus.EnumIndex = 8,
.Interface.Profibus.EnumValue = 6,
.Interface.Profibus.pEnum = "Profibus",
.Interface.CCLink.EnumIndex = 9,
.Interface.CCLink.EnumValue = 7,
.Interface.CCLink.pEnum = "CCLink",
.SingleOption.All.EnumIndex = 0,
.SingleOption.All.pEnum = "All",
.SingleOption.Cluster.EnumIndex = 1,
.SingleOption.Cluster.pEnum = "Cluster",
.SingleOption.SPS.EnumIndex = 2,
.SingleOption.SPS.pEnum = "SPS",
.SingleOption.PFO.EnumIndex = 3,
.SingleOption.PFO.pEnum = "PFO",
.SingleOption.PFO2.EnumIndex = 4,
.SingleOption.PFO2.pEnum = "PFO2",
.SingleOption.PFO3.EnumIndex = 5,
.SingleOption.PFO3.pEnum = "PFO3",
.Option.Not_Avaiable.EnumValue = 0,
.Option.Not_Avaiable.pEnum = "n.a.",
.Option.SPS.EnumValue = 1,
.Option.SPS.pEnum = "SPS",
.Option.PFO.EnumValue = 2,
.Option.PFO.pEnum = "PFO",
.Option.Cluster.EnumValue = 3,
.Option.Cluster.pEnum = "Cluster",
.Option.SPS_PFO.EnumValue = 4,
.Option.SPS_PFO.pEnum = "SPS_PFO",
.Option.SPS_Cluster.EnumValue = 5,
.Option.SPS_Cluster.pEnum = "SPS_Cluster",
.Option.PFO_Cluster.EnumValue = 6,
.Option.PFO_Cluster.pEnum = "PFO_Cluster",
.Option.SPS_PFO_Cluster.EnumValue = 7,
.Option.SPS_PFO_Cluster.pEnum = "SPS_PFO_Cluster",
.Option.PFO2.EnumValue = 8,
.Option.PFO2.pEnum = "PFO2",
.Option.SPS_PFO2.EnumValue = 9,
.Option.SPS_PFO2.pEnum = "SPS_PFO2",
.Option.PFO2_Cluster.EnumValue = 10,
.Option.PFO2_Cluster.pEnum = "PFO2_Cluster",
.Option.SPS_PFO2_Cluster.EnumValue = 11,
.Option.SPS_PFO2_Cluster.pEnum = "SPS_PFO2_Cluster",
.Option.PFO3.EnumValue = 12,
.Option.PFO3.pEnum = "PFO3",
.Option.SPS_PFO3.EnumValue = 13,
.Option.SPS_PFO3.pEnum = "SPS_PFO3",
.Option.Unknown.EnumValue = 255,
.Option.Unknown.pEnum = "Unknown",
.TestHardware.All.EnumIndex = 0,
.TestHardware.All.pEnum = "All",
.TestHardware.NI_Power_Connector.EnumIndex = 1,
.TestHardware.NI_Power_Connector.pEnum = "NI Power Connector",
.TestHardware.Interface_RS232_RS485.EnumIndex = 2,
.TestHardware.Interface_RS232_RS485.pEnum = "Interface RS232/RS485",
.TestHardware.Logic.EnumIndex = 3,
.TestHardware.Logic.pEnum = "Logic",
.TestHardware.DeviceNet.EnumIndex = 4,
.TestHardware.DeviceNet.pEnum = "DeviceNet",
.TestHardware.EtherCAT.EnumIndex = 5,
.TestHardware.EtherCAT.pEnum = "EtherCAT",
.TestHardware.NI_Analog_Sensor.EnumIndex = 6,
.TestHardware.NI_Analog_Sensor.pEnum = "NI Analog Sensor",
.ControllerHardwareVersion.All.EnumIndex = 0,
.ControllerHardwareVersion.All.EnumValue = -1,
.ControllerHardwareVersion.All.pEnum = "All",
.ControllerHardwareVersion.Not_Avaiable.EnumIndex = 1,
.ControllerHardwareVersion.Not_Avaiable.EnumValue = 0,
.ControllerHardwareVersion.Not_Avaiable.pEnum = "n.a.",
.ControllerHardwareVersion.Version_1.EnumIndex = 2,
.ControllerHardwareVersion.Version_1.EnumValue = 1,
.ControllerHardwareVersion.Version_1.pEnum = "Version 1",
.InterfaceHardwareVersion.All.EnumIndex = 0,
.InterfaceHardwareVersion.All.EnumValue = -1,
.InterfaceHardwareVersion.All.pEnum = "All",
.InterfaceHardwareVersion.Not_Avaiable.EnumIndex = 1,
.InterfaceHardwareVersion.Not_Avaiable.EnumValue = 0,
.InterfaceHardwareVersion.Not_Avaiable.pEnum = "n.a.",
.InterfaceHardwareVersion.Version_1.EnumIndex = 2,
.InterfaceHardwareVersion.Version_1.EnumValue = 1,
.InterfaceHardwareVersion.Version_1.pEnum = "Version 1",
.Module1HardwareVersion.All.EnumIndex = 0,
.Module1HardwareVersion.All.EnumValue = -1,
.Module1HardwareVersion.All.pEnum = "All",
.Module1HardwareVersion.Not_Avaiable.EnumIndex = 1,
.Module1HardwareVersion.Not_Avaiable.EnumValue = 0,
.Module1HardwareVersion.Not_Avaiable.pEnum = "n.a.",
.Module2HardwareVersion.All.EnumIndex = 0,
.Module2HardwareVersion.All.EnumValue = -1,
.Module2HardwareVersion.All.pEnum = "All",
.Module2HardwareVersion.Not_Avaiable.EnumIndex = 1,
.Module2HardwareVersion.Not_Avaiable.EnumValue = 0,
.Module2HardwareVersion.Not_Avaiable.pEnum = "n.a.",
.Module3HardwareVersion.All.EnumIndex = 0,
.Module3HardwareVersion.All.EnumValue = -1,
.Module3HardwareVersion.All.pEnum = "All",
.Module3HardwareVersion.Not_Avaiable.EnumIndex = 1,
.Module3HardwareVersion.Not_Avaiable.EnumValue = 0,
.Module3HardwareVersion.Not_Avaiable.pEnum = "n.a.",
.Module4HardwareVersion.All.EnumIndex = 0,
.Module4HardwareVersion.All.EnumValue = -1,
.Module4HardwareVersion.All.pEnum = "All",
.Module4HardwareVersion.Not_Avaiable.EnumIndex = 1,
.Module4HardwareVersion.Not_Avaiable.EnumValue = 0,
.Module4HardwareVersion.Not_Avaiable.pEnum = "n.a.",
.ControllerAssemblyVariant.All.EnumIndex = 0,
.ControllerAssemblyVariant.All.EnumValue = -1,
.ControllerAssemblyVariant.All.pEnum = "All",
.ControllerAssemblyVariant.Not_Avaiable.EnumIndex = 1,
.ControllerAssemblyVariant.Not_Avaiable.EnumValue = 0,
.ControllerAssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.ControllerAssemblyVariant.Version_1.EnumIndex = 2,
.ControllerAssemblyVariant.Version_1.EnumValue = 1,
.ControllerAssemblyVariant.Version_1.pEnum = "Version 1",
.InterfaceAssemblyVariant.All.EnumIndex = 0,
.InterfaceAssemblyVariant.All.EnumValue = -1,
.InterfaceAssemblyVariant.All.pEnum = "All",
.InterfaceAssemblyVariant.Not_Avaiable.EnumIndex = 1,
.InterfaceAssemblyVariant.Not_Avaiable.EnumValue = 0,
.InterfaceAssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.InterfaceAssemblyVariant.Version_1.EnumIndex = 2,
.InterfaceAssemblyVariant.Version_1.EnumValue = 1,
.InterfaceAssemblyVariant.Version_1.pEnum = "Version 1",
.Module1AssemblyVariant.All.EnumIndex = 0,
.Module1AssemblyVariant.All.EnumValue = -1,
.Module1AssemblyVariant.All.pEnum = "All",
.Module1AssemblyVariant.Not_Avaiable.EnumIndex = 1,
.Module1AssemblyVariant.Not_Avaiable.EnumValue = 0,
.Module1AssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.Module2AssemblyVariant.All.EnumIndex = 0,
.Module2AssemblyVariant.All.EnumValue = -1,
.Module2AssemblyVariant.All.pEnum = "All",
.Module2AssemblyVariant.Not_Avaiable.EnumIndex = 1,
.Module2AssemblyVariant.Not_Avaiable.EnumValue = 0,
.Module2AssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.Module3AssemblyVariant.All.EnumIndex = 0,
.Module3AssemblyVariant.All.EnumValue = -1,
.Module3AssemblyVariant.All.pEnum = "All",
.Module3AssemblyVariant.Not_Avaiable.EnumIndex = 1,
.Module3AssemblyVariant.Not_Avaiable.EnumValue = 0,
.Module3AssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.Module4AssemblyVariant.All.EnumIndex = 0,
.Module4AssemblyVariant.All.EnumValue = -1,
.Module4AssemblyVariant.All.pEnum = "All",
.Module4AssemblyVariant.Not_Avaiable.EnumIndex = 1,
.Module4AssemblyVariant.Not_Avaiable.EnumValue = 0,
.Module4AssemblyVariant.Not_Avaiable.pEnum = "n.a.",
.Module1.All.EnumIndex = 0,
.Module1.All.EnumValue = -1,
.Module1.All.pEnum = "All",
.Module1.Not_Avaiable.EnumIndex = 1,
.Module1.Not_Avaiable.EnumValue = 0,
.Module1.Not_Avaiable.pEnum = "n.a.",
.Module1.Three_Axis_Motion_Controller.EnumIndex = 2,
.Module1.Three_Axis_Motion_Controller.EnumValue = 1,
.Module1.Three_Axis_Motion_Controller.pEnum = "Three Axis Motion Controller",
.Module2.All.EnumIndex = 0,
.Module2.All.EnumValue = -1,
.Module2.All.pEnum = "All",
.Module2.Not_Avaiable.EnumIndex = 1,
.Module2.Not_Avaiable.EnumValue = 0,
.Module2.Not_Avaiable.pEnum = "n.a.",
.Module3.All.EnumIndex = 0,
.Module3.All.EnumValue = -1,
.Module3.All.pEnum = "All",
.Module3.Not_Avaiable.EnumIndex = 1,
.Module3.Not_Avaiable.EnumValue = 0,
.Module3.Not_Avaiable.pEnum = "n.a.",
.Module4.All.EnumIndex = 0,
.Module4.All.EnumValue = -1,
.Module4.All.pEnum = "All",
.Module4.Not_Avaiable.EnumIndex = 1,
.Module4.Not_Avaiable.EnumValue = 0,
.Module4.Not_Avaiable.pEnum = "n.a.",
};

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Byte Wert in einen Zahlen Wert um
//  Input           : -
//  Output *Pointer : -
//         *return  : Zahlenwert der Test Hardware Information
//******************************************************************************************************************************
SINT32 HardwareEnums_TranslateBytesInIntValue_1(SINT8 EnumIndex)
{
	return (SINT32)pow(2, EnumIndex);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Byte Wert in einen Zahlen Wert um
//  Input           : -
//  Output *Pointer : -
//         *return  : Zahlenwert der Test Hardware Information
//******************************************************************************************************************************
SINT32 HardwareEnums_TranslateBytesInIntValue_2(SINT8 EnumIndex1, SINT8 EnumIndex2)
{
	SINT32 HardwareInformationValue;
	
	if(EnumIndex1 == -1 || EnumIndex2 == -1)	return -1;
	HardwareInformationValue = (SINT32)pow(2, EnumIndex1) + (SINT32)pow(2, EnumIndex2);
	return HardwareInformationValue;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Byte Wert in einen Zahlen Wert um
//  Input           : -
//  Output *Pointer : -
//         *return  : Zahlenwert der Test Hardware Information
//******************************************************************************************************************************
SINT32 HardwareEnums_TranslateBytesInIntValue_3(SINT8 EnumIndex1, SINT8 EnumIndex2, SINT8 EnumIndex3)
{
	SINT32 HardwareInformationValue;
	
	if(EnumIndex1 == -1 || EnumIndex2 == -1 || EnumIndex3 == -1)	return -1;
	HardwareInformationValue = (SINT32)pow(2, EnumIndex1) + (SINT32)pow(2, EnumIndex2) + (SINT32)pow(2, EnumIndex3);
	return HardwareInformationValue;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Byte Wert in einen Zahlen Wert um
//  Input           : -
//  Output *Pointer : -
//         *return  : Zahlenwert der Test Hardware Information
//******************************************************************************************************************************
SINT32 HardwareEnums_TranslateBytesInIntValue_4(SINT8 EnumIndex1, SINT8 EnumIndex2, SINT8 EnumIndex3, SINT8 EnumIndex4)
{
	SINT32 HardwareInformationValue;
	
	if(EnumIndex1 == -1 || EnumIndex2 == -1 || EnumIndex3 == -1 || EnumIndex4 == -1)	return -1;
	HardwareInformationValue = (SINT32)pow(2, EnumIndex1) + (SINT32)pow(2, EnumIndex2) + (SINT32)pow(2, EnumIndex3) + (SINT32)pow(2, EnumIndex4);
	return HardwareInformationValue;
}





//Liefert alle Enum Indexe des EnumValues
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Wandelt den Enum Wert in einen BOOL Array um (1 bedeutet, das dieser Eintrag unterstützt wird)
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void GetEnumIndex32BitArray(SINT32 EnumValue, BOOL8 *pEnumIndexArray)
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
void GetEnumIndex16BitArray(SINT16 EnumValue, BOOL8 *pEnumIndexArray)
{
	int i;
	
	for (i = 15; i >= 0; i--)	pEnumIndexArray[i] = ((EnumValue >> i) & 0x1);
}







//Liefert die Struktur Informationen 
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Name Inhalt als String zurück
//  Input           : -
//  Output *Pointer : pHardwareTypeString:	Textstring, inwelchem nach dem Name Inhalt gesucht und zurück geschrieben wird 
//         *return  : 
//******************************************************************************************************************************
BOOL8 HardwareEnums_GetEnumString(char *pHardwareTypeString)
{
	BOOL8 SetEnumString = FALSE;
	char QuoteString[2], CommaString[3];
	QuoteString[0] = 0x22;
	QuoteString[1] = 0;
	CommaString[0] = 0x22;
	CommaString[1] = 0x2C;
	CommaString[2] = 0;
	
	//Zuerst den Inhalt nach pName heraus finden
	if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "pEnum") != NULL)
	{
		//Anschliessend nach " Zeichen den Inhalt haben
		if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, QuoteString) != NULL)
		{
			//, aus dem String entfernen
			HelpFunctions_StringReplace(CommaString, "", pHardwareTypeString);
			SetEnumString = TRUE;
		}
	}
	return SetEnumString;
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Enum Index zurück des HardwareType
//  Input           : -
//  Output *Pointer : pHardwareTypeString:	Hardware Type String
//									: pEnumIndex: Enum Index des HardwareTypes 
//         *return  : -
//******************************************************************************************************************************
void HardwareEnums_GetEnumIndex(char *pHardwareTypeString, SINT16 *pEnumIndex)
{
	SINT32 Temp;
	
	//Zuerst den Inhalt nach pName heraus finden
	if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "EnumIndex") != NULL)
	{
		//Anschliessend nach = Zeichen den Inhalt haben
		if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "=") != NULL)
		{
			//, aus dem String entfernen
			HelpFunctions_StringReplace(",", "", pHardwareTypeString);
			sscanf(pHardwareTypeString,   "%d", &Temp);
			*pEnumIndex = (SINT16)Temp;
		}
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Enum Value zurück des HardwareType
//  Input           : -
//  Output *Pointer : pHardwareTypeString:	Hardware Type String
//									: pEnumValue: Enum Value des HardwareTypes 
//         *return  : -
//******************************************************************************************************************************
void HardwareEnums_GetEnumValue(char *pHardwareTypeString, SINT16 *pEnumValue)
{
	SINT32 Temp;
	
	//Zuerst den Inhalt nach pName heraus finden
	if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "EnumValue") != NULL)
	{
		//Anschliessend nach = Zeichen den Inhalt haben
		if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "=") != NULL)
		{
			//, aus dem String entfernen
			HelpFunctions_StringReplace(",", "", pHardwareTypeString);
			sscanf(pHardwareTypeString,   "%d", &Temp);
			*pEnumValue = (SINT16)Temp;
		}
	}
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liefert den Enum Value zurück des HardwareType
//  Input           : -
//  Output *Pointer : pHardwareTypeString:	Hardware Type String
//									: pEnumValue: Enum Value des HardwareTypes 
//         *return  : -
//******************************************************************************************************************************
void HardwareEnums_GetEnumValueInt32(char *pHardwareTypeString, SINT32 *pEnumValue)
{
	//Zuerst den Inhalt nach pName heraus finden
	if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "EnumValue") != NULL)
	{
		//Anschliessend nach = Zeichen den Inhalt haben
		if(HelpFunctions_StringAfterSubstring(pHardwareTypeString, "=") != NULL)
		{
			//, aus dem String entfernen
			HelpFunctions_StringReplace(",", "", pHardwareTypeString);
			sscanf(pHardwareTypeString,   "%d", pEnumValue);
		}
	}
}
---