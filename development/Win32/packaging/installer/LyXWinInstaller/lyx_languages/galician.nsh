!ifndef _LYX_LANGUAGES_GALICIAN_NSH_
!define _LYX_LANGUAGES_GALICIAN_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_GALICIAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Este asistente vai-no guiar na instalaci�n do LyX no seu computador.\r\n\
					  \r\n\
					  Para poder instalar o LyX precisa de privil�xios de administrador.\r\n\
					  \r\n\
					  Recomenda-se fechar todas as outras aplicaci�ns antes de iniciar a instalaci�n. Isto posibilita actualizar os ficheiros do sistema relevantes sen ter que reiniciar o computador.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Documento LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Instalar para todos os usu�rios?"
LangString SecFileAssocTitle "${LYX_LANG}" "Asociaci�n dos ficheiros"
LangString SecDesktopTitle "${LYX_LANG}" "Icone do ambiente de traballo"

LangString SecCoreDescription "${LYX_LANG}" "Os ficheiros LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Programa para visualizar documentos en Postscript e PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Xestor de refer�ncias bibliogr�ficas e editor de ficheiros BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalar o LyX monousu�rio ou multiusu�rio."
LangString SecFileAssocDescription "${LYX_LANG}" "Asociar a extensi�n .lyx co LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Cria un icone do LyX no ambiente de traballo."

LangString LangSelectHeader "${LYX_LANG}" "Selecci�n da lingua dos menus no LyX"
LangString AvailableLang "${LYX_LANG}" " L�nguas dispon�beis "

LangString MissProgHeader "${LYX_LANG}" "Verificaci�n dos programas neces�rios"
LangString MissProgCap "${LYX_LANG}" "O(s) seguinte(s) programa(s) neces�rio(s) van ser instalados adicionalmente"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, unha distribuci�n de LaTeX"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, un interprete de PostScript e PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, um conversor de imaxes"
LangString MissProgAspell "${LYX_LANG}" "Aspell, un corrector ortogr�fico"
LangString MissProgMessage "${LYX_LANG}" 'Non � neces�rio instalar programas adicionais.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribuici�n de LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Escolha a distribuci�n de LaTeX que vai usar o LyX.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Opcionalmente pode especificar o cami�o do ficheiro "latex.exe" e posteriormente \
					   escoller a distribuici�n de LaTeX que vai usar o LyX.\r\n\
					   Sen LaTeX o LyX non pode (pr�-)imprimir os documentos!\r\n\
					   \r\n\
					   O instalador detectou a distribuci�n de LaTeX "$LaTeXName"\
					   no seu sistema, no cami�o que se mostra abaixo.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Especifique abaixo o cami�o ao ficheiro "latex.exe". Posteriormente escolla \
					       a distribuci�n de LaTeX que debe usar o LyX.\r\n\
					       Sem LaTeX o LyX non pode (pr�-)imprimir os documentos!\r\n\
					       \r\n\
					       O instalador non pudo achar nengunha distribuci�n de LaTeX no seu sistema.'
LangString PathName "${LYX_LANG}" 'Cami�o ao ficheiro "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "Non usar LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'O ficheiro "latex.exe" non est� no cami�o especificado.'

LangString LatexInfo "${LYX_LANG}" 'Agora lanzar�-se o instalador da distribuci�n de LaTeX "MiKTeX".$\r$\n\
			            Para instalar o programa prema no bot�n "Pr�ximo" na xanela de instalaci�n at� a instalaci�n comezar.$\r$\n\
				    $\r$\n\
				    !!! Por favor use todas as opci�ns por defeito do instalador do MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" 'Non se achou nengunha distribuci�n de LaTeX!$\r$\n\
                      		      Non se pode usar o LyX sen unha distribuci�n de LaTeX como a "MiKTeX"!$\r$\n\
				      Por iso a instalaci�n vai ser abortada.'

LangString GSviewInfo "${LYX_LANG}" 'Agora lanzar�-se o instalador do programa "GSview".$\r$\n\
			             Para instalar prema no bot�n "Setup" na primeira xanela de instalaci�n$\r$\n\
				     escolla a l�ngua e despois prema no bot�n "Pr�ximo" na seguinte xanela do instalador.$\r$\n\
				     Pode usar todas as opci�ns por defeito no instalador do GSview.'
LangString GSviewError "${LYX_LANG}" 'Non se deu instalado o programa "GSview"!$\r$\n\
		                      De todos os xeitos a instalaci�n do LyX continua.$\r$\n\
				      Tente instalar m�is adiante o GSview.'

LangString JabRefInfo "${LYX_LANG}" 'Agora lanzar�-se o instalador do programa "JabRef".$\r$\n\
				     Pode usar todas as opci�ns por defeito no instalador do JabRef.'
