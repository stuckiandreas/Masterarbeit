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
//Because of name conflicts, Windows headers must be included before utility.h and formatio.h
#include <windows.h>
#include <userint.h>
#include <cvinetv.h>
#include <formatio.h>
#include <rs232.h>
#include <stdint.h>
#include <stdio.h>
#include <utility.h>

#include "SharedVariable.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
/*Test Hardware*/
static void CVICALLBACK DataCallbackTestName (void * handle, CNVData DataTestName, void * callbackData);
static void CVICALLBACK DataCallbackTestDescription (void * handle, CNVData DataTestDescription, void * callbackData);
static void CVICALLBACK DataCallbackCreationTest (void * handle, CNVData DataCreationTest, void * callbackData);
static void CVICALLBACK DataCallbackCreationAuthor (void * handle, CNVData DataCreationAuthor, void * callbackData);
static void CVICALLBACK DataCallbackTestModification (void * handle, CNVData DataTestModification, void * callbackData);
static void CVICALLBACK DataCallbackTestVersion (void * handle, CNVData DataTestVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestDurating (void * handle, CNVData DataTestDurating, void * callbackData);
static void CVICALLBACK DataCallbackTestModulName (void * handle, CNVData DataTestModulName, void * callbackData);
static void CVICALLBACK DataCallbackTestExternalIsolationValveOption (void * handle, CNVData DataExternalIsolationValveOption, void * callbackData);
static void CVICALLBACK DataCallbackTestValveSerie (void * handle, CNVData DataValveSerie, void * callbackData);
static void CVICALLBACK DataCallbackTestController (void * handle, CNVData DataTestController, void * callbackData);
static void CVICALLBACK DataCallbackTestInterface (void * handle, CNVData DataTestInterface, void * callbackData);
static void CVICALLBACK DataCallbackTestOption (void * handle, CNVData DataTestOption, void * callbackData);
static void CVICALLBACK DataCallbackTestHardware (void * handle, CNVData DataTestHardware, void * callbackData);
static void CVICALLBACK DataCallbackTestControllerHardwareVersion (void * handle, CNVData DataTestControllerHardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestInterfaceHardwareVersion (void * handle, CNVData DataTestInterfaceHardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestModule1HardwareVersion (void * handle, CNVData DataTestModule1HardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestModule2HardwareVersion (void * handle, CNVData DataTestModule2HardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestModule3HardwareVersion (void * handle, CNVData DataTestModule3HardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestModule4HardwareVersion (void * handle, CNVData DataTestModule4HardwareVersion, void * callbackData);
static void CVICALLBACK DataCallbackTestControllerAssemblyVariant (void * handle, CNVData DataTestControllerAssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestInterfaceAssemblyVariant (void * handle, CNVData DataTestInterfaceAssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestModule1AssemblyVariant (void * handle, CNVData DataTestModule1AssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestModule2AssemblyVariant (void * handle, CNVData DataTestModule2AssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestModule3AssemblyVariant (void * handle, CNVData DataTestModule3AssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestModule4AssemblyVariant (void * handle, CNVData DataTestModule4AssemblyVariant, void * callbackData);
static void CVICALLBACK DataCallbackTestModule1 (void * handle, CNVData DataTestModule1, void * callbackData);
static void CVICALLBACK DataCallbackTestModule2 (void * handle, CNVData DataTestModule2, void * callbackData);
static void CVICALLBACK DataCallbackTestModule3 (void * handle, CNVData DataTestModule3, void * callbackData);
static void CVICALLBACK DataCallbackTestModule4 (void * handle, CNVData DataTestModule4, void * callbackData);
static void CVICALLBACK StatusCallbackTestName (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestDescription (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackCreationTest (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackCreationAuthor (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModification (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestDurating (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModulName (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestExternalIsolationValveOption (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestValveSerie (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestController (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestInterface (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestOption (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestHardware (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestControllerHardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestInterfaceHardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule1HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule2HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule3HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule4HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestControllerAssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestInterfaceAssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule1AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule2AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule3AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule4AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule1 (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule2 (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule3 (void * handle, CNVConnectionStatus status, int error, void * callbackData);
static void CVICALLBACK StatusCallbackTestModule4 (void * handle, CNVConnectionStatus status, int error, void * callbackData);

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
SINT16 TestVersion;
SINT16 TestOption, TestHardware, TestControllerHardwareVersion, TestInterfaceHardwareVersion, TestModule1HardwareVersion, TestModule2HardwareVersion;
SINT16 TestModule3HardwareVersion, TestModule4HardwareVersion, TestControllerAssemblyVariant, TestInterfaceAssemblyVariant, TestModule1AssemblyVariant;
SINT16 TestModule2AssemblyVariant, TestModule3AssemblyVariant, TestModule4AssemblyVariant, TestModule1, TestModule2, TestModule3, TestModule4, TestExternalIsolationValveOption;
SINT32 TestValveSerie, TestController, TestInterface;
char ReceiveTestName[TEST_NAME_LENGTH], ReceiveTestDescription[TEST_DESCRIPTION_LENGTH], ReceiveCreationTest[CREATION_TEST_LENGTH], ReceiveCreationAuthor[CREATION_AUTHOR_LENGTH];
char ReceiveTestModification[TEST_MODIFICATION_LENGTH], ReceiveTestDurating[TEST_DURATION_LENGTH], ReceiveTestModulName[TEST_MODUL_NAME_LENGTH];
CNVSubscriber	sTestName, sTestDescription, sCreationTest, sCreationAuthor, sTestVersion, sTestDurating, sTestModulName, sTestExternalIsolationValveOption;
CNVSubscriber	sTestValveSerie, sTestModification, sTestController, sTestInterface, sTestOption, sTestHardware, sTestControllerHardwareVersion, sTestInterfaceHardwareVersion;	
CNVSubscriber	sTestModule1HardwareVersion, sTestModule2HardwareVersion, sTestModule3HardwareVersion, sTestModule4HardwareVersion, sTestControllerAssemblyVariant;
CNVSubscriber	sTestInterfaceAssemblyVariant, sTestModule1AssemblyVariant, sTestModule2AssemblyVariant, sTestModule3AssemblyVariant, sTestModule4AssemblyVariant;
CNVSubscriber	sTestModule1, sTestModule2, sTestModule3, sTestModule4;

/*Test Hardware*/
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Initialisiert die Umgebungsvariablen um die Test Hardware Anforderungen abspeichern zu können. 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_InitNetworkvariableTestHardware(void)
{
	int ErrorSharedVariableTestName, ErrorSharedVariableDescription, ErrorSharedVariableCreationDate, ErrorSharedVariableAuthor, ErrorSharedVariableVersion;
	int ErrorSharedVariableTestDurating, ErrorSharedVariableModulName, ErrorSharedVariableExternalIsolationValveOption, ErrorSharedVariableValveSerie, ErrorSharedVariableModification;
	int ErrorSharedVariableController, ErrorSharedVariableInterface, ErrorSharedVariableOption, ErrorSharedVariableTestHardware, ErrorSharedVariableControllerHardwareVersion;
	int ErrorSharedVariableInterfaceHardwareVersion, ErrorSharedVariableModule1HardwareVersion, ErrorSharedVariableModule2HardwareVersion, ErrorSharedVariableModule3HardwareVersion;
	int ErrorSharedVariableModule4HardwareVersion, ErrorSharedVariableControllerAssemblyVariant, ErrorSharedVariableInterfaceAssemblyVariant, ErrorSharedVariableModule1AssemblyVariant;
	int ErrorSharedVariableModule2AssemblyVariant, ErrorSharedVariableModule3AssemblyVariant, ErrorSharedVariableModule4AssemblyVariant, ErrorSharedVariableModule1;
	int ErrorSharedVariableModule2, ErrorSharedVariableModule3, ErrorSharedVariableModule4;
	char VariableName[MAX_PATHNAME_LEN], VariableDescription[MAX_PATHNAME_LEN], VariableCreationDate[MAX_PATHNAME_LEN], VariableAuthor[MAX_PATHNAME_LEN];
	char VariableVersion[MAX_PATHNAME_LEN], VariableDurating[MAX_PATHNAME_LEN], VariableModulName[MAX_PATHNAME_LEN], VariableExternalIsolationValveOption[MAX_PATHNAME_LEN];
	char VariableValveSerie[MAX_PATHNAME_LEN], VariableModification[MAX_PATHNAME_LEN], VariableController[MAX_PATHNAME_LEN], VariableInterface[MAX_PATHNAME_LEN], VariableOption[MAX_PATHNAME_LEN];
	char VariableTestHardware[MAX_PATHNAME_LEN], VariableControllerHardwareVersion[MAX_PATHNAME_LEN], VariableInterfaceHardwareVersion[MAX_PATHNAME_LEN], VariableModule1HardwareVersion[MAX_PATHNAME_LEN];
	char VariableModule2HardwareVersion[MAX_PATHNAME_LEN], VariableModule3HardwareVersion[MAX_PATHNAME_LEN], VariableModule4HardwareVersion[MAX_PATHNAME_LEN], VariableControllerAssemblyVariant[MAX_PATHNAME_LEN];
	char VariableInterfaceAssemblyVariant[MAX_PATHNAME_LEN], VariableModule1AssemblyVariant[MAX_PATHNAME_LEN], VariableModule2AssemblyVariant[MAX_PATHNAME_LEN], VariableModule3AssemblyVariant[MAX_PATHNAME_LEN];
	char VariableModule4AssemblyVariant[MAX_PATHNAME_LEN], VariableModule1[MAX_PATHNAME_LEN], VariableModule2[MAX_PATHNAME_LEN], VariableModule3[MAX_PATHNAME_LEN], VariableModule4[MAX_PATHNAME_LEN];
	char InstallSharedVariablePath[MAX_PATHNAME_LEN] = INSTALL_SHARED_VARIABLE_PATH;
	
	//Die Umgebungsvariable Name, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableName[0] = 0;
	strcat(VariableName, "\\\\localhost\\TestInformation_SharedVariable\\Name");
	if (VariableName[0] == '\\')
	{		
  	ErrorSharedVariableTestName = CNVCreateSubscriber (VariableName, DataCallbackTestName, StatusCallbackTestName, NULL, 10000, 0, &sTestName);
		if (ErrorSharedVariableTestName < 0)	
		{
			//Könnte gut möglich sein, dass die Umgebungsvariable noch nicht erzeugt wurde
			MessagePopup ("ErrorSharedVariableTestName", CNVGetErrorDescription(ErrorSharedVariableTestName));
			system(InstallSharedVariablePath);
		}
	}
	
	//Die Umgebungsvariable Description, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableDescription[0] = 0;
	strcat(VariableDescription, "\\\\localhost\\TestInformation_SharedVariable\\Description");
	if (VariableDescription[0] == '\\')
	{		
  	ErrorSharedVariableDescription = CNVCreateSubscriber (VariableDescription, DataCallbackTestDescription, StatusCallbackTestDescription, NULL, 10000, 0, &sTestDescription);
		if (ErrorSharedVariableDescription < 0)	MessagePopup ("ErrorSharedVariableDescription", CNVGetErrorDescription(ErrorSharedVariableDescription));
	}
	
	//Die Umgebungsvariable CreationDate, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableCreationDate[0] = 0;
	strcat(VariableCreationDate, "\\\\localhost\\TestInformation_SharedVariable\\CreationDate");
	if (VariableCreationDate[0] == '\\')
	{		
  	ErrorSharedVariableCreationDate = CNVCreateSubscriber (VariableCreationDate, DataCallbackCreationTest, StatusCallbackCreationTest, NULL, 10000, 0, &sCreationTest);
		if (ErrorSharedVariableCreationDate < 0)	MessagePopup ("ErrorSharedVariableCreationDate", CNVGetErrorDescription(ErrorSharedVariableCreationDate));
	}
	
	//Die Umgebungsvariable Author, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableAuthor[0] = 0;
	strcat(VariableAuthor, "\\\\localhost\\TestInformation_SharedVariable\\Author");
	if (VariableAuthor[0] == '\\')
	{		
  	ErrorSharedVariableAuthor = CNVCreateSubscriber (VariableAuthor, DataCallbackCreationAuthor, StatusCallbackCreationAuthor, NULL, 10000, 0, &sCreationAuthor);
		if (ErrorSharedVariableAuthor < 0)	MessagePopup ("ErrorSharedVariableAuthor", CNVGetErrorDescription(ErrorSharedVariableAuthor));
	}
	
	//Die Umgebungsvariable Version, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableVersion[0] = 0;
	strcat(VariableVersion, "\\\\localhost\\TestInformation_SharedVariable\\Version");
	if (VariableVersion[0] == '\\')
	{		
  	ErrorSharedVariableVersion = CNVCreateSubscriber (VariableVersion, DataCallbackTestVersion, StatusCallbackTestVersion, 0, 10000, 0, &sTestVersion);
		if (ErrorSharedVariableVersion < 0)	MessagePopup ("ErrorSharedVariableVersion", CNVGetErrorDescription(ErrorSharedVariableVersion));
	}
	
	//Die Umgebungsvariable Modification, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModification[0] = 0;
	strcat(VariableModification, "\\\\localhost\\TestInformation_SharedVariable\\Modification");
	if (VariableModification[0] == '\\')
	{		
  	ErrorSharedVariableModification = CNVCreateSubscriber (VariableModification, DataCallbackTestModification, StatusCallbackTestModification, NULL, 10000, 0, &sTestModification);
		if (ErrorSharedVariableModification < 0)	MessagePopup ("ErrorSharedVariableModification", CNVGetErrorDescription(ErrorSharedVariableModification));
	}
	
	//Die Umgebungsvariable Durating, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableDurating[0] = 0;
	strcat(VariableDurating, "\\\\localhost\\TestInformation_SharedVariable\\Durating");
	if (VariableDurating[0] == '\\')
	{		
  	ErrorSharedVariableTestDurating = CNVCreateSubscriber (VariableDurating, DataCallbackTestDurating, StatusCallbackTestDurating, NULL, 10000, 0, &sTestDurating);
		if (ErrorSharedVariableTestDurating < 0)	MessagePopup ("ErrorSharedVariableTestDurating", CNVGetErrorDescription(ErrorSharedVariableTestDurating));
	}
	
	//Die Umgebungsvariable ModulName, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModulName[0] = 0;
	strcat(VariableModulName, "\\\\localhost\\TestInformation_SharedVariable\\ModulName");
	if (VariableModulName[0] == '\\')
	{		
  	ErrorSharedVariableModulName = CNVCreateSubscriber (VariableModulName, DataCallbackTestModulName, StatusCallbackTestModulName, NULL, 10000, 0, &sTestModulName);
		if (ErrorSharedVariableModulName < 0)	MessagePopup ("ErrorSharedVariableModulName", CNVGetErrorDescription(ErrorSharedVariableModulName));
	}
	
	//Die Umgebungsvariable ExternalIsolationValveOption, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableExternalIsolationValveOption[0] = 0;
	strcat(VariableExternalIsolationValveOption, "\\\\localhost\\TestInformation_SharedVariable\\ExternalIsolationValveOption");
	if (VariableExternalIsolationValveOption[0] == '\\')
	{		
  	ErrorSharedVariableExternalIsolationValveOption = CNVCreateSubscriber (VariableExternalIsolationValveOption, DataCallbackTestExternalIsolationValveOption, StatusCallbackTestExternalIsolationValveOption, NULL, 10000, 0, &sTestExternalIsolationValveOption);
		if (ErrorSharedVariableExternalIsolationValveOption < 0)	MessagePopup ("ErrorSharedVariableExternalIsolationValveOption", CNVGetErrorDescription(ErrorSharedVariableExternalIsolationValveOption));
	}
	
	//Die Umgebungsvariable ValveSerie, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableValveSerie[0] = 0;
	strcat(VariableValveSerie, "\\\\localhost\\TestInformation_SharedVariable\\ValveSerie");
	if (VariableValveSerie[0] == '\\')
	{		
  	ErrorSharedVariableValveSerie = CNVCreateSubscriber (VariableValveSerie, DataCallbackTestValveSerie, StatusCallbackTestValveSerie, NULL, 10000, 0, &sTestValveSerie);
		if (ErrorSharedVariableValveSerie < 0)	MessagePopup ("ErrorSharedVariableValveSerie", CNVGetErrorDescription(ErrorSharedVariableValveSerie));
	}
	
	//Die Umgebungsvariable Controller, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableController[0] = 0;
	strcat(VariableController, "\\\\localhost\\TestInformation_SharedVariable\\Controller");
	if (VariableController[0] == '\\')
	{		
  	ErrorSharedVariableController = CNVCreateSubscriber (VariableController, DataCallbackTestController, StatusCallbackTestController, NULL, 10000, 0, &sTestController);
		if (ErrorSharedVariableController < 0)	MessagePopup ("ErrorSharedVariableController", CNVGetErrorDescription(ErrorSharedVariableController));
	}
	
	//Die Umgebungsvariable Interface, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableInterface[0] = 0;
	strcat(VariableInterface, "\\\\localhost\\TestInformation_SharedVariable\\Interface");
	if (VariableInterface[0] == '\\')
	{		
  	ErrorSharedVariableInterface = CNVCreateSubscriber (VariableInterface, DataCallbackTestInterface, StatusCallbackTestInterface, NULL, 10000, 0, &sTestInterface);
		if (ErrorSharedVariableInterface < 0)	MessagePopup ("ErrorSharedVariableInterface", CNVGetErrorDescription(ErrorSharedVariableInterface));
	}
	
	//Die Umgebungsvariable Option, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableOption[0] = 0;
	strcat(VariableOption, "\\\\localhost\\TestInformation_SharedVariable\\Option");
	if (VariableOption[0] == '\\')
	{		
  	ErrorSharedVariableOption = CNVCreateSubscriber (VariableOption, DataCallbackTestOption, StatusCallbackTestOption, NULL, 10000, 0, &sTestOption);
		if (ErrorSharedVariableOption < 0)	MessagePopup ("ErrorSharedVariableOption", CNVGetErrorDescription(ErrorSharedVariableOption));
	}
	
	//Die Umgebungsvariable TestHardware, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableTestHardware[0] = 0;
	strcat(VariableTestHardware, "\\\\localhost\\TestInformation_SharedVariable\\TestHardware");
	if (VariableTestHardware[0] == '\\')
	{		
  	ErrorSharedVariableTestHardware = CNVCreateSubscriber (VariableTestHardware, DataCallbackTestHardware, StatusCallbackTestHardware, NULL, 10000, 0, &sTestHardware);
		if (ErrorSharedVariableTestHardware < 0)	MessagePopup ("ErrorSharedVariableTestHardware", CNVGetErrorDescription(ErrorSharedVariableTestHardware));
	}
	
	//Die Umgebungsvariable ControllerHardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableControllerHardwareVersion[0] = 0;
	strcat(VariableControllerHardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\ControllerHardwareVersion");
	if (VariableControllerHardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableControllerHardwareVersion = CNVCreateSubscriber (VariableControllerHardwareVersion, DataCallbackTestControllerHardwareVersion, StatusCallbackTestControllerHardwareVersion, NULL, 10000, 0, &sTestControllerHardwareVersion);
		if (ErrorSharedVariableControllerHardwareVersion < 0)	MessagePopup ("ErrorSharedVariableControllerHardwareVersion", CNVGetErrorDescription(ErrorSharedVariableControllerHardwareVersion));
	}
	
	//Die Umgebungsvariable InterfaceHardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableInterfaceHardwareVersion[0] = 0;
	strcat(VariableInterfaceHardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\InterfaceHardwareVersion");
	if (VariableInterfaceHardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableInterfaceHardwareVersion = CNVCreateSubscriber (VariableInterfaceHardwareVersion, DataCallbackTestInterfaceHardwareVersion, StatusCallbackTestInterfaceHardwareVersion, NULL, 10000, 0, &sTestInterfaceHardwareVersion);
		if (ErrorSharedVariableInterfaceHardwareVersion < 0)	MessagePopup ("ErrorSharedVariableInterfaceHardwareVersion", CNVGetErrorDescription(ErrorSharedVariableInterfaceHardwareVersion));
	}
	
	//Die Umgebungsvariable Module1HardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule1HardwareVersion[0] = 0;
	strcat(VariableModule1HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module1HardwareVersion");
	if (VariableModule1HardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableModule1HardwareVersion = CNVCreateSubscriber (VariableModule1HardwareVersion, DataCallbackTestModule1HardwareVersion, StatusCallbackTestModule1HardwareVersion, NULL, 10000, 0, &sTestModule1HardwareVersion);
		if (ErrorSharedVariableModule1HardwareVersion < 0)	MessagePopup ("ErrorSharedVariableModule1HardwareVersion", CNVGetErrorDescription(ErrorSharedVariableModule1HardwareVersion));
	}
	
	//Die Umgebungsvariable Module2HardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule2HardwareVersion[0] = 0;
	strcat(VariableModule2HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module2HardwareVersion");
	if (VariableModule2HardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableModule2HardwareVersion = CNVCreateSubscriber (VariableModule2HardwareVersion, DataCallbackTestModule2HardwareVersion, StatusCallbackTestModule2HardwareVersion, NULL, 10000, 0, &sTestModule2HardwareVersion);
		if (ErrorSharedVariableModule2HardwareVersion < 0)	MessagePopup ("ErrorSharedVariableModule2HardwareVersion", CNVGetErrorDescription(ErrorSharedVariableModule2HardwareVersion));
	}
	
	//Die Umgebungsvariable Module3HardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule3HardwareVersion[0] = 0;
	strcat(VariableModule3HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module3HardwareVersion");
	if (VariableModule3HardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableModule3HardwareVersion = CNVCreateSubscriber (VariableModule3HardwareVersion, DataCallbackTestModule3HardwareVersion, StatusCallbackTestModule3HardwareVersion, NULL, 10000, 0, &sTestModule3HardwareVersion);
		if (ErrorSharedVariableModule3HardwareVersion < 0)	MessagePopup ("ErrorSharedVariableModule3HardwareVersion", CNVGetErrorDescription(ErrorSharedVariableModule3HardwareVersion));
	}
	
	//Die Umgebungsvariable Module4HardwareVersion, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule4HardwareVersion[0] = 0;
	strcat(VariableModule4HardwareVersion, "\\\\localhost\\TestInformation_SharedVariable\\Module4HardwareVersion");
	if (VariableModule4HardwareVersion[0] == '\\')
	{		
  	ErrorSharedVariableModule4HardwareVersion = CNVCreateSubscriber (VariableModule4HardwareVersion, DataCallbackTestModule4HardwareVersion, StatusCallbackTestModule4HardwareVersion, NULL, 10000, 0, &sTestModule4HardwareVersion);
		if (ErrorSharedVariableModule4HardwareVersion < 0)	MessagePopup ("ErrorSharedVariableModule4HardwareVersion", CNVGetErrorDescription(ErrorSharedVariableModule4HardwareVersion));
	}
	
	//Die Umgebungsvariable ControllerAssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableControllerAssemblyVariant[0] = 0;
	strcat(VariableControllerAssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\ControllerAssemblyVariant");
	if (VariableControllerAssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableControllerAssemblyVariant = CNVCreateSubscriber (VariableControllerAssemblyVariant, DataCallbackTestControllerAssemblyVariant, StatusCallbackTestControllerAssemblyVariant, NULL, 10000, 0, &sTestControllerAssemblyVariant);
		if (ErrorSharedVariableControllerAssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableControllerAssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableControllerAssemblyVariant));
	}
	
	//Die Umgebungsvariable InterfaceAssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableInterfaceAssemblyVariant[0] = 0;
	strcat(VariableInterfaceAssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\InterfaceAssemblyVariant");
	if (VariableInterfaceAssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableInterfaceAssemblyVariant = CNVCreateSubscriber (VariableInterfaceAssemblyVariant, DataCallbackTestInterfaceAssemblyVariant, StatusCallbackTestInterfaceAssemblyVariant, NULL, 10000, 0, &sTestInterfaceAssemblyVariant);
		if (ErrorSharedVariableInterfaceAssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableInterfaceAssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableInterfaceAssemblyVariant));
	}
	
	//Die Umgebungsvariable Module1AssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule1AssemblyVariant[0] = 0;
	strcat(VariableModule1AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module1AssemblyVariant");
	if (VariableModule1AssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableModule1AssemblyVariant = CNVCreateSubscriber (VariableModule1AssemblyVariant, DataCallbackTestModule1AssemblyVariant, StatusCallbackTestModule1AssemblyVariant, NULL, 10000, 0, &sTestModule1AssemblyVariant);
		if (ErrorSharedVariableModule1AssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableModule1AssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableModule1AssemblyVariant));
	}
	
	//Die Umgebungsvariable Module2AssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule2AssemblyVariant[0] = 0;
	strcat(VariableModule2AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module2AssemblyVariant");
	if (VariableModule2AssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableModule2AssemblyVariant = CNVCreateSubscriber (VariableModule2AssemblyVariant, DataCallbackTestModule2AssemblyVariant, StatusCallbackTestModule2AssemblyVariant, NULL, 10000, 0, &sTestModule2AssemblyVariant);
		if (ErrorSharedVariableModule2AssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableModule2AssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableModule2AssemblyVariant));
	}
	
	//Die Umgebungsvariable Module3AssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule3AssemblyVariant[0] = 0;
	strcat(VariableModule3AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module3AssemblyVariant");
	if (VariableModule3AssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableModule3AssemblyVariant = CNVCreateSubscriber (VariableModule3AssemblyVariant, DataCallbackTestModule3AssemblyVariant, StatusCallbackTestModule3AssemblyVariant, NULL, 10000, 0, &sTestModule3AssemblyVariant);
		if (ErrorSharedVariableModule3AssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableModule3AssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableModule3AssemblyVariant));
	}
	
	//Die Umgebungsvariable Module4AssemblyVariant, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule4AssemblyVariant[0] = 0;
	strcat(VariableModule4AssemblyVariant, "\\\\localhost\\TestInformation_SharedVariable\\Module4AssemblyVariant");
	if (VariableModule4AssemblyVariant[0] == '\\')
	{		
  	ErrorSharedVariableModule4AssemblyVariant = CNVCreateSubscriber (VariableModule4AssemblyVariant, DataCallbackTestModule4AssemblyVariant, StatusCallbackTestModule4AssemblyVariant, NULL, 10000, 0, &sTestModule4AssemblyVariant);
		if (ErrorSharedVariableModule4AssemblyVariant < 0)	MessagePopup ("ErrorSharedVariableModule4AssemblyVariant", CNVGetErrorDescription(ErrorSharedVariableModule4AssemblyVariant));
	}
	
	//Die Umgebungsvariable Module1, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule1[0] = 0;
	strcat(VariableModule1, "\\\\localhost\\TestInformation_SharedVariable\\Module1");
	if (VariableModule1[0] == '\\')
	{		
  	ErrorSharedVariableModule1 = CNVCreateSubscriber (VariableModule1, DataCallbackTestModule1, StatusCallbackTestModule1, NULL, 10000, 0, &sTestModule1);
		if (ErrorSharedVariableModule1 < 0)	MessagePopup ("ErrorSharedVariableModule1", CNVGetErrorDescription(ErrorSharedVariableModule1));
	}
	
	//Die Umgebungsvariable Module2, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule2[0] = 0;
	strcat(VariableModule2, "\\\\localhost\\TestInformation_SharedVariable\\Module2");
	if (VariableModule2[0] == '\\')
	{		
  	ErrorSharedVariableModule2 = CNVCreateSubscriber (VariableModule2, DataCallbackTestModule2, StatusCallbackTestModule2, NULL, 10000, 0, &sTestModule2);
		if (ErrorSharedVariableModule2 < 0)	MessagePopup ("ErrorSharedVariableModule2", CNVGetErrorDescription(ErrorSharedVariableModule2));
	}
	
	//Die Umgebungsvariable Module3, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule3[0] = 0;
	strcat(VariableModule3, "\\\\localhost\\TestInformation_SharedVariable\\Module3");
	if (VariableModule3[0] == '\\')
	{		
  	ErrorSharedVariableModule3 = CNVCreateSubscriber (VariableModule3, DataCallbackTestModule3, StatusCallbackTestModule3, NULL, 10000, 0, &sTestModule3);
		if (ErrorSharedVariableModule3 < 0)	MessagePopup ("ErrorSharedVariableModule3", CNVGetErrorDescription(ErrorSharedVariableModule3));
	}
	
	//Die Umgebungsvariable Module4, welche Information ausliest um diese anschliessend in der Datenbank abzuspeichern
	VariableModule4[0] = 0;
	strcat(VariableModule4, "\\\\localhost\\TestInformation_SharedVariable\\Module4");
	if (VariableModule4[0] == '\\')
	{		
  	ErrorSharedVariableModule4 = CNVCreateSubscriber (VariableModule4, DataCallbackTestModule4, StatusCallbackTestModule4, NULL, 10000, 0, &sTestModule4);
		if (ErrorSharedVariableModule4 < 0)	MessagePopup ("ErrorSharedVariableModule4", CNVGetErrorDescription(ErrorSharedVariableModule4));
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Beendet die Umgebungsvariablen um die Test Hardware Anforderungen abspeichern zu können. 
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
void SharedVariable_EndNetworkvariableTestHardware(void)
{
	CNVDispose(sTestName);
	CNVDispose(sTestDescription);
	CNVDispose(sCreationTest);
	CNVDispose(sCreationAuthor);
	CNVDispose(sTestVersion);
	CNVDispose(sTestModification);
	CNVDispose(sTestDurating);
	CNVDispose(sTestModulName);
	CNVDispose(sTestExternalIsolationValveOption);
	CNVDispose(sTestValveSerie);
	CNVDispose(sTestController);
	CNVDispose(sTestInterface);
	CNVDispose(sTestOption);
	CNVDispose(sTestHardware);
	CNVDispose(sTestControllerHardwareVersion);
	CNVDispose(sTestInterfaceHardwareVersion);
	CNVDispose(sTestModule1HardwareVersion);
	CNVDispose(sTestModule2HardwareVersion);
	CNVDispose(sTestModule3HardwareVersion);
	CNVDispose(sTestModule4HardwareVersion);
	CNVDispose(sTestControllerAssemblyVariant);
	CNVDispose(sTestInterfaceAssemblyVariant);
	CNVDispose(sTestModule1AssemblyVariant);
	CNVDispose(sTestModule2AssemblyVariant);
	CNVDispose(sTestModule3AssemblyVariant);
	CNVDispose(sTestModule4AssemblyVariant);
	CNVDispose(sTestModule1);
	CNVDispose(sTestModule2);
	CNVDispose(sTestModule3);
	CNVDispose(sTestModule4);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestName (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Name im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Name Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestName (void * handle, CNVData DataTestName, void * callbackData)
{
	char *pReceiveTestNamePointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestName == 0)	return;
	
	CNVGetDataType (DataTestName, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestName, type, &pReceiveTestNamePointer);
		strcpy(ReceiveTestName, pReceiveTestNamePointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestDescription (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Description im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Description Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestDescription (void * handle, CNVData DataTestDescription, void * callbackData)
{
	char *pReceiveTestDescriptionPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestDescription == 0)	return;
	
	CNVGetDataType (DataTestDescription, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestDescription, type, &pReceiveTestDescriptionPointer);
		strcpy(ReceiveTestDescription, pReceiveTestDescriptionPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackCreationTest (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable CreationDate im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der CreationDate Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackCreationTest (void * handle, CNVData DataCreationTest, void * callbackData)
{
	char *pReceiveCreationTestPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataCreationTest == 0)	return;
	
	CNVGetDataType (DataCreationTest, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataCreationTest, type, &pReceiveCreationTestPointer);
		strcpy(ReceiveCreationTest, pReceiveCreationTestPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackCreationAuthor (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Author im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Author Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackCreationAuthor (void * handle, CNVData DataCreationAuthor, void * callbackData)
{
	char *pReceiveCreationAuthorPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataCreationAuthor == 0)	return;
	
	CNVGetDataType (DataCreationAuthor, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataCreationAuthor, type, &pReceiveCreationAuthorPointer);
		strcpy(ReceiveCreationAuthor, pReceiveCreationAuthorPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Version im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Version Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestVersion (void * handle, CNVData DataTestVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestVersion == 0)	return;
	
	CNVGetDataType (DataTestVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestVersion, type, &TestVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModification (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Durating im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Modification Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModification (void * handle, CNVData DataTestModification, void * callbackData)
{
	char *pReceiveTestModificationPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModification == 0)	return;
	
	CNVGetDataType (DataTestModification, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestModification, type, &pReceiveTestModificationPointer);
		strcpy(ReceiveTestModification, pReceiveTestModificationPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestDurating (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Durating im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Durating Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestDurating (void * handle, CNVData DataTestDurating, void * callbackData)
{
	char *pReceiveTestDuratingPointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestDurating == 0)	return;
	
	CNVGetDataType (DataTestDurating, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestDurating, type, &pReceiveTestDuratingPointer);
		strcpy(ReceiveTestDurating, pReceiveTestDuratingPointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModulName (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable ModulName im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der ModulName Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModulName (void * handle, CNVData DataTestModulName, void * callbackData)
{
	char *pReceiveTestModulNamePointer;
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModulName == 0)	return;
	
	CNVGetDataType (DataTestModulName, &type, &nDims);

	//Handelt sich um ein String, sollen dieser ausgelesen werden
	if (type == CNVString)
	{
		CNVGetScalarDataValue (DataTestModulName, type, &pReceiveTestModulNamePointer);
		strcpy(ReceiveTestModulName, pReceiveTestModulNamePointer);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestExternalIsolationValveOption (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable ExternalIsolationValveOption im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der ExternalIsolationValveOption Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestExternalIsolationValveOption (void * handle, CNVData DataTestExternalIsolationValveOption, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestExternalIsolationValveOption == 0)	return;
	
	CNVGetDataType (DataTestExternalIsolationValveOption, &type, &nDims);

	//Handelt sich um ein SINT32, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestExternalIsolationValveOption, type, &TestExternalIsolationValveOption);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestValveSerie (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable ValveSerie im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der ValveSerie Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestValveSerie (void * handle, CNVData DataTestValveSerie, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestValveSerie == 0)	return;
	
	CNVGetDataType (DataTestValveSerie, &type, &nDims);

	//Handelt sich um ein SINT32, sollen dieser ausgelesen werden
	if (type == CNVInt32)
	{
		CNVGetScalarDataValue (DataTestValveSerie, type, &TestValveSerie);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestController (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Controller im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Controller Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestController (void * handle, CNVData DataTestController, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestController == 0)	return;
	
	CNVGetDataType (DataTestController, &type, &nDims);

	//Handelt sich um ein SINT32, sollen dieser ausgelesen werden
	if (type == CNVInt32)
	{
		CNVGetScalarDataValue (DataTestController, type, &TestController);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestInterface (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Interface im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Interface Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestInterface (void * handle, CNVData DataTestInterface, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestInterface == 0)	return;
	
	CNVGetDataType (DataTestInterface, &type, &nDims);

	//Handelt sich um ein SINT32, sollen dieser ausgelesen werden
	if (type == CNVInt32)
	{
		CNVGetScalarDataValue (DataTestInterface, type, &TestInterface);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestOption (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Option im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Option Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestOption (void * handle, CNVData DataTestOption, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestOption == 0)	return;
	
	CNVGetDataType (DataTestOption, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestOption, type, &TestOption);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestHardware (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable TestHardware im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der TestHardware Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestHardware (void * handle, CNVData DataTestHardware, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestHardware == 0)	return;
	
	CNVGetDataType (DataTestHardware, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestHardware, type, &TestHardware);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestControllerHardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable ControllerHardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der ControllerHardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestControllerHardwareVersion (void * handle, CNVData DataTestControllerHardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestControllerHardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestControllerHardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestControllerHardwareVersion, type, &TestControllerHardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestInterfaceHardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable InterfaceHardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der InterfaceHardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestInterfaceHardwareVersion (void * handle, CNVData DataTestInterfaceHardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestInterfaceHardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestInterfaceHardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestInterfaceHardwareVersion, type, &TestInterfaceHardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule1HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module1HardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1HardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule1HardwareVersion (void * handle, CNVData DataTestModule1HardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule1HardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestModule1HardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule1HardwareVersion, type, &TestModule1HardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule2HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module2HardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module2HardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule2HardwareVersion (void * handle, CNVData DataTestModule2HardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule2HardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestModule2HardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule2HardwareVersion, type, &TestModule2HardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule3HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module3HardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module3HardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule3HardwareVersion (void * handle, CNVData DataTestModule3HardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule3HardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestModule3HardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule3HardwareVersion, type, &TestModule3HardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule4HardwareVersion (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module4HardwareVersion im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module4HardwareVersion Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule4HardwareVersion (void * handle, CNVData DataTestModule4HardwareVersion, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule4HardwareVersion == 0)	return;
	
	CNVGetDataType (DataTestModule4HardwareVersion, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule4HardwareVersion, type, &TestModule4HardwareVersion);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestControllerAssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable ControllerAssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der ControllerAssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestControllerAssemblyVariant (void * handle, CNVData DataTestControllerAssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestControllerAssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestControllerAssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestControllerAssemblyVariant, type, &TestControllerAssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestInterfaceAssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable InterfaceAssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der InterfaceAssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestInterfaceAssemblyVariant (void * handle, CNVData DataTestInterfaceAssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestInterfaceAssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestInterfaceAssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestInterfaceAssemblyVariant, type, &TestInterfaceAssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule1AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module1AssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule1AssemblyVariant (void * handle, CNVData DataTestModule1AssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule1AssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestModule1AssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule1AssemblyVariant, type, &TestModule1AssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule2AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module2AssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule2AssemblyVariant (void * handle, CNVData DataTestModule2AssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule2AssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestModule2AssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule2AssemblyVariant, type, &TestModule2AssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule3AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module3AssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule3AssemblyVariant (void * handle, CNVData DataTestModule3AssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule3AssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestModule3AssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule3AssemblyVariant, type, &TestModule3AssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule4AssemblyVariant (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module4AssemblyVariant im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule4AssemblyVariant (void * handle, CNVData DataTestModule4AssemblyVariant, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule4AssemblyVariant == 0)	return;
	
	CNVGetDataType (DataTestModule4AssemblyVariant, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule4AssemblyVariant, type, &TestModule4AssemblyVariant);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule1 (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module1 im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module1AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule1 (void * handle, CNVData DataTestModule1, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule1 == 0)	return;
	
	CNVGetDataType (DataTestModule1, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule1, type, &TestModule1);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule2 (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module2 im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module2AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule2 (void * handle, CNVData DataTestModule2, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule2 == 0)	return;
	
	CNVGetDataType (DataTestModule2, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule2, type, &TestModule2);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule3 (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module3 im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module3AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule3 (void * handle, CNVData DataTestModule3, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule3 == 0)	return;
	
	CNVGetDataType (DataTestModule3, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule3, type, &TestModule3);
	}
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche für die Fehler der Verbindungsaufbau mit der Umgebungsvariable zeigt
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK StatusCallbackTestModule4 (void * handle, CNVConnectionStatus status, int error, void * callbackData)
{
  if (error < 0)
  {
		printf("Fehler bei der Verbindung Variable Module4 im TestUpdateFirmware Programm\n");
		return;
  }
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Callback Funktion, welche aufgerufen wird, wenn sich der Module4AssemblyVariant Umgebungsvariable verändert hat
//  Input           : -
//  Output *Pointer : -
//         *return  : -
//******************************************************************************************************************************
static void CVICALLBACK DataCallbackTestModule4 (void * handle, CNVData DataTestModule4, void * callbackData)
{
  unsigned long   nDims;
  CNVDataType     type;
	
	if (DataTestModule4 == 0)	return;
	
	CNVGetDataType (DataTestModule4, &type, &nDims);

	//Handelt sich um ein SINT16, sollen dieser ausgelesen werden
	if (type == CNVInt16)
	{
		CNVGetScalarDataValue (DataTestModule4, type, &TestModule4);
	}
}
---
