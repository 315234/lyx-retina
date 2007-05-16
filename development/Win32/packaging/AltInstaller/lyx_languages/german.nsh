!ifndef _LYX_LANGUAGES_GERMAN_NSH_
!define _LYX_LANGUAGES_GERMAN_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_GERMAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Dieser Assistent wird Sie durch die Installation von LyX begleiten.\r\n\
					  \r\n\
					  Sie ben�tigen Administratorrechte um LyX zu installieren.\r\n\
					  \r\n\
					  Es wird empfohlen vor der Installation alle anderen Programme zu schlie�en, damit bestimmte Systemdateien ohne Neustart ersetzt werden k�nnen.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-Dokument"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "F�r alle Nutzer installieren?"
LangString SecFileAssocTitle "${LYX_LANG}" "Dateizuordnungen"
LangString SecDesktopTitle "${LYX_LANG}" "Desktopsymbol"

LangString SecCoreDescription "${LYX_LANG}" "Das Programm LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Programm zum Betrachten von Postscript- und PDF-Dokumenten."
LangString SecInstJabRefDescription "${LYX_LANG}" "Manager f�r bibliografische Referenzen und Editor f�r BibTeX-Dateien."
LangString SecAllUsersDescription "${LYX_LANG}" "LyX f�r alle Nutzer oder nur f�r den aktuellen Nutzer installieren."
LangString SecFileAssocDescription "${LYX_LANG}" "Vern�pfung zwischen LyX und der .lyx Dateiendung."
LangString SecDesktopDescription "${LYX_LANG}" "Verkn�pfung zu LyX auf dem Desktop."

LangString LangSelectHeader "${LYX_LANG}" "Wahl der Men�sprache von LyX"
LangString AvailableLang "${LYX_LANG}" " Verf�gbare Sprachen "

LangString MissProgHeader "${LYX_LANG}" "Suche nach ben�tigten Programmen"
LangString MissProgCap "${LYX_LANG}" "Es werden folgende ben�tigte Programme zus�tzlich installiert"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, eine LaTeX-Distribution"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, ein Interpreter f�r PostScript und PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, ein Bildkonverter"
LangString MissProgAspell "${LYX_LANG}" "Aspell, eine Rechtschreibpr�fung"
LangString MissProgMessage "${LYX_LANG}" 'Es m�ssen keine zus�tzlichen Programme installiert werden.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'LaTeX-Distribution'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Legen Sie die LaTeX-Distribution fest die LyX verwenden soll.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Optional k�nnen Sie hier den Pfad zur Datei "latex.exe" angeben und damit die \
					   LaTeX-Distribution festlegen die LyX verwenden soll.\r\n\
					   Wenn Sie kein LaTeX verwenden, kann LyX keine Dokumente ausgeben!\r\n\
					   \r\n\
					   Der Installer hat auf Ihrem System die LaTeX-Distribution\r\n\
					   "$LaTeXName" erkannt. Unten angegeben ist ihr Pfad.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Geben Sie unten den Pfad zur Datei "latex.exe" an. Damit legen Sie fest welche \
					       LaTeX-Distribution LyX verwenden soll.\r\n\
					       Wenn Sie kein LaTeX verwenden, kann LyX keine Dokumente ausgeben!\r\n\
					       \r\n\
					       Der Installer konnte auf Ihrem System keine LaTeX-Distribution finden.'
LangString PathName "${LYX_LANG}" 'Pfad zur Datei "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Kein LaTeX benutzen"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Kann die Datei "latex.exe" nicht finden.'

LangString LatexInfo "${LYX_LANG}" 'Als N�chstes wird der Installer der LaTeX-Distribution "MiKTeX" gestartet.$\r$\n\
			            Um das Programm zu installieren, dr�cken Sie den "Weiter"-Knopf in den Installerfenstern bis die Installation beginnt.$\r$\n\
				    $\r$\n\
				    !!! Bitte verwenden Sie alle voreingestellten Optionen des MiKTeX-Installers !!!'
LangString LatexError1 "${LYX_LANG}" 'Es konnte keine LaTeX-Distribution gefunden werden!$\r$\n\
                      		      LyX kann ohne eine LaTeX-Distribution wie z.B. "MiKTeX" nicht benutzt werden!$\r$\n\
				      Die Installation wird daher abgebrochen.'
				    
LangString GSviewInfo "${LYX_LANG}" 'Als N�chstes wird der Installer des Programms "GSview" gestartet.$\r$\n\
			             Um das Programm zu installieren, dr�cken Sie den "Setup"-Knopf im ersten Installerfenster,$\r$\n\
				     w�hlen eine Sprache und dr�cken dann den "Weiter"-Knopf in den folgenden Installerfenstern.$\r$\n\
				     Sie k�nnen alle voreingestellten Optionen des GSview-Installers verwenden.'
LangString GSviewError "${LYX_LANG}" 'Das Programm "GSview" konnte nicht erfolgreich installiert werden!$\r$\n\
		                      Die Installation wird trotzdem fortgesetzt.$\r$\n\
				      Versuchen Sie GSview sp�ter noch einmal zu installieren.'
				      
LangString JabRefInfo "${LYX_LANG}" 'Als N�chstes wird der Installer des Programms "JabRef" gestartet.$\r$\n\
			             Sie k�nnen alle voreingestellten Optionen des JabRef-Installers verwenden.'
LangString JabRefError "${LYX_LANG}" 'Das Programm "JabRef" konnte nicht erfolgreich installiert werden!$\r$\n\
		                      Der Installer wird trotzdem fortgesetzt.$\r$\n\
				      Versuchen Sie JabRef sp�ter noch einmal zu installieren.'

