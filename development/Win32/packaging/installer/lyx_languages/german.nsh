!ifndef _LYX_LANGUAGES_GERMAN_NSH_
!define _LYX_LANGUAGES_GERMAN_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_GERMAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString SecAllUsersTitle "${LYX_LANG}" "F�r alle Nutzer installieren?"
LangString SecFileAssocTitle "${LYX_LANG}" "Dateizuordnungen"
LangString SecDesktopTitle "${LYX_LANG}" "Desktopsymbol"

LangString SecCoreDescription "${LYX_LANG}" "Das Programm ${PRODUCT_NAME}."
LangString SecAllUsersDescription "${LYX_LANG}" "F�r alle Nutzer oder nur f�r den aktuellen Nutzer installieren. (Dazu werden Administratorrechte ben�tigt.)"
LangString SecFileAssocDescription "${LYX_LANG}" "Vern�pfung zwischen ${PRODUCT_NAME} und der .lyx Dateiendung."
LangString SecDesktopDescription "${LYX_LANG}" "Verkn�pfung zu ${PRODUCT_NAME} auf dem Desktop."

LangString ModifyingConfigureFailed "${LYX_LANG}" "Der 'path_prefix' (Liste mit Programmpfaden) konnte nicht im Konfigurationsskript gesetzt werden."
LangString CreateCmdFilesFailed "${LYX_LANG}" "Konnte die Dateien lyx.cmd und reLyX.cmd nicht erstellen."
LangString RunConfigureFailed "${LYX_LANG}" "Konnte das Konfigurationsskript nicht ausf�hren."

LangString FinishPageMessage "${LYX_LANG}" "LyX sollte nun installiert sein. Jedoch haben einige Nutzer berichtet, dass das Konfigurationsskript nicht ausgef�hrt werden konnte und LyX daher nicht startet. Die Fehlermeldung lautet in diesem Fall $\'reporting a missing textclass.lst file$\'. Um den Fehler zu beheben, f�hren Sie bitte den Befehl $\'sh configure$\' aus Windows Kommandozeile aus, wenn Sie sich im Verzeichnis $\'<LyX>\Resources\lyx$\' befinden."
LangString FinishPageRun "${LYX_LANG}" "LyX starten"

LangString DownloadPageField2 "${LYX_LANG}" "&Nicht installieren"

LangString MinSYSHeader "${LYX_LANG}" "MinSYS"
LangString MinSYSDescription "${LYX_LANG}" "MinSYS ist eine Unix-Umgebung f�r Skripte (www.mingw.org/msys.shtml) die von ${PRODUCT_NAME} f�r das Ausf�hren verschiedener Skripte ben�tigt wird."
LangString EnterMinSYSFolder "${LYX_LANG}" "Geben sie den Pfad zur Datei sh.exe an."
LangString InvalidMinSYSFolder "${LYX_LANG}" "Kann die Datei sh.exe nicht finden."
LangString MinSYSDownloadLabel "${LYX_LANG}" "&MinSYS herunterladen"
LangString MinSYSFolderLabel "${LYX_LANG}" "&Ordner der die Datei sh.exe enth�lt"

LangString PythonHeader "${LYX_LANG}" "Python"
LangString PythonDescription "${LYX_LANG}" "Ein Interpreter der Skriptsprache Python (www.python.org) ist notwendig damit ${PRODUCT_NAME} Skripte ausf�hren kann."
LangString EnterPythonFolder "${LYX_LANG}" "Geben sie den Pfad zur Datei python.exe an."
LangString InvalidPythonFolder "${LYX_LANG}" "Kann die Datei python.exe nicht finden."
LangString PythonDownloadLabel "${LYX_LANG}" "&Python herunterladen"
LangString PythonFolderLabel "${LYX_LANG}" "&Ordner der die Datei python.exe enth�lt"

LangString MiKTeXHeader "${LYX_LANG}" "MiKTeX"
LangString MiKTeXDescription "${LYX_LANG}" "MiKTeX (www.miktex.org) ist eine LaTeX-Distribution f�r Windows."
LangString EnterMiKTeXFolder "${LYX_LANG}" "Geben sie den Pfad zur Datei latex.exe an."
LangString InvalidMiKTeXFolder "${LYX_LANG}" "Kann die Datei latex.exe nicht finden."
LangString MiKTeXDownloadLabel "${LYX_LANG}" "&MiKTeX herunterladen"
LangString MiKTeXFolderLabel "${LYX_LANG}" "&Ordner der die Datei latex.exe enth�lt"


LangString ImageMagickHeader "${LYX_LANG}" "ImageMagick"
LangString ImageMagickDescription "${LYX_LANG}" "ImageMagick (www.imagemagick.org/script/index.php) wird ben�tigt, um Grafikdateien in verschiedene Dateiformate umzuwandeln."
LangString EnterImageMagickFolder "${LYX_LANG}" "Geben sie den Pfad zur Datei convert.exe an."
LangString InvalidImageMagickFolder "${LYX_LANG}" "Kann die Datei convert.exe nicht finden."
LangString ImageMagickDownloadLabel "${LYX_LANG}" "&ImageMagic herunterladen"
LangString ImageMagickFolderLabel "${LYX_LANG}" "&Ordner der die Datei convert.exe enth�lt"

LangString GhostscriptHeader "${LYX_LANG}" "Ghostscript"
LangString GhostscriptDescription "${LYX_LANG}" "Ghostscript (http://www.cs.wisc.edu/~ghost/) wird f�r PDF- und PostScript-Grafikdateien ben�tigt."
LangString EnterGhostscriptFolder "${LYX_LANG}" "Geben sie den Pfad zur Datei gswin32c.exe an."
LangString InvalidGhostscriptFolder "${LYX_LANG}" "Kann die Datei gswin32c.exe nicht finden."
LangString GhostscriptDownloadLabel "${LYX_LANG}" "&Ghostscript herunterladen"
LangString GhostscriptFolderLabel "${LYX_LANG}" "&Ordner der die Datei gswin32c.exe enth�lt"

LangString SummaryTitle "${LYX_LANG}" "Software Zusammenfassung"
LangString SummaryPleaseInstall "${LYX_LANG}" "Bitte installieren sie die heruntergeladenen Programme und starten dann ${PRODUCT_NAME}'s Installer nochmal."
LangString SummaryPathPrefix "${LYX_LANG}" "Die Liste mit den Programmpfaden, der so genannte 'path_prefix' wird zur Datei 'lyxrc.defaults' hinzugef�gt:"

LangString UILangageTitle "${LYX_LANG}" "Die Sprache von LyXs Benutzeroberfl�che"
LangString UILangageDescription "${LYX_LANG}" "Wird f�r die Men�s, Mitteilungen, usw. verwendet"
LangString UILanguageAvailableLanguages "${LYX_LANG}" " Verf�gbare Sprachen "

LangString UnNotInRegistryLabel "${LYX_LANG}" "Kann $(^Name) nicht in der Registry finden. $\r$\n Desktopsymbole und Eintr�ge im Startmen� k�nnen nicht entfernt werden."
LangString UnNotAdminLabel "${LYX_LANG}" "Sie ben�tigen Administratorrechte $\r$\n um $(^Name) zu deinstallieren."
LangString UnReallyRemoveLabel "${LYX_LANG}" "Sind Sie sicher, dass sie $(^Name) und all seine Komponenten deinstalliern m�chten?"
LangString UnRemoveSuccessLabel "${LYX_LANG}" "$(^Name) wurde erfolgreich von ihrem Computer entfernt."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_GERMAN_NSH_
