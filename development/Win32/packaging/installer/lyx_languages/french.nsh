
!ifndef _LYX_LANGUAGES_FRENCH_NSH_
!define _LYX_LANGUAGES_FRENCH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_FRENCH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString SecAllUsersTitle "${LYX_LANG}" "Installer pour tous les utilisateurs ?"
LangString SecFileAssocTitle "${LYX_LANG}" "Associations de fichiers"
LangString SecDesktopTitle "${LYX_LANG}" "Ic�ne du bureau"

LangString SecCoreDescription "${LYX_LANG}" "Les fichiers ${PRODUCT_NAME}"
LangString SecAllUsersDescription "${LYX_LANG}" "Installer pour tous les utilisateurs (n�cessite les droits d'administrateur), ou seulement pour l'utilisateur courant ?."
LangString SecFileAssocDescription "${LYX_LANG}" "Cr�e les associations entre l'ex�cutable et le suffixe .lyx."
LangString SecDesktopDescription "${LYX_LANG}" "Une ic�ne ${PRODUCT_NAME} sur le bureau."

LangString ModifyingConfigureFailed "${LYX_LANG}" "�chec de l'allocation 'path_prefix' lors de la configuration."
LangString CreateCmdFilesFailed "${LYX_LANG}" "�chec de la cr�ation des fichiers lyx.cmd et reLyX.cmd"
LangString RunConfigureFailed "${LYX_LANG}" "�chec de la tentative de configuration initiale de LyX."

LangString FinishPageMessage "${LYX_LANG}" "LyX devrait �tre install�. Cependant, quelques utilisateurs ont signal� que le script de configuration �chouait; en cons�quence, LyX refuse de d�marrer avec un message indiquant l'absence du fichier textclass.lst. Pour rectifier l'installation, ex�cuter 'sh configure' depuis le r�pertoire LyX/Resources/lyx."
LangString FinishPageRun "${LYX_LANG}" "D�marrer LyX"

LangString DownloadPageField2 "${LYX_LANG}" "&Ne pas installer"

LangString MinSYSHeader "${LYX_LANG}" "MinSYS"
LangString MinSYSDescription "${LYX_LANG}" "MinSYS est un environnement minimal pour g�rer des commandes (www.mingw.org/msys.shtml) dont ${PRODUCT_NAME} a besoin pour ses traitements."
LangString EnterMinSYSFolder "${LYX_LANG}" "Merci de saisir le chemin conduisant au r�pertoire o� se trouve sh.exe"
LangString InvalidMinSYSFolder "${LYX_LANG}" "sh.exe introuvable"
LangString MinSYSDownloadLabel "${LYX_LANG}" "&T�l�charger MinSYS"
LangString MinSYSFolderLabel "${LYX_LANG}" "&R�pertoire contenant sh.exe"

LangString PythonHeader "${LYX_LANG}" "Python"
LangString PythonDescription "${LYX_LANG}" "Le langage de commandes Python (www.python.org) doit �tre install� ou ${PRODUCT_NAME} ne pourra pas ex�cuter un certain nombre de commandes."
LangString EnterPythonFolder "${LYX_LANG}" "Merci de saisir le chemin conduisant au r�pertoire o� se trouve Python.exe"
LangString InvalidPythonFolder "${LYX_LANG}" "Python.exe introuvable"
LangString PythonDownloadLabel "${LYX_LANG}" "&T�l�charger Python"
LangString PythonFolderLabel "${LYX_LANG}" "&R�pertoire contenant Python.exe"

LangString MiKTeXHeader "${LYX_LANG}" "MiKTeX"
LangString MiKTeXDescription "${LYX_LANG}" "MiKTeX (www.miktex.org) est une distribution moderne de TeX pour Windows."
LangString EnterMiKTeXFolder "${LYX_LANG}" "Merci de saisir le chemin conduisant au r�pertoire o� se trouve latex.exe"
LangString InvalidMiKTeXFolder "${LYX_LANG}" "latex.exe introuvable"
LangString MiKTeXDownloadLabel "${LYX_LANG}" "&T�l�charger MiKTeX"
LangString MiKTeXFolderLabel "${LYX_LANG}" "&R�pertoire contenant latex.exe"


LangString ImageMagickHeader "${LYX_LANG}" "ImageMagick"
LangString ImageMagickDescription "${LYX_LANG}" "Les outils graphiques de ImageMagick (www.imagemagick.org/script/index.php) peuvent �tre utilis�s pour convertir les fichiers graphiques dans n'importe quel format n�cessaire."
LangString EnterImageMagickFolder "${LYX_LANG}" "Merci de saisir le chemin conduisant au r�pertoire o� se trouve convert.exe"
LangString InvalidImageMagickFolder "${LYX_LANG}" "convert.exe introuvable"
LangString ImageMagickDownloadLabel "${LYX_LANG}" "&T�l�charger ImageMagick"
LangString ImageMagickFolderLabel "${LYX_LANG}" "&R�pertoire contenant convert.exe"

LangString GhostscriptHeader "${LYX_LANG}" "Ghostscript"
LangString GhostscriptDescription "${LYX_LANG}" "Ghostscript (http://www.cs.wisc.edu/~ghost/) est utilis� pour convertir les graphiques depuis/vers PostScript."
LangString EnterGhostscriptFolder "${LYX_LANG}" "Merci de saisir le chemin conduisant au r�pertoire o� se trouve gswin32c.exe"
LangString InvalidGhostscriptFolder "${LYX_LANG}" "gswin32c.exe introuvable"
LangString GhostscriptDownloadLabel "${LYX_LANG}" "&T�l�charger Ghostscript"
LangString GhostscriptFolderLabel "${LYX_LANG}" "&R�pertoire contenant gswin32c.exe"

LangString SummaryTitle "${LYX_LANG}" "R�sum� d'installation"
LangString SummaryPleaseInstall "${LYX_LANG}" "Merci d'installer les fichiers t�l�charg�s, puis d'ex�cuter de nouveau l'installation de LyX."
LangString SummaryPathPrefix "${LYX_LANG}" "Durant l'installation, une cha�ne de caract�res 'path_prefix'  sera ajout�e � 'lyxrc.defaults', elle contiendra :"

LangString UILangageTitle "${LYX_LANG}" "Le langage de l'interface de LyX"
LangString UILangageDescription "${LYX_LANG}" "Tel qu'il est utilis� pour les menus, messages, etc."
LangString UILanguageAvailableLanguages "${LYX_LANG}" " Langages disponibles"

LangString UnNotInRegistryLabel "${LYX_LANG}" "$(^Name) introuvable dans le base des registres. $\r$\nLes raccourcis sur le bureau et dans le menu de d�marrage ne seront pas supprim�s."
LangString UnNotAdminLabel "${LYX_LANG}" "D�sol� ! Vous devez avoir les droits d'administration$\r$\npour installer$(^Name)."
LangString UnReallyRemoveLabel "${LYX_LANG}" "�tes vous s�r(e) de vouloir supprimer compl�tement $(^Name) et tous ses composants ?"
LangString UnRemoveSuccessLabel "${LYX_LANG}" "$(^Name) supprim� de votre ordinateur avec succ�s."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_FRENCH_NSH_
