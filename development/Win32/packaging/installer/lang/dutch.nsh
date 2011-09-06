!insertmacro LANGFILE_EXT "Dutch"

${LangFileString} WelcomePageText "Dit installatie programma zal LyX op uw systeem installeren.$\r$\n\
					  $\r$\n\
					  U heeft systeem-beheerrechten nodig om LyX te installeren.$\r$\n\
					  $\r$\n\
					  Het is aanbevolen om alle andere programma's af te sluiten voordat u Setup start. Dit zorgt ervoor dat Setup bepaalde systeembestanden kan bijwerken zonder uw systeem opnieuw op te starten.$\r$\n\
					  $\r$\n\
					  $_CLICK"

${LangFileString} FileTypeTitle "LyX-Document"

${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "Installeer voor alle gebruikers?"
${LangFileString} SecFileAssocTitle "Bestand associaties"
${LangFileString} SecDesktopTitle "Bureaublad pictogram"

${LangFileString} SecCoreDescription "De LyX bestanden."
${LangFileString} SecInstJabRefDescription "Bibliography reference manager and editor for BibTeX files."
${LangFileString} SecAllUsersDescription "Installeer LyX voor alle gebruikers of uitsluitend de huidige gebruiker?"
${LangFileString} SecFileAssocDescription "Associeer het LyX programma met de .lyx extensie."
${LangFileString} SecDesktopDescription "Een LyX pictogram op het Bureaublad."

${LangFileString} LangSelectHeader "Taalselectie voor LyX's menu"
${LangFileString} AvailableLang " Beschikbare Talen "

${LangFileString} EnterLaTeXHeader1 'LaTeX software'
${LangFileString} EnterLaTeXHeader2 'Geef aan welke LaTeX-software LyX moet gebruiken.'
${LangFileString} EnterLaTeXFolder 'U kunt hier opgeven in welke map het programma $\"latex.exe$\" zich bevindt en op deze manier bepalen \
					   welke LaTeX software gebruikt wordt door LyX.$\r$\n\
					   Zonder LaTeX kan LyX geen documenten generen!$\r$\n\
					   $\r$\n\
					   Het installatie programma heeft de LaTeX software \
					   $\"$LaTeXName$\" op uw systeem gevonden in de volgende map.'
${LangFileString} EnterLaTeXFolderNone 'U kunt hier opgeven in welke map het programma $\"latex.exe$\" zich bevindt en op deze manier bepalen \
					   welke LaTeX software gebruikt wordt door LyX.$\r$\n\
					   Zonder LaTeX kan LyX geen documenten generen!$\r$\n\
					       $\r$\n\
					       Het installatie programma heeft geen LaTeX software op uw systeem gevonden.'
${LangFileString} PathName 'Map met het programma $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "Gebruik geen LaTeX"
${LangFileString} InvalidLaTeXFolder '$\"latex.exe$\" is niet gevonden.'

${LangFileString} LatexInfo 'Het installatieprogramma van $\"MiKTeX$\" word gestart.$\r$\n\
			            Om het programma te installeren klik op de $\"Next$\"-knop in het installatie venster totdat de installatie begint.$\r$\n\
				    $\r$\n\
				    !!! Gebruik de standaard opties tijdens de installatie van MiKTeX !!!'
${LangFileString} LatexError1 'Er is geen LaTeX software gevonden!$\r$\n\
                      		      LyX kan niet worden gebruikt zonder LaTeX software zoals $\"MiKTeX$\"!$\r$\n\
				      De installatie wordt daarom afgebroken.'

${LangFileString} JabRefInfo 'Het installatie programma van $\"JabRef$\" wordt nu gestart.$\r$\n\
				     U kunt de standaard opties gebruiken.'
${LangFileString} JabRefError 'Het programma $\"JabRef$\" kon niet worden geinstalleerd!$\r$\n\
		                      De LyX installatie gaat desondanks verder.$\r$\n\
				      Probeer JabRef later te installeren.'
				      
${LangFileString} LatexConfigInfo "De volgende configuratie van LyX zal enige tijd duren."

${LangFileString} MiKTeXPathInfo "Opdat elke gebruiker zijn MiKTeX configuratie kan aanpassen$\r$\n\
					 is het noodzakelijk alle gebruikers schrijfpermissie te geven in MiKTeX's installatie map$\r$\n\
					 $MiKTeXPath $\r$\n\
					 en submappen."
${LangFileString} MiKTeXInfo 'LyX gebruikt de LaTeX software $\"MiKTeX$\".$\r$\n\
				     Het is aanbevolen MiKTeX-updates te installeren via de $\"MiKTeX Update Wizard$\"$\r$\n\
				     voordat u LyX voor de eerste keer gebruikt.$\r$\n\
				     Wilt u controleren of er updates voor MiKTeX beschikbaar zijn?'

${LangFileString} UpdateNotAllowed "This update package can only update ${PRODUCT_VERSION_OLD}!"
${LangFileString} ModifyingConfigureFailed "Mislukte poging om 'path_prefix' te registreren tijdens de configuratie"
${LangFileString} CreateCmdFilesFailed "lyx.bat kon niet worden aangemaakt"
${LangFileString} RunConfigureFailed "Mislukte configuratie poging"
${LangFileString} NotAdmin "U heeft systeem-beheerrechten nodig om LyX te installeren!"
${LangFileString} InstallRunning "Het installatieprogramma is al gestart!"
${LangFileString} StillInstalled "LyX is reeds geinstalleerd! Verwijder LyX eerst."

${LangFileString} FinishPageMessage "Gefeliciteerd! LyX is succesvol geinstalleerd.$\r$\n\
					    $\r$\n\
					    (De eerste keer dat u LyX start kan dit enige seconden duren.)"
${LangFileString} FinishPageRun "Start LyX"

${LangFileString} UnNotInRegistryLabel "LyX is niet gevonden in het Windows register.$\r$\n\
					       Snelkoppelingen op het Bureaublad en in het Start Menu worden niet verwijderd."
${LangFileString} UnInstallRunning "U moet LyX eerst afsluiten!"
${LangFileString} UnNotAdminLabel "U heeft systeem-beheerrechten nodig om LyX te verwijderen!"
${LangFileString} UnReallyRemoveLabel "Weet u zeker dat u LyX en alle componenten volledig wil verwijderen van deze computer?"
${LangFileString} UnLyXPreferencesTitle 'LyX$\'s user preferences'

${LangFileString} SecUnMiKTeXDescription "Verwijder de LaTeX software MiKTeX."
${LangFileString} SecUnJabRefDescription "Verwijder de bibliografie manager JabRef."
${LangFileString} SecUnPreferencesDescription 'Verwijder LyX$\'s configuratie map$\r$\n\
					   $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\"$\r$\n\
					   voor alle gebruikers.'
${LangFileString} SecUnProgramFilesDescription "Verwijder LyX en alle bijbehorende onderdelen."

