!ifndef _LYX_LANGUAGES_FRENCH_NSH_
!define _LYX_LANGUAGES_FRENCH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_FRENCH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Cet assistant vous guidera durant l'installation de LyX.\r\n\
					  \r\n\
					  Vous devez �tre administrateur pour installer LyX.\r\n\
					  \r\n\
					  Avant de d�buter l'installation, il est recommand� de fermer toutes les autres applications. Cela permettra de mettre � jour certains fichiers syst�me sans red�marrer votre ordinateur.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Document LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Installer pour tous les utilisateurs ?"
LangString SecFileAssocTitle "${LYX_LANG}" "Associations de fichiers"
LangString SecDesktopTitle "${LYX_LANG}" "Ic�ne du bureau"

LangString SecCoreDescription "${LYX_LANG}" "Les fichiers LyX"
LangString SecInstGSviewDescription "${LYX_LANG}" "Application permettant d'afficher les documents PostScript - et PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Bibliography reference manager and editor for BibTeX files."
LangString SecAllUsersDescription "${LYX_LANG}" "Installer LyX pour tous les utilisateurs, ou seulement pour l'utilisateur courant ?."
LangString SecFileAssocDescription "${LYX_LANG}" "Les fichiers de suffixe .lyx seront automatiquement ouverts dans LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Une ic�ne LyX sur le bureau."

LangString LangSelectHeader "${LYX_LANG}" "S�lection de la langue pour les menus de LyX"
LangString AvailableLang "${LYX_LANG}" " Langues disponibles"

LangString MissProgHeader "${LYX_LANG}" "V�rification des applications requises."
LangString MissProgCap "${LYX_LANG}" "Le(s) application(s) requise(s) suivante(s) seront �galement install�es."
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, une distribution LaTeX"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, un interpr�teur pour PostScript and PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, un convertisseur graphique"
LangString MissProgAspell "${LYX_LANG}" "Aspell, un correcteur orthographique"
LangString MissProgMessage "${LYX_LANG}" 'Aucune application suppl�mentaire ne doit �tre install�e.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribution LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Choisir la distribution LaTeX que LyX devra utiliser.'
LangString EnterLaTeXFolder "${LYX_LANG}" '�ventuellement, vous pouvez fixer ici le chemin d$\'acc�s au fichier "latex.exe" et de ce fait fixer \
					   la distribution LaTeX utilis�e par Lyx.\r\n\
					   Si vous n$\'utilisez pas LaTeX, LyX ne peut cr�er de documents !\r\n\
					   \r\n\
					   L$\'assistant d$\'installation a d�tect� la distribution LaTeX \
					   "$LaTeXName" sur votre syst�me. Le chemin d$\'acc�s est affich� ci-dessous.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Fixez ci-dessous le chemin d$\'acc�s au fichier "latex.exe". De ce fait vous fixez la \
					       distribution LaTeX utilis�e par Lyx.\r\n\
					       Si vous n$\'utilisez pas LaTeX, LyX ne peut cr�er de documents !\r\n\
					       \r\n\
					       L$\'assistant d$\'installation n$\'a pas pu trouver de distribution LaTeX sur votre syst�me.'
LangString PathName "${LYX_LANG}" 'Chemin vers le fichier "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "N'utilisez pas LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" '"latex.exe" introuvable.'

LangString LatexInfo "${LYX_LANG}" 'Lancement de l$\'installation de la distribution LaTeX "MiKTeX".$\r$\n\
			            Pour installer l$\'application, appuyer sur le bouton "Next" de la fen�tre d$\'installation jusqu$\'� ce que celle-ci commence.$\r$\n\
				    $\r$\n\
				    !!! Prenez garde � utiliser toutes les options implicites du programme d$\'installation MikTeX !!!'
LangString LatexError1 "${LYX_LANG}" 'Distribution LaTeX introuvable !$\r$\n\
                      		      LyX ne peut �tre utilis� sans distribution LaTeX, comme par exemple "MiKTeX" !$\r$\n\
				      L$\'installation de LyX va donc �tre abandonn�e.'
			    
LangString GSviewInfo "${LYX_LANG}" 'Lancement de l$\'installation de l$\'application "GSview".$\r$\n\
			             Pour installer l$\'application, appuyer sur le bouton "Setup" de la premi�re fen�tre d$\'installation$\r$\n\
				     choisir une langue, puis appuyer sur le bouton "Next" de la fen�tre d$\'installation suivante.$\r$\n\
				     Vous pouvez utiliser les options implicites du programme d$\'installation GSview.'
LangString GSviewError "${LYX_LANG}" 'L$\'application "GSview" n$\'a pas pu �tre install�e correctement !$\r$\n\
		                      L$\'installation de LyX va continuer.$\r$\n\
				      Essayer d$\'installer GSview de nouveau plus tard.'

LangString JabRefInfo "${LYX_LANG}" 'Lancement de l$\'installation de l$\'application "JabRef".$\r$\n\
				     Vous pouvez utiliser les options implicites du programme d$\'installation JabRef.'
