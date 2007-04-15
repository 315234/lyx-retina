!ifndef _LYX_LANGUAGES_POLISH_NSH_
!define _LYX_LANGUAGES_POLISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_POLISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Kreator przeprowadzi Ciebie przez proces instalacji LyX-a.\r\n\
					  \r\n\
					  Wymagane s� prawa administratora aby zainstalowa� LyX-a.\r\n\
					  \r\n\
					  Zalecane jest pozamykanie wszystkich innych aplikacji przed uruchomieniem programu instalacyjnego. Pozwoli to zaktualizowa� pliki systemowe bez wymagania restartu komputera.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-Document"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Instalacja dla wszystkich u�ytkownik�w?"
LangString SecFileAssocTitle "${LYX_LANG}" "Skojarzenie plik�w .lyx"
LangString SecDesktopTitle "${LYX_LANG}" "Ikona na pulpicie"

LangString SecCoreDescription "${LYX_LANG}" "Pliki LyX-a."
LangString SecInstGSviewDescription "${LYX_LANG}" "Program do podgl�du dokument�w Postscript i PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Bibliography reference manager and editor for BibTeX files."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalacja dla wszystkich u�ytkownik�w lub tylko dla bie��cego u�ytkownika."
LangString SecFileAssocDescription "${LYX_LANG}" "Skojarzenie LyX-a z plikami o rozszerzeniu .lyx."
LangString SecDesktopDescription "${LYX_LANG}" "Ikona LyX-a na pulpicie."

LangString LangSelectHeader "${LYX_LANG}" "Wyb�r j�zyka menu LyX-a"
LangString AvailableLang "${LYX_LANG}" " Dost�pne j�zyki "

LangString MissProgHeader "${LYX_LANG}" "Sprawdzenie zainstalowania wymaganych program�w"
LangString MissProgCap "${LYX_LANG}" "Zostan� dodatkowo zainstalowane nast�puj�ce wymagane programy"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, dystrybucja LaTeX-a"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, interpreter PostScript i PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, konwerter rysunk�w"
LangString MissProgAspell "${LYX_LANG}" "Aspell, kontrola pisowni"
LangString MissProgMessage "${LYX_LANG}" 'Brak dodatkowych program�w do instalacji.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Dystrybucja LaTeX-a'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Wybierz dystrybucj� LaTeX-a, kt�r� chcesz u�ywa� z LyX-em.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'You can optionally set here the path to the file "latex.exe" and therewith set the \
					   LaTeX-distribution that should be used by LyX.\r\n\
					   If you don$\'t use LaTeX, LyX cannot output documents!\r\n\
					   \r\n\
					   The installer has detected the LaTeX-distribution \
					   "$LaTeXName" on your system. Displayed below is its path.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Set below the path to the file "latex.exe". Therewith you set which \
					       LaTeX-distribution should be used by LyX.\r\n\
					       If you don$\'t use LaTeX, LyX cannot output documents!\r\n\
					       \r\n\
					       The installer couldn$\'t find a LaTeX-distribution on your system.'
LangString PathName "${LYX_LANG}" 'Path to the file "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Nie u�ywaj LaTeX-a"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Plik "latex.exe" nie znajduje si� w podanej �cie�ce.'

LangString LatexInfo "${LYX_LANG}" 'Teraz zostanie uruchomiony instalator dystrybucji LaTeX-a "MiKTeX".$\r$\n\
			            Aby zainstalowa� ten program naci�nij przycisk "Dalej" w oknie instalatora.$\r$\n\
				    $\r$\n\
				    !!! Prosz� u�y� wszystkich domy�lnych opcji instalatora MiKTeX-a !!!'
LangString LatexError1 "${LYX_LANG}" 'Nie znaleziono dystrybucji LaTeX-a!$\r$\n\
                      		      LyX nie mo�e by� u�yty bez dystrybucji LaTeX-a takiej jak "MiKTeX"!$\r$\n\
				      Z tego powodu instalacja zostanie przerwana.'
		
LangString GSviewInfo "${LYX_LANG}" 'Teraz zostanie uruchomiony program instalacyjny "GSview".$\r$\n\
			             Aby go zainstalowa� naci�nij przycisk "Setup" w pierwszym oknie instalatora,$\r$\n\
				     w kolejnym oknie wybierz j�zyk i przyci�nij przycisk "Next".$\r$\n\
				     Mo�esz u�y� wszystkich domy�lnych opcji instalacji.'
LangString GSviewError "${LYX_LANG}" 'Program "GSview" nie zosta� pomy�lnie zainstalowany!$\r$\n\
		                      Mimo to instalacja b�dzie kontynuowana.$\r$\n\
				      Spr�buj p�niej zainstalowa� program GSview.'

