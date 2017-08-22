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
//muss vor utility.h stehen
#include <windows.h>
#include <utility.h>
#include <userint.h>

#include "Database.h"
#include "Definelist.h"
#include "DisplayErrorCompileProject.h"
#include "HelpFunctions.h"
#include "SharedVariable.h"
#include "Typedefinitions.h" 

//******************************************************************************************************************************
// Function Prototypes
//******************************************************************************************************************************
static void BatchFile(char *pTestProjectName);
static void BatchFileTTIC2(void);
static void BatchFileTTIC2Commander(void);
static void CheckIfTestHardwareIsActuall(BOOL8 *pTestHardwareCompatibleList);
static void CompileTests(BOOL8 *pTestHardwareCompatibleList);
static void DisplayErrorCompileProject(void);
static void UpdateExeTTIC2Folder(void);
static void ReadStructureFromFirmware(void);
static void ResultCompileProject(char *pTestProjectName);
static void ReplacementFirmwareStructureFiles(void);
static void ReplacementPlatformConfigFile(void);
static void RunBatchFile(char *pAdressBatchFile); 

//******************************************************************************************************************************
// Global Variables
//******************************************************************************************************************************
UINT16 CountErrorCompileProject = 0, ErrorCompileListLength = 0, CountTests = 0;
char **ppArrayFileListWithCompileErrors, *pListCompileErrors, TTIC2CommanderFolder[MAX_PATHNAME_LEN];

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ruft die Testroutine auf 
//  Input           : argc 		gibt Anzahl Argumente an
//									:	*argv[] Text der einzelnen Einträge
//  Output *Pointer : -
//         *return  : 
//******************************************************************************************************************************
int main(int argc, char *argv[])
{
	//schreibt den Projektnamen in eine Globalen String
	HelpFunctions_GetProjectPathName();
	
	//Auslesung Strucktur der Firmware
	ReadStructureFromFirmware();
	
	//Ersetzung des c und h Files, inwelcher die Strucktur der Software definiert ist
	ReplacementFirmwareStructureFiles();
	
	//Ersetzt das PlatformConfig h File in allen verwendeten Ordner (Test, TTIC2, TTIC2Commander)
	ReplacementPlatformConfigFile(); 
	
	//Erstellt Liste mit den Tests
	CountTests = HelpFunctions_WriteExeFiles(TestDirection);
	BOOL8 TestHardwareCompatible[CountTests];
	
+++
	//Schauen ob alle Testhardware Informationen noch aktuell sind
	CheckIfTestHardwareIsActuall(TestHardwareCompatible);
---
	
	//batch File erstellen mit allen Tests
	CompileTests(TestHardwareCompatible);
	
	//Den exe Ordner der Oberfläche erneuern
	UpdateExeTTIC2Folder(); 
	
	//Allozierter Speicher wider freigeben
	free(ppArrayFileList);
	free(ppArrayFileListWithCompileErrors);
	free(pListCompileErrors);
	CNVFinish();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Erstellt das Batch File, in welchem definiert wird welche Projekte neu kompiliert werden 
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CompileTests(BOOL8 *pTestHardwareCompatibleList)
{
	UINT16 IndexTest;
	
	//Batch File beschreiben
	//Einzelnen Projekte aufrufen um sie kompelieren zu lassen
	for(IndexTest = 0; IndexTest < CountTests; IndexTest++)
	{
		//Nur den Test kompilieren, wenn das Ventil die entsprechende Controller Hardware unterstützt
		//if(pTestHardwareCompatibleList[IndexTest] == TRUE)	BatchFile(ppArrayFileList[IndexTest]);
		BatchFile(ppArrayFileList[IndexTest]);
	}
	
	//Oberfläche kompilieren
	BatchFileTTIC2();
	
	//Kommunikationstool für IC2 kompilieren
	BatchFileTTIC2Commander();
	
	//Anzeige der fehlerhaften Projekte
	DisplayErrorCompileProject();
}

//******************************************************************************************************************************
//  Description     : Erstellt ein Batch File mit dem übergebenen Test Projekt
//  Input           : IndexTest:	Index des Test aus der Test Liste	
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
static void BatchFile(char *pTestProjectName)
{
	char pAdressBatchFile[MAX_PATHNAME_LEN];
	FILE *BatchFile;
	
	//Batch File Adresse definieren, im ProjektOrdner ablegen
	strcpy(pAdressBatchFile, ProjectDirection);
	strcat(pAdressBatchFile, BATCHFILENAME);
	
	//Batch File erzeugen
	BatchFile = fopen(pAdressBatchFile, "w+");
	
	fprintf(BatchFile, CHANGE_FILE_DIRECTION_COMMAND);
	fprintf(BatchFile, COMPILE_CVI_EXE_DIRECTION);
	fprintf(BatchFile, NEWLINE);
	
	fprintf(BatchFile, COMPILE_CVI_PROJECT_COMMAND);
	fprintf(BatchFile, TestDirection);
	fprintf(BatchFile, BETWEENCHAR);
	fprintf(BatchFile, pTestProjectName);
	fprintf(BatchFile, BETWEENCHAR);
	
	fprintf(BatchFile, pTestProjectName);
	fprintf(BatchFile, PROJECT_SUFFIX);
	fprintf(BatchFile, WRITE_IN_TEXTFILE);
	fprintf(BatchFile, ADRESS_REPORT_COMPILER); 
	fprintf(BatchFile, NEWLINE);
	
	//Batch File wider schliessen
	fclose(BatchFile);
	
	//Batch File ausführen
	system(pAdressBatchFile);
	
	//Batch File löschen, um laufend ein neues erzeugen können, welches das nächste Projekt aufruft zum compilieren
	remove(pAdressBatchFile);
	
	//Auswertung Compiler
	ResultCompileProject(pTestProjectName);
}

//******************************************************************************************************************************
//  Description     : Erstellt das Batch File für die Test Oberfläche
//  Input           : 
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
static void BatchFileTTIC2(void)
{
	char pAdressBatchFile[MAX_PATHNAME_LEN];
	FILE *BatchFile;
	
	//Batch File Adresse definieren, im ProjektOrdner ablegen
	strcpy(pAdressBatchFile, ProjectDirection);
	strcat(pAdressBatchFile, BATCHFILENAME);
	
	//Batch File erzeugen
	BatchFile = fopen(pAdressBatchFile, "w");
	
	fprintf(BatchFile, CHANGE_FILE_DIRECTION_COMMAND);
	fprintf(BatchFile, COMPILE_CVI_EXE_DIRECTION);
	fprintf(BatchFile, NEWLINE);

	fprintf(BatchFile, COMPILE_CVI_PROJECT_COMMAND);
	fprintf(BatchFile, TTIC2Direction);
	fprintf(BatchFile, TTIC2NAME);
	fprintf(BatchFile, PROJECT_SUFFIX);
	fprintf(BatchFile, WRITE_IN_TEXTFILE);
	fprintf(BatchFile, ADRESS_REPORT_COMPILER); 
	fprintf(BatchFile, NEWLINE);
	
	//Batch File wider schliessen
	fclose(BatchFile);
	
	//Batch File ausführen
	system(pAdressBatchFile);
	
	//Batch File löschen, um laufend ein neues erzeugen können, welches das nächste Projekt aufruft zum compilieren
	remove(pAdressBatchFile);
	
	//Auswertung Compiler
	ResultCompileProject(TTIC2NAME);
}

//******************************************************************************************************************************
//  Description     : Erstellt das Batch File für den Test Commander
//  Input           : 
//  Output *Pointer : 
//         *return  :  
//******************************************************************************************************************************
static void BatchFileTTIC2Commander(void)
{
	char pAdressBatchFile[MAX_PATHNAME_LEN];
	FILE *BatchFile;
	
	//Batch File Adresse definieren, im ProjektOrdner ablegen
	strcpy(pAdressBatchFile, ProjectDirection);
	strcat(pAdressBatchFile, BATCHFILENAME);
	
	//Batch File erzeugen
	BatchFile = fopen(pAdressBatchFile, "w+");
	
	fprintf(BatchFile, CHANGE_FILE_DIRECTION_COMMAND);
	fprintf(BatchFile, COMPILE_CVI_EXE_DIRECTION);
	fprintf(BatchFile, NEWLINE);

	fprintf(BatchFile, COMPILE_CVI_PROJECT_COMMAND);
	fprintf(BatchFile, TTIC2CommanderFolder);
	fprintf(BatchFile, TTIC2COMMANDER_NAME);
	fprintf(BatchFile, PROJECT_SUFFIX);
	fprintf(BatchFile, WRITE_IN_TEXTFILE);
	fprintf(BatchFile, ADRESS_REPORT_COMPILER); 
	fprintf(BatchFile, NEWLINE);
	
	//Batch File wider schliessen
	fclose(BatchFile);
	
	//Batch File ausführen
	system(pAdressBatchFile);
	
	//Batch File löschen, um laufend ein neues erzeugen können, welches das nächste Projekt aufruft zum compilieren
	remove(pAdressBatchFile);
	
	//Auswertung Compiler
	ResultCompileProject(TTIC2COMMANDER_NAME);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Kontrolliert anhand des Textfile vom Kommandofenster ob das Projekt fehlerfrei kompiliert werden konnte 
//  Input           : CountTests: Anzahl Tests die zur Verfügung stehen
//									:	
//  Output *Pointer : -
//         *return  : Gibt die Länge der Kompilierfehlerliste zurück 
//******************************************************************************************************************************
static void ResultCompileProject(char *pTestProjectName)
{
	//Textfile öffnen, inwelchem der Kompiliervorgang dokumentiert wurde
	static UINT8 EntryFunction = 0;
	static UINT16 CountErrorCompileProjectLengthMalloc = ADDITIONAL_ERROR_FILES, StartIndexListCompileError = 0, ActualIndexListCompileError = 0, ErrorCompileListLengthMalloc = NUMBER_CHAR_LIST_COMPILE_ERRORS;
	UINT16 SizeTextFile = 0, StrlenCompileErrorTest = 0; 
	char *pFoundSearchString;
	FILE *ReportCompileProcedure;
	
	if(EntryFunction == 0)
	{
		ppArrayFileListWithCompileErrors = (char **) malloc(sizeof(char*) * ADDITIONAL_ERROR_FILES);
		pListCompileErrors = (char *) malloc(sizeof(char) * NUMBER_CHAR_LIST_COMPILE_ERRORS);
		pListCompileErrors[0] = 0;
	}
	EntryFunction++;
	
	ReportCompileProcedure = fopen(ADRESS_REPORT_COMPILER, "r");
	fseek(ReportCompileProcedure, 0, SEEK_END);
	SizeTextFile = (UINT16)ftell(ReportCompileProcedure);
	fseek(ReportCompileProcedure, 0, SEEK_SET);
	
	char TextFileContent[SizeTextFile + 1];
	TextFileContent[SizeTextFile] = 0;
	
	fread(TextFileContent, sizeof(char), SizeTextFile, ReportCompileProcedure);
	
	pFoundSearchString = strstr(TextFileContent, "Build succeeded");

	//In Liste schreiben, in welchem die Test stehen welchen ein Fehler beim kompilieren aufgetreten ist
	if(pFoundSearchString == NULL)	
	{
		//In Liste schreiben um die Information nicht zu verlieren
		strcat(pListCompileErrors, pTestProjectName);
		StrlenCompileErrorTest = (UINT16)strlen(pListCompileErrors);
		//Abschlusszeichen miteinbeziehen
		ActualIndexListCompileError = StrlenCompileErrorTest + 1;
		//String abschlissen
		pListCompileErrors[ActualIndexListCompileError - 1] = ';';
		pListCompileErrors[ActualIndexListCompileError] = 0; 
		ErrorCompileListLength = ActualIndexListCompileError;
		
		//Die Adresse weitersetzen, um den nächsten Test abspeichern zu können
		StartIndexListCompileError = ActualIndexListCompileError;
		
		CountErrorCompileProject++;
		//dynamisch die Projekt Compile Fehler Liste vergrössern
		if((CountErrorCompileProjectLengthMalloc - CountErrorCompileProject) < 2)
		{
			CountErrorCompileProjectLengthMalloc = CountErrorCompileProjectLengthMalloc + ADDITIONAL_ERROR_FILES;
			ppArrayFileListWithCompileErrors = (char **) realloc(ppArrayFileListWithCompileErrors, sizeof(char*) * CountErrorCompileProjectLengthMalloc);
		}
		//dynamisch die Projekt Compile Liste, in welchem alle Test nacheinander hineingeschriben werden, vergrössern
		if(ErrorCompileListLengthMalloc - ErrorCompileListLength < 50)
		{
			 ErrorCompileListLengthMalloc = ErrorCompileListLengthMalloc + NUMBER_CHAR_LIST_COMPILE_ERRORS;
			 pListCompileErrors = (char *) realloc(pListCompileErrors, sizeof(char) * ErrorCompileListLengthMalloc);
		}
	}
	fclose(ReportCompileProcedure);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Zeigt die nicht kompilierbaren Projekte grafisch dar sowie, diese auch in ein Textfile abgelegt werden 
//  Input           : CountErrorCompileProject:	Gibt die Anzahl Projekte an, welche nicht kompilierbar waren
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void DisplayErrorCompileProject(void)
{
	int Hpanel, StatusUserInterface;
	UINT16 IndexErrorCompileProject, IndexListCompileError, StrlenCompileErrorTest, ActualIndexListCompileError = 0;
	char ErrorCompileFileAdress[MAX_PATHNAME_LEN];
	FILE *ErrorCompileFile;
	
	//Liste mit den Compile Fehlern der einzelnen Test unterteilen
	//Jetzt alle ; Zeichen mit Abschlusszeichen ersetzen
	for (IndexListCompileError = 0; IndexListCompileError < ErrorCompileListLength; IndexListCompileError++)
	{
		//Auf ; detektieren um einzelne Informationen zu trennen
		if (pListCompileErrors[IndexListCompileError] == ';')
		{
			pListCompileErrors[IndexListCompileError] = 0;
		}
	}	//end for
	
	IndexListCompileError = 0;
	//Die ganze Liste in die Array Liste eintragen
	for(IndexErrorCompileProject = 0; IndexErrorCompileProject < CountErrorCompileProject; IndexErrorCompileProject++)
	{
		ppArrayFileListWithCompileErrors[IndexErrorCompileProject] = &pListCompileErrors[ActualIndexListCompileError];
		StrlenCompileErrorTest = (UINT16)strlen(&pListCompileErrors[ActualIndexListCompileError]);
		ActualIndexListCompileError = ActualIndexListCompileError + StrlenCompileErrorTest + 1;
	}
	
	//Textfile erstellen in welchem die nicht kompilierbaren Projekte aufgelistet werden
	strcpy(ErrorCompileFileAdress, ProjectDirection);
	strcat(ErrorCompileFileAdress, ERROR_COMPILE_TEXTFILE);
	ErrorCompileFile = fopen(ErrorCompileFileAdress, "w+");
	
	//Oberfläche laden
	Hpanel = LoadPanel (0, "DisplayErrorCompileProject.uir", PANEL);

	DisplayPanel(Hpanel);
	
	for(IndexErrorCompileProject = 0; IndexErrorCompileProject < CountErrorCompileProject; IndexErrorCompileProject++)
	{
		//Auf die Oberfläche schreiben
		InsertTextBoxLine(Hpanel, PANEL_TEXTBOX_ERRORCOMPILE, IndexErrorCompileProject, ppArrayFileListWithCompileErrors[IndexErrorCompileProject]);
		fprintf(ErrorCompileFile, ppArrayFileListWithCompileErrors[IndexErrorCompileProject]);
		fprintf(ErrorCompileFile, "\n");
	}
	
	if(CountErrorCompileProject == 0)	
	{
		InsertTextBoxLine(Hpanel, PANEL_TEXTBOX_ERRORCOMPILE, IndexErrorCompileProject, "All Project Builds sucessful");
		fprintf(ErrorCompileFile, "All Project Builds sucessful");
	}
		
	StatusUserInterface = RunUserInterface();
	
	//Textfile wider schliessen
	fclose(ErrorCompileFile);
	DiscardPanel(Hpanel);
	//CloseCVIRTE ();
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Liest die aktuellen Parameter aus der Software aus 
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void ReadStructureFromFirmware(void)
{
	int result;
	//Ausführung des ParametertreeStructBuild
	char ReadStructureFromFirmwareFile[MAX_PATHNAME_LEN];
	
	strcpy(ReadStructureFromFirmwareFile, ParametertreeStructBuildDirection);
	strcat(ReadStructureFromFirmwareFile, READ_STRUCTURE_FIRMWARE_FILE_DIRECTION);
	
	result = system(ReadStructureFromFirmwareFile);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ersetzt die FirmwareStrutureFiles im Test Ordner 
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void ReplacementFirmwareStructureFiles(void)
{
	char StructCFile[MAX_PATHNAME_LEN], StructHFile[MAX_PATHNAME_LEN], StructCFileActual[MAX_PATHNAME_LEN], StructHFileActual[MAX_PATHNAME_LEN], StructCFileTTIC2[MAX_PATHNAME_LEN], StructHFileTTIC2[MAX_PATHNAME_LEN], StructCFileTTIC2Commander[MAX_PATHNAME_LEN], StructHFileTTIC2Commander[MAX_PATHNAME_LEN], *pRemoveSourceInString;

	//Aktuelles Strucktur Files, welche gearde aus dem Ventil ausgelesen wurden
	strcpy(StructCFileActual, ParametertreeStructBuildDirection);
	strcat(StructCFileActual, BETWEENCHAR);
	strcat(StructCFileActual, PARAMETER_TREE_STRUCT_BUILD);
	strcat(StructCFileActual, BETWEENCHAR);
	strcat(StructCFileActual, ControllerHardware);
	strcat(StructCFileActual, BETWEENCHAR);
	strcpy(StructHFileActual, StructCFileActual);
	strcat(StructCFileActual, STRUCTCFILE);
	strcat(StructHFileActual, STRUCTHFILE);
	
	//Geht durch alle Tests durch um den Pfad des Struckturfiles zu wissen
	strcpy(StructCFile, TestDirection);
	strcat(StructCFile, BETWEENCHAR);
	strcat(StructCFile, SHARED_GENERAL_FILES);
	strcat(StructCFile, BETWEENCHAR);
	strcpy(StructHFile, StructCFile);
	strcat(StructCFile, STRUCTCFILE);
	strcat(StructHFile, STRUCTHFILE);
	
	CopyFile(StructCFileActual, StructCFile);
	CopyFile(StructHFileActual, StructHFile);

	
	//Testoberfläche mit der neuen Parameter Strucktur ausgestatet werden
	strcpy(StructCFileTTIC2, TTIC2Direction);
	strcpy(StructHFileTTIC2, StructCFileTTIC2);
	strcat(StructCFileTTIC2, STRUCTCFILE);
	strcat(StructHFileTTIC2, STRUCTHFILE);
	
	CopyFile(StructCFileActual, StructCFileTTIC2);
	CopyFile(StructHFileActual, StructHFileTTIC2);
	
	//Kommunikationsoberfläche mit der neuen Parameter Strucktur ausgestatet werden
	
	//TTIC2Commander Folder 
	strcpy(TTIC2CommanderFolder, TTIC2Direction);
	pRemoveSourceInString = strstr(TTIC2CommanderFolder, "Source");
	if(pRemoveSourceInString != NULL)	*pRemoveSourceInString = 0;	
	strcat(TTIC2CommanderFolder, TTIC2COMMANDER_FOLDER);
	
	strcpy(StructCFileTTIC2Commander, TTIC2CommanderFolder);
	strcpy(StructHFileTTIC2Commander, StructCFileTTIC2Commander);
	strcat(StructCFileTTIC2Commander, STRUCTCFILE);
	strcat(StructHFileTTIC2Commander, STRUCTHFILE);
	
	CopyFile(StructCFileActual, StructCFileTTIC2Commander);
	CopyFile(StructHFileActual, StructHFileTTIC2Commander);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Ersetzt das Platform Config File im Test Ordner 
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void ReplacementPlatformConfigFile(void)
{
	char PlatformConfigHFile[MAX_PATHNAME_LEN], PlatformConfigHActual[MAX_PATHNAME_LEN], PlatformConfigHTTIC2[MAX_PATHNAME_LEN], PlatformConfigHTTIC2Commander[MAX_PATHNAME_LEN], *pRemoveSourceInString, FirmwareCode[MAX_PATHNAME_LEN] = "";

	HelpFunctions_GetFirmwareCodeString(FirmwareCode);
	
	//Aktuelles Strucktur Files, welche gearde aus dem Ventil ausgelesen wurden
	strcpy(PlatformConfigHActual, ParametertreeStructBuildDirection);
	strcat(PlatformConfigHActual, BETWEENCHAR);
	strcat(PlatformConfigHActual, PLATFORM_DIRECTION);
	strcat(PlatformConfigHActual, BETWEENCHAR);
	strcat(PlatformConfigHActual, FirmwareCode);
	strcat(PlatformConfigHActual, BETWEENCHAR);
	strcat(PlatformConfigHActual, PLATFORM_CONFIG_FILE);
	
	//Geht durch alle Tests durch um den Pfad des Struckturfiles zu wissen
	strcpy(PlatformConfigHFile, TestDirection);
	strcat(PlatformConfigHFile, BETWEENCHAR);
	strcat(PlatformConfigHFile, SHARED_FILES);
	strcat(PlatformConfigHFile, BETWEENCHAR);
	strcat(PlatformConfigHFile, PLATFORM_DIRECTION);
	strcat(PlatformConfigHFile, BETWEENCHAR);
	strcat(PlatformConfigHFile, PLATFORM_CONFIG_FILE);
	
	CopyFile(PlatformConfigHActual, PlatformConfigHFile);

	
	//Testoberfläche mit der neuen Parameter Strucktur ausgestatet werden
	strcpy(PlatformConfigHTTIC2, TTIC2Direction);
	strcat(PlatformConfigHTTIC2, PLATFORM_CONFIG_FILE);
	
	CopyFile(PlatformConfigHActual, PlatformConfigHTTIC2);
	
	//Kommunikationsoberfläche mit der neuen Parameter Strucktur ausgestatet werden
	
	//TTIC2Commander Folder 
	strcpy(TTIC2CommanderFolder, TTIC2Direction);
	pRemoveSourceInString = strstr(TTIC2CommanderFolder, "Source");
	if(pRemoveSourceInString != NULL)	*pRemoveSourceInString = 0;	
	strcat(TTIC2CommanderFolder, TTIC2COMMANDER_FOLDER);
	
	strcpy(PlatformConfigHTTIC2Commander, TTIC2CommanderFolder);
	strcat(PlatformConfigHTTIC2Commander, PLATFORM_CONFIG_FILE);
	
	CopyFile(PlatformConfigHActual, PlatformConfigHTTIC2Commander);
}

//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Updatet den exe Ordner mit den Files für die TTIC2 Oberfläche 
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void UpdateExeTTIC2Folder(void)
{
	char *pSourceName, ExeTTIC2Folder[MAX_PATHNAME_LEN], ChangeUIR[MAX_PATHNAME_LEN], ActualUIR[MAX_PATHNAME_LEN], TTIC2Exe[MAX_PATHNAME_LEN], TTIC2ExeActual[MAX_PATHNAME_LEN], TTIC2Ini[MAX_PATHNAME_LEN], TTIC2IniActual[MAX_PATHNAME_LEN];
	
	strcpy(ExeTTIC2Folder, TTIC2Direction);
	pSourceName = strstr(ExeTTIC2Folder, "Source");
	if(pSourceName != NULL)	*pSourceName = 0;	
	strcat(ExeTTIC2Folder, EXE_TTIC2_FOLDER);
	
	//UIR Files ersetzen
	
	//ChangeHardware.uir
	strcpy(ChangeUIR, ExeTTIC2Folder);
	strcat(ChangeUIR, CHANGE_HARDWARE_UIR);
	strcpy(ActualUIR, TTIC2Direction);
	strcat(ActualUIR, CHANGE_HARDWARE_UIR);
	CopyFile(ActualUIR, ChangeUIR);
	
	//CollectionTest.uir
	strcpy(ChangeUIR, ExeTTIC2Folder);
	strcat(ChangeUIR, COLLECTION_TEST_UIR);
	strcpy(ActualUIR, TTIC2Direction);
	strcat(ActualUIR, COLLECTION_TEST_UIR);
	CopyFile(ActualUIR, ChangeUIR);
	
	//Report.uir
	strcpy(ChangeUIR, ExeTTIC2Folder);
	strcat(ChangeUIR, REPORT_UIR);
	strcpy(ActualUIR, TTIC2Direction);
	strcat(ActualUIR, REPORT_UIR);
	CopyFile(ActualUIR, ChangeUIR);
	
	//SurfaceTest.uir
	strcpy(ChangeUIR, ExeTTIC2Folder);
	strcat(ChangeUIR, SURFACE_TEST_UIR);
	strcpy(ActualUIR, TTIC2Direction);
	strcat(ActualUIR, SURFACE_TEST_UIR);
	CopyFile(ActualUIR, ChangeUIR);
	
	//LoadTestInformation.uir
	strcpy(ChangeUIR, ExeTTIC2Folder);
	strcat(ChangeUIR, LOAD_TEST_INFORMATION_UIR);
	strcpy(ActualUIR, TTIC2Direction);
	strcat(ActualUIR, LOAD_TEST_INFORMATION_UIR);
	CopyFile(ActualUIR, ChangeUIR);
	
	//TTIC2 exe ersetzen
	strcpy(TTIC2Exe, ExeTTIC2Folder);
	strcat(TTIC2Exe, TTIC2_EXE);
	strcpy(TTIC2ExeActual, TTIC2Direction);
	strcat(TTIC2ExeActual, TTIC2_EXE);
	CopyFile(TTIC2ExeActual, TTIC2Exe);
	
	//TTIC2 ini ersetzen
	strcpy(TTIC2Ini, ExeTTIC2Folder);
	strcat(TTIC2Ini, TTIC2_INI);
	strcpy(TTIC2IniActual, TTIC2Direction);
	strcat(TTIC2IniActual, TTIC2_INI);
	CopyFile(TTIC2IniActual, TTIC2Ini);
}
+++
//******************************************************************************************************************************
//  Procedure
//******************************************************************************************************************************
//  Description     : Es werden alle Tests aufgerufen und die Funktion im Test aufgerufen, um die Testinformationen in der Datenbank zu kontrollieren
//  Input           : 
//									:	
//  Output *Pointer : -
//         *return  :  
//******************************************************************************************************************************
static void CheckIfTestHardwareIsActuall(BOOL8 *pTestHardwareCompatibleList)
{
	UINT16 IndexTest, ControllerHardwareValue;
	char TestExeFile[MAX_PATHNAME_LEN];
	
	HelpFunctions_InitGlobalVariableTestOptions();
	
	ControllerHardwareValue = HelpFunctions_GetControllerHardwareValue();
	SharedVariable_InitNetworkvariableTestHardware();
	
	//Zuerst müssen alle TestHardwareTypes kontrolliert werden, ob sie noch aktuell in der Datenbank sind
	Database_CheckAllHardwareTypes();
	
	//Bit 0 setzen für Info In Database Funktion im Test ausführen -> passiert für alle Tests
	HelpFunctions_WriteGlobalVariableTestOptions(INFO_OPTION_INDEX, "1");
	
	Delay(1);

	//Test aufrufem um die Testinformationen in der Datenbank zu aktuallisieren
	for(IndexTest = 0; IndexTest < CountTests; IndexTest++)
	{
		strcpy(TestExeFile, TestDirection);
		strcat(TestExeFile, BETWENCHAR);
		strcat(TestExeFile, ppArrayFileList[IndexTest]);
		strcat(TestExeFile, BETWENCHAR);
		strcat(TestExeFile, ppArrayFileList[IndexTest]);
		strcat(TestExeFile, ".exe");
		system(TestExeFile);
		
		//Testinformationen in der Datenbank aktuallisieren
		Database_CheckAllTestInformation();

		if(TestController == -1 || ControllerHardwareValue == (UINT16)TestController)	pTestHardwareCompatibleList[IndexTest] = TRUE;
		else																																					pTestHardwareCompatibleList[IndexTest] = FALSE;
	}
	
	//>Anschliessend wieder das Bit zurück setzen
	HelpFunctions_WriteGlobalVariableTestOptions(INFO_OPTION_INDEX, "0");
	
	SharedVariable_EndNetworkvariableTestHardware();
}
---