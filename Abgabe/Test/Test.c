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
//Because of name conflicts, Windows headers must be included before utility.h and formatio.h
#include <windows.h>
#include <cvinetv.h>
#include <formatio.h>
#include <rs232.h>
#include <stdint.h>
#include <stdio.h>
#include <utility.h>
//um die Funktion von windows.h benützen zu können
#undef GetSystemTime

#include "Command.h"
#include "CommandTest.h" 
#include "CommandTranslateEnum.h"
#include "DefineEnums.h"
#include "DefineStructFileService.h"
#include "GenerateReport.h"
#include "HardwareEnums.h"
#include "HelpFunctions.h"														 
#include "MainIC2.h"
#include "InterfaceCommunication.h"
#include "ParameterProcessing.h"
#include "PlatformConfig.h"
#include "ReadWriteDIO_PC_IC2H1.h"
#include "SharedVariable.h"
#include "Test.h"
#include "TestEnvironment.h"
#include "Typedefinitions.h"
#include "UsbLabView.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void AccessMode(void);
static void Test(void);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
++
SINT16 TestVersion 													= 1;
char TestName[TEST_NAME_LENGTH] 						= "AccessMode";
//Format YYYYMMDD
char CreationTest[CREATION_TEST_LENGTH] 		= "20170510";
char CreationAuthor[CREATION_AUTHOR_LENGTH]	= "Stucki Andreas";
--

