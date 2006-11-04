/*

LyX Installer Language File
Language: French
Author: Jean-Pierre Chr�tien

*/

!insertmacro LanguageString TEXT_NO_PRIVILEDGES "Vous n'avez pas les privil�ges d'administrateur.$\r$\n$\r$\nBien que vous puissiez installer ${APP_NAME} et le syst�me typographique, les utilitaires de conversion de format graphique et de pr�visualisation ne peuvent �tre install�s sans ces privil�ges."

!insertmacro LanguageString TEXT_WELCOME_DOWNLOAD "Cet assistant va vous guider tout au long de l'installation de $(^NameDA), le constructeur de documents qui encourage une saisie fond�e sur la structure du document plut�t que sur son apparence.\r\n\r\nCet assistant d'installation va param�trer un environnement complet d'utilisation de ${APP_NAME} et peut t�l�charger automatiquement les composants manquants.\r\n\r\n$(^ClickNext)"
!insertmacro LanguageString TEXT_WELCOME_INSTALL "Cet assistant va vous guider tout au long de l'installation de $(^NameDA), le constructeur de documents qui encourage une saisie fond�e sur la structure du document plut�t que sur son apparence.\r\n\r\nCet assistant d'installation va param�trer un environnement complet d'utilisation de ${APP_NAME} et peut installer automatiquement les composants manquants.\r\n\r\n$(^ClickNext)"

!insertmacro LanguageString TEXT_USER_TITLE "Choix des utilisateurs"
!insertmacro LanguageString TEXT_USER_SUBTITLE "S�lection des utilisateurs d�sirant utiliser $(^NameDA)."
!insertmacro LanguageString TEXT_USER_INFO "Choix entre installer $(^NameDA) seulement pour vous-m�me  ou bien pour tous les utilisateurs du syst�me. $(^ClickNext)"
!insertmacro LanguageString TEXT_USER_CURRENT "Installer seulement pour moi."
!insertmacro LanguageString TEXT_USER_ALL "Installer pour tous les utilisateurs."

!insertmacro LanguageString TEXT_REINSTALL_TITLE "R�installation"
!insertmacro LanguageString TEXT_REINSTALL_SUBTITLE "R�installation �ventuelle des fichiers programmes de $(^NameDA)."
!insertmacro LanguageString TEXT_REINSTALL_INFO "$(^NameDA) est d�j� install�. Cet assistant vous permettra de modifier le langage de l'interface ou l'emplacement des applications externes, ou de t�l�charger de nouveaux dictionnaires. Si vous souhaitez r�installer aussi les fichiers programmes, cochez la case ci-dessous. $(^ClickNext)"
!insertmacro LanguageString TEXT_REINSTALL_ENABLE "R�installer les fichiers programmes $(^NameDA)"

!insertmacro LanguageString TEXT_EXTERNAL_NOPRIVILEDGES "(Privil�ges d'administration requis)"

!insertmacro LanguageString TEXT_EXTERNAL_LATEX_TITLE "Syst�me typographique LaTeX"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_SUBTITLE "Un syst�me typographique est requis pour pouvoir cr�er des documents imprimables ou publiables."
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger MikTeX ou si vous voulez utiliser une distribution LaTeX existante. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer MikTeX ou si vous voulez utiliser une distribution LaTeX existante. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_DOWNLOAD "T�l�charger MikTeX"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_INSTALL "Installer MikTeX"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_FOLDER "Utiliser une distribution LaTeX existante dans le r�pertoire suivant :"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_LATEX}."
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_NONE "Installer sans distribution LaTeX (non recommand�)"
!insertmacro LanguageString TEXT_EXTERNAL_LATEX_NOTFOUND "${BIN_LATEX} n'existe pas dans le r�pertoire sp�cifi�."

!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_TITLE "ImageMagick"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_SUBTITLE "ImageMagick est requis pour les conversions de formats graphiques vari�s."
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger ImageMagick ou si vous en disposez d�j�. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer ImageMagick ou si vous en disposez d�j�. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_DOWNLOAD "T�l�charger ImageMagick"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_INSTALL "Installer ImageMagick"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_FOLDER "Utiliser une installation ImageMagick existante dans le r�pertoire suivant :"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_IMAGEMAGICK}."
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_NONE "Installer sans ImageMagick (non recommand�)"
!insertmacro LanguageString TEXT_EXTERNAL_IMAGEMAGICK_NOTFOUND "${BIN_IMAGEMAGICK} n'existe pas dans le r�pertoire sp�cifi�."

!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_TITLE "Ghostscript"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_SUBTITLE "Ghostscript est requis pour pr�visualiser les documents au format PostScript."
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_INFO_DOWNLOAD "Merci de pr�ciser si vous d�sirez t�l�charger Ghostscript ou si vous en disposez d�j�. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_INFO_INSTALL "Merci de pr�ciser si vous d�sirez installer Ghostscript ou si vous en disposez d�j�. $(^ClickNext)"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_DOWNLOAD "T�l�charger Ghostscript"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_INSTALL "Installer Ghostscript"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_FOLDER "Utiliser une installation Ghostscript existante dans le r�pertoire suivant :"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_FOLDER_INFO "Ce r�pertoire doit contenir ${BIN_GHOSTSCRIPT}."
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_NONE "Installer sans GhostScript (non recommand�)"
!insertmacro LanguageString TEXT_EXTERNAL_GHOSTSCRIPT_NOTFOUND "${BIN_GHOSTSCRIPT} n'existe pas dans le r�pertoire sp�cifi�."