LangString LatexConfigInfo "${LYX_LANG}" "Die folgende Konfiguration von LyX wird eine Weile dauern."

LangString AspellInfo "${LYX_LANG}" 'Es werden nun W�rterb�cher f�r die Rechtschreibpr�fung "Aspell" heruntergeladen und installiert.$\r$\n\
				     Jedes W�rterbuch hat andere Lizenzbedingungen die Sie vor der Installation angezeigt bekommen.'
LangString AspellDownloadFailed "${LYX_LANG}" "Das W�rterbuch konnte nicht heruntergeladen werden!"
LangString AspellInstallFailed "${LYX_LANG}" "Das W�rterbuch konnte nicht installiert werden!"
LangString AspellPartAnd "${LYX_LANG}" " und "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Es ist bereits ein W�rterbuch der Sprache'
LangString AspellNone "${LYX_LANG}" 'Es wurde kein W�rterbuch f�r die Rechtschreibpr�fung "Aspell" installiert.$\r$\n\
				     W�rterb�cher k�nnen von$\r$\n\
				     ${AspellLocation}$\r$\n\
				     heruntergeladen werden. Wollen Sie jetzt W�rterb�cher herunterladen?'
LangString AspellPartStart "${LYX_LANG}" "Es wurde erfolgreich "
LangString AspellPart1 "${LYX_LANG}" "ein englisches W�rterbuch"
LangString AspellPart2 "${LYX_LANG}" "ein W�rterbuch der Sprache $LangName"
LangString AspellPart3 "${LYX_LANG}" "ein$\r$\n\
				      W�rterbuch der Sprache $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' f�r die Rechtschreibpr�fung "Aspell" installiert.$\r$\n\
				      Weitere W�rterb�cher k�nnen von$\r$\n\
				      ${AspellLocation}$\r$\n\
				      heruntergeladen werden. Wollen Sie jetzt andere W�rterb�cher herunterladen?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Damit jeder Benutzer sp�ter MiKTeX f�r seine Bed�rfnisse anpassen kann,$\r$\n\
					 ist es erforderlich f�r MiKTeXs Installationsordner$\r$\n\
					 $MiKTeXPath $\r$\n\
					 und seine Unterordner Schreibrechte f�r alle Benutzer zu setzen."
LangString MiKTeXInfo "${LYX_LANG}" 'Die LaTeX-Distribution "MiKTeX" wird zusammen mit LyX verwendet werden.$\r$\n\
				     Es wird empfohlen dass Sie verf�gbare MiKTeX-Updates mit Hilfe des Programms "MiKTeX Update Wizard"$\r$\n\
				     installieren, bevor sie LyX das erste Mal benutzen.$\r$\n\
				     M�chten Sie jetzt nach Updates f�r MiKTeX suchen?'

LangString UpdateNotAllowed "${LYX_LANG}" "Dieses Update-Paket kann nur ${PRODUCT_VERSION_OLD} updaten!"
LangString ModifyingConfigureFailed "${LYX_LANG}" 'Der "PATH_pr�fix" (Liste mit Programmpfaden) konnte nicht im Konfigurationsskript gesetzt werden.'
LangString CreateCmdFilesFailed "${LYX_LANG}" "Konnte die Datei lyx.bat nicht erzeugen."
LangString RunConfigureFailed "${LYX_LANG}" "Konnte das Konfigurationsskript nicht ausf�hren."
LangString NotAdmin "${LYX_LANG}" "Sie ben�tigen Administratorrechte um LyX zu installieren!"
LangString InstallRunning "${LYX_LANG}" "Der Installer l�uft bereits!"
LangString StillInstalled "${LYX_LANG}" "LyX ist bereits installiert! Deinstallieren Sie LyX zuerst."

LangString FinishPageMessage "${LYX_LANG}" "Gl�ckwunsch! LyX wurde erfolgreich installiert.\r\n\
					    \r\n\
					    (Der erste Start von LyX kann etwas l�nger dauern.)"
LangString FinishPageRun "${LYX_LANG}" "LyX starten"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Kann LyX nicht in der Registry finden.$\r$\n\
					       Desktopsymbole und Eintr�ge im Startmen� k�nnen nicht entfernt werden."
LangString UnInstallRunning "${LYX_LANG}" "Sie m�ssen LyX zuerst beenden!"
LangString UnNotAdminLabel "${LYX_LANG}" "Sie ben�tigen Administratorrechte um LyX zu deinstallieren!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Sind Sie sicher, dass sie LyX und all seine Komponenten deinstallieren m�chten?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyXs Benutzereinstellungen'
LangString UnGSview "${LYX_LANG}" 'Bitte dr�cken Sie im n�chsten Fenster den Knopf "Uninstall" um das$\r$\n\
				   Postscript und PDF-Betrachtungsprogramm "GSview" zu deinstallieren.'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX wurde erfolgreich von ihrem Computer entfernt."

LangString SecUnAspellDescription "${LYX_LANG}" "Deinstalliert die Rechtschreibpr�fung Aspell und all seine W�rterb�cher."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Deinstalliert die LaTeX-Distribution MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Deinstalliert den Bibliografiemanager JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'L�scht LyXs Benutzereinstellungen,$\r$\n\
						      (den Ornder:$\r$\n\
						      "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}")$\r$\n\
						      f�r alle Benutzer.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Deinstalliert LyX und all seine Komponenten."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_GERMAN_NSH_
