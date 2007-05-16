!ifndef _LYX_LANGUAGES_SLOVAK_NSH_
!define _LYX_LANGUAGES_SLOVAK_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_SLOVAK}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Tento sprievodca V�m pom�ha in�talova� LyX.\r\n\
					  \r\n\
					  Pre in�tal�ciu LyX potrebujete administr�torsk� pr�va.\r\n\
					  \r\n\
					  Odpor��uje sa zavrie� v�etk� ostatn� aplik�cie pred �tartom Setup. T�m sa umo��uje nahradi� relevantn� d�ta s�stavy bez nov�ho �tartu po�ita�e.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX dokument"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "In�talova� pre v�etk�ch u��vate�ov?"
LangString SecFileAssocTitle "${LYX_LANG}" "Usporiadanie data"
LangString SecDesktopTitle "${LYX_LANG}" "Symbol desktopu"

LangString SecCoreDescription "${LYX_LANG}" "Program LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Program na pozeranie dokumentov Postscript a PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Manager pre bibliografick� odkaz a editor pre BibTeX files."
LangString SecAllUsersDescription "${LYX_LANG}" "In�talova� LyX pre v�etk�ch u��vatelov alebo len pre teraj�� u��vate�."
LangString SecFileAssocDescription "${LYX_LANG}" "Data s lyx roz��ren�m sa automaticky otv�raj� v LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Symbol LyX na desktopu."

LangString LangSelectHeader "${LYX_LANG}" "Vyber jazyka pre LyX"
LangString AvailableLang "${LYX_LANG}" " Pohotov� jazyky "

LangString MissProgHeader "${LYX_LANG}" "Hladajte vy�adovan� programy"
LangString MissProgCap "${LYX_LANG}" "Nasleduj�ce vy�adovan� programy sa in�taluj� dodatocne"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, je LaTeX distrib�cia"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, je interpret�tor pre PostScript a PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, konvertuje obrazy"
LangString MissProgAspell "${LYX_LANG}" "Aspell, je overovanie pravopisu"
LangString MissProgMessage "${LYX_LANG}" 'Netreba in�talova� �al�ie programy.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'LaTeX distrib�cia'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Urcujte LaTeX distrib�ciu, ktor� m� u��va� LyX.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Tu m��ete pou��va� chod k "latex.exe" a t�m pou��va� chod \
					   LaTeX-distrib�ciu, ktor� by mohla by� pou��van� cestou LyXu.\r\n\
					   Ak nepou��vate LaTeX, LyX nem��e produkova� dokumenty!\r\n\
					   \r\n\
					   In�talat�r detektoval LaTeX-distrib�ciu \
					   "$LaTeXName" na va�ej s�stave. Dolu vid�te jeho chod.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Pou��vajte dolu chod k "latex.exe". T�m urcujete, ktor� \
					       LaTeX-distrib�cia m� by� pou��van� LyXom.\r\n\
					       Ak nepou��vate LaTeX, LyX nem��e produkova� dokumenty!\r\n\
					       \r\n\
					       In�talat�r nemohol n�jst LaTeX-distrib�ciu na va�ej s�stave.'
LangString PathName "${LYX_LANG}" 'Chod k "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Neu��vajte LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Mapa "latex.exe" nie je na �pecifikovanej ceste.'

LangString LatexInfo "${LYX_LANG}" 'Teraz m��ete �tartova� LaTeX distrib�ciu "MiKTeX".$\r$\n\
			            Na in�talovanie programu tla�te gomb�k "Next" v in�talovac�ch okn�ch a� sa za��na in�tal�cia.$\r$\n\
				    $\r$\n\
				    !!! Pros�m u��vajte v�etk� obsiahnut� nastavenia in�talat�ra MiKTeX !!!'

LangString LatexError1 "${LYX_LANG}" 'LaTeX distrib�cia sa nemohla n�jst!$\r$\n\
                      		      LyX sa nem��e u��va� bez LaTeX distrib�cie ako "MiKTeX"!$\r$\n\
				      Preto sa in�tal�cia zastavila.'
			
LangString GSviewInfo "${LYX_LANG}" 'Teraz �tartuje in�talat�r programu "GSview" .$\r$\n\
			             Na in�tal�ciu programu tla�te gomb�k "Setup" v prvom in�tala�nom okne$\r$\n\
				     Vyberajte jazyk a tla�te gomb�k "Next" v nasleduj�cich in�tala�n�ch okn�ch.$\r$\n\
				     M��ete u��vat v�etk� obsiahnut� nastavenia in�talat�ra GSview.'
LangString GSviewError "${LYX_LANG}" 'Program "GSview" sa nemohol in�talova� �spe�ne!$\r$\n\
		                      Predsa in�tal�cia pokra�uje.$\r$\n\
				      Sk�ste in�talova� GSview e�te raz nesk�r.'

