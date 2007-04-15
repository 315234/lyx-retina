!ifndef _LYX_LANGUAGES_SWEDISH_NSH_
!define _LYX_LANGUAGES_SWEDISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_SWEDISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Denna guide tar dig igenom installationen av LyX.\r\n\
					  \r\n\
					  Du beh�ver administrat�rsr�ttigheter f�r att installera LyX.\r\n\
					  \r\n\
					  Det rekommenderas att du avslutar alla andra program innan du forts�tter installationen. Detta till�ter att installationen uppdaterar n�dv�ndiga systemfiler utan att beh�va starta om din dator.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-dokument"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Installera f�r alla anv�ndare?"
LangString SecFileAssocTitle "${LYX_LANG}" "Filassociationer"
LangString SecDesktopTitle "${LYX_LANG}" "Skrivbordsikon"

LangString SecCoreDescription "${LYX_LANG}" "LyX-filer."
LangString SecInstGSviewDescription "${LYX_LANG}" "Program f�r att visa Postscript- och PDF-dokument."
LangString SecInstJabRefDescription "${LYX_LANG}" "Program f�r att hantera bibliografiska referenser och redigera BibTeX-filer."
LangString SecAllUsersDescription "${LYX_LANG}" "Installera LyX f�r alla anv�ndare, eller enbart f�r den aktuella anv�ndare."
LangString SecFileAssocDescription "${LYX_LANG}" "Skapa en association mellan programmet och filtill�get .lyx."
LangString SecDesktopDescription "${LYX_LANG}" "En LyXikon p� skrivbordet."

LangString LangSelectHeader "${LYX_LANG}" "Val av spr�k i LyXs menyer"
LangString AvailableLang "${LYX_LANG}" " Tillg�ngliga spr�k "

LangString MissProgHeader "${LYX_LANG}" "Leta efter n�dv�ndiga program"
LangString MissProgCap "${LYX_LANG}" "F�ljande n�dv�ndiga program kommer dessutom att installeras"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, en LaTeX-distribution"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, ett konverteringsprogram mm f�r PostScript och PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, ett konverteringsprogram f�r bilder"
LangString MissProgAspell "${LYX_LANG}" "Aspell, ett r�ttstavningsprogram"
LangString MissProgMessage "${LYX_LANG}" 'Inga ytterligare program beh�ver installeras.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'LaTeX-distribution'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'St�ll in LaTeX-distributionen som LyX skall anv�nda.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Alternativt kan du h�r st�lla in s�kv�gen till filen "latex.exe" och d�rmed manuellt best�mma vilken \
					   LaTeX-distribution som skall anv�ndas av LyX.\r\n\
					   Utan LaTeX kan LyX bara redigera LyX-filer, inte producera t.ex. PDF-dokument!\r\n\
					   \r\n\
					   Installationsprogrammet har uppt�ckt LaTeX-distributionen \
					   "$LaTeXName" i ditt system. Nedan visas dess s�kv�g.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'S�tt nedan s�kv�gen till filen "latex.exe". D�rmed v�ljer du vilken \
					       LaTeX-distribution som skall anv�ndas av LyX.\r\n\
					       Utan LaTeX kan LyX bara redigera LyX-filer, inte producera t.ex. PDF-dokument!\r\n\
					       \r\n\
					       Installationsprogrammetn kunde inte hitta LaTeX-distributionen i ditt system.'
LangString PathName "${LYX_LANG}" 'S�kv�g till filen "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Anv�nd inte LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Kan inte hitta "latex.exe"'

LangString LatexInfo "${LYX_LANG}" 'Nu kommer installationsprogrammet f�r LaTeX-distribution "MiKTeX" att k�ras.$\r$\n\
			            F�r att installera programmet, klicka p� knappen "Next" i installationsf�nstret tills installation b�rjar.$\r$\n\
				    $\r$\n\
				    !!! Var sn�ll och anv�nd standardinst�llningarna i installationsprogrammet f�r MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" 'Kan inte hitta n�gon LaTeX-distribution!$\r$\n\
                      		      LyX kan inte anv�ndas utan en LaTeX-distribution som tex "MiKTeX"!$\r$\n\
				      Installation avbryts d�rf�r nu.'
				    
LangString GSviewInfo "${LYX_LANG}" 'Nu kommer installationsprogrammet f�r "GSview" att k�ras.$\r$\n\
			             F�r att installera programmet, klicka p� knappen "Setup" i den f�rsta dialogrutan i installationsprogrammet\r$\n\
				     v�lj ett spr�k och klicka sedan p� knappen "Next" i det n�stkommande installationsf�nstret.$\r$\n\
				     Du kan anv�nda alla standardalternativ i installationsprogrammet f�r GSview.'
LangString GSviewError "${LYX_LANG}" 'Programmet "GSview" kunde inte installeras ordentligt!$\r$\n\
		                      Installationen av LyX kommer att forts�tta �nd�.$\r$\n\
				      F�rs�k installera GSview igen senare.'

LangString JabRefInfo "${LYX_LANG}" 'Nu kommer installationsprogrammet f�r "JabRef" att k�ras.$\r$\n\
				     Du kan anv�nda alla standardalternativ i installationsprogrammet f�r JabRef.'
