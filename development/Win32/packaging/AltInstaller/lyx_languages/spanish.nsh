!ifndef _LYX_LANGUAGES_SPANISH_NSH_
!define _LYX_LANGUAGES_SPANISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_SPANISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Este programa instalar� LyX en su ordenador.\r\n\
					  \r\n\
					  Usted necesita privilegios de administrador para instalar LyX.\r\n\
					  \r\n\
					  Se recomienda que cierre todas las dem�s aplicaciones antes de iniciar la instalaci�n. Esto har� posible actualizar archivos relacionados con el sistema sin tener que reiniciar su ordenador.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Documento LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Instalar para todos los usuarios"
LangString SecFileAssocTitle "${LYX_LANG}" "Asociar ficheros"
LangString SecDesktopTitle "${LYX_LANG}" "Icono de escritorio"

LangString SecCoreDescription "${LYX_LANG}" "Los ficheros de LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Programa para ver documentos Postscript y PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Administrador de referencias bibliograficas e editor de ficheros BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalar LyX para todos los usuarios o s�lo para el usuario actual."
LangString SecFileAssocDescription "${LYX_LANG}" "Asociar la extensi�n .lyx con LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Crear un icono de LyX en el escritorio."

LangString LangSelectHeader "${LYX_LANG}" "Selecci�n del idioma del menu de LyX "
LangString AvailableLang "${LYX_LANG}" " Idiomas disponibles "

LangString MissProgHeader "${LYX_LANG}" "Verificaci�n de programas necesarios "
LangString MissProgCap "${LYX_LANG}" "Los siguientes programas necesarios ser�n tambi�n instalados "
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, una distribuci�n de LaTeX "
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, un interprete para PostScript y PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, un conversor de im�genes "
LangString MissProgAspell "${LYX_LANG}" "Aspell, un corrector ortogr�fico"
LangString MissProgMessage "${LYX_LANG}" 'No es necesario instalar programas adicionales.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribuci�n LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Elija la distribuci�n de LaTeX que deber�a emplear LyX.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Opcionalmente puede especificar el camino del fichero "latex.exe" y posteriormente \
					   elegir la distribuci�n de LaTeX que va usar LyX.\r\n\
					   �Sin LaTeX LyX no puede (pre)imprimir documentos!\r\n\
					   \r\n\
					   El instalador ha detectado la distribuci�n de LaTeX "$LaTeXName" \
					   en su sistema, en el camino que se muestra abajo.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Especifique abajo el camino al fichero "latex.exe". Posteriormente elija \
					       la distribuci�n de LaTeX que debe usar LyX.\r\n\
					       �Sin LaTeX LyX no puede (pre)imprimir documentos!\r\n\
					       \r\n\
					       El instalador non pudo encontrar ninguna distribuci�n LaTeX en su sistema.'
LangString PathName "${LYX_LANG}" 'Camino al fichero "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "No usar LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Imposible encontrar "latex.exe".'

LangString LatexInfo "${LYX_LANG}" 'Ahora se lanzar� el instalador de "MiKTeX", la distribuci�n de LaTeX.$\r$\n\
			            Para instalar el programa presione el bot�n "Next" en la  ventana del instalador hasta que la aplicaci�n arranque.$\r$\n\
				    $\r$\n\
				    ��� Por favor use todas las opciones por defecto del instalador de MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" '�No se ha encontrado ninguna distribuci�n de LaTeX!$\r$\n\
                      		      �LyX no funciona sin una distribuci�n de LaTeX como "MiKTeX"!$\r$\n\
				      Por lo tanto la instalaci�n se abortar�.'

LangString GSviewInfo "${LYX_LANG}" 'Ahora se lanzar� el instalador del programa "GSview".$\r$\n\
			             Para instalar el programa presione el bot�n "Setup" en la primera ventana del instalador$\r$\n\
				     elija un idioma y luego presione el bot�n "Next" en la siguiente ventana del instalador.$\r$\n\
				     Puede utilizar todas las opciones por defecto del instalador de GSview.'
LangString GSviewError "${LYX_LANG}" '�El programa "GSview" no pudo instalarse con �xito!$\r$\n\
		                      La instalaci�n se continuar� de toda forma.$\r$\n\
				      Intente instalar otra vez GSview m�s tarde.'

LangString JabRefInfo "${LYX_LANG}" 'Ahora se lanzar� el instalador del programa "JabRef".$\r$\n\
				     Puede utilizar todas las opciones por defecto del instaldor de JabRef.'
