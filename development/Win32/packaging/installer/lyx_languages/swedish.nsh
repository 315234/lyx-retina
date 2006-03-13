!ifndef _LYX_LANGUAGES_SWEDISH_NSH_
!define _LYX_LANGUAGES_SWEDISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_SWEDISH}

!define SwedishInputFolderPath "Var god ange s�kv�g till katalogen som inneh�ller"
!define SwedishUnableToFind "Kan inte hitta"
!define SwedishDownload "&Ladda ner"
!define SwedishFolderContaining "&Katalog inneh�llande"

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString SecAllUsersTitle "${LYX_LANG}" "Installera f�r alla anv�ndare?"
LangString SecFileAssocTitle "${LYX_LANG}" "Program associerade till olika filtyper" 
LangString SecDesktopTitle "${LYX_LANG}" "Skrivbordsikon"

LangString SecCoreDescription "${LYX_LANG}" "${PRODUCT_NAME}-filer."
LangString SecAllUsersDescription "${LYX_LANG}" "Installera f�r alla anv�ndare, eller enbart f�r den aktuella anv�ndare. (Kr�ver administrat�rsr�ttigheter.)"
LangString SecFileAssocDescription "${LYX_LANG}" "Skapa en association mellan programmet och filtill�get .lyx."
LangString SecDesktopDescription "${LYX_LANG}" "Skapa en ${PRODUCT_NAME}-ikon p� skrivbordet."

LangString ModifyingConfigureFailed "${LYX_LANG}" "Misslyckades med att s�tta s�kv�gsprefixet i konfigurationsprogrammet"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Misslyckades med att skapa filerna lyx.cmd och reLyX.cmd"
LangString RunConfigureFailed "${LYX_LANG}" "Misslyckades med att k�ra konfigurationsprogrammet"

LangString FinishPageMessage "${LYX_LANG}" "LyX borde nu ha installerats. Dessv�rre har en del anv�ndare rapporterat att konfigurationsskriptet ibland misslyckats. Om detta h�nder �r inte LyX ordentligt konfigurerat och v�grar starta med ett felmeddelande om att filen 'textclass.lst' saknas. F�r att �tg�rda detta problem, �ppna ett kommandof�nster (k�r programmet 'cmd'), och g� till katalogen 'LyX/Resources directory'. I den katalogen k�r du sedan kommandot 'sh configure' vilket kommer att konfigurar LyX."
LangString FinishPageRun "${LYX_LANG}" "Starta LyX"

LangString DownloadPageField2 "${LYX_LANG}" "Installera &ej"

LangString MinSYSHeader "${LYX_LANG}" "MinSYS"
LangString MinSYSDescription "${LYX_LANG}" "MinSYS �r en minimal, UNIX-liknande skriptmilj� (www.mingw.org/msys.shtml) vilken ${PRODUCT_NAME} anv�nder f�r ett flertal funktioner."
LangString EnterMinSYSFolder "${LYX_LANG}" "${SwedishInputFolderPath} sh.exe"
LangString InvalidMinSYSFolder "${LYX_LANG}" "${SwedishUnableToFind} sh.exe"
LangString MinSYSDownloadLabel "${LYX_LANG}" "${SwedishDownload} MinSYS"
LangString MinSYSFolderLabel "${LYX_LANG}" "${SwedishFolderContaining} sh.exe"

LangString PythonHeader "${LYX_LANG}" "Python"
LangString PythonDescription "${LYX_LANG}" "Skriptspr�ket Python (www.python.org) b�r installeras, annars kan ett flertal skript inte anv�ndas av ${PRODUCT_NAME}." 
LangString EnterPythonFolder "${LYX_LANG}" "${SwedishInputFolderPath} Python.exe"
LangString InvalidPythonFolder "${LYX_LANG}" "${SwedishUnableToFind} Python.exe"
LangString PythonDownloadLabel "${LYX_LANG}" "${SwedishDownload} Python"
LangString PythonFolderLabel "${LYX_LANG}" "${SwedishFolderContaining} Python.exe"

