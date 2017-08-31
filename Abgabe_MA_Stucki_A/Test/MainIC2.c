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
#include <stdio.h>
#include <utility.h>
#include <userint.h>

#include "GenerateReport.h"
#include "HelpFunctions.h"
#include "MainIC2.h"
#include "ParameterProcessing.h"
#include "SharedVariable.h" 
#include "Test.h"
#include "TestEnvironment.h"
#include "Typedefinitions.h"

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
UINT8 TestState = ERRORLESS;

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ruft die Testroutine auf 
//  Input           : argc 		gibt Anzahl Argumente an
//									:	*argv[] Text der einzelnen Einträge
//  Output *Pointer : -
//         *return  : RETURNVALUETEST:	Gibt das Testresultat zurück (0: SUCCESSFUL,	1:USERCANCELED, 2:ERRORTEST) 
//******************************************************************************************************************************
int main(int argc, char *argv[])
{
	//lest die Globale Opton Varible aus
	TestEnvironment_ReadOptionString();
+++
	//schreibt alle Testinformations Umgebungsvariablen
	if(strlen(TestOptionsString) != 0 && TestOptionsString[INFO_OPTION_INDEX] == '1')
	{
		Test_WriteInfoTestInSharedVariable();
		return 0;
	}
	
	//schreibt die Test Version in die Umgebungsvariable
	if(strlen(TestOptionsString) != 0 && TestOptionsString[TEST_VERSION_OPTION_INDEX] == '1')
	{
		SharedVariable_InitNetworkvariableTTIC2TestVersion();
		SharedVariable_WriteTTIC2TestVersionValue(TestVersion);
		SharedVariable_EndNetworkvariableTTIC2TestVersion();
		return 0;
	}
---	
	//Berechnung des Zeitoffsets
	HelpFunctions_SetTimeOffset();
	//ruft die Oberfläche des Tests auf
	Test_TestProcedure();
	//Das Ergebnis in die Umgebungsvariable ReturnValue schreiben
	if(TestState == ERRORLESS)					SharedVariable_WriteTestReturnValue("Succesful");
	else if(TestState == USERCANCELLED)	SharedVariable_WriteTestReturnValue("UserCanceled");
	else																SharedVariable_WriteTestReturnValue("ErrorTest");
	CNVFinish();

	return TestState;
}

											
