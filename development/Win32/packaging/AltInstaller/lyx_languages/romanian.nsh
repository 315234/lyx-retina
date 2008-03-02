!ifndef _LYX_LANGUAGES_ROMANIAN_NSH_
!define _LYX_LANGUAGES_ROMANIAN_NSH_

!ifdef LYX_LANG
 !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_ROMANIAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Acest asistent v� va ghida �n procesul de instalare a programului LyX. \r\n\
                                         \r\n\
                                         Trebuie s� ave�i drepturi de administrator ca s� pute�i instala LyX. \r\n\
                                         \r\n\
                                         Este recomandat ca s� �nchide�i celelalte aplica�ii �nainte de �nceperea instal�rii. Acest lucru va permite upgradarea fi�ierelor de sistem relevante f�r� s� fi�i nevoi�i s� reporni�i calculatorul.  \r\n\
                                         \r\n\
                                         $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Document LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Dori�i s� instala�i pentru to�i utilizatorii?"
LangString SecFileAssocTitle "${LYX_LANG}" "Asocierea fi�ierelor"
LangString SecDesktopTitle "${LYX_LANG}" "Iconi�� pe desktop"

LangString SecCoreDescription "${LYX_LANG}" "Fi�ierele LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Program  de vizualizat documente tip Postscript �i PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Gestionarul de Bibliografie �i editorul de fi�iere tip BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "Instaleaz� LyX pentru to�i utilizatorii sau doar pentru utilizatorul curent."
LangString SecFileAssocDescription "${LYX_LANG}" "Fi�ierele cu extensia .lyx vor fi deschise automat cu LyX."
LangString SecDesktopDescription "${LYX_LANG}" "A iconi�� LyX pe desktop."

LangString LangSelectHeader "${LYX_LANG}" "Selec�ia de limbaj pentru meniurile LyX."
LangString AvailableLang "${LYX_LANG}" "Limbajele disponibile."

LangString MissProgHeader "${LYX_LANG}" "Verific� pentru programele necesare."
LangString MissProgCap "${LYX_LANG}" "Urm�toarele programe necesare vor fi instalate adi�ional."
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, o distribu�ie LaTeX"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, un vizualizator de documente tip PostScript �i PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, un convertor de imagini"
LangString MissProgAspell "${LYX_LANG}" "Aspell, un corector ortografic"
LangString MissProgMessage "${LYX_LANG}" 'Toate programele necesare sunt deja instalate.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribu�ia LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Seteaz� distribu�ia LaTeX pe care s� o foloseasc� LyX.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Op�ional pute�i seta calea c�tre fi�ierul "latex.exe" �i, implicit, seta distribu�ia \ LaTeX pe care s� o foloseasc� LyX. \r\n\
                                          Dac� nu folosi�i LaTeX, LyX nu va putea s� produc� fi�iere!\r\n\
                                          \r\n\
                                          Programul de instalare a detectat distribu�ia LaTeX \
                                          "$LaTeXName" instalat� pe calculatorul dumneavoastr�. Dedesupt este afi�at� calea acestei distribu�ii.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Seta�i calea c�tre fi�ierul "latex.exe" �i, implicit, seta distribu�ia \ LaTeX pe care s� o foloseasc� LyX. \r\n\
                                          Dac� nu folosi�i LaTeX, LyX nu va putea s� produc� fi�iere!\r\n\
                                          \r\n\
                                          Programul de instalare nu a detectat nici o distribu�ia LaTeX instalat� pe calculatorul dumneavoastr�.'

LangString PathName "${LYX_LANG}" 'Calea c�tre fi�ierul "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Nu folosi LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Fi�ierul "latex.exe" nu se afl� �n calea specificat�.'

LangString LatexInfo "${LYX_LANG}" 'Programul de instalare al distribu�iei LaTeX  "MiKTeX" va fi lansat. $\r$\n\
                                   Pentru instalare ap�sa�i butonul "Next" �n fereastra prgramului de instalare p�n� c�nd instalarea �nncepe. $\r$\n\
                                   $\r$\n\
                                   !!! V� rug�m s� folosi�i set�rile implicite ale programului de instalat MiKTex !!!'
LangString LatexError1 "${LYX_LANG}" 'Nu am detectat nici o distribu�ia LaTeX instalat� pe calculatorul dumneavoastr�!$\r$\n\
                                     LyX nu poate fi folosit f�r� o distribu�ia LaTeX ca  "MiKTeX"!$\r$\n\
                                     Instalarea va fi abandonat�.'

LangString GSviewInfo "${LYX_LANG}" 'Acum va fi lansat programul de instalare al aplica�iei "GSview".$\r$\n\
                                    Pentru instalare ap�sa�i butonul "Setup" �n fereastra prgramului de instalare,$\r$\n\
                                    alege�i un limbaj �i ap�sa�i butonul "Next" �n urmatoarea fereastr�$\r$\n\
                                    Este indicat s� folosi�i set�rile implicite ale programului de instalat GSview.'
LangString GSviewError "${LYX_LANG}" 'Programul "GSview" nu a putut fi instalat!$\r$\n\
                                     Programul de instalare LyX va continua totu�i.$\r$\n\
                                     �ncerca�i s� instala�i GSview mai t�rziu.'

LangString JabRefInfo "${LYX_LANG}" 'Acum va fi lansat programul de instalare al aplica�iei "JabRef".$\r$\n\
                                     Este indicat s� folosi�i set�rile implicite ale programului de instalat  JabRef.'