LangString MiKTeXHeader "${LYX_LANG}" "MiKTeX"
LangString MiKTeXDescription "${LYX_LANG}" "MiKTeX (www.miktex.org) �r en modern implementering av TeX f�r Windows."
LangString EnterMiKTeXFolder "${LYX_LANG}" "${SwedishInputFolderPath} latex.exe"
LangString InvalidMiKTeXFolder "${LYX_LANG}" "${SwedishUnableToFind} latex.exe"
LangString MiKTeXDownloadLabel "${LYX_LANG}" "${SwedishDownload} MiKTeX"
LangString MiKTeXFolderLabel "${LYX_LANG}" "${SwedishFolderContaining} latex.exe"


LangString ImageMagickHeader "${LYX_LANG}" "ImageMagick"
LangString ImageMagickDescription "${LYX_LANG}" "Verktygen i paketet ImageMagick (www.imagemagick.org/script/index.php) anv�nds av LyX f�r att konvertera bilder till r�tt filformat."
LangString EnterImageMagickFolder "${LYX_LANG}" "${SwedishInputFolderPath} convert.exe"
LangString InvalidImageMagickFolder "${LYX_LANG}" "${SwedishUnableToFind} convert.exe"
LangString ImageMagickDownloadLabel "${LYX_LANG}" "${SwedishDownload} ImageMagick"
LangString ImageMagickFolderLabel "${LYX_LANG}" "${SwedishFolderContaining} convert.exe"

LangString GhostscriptHeader "${LYX_LANG}" "Ghostscript"
LangString GhostscriptDescription "${LYX_LANG}" "Ghostscript (www.cs.wisc.edu/~ghost/) anv�nds f�r att konvertera bilder till/fr�n PostScript."
LangString EnterGhostscriptFolder "${LYX_LANG}" "${SwedishInputFolderPath} gswin32c.exe"
LangString InvalidGhostscriptFolder "${LYX_LANG}" "${SwedishUnableToFind} gswin32c.exe"
LangString GhostscriptDownloadLabel "${LYX_LANG}" "${SwedishDownload} Ghostscript"
LangString GhostscriptFolderLabel "${LYX_LANG}" "${SwedishFolderContaining} gswin32c.exe"

LangString SummaryTitle "${LYX_LANG}" "Sammanfattning av programvara"
LangString SummaryPleaseInstall "${LYX_LANG}" "Var god installera f�rst den kompletterande programvara du laddat ner. K�r sedan LyX installationsprogram igen."
LangString SummaryPathPrefix "${LYX_LANG}" "Ett s�kv�gsprefix kommer att l�ggas till i filen 'lyxrc.defaults'. Denna inneh�ller f�ljande:"

LangString UILangageTitle "${LYX_LANG}" "Spr�k i LyX anv�ndargr�nsnitt"
LangString UILangageDescription "${LYX_LANG}" "Detta spr�k anv�nds i menyer, meddelanden, m.m."
LangString UILanguageAvailableLanguages "${LYX_LANG}" " Tillg�ngliga spr�k "

LangString UnNotInRegistryLabel "${LYX_LANG}" "${SwedishUnableToFind} $(^Name) i registret\r$\nGenv�gar p� skrivbordet och i startmenyn kommer inte att tas bort."
LangString UnNotAdminLabel "${LYX_LANG}" "Tyv�rr! Du m�ste ha administrat�rsr�ttigheter f�r\r$\natt avinstallera $(^Name)."
LangString UnReallyRemoveLabel "${LYX_LANG}" "�r du s�ker p� att du verkligen vill avinstallera $(^Name) och alla dess komponenter fullst�ndigt?"
LangString UnRemoveSuccessLabel "${LYX_LANG}" "Avinstallationen av $(^Name) fr�n din dator var framg�ngsrik."

!undef SwedishInputFolderPath
!undef SwedishUnableToFind
!undef SwedishDownload
!undef SwedishFolderContaining

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_SWEDISH_NSH_