LangString JabRefInfo "${LYX_LANG}" 'Now the installer of the program "JabRef" will be launched.$\r$\n\
				     You can use all default options of the JabRef-installer.'
LangString JabRefError "${LYX_LANG}" 'The program "JabRef" could not successfully be installed!$\r$\n\
		                      The installer will continue anyway.$\r$\n\
				      Try to install JabRef again later.'

LangString LatexConfigInfo "${LYX_LANG}" "Dalsza konfiguracja LyX-a chwil� potrwa."

LangString AspellInfo "${LYX_LANG}" 'Now dictionaries for the spellchecker "Aspell" will be downloaded and installed.$\r$\n\
				     Every dictionary has a different license that will be displayed before the installation.'
LangString AspellDownloadFailed "${LYX_LANG}" "No Aspell spellchecker dictionary could be downloaded!"
LangString AspellInstallFailed "${LYX_LANG}" "No Aspell spellchecker dictionary could be installed!"
LangString AspellPartAnd "${LYX_LANG}" " and "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'There is already installed a dictionary of the language'
LangString AspellNone "${LYX_LANG}" 'No dictionary for the spellchecker "Aspell" has been installed.$\r$\n\
				     Dictionaries can be downloaded from$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Do you want to download dictionaries now?'
LangString AspellPartStart "${LYX_LANG}" "There was successfully installed "
LangString AspellPart1 "${LYX_LANG}" "an english dictionary"
LangString AspellPart2 "${LYX_LANG}" "a dictionary of the language $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
				      dictionary of the language $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' for the spellchecker "Aspell".$\r$\n\
				      More dictionaries can be downloaded from$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Do you want to download other dictionaries now?'

LangString MiKTeXPathInfo "${LYX_LANG}" "In order that every user is later able to customize MiKTeX for his needs$\r$\n\
					 it is necessary to set write permissions for all users to MiKTeX's installation folder$\r$\n\
					 $MiKTeXPath $\r$\n\
					 and its subfolders."
LangString MiKTeXInfo "${LYX_LANG}" 'The LaTeX-distribution "MiKTeX" will be used together with LyX.$\r$\n\
				     It is recommended to install available MiKTeX-updates using the program "MiKTeX Update Wizard"$\r$\n\
				     before you use LyX for the first time.$\r$\n\
				     Would you now check for MiKTeX updates?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Nieudana pr�ba ustawienia zmiennej 'path_prefix' w pliku skryptu konfigracyjnego"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Niedana pr�ba utworzenia plik�w lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Niedana pr�ba wykonania skryptu konfiguracyjnego"
LangString NotAdmin "${LYX_LANG}" "Musisz mie� prawa administratora aby zainstalowa� LyX-a!"
LangString InstallRunning "${LYX_LANG}" "Instalator jest ju� uruchomiony!"
LangString StillInstalled "${LYX_LANG}" "LyX jest ju� zainstalowany! Aby kontynowa� musisz go najpierw usun��."

LangString FinishPageMessage "${LYX_LANG}" "Gratulacje! LyX zosta� pomy�lnie zainstalowany.\r\n\
					    \r\n\
					    (Pierwsze uruchomienie mo�e potrwa� kilka sekund.)"
LangString FinishPageRun "${LYX_LANG}" "Uruchom LyX-a"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nie mo�na znale�� LyX-a w rejestrze.$\r$\n\
	                                       Skr�ty na pulpicie i w menu Start nie zostan� usuni�te."
LangString UnInstallRunning "${LYX_LANG}" "Musisz najpierw zamkn�� LyX-a!"
LangString UnNotAdminLabel "${LYX_LANG}" "Musisz posiada� prawa administratora do deinstalacji programu LyX."
LangString UnReallyRemoveLabel "${LYX_LANG}" "Czy na pewno chcesz usun�� LyX-a i wszystkie jego komponenty?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX$\'s user preferences'
LangString UnGSview "${LYX_LANG}" 'Naci�nij przycisk "Odinstaluj" w nast�pnym oknie aby odinstalowa�$\r$\n\
				   przegl�dark� Postscript i PDF "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX zosta� pomy�lnie usuni�ty z Twojego komputera."

LangString SecUnAspellDescription "${LYX_LANG}" "Uninstalls the spellchecker Aspell and all of its dictionaries."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Uninstalls the LaTeX-distribution MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Uninstalls the bibliography manager JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Deletes LyX$\'s configuration folder$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   for all users.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Uninstall LyX and all of its components."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_POLISH_NSH_