LangString JabRefInfo "${LYX_LANG}" 'Teraz sa �tartuje in�talat�r programu "JabRef".$\r$\n\
				     M�zete v�etk� obsiahnut� nastavenia in�talat�ra JabRef.'
LangString JabRefError "${LYX_LANG}" 'Program "JabRef" sa nemohol in�talova� �spe�ne!$\r$\n\
		                      Predsa in�tal�cia pokra�uje.$\r$\n\
				      Sk�ste in�talova� JabRef e�te raz nesk�r.'
				
LangString LatexConfigInfo "${LYX_LANG}" "Na nasleduj�cu konfigur�ciu LyXu treba po�ka� chv�u."

LangString AspellInfo "${LYX_LANG}" 'Teraz sa in�taluj� slovn�ky pre sk��obn� program "Aspell".$\r$\n\
				     Ka�d� slovn�k m� in� licensiu, ktor� sa predstavuje pred downloadom a in�tal�ciou.'
LangString AspellDownloadFailed "${LYX_LANG}" "Aspell slovn�k sa nemohol downloadova�!"
LangString AspellInstallFailed "${LYX_LANG}" "Aspell slovn�k sa nemohol in�talova�!"
LangString AspellPartAnd "${LYX_LANG}" " a "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'U� je in�talovan� slovn�k jazyku'
LangString AspellNone "${LYX_LANG}" 'Slovn�k pre "Aspell" nebol in�talovan�.$\r$\n\
				     Slovn�ky je mo�n� downloadova� od$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Chcete teraz downloadova� slovn�ky?'
LangString AspellPartStart "${LYX_LANG}" "Teraz je �spe�ne in�talovan� "
LangString AspellPart1 "${LYX_LANG}" "anglick� slovn�k"
LangString AspellPart2 "${LYX_LANG}" "slovn�k jazyku $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
				      slovn�k jazyku $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' pre "Aspell".$\r$\n\
				      Viac slovn�kov je mo�no downloadova� od$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Chcete teraz downloadova� in� slovn�ky?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Aby ka�d� pou��vatel bol nesk�r schopn� pou��vat MiKTeX pre svoje potreby$\r$\n\
					 je treba nap�sat povolenia pre v�etk�ch pou��vatelov do in�talacn�ho papiera MiKTeX $\r$\n\
					 $MiKTeXPath $\r$\n\
					 aj do podriaden�ch papierov."
LangString MiKTeXInfo "${LYX_LANG}" 'LaTeX distrib�cia "MiKTeX" sa pou��va spolu s LyXom.$\r$\n\
				     Doporu�uje sa in�talovat platn� MiKTeX aktualiz�cie pomocou programu "MiKTeX Update Wizard"$\r$\n\
				     pred prv�m pou��van�m LyXu.$\r$\n\
				     Chcete teraz testova� MiKTeX aktualiz�cie?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Nemohol uvies� v platnost 'path_prefix' (zoznam s cestami programu) v konfigura�nom skripte"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Nemohol pr�pravi� lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Nemohol vykona� konfigura�n� skript"
LangString NotAdmin "${LYX_LANG}" "Pre in�tal�ciu LyX potrebujete administr�torsk� pr�va!"
LangString InstallRunning "${LYX_LANG}" "In�talat�r u� beh�!"
LangString StillInstalled "${LYX_LANG}" "LyX je u� in�talovan�! Najprv treba odin�talova� LyX."

LangString FinishPageMessage "${LYX_LANG}" "Gratul�cia! LyX bol �spe�ne in�talovan�.\r\n\
					    \r\n\
					    (Prv� LyX �tart m��e trva� niekolko sekund.)"
LangString FinishPageRun "${LYX_LANG}" "�tartova� LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nem��e n�js� LyX v registre.$\r$\n\
					       Symboly na desktope a v �tartovacom Menu sa nedaj� odstrani�."
LangString UnInstallRunning "${LYX_LANG}" "Najprv zavr� LyX!"
LangString UnNotAdminLabel "${LYX_LANG}" "Pre odin�tal�ciu LyX potrebujete administr�torsk� pr�va!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Ste si ist�, �e chcete kompletne odin�talova� LyX a v�etk� jeho su�iastky?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX prednosti u��va�e�a'
LangString UnGSview "${LYX_LANG}" 'Pros�m tla�te gomb�k "Uninstall" v nasleduj�com okne aby Ste in�talovali $\r$\n\
				   Postscript a PDF-viewer "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX je �spe�ne odstr�nen� od V�ho po�ita�a."

LangString SecUnAspellDescription "${LYX_LANG}" "Odin�taluje Aspell a v�etk� jeho slovn�ky."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Odin�taluje LaTeX distrib�ciu MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Odin�taluje bibliografick� manager JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Odstr�ni konfiguracn� papiere LyXu $\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   pre v�etk�ch u�ivate�ov.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Odin�taluj LyX a v�etk� jeho su�iastky."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_SLOVAK_NSH_





