!ifndef _LYX_LANGUAGES_SPANISH_NSH_
!define _LYX_LANGUAGES_SPANISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_SPANISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString SecAllUsersTitle "${LYX_LANG}" "Instalar para todos los usuarios"
LangString SecFileAssocTitle "${LYX_LANG}" "Asociar ficheros"
LangString SecDesktopTitle "${LYX_LANG}" "Icono de escritorio"

LangString SecCoreDescription "${LYX_LANG}" "Los ficheros de ${PRODUCT_NAME}."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalar para todos los usuarios o s�lo para el usuario actual. (Requiere privilegios de Administrador.)"
LangString SecFileAssocDescription "${LYX_LANG}" "Asociar la extesi�n .lyx con LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Crear un icono de ${PRODUCT_NAME} en el escritorio."

LangString ModifyingConfigureFailed "${LYX_LANG}" "Error al intentar a�adir 'path_prefix' durante la ejecuci�n del programa de configuraci�n"
LangString RunConfigureFailed "${LYX_LANG}" "Error al intentar ejecutar el programa de configuraci�n"

LangString FinishPageMessage "${LYX_LANG}" "Enhorabuena! LyX ha sido instalado satisfactoriamente."
LangString FinishPageRun "${LYX_LANG}" "Ejecutar LyX"

LangString DownloadPageField2 "${LYX_LANG}" "&No instalar"

LangString MinSYSHeader "${LYX_LANG}" "MinSYS"
LangString MinSYSDescription "${LYX_LANG}" "MinSYS es un peque�o entorno unix (www.mingw.org/msys.shtml) que ${PRODUCT_NAME} necesita para ejecutar algunos ficheros de comandos."
LangString EnterMinSYSFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene sh.exe"
LangString InvalidMinSYSFolder "${LYX_LANG}" "Imposible encontrar sh.exe"
LangString MinSYSDownloadLabel "${LYX_LANG}" "&Descargar MinSYS"
LangString MinSYSFolderLabel "${LYX_LANG}" "&Directorio que contiene sh.exe"

LangString PythonHeader "${LYX_LANG}" "Python"
LangString PythonDescription "${LYX_LANG}" "EL lenguaje de programaci�n de comandos Python (www.python.org) debe estar instalado o de lo contrario ${PRODUCT_NAME} no ser� capaz de ejecutar ciertos ficheros de comandos."
LangString EnterPythonFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene Python.exe"
LangString InvalidPythonFolder "${LYX_LANG}" "Imposible encontrar Python.exe"
LangString PythonDownloadLabel "${LYX_LANG}" "&Descargar Python"
LangString PythonFolderLabel "${LYX_LANG}" "&Directorio que contiene Python.exe"

LangString MiKTeXHeader "${LYX_LANG}" "MiKTeX"
LangString MiKTeXDescription "${LYX_LANG}" "MiKTeX (www.miktex.org) es una implementaci�n del sistema TeX para Windows."
LangString EnterMiKTeXFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene latex.exe"
LangString InvalidMiKTeXFolder "${LYX_LANG}" "Imposible encontrar latex.exe"
LangString MiKTeXDownloadLabel "${LYX_LANG}" "&Descargar MiKTeX"
LangString MiKTeXFolderLabel "${LYX_LANG}" "&Directorio que contiene latex.exe"

LangString PerlHeader "${LYX_LANG}" "Perl"
LangString PerlDescription "${LYX_LANG}" "Si pretende usar reLyX para convertir documentos LaTeX en documentos LyX, necesitar� instalar Perl (www.perl.com)."
LangString EnterPerlFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene Perl.exe"
LangString InvalidPerlFolder "${LYX_LANG}" "Imposible encontrar Perl.exe"
LangString PerlDownloadLabel "${LYX_LANG}" "&Descargar Perl"
LangString PerlFolderLabel "${LYX_LANG}" "&Directorio que contiene perl.exe"

LangString ImageMagickHeader "${LYX_LANG}" "ImageMagick"
LangString ImageMagickDescription "${LYX_LANG}" "ImageMagick (www.imagemagick.org/script/index.php) permite convertir ficheros gr�ficos a cualquier formato que se necesite."
LangString EnterImageMagickFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene convert.exe"
LangString InvalidImageMagickFolder "${LYX_LANG}" "Imposible encontrar convert.exe"
LangString ImageMagickDownloadLabel "${LYX_LANG}" "&Descargar ImageMagick"
LangString ImageMagickFolderLabel "${LYX_LANG}" "&Directorio que contiene convert.exe"

LangString GhostscriptHeader "${LYX_LANG}" "Ghostscript"
LangString GhostscriptDescription "${LYX_LANG}" "Ghostscript (http://www.cs.wisc.edu/~ghost/) es usado para convertir im�genes a/en formato PostScript."
LangString EnterGhostscriptFolder "${LYX_LANG}" "Por favor, introduzca la ruta completa del directorio que contiene gswin32c.exe"
LangString InvalidGhostscriptFolder "${LYX_LANG}" "Imposible encontrar gswin32c.exe"
LangString GhostscriptDownloadLabel "${LYX_LANG}" "&Descargar Ghostscript"
LangString GhostscriptFolderLabel "${LYX_LANG}" "&Directorio que contiene gswin32c.exe"

LangString SummaryTitle "${LYX_LANG}" "Resumen de la instalaci�n"
LangString SummaryPleaseInstall "${LYX_LANG}" "Por favor, instale los programas descargados y vuelva a ejecutar el insralador de LyX una vez m�s."
LangString SummaryPathPrefix "${LYX_LANG}" "Durante la instalaci�n el siguiente 'path_prefix' se a�adir� a 'lyxrc.defaults':"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Imposible encontrar $(^Name) en el registro$\r$\nLos accesos r�pidos del escritorio y del Men� de Inicio no ser�n eliminados."
LangString UnNotAdminLabel "${LYX_LANG}" "Lo siento! Necesita privilegios de administrador$\r$\npara desinstalar $(^Name)."
LangString UnReallyRemoveLabel "${LYX_LANG}" "�Est� seguro de que desea eliminar completamente $(^Name) y todos sus componentes?"
LangString UnRemoveSuccessLabel "${LYX_LANG}" "$(^Name) fue eliminado satisfactoriamente de su ordenador."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_SPANISH_NSH_