LangString JabRefError "${LYX_LANG}" 'Programmet "JabRef" kunde inte installeras ordentligt!$\r$\n\
		                      Installationen av LyX kommer att forts�tta �nd�.$\r$\n\
				      F�rs�k installera GSview igen senare.'
				      
LangString LatexConfigInfo "${LYX_LANG}" "F�ljande konfigurering av LyX kommer att ta ett tag."

LangString AspellInfo "${LYX_LANG}" 'Nu kommer ordlistor f�r stavningsprogrammet "Aspell" att laddas ner och installeras.$\r$\n\
				     Varje ordlista har en egen licens vilken kommer att visas f�re installationen.'
LangString AspellDownloadFailed "${LYX_LANG}" "Ingen ordlista f�r stavningsprogrammet Aspell kunde laddas ner!"
LangString AspellInstallFailed "${LYX_LANG}" "Ingen ordlista f�r stavningsprogrammet Aspell kunde installeras!"
LangString AspellPartAnd "${LYX_LANG}" " and "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Det finns redan en installerad ordlista f�r spr�ket'
LangString AspellNone "${LYX_LANG}" 'Ingen ordlista f�r stavningsprogrammet "Aspell" har blivit installerad.$\r$\n\
				     Ordlistor kan laddas ner fr�n\r$\n\
				     ${AspellLocation}$\r$\n\
				     Vill du ladda ner ordlistor nu?'
LangString AspellPartStart "${LYX_LANG}" "Det installerades framg�ngsrikt "
LangString AspellPart1 "${LYX_LANG}" "en engelsk ordlista"
LangString AspellPart2 "${LYX_LANG}" "en ordlista f�r spr�ket $LangName"
LangString AspellPart3 "${LYX_LANG}" "en$\r$\n\
				      ordlista f�r spr�ket $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' f�r stavningsprogrammet "Aspell".$\r$\n\
				      Ytterligare ordlistor kan laddas ner fr�n\r$\n\
				      ${AspellLocation}$\r$\n\
				      Vill du ladda ner ytterligare ordlistor nu?'

LangString MiKTeXPathInfo "${LYX_LANG}" "F�r att varje anv�ndare senare skall kunna anpassa MiKTeX f�r sina behov$\r$\n\
					 �r det n�dv�ndigt att ge skrivr�ttigheter till alla anv�ndare f�r MiKTeX's installationskatalog$\r$\n\
					 $MiKTeXPath $\r$\n\
					 och dess underkataloger."
LangString MiKTeXInfo "${LYX_LANG}" 'LaTeX-distributionen "MiKTeX" kommer att anv�ndas tillsammans med LyX.$\r$\n\
				     Det rekommenderas att installera tillg�ngliga uppdateringar av MiKTeX med hj�lp av programmet "MiKTeX Update Wizard".$\r$\n\
				     innan du anv�nder LyX f�r f�rsta g�ngen.$\r$\n\
				     Vill du kontrollera om det finns uppdateringar tillg�ngliga f�r MiKTeX nu?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update LyX ${PRODUCT_UPDATE_ALLOWED}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Misslyckades med att s�tta  'path_prefix' i konfigurationsskriptet"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Kunde inte skapa filen (kommandoskriptet) lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Misslyckades med att k�ra konfigurationsskriptet"
LangString NotAdmin "${LYX_LANG}" "Du m�ste ha administrat�rsr�ttigheter f�r att installera LyX!"
LangString InstallRunning "${LYX_LANG}" "Installationsprogrammet k�rs readan!"
LangString StillInstalled "${LYX_LANG}" "LyX �r redan installerat! Avinstallera LyX f�rst."

LangString FinishPageMessage "${LYX_LANG}" "Gratulerar! LyX har installerats framg�ngsrikt.\r\n\
					    \r\n\
					    (F�rsta g�ngen LyX startas tar det litet l�ngre tid d� programmet konfigurerar sig sj�lv automatiskt.)"
LangString FinishPageRun "${LYX_LANG}" "K�r LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Kan inte hitta LyX i registret\r$\n\
					       Genv�gar p� skrivbordet och i startmeny kommer inte att tas bort."
LangString UnInstallRunning "${LYX_LANG}" "Du m�ste st�nga LyX f�rst!"
LangString UnNotAdminLabel "${LYX_LANG}" "Du m�ste ha administrat�rsr�ttigheter f�r att avinstallera LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "�r du s�ker p� att du verkligen vill fullst�ndigt avinstallera LyX och alla dess komponenter?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX$\'s anv�ndarinst�llningar'
LangString UnGSview "${LYX_LANG}" 'Var god tryck p� knappen "Avinstallera" i f�ljande f�nster f�r att avinstallera$\r$\n\
				    "GSview" som visar Postscript och PDF.'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX har framg�ngsrikt avinstallerats fr�n din dator."

LangString SecUnAspellDescription "${LYX_LANG}" "Avinstallerar stavningsprogrammet Aspell och alla dess kataloger."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Avinstallerar LaTeX-distributionen MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Avinstallerar programmet JabRef som anv�nds f�r att hantera bibliografiska referenser."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Raderar LyX$\'s konfigurationskatalog$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   f�r alla anv�ndare.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Avinstallera LyX och alla dess komponenter."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_SWEDISH_NSH_
