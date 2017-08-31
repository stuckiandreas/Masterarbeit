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
#include <userint.h>
#include <cvinetv.h>
#include <formatio.h>
#include <rs232.h>
#include <stdint.h>
#include <stdio.h>
#include <utility.h>

#include "GenerateReport.h"
#include "HelpFunctions.h" 
#include "SharedVariable.h"
#include "Test.h" 
#include "TestEnvironment.h"
#include "Typedefinitions.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestOptions (void * handle, CNVData DataTestOptions, void * callbackData);
static void CVICALLBACK DataCallbackTestStatus (void * handle, CNVData DataTestStatus, void * callbackData);
static void CVICALLBACK StatusCallback (void * handle, CNVConnectionStatus status, int error, void * callbackData);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
char ReceiveTestStatus[TEST_STATUS_LENGTH], ReceiveTestOptions[OPTIONS_MAX_LENGTH];
CNVData	data = 0;
CNVWriter				wReturnValue, wTestReport, wTestVersionValue, wTestNameValue, wTestDescriptionValue, wTestDescriptionValue;
CNVWriter				wCreationTestValue, wCreationAuthorValue, wTestDuratingValue, wTestModulNameValue, wTestExternalIsolationValveOptionValue;
CNVWriter 			wTestValveSerieValue, wTestControllerValue, wTestInterfaceValue, wTestOptionValue, wTestHardwareValue;
CNVWriter 			wTestControllerHardwareVersionValue, wTestInterfaceHardwareVersionValue, wTestModule1HardwareVersionValue, wTestModule2HardwareVersionValue;
CNVWriter 			wTestModule3HardwareVersionValue, wTestModule4HardwareVersionValue, wTestControllerAssemblyVariantValue, wTestInterfaceAssemblyVariantValue;
CNVWriter 			wTestModule1AssemblyVariantValue, wTestModule2AssemblyVariantValue, wTestModule3AssemblyVariantValue, wTestModule4AssemblyVariantValue;
CNVWriter 			wTestModule1Value, wTestModule2Value, wTestModule3Value, wTestModule4Value, wTestModificationValue, wTTIC2TestVersionValue;
CNVSubscriber		sTestStatus, sTestOptions;



