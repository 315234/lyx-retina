/*

LyX Installer Language File
Language: German
Author: Uwe St�hr

*/

!insertmacro LANGFILE_EXT "German"

${LangFileString} TEXT_INSTALL_CURRENTUSER "(Installiert f�r den aktuellen Benutzer)"

${LangFileString} TEXT_WELCOME "Dieser Assistent wird Sie durch die Installation von $(^NameDA) begleiten.$\r$\n\
				$\r$\n\
				$_CLICK"

${LangFileString} TEXT_CONFIGURE_MIKTEXFNDB "Aktualisiere MiKTeXs Dateinamen Datenbank..."
${LangFileString} TEXT_CONFIGURE_LYX "Konfiguriere LyX (MiKTeX kann fehlende Pakte herunterladen, was etwas Zeit braucht) ..."
${LangFileString} TEXT_CONFIGURE_PYTHON "Kompiliere Python Skripte..."

${LangFileString} TEXT_FINISH_DESKTOP "Ein Symbol auf der Arbeitsoberfl�che erzeugen"
${LangFileString} TEXT_FINISH_WEBSITE "Besuchen Sie lyx.org f�r aktuelle Neuigkeiten"

${LangFileString} FileTypeTitle "LyX-Dokument"

${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "F�r alle Nutzer installieren?"
${LangFileString} SecFileAssocTitle "Dateizuordnungen"
${LangFileString} SecDesktopTitle "Desktopsymbol"

${LangFileString} SecCoreDescription "Das Programm LyX."
${LangFileString} SecInstJabRefDescription "Manager f�r bibliografische Referenzen und Editor f�r BibTeX-Dateien."
${LangFileString} SecAllUsersDescription "LyX f�r alle Nutzer oder nur f�r den aktuellen Nutzer installieren."
${LangFileString} SecFileAssocDescription "Vern�pfung zwischen LyX und der .lyx Dateiendung."
${LangFileString} SecDesktopDescription "Verkn�pfung zu LyX auf dem Desktop."

${LangFileString} EnterLaTeXHeader1 'LaTeX-Distribution'
${LangFileString} EnterLaTeXHeader2 'Legen Sie die LaTeX-Distribution fest die LyX verwenden soll.'
${LangFileString} EnterLaTeXFolder 'Optional k�nnen Sie hier den Pfad zur Datei $\"latex.exe$\" angeben und damit die \
					   LaTeX-Distribution festlegen die LyX verwenden soll.$\r$\n\
					   Wenn Sie kein LaTeX verwenden, kann LyX keine Dokumente ausgeben!$\r$\n\
					   $\r$\n\
					   Der Installer hat auf Ihrem System die LaTeX-Distribution$\r$\n\
					   $\"$LaTeXName$\" erkannt. Unten angegeben ist ihr Pfad.'
${LangFileString} EnterLaTeXFolderNone 'Geben Sie unten den Pfad zur Datei $\"latex.exe$\" an. Damit legen Sie fest welche \
					       LaTeX-Distribution LyX verwenden soll.$\r$\n\
					       Wenn Sie kein LaTeX verwenden, kann LyX keine Dokumente ausgeben!$\r$\n\
					       $\r$\n\
					       Der Installer konnte auf Ihrem System keine LaTeX-Distribution finden.'
${LangFileString} PathName 'Pfad zur Datei $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "Kein LaTeX benutzen"
${LangFileString} InvalidLaTeXFolder 'Kann die Datei $\"latex.exe$\" nicht finden.'

${LangFileString} LatexInfo 'Als N�chstes wird der Installer der LaTeX-Distribution $\"MiKTeX$\" gestartet.$\r$\n\
			            Um das Programm zu installieren, dr�cken Sie den $\"Weiter$\"-Knopf in den Installerfenstern bis die Installation beginnt.$\r$\n\
				    $\r$\n\
				    !!! Bitte verwenden Sie alle voreingestellten Optionen des MiKTeX-Installers !!!'
${LangFileString} LatexError1 'Es konnte keine LaTeX-Distribution gefunden werden!$\r$\n\
                      		      LyX kann ohne eine LaTeX-Distribution wie z.B. $\"MiKTeX$\" nicht benutzt werden!$\r$\n\
				      Die Installation wird daher abgebrochen.'

${LangFileString} HunspellFailed 'Herunterladen des W�rterbuchs f�r Sprache $\"$R3$\" fehlgeschlagen.'
${LangFileString} ThesaurusFailed 'Herunterladen des Thesaurus f�r Sprache $\"$R3$\" fehlgeschlagen.'

${LangFileString} JabRefInfo 'Als N�chstes wird der Installer des Programms $\"JabRef$\" gestartet.$\r$\n\
			             Sie k�nnen alle voreingestellten Optionen des JabRef-Installers verwenden.'
${LangFileString} JabRefError 'Das Programm $\"JabRef$\" konnte nicht erfolgreich installiert werden!$\r$\n\
		                      Der Installer wird trotzdem fortgesetzt.$\r$\n\
				      Versuchen Sie JabRef sp�ter noch einmal zu installieren.'

${LangFileString} LatexConfigInfo "Die folgende Konfiguration von LyX wird eine Weile dauern."

${LangFileString} MiKTeXPathInfo "Damit jeder Benutzer sp�ter MiKTeX f�r seine Bed�rfnisse anpassen kann,$\r$\n\
					 ist es erforderlich f�r MiKTeXs Installationsordner$\r$\n\
					 $MiKTeXPath $\r$\n\
					 und seine Unterordner Schreibrechte f�r alle Benutzer zu setzen."
${LangFileString} MiKTeXInfo 'Die LaTeX-Distribution $\"MiKTeX$\" wird zusammen mit LyX verwendet werden.$\r$\n\
				     Es wird empfohlen dass Sie verf�gbare MiKTeX-Updates mit Hilfe des Programms $\"MiKTeX Update Wizard$\"$\r$\n\
				     installieren, bevor sie LyX das erste Mal benutzen.$\r$\n\
				     M�chten Sie jetzt nach Updates f�r MiKTeX suchen?'

${LangFileString} UpdateNotAllowed "Dieses Update-Paket kann nur ${PRODUCT_VERSION_OLD} updaten!"
${LangFileString} ModifyingConfigureFailed 'Der $\"PATH_pr�fix$\" (Liste mit Programmpfaden) konnte nicht im Konfigurationsskript gesetzt werden.'
${LangFileString} RunConfigureFailed "Konnte das Konfigurationsskript nicht ausf�hren."
${LangFileString} NotAdmin "Sie ben�tigen Administratorrechte um LyX zu installieren!"
${LangFileString} InstallRunning "Der Installer l�uft bereits!"
${LangFileString} StillInstalled "LyX ist bereits installiert! Deinstallieren Sie LyX zuerst."

${LangFileString} FinishPageMessage "Gl�ckwunsch! LyX wurde erfolgreich installiert.$\r$\n\
					    $\r$\n\
					    (Der erste Start von LyX kann etwas l�nger dauern.)"
${LangFileString} FinishPageRun "LyX starten"

${LangFileString} UnNotInRegistryLabel "Kann LyX nicht in der Registry finden.$\r$\n\
					       Desktopsymbole und Eintr�ge im Startmen� k�nnen nicht entfernt werden."
${LangFileString} UnInstallRunning "Sie m�ssen LyX zuerst beenden!"
${LangFileString} UnNotAdminLabel "Sie ben�tigen Administratorrechte um LyX zu deinstallieren!"
${LangFileString} UnReallyRemoveLabel "Sind Sie sicher, dass sie LyX und all seine Komponenten deinstallieren m�chten?"
${LangFileString} UnLyXPreferencesTitle 'LyXs Benutzereinstellungen'

${LangFileString} SecUnMiKTeXDescription "Deinstalliert die LaTeX-Distribution MiKTeX."
${LangFileString} SecUnJabRefDescription "Deinstalliert den Bibliografiemanager JabRef."
${LangFileString} SecUnPreferencesDescription 'L�scht LyXs Benutzereinstellungen,$\r$\n\
						      (den Ornder:$\r$\n\
						      $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\")$\r$\n\
						      f�r alle Benutzer.'
${LangFileString} SecUnProgramFilesDescription "Deinstalliert LyX und all seine Komponenten."

