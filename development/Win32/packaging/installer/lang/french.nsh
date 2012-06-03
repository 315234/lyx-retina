/*

LyX Installer Language File
Language: French
Author: Jean-Pierre Chr�tien

*/

!insertmacro LANGFILE_EXT "French"

${LangFileString} TEXT_NO_PRIVILEDGES "Vous n'avez pas les privil�ges d'administrateur.$\r$\nLes utilitaires de conversion de format graphique ne peuvent �tre install�s sans ces privil�ges."

${LangFileString} TEXT_INSTALL_CURRENTUSER "(Installation pour l'utilisateur courant)"

${LangFileString} TEXT_WELCOME "Cet assistant va vous guider tout au long de l'installation de $(^NameDA).$\r$\n$\r$\n$_CLICK"

${LangFileString} TEXT_REINSTALL_TITLE "R�installation"
${LangFileString} TEXT_REINSTALL_SUBTITLE "R�installation �ventuelle des fichiers programmes de $(^NameDA) ?"
${LangFileString} TEXT_REINSTALL_INFO "$(^NameDA) est d�j� install�. Cet assistant vous permettra de modifier l'emplacement des applications externes, ou de t�l�charger de nouveaux dictionnaires. Si vous souhaitez r�installer aussi les fichiers programmes, cochez la case ci-dessous. $(^ClickNext)"
${LangFileString} TEXT_REINSTALL_ENABLE "R�installer les fichiers programmes $(^NameDA)"

${LangFileString} TEXT_EXTERNAL_NOPRIVILEDGES "(Privil�ges d'administration requis)"

${LangFileString} TEXT_EXTERNAL_LATEX_TITLE "Syst�me typographique LaTeX"
${LangFileString} TEXT_EXTERNAL_LATEX_SUBTITLE "Un syst�me typographique est requis pour pouvoir cr�er des documents imprimables ou publiables."
${LangFileString} TEXT_EXTERNAL_LATEX_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger MikTeX ou si vous voulez utiliser une distribution LaTeX existante. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_LATEX_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer MikTeX ou si vous voulez utiliser une distribution LaTeX existante. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_LATEX_DOWNLOAD "T�l�charger MikTeX"
${LangFileString} TEXT_EXTERNAL_LATEX_INSTALL "Installer MikTeX"
${LangFileString} TEXT_EXTERNAL_LATEX_FOLDER "Utiliser une distribution LaTeX existante dans le r�pertoire suivant :"
${LangFileString} TEXT_EXTERNAL_LATEX_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_LATEX}."
${LangFileString} TEXT_EXTERNAL_LATEX_NONE "Installer sans distribution LaTeX (non recommand�)"
${LangFileString} TEXT_EXTERNAL_LATEX_NOTFOUND "${BIN_LATEX} n'existe pas dans le r�pertoire sp�cifi�."

${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_TITLE "ImageMagick"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_SUBTITLE "ImageMagick est requis pour les conversions de formats graphiques vari�s."
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger ImageMagick ou si vous en disposez d�j�. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer ImageMagick ou si vous en disposez d�j�. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_DOWNLOAD "T�l�charger ImageMagick"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_INSTALL "Installer ImageMagick"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_FOLDER "Utiliser une installation ImageMagick existante dans le r�pertoire suivant :"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_IMAGEMAGICK}."
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_NONE "Installer sans ImageMagick (non recommand�)"
${LangFileString} TEXT_EXTERNAL_IMAGEMAGICK_NOTFOUND "${BIN_IMAGEMAGICK} n'existe pas dans le r�pertoire sp�cifi�."