/*TTIC2 Programm*/
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Initialisiert alle Umgebungsvariablen für das TTIC2 Programm
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_InitNetworkvariableTTIC2 (void)
{
	int ErrorSharedVariableTestStatus, ErrorSharedVariableTestOptions;
	char VariableTestStatus[MAX_PATHNAME_LEN], VariableReturnValue[MAX_PATHNAME_LEN], VariableTestReport[MAX_PATHNAME_LEN], VariableTestOptions[MAX_PATHNAME_LEN];

	//Die Umgebungsvariable TestStatus, welche von der Oberfläche geschriben wird
	VariableTestStatus[0] = 0;
	strcat(VariableTestStatus, "\\\\localhost\\System\\TestStatus");
	if (VariableTestStatus[0] == '\\')
	{		
  	ErrorSharedVariableTestStatus = CNVCreateSubscriber (VariableTestStatus, DataCallbackTestStatus, StatusCallback, NULL, 10000, 0, &sTestStatus);
		if (ErrorSharedVariableTestStatus < 0)	MessagePopup ("ErrorSharedVariableTestStatus", CNVGetErrorDescription(ErrorSharedVariableTestStatus));
	}
	
	//Die Umgebungsvariable TestOptions, welche die Wünsche von der Oberfläche entgegen nimmt (Index 0: Info Funktion Test ausführen, Index 1: Pass Commands im Report anzeigen, Index 2: Sollen die Parameter nach dem Test zurückgesetzt werden)
	VariableTestOptions[0] = 0;
	strcat(VariableTestOptions, "\\\\localhost\\System\\TestOptions");
	if (VariableTestOptions[0] == '\\')
	{		
  	ErrorSharedVariableTestOptions = CNVCreateSubscriber (VariableTestOptions, DataCallbackTestOptions, StatusCallback, NULL, 10000, 0, &sTestOptions);
		if (ErrorSharedVariableTestOptions < 0)	MessagePopup ("ErrorSharedVariableTestOptions", CNVGetErrorDescription(ErrorSharedVariableTestOptions));
	}
	
	//Die Umgebungsvariable Returnvalue, welche der Oberfläche das Ergebnis zurück gibt 
	VariableReturnValue[0] = 0;
	strcat(VariableReturnValue, "\\\\localhost\\System\\TestReturnValue");
	CNVCreateWriter (VariableReturnValue, NULL, 0,10000, 0, &wReturnValue); 
	
	//Die Umgebungsvariable TestReport, welche der Oberfläche dauernt die Testergebnisse liefert
	VariableTestReport[0] = 0;
	strcat(VariableTestReport, "\\\\localhost\\System\\TestReport");
	CNVCreateWriter (VariableTestReport, NULL, 0,10000, 0, &wTestReport); 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beendet alle Umgebungsvariablen unter System
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_EndNetworkvariable (void)
{
	CNVDispose (sTestStatus);
	CNVDispose (sTestOptions);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallback (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
    //MessagePopup ("Error Connecting Shared Variable TestStatus", CNVGetErrorDescription(error));
		GenerateReport_AddText("Fehler bei der Verbindung Variable TestStatus im Test\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der TestStatus Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestStatus (void * handle, CNVData DataTestStatus, void * callbackData)
{
	char *pReceiveTestStatusPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestStatus == 0)	return;
	
	CNVGetDataType (DataTestStatus, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestStatus, type, &pReceiveTestStatusPointer);
		strcpy(ReceiveTestStatus, pReceiveTestStatusPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der TestOptions Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestOptions (void * handle, CNVData DataTestOptions, void * callbackData)
{
	char *ReceiveTestOptionsPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestOptions == 0)	return;
	
	CNVGetDataType (DataTestOptions, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestOptions, type, &ReceiveTestOptionsPointer);
		strcpy(ReceiveTestOptions, ReceiveTestOptionsPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Funktion, welche eine Message auf die TestReport Umgebungsvariable schreibt
//  Input           : message: 	String, welche auf die Umgebungsvariable geschrieben werden sollte
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_AddReport(char *Message)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, Message);
	CNVWrite (wTestReport, data, REPORT_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die TestReturnValue Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestReturnValue(char *ReturnValue)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, ReturnValue);
	CNVWrite (wReturnValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Fragt die Umgebungsvariable ab ob der User den Test unterbrochen hat  
//  Input           : 
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_GetStateUser(void)
{
	if(ReceiveTestStatus != NULL && strlen(ReceiveTestStatus) > 0)
	{
		if(strcmp("Cancel", ReceiveTestStatus) == 0)
		{	
			TestState = USERCANCELLED;
		}
	}
}



//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beendet alle Umgebungsvariablen für das TestUpdateFirmware bezüglich der Test Hardware Informationen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_EndNetworkvariableTTIC2TestVersion (void)
{
	CNVDisposeData(data);
	CNVDispose(wTTIC2TestVersionValue);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Initialisiert alle Umgebungsvariablen für das TestUpdateFirmware bezüglich der Test Hardware Informationen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_InitNetworkvariableTTIC2TestVersion (void)
{	 
	char VariableTestVersion[MAX_PATHNAME_LEN];
	
	//Die Umgebungsvariable Version, welche dem TestUpdateFirmware Programm zur Verfügung stellt 
	VariableTestVersion[0] = 0;
	strcat(VariableTestVersion, "\\\\localhost\\TTIC2_SharedVariable\\TestVersion");
	CNVCreateWriter (VariableTestVersion, NULL, 0, 10000, 0, &wTTIC2TestVersionValue); 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Test Version Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTTIC2TestVersionValue(SINT16 TestVersion)
{
	CNVCreateScalarDataValue(&data, CNVInt16, TestVersion);
	CNVWrite (wTTIC2TestVersionValue, data, RETURNVALUE_LENGTH);
}






+++
/*Test Hardware*/
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Initialisiert alle Umgebungsvariablen für das TestUpdateFirmware bezüglich der Test Hardware Informationen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_InitNetworkvariableTestHardware(void)
{
	char VariableName[MAX_PATHNAME_LEN], VariableDescription[MAX_PATHNAME_LEN], VariableCreationDate[MAX_PATHNAME_LEN], VariableAuthor[MAX_PATHNAME_LEN]; 
	char VariableVersion[MAX_PATHNAME_LEN], VariableDurating[MAX_PATHNAME_LEN], VariableModulName[MAX_PATHNAME_LEN], VariableExternalIsolationValveOption[MAX_PATHNAME_LEN];
	char VariableValveSerie[MAX_PATHNAME_LEN], VariableController[MAX_PATHNAME_LEN], VariableInterface[MAX_PATHNAME_LEN], VariableOption[MAX_PATHNAME_LEN], VariableModification[MAX_PATHNAME_LEN];
	char VariableTestHardware[MAX_PATHNAME_LEN], VariableTestControllerHardwareVersion[MAX_PATHNAME_LEN], VariableTestInterfaceHardwareVersion[MAX_PATHNAME_LEN];
	char VariableTestModule1HardwareVersion[MAX_PATHNAME_LEN], VariableTestModule2HardwareVersion[MAX_PATHNAME_LEN], VariableTestModule3HardwareVersion[MAX_PATHNAME_LEN];
	char VariableTestModule4HardwareVersion[MAX_PATHNAME_LEN], VariableTestControllerAssemblyVariant[MAX_PATHNAME_LEN], VariableTestInterfaceAssemblyVariant[MAX_PATHNAME_LEN];
	char VariableTestModule1AssemblyVariant[MAX_PATHNAME_LEN], VariableTestModule2AssemblyVariant[MAX_PATHNAME_LEN], VariableTestModule3AssemblyVariant[MAX_PATHNAME_LEN], VariableTestModule4AssemblyVariant[MAX_PATHNAME_LEN];
	char VariableTestModule1[MAX_PATHNAME_LEN], VariableTestModule2[MAX_PATHNAME_LEN], VariableTestModule3[MAX_PATHNAME_LEN], VariableTestModule4[MAX_PATHNAME_LEN];
	
	//Die Umgebungsvariable Name, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableName[0] = 0;
	strcat(VariableName, "\\\\localhost\\TestInformation_SharedVariable\\Name");
	CNVCreateWriter (VariableName, NULL, 0, 10000, 0, &wTestNameValue);
	
	//Die Umgebungsvariable Description, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableDescription[0] = 0;
	strcat(VariableDescription, "\\\\localhost\\TestInformation_SharedVariable\\Description");
	CNVCreateWriter (VariableDescription, NULL, 0, 10000, 0, &wTestDescriptionValue); 
	
	//Die Umgebungsvariable CreationDate, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableCreationDate[0] = 0;
	strcat(VariableCreationDate, "\\\\localhost\\TestInformation_SharedVariable\\CreationDate");
	CNVCreateWriter (VariableCreationDate, NULL, 0, 10000, 0, &wCreationTestValue); 
	
	//Die Umgebungsvariable Author, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableAuthor[0] = 0;
	strcat(VariableAuthor, "\\\\localhost\\TestInformation_SharedVariable\\Author");
	CNVCreateWriter (VariableAuthor, NULL, 0, 10000, 0, &wCreationAuthorValue); 
	
	//Die Umgebungsvariable Version, welche dem TestUpdateFirmware Programm zur Verfügung stellt 
	VariableVersion[0] = 0;
	strcat(VariableVersion, "\\\\localhost\\TestInformation_SharedVariable\\Version");
	CNVCreateWriter (VariableVersion, NULL, 0, 10000, 0, &wTestVersionValue); 
	
	//Die Umgebungsvariable Modification, welche dem TestUpdateFirmware Programm zur Verfügung stellt 
	VariableModification[0] = 0;
	strcat(VariableModification, "\\\\localhost\\TestInformation_SharedVariable\\Modification");
	CNVCreateWriter (VariableModification, NULL, 0, 10000, 0, &wTestModificationValue); 

	//Die Umgebungsvariable TestDurating, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableDurating[0] = 0;
	strcat(VariableDurating, "\\\\localhost\\TestInformation_SharedVariable\\Durating");
	CNVCreateWriter (VariableDurating, NULL, 0, 10000, 0, &wTestDuratingValue); 
	
	//Die Umgebungsvariable ModulName, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableModulName[0] = 0;
	strcat(VariableModulName, "\\\\localhost\\TestInformation_SharedVariable\\ModulName");
	CNVCreateWriter (VariableModulName, NULL, 0, 10000, 0, &wTestModulNameValue); 
	
	//Die Umgebungsvariable ExternalIsolationValveOption, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableExternalIsolationValveOption[0] = 0;
	strcat(VariableExternalIsolationValveOption, "\\\\localhost\\TestInformation_SharedVariable\\ExternalIsolationValveOption");
	CNVCreateWriter (VariableExternalIsolationValveOption, NULL, 0, 10000, 0, &wTestExternalIsolationValveOptionValue); 
	
	//Die Umgebungsvariable ValveSerie, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableValveSerie[0] = 0;
	strcat(VariableValveSerie, "\\\\localhost\\TestInformation_SharedVariable\\ValveSerie");
	CNVCreateWriter (VariableValveSerie, NULL, 0, 10000, 0, &wTestValveSerieValue); 
	
	//Die Umgebungsvariable Controller, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableController[0] = 0;
	strcat(VariableController, "\\\\localhost\\TestInformation_SharedVariable\\Controller");
	CNVCreateWriter (VariableController, NULL, 0, 10000, 0, &wTestControllerValue); 

	//Die Umgebungsvariable Interface, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableInterface[0] = 0;
	strcat(VariableInterface, "\\\\localhost\\TestInformation_SharedVariable\\Interface");
	CNVCreateWriter (VariableInterface, NULL, 0, 10000, 0, &wTestInterfaceValue); 
	
	//Die Umgebungsvariable Option, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableOption[0] = 0;
	strcat(VariableOption, "\\\\localhost\\TestInformation_SharedVariable\\Option");
	CNVCreateWriter (VariableOption, NULL, 0, 10000, 0, &wTestOptionValue); 
	
	//Die Umgebungsvariable TestHardware, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestHardware[0] = 0;
	strcat(VariableTestHardware, "\\\\localhost\\TestInformation_SharedVariable\\TestHardware");
	CNVCreateWriter (VariableTestHardware, NULL, 0, 10000, 0, &wTestHardwareValue); 
	
	//Die Umgebungsvariable ControllerHardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestControllerHardwareVersion[0] = 0;
	strcat(VariableTestControllerHardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\ControllerHardwareVersion");
	CNVCreateWriter (VariableTestControllerHardwareVersion, NULL, 0, 10000, 0, &wTestControllerHardwareVersionValue); 
	
	//Die Umgebungsvariable InterfaceHardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestInterfaceHardwareVersion[0] = 0;
	strcat(VariableTestInterfaceHardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\InterfaceHardwareVersion");
	CNVCreateWriter (VariableTestInterfaceHardwareVersion, NULL, 0, 10000, 0, &wTestInterfaceHardwareVersionValue); 

	//Die Umgebungsvariable Module1HardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule1HardwareVersion[0] = 0;
	strcat(VariableTestModule1HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module1HardwareVersion");
	CNVCreateWriter (VariableTestModule1HardwareVersion, NULL, 0, 10000, 0, &wTestModule1HardwareVersionValue); 
	
	//Die Umgebungsvariable Module2HardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule2HardwareVersion[0] = 0;
	strcat(VariableTestModule2HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module2HardwareVersion");
	CNVCreateWriter (VariableTestModule2HardwareVersion, NULL, 0, 10000, 0, &wTestModule2HardwareVersionValue); 
	
	//Die Umgebungsvariable Module3HardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule3HardwareVersion[0] = 0;
	strcat(VariableTestModule3HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module3HardwareVersion");
	CNVCreateWriter (VariableTestModule3HardwareVersion, NULL, 0, 10000, 0, &wTestModule3HardwareVersionValue); 
	
	//Die Umgebungsvariable Module4HardwareVersion, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule4HardwareVersion[0] = 0;
	strcat(VariableTestModule4HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module4HardwareVersion");
	CNVCreateWriter (VariableTestModule4HardwareVersion, NULL, 0, 10000, 0, &wTestModule4HardwareVersionValue); 
	
	//Die Umgebungsvariable ControllerAssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestControllerAssemblyVariant[0] = 0;
	strcat(VariableTestControllerAssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\ControllerAssemblyVariant");
	CNVCreateWriter (VariableTestControllerAssemblyVariant, NULL, 0, 10000, 0, &wTestControllerAssemblyVariantValue); 
	
	//Die Umgebungsvariable InterfaceAssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestInterfaceAssemblyVariant[0] = 0;
	strcat(VariableTestInterfaceAssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\InterfaceAssemblyVariant");
	CNVCreateWriter (VariableTestInterfaceAssemblyVariant, NULL, 0, 10000, 0, &wTestInterfaceAssemblyVariantValue); 
	
	//Die Umgebungsvariable Module1AssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule1AssemblyVariant[0] = 0;
	strcat(VariableTestModule1AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module1AssemblyVariant");
	CNVCreateWriter (VariableTestModule1AssemblyVariant, NULL, 0, 10000, 0, &wTestModule1AssemblyVariantValue); 
	
	//Die Umgebungsvariable Module2AssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule2AssemblyVariant[0] = 0;
	strcat(VariableTestModule2AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module2AssemblyVariant");
	CNVCreateWriter (VariableTestModule2AssemblyVariant, NULL, 0, 10000, 0, &wTestModule2AssemblyVariantValue); 
	
	//Die Umgebungsvariable Module3AssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule3AssemblyVariant[0] = 0;
	strcat(VariableTestModule3AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module3AssemblyVariant");
	CNVCreateWriter (VariableTestModule3AssemblyVariant, NULL, 0, 10000, 0, &wTestModule3AssemblyVariantValue); 
	
	//Die Umgebungsvariable Module4AssemblyVariant, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule4AssemblyVariant[0] = 0;
	strcat(VariableTestModule4AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module4AssemblyVariant");
	CNVCreateWriter (VariableTestModule4AssemblyVariant, NULL, 0, 10000, 0, &wTestModule4AssemblyVariantValue); 
	
	//Die Umgebungsvariable Module1, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule1[0] = 0;
	strcat(VariableTestModule1, "\\\\localhost\\TestInformation_SharedVariable\\Module1");
	CNVCreateWriter (VariableTestModule1, NULL, 0, 10000, 0, &wTestModule1Value); 
	
	//Die Umgebungsvariable Module2, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule2[0] = 0;
	strcat(VariableTestModule2, "\\\\localhost\\TestInformation_SharedVariable\\Module2");
	CNVCreateWriter (VariableTestModule2, NULL, 0, 10000, 0, &wTestModule2Value); 
	
	//Die Umgebungsvariable Module3, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule3[0] = 0;
	strcat(VariableTestModule3, "\\\\localhost\\TestInformation_SharedVariable\\Module3");
	CNVCreateWriter (VariableTestModule3, NULL, 0, 10000, 0, &wTestModule3Value); 
	
	//Die Umgebungsvariable Module4, welche dem TestUpdateFirmware Programm zur Verfügung stellt
	VariableTestModule4[0] = 0;
	strcat(VariableTestModule4, "\\\\localhost\\TestInformation_SharedVariable\\Module4");
	CNVCreateWriter (VariableTestModule4, NULL, 0, 10000, 0, &wTestModule4Value); 
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beendet die Umgebungsvariable zur Abspeicherung der Test Hardware Informationen
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_EndNetworkvariableTestHardware(void)
{
	CNVDisposeData(data);
	CNVDispose(wTestNameValue);
	CNVDispose(wTestDescriptionValue);
	CNVDispose(wCreationTestValue);
	CNVDispose(wCreationAuthorValue);
	CNVDispose(wTestVersionValue);
	CNVDispose(wTestModificationValue);
	CNVDispose(wTestDuratingValue);
	CNVDispose(wTestModulNameValue);
	CNVDispose(wTestExternalIsolationValveOptionValue);
	CNVDispose(wTestValveSerieValue);
	CNVDispose(wTestControllerValue);
	CNVDispose(wTestInterfaceValue);
	CNVDispose(wTestOptionValue);
	CNVDispose(wTestHardwareValue);
	CNVDispose(wTestControllerHardwareVersionValue);
	CNVDispose(wTestInterfaceHardwareVersionValue);
	CNVDispose(wTestModule1HardwareVersionValue);
	CNVDispose(wTestModule2HardwareVersionValue);
	CNVDispose(wTestModule3HardwareVersionValue);
	CNVDispose(wTestModule4HardwareVersionValue);
	CNVDispose(wTestControllerAssemblyVariantValue);
	CNVDispose(wTestInterfaceAssemblyVariantValue);
	CNVDispose(wTestModule1AssemblyVariantValue);
	CNVDispose(wTestModule2AssemblyVariantValue);
	CNVDispose(wTestModule3AssemblyVariantValue);
	CNVDispose(wTestModule4AssemblyVariantValue);
	CNVDispose(wTestModule1Value);
	CNVDispose(wTestModule2Value);
	CNVDispose(wTestModule3Value);
	CNVDispose(wTestModule4Value);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Name Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestNameValue(char *pTestName)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pTestName);
	CNVWrite (wTestNameValue, data, TEST_NAME_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Description Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestDescriptionValue(char *pTestDescription)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pTestDescription);
	CNVWrite (wTestDescriptionValue, data, TEST_DESCRIPTION_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die CreationDate Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteCreationTestValue(char *pCreationTest)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pCreationTest);
	CNVWrite (wCreationTestValue, data, CREATION_TEST_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Author Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteCreationAuthorValue(char *pCreationAuthor)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pCreationAuthor);
	CNVWrite (wCreationAuthorValue, data, CREATION_AUTHOR_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Version Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestVersionValue(SINT16 TestVersion)
{
	CNVCreateScalarDataValue(&data, CNVInt16, TestVersion);
	CNVWrite (wTestVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Modification Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModificationValue(char *pTestModification)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pTestModification);
	CNVWrite (wTestModificationValue, data, TEST_MODIFICATION_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Author Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestDuratingValue(char *pTestDurating)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pTestDurating);
	CNVWrite (wTestDuratingValue, data, TEST_DURATION_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die ModulName Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModulNameValue(char *pModulName)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVString, pModulName);
	CNVWrite (wTestModulNameValue, data, TEST_MODUL_NAME_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die ExternalIsolationValveOption Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestExternalIsolationValveOptionValue(SINT16 ExternalIsolationValveOption)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, ExternalIsolationValveOption);
	CNVWrite (wTestExternalIsolationValveOptionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die ValveSerie Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestValveSerieValue(SINT32 ValveSerie)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt32, ValveSerie);
	CNVWrite (wTestValveSerieValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Controller Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestControllerValue(SINT32 Controller)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt32, Controller);
	CNVWrite (wTestControllerValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Interface Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestInterfaceValue(SINT32 Interface)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt32, Interface);
	CNVWrite (wTestInterfaceValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Option Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestOptionValue(SINT16 Option)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Option);
	CNVWrite (wTestOptionValue, data, RETURNVALUE_LENGTH);
}


//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die TestHardware Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestHardwareValue(SINT16 TestHardware)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, TestHardware);
	CNVWrite (wTestHardwareValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die ControllerHardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestControllerHardwareVersionValue(SINT16 ControllerHardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, ControllerHardwareVersion);
	CNVWrite (wTestControllerHardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die InterfaceHardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestInterfaceHardwareVersionValue(SINT16 InterfaceHardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, InterfaceHardwareVersion);
	CNVWrite (wTestInterfaceHardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module1HardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule1HardwareVersionValue(SINT16 Module1HardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module1HardwareVersion);
	CNVWrite (wTestModule1HardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module2HardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule2HardwareVersionValue(SINT16 Module2HardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module2HardwareVersion);
	CNVWrite (wTestModule2HardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module3HardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule3HardwareVersionValue(SINT16 Module3HardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module3HardwareVersion);
	CNVWrite (wTestModule3HardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module4HardwareVersion Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule4HardwareVersionValue(SINT16 Module4HardwareVersion)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module4HardwareVersion);
	CNVWrite (wTestModule4HardwareVersionValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die ControllerAssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestControllerAssemblyVariantValue(SINT16 ControllerAssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, ControllerAssemblyVariant);
	CNVWrite (wTestControllerAssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die InterfaceAssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestInterfaceAssemblyVariantValue(SINT16 InterfaceAssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, InterfaceAssemblyVariant);
	CNVWrite (wTestInterfaceAssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module1AssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule1AssemblyVariantValue(SINT16 Module1AssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module1AssemblyVariant);
	CNVWrite (wTestModule1AssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module2AssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule2AssemblyVariantValue(SINT16 Module2AssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module2AssemblyVariant);
	CNVWrite (wTestModule2AssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module3AssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule3AssemblyVariantValue(SINT16 Module3AssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module3AssemblyVariant);
	CNVWrite (wTestModule3AssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module4AssemblyVariant Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule4AssemblyVariantValue(SINT16 Module4AssemblyVariant)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module4AssemblyVariant);
	CNVWrite (wTestModule4AssemblyVariantValue, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module1 Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule1Value(SINT16 Module1)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module1);
	CNVWrite (wTestModule1Value, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module2 Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule2Value(SINT16 Module2)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module2);
	CNVWrite (wTestModule2Value, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module3 Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule3Value(SINT16 Module3)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module3);
	CNVWrite (wTestModule3Value, data, RETURNVALUE_LENGTH);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Schreibt auf die Module4 Umgebungsvariable  
//  Input           : ReturnValue
//  Output *Pointer : -
//         *return  : - 
//******************************************************************************************************************************
void SharedVariable_WriteTestModule4Value(SINT16 Module4)
{
	CNVData	data = 0;
	
	CNVCreateScalarDataValue(&data, CNVInt16, Module4);
	CNVWrite (wTestModule4Value, data, RETURNVALUE_LENGTH);
}
---