+++
//******************************************************************************************************************************
//  Headerfile
//******************************************************************************************************************************
//                                                               __    __     _______
//  VAT Vacuumvalves AG                                          \ \  / / /\ |__   __|
//  Control Valve Group                                           \ \/ / /  \   | |
//  CH-9469 Haag (Switzerland)                                     \  / / /\ \  | |
//                                                                  \/ /_/  \_\ |_|
//  Copyright(c) VAT Vacuumvalves AG / Switzerland
//
//******************************************************************************************************************************
#ifndef HARDWAREENUMS_H
#define HARDWAREENUMS_H

//******************************************************************************************************************************
//** I N C L U D E S
//******************************************************************************************************************************
#include <cvinetv.h>

#include "Typedefinitions.h"

//******************************************************************************************************************************
//** F U N K T I O N E N
//******************************************************************************************************************************
SINT32 HardwareEnums_TranslateBytesInIntValue_1(SINT8 EnumIndex);
SINT32 HardwareEnums_TranslateBytesInIntValue_2(SINT8 EnumIndex1, SINT8 EnumIndex2);
SINT32 HardwareEnums_TranslateBytesInIntValue_3(SINT8 EnumIndex1, SINT8 EnumIndex2, SINT8 EnumIndex3);
SINT32 HardwareEnums_TranslateBytesInIntValue_4(SINT8 EnumIndex1, SINT8 EnumIndex2, SINT8 EnumIndex3, SINT8 EnumIndex4);

//Liefert alle Enum Indexe des EnumValues
void GetEnumIndex32BitArray(SINT32 EnumValue, BOOL8 *pEnumIndexArray);
void GetEnumIndex16BitArray(SINT16 EnumValue, BOOL8 *pEnumIndexArray);

//Liefert die Struktur Informationen
BOOL8 HardwareEnums_GetEnumString(char *pHardwareTypeString);
void HardwareEnums_GetEnumIndex(char *pHardwareTypeString, SINT16 *pEnumIndex);
void HardwareEnums_GetEnumValue(char *pHardwareTypeString, SINT16 *pEnumValue);
void HardwareEnums_GetEnumValueInt32(char *pHardwareTypeString, SINT32 *pEnumValue);

//******************************************************************************************************************************
//** D E F I N E S
//******************************************************************************************************************************
//Abspeicherung Hardware Types 
struct ModulName
{
	const char *pEnum;
};

struct ValveSerieType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct ControllerType
{
	const SINT8 EnumIndex; 
	const SINT32 EnumValue;
	const char *pEnum;
};

struct InterfaceType
{
	const SINT8 EnumIndex;
	const SINT32 EnumValue;
	const char *pEnum;
};

struct SingleOptionType
{
	const SINT8 EnumIndex;
	const char *pEnum;
};

struct OptionType
{
	const SINT16 EnumValue;
	const char *pEnum;
};

struct TestHardwareType
{
	const SINT8 EnumIndex;
	const char *pEnum;
};

