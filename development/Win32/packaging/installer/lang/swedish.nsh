!insertmacro LANGFILE_EXT "Swedish"

${LangFileString} WelcomePageText "Denna guide tar dig igenom installationen av LyX.$\r$\n\
					  $\r$\n\
					  Du beh�ver administrat�rsr�ttigheter f�r att installera LyX.$\r$\n\
					  $\r$\n\
					  Det rekommenderas att du avslutar alla andra program innan du forts�tter installationen. Detta till�ter att installationen uppdaterar n�dv�ndiga systemfiler utan att beh�va starta om din dator.$\r$\n\
					  $\r$\n\
					  $_CLICK"

${LangFileString} FileTypeTitle "LyX-dokument"

${LangFileString} SecInstGSviewTitle "GSview"
${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "Installera f�r alla anv�ndare?"
${LangFileString} SecFileAssocTitle "Filassociationer"
${LangFileString} SecDesktopTitle "Skrivbordsikon"

${LangFileString} SecCoreDescription "LyX-filer."
${LangFileString} SecInstGSviewDescription "Program f�r att visa Postscript- och PDF-dokument."
${LangFileString} SecInstJabRefDescription "Program f�r att hantera bibliografiska referenser och redigera BibTeX-filer."
${LangFileString} SecAllUsersDescription "Installera LyX f�r alla anv�ndare, eller enbart f�r den aktuella anv�ndare."
${LangFileString} SecFileAssocDescription "Skapa en association mellan programmet och filtill�get .lyx."
${LangFileString} SecDesktopDescription "En LyXikon p� skrivbordet."

${LangFileString} LangSelectHeader "Val av spr�k i LyXs menyer"
${LangFileString} AvailableLang " Tillg�ngliga spr�k "

${LangFileString} EnterLaTeXHeader1 'LaTeX-distribution'
${LangFileString} EnterLaTeXHeader2 'St�ll in LaTeX-distributionen som LyX skall anv�nda.'
${LangFileString} EnterLaTeXFolder 'Alternativt kan du h�r st�lla in s�kv�gen till filen $\"latex.exe$\" och d�rmed manuellt best�mma vilken \
					   LaTeX-distribution som skall anv�ndas av LyX.$\r$\n\
					   Utan LaTeX kan LyX bara redigera LyX-filer, inte producera t.ex. PDF-dokument!$\r$\n\
					   $\r$\n\
					   Installationsprogrammet har uppt�ckt LaTeX-distributionen \
					   $\"$LaTeXName$\" i ditt system. Nedan visas dess s�kv�g.'
${LangFileString} EnterLaTeXFolderNone 'S�tt nedan s�kv�gen till filen $\"latex.exe$\". D�rmed v�ljer du vilken \
					       LaTeX-distribution som skall anv�ndas av LyX.$\r$\n\
					       Utan LaTeX kan LyX bara redigera LyX-filer, inte producera t.ex. PDF-dokument!$\r$\n\
					       $\r$\n\
					       Installationsprogrammetn kunde inte hitta LaTeX-distributionen i ditt system.'
${LangFileString} PathName 'S�kv�g till filen $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "Anv�nd inte LaTeX"
${LangFileString} InvalidLaTeXFolder 'Kan inte hitta $\"latex.exe$\"'

${LangFileString} LatexInfo 'Nu kommer installationsprogrammet f�r LaTeX-distribution $\"MiKTeX$\" att k�ras.$\r$\n\
			            F�r att installera programmet, klicka p� knappen $\"Next$\" i installationsf�nstret tills installation b�rjar.$\r$\n\
				    $\r$\n\
				    !!! Var sn�ll och anv�nd standardinst�llningarna i installationsprogrammet f�r MiKTeX !!!'
${LangFileString} LatexError1 'Kan inte hitta n�gon LaTeX-distribution!$\r$\n\
                      		      LyX kan inte anv�ndas utan en LaTeX-distribution som tex $\"MiKTeX$\"!$\r$\n\
				      Installation avbryts d�rf�r nu.'
				    
${LangFileString} GSviewInfo 'Nu kommer installationsprogrammet f�r $\"GSview$\" att k�ras.$\r$\n\
			             F�r att installera programmet, klicka p� knappen $\"Setup$\" i den f�rsta dialogrutan i installationsprogrammet\r$\n\
				     v�lj ett spr�k och klicka sedan p� knappen $\"Next$\" i det n�stkommande installationsf�nstret.$\r$\n\
				     Du kan anv�nda alla standardalternativ i installationsprogrammet f�r GSview.'
${LangFileString} GSviewError 'Programmet $\"GSview$\" kunde inte installeras ordentligt!$\r$\n\
		                      Installationen av LyX kommer att forts�tta �nd�.$\r$\n\
				      F�rs�k installera GSview igen senare.'

${LangFileString} JabRefInfo 'Nu kommer installationsprogrammet f�r $\"JabRef$\" att k�ras.$\r$\n\
				     Du kan anv�nda alla standardalternativ i installationsprogrammet f�r JabRef.'
${LangFileString} JabRefError 'Programmet $\"JabRef$\" kunde inte installeras ordentligt!$\r$\n\
		                      Installationen av LyX kommer att forts�tta �nd�.$\r$\n\
				      F�rs�k installera GSview igen senare.'
				      
${LangFileString} LatexConfigInfo "F�ljande konfigurering av LyX kommer att ta ett tag."

${LangFileString} MiKTeXPathInfo "F�r att varje anv�ndare senare skall kunna anpassa MiKTeX f�r sina behov$\r$\n\
					 �r det n�dv�ndigt att ge skrivr�ttigheter till alla anv�ndare f�r MiKTeX's installationskatalog$\r$\n\
					 $MiKTeXPath $\r$\n\
					 och dess underkataloger."
${LangFileString} MiKTeXInfo 'LaTeX-distributionen $\"MiKTeX$\" kommer att anv�ndas tillsammans med LyX.$\r$\n\
				     Det rekommenderas att installera tillg�ngliga uppdateringar av MiKTeX med hj�lp av programmet $\"MiKTeX Update Wizard$\".$\r$\n\
				     innan du anv�nder LyX f�r f�rsta g�ngen.$\r$\n\
				     Vill du kontrollera om det finns uppdateringar tillg�ngliga f�r MiKTeX nu?'

${LangFileString} UpdateNotAllowed "This update package can only update ${PRODUCT_VERSION_OLD}!"
${LangFileString} ModifyingConfigureFailed "Misslyckades med att s�tta  'path_prefix' i konfigurationsskriptet"
${LangFileString} CreateCmdFilesFailed "Kunde inte skapa filen (kommandoskriptet) lyx.bat"
${LangFileString} RunConfigureFailed "Misslyckades med att k�ra konfigurationsskriptet"
${LangFileString} NotAdmin "Du m�ste ha administrat�rsr�ttigheter f�r att installera LyX!"
${LangFileString} InstallRunning "Installationsprogrammet k�rs readan!"
${LangFileString} StillInstalled "LyX �r redan installerat! Avinstallera LyX f�rst."

${LangFileString} FinishPageMessage "Gratulerar! LyX har installerats framg�ngsrikt.$\r$\n\
					    $\r$\n\
					    (F�rsta g�ngen LyX startas tar det litet l�ngre tid d� programmet konfigurerar sig sj�lv automatiskt.)"
${LangFileString} FinishPageRun "K�r LyX"

${LangFileString} UnNotInRegistryLabel "Kan inte hitta LyX i registret\r$\n\
					       Genv�gar p� skrivbordet och i startmeny kommer inte att tas bort."
${LangFileString} UnInstallRunning "Du m�ste st�nga LyX f�rst!"
${LangFileString} UnNotAdminLabel "Du m�ste ha administrat�rsr�ttigheter f�r att avinstallera LyX!"
${LangFileString} UnReallyRemoveLabel "�r du s�ker p� att du verkligen vill fullst�ndigt avinstallera LyX och alla dess komponenter?"
${LangFileString} UnLyXPreferencesTitle 'LyX$\'s anv�ndarinst�llningar'
${LangFileString} UnGSview 'Var god tryck p� knappen $\"Avinstallera$\" i f�ljande f�nster f�r att avinstallera$\r$\n\
				    $\"GSview$\" som visar Postscript och PDF.'
${LangFileString} UnRemoveSuccessLabel "LyX har framg�ngsrikt avinstallerats fr�n din dator."

${LangFileString} SecUnMiKTeXDescription "Avinstallerar LaTeX-distributionen MiKTeX."
${LangFileString} SecUnJabRefDescription "Avinstallerar programmet JabRef som anv�nds f�r att hantera bibliografiska referenser."
${LangFileString} SecUnPreferencesDescription 'Raderar LyX$\'s konfigurationskatalog$\r$\n\
					   $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\"$\r$\n\
					   f�r alla anv�ndare.'
${LangFileString} SecUnProgramFilesDescription "Avinstallera LyX och alla dess komponenter."

