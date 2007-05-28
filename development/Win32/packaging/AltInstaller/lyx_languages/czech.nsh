!ifndef _LYX_LANGUAGES_CZECH_NSH_
!define _LYX_LANGUAGES_CZECH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_CZECH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Tento pomocn�k v�s provede instalac� LyXu.\r\n\
					  \r\n\
					  K instalaci LyXu budete pot�ebovat administr�torsk� pr�va.\r\n\
					  \r\n\
					  Je doporu�eno zav��t v�echny ostatn� aplikace p�ed spu�t�n�m instalace. Umo�n�te t�m aktualizovat pat�i�n� syst�mov� soubory bez nutnosti restartovat po��ta�.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-dokument�"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Instalovat pro v�echny u�ivatele?"
LangString SecFileAssocTitle "${LYX_LANG}" "Asociovat soubory"
LangString SecDesktopTitle "${LYX_LANG}" "Ikonu na plochu"

LangString SecCoreDescription "${LYX_LANG}" "Soubory LyXu."
LangString SecInstGSviewDescription "${LYX_LANG}" "Prohl�e� Postscriptu and PDF-dokument�."
LangString SecInstJabRefDescription "${LYX_LANG}" "Spr�vce pro bibliografii a editor soubor� BibTeXu."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalovat LyX pro v�echny u�ivatele nebo pouze pro sou�asn�ho u�ivatele."
LangString SecFileAssocDescription "${LYX_LANG}" "Soubory s p��ponou .lyx se automaticky otev�ou v LyXu."
LangString SecDesktopDescription "${LYX_LANG}" "Ikonu LyXu na plochu."

LangString LangSelectHeader "${LYX_LANG}" "V�b�r jazyka prost�ed� LyXu"
LangString AvailableLang "${LYX_LANG}" " Dostupn� jazyky "

LangString MissProgHeader "${LYX_LANG}" "Kontrola pot�ebn�ch program�"
LangString MissProgCap "${LYX_LANG}" "N�sleduj�c� programy budou p�iinstalov�ny"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, distribuce LaTeXu"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, interpretr PostScriptu a PDF soubor�"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, konvertor obr�zk�"
LangString MissProgAspell "${LYX_LANG}" "Aspell, kontrola pravopisu"
LangString MissProgMessage "${LYX_LANG}" '��dn� dal�� programy nen� t�eba p�iinstalovat.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribuce LaTeXu'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Nastavte distribuci LaTeXu, kterou m� LyX pou��vat.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'M��ete zde voliteln� nastavit cestu k souboru "latex.exe" a t�m ur�it \
					   distribuci LaTeXu, kterou bude pou��vat Lyx.\r\n\
					   Pokud nepou�ijete LaTeX, LyX nem��e vys�zet v�sledn� dokumenty!\r\n\
					   \r\n\
					   Instal�tor nalezl na va�em po��ta�i LaTeXovou distribuci \
					   "$LaTeXName". N�e je uvedena jej� cesta.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'N�e nastavte cestu k souboru "latex.exe". Ur��te t�m, kter� \
					       distribuce LaTeXu bude pou��v�na LyXem.\r\n\
					       Pokud nepou�ijete LaTeX, LyX nem��e vys�zet v�sledn� dokumenty!\r\n\
					       \r\n\
					       Instal�tor nemohl nal�zt distribuci LaTeXu na va�em po��ta�i.'
LangString PathName "${LYX_LANG}" 'Cesta k souboru "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Nepou��vat LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Soubor "latex.exe" nen� v zadan� cest�.'

LangString LatexInfo "${LYX_LANG}" 'Nyn� bude spu�t�n instal�tor LaTeXov�-distribuce "MiKTeX".$\r$\n\
			            V instal�toru pokra�ujte tla��tkem "Next" dokud instalace neza�ne.$\r$\n\
				    $\r$\n\
				    !!! U�ijte v�echny p�edvolby instal�toru MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" '��dn� LaTeXov�-distribuce nebyla nalezena!$\r$\n\
                      		      Nem��ete pou��vat LyX bez distribuce LaTeXu jako je nap�. "MiKTeX"!$\r$\n\
				      Instalace bude proto p�eru�ena.'
			    
LangString GSviewInfo "${LYX_LANG}" 'Nyn� bude spu�t�n instal�tor programu "GSview".$\r$\n\
			             K jeho instalaci zm��kn�te nejprve tla��tko "Setup" v prvn�m okn�,$\r$\n\
				     vyberte jazyk a stiskn�t� tla��tko "Next" v n�sleduj�c�m okn�.$\r$\n\
				     M��ete pou��t v�echny p�edvolby tohoto instal�toru.'
LangString GSviewError "${LYX_LANG}" 'Program "GSview" nebyl �sp�n� nainstalov�n!$\r$\n\
		                      Instal�tor bude navzdory tomu pokra�ovat.$\r$\n\
				      Pokuste se nainstalovat GSview pozd�ji.'
				      
LangString JabRefInfo "${LYX_LANG}" 'Nyn� bude spu�t�n instal�tor programu "JabRef".$\r$\n\
				     M��ete pou��t v�echny p�edvolby instal�toru JabRef.'
