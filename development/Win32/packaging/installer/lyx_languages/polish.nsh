!ifndef _LYX_LANGUAGES_POLISH_NSH_
!define _LYX_LANGUAGES_POLISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_POLISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString SecAllUsersTitle "${LYX_LANG}" "Instalacja dla wszystkich u�ytkownik�w?"
LangString SecFileAssocTitle "${LYX_LANG}" "Skojarzenie plik�w .lyx"
LangString SecDesktopTitle "${LYX_LANG}" "Ikona na pulpicie"

LangString SecCoreDescription "${LYX_LANG}" "Pliki ${PRODUCT_NAME}-a."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalacja dla wszystkich u�ytkownik�w lub tylko dla bie��cego u�ytkownika. Instalacja dla wszystkich wymaga praw administratora."
LangString SecFileAssocDescription "${LYX_LANG}" "Skojarzenie LyX-a z plikami o rozszerzeniu .lyx."
LangString SecDesktopDescription "${LYX_LANG}" "Ikona ${PRODUCT_NAME}-a na pulpicie."

LangString ModifyingConfigureFailed "${LYX_LANG}" "Nieudana pr�ba ustawienia zmiennej 'path_prefix' w pliku skryptu konfigracyjnego"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Niedana pr�ba utworzenia plik�w lyx.cmd i reLyX.cmd"
LangString RunConfigureFailed "${LYX_LANG}" "Niedana pr�ba wykonania skryptu konfiguracyjnego"

LangString FinishPageMessage "${LYX_LANG}" "LyX powinien by� ju� teraz zainstalowany. Jednak niekt�rzy u�ytkownicy raportuj�, �e skrypt konfiguracyjny nie wykonuje si�. W efekcie LyX nie uruchamia si� zg�aszaj�c brak pliku textclass.lst. Dla naprawy instalacji LyX-a uruchom polecenie 'sh configure' w katalogu $\'<LyX>/Resources/lyx$\'."
LangString FinishPageRun "${LYX_LANG}" "Uruchom LyX"

LangString DownloadPageField2 "${LYX_LANG}" "&Nie instaluj"

LangString MinSYSHeader "${LYX_LANG}" "MinSYS"
LangString MinSYSDescription "${LYX_LANG}" "MinSYS jest minimalnym �rodowiskiem uniksowym (www.mingw.org/msys.shtml), kt�rego wymaga ${PRODUCT_NAME} do uruchomienia niekt�rych skrypt�w."
LangString EnterMinSYSFolder "${LYX_LANG}" "Prosz� poda� �cie�k� do folderu zawieraj�cego sh.exe"
LangString InvalidMinSYSFolder "${LYX_LANG}" "Nie mo�na znale�� sh.exe"
LangString MinSYSDownloadLabel "${LYX_LANG}" "&Pobierz MinSYS"
LangString MinSYSFolderLabel "${LYX_LANG}" "&Folder zawieraj�cy sh.exe"

LangString PythonHeader "${LYX_LANG}" "Python"
LangString PythonDescription "${LYX_LANG}" "J�zyk skryptowy Python (www.python.org) musi by� zainstalowany, inaczej ${PRODUCT_NAME} nie b�dzie m�g� uruchomi� niekt�rych skrypt�w."
LangString EnterPythonFolder "${LYX_LANG}" "Prosz� poda� �cie�k� do folderu zawieraj�cego Python.exe"
LangString InvalidPythonFolder "${LYX_LANG}" "Nie mo�na znale�� Python.exe"
LangString PythonDownloadLabel "${LYX_LANG}" "&Pobierz Python"
LangString PythonFolderLabel "${LYX_LANG}" "&Folder zawieraj�cy Python.exe"

LangString MiKTeXHeader "${LYX_LANG}" "MiKTeX"
LangString MiKTeXDescription "${LYX_LANG}" "MiKTeX (www.miktex.org) jest implementacj� TeX-a dla Windows."
LangString EnterMiKTeXFolder "${LYX_LANG}" "Prosz� poda� �cie�k� do folderu zawieraj�cego latex.exe"
LangString InvalidMiKTeXFolder "${LYX_LANG}" "Nie mo�na znale�� latex.exe"
LangString MiKTeXDownloadLabel "${LYX_LANG}" "&Pobierz MiKTeX"
LangString MiKTeXFolderLabel "${LYX_LANG}" "&Folder zawieraj�cy latex.exe"


LangString ImageMagickHeader "${LYX_LANG}" "ImageMagick"
LangString ImageMagickDescription "${LYX_LANG}" "Narz�dzia pakietu ImageMagick (www.imagemagick.org/script/index.php) s� u�ywane do konwersji plik�w graficznych do wymaganych format�w."
LangString EnterImageMagickFolder "${LYX_LANG}" "Prosz� poda� �cie�k� do folderu zawieraj�cego convert.exe"
LangString InvalidImageMagickFolder "${LYX_LANG}" "Nie mo�na znale�� convert.exe"
LangString ImageMagickDownloadLabel "${LYX_LANG}" "&Pobierz ImageMagick"
LangString ImageMagickFolderLabel "${LYX_LANG}" "&Folder zawieraj�cy convert.exe"

LangString GhostscriptHeader "${LYX_LANG}" "Ghostscript"
LangString GhostscriptDescription "${LYX_LANG}" "Ghostscript (www.cs.wisc.edu/~ghost/) jest u�ywany do konwersji z i do formatu PostScript."
LangString EnterGhostscriptFolder "${LYX_LANG}" "Prosz� poda� �cie�k� do folderu zawieraj�cego gswin32c.exe"
LangString InvalidGhostscriptFolder "${LYX_LANG}" "Nie mo�na znale�� gswin32c.exe"
LangString GhostscriptDownloadLabel "${LYX_LANG}" "&Pobierz Ghostscript"
LangString GhostscriptFolderLabel "${LYX_LANG}" "&Folder zawieraj�cy gswin32c.exe"

LangString SummaryTitle "${LYX_LANG}" "Podsumowanie oprogramowania"
LangString SummaryPleaseInstall "${LYX_LANG}" "Prosz� zainstalowa� pobrane pliki i uruchomi� instalator LyX-a ponownie."
LangString SummaryPathPrefix "${LYX_LANG}" "Do zmiennej 'path_prefix' w pliku 'lyxrc.defaults' zostan� dodane nast�puj�ce �cie�ki:"

LangString UILangageTitle "${LYX_LANG}" "J�zyk interfejsu LyX-a"
LangString UILangageDescription "${LYX_LANG}" "Kt�ry b�dzie u�yty w menu, komunikatach itp."
LangString UILanguageAvailableLanguages "${LYX_LANG}" " Dost�pne j�zyki "

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nie mo�na znale�� $(^Name) w rejestrze$\r$\nSkr�ty na pulpicie i w menu Start nie zostan� usuni�te."
LangString UnNotAdminLabel "${LYX_LANG}" "Musisz posiada� prawa administratora$\r$\ndo deinstalacji programu $(^Name)."
LangString UnReallyRemoveLabel "${LYX_LANG}" "Czy na pewno chcesz usun�� $(^Name) i wszystkie jego komponenty?"
LangString UnRemoveSuccessLabel "${LYX_LANG}" "$(^Name) zosta� pomy�lnie usuni�ty z Twojego komputera."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_POLISH_NSH_

