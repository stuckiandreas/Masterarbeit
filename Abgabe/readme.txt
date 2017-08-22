Die Abgabe ist in folgende Punkte unterteilt:
-ETIC2: Auswertungsprogramm (C#)
-TTIC2: Testoberfläche (Auszug C-Code, welcher bearbeitet wurde)
-Test: Ein Beispieltext, um zu zeigen, was im C-Code bearbeitet wurde
-TestUpdateFirmware: Aktuallisierung der Testinformationen (Auszug C-Code, welcher bearbeitet wurde)
-SQL: Skript, welches die Datenbankfelder erzeugt
-Dokumentation: Beschreibung der Masterarbeit

Wichtig:

Editor:
-Editor Einstellung Tabulatorbreite 2 (Unter Notepade++: Einstellungen/Optionen/Sprache/Tabularbreite 2)
-Sprache master thesis code einfügen 
	(Unter Notepade++: Sprachen/Benutzerdefinierte Sprache/Importieren -> master thesis code.uml auswählen)
	(Blockanfang mit ++, Blockende mit -- signalisiert)
	-> Den Code, welcher für die Masterarbeit erstellt wurde, wird grün hervorgehoben

ETIC2:
-zuerst muss die Datenbanktabelle erzeugt werden (Skript mit Daten SoftwareVersionsDatabase_schema_data.sql)
-Datenbankeinstellungen anpassen (VisualStudio/Project/Properties unter Settings DatabaseName und ServerName anpassen)
	(alternativ im laufendem Programm: Menuliste/Program/Settings, anschliessend muss der Refresh Knopf gedrückt werden um die Daten zu laden)