LangString JabRefError "${LYX_LANG}" 'Program "JabRef" nebyl �sp�n� nainstalov�n.$\r$\n\
		                      Instal�tor bude navzdory tomu pokra�ovat.$\r$\n\
				      Pokuste se nainstalovat JabRef pozd�ji.'
				      
LangString LatexConfigInfo "${LYX_LANG}" "N�sleduj�c� konfigurov�n� LyXu chv�li potrv�."

LangString AspellInfo "${LYX_LANG}" 'Nyn� budou sta�eny a nainstalov�ny slovn�ky pro kontrolu pravopisu programu "Aspell".$\r$\n\
				     Ka�d� slovn�k m� odli�nou licenci, kter� bude zobrazena p�ed instalac�.'
LangString AspellDownloadFailed "${LYX_LANG}" "Nelze st�hnout ��dn� slovn�ku Aspellu!"
LangString AspellInstallFailed "${LYX_LANG}" "Nelze nainstalovat ��dn� slovn�k Aspellu!"
LangString AspellPartAnd "${LYX_LANG}" " a "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Slovn�k jazyka je ji� naistalov�n'
LangString AspellNone "${LYX_LANG}" 'Nebyl nainstalov�n ��dn� slovn�k pro Aspell.$\r$\n\
				     Slovn�ky pro kontrolu pravopisu lze st�hnout z$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Chcete je st�hnout nyn�?'
LangString AspellPartStart "${LYX_LANG}" "Byl �sp�n� nainstalov�n"
LangString AspellPart1 "${LYX_LANG}" "anglick� slovn�k"
LangString AspellPart2 "${LYX_LANG}" "slovn�k jazyka: $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
				      slovn�k jazyka: $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' pro kontrolu pravopisu programem "Aspell".$\r$\n\
				      V�ce slovn�k� m��e b�t sta�eno z$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Chcete nyn� st�hnout jin� slovn�k ?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Aby ka�d� u�ivatel mohl pozd�ji p�izp�sobit MiKTeX pro sv� pot�eby$\r$\n\
					 je t�eba nastavit pr�va z�pisu pro v�echny u�ivatele do adres��e MiKTeXu$\r$\n\
					 $MiKTeXPath $\r$\n\
					 a v�ech jeho podadres���."
LangString MiKTeXInfo "${LYX_LANG}" 'LaTeXov� distribuce "MiKTeX" bude pou�ita dohromady s LyXem.$\r$\n\
				     Je doporu�eno nainstalovat dostupn� aktualizace MiKTeXu pomoc� programu "MiKTeX Update Wizard"$\r$\n\
				     p�edt�m ne� poprv� spust�te LyX.$\r$\n\
				     Chcete zkontrolovat dostupn� aktualizace MiKTeXu nyn�?'
				     
LangString UpdateNotAllowed "${LYX_LANG}" "Tento aktualiza�n� bal��ek m��e b�t pou��t jen na ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Nelze nastavit 'path_prefix' v konfigura�n�m skriptu"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Nelze vytvo�it lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Nelze spustit konfigura�n� skript"
LangString NotAdmin "${LYX_LANG}" "Pot�ebujete administr�torsk� pr�va pro nainstalov�n� LyXu!"
LangString InstallRunning "${LYX_LANG}" "Instal�tor je ji� spu�t�n!"
LangString StillInstalled "${LYX_LANG}" "LyX je ji� nainstalov�n! Nejprve LyX odinstalujte."

LangString FinishPageMessage "${LYX_LANG}" "Blahop�ejeme! LyX byl �sp�n� nainstalov�n.\r\n\
					    \r\n\
					    (Prvn� spu�t�n� LyXu m��e trvat del�� dobu.)"
LangString FinishPageRun "${LYX_LANG}" "Spustit LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nelze nal�zt LyX v registrech.$\r$\n\
					       Z�stupce na plo�e a ve Start menu nebude smaz�n."
LangString UnInstallRunning "${LYX_LANG}" "Nejprve mus�te zav��t LyX!"
LangString UnNotAdminLabel "${LYX_LANG}" "Mus�te m�t administr�torsk� pr�va pro odinstalov�n� LyXu!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Chcete opravdu smazat LyX a v�echny jeho komponenty?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'U�ivatelsk� nastaven� LyXu'
LangString UnGSview "${LYX_LANG}" 'Zvolte tla��tko "Uninstall" v dal��m okn� pro odinstalov�n�$\r$\n\
				   prohl�e�e "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX byl �sp�n� odstran�n z va�eho po��ta�e."

LangString SecUnAspellDescription "${LYX_LANG}" 'Odinstalovat program pro kontrolu pravopisu Aspell a v�echny jeho slovn�ky.'
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Odinstalovat LaTeXovou-distribuci MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Odinstalovat mana�er bibliografie JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Smazat konfigura�n� adres�� LyXu$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   pro v�echny u�ivatele.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Odinstalovat LyX a v�echny jeho komponenty."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_CZECH_NSH_