struct ControllerHardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct InterfaceHardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module1HardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module2HardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module3HardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module4HardwareVersionType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct ControllerAssemblyVariantType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct InterfaceAssemblyVariantType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module1AssemblyVariantType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module2AssemblyVariantType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module3AssemblyVariantType
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module4AssemblyVariantType
{
	const SINT8 EnumIndex; 
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module1Type
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module2Type
{
	const SINT8 EnumIndex; 
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module3Type
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct Module4Type
{
	const SINT8 EnumIndex;
	const SINT16 EnumValue;
	const char *pEnum;
};

struct ModulNames
{
  struct ModulName General;                                    
  struct ModulName System;
	struct ModulName Valve;
	struct ModulName Position_Control;
	struct ModulName Pressure_Control;
	struct ModulName Pressure_Sensor;
	struct ModulName Interface_RS232_RS485;
	struct ModulName Interface_DeviceNet;
	struct ModulName Interface_EtherCAT;
	struct ModulName Interface_Logic;
	struct ModulName Interface_Service;
	struct ModulName Power_Connector_IO;
	struct ModulName Cluster;
	struct ModulName Power_Fail_Option;
	struct ModulName NVMemory;
	struct ModulName Drive_Unit;
	struct ModulName Parameter;
};

struct ValveSeries
{
  struct ValveSerieType All;
	struct ValveSerieType Unknown;
	struct ValveSerieType R163;
  struct ValveSerieType R612;
	struct ValveSerieType R613;
	struct ValveSerieType R615;
	struct ValveSerieType R616;
	struct ValveSerieType R620;
	struct ValveSerieType R642;
	struct ValveSerieType R650;
	struct ValveSerieType R590;
	struct ValveSerieType R651;
	struct ValveSerieType R652;
	struct ValveSerieType R653;
	struct ValveSerieType R655;
	struct ValveSerieType R670;
	struct ValveSerieType R940;
	struct ValveSerieType R950;
	struct ValveSerieType R951;
	struct ValveSerieType R952;
	struct ValveSerieType R980;
};

struct ControllerTypes
{
  struct ControllerType All;                                    
	struct ControllerType Unknown;
  struct ControllerType IC2H1;
	struct ControllerType IC2H2;
	struct ControllerType IC2H3;
	struct ControllerType IC2H4;
	struct ControllerType IC2H5;
};

struct InterfaceTypes
{
  struct InterfaceType All;                                    
	struct InterfaceType Unknown;
	struct InterfaceType Not_Avaiable;
  struct InterfaceType RS232_RS485;
	struct InterfaceType EtherCAT;
	struct InterfaceType DeviceNet;
	struct InterfaceType Onboard;
	struct InterfaceType Logic;
	struct InterfaceType Profibus;
	struct InterfaceType CCLink; 
};

struct SingleOptionTypes
{
  struct SingleOptionType All;                                    
  struct SingleOptionType Cluster;
	struct SingleOptionType SPS;
	struct SingleOptionType PFO;
	struct SingleOptionType PFO2;
	struct SingleOptionType PFO3;
};

struct OptionTypes
{
  struct OptionType Not_Avaiable;                                    
	struct OptionType SPS;
	struct OptionType PFO;
  struct OptionType Cluster;
	struct OptionType SPS_PFO;
	struct OptionType SPS_Cluster;
	struct OptionType PFO_Cluster;
	struct OptionType SPS_PFO_Cluster;
	struct OptionType PFO2;
	struct OptionType SPS_PFO2;
	struct OptionType PFO2_Cluster;
	struct OptionType SPS_PFO2_Cluster;
	struct OptionType PFO3;
	struct OptionType SPS_PFO3;
	struct OptionType Unknown;
};

struct TestHardwareTypes
{
  struct TestHardwareType All;                                    
  struct TestHardwareType NI_Power_Connector;
	struct TestHardwareType Interface_RS232_RS485;
	struct TestHardwareType Logic;
	struct TestHardwareType DeviceNet;
	struct TestHardwareType EtherCAT;
	struct TestHardwareType NI_Analog_Sensor;
};

struct ControllerHardwareVersionTypes
{
  struct ControllerHardwareVersionType All;                                    
	struct ControllerHardwareVersionType Not_Avaiable;
  struct ControllerHardwareVersionType Version_1;
};

struct InterfaceHardwareVersionTypes
{
  struct InterfaceHardwareVersionType All;                                    
	struct InterfaceHardwareVersionType Not_Avaiable;
  struct InterfaceHardwareVersionType Version_1;
};

struct Module1HardwareVersionTypes
{
  struct Module1HardwareVersionType All;                                    
	struct Module1HardwareVersionType Not_Avaiable;
};

struct Module2HardwareVersionTypes
{
  struct Module2HardwareVersionType All;                                    
	struct Module2HardwareVersionType Not_Avaiable;
};

struct Module3HardwareVersionTypes
{
  struct Module3HardwareVersionType All;                                    
	struct Module3HardwareVersionType Not_Avaiable;
};

struct Module4HardwareVersionTypes
{
  struct Module4HardwareVersionType All;                                    
	struct Module4HardwareVersionType Not_Avaiable;
};

struct ControllerAssemblyVariantTypes
{
  struct ControllerAssemblyVariantType All;                                    
	struct ControllerAssemblyVariantType Not_Avaiable;
  struct ControllerAssemblyVariantType Version_1;
};

struct InterfaceAssemblyVariantTypes
{
  struct InterfaceAssemblyVariantType All;                                    
	struct InterfaceAssemblyVariantType Not_Avaiable;
  struct InterfaceAssemblyVariantType Version_1;
};

struct Module1AssemblyVariantTypes
{
  struct Module1AssemblyVariantType All;                                    
	struct Module1AssemblyVariantType Not_Avaiable;
};

struct Module2AssemblyVariantTypes
{
  struct Module2AssemblyVariantType All;                                    
	struct Module2AssemblyVariantType Not_Avaiable;
};

struct Module3AssemblyVariantTypes
{
  struct Module3AssemblyVariantType All;                                    
	struct Module3AssemblyVariantType Not_Avaiable;
};

struct Module4AssemblyVariantTypes
{
  struct Module4AssemblyVariantType All;                                    
	struct Module4AssemblyVariantType Not_Avaiable;
};

struct Module1Types
{
  struct Module1Type All;  
	struct Module1Type Not_Avaiable;
	struct Module1Type Three_Axis_Motion_Controller;
};

struct Module2Types
{
  struct Module2Type All;                                    
	struct Module2Type Not_Avaiable;
};

struct Module3Types
{
  struct Module3Type All;                                    
	struct Module3Type Not_Avaiable;
};

struct Module4Types
{
  struct Module4Type All;                                    
	struct Module4Type Not_Avaiable;
};

struct HardwareDefinition
{
  struct ModulNames Modul;
	struct ValveSeries ValveSerie;
	struct ControllerTypes Controller;
	struct InterfaceTypes Interface;
	struct SingleOptionTypes SingleOption;
	struct OptionTypes Option;
	struct TestHardwareTypes TestHardware;
	struct ControllerHardwareVersionTypes ControllerHardwareVersion;
	struct InterfaceHardwareVersionTypes InterfaceHardwareVersion;
	struct Module1HardwareVersionTypes Module1HardwareVersion;
	struct Module2HardwareVersionTypes Module2HardwareVersion;
	struct Module3HardwareVersionTypes Module3HardwareVersion;
	struct Module4HardwareVersionTypes Module4HardwareVersion;
	struct ControllerAssemblyVariantTypes ControllerAssemblyVariant;
	struct InterfaceAssemblyVariantTypes InterfaceAssemblyVariant;
	struct Module1AssemblyVariantTypes Module1AssemblyVariant;
	struct Module2AssemblyVariantTypes Module2AssemblyVariant;
	struct Module3AssemblyVariantTypes Module3AssemblyVariant;
	struct Module4AssemblyVariantTypes Module4AssemblyVariant;
	struct Module1Types Module1;
	struct Module2Types Module2;
	struct Module3Types Module3;
	struct Module4Types Module4;
};

//******************************************************************************************************************************
//** V A R I A B L E N
//******************************************************************************************************************************
extern struct HardwareDefinition TestHardwareDefinition; 

#endif
---