LangString JabRefError "${LYX_LANG}" '�El programa "JabRef" no pudo instalarse con �xito!$\r$\n\
		                      La instalaci�n se continuar� de toda forma.$\r$\n\
				      Intente instalar otra vez JabRef m�s tarde.'

LangString LatexConfigInfo "${LYX_LANG}" "La siguiente configuraci�n de LyX va a tardar un poco."

LangString AspellInfo "${LYX_LANG}" 'Ahora se van a descargar e instalar diccionarios del corrector ortogr�fico "Aspell".$\r$\n\
				     Cada diccionario tiene una licencia diferente, que se mostrar� antes da instalaci�n.'
LangString AspellDownloadFailed "${LYX_LANG}" "�No se pudo descargar ning�n diccionario para Aspell!"
LangString AspellInstallFailed "${LYX_LANG}" "�No se pudo instalar ning�n diccionario para Aspell!"
LangString AspellPartAnd "${LYX_LANG}" " y "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Ya hay instalado un diccionario de esta lengua'
LangString AspellNone "${LYX_LANG}" 'No se ha instalado ning�n diccionario para "Aspell".$\r$\n\
				     Los diccionarios puede descargarse de$\r$\n\
				     ${AspellLocation}$\r$\n\
				     �Desea descargar diccionarios ahora?'
LangString AspellPartStart "${LYX_LANG}" "Se ha instalado con �xito "
LangString AspellPart1 "${LYX_LANG}" "un diccionario de ingl�s"
LangString AspellPart2 "${LYX_LANG}" "un diccionario de $LangName"
LangString AspellPart3 "${LYX_LANG}" "un$\r$\n\
				      diccionario de $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' para el corrector ortogr�fico "Aspell".$\r$\n\
				      Es posible descargar mas diccionarios de$\r$\n\
				      ${AspellLocation}$\r$\n\
				      �Desea descargar otros diccionarios ahora?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Para que cada usuario pueda personalizar posteriormente MiKTeX acorde sus$\r$\n\
					 necesidades, es necesario que la carpeta donde fue instalado MiKTeX's $\r$\n\
					 $MiKTeXPath $\r$\n\
					 y sus subcarpetas tengan permiso de escritura para todos los usuarios."
LangString MiKTeXInfo "${LYX_LANG}" 'Junto con LyX se va usar la distribuci�n de LaTeX "MiKTeX".$\r$\n\
				     Antes de usar LyX por primera vez, se recomienda instalar las actualizaciones disponibles$\r$\n\
				     de MiKTeX con el instalador "MiKTeX Update Wizard"$\r$\n\
				     �Desea comprobar ahora si hay actualizaciones de MiKTeX?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Error al intentar a�adir 'path_prefix' durante la ejecuci�n del programa de configuraci�n"
LangString CreateCmdFilesFailed "${LYX_LANG}" "No se pudo crear lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Error al intentar ejecutar el programa de configuraci�n"
LangString NotAdmin "${LYX_LANG}" "Necesita privilegios de administrador para instalar LyX!"
LangString InstallRunning "${LYX_LANG}" "El instalador ya esta siendo ejecutado!"
LangString StillInstalled "${LYX_LANG}" "�LyX ya esta instalado! Antes desinstale LyX."

LangString FinishPageMessage "${LYX_LANG}" "�Enhorabuena! LyX ha sido instalado con �xito.\r\n\
					    \r\n\
					    (El primer arranque de LyX puede tardar algunos segundos.)"
LangString FinishPageRun "${LYX_LANG}" "Ejecutar LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Imposible encontrar LyX en el registro.$\r$\n\
					       Los accesos r�pidos del escritorio y del Men� de Inicio no ser�n eliminados."
LangString UnInstallRunning "${LYX_LANG}" "Antes cierre LyX!"
LangString UnNotAdminLabel "${LYX_LANG}" "Necesita privilegios de administrador para desinstalar LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "�Est� seguro de que desea eliminar completamente LyX y todos sus componentes?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Preferencias de usuario de LyX'
LangString UnGSview "${LYX_LANG}" 'Por favor, haga clic sobre el bot�n "Uninstall" en la siguiente ventana para de-instalar$\r$\n\
				   el programa "GSview" (Postscript y PDF-viewer).'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX fue eliminado con �xito de su ordenador."

LangString SecUnAspellDescription "${LYX_LANG}" "Desinstala el corrector ortogr�fico Aspell y todos sus diccionarios."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Desinstala a distribuci�n de LaTeX MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Desinstala el administrador de referencias bibliograficas JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Elimina las carpetas de configuraci�n de LyX$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   de todos los usuarios.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Desinstala LyX y todos sus componentes."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_SPANISH_NSH_