${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_TITLE "Ghostscript"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_SUBTITLE "Ghostscript est requis pour pr�visualiser les documents au format PostScript."
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger Ghostscript ou si vous en disposez d�j�. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer Ghostscript ou si vous en disposez d�j�. $(^ClickNext)"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_DOWNLOAD "T�l�charger Ghostscript"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_INSTALL "Installer Ghostscript"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_FOLDER "Utiliser une installation Ghostscript existante dans le r�pertoire suivant :"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_GHOSTSCRIPT}."
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_NONE "Installer sans GhostScript (non recommand�)"
${LangFileString} TEXT_EXTERNAL_GHOSTSCRIPT_NOTFOUND "${BIN_GHOSTSCRIPT} n'existe pas dans le r�pertoire sp�cifi�."

${LangFileString} TEXT_VIEWER_TITLE "Visualisation des documents"
${LangFileString} TEXT_VIEWER_SUBTITLE "Un afficheur est requis pour pr�senter les r�sultats des traitements � l'�cran."
${LangFileString} TEXT_VIEWER_INFO_DOWNLOAD "Il n'y a pas d'afficheur install� sur votre syst�me pour pr�visualiser les fichiers PDF et PostScript , formats courants de la version typographique des documents. Si vous cochez la case ci-dessous, l'afficheur PDF/PostScript Gaviez sera t�l�charg�. $(^ClickNext)"
${LangFileString} TEXT_VIEWER_INFO_INSTALL "Il n'y a pas d'afficheur install� sur votre syst�me pour pr�visualiser les fichiers PDF et PostScript , formats courants de la version typographique des documents. Si vous cochez la case ci-dessous, l'afficheur PDF/PostScript GSView sera install�. $(^ClickNext)"
${LangFileString} TEXT_VIEWER_DOWNLOAD "T�l�charger GSView"
${LangFileString} TEXT_VIEWER_INSTALL "Installer GSView"

${LangFileString} TEXT_DICT_TITLE "Dictionnaires orthographiques"
${LangFileString} TEXT_DICT_SUBTITLE "S�lectionner les langues pour lesquelles vous d�sirez un dictionnaire orthographique."
${LangFileString} TEXT_DICT_TOP "Pour v�rifier l'orthographe des documents dans un langage donn�, un dictionnaire contenant les mots admis dot �tre t�l�charg�. S�lectionner les dictionnaires � t�l�charger. $_CLICK"
${LangFileString} TEXT_DICT_LIST "S�lectionner les dictionnaires � t�l�charger :"

${LangFileString} TEXT_LANGUAGE_TITLE "Choix de la langue"
${LangFileString} TEXT_LANGUAGE_SUBTITLE "Choix de la langue de l'interface $(^NameDA)."
${LangFileString} TEXT_LANGUAGE_INFO "S�lectionner la langue � utiliser dans l'interface ${APP_NAME}. $(^ClickNext)"

${LangFileString} TEXT_DOWNLOAD_FAILED_LATEX "Le t�l�chargement de MikTeX a �chou�. Voulez-vous r�essayer ?"
${LangFileString} TEXT_DOWNLOAD_FAILED_IMAGEMAGICK "Le t�l�chargement de ImageMagick a �chou�. Voulez-vous r�essayer ?"
${LangFileString} TEXT_DOWNLOAD_FAILED_GHOSTSCRIPT "Le t�l�chargement de Ghostscript a �chou�. Voulez-vous r�essayer ?"
${LangFileString} TEXT_DOWNLOAD_FAILED_VIEWER "Le t�l�chargement de GSView a �chou�. Voulez-vous r�essayer ?"
${LangFileString} TEXT_DOWNLOAD_FAILED_DICT "Le t�l�chargement du dictionnaire orthographique $DictionaryLangName a �chou�. Voulez-vous r�essayer ?"

${LangFileString} TEXT_NOTINSTALLED_LATEX "L'installation de MikTeX est rest�e incompl�te. Voulez-vous relancer l'installation de MikTeX ?"
${LangFileString} TEXT_NOTINSTALLED_IMAGEMAGICK "L'installation de ImageMagick est rest�e incompl�te. Voulez-vous relancer l'installation de ImageMagick ?"
${LangFileString} TEXT_NOTINSTALLED_GHOSTSCRIPT "L'installation de Ghostscript est rest�e incompl�te. Voulez-vous relancer l'installation de Ghostscript ?"
${LangFileString} TEXT_NOTINSTALLED_VIEWER "Vous n'avez pas associ� GSView aux types de fichier PDF/PostScript. Voulez-vous relancer l'installation de GSView ?"
${LangFileString} TEXT_NOTINSTALLED_DICT "L'installation du dictionnaire orthographique $DictionaryLangName est rest�e incompl�te. Voulez-vous relancer l'installation ?"

${LangFileString} TEXT_CONFIGURE_MIKTEXFNDB "Mise � jour de la base de donn�es MiKTeX..."

${LangFileString} TEXT_CONFIGURE_LYX "Configuration de LyX (MiKTeX peut t�l�charger les paquetages manquants, ceci peut prendre du temps) ..."

${LangFileString} TEXT_FINISH_DESKTOP "Cr�er un raccourci sur le bureau"
${LangFileString} TEXT_FINISH_WEBSITE "Consulter les derni�res nouvelles, trucs et astuces sur le site lyx.org"

${LangFileString} WelcomePageText "Cet assistant vous guidera durant l'installation de LyX.$\r$\n\
					  $\r$\n\
					  Vous devez �tre administrateur pour installer LyX.$\r$\n\
					  $\r$\n\
					  Avant de d�buter l'installation, il est recommand� de fermer toutes les autres applications. Cela permettra de mettre � jour certains fichiers syst�me sans red�marrer votre ordinateur.$\r$\n\
					  $\r$\n\
					  $_CLICK"

${LangFileString} FileTypeTitle "Document LyX"

${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "Installer pour tous les utilisateurs ?"
${LangFileString} SecFileAssocTitle "Associations de fichiers"
${LangFileString} SecDesktopTitle "Ic�ne du bureau"

${LangFileString} SecCoreDescription "Les fichiers LyX"
${LangFileString} SecInstJabRefDescription "Gestionnaire de r�f�rences bibliographiques et �diteur de ficjiers BibTeX."
${LangFileString} SecAllUsersDescription "Installer LyX pour tous les utilisateurs, ou seulement pour l'utilisateur courant ?."
${LangFileString} SecFileAssocDescription "Les fichiers de suffixe .lyx seront automatiquement ouverts dans LyX."
${LangFileString} SecDesktopDescription "Une ic�ne LyX sur le bureau."

${LangFileString} LangSelectHeader "S�lection de la langue pour les menus de LyX"
${LangFileString} AvailableLang " Langues disponibles"

${LangFileString} EnterLaTeXHeader1 'Distribution LaTeX'
${LangFileString} EnterLaTeXHeader2 'Choisir la distribution LaTeX que LyX devra utiliser.'
${LangFileString} EnterLaTeXFolder '�ventuellement, vous pouvez fixer ici le chemin d$\'acc�s au fichier $\"latex.exe$\" et de ce fait fixer \
					   la distribution LaTeX utilis�e par Lyx.$\r$\n\
					   Si vous n$\'utilisez pas LaTeX, LyX ne peut cr�er de documents !$\r$\n\
					   $\r$\n\
					   L$\'assistant d$\'installation a d�tect� la distribution LaTeX \
					   $\"$LaTeXName$\" sur votre syst�me. Le chemin d$\'acc�s est affich� ci-dessous.'
${LangFileString} EnterLaTeXFolderNone 'Fixez ci-dessous le chemin d$\'acc�s au fichier $\"latex.exe$\". De ce fait vous fixez la \
					       distribution LaTeX utilis�e par Lyx.$\r$\n\
					       Si vous n$\'utilisez pas LaTeX, LyX ne peut cr�er de documents !$\r$\n\
					       $\r$\n\
					       L$\'assistant d$\'installation n$\'a pas pu trouver de distribution LaTeX sur votre syst�me.'
${LangFileString} PathName 'Chemin vers le fichier $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "N'utilisez pas LaTeX"
${LangFileString} InvalidLaTeXFolder '$\"latex.exe$\" introuvable dans le chemin d$\'acc�s sp�cifi�.'

${LangFileString} LatexInfo 'Lancement de l$\'installation de la distribution LaTeX $\"MiKTeX$\".$\r$\n\
			            Pour installer l$\'application, appuyer sur le bouton $\"Next$\" de la fen�tre d$\'installation jusqu$\'� ce que celle-ci commence.$\r$\n\
				    $\r$\n\
				    !!! Prenez garde � utiliser toutes les options implicites du programme d$\'installation MikTeX !!!'
${LangFileString} LatexError1 'Distribution LaTeX introuvable !$\r$\n\
                      		      LyX ne peut �tre utilis� sans distribution LaTeX, comme par exemple $\"MiKTeX$\" !$\r$\n\
				      L$\'installation de LyX va donc �tre abandonn�e.'

${LangFileString} HunspellFailed 'Download of dictionary for language $\"$R3$\" failed.'
${LangFileString} ThesaurusFailed 'Download of thesaurus for language $\"$R3$\" failed.'

${LangFileString} JabRefInfo 'Lancement de l$\'installation de l$\'application $\"JabRef$\".$\r$\n\
				     Vous pouvez utiliser les options implicites du programme d$\'installation JabRef.'
${LangFileString} JabRefError 'L$\'application $\"JabRef$\" n$\'a pas pu �tre install�e correctement !$\r$\n\
		                      L$\'installation de LyX va continuer.$\r$\n\
				      Essayer d$\'installer JabRef de nouveau plus tard.'

${LangFileString} LatexConfigInfo "La configuration de LyX qui va suivre prendra un moment."

${LangFileString} MiKTeXPathInfo "Pour que chaque utilisateur soit capable de personnaliser MiKTeX pour ses besoins$\r$\n\
					 il est n�cessaire de positionner les permissions en �criture dans le r�pertoire d$\'installation de MiKTeX pour tous les utilisateurs$\r$\n\
					 ($MiKTeXPath) $\r$\n\
					 et pour ses sous-r�pertoires."
${LangFileString} MiKTeXInfo 'La distribution LaTeX $\"MiKTeX$\" sera utilis�e par LyX.$\r$\n\
				     Il est recommand� d$\'installer les mises � jour en utilisant l$\'application $\"MiKTeX Update Wizard$\"$\r$\n\
				     avant d$\'utiliser LyX pour la premi�re fois.$\r$\n\
				     Voulez-vous v�rifier maintenant les mises � jour de MiKTeX ?'

${LangFileString} UpdateNotAllowed "Ce paquetage ne peut mettre � jour que ${PRODUCT_VERSION_OLD}!"
${LangFileString} ModifyingConfigureFailed "�chec de l'allocation 'path_prefix' lors de la configuration."
${LangFileString} CreateCmdFilesFailed "Cr�ation de lyx.bat impossible."
${LangFileString} RunConfigureFailed "�chec de la tentative de configuration initiale de LyX."
${LangFileString} NotAdmin "Vous devez avoir les droits d'administration pour installer LyX !"
${LangFileString} InstallRunning "Le programme d'installation est toujours en cours !"
${LangFileString} StillInstalled "LyX est d�j� install� ! Le d�sinstaller d'abord."

${LangFileString} FinishPageMessage "F�licitations ! LyX est install� avec succ�s.$\r$\n\
					    $\r$\n\
					    (Le premier d�marrage de LyX peut demander quelques secondes.)"
${LangFileString} FinishPageRun "D�marrer LyX"

${LangFileString} UnNotInRegistryLabel "LyX introuvable dans le base des registres.$\r$\n\
					       Les raccourcis sur le bureau et dans le menu de d�marrage ne seront pas supprim�s."
${LangFileString} UnInstallRunning "Vous devez fermer LyX d'abord !"
${LangFileString} UnNotAdminLabel "Vous devez avoir les droits d'administration pour d�sinstaller LyX !"
${LangFileString} UnReallyRemoveLabel "�tes vous s�r(e) de vouloir supprimer compl�tement LyX et tous ses composants ?"
${LangFileString} UnLyXPreferencesTitle 'Pr�f�rences utilisateurs de LyX'

${LangFileString} SecUnMiKTeXDescription "D�sinstalle la distribution LaTeX MiKTeX."
${LangFileString} SecUnJabRefDescription "Uninstalls the bibliography manager JabRef."
${LangFileString} SecUnPreferencesDescription 'Supprime le r�pertoire de configuration de LyX$\r$\n\
					   $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\"$\r$\n\
					   pour tous les utilisateurs.'
${LangFileString} SecUnProgramFilesDescription "D�sinstaller LyX et tous ses composants."