LangString JabRefError "${LYX_LANG}" 'Programul "JabRef" nu a putut fi instalat!$\r$\n\
                                     Programul de instalare LyX va continua totu�i.$\r$\n\
                                     �ncerca�i s� instala�i JabRef mai t�rziu.'

LangString LatexConfigInfo "${LYX_LANG}" "Configurarea programului LyX va dura o perioad� de timp."

LangString AspellInfo "${LYX_LANG}" 'Acum vor fi desc�rcate �i instalate dic�ionarele corectorului ortografic  "Aspell".$\r$\n\
                                    Fiecare dic�ionar are o licen�� diferit� ce va fi afi�at� �naintea instal�rii.'
LangString AspellDownloadFailed "${LYX_LANG}" 'Nu au putut fi desc�rcate dic�ionarele corectorului ortografic   "Aspell"!'
LangString AspellInstallFailed "${LYX_LANG}" 'Nu au putut fi instalate dic�ionarele corectorului ortografic  "Aspell"!'
LangString AspellPartAnd "${LYX_LANG}" " �i "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Exist� deja un dic�ionar instalat.'
LangString AspellNone "${LYX_LANG}" 'Nu au putut fi instalate dic�ionarele corectorului ortografic  "Aspell".$\r$\n\
                                    Dic�ionarele pot fi desc�rcate de la$\r$\n\
                                    ${AspellLocation}$\r$\n\
                                    Dori�i s� desc�rca�i dic�ionarele acum?'
LangString AspellPartStart "${LYX_LANG}" "A fost instalat cu succes "
LangString AspellPart1 "${LYX_LANG}" "Un dic�ionar englezesc"
LangString AspellPart2 "${LYX_LANG}" "Un dic�ionar pentru limba $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
                                       dic�ionarul pentru limba $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" 'pentru corectorul ortografic "Aspell".$\r$\n\
                                     Mai multe dic�ionare pot fi desc�rcate de la$\r$\n\
                                     ${AspellLocation}$\r$\n\
                                     Dori�i s� desc�rca�i dic�ionarele acum?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Pentru ca utilizatorii s� poat� modifica set�rile MiKTeX$\r$\n\
                                        este necesar s� seta�i dreptul de scriere la directorul$\r$\n\
                                        $MiKTeXPath $\r$\n\
                                         unde a fost instalat MiKTex �i toate subdirectoarele pentru to�i utilizatorii."
LangString MiKTeXInfo "${LYX_LANG}" 'Distribu�ia LaTeX "MiKTeX" va fi folosit� cu LyX.$\r$\n\
                                    Este recomandat s� instala�i actualiz�ri ale programului MiKTex folosind programul "MiKTeX Update Wizard"$\r$\n\
                                    �nainte de a folosi LyX pentru prima oar�.$\r$\n\
                                    Dori�i s� verifica�i dac� sunt disponibile actualiz�ri pentru MiKTeX?'

LangString UpdateNotAllowed "${LYX_LANG}" "Acest program de actualizare poate actualiza numai ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Nu am putut seta 'path_prefix' �n procesul de configurare"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Nu am putut crea lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Nu am putut executa scriptul de configurare"
LangString NotAdmin "${LYX_LANG}" "Trebuie s� ave�i drepturi de administrator pentru instalarea programului LyX!"
LangString InstallRunning "${LYX_LANG}" "Programul de instalare este deja pornit!"
LangString StillInstalled "${LYX_LANG}" "LyX este deja instalat! Dezinstala�i LyX prima oar�."

LangString FinishPageMessage "${LYX_LANG}" "Felicit�ri! LyX a fost instalat cu succes.\r\n\
                                           \r\n\
                                           (Prima oar� c�nd porni�i LyX s-ar putea s� dureze c�teva secunde.)"
LangString FinishPageRun "${LYX_LANG}" "Lanseaz� LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nu am g�sit LyX �n registri.$\r$\n\
                                              Scurt�turile de pe desktop �i Start Menu nu vor fi �terse."
LangString UnInstallRunning "${LYX_LANG}" "Trebuie s� inchide�i LyX prima oar�!"
LangString UnNotAdminLabel "${LYX_LANG}" "Trebuie s� ave�i drepturi de administrator pentru dezinstalarea programului LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Sunte�i sigur c� dori�i s� dezinstala�i programul LyX �i toate componentele lui?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Preferin�ele utilizatorului pentru LyX$'
LangString UnGSview "${LYX_LANG}" 'Ap�sa�i butonul "Uninstall" �n urmatoarea fereastr� pentru a dezinstala  $\r$\n\
                                  vizualizatorul de documente tip Postscript �i PDF  "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX a fost dezinstalat cu succes de pe calculatorul dumneavoastr�."

LangString SecUnAspellDescription "${LYX_LANG}" 'Dezinstaleaz�  corectorul ortografic "Aspell" �i toate componentele lui.'
LangString SecUnMiKTeXDescription "${LYX_LANG}" 'Dezinstaleaz� Distribu�ia LaTeX "MiKTeX".'
LangString SecUnJabRefDescription "${LYX_LANG}" "Dezinstaleaz� gestionarul de bibliografie JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" '�terge directorul cu set�rile LyX$\r$\n\
					  "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
                                          pentru to�i utilizatorii.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" " Dezinstala�i programul LyX �i toate componentele lui."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_ROMANIAN_NSH_