char Command [COMMAND_LENGTH], PathNameUsbLogFile[MAX_PATHNAME_LEN], PathNameLogFile[MAX_PATHNAME_LEN];
BOOL8 IsErrorLastControlMode = FALSE;
UINT8 Error = 0, ExitTestActive = 0;
UINT64 StartUpCount;
FILE *LogFile_Test;
++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt die Testinformationen in die Umgebungsvariable
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void Test_WriteInfoTestInSharedVariable(void)
{
	char TestDuration[TEST_DURATION_LENGTH]					= "00:01:32";
	char TestDescription[TEST_DESCRIPTION_LENGTH] 	= "Prüft den AccessMode";
	char TestModification[TEST_MODIFICATION_LENGTH] = "";
	SINT16 ExternalIsolationValveOption =  0;
	SINT16 OptionValue, TestHardwareValue, ControllerHardwareVersionValue, InterfaceHardwareVersionValue, Module1HardwareVersionValue;
	SINT16 Module2HardwareVersionValue, Module3HardwareVersionValue, Module4HardwareVersionValue, ControllerAssemblyVariantValue, InterfaceAssemblyVariantValue;
	SINT16 Module1AssemblyVariantValue, Module2AssemblyVariantValue, Module3AssemblyVariantValue, Module4AssemblyVariantValue, Module1Value, Module2Value, Module3Value, Module4Value;
	SINT32 ValveSerieValue, ControllerValue, InterfaceValue;
	ValveSerieValue = 												HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.ValveSerie.All.EnumIndex);
	ControllerValue = 												HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Controller.All.EnumIndex);
	InterfaceValue = 													HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Interface.All.EnumIndex);
	OptionValue = 										(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.SingleOption.All.EnumIndex); 
	TestHardwareValue = 							(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.TestHardware.All.EnumIndex);
	ControllerHardwareVersionValue =  (SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.ControllerHardwareVersion.All.EnumIndex);
	InterfaceHardwareVersionValue = 	(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.InterfaceHardwareVersion.All.EnumIndex);
	ControllerAssemblyVariantValue =	(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.ControllerAssemblyVariant.All.EnumIndex);
	InterfaceAssemblyVariantValue =		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.InterfaceAssemblyVariant.All.EnumIndex);
	Module1HardwareVersionValue = 		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module1HardwareVersion.All.EnumIndex);
	Module2HardwareVersionValue = 		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module2HardwareVersion.All.EnumIndex);
	Module3HardwareVersionValue = 		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module3HardwareVersion.All.EnumIndex);
	Module4HardwareVersionValue =			(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module4HardwareVersion.All.EnumIndex);
	Module1AssemblyVariantValue = 		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module1AssemblyVariant.All.EnumIndex);
	Module2AssemblyVariantValue =			(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module2AssemblyVariant.All.EnumIndex);
	Module3AssemblyVariantValue = 		(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module3AssemblyVariant.All.EnumIndex);
	Module4AssemblyVariantValue =			(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module4AssemblyVariant.All.EnumIndex);
	Module1Value = 										(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module1.All.EnumIndex);
	Module2Value = 										(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module2.All.EnumIndex);
	Module3Value =										(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module3.All.EnumIndex);
	Module4Value =										(SINT16)HardwareEnums_TranslateBytesInIntValue_1(TestHardwareDefinition.Module4.All.EnumIndex);
	
	//Alle benötigten Umgebungsvariablen initiallisieren
	SharedVariable_InitNetworkvariableTestHardware();
	
	SharedVariable_WriteTestNameValue(TestName);
	SharedVariable_WriteTestDescriptionValue(TestDescription);
	SharedVariable_WriteCreationTestValue(CreationTest);
	SharedVariable_WriteCreationAuthorValue(CreationAuthor);
	SharedVariable_WriteTestVersionValue(TestVersion);
	SharedVariable_WriteTestModificationValue(TestModification);
	SharedVariable_WriteTestDuratingValue(TestDuration);
	SharedVariable_WriteTestModulNameValue(TestHardwareDefinition.Modul.System.pEnum);
	SharedVariable_WriteTestExternalIsolationValveOptionValue(ExternalIsolationValveOption);
	SharedVariable_WriteTestValveSerieValue(ValveSerieValue);
	SharedVariable_WriteTestControllerValue(ControllerValue);
	SharedVariable_WriteTestInterfaceValue(InterfaceValue);
	SharedVariable_WriteTestOptionValue(OptionValue);
	SharedVariable_WriteTestHardwareValue(TestHardwareValue);
	SharedVariable_WriteTestControllerHardwareVersionValue(ControllerHardwareVersionValue);
	SharedVariable_WriteTestInterfaceHardwareVersionValue(InterfaceHardwareVersionValue);
	SharedVariable_WriteTestControllerAssemblyVariantValue(ControllerAssemblyVariantValue);
	SharedVariable_WriteTestInterfaceAssemblyVariantValue(InterfaceAssemblyVariantValue);
	SharedVariable_WriteTestModule1HardwareVersionValue(Module1HardwareVersionValue);
	SharedVariable_WriteTestModule2HardwareVersionValue(Module2HardwareVersionValue);
	SharedVariable_WriteTestModule3HardwareVersionValue(Module3HardwareVersionValue);
	SharedVariable_WriteTestModule4HardwareVersionValue(Module4HardwareVersionValue);
	SharedVariable_WriteTestModule1AssemblyVariantValue(Module1AssemblyVariantValue);
	SharedVariable_WriteTestModule2AssemblyVariantValue(Module2AssemblyVariantValue);
	SharedVariable_WriteTestModule3AssemblyVariantValue(Module3AssemblyVariantValue);
	SharedVariable_WriteTestModule4AssemblyVariantValue(Module4AssemblyVariantValue);
	SharedVariable_WriteTestModule1Value(Module1Value);
	SharedVariable_WriteTestModule2Value(Module2Value);
	SharedVariable_WriteTestModule3Value(Module3Value);
	SharedVariable_WriteTestModule4Value(Module4Value);
	
	SharedVariable_EndNetworkvariableTestHardware();
}
--
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Führt die Testprozedur aus.
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
void Test_TestProcedure(void)
{
	//Network Variable initialisieren
	if(TestState == ERRORLESS)	SharedVariable_InitNetworkvariableTTIC2();

	//TestReturnValue Status auf Running setzen
	SharedVariable_WriteTestReturnValue("Running");	

	//schreibt den Projektnamen in eine Globalen String
	if(TestState == ERRORLESS)	HelpFunctions_GetProjectPathName();

	//Wenn Hardware vorhanden, wird der Power Stecker initiallisiert
	TestEnvironment_InitPowerConnector();
	
	//Testinformationen in Report schreiben
	if(TestState == ERRORLESS)	TestEnvironment_ValveIdentificationInReport();
	
	//HomingProcessTime und PositionProcessTime
	if(TestState == ERRORLESS)	TestEnvironment_InitTestVariable();
	
	//Aufruf der Testfunktion
	if(TestState == ERRORLESS)	Test();
	
	//Werte wider zurücksetzen
	if(TestState == ERRORLESS)																																						HelpFunctions_RestoreParameters(USER_RESTORE_PARAMETERS);
	else if(strlen(TestOptionsString) != 0 && TestOptionsString[TERMINATE_BY_ERROR_OPTION_INDEX] == '0')	HelpFunctions_RestoreParameters(USER_RESTORE_PARAMETERS);

	//Auswertung Test Procedur
	TestEnvironment_ReportTest();
	
	//Stellt ein Zertifikat aus, wenn der Test erfolgreich durchgeführt worden ist
	TestEnvironment_TestCertificate();
	
	//Report File wieder schliessen
	GenerateReport_CloseFile();
	//LogFile wider schliessen
	fclose(LogFile_Test);
	
	//Den Port schliessen
	UsbDisconnect(&UsbReferece, &LogReference);
	UsbConnected = 0;
	
	//DeviceNet Kommunikation beenden
	if(DeviceNetConnected == TRUE) 
	{
		InterfaceCommunication_DeviceNetDisconnectSlave();
		free(pReceiveDataAddres);
	}
	DeviceNetConnected = FALSE;

	//Network Variable wider freigeben 
	SharedVariable_EndNetworkvariable();
#if !defined(PLATFORMCONFIG_INTERFACE_ETHERCAT_NA) && defined(TEST_ETHERCAT)
	PDO_SDOSharedVariable_EndNetworkvariable();
#endif

	//Beendet den Power Connector Speisung
	TestEnvironment_TerminatePowerConnector();
}  //end function

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ruft die verschidenen Testroutinen auf
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
static void Test(void)
{
	AccessMode();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Prüft den Access Mode
//  Input           : -
//  Output *Pointer : -
//         *return  :	-
//******************************************************************************************************************************
static void AccessMode(void)
{
	char ReportMessage[REPORT_LENGTH]; 
	
	//Prüfen ob der Remote Mode gesetzt werden kann
	HelpFunctions_RemoteMode();
	Delay(Parameter.System.Identification.Configuration.TaskCycleTime.Value);
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, SERVICE, "Access Mode abfragen");
	if(Parameter.System.AccessMode.Value != AM_REMOTE)
	{
		sprintf(ReportMessage, "Access Mode konnte nicht in Remote versetzt werden: erhalten %d, erwartet: %d", Parameter.System.AccessMode.Value, AM_REMOTE);
		GenerateReport_AddFail(ReportMessage);
	}
	else	GenerateReport_AddPass("Konnte Access Mode auf Remote setzen");
	
	//Prüfen ob der Local Mode gesetzt werden kann
	HelpFunctions_LocalMode();
	Delay(Parameter.System.Identification.Configuration.TaskCycleTime.Value);
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, SERVICE, "Access Mode abfragen");
	if(Parameter.System.AccessMode.Value != AM_LOCAL)
	{
		sprintf(ReportMessage, "Access Mode konnte nicht in Local versetzt werden: erhalten %d, erwartet: %d", Parameter.System.AccessMode.Value, AM_LOCAL);
		GenerateReport_AddFail(ReportMessage);
	}
	else	GenerateReport_AddPass("Konnte Access Mode auf Local setzen");
	
	//Wenn RS232 Interface vorhanden ist
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.Identification.Hardware.InterfaceType, SERVICE, "Interface Type abfragen");
	if(Parameter.System.Identification.Hardware.InterfaceType.Value == IT_RS232_RS485)
	{
		//Remote Locked Mode setzen
		Command_c01_SetAccessMode(AM_REMOTELOCKED, RS232INTERFACE);
		Delay(Parameter.System.Identification.Configuration.TaskCycleTime.Value);
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, SERVICE, "Access Mode abfragen");
		if(Parameter.System.AccessMode.Value != AM_REMOTELOCKED)
		{
			sprintf(ReportMessage, "Access Mode konnte nicht in Remote Locked versetzt werden: erhalten %d, erwartet: %d", Parameter.System.AccessMode.Value, AM_REMOTELOCKED);
			GenerateReport_AddFail(ReportMessage);
		}
		else	GenerateReport_AddPass("Konnte Access Mode auf Remote Locked setzen");
		
		//Wieder in Remote Mode gehen
		Command_c01_SetAccessMode(AM_REMOTE, RS232INTERFACE);
		Delay(Parameter.System.Identification.Configuration.TaskCycleTime.Value);
		HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, SERVICE, "Access Mode abfragen");
		if(Parameter.System.AccessMode.Value != AM_REMOTE)
		{
			sprintf(ReportMessage, "Access Mode konnte nicht aus Remote Locked in Remote versetzt werden: erhalten %d, erwartet: %d", Parameter.System.AccessMode.Value, AM_REMOTE);
			GenerateReport_AddFail(ReportMessage);
		}
		else	GenerateReport_AddPass("Access Mode konnte aus Remote Locked in Remote versetzt werden");
	}
	
	//Nach einem Neustart muss das Ventil im Remote Mode sein
	HelpFunctions_RestartValve(SERVICE);
	Delay(Parameter.System.Identification.Configuration.TaskCycleTime.Value);
	HelpFunctions_GetCommand((struct ParameterType_GENERAL_INFORMATION *) &Parameter.System.AccessMode, SERVICE, "Access Mode abfragen");
	if(Parameter.System.AccessMode.Value != AM_REMOTE)
	{
		sprintf(ReportMessage, "Access Mode ist nach einem Restart nicht im Remote Mode: erhalten %d, erwartet: %d", Parameter.System.AccessMode.Value, AM_REMOTE);
		GenerateReport_AddFail(ReportMessage);
	}
	else	GenerateReport_AddPass("Access Mode ist nach einem Restart im Remote Mode");
}