LangString JabRefError "${LYX_LANG}" 'Non se deu instalado o programa "JabRef"!$\r$\n\
		                      De todos os xeitos a instalaci�n do LyX continua.$\r$\n\
				      Tente instalar mais adiante o JabRef.'

LangString LatexConfigInfo "${LYX_LANG}" "A configuraci�n seguinte do LyX ir� demorar un pouco."

LangString AspellInfo "${LYX_LANG}" 'Agora se van descarregar e instalar dicion�rios do corrector ortogr�fico "Aspell".$\r$\n\
				     Cada dicion�rio ten unha licenza diferente, que se mostrar� antes da instalaci�n.'
LangString AspellDownloadFailed "${LYX_LANG}" "Non se deu descarregadao nengun dicion�rio do Aspell!"
LangString AspellInstallFailed "${LYX_LANG}" "Non se deu instalado nengun dicion�rio do Aspell!"
LangString AspellPartAnd "${LYX_LANG}" " e "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Xa ha instalado un dicion�rio desta l�ngua'
LangString AspellNone "${LYX_LANG}" 'Non se instalou nengun dicion�rio do "Aspell".$\r$\n\
				     Os dicion�rios poden descarregar-se de$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Desexa descarregar dicion�rios agora?'
LangString AspellPartStart "${LYX_LANG}" "Instalou-se con suceso "
LangString AspellPart1 "${LYX_LANG}" "un dicion�rio de ingl�s"
LangString AspellPart2 "${LYX_LANG}" "un dicion�rio de $LangName"
LangString AspellPart3 "${LYX_LANG}" "un$\r$\n\
				      dicion�rio de $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' para o corrector ortogr�fico "Aspell".$\r$\n\
				      � pos�bel descarregar m�is dicion�rios de$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Desexa descarregar outros dicion�rios agora?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Para que cada usu�rio poda personalizar posteriormente o MiKTeX segundo as suas$\r$\n\
					 necesidades, cumpre a pasta onde foi instalado o MiKTeX$\r$\n\
					 $MiKTeXPath $\r$\n\
					 e as suas subpastas teren permiso de escritura para todos os usu�rios."
LangString MiKTeXInfo "${LYX_LANG}" 'Xunto co LyX vai-se empregar a distribuci�n de LaTeX "MiKTeX".$\r$\n\
				     Antes de usar o LyX pola primeira vez, recomenda-se instalar as actualizaci�ns dispon�beis$\r$\n\
				     do MiKTeX co instalador "MiKTeX Update Wizard"$\r$\n\
				     Desexa comprovar agora se ha actualizaci�ns do MiKTeX?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update LyX ${PRODUCT_UPDATE_ALLOWED}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Non se puido engadir o 'path_prefix' no script de configuraci�n"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Non se deu criado o lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "Non se deu executado o script de configuraci�n"
LangString NotAdmin "${LYX_LANG}" "Precisa de privil�xios de administrador para instalar o LyX!"
LangString InstallRunning "${LYX_LANG}" "O instalador xa est� a correr!"
LangString StillInstalled "${LYX_LANG}" "O LyX xa est� instalado! Desinstale o LyX primeiro."

LangString FinishPageMessage "${LYX_LANG}" "Parab�ns! O LyX foi instalado con suceso.\r\n\
					    \r\n\
					    (O primeiro in�cio do LyX pode levar alguns segundos.)"
LangString FinishPageRun "${LYX_LANG}" "Lanzar o LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Non se da achado o LyX no registo.$\r$\n\
					       Non se eliminar�n os atallos para o ambiente de traballo e no menu de In�cio."
LangString UnInstallRunning "${LYX_LANG}" "Debe fechar o LyX en primeiro lugar!"
LangString UnNotAdminLabel "${LYX_LANG}" "Precisa de privil�xios de administrador para desinstalar o LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Seguro que quer eliminar completamente o LyX e todos os seus componentes?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Prefer�ncias de usu�rio do LyX'
LangString UnGSview "${LYX_LANG}" 'Por favor prema no bot�n "Desinstalar" na pr�xima xanela para desinstalar$\r$\n\
				   o visualizador de Postscript e PDF "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "Eliminou-se o LyX do seu computador."

LangString SecUnAspellDescription "${LYX_LANG}" "Desinstala o corrector ortogr�fico Aspell e todos os seus dicion�rios."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Desinstala a distribuci�n de LaTeX MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Desinstala o xestor de refer�ncias bibliogr�ficas JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Elimina as pastas de configuraci�n do  LyX$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   de todos os usu�rios.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Desinstala LyX e todos os seus componentes."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_GALICIAN_NSH_