LangString JabRefError "${LYX_LANG}" 'L$\'application "JabRef" n$\'a pas pu �tre install�e correctement !$\r$\n\
		                      L$\'installation de LyX va continuer.$\r$\n\
				      Essayer d$\'installer JabRef de nouveau plus tard.'
				      
LangString LatexConfigInfo "${LYX_LANG}" "La configuration de LyX qui va suivre prendra un moment."

LangString AspellInfo "${LYX_LANG}" 'Maintenant les dictionnaires du correcteur orthographique "Aspell" vont �tre t�l�charg�s et install�s.$\r$\n\
				     Chaque dictionnaire est dot� d$\'une licence diff�rente qui  sera affich�e avant l$\'installation.'
LangString AspellDownloadFailed "${LYX_LANG}" "Aucun dictionnaire Aspell n$\'a pu �tre t�l�charg� !"
LangString AspellInstallFailed "${LYX_LANG}" "Aucun dictionnaire Aspell n$\'a pu �tre install� !"
LangString AspellPartAnd "${LYX_LANG}" " et "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Un dictionnaire est d�j� install� pour la langue'
LangString AspellNone "${LYX_LANG}" 'Aucun dictionnaire pour le correcteur orthographique Aspell n$\'a pu �tre install�.$\r$\n\
				     Les dictionnaires peuvent �tre t�l�charg�s depuis$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Voulez-vous t�l�charger les dictionnaires maintenant ?'
LangString AspellPartStart "${LYX_LANG}" "Ont �t� install�s avec succ�s "
LangString AspellPart1 "${LYX_LANG}" "un dictionnaire anglophone"
LangString AspellPart2 "${LYX_LANG}" "un dictionnaire pour la langue $LangName"
LangString AspellPart3 "${LYX_LANG}" "un$\r$\n\
				      dictionnaire pour la langue $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' pour le correcteur orthographique "Aspell".$\r$\n\
				      D$\'autres dictionnaires peuvent �tre t�l�charg�s depuis$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Voulez-vous t�l�charger d$\'autres dictionnaires maintenant ?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Pour que chaque utilisateur soit capable de personnaliser MiKTeX pour ses besoins$\r$\n\
					 il est n�cessaire de positionner les permissions en �criture dans le r�pertoire d$\'installation de MiKTeX pour tous les utilisateurs$\r$\n\
					 ($MiKTeXPath) $\r$\n\
					 et pour ses sous-r�pertoires."
LangString MiKTeXInfo "${LYX_LANG}" 'La distribution LaTeX "MiKTeX" sera utilis�e par LyX.$\r$\n\
				     Il est recommand� d$\'installer les mises � jour en utilisant l$\'application "MiKTeX Update Wizard"$\r$\n\
				     avant d$\'utiliser LyX pour la premi�re fois.$\r$\n\
				     Voulez-vous v�rifier maintenant les mises � jour de MiKTeX ?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "�chec de l'allocation 'path_prefix' lors de la configuration."
LangString CreateCmdFilesFailed "${LYX_LANG}" "Cr�ation de lyx.bat impossible."
LangString RunConfigureFailed "${LYX_LANG}" "�chec de la tentative de configuration initiale de LyX."
LangString NotAdmin "${LYX_LANG}" "Vous devez avoir les droits d'administration pour installer LyX !"
LangString InstallRunning "${LYX_LANG}" "Le programme d'installation est toujours en cours !"
LangString StillInstalled "${LYX_LANG}" "LyX est d�j� install� ! Le d�sinstaller d'abord."

LangString FinishPageMessage "${LYX_LANG}" "F�licitations ! LyX est install� avec succ�s.\r\n\
					    \r\n\
					    (Le premier d�marrage de LyX peut demander quelques secondes.)"
LangString FinishPageRun "${LYX_LANG}" "D�marrer LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "LyX introuvable dans le base des registres.$\r$\n\
					       Les raccourcis sur le bureau et dans le menu de d�marrage ne seront pas supprim�s."
LangString UnInstallRunning "${LYX_LANG}" "Vous devez fermer LyX d'abord !"
LangString UnNotAdminLabel "${LYX_LANG}" "Vous devez avoir les droits d'administration pour d�sinstaller LyX !"
LangString UnReallyRemoveLabel "${LYX_LANG}" "�tes vous s�r(e) de vouloir supprimer compl�tement LyX et tous ses composants ?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Pr�f�rences utilisateurs de LyX'
LangString UnGSview "${LYX_LANG}" 'Merci d$\'appuyer sur le bouton "D�sinstaller" de la prochaine fen�tre pour d�sinstaller.$\r$\n\
				   l$\'afficheur Postscript/PDF "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX supprim� de votre ordinateur avec succ�s."

LangString SecUnAspellDescription "${LYX_LANG}" "D�sinstalle le correcteur orthographique Aspell et tous ses dictionnaires."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "D�sinstalle la distribution LaTeX MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Uninstalls the bibliography manager JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Supprime le r�pertoire de configuration de LyX$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   pour tous les utilisateurs.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "D�sinstaller LyX et tous ses composants."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_FRENCH_NSH_