!insertmacro LanguageString TEXT_VIEWER_TITLE "Visualisation des documents"
!insertmacro LanguageString TEXT_VIEWER_SUBTITLE "Un afficheur est requis pour pr�senter les r�sultats des traitements � l'�cran."
!insertmacro LanguageString TEXT_VIEWER_INFO_DOWNLOAD "Il n'y a pas d'afficheur install� sur votre syst�me pour pr�visualiser les fichiers PDF et PostScript , formats courants de la version typographique des documents. Si vous cochez la case ci-dessous, l'afficheur PDF/PostScript Gaviez sera t�l�charg�. $_CLICK"
!insertmacro LanguageString TEXT_VIEWER_INFO_INSTALL "Il n'y a pas d'afficheur install� sur votre syst�me pour pr�visualiser les fichiers PDF et PostScript , formats courants de la version typographique des documents. Si vous cochez la case ci-dessous, l'afficheur PDF/PostScript GSView sera install�. $_CLICK"
!insertmacro LanguageString TEXT_VIEWER_DOWNLOAD "T�l�charger GSView"
!insertmacro LanguageString TEXT_VIEWER_INSTALL "Installer GSView"

!insertmacro LanguageString TEXT_DICT_TITLE "Dictionnaires orthographiques"
!insertmacro LanguageString TEXT_DICT_SUBTITLE "S�lection des langues pour lesquelles vous d�sirez un dictionnaire orthographique."
!insertmacro LanguageString TEXT_DICT_TOP "Pour v�rifier l'orthographe des documents dans un langage donn�, un dictionnaire contenant les mots admis dot �tre t�l�charg�. S�lectionner les dictionnaires � t�l�charger. $_CLICK"
!insertmacro LanguageString TEXT_DICT_LIST "S�lectionner les dictionnaires � t�l�charger :"

!insertmacro LanguageString TEXT_LANGUAGE_TITLE "Choix de la langue"
!insertmacro LanguageString TEXT_LANGUAGE_SUBTITLE "Choix de la langue de l'interface $(^NameDA)."
!insertmacro LanguageString TEXT_LANGUAGE_INFO "S�lectionner la langue � utiliser dans l'interface ${APP_NAME}. $(^ClickNext)"

!insertmacro LanguageString TEXT_DOWNLOAD_FAILED_LATEX "Le t�l�chargement de MikTeX a �chou�. Voulez-vous r�essayer ?"
!insertmacro LanguageString TEXT_DOWNLOAD_FAILED_IMAGEMAGICK "Le t�l�chargement de ImageMagick a �chou�. Voulez-vous r�essayer ?"
!insertmacro LanguageString TEXT_DOWNLOAD_FAILED_GHOSTSCRIPT "Le t�l�chargement de Ghostscript a �chou�. Voulez-vous r�essayer ?"
!insertmacro LanguageString TEXT_DOWNLOAD_FAILED_VIEWER "Le t�l�chargement de GSView a �chou�. Voulez-vous r�essayer ?"
!insertmacro LanguageString TEXT_DOWNLOAD_FAILED_DICT "Le t�l�chargement du dictionnaire orthographique ${DICT_NAME} a �chou�. Voulez-vous r�essayer ?"

!insertmacro LanguageString TEXT_NOTINSTALLED_LATEX "L'installation de MikTeX est rest�e incompl�te. Voulez-vous relancer l'installation de MikTeX ?"
!insertmacro LanguageString TEXT_NOTINSTALLED_IMAGEMAGICK "L'installation de ImageMagick est rest�e incompl�te. Voulez-vous relancer l'installation de ImageMagick ?"
!insertmacro LanguageString TEXT_NOTINSTALLED_GHOSTSCRIPT "L'installation de Ghostscript est rest�e incompl�te. Voulez-vous relancer l'installation de Ghostscript ?"
!insertmacro LanguageString TEXT_NOTINSTALLED_VIEWER "Vous n'avez pas associ� GSView aux types de fichier PDF/PostScript. Voulez-vous relancer l'installation de GSView ?"
!insertmacro LanguageString TEXT_NOTINSTALLED_DICT "L'installation du dictionnaire orthographique ${DICT_NAME} est rest�e incompl�te. Voulez-vous relancer l'installation ?"

!insertmacro LanguageString TEXT_FINISH_DESKTOP "Cr�er un raccourci sur le bureau"
!insertmacro LanguageString TEXT_FINISH_WEBSITE "Consulter les derni�res nouvelles, trucs et astuces sur le site lyx.org"

!insertmacro LanguageString UNTEXT_WELCOME "Cet assistant va vous guider tout au long de la d�sinstallation de $(^NameDA). Merci de quitter $(^NameDA) avant de poursuivre.\r\n\r\nNotez bien que cet assistant ne d�sinstallera que LyX proprement dit. Si vous d�sirez d�sinstaller d'autres applications que vous avez install�es pour les besoins de $(^NameDA), effectuez cette d�sinstallation via la page � Ajout/Suppression de programmes �.\r\n\r\n$_CLICK"
