!insertmacro LANGFILE_EXT "Galician"

${LangFileString} WelcomePageText "Este asistente vai-no guiar na instalaci�n do LyX no seu computador.$\r$\n\
					  $\r$\n\
					  Para poder instalar o LyX precisa de privil�xios de administrador.$\r$\n\
					  $\r$\n\
					  Recomenda-se fechar todas as outras aplicaci�ns antes de iniciar a instalaci�n. Isto posibilita actualizar os ficheiros do sistema relevantes sen ter que reiniciar o computador.$\r$\n\
					  $\r$\n\
					  $_CLICK"

${LangFileString} FileTypeTitle "Documento LyX"

${LangFileString} SecInstGSviewTitle "GSview"
${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "Instalar para todos os usu�rios?"
${LangFileString} SecFileAssocTitle "Asociaci�n dos ficheiros"
${LangFileString} SecDesktopTitle "Icone do ambiente de traballo"

${LangFileString} SecCoreDescription "Os ficheiros LyX."
${LangFileString} SecInstGSviewDescription "Programa para visualizar documentos en Postscript e PDF."
${LangFileString} SecInstJabRefDescription "Xestor de refer�ncias bibliogr�ficas e editor de ficheiros BibTeX."
${LangFileString} SecAllUsersDescription "Instalar o LyX monousu�rio ou multiusu�rio."
${LangFileString} SecFileAssocDescription "Asociar a extensi�n .lyx co LyX."
${LangFileString} SecDesktopDescription "Cria un icone do LyX no ambiente de traballo."

${LangFileString} LangSelectHeader "Selecci�n da lingua dos menus no LyX"
${LangFileString} AvailableLang " L�nguas dispon�beis "

${LangFileString} EnterLaTeXHeader1 'Distribuici�n de LaTeX'
${LangFileString} EnterLaTeXHeader2 'Escolha a distribuci�n de LaTeX que vai usar o LyX.'
${LangFileString} EnterLaTeXFolder 'Opcionalmente pode especificar o cami�o do ficheiro $\"latex.exe$\" e posteriormente \
					   escoller a distribuici�n de LaTeX que vai usar o LyX.$\r$\n\
					   Sen LaTeX o LyX non pode (pr�-)imprimir os documentos!$\r$\n\
					   $\r$\n\
					   O instalador detectou a distribuci�n de LaTeX $\"$LaTeXName$\"\
					   no seu sistema, no cami�o que se mostra abaixo.'
${LangFileString} EnterLaTeXFolderNone 'Especifique abaixo o cami�o ao ficheiro $\"latex.exe$\". Posteriormente escolla \
					       a distribuci�n de LaTeX que debe usar o LyX.$\r$\n\
					       Sem LaTeX o LyX non pode (pr�-)imprimir os documentos!$\r$\n\
					       $\r$\n\
					       O instalador non pudo achar nengunha distribuci�n de LaTeX no seu sistema.'
${LangFileString} PathName 'Cami�o ao ficheiro $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "Non usar LaTeX"
${LangFileString} InvalidLaTeXFolder 'O ficheiro $\"latex.exe$\" non est� no cami�o especificado.'

${LangFileString} LatexInfo 'Agora lanzar�-se o instalador da distribuci�n de LaTeX $\"MiKTeX$\".$\r$\n\
			            Para instalar o programa prema no bot�n $\"Pr�ximo$\" na xanela de instalaci�n at� a instalaci�n comezar.$\r$\n\
				    $\r$\n\
				    !!! Por favor use todas as opci�ns por defeito do instalador do MiKTeX !!!'
${LangFileString} LatexError1 'Non se achou nengunha distribuci�n de LaTeX!$\r$\n\
                      		      Non se pode usar o LyX sen unha distribuci�n de LaTeX como a $\"MiKTeX$\"!$\r$\n\
				      Por iso a instalaci�n vai ser abortada.'

${LangFileString} GSviewInfo 'Agora lanzar�-se o instalador do programa $\"GSview$\".$\r$\n\
			             Para instalar prema no bot�n $\"Setup$\" na primeira xanela de instalaci�n$\r$\n\
				     escolla a l�ngua e despois prema no bot�n $\"Pr�ximo$\" na seguinte xanela do instalador.$\r$\n\
				     Pode usar todas as opci�ns por defeito no instalador do GSview.'
${LangFileString} GSviewError 'Non se deu instalado o programa $\"GSview$\"!$\r$\n\
		                      De todos os xeitos a instalaci�n do LyX continua.$\r$\n\
				      Tente instalar m�is adiante o GSview.'

${LangFileString} JabRefInfo 'Agora lanzar�-se o instalador do programa $\"JabRef$\".$\r$\n\
				     Pode usar todas as opci�ns por defeito no instalador do JabRef.'
${LangFileString} JabRefError 'Non se deu instalado o programa $\"JabRef$\"!$\r$\n\
		                      De todos os xeitos a instalaci�n do LyX continua.$\r$\n\
				      Tente instalar mais adiante o JabRef.'

${LangFileString} LatexConfigInfo "A configuraci�n seguinte do LyX ir� demorar un pouco."

${LangFileString} MiKTeXPathInfo "Para que cada usu�rio poda personalizar posteriormente o MiKTeX segundo as suas$\r$\n\
					 necesidades, cumpre a pasta onde foi instalado o MiKTeX$\r$\n\
					 $MiKTeXPath $\r$\n\
					 e as suas subpastas teren permiso de escritura para todos os usu�rios."
${LangFileString} MiKTeXInfo 'Xunto co LyX vai-se empregar a distribuci�n de LaTeX $\"MiKTeX$\".$\r$\n\
				     Antes de usar o LyX pola primeira vez, recomenda-se instalar as actualizaci�ns dispon�beis$\r$\n\
				     do MiKTeX co instalador $\"MiKTeX Update Wizard$\"$\r$\n\
				     Desexa comprovar agora se ha actualizaci�ns do MiKTeX?'

${LangFileString} UpdateNotAllowed "This update package can only update ${PRODUCT_VERSION_OLD}!"
${LangFileString} ModifyingConfigureFailed "Non se puido engadir o 'path_prefix' no script de configuraci�n"
${LangFileString} CreateCmdFilesFailed "Non se deu criado o lyx.bat"
${LangFileString} RunConfigureFailed "Non se deu executado o script de configuraci�n"
${LangFileString} NotAdmin "Precisa de privil�xios de administrador para instalar o LyX!"
${LangFileString} InstallRunning "O instalador xa est� a correr!"
${LangFileString} StillInstalled "O LyX xa est� instalado! Desinstale o LyX primeiro."

${LangFileString} FinishPageMessage "Parab�ns! O LyX foi instalado con suceso.$\r$\n\
					    $\r$\n\
					    (O primeiro in�cio do LyX pode levar alguns segundos.)"
${LangFileString} FinishPageRun "Lanzar o LyX"

${LangFileString} UnNotInRegistryLabel "Non se da achado o LyX no registo.$\r$\n\
					       Non se eliminar�n os atallos para o ambiente de traballo e no menu de In�cio."
${LangFileString} UnInstallRunning "Debe fechar o LyX en primeiro lugar!"
${LangFileString} UnNotAdminLabel "Precisa de privil�xios de administrador para desinstalar o LyX!"
${LangFileString} UnReallyRemoveLabel "Seguro que quer eliminar completamente o LyX e todos os seus componentes?"
${LangFileString} UnLyXPreferencesTitle 'Prefer�ncias de usu�rio do LyX'
${LangFileString} UnGSview 'Por favor prema no bot�n $\"Desinstalar$\" na pr�xima xanela para desinstalar$\r$\n\
				   o visualizador de Postscript e PDF $\"GSview$\".'
${LangFileString} UnRemoveSuccessLabel "Eliminou-se o LyX do seu computador."

${LangFileString} SecUnMiKTeXDescription "Desinstala a distribuci�n de LaTeX MiKTeX."
${LangFileString} SecUnJabRefDescription "Desinstala o xestor de refer�ncias bibliogr�ficas JabRef."
${LangFileString} SecUnPreferencesDescription 'Elimina as pastas de configuraci�n do  LyX$\r$\n\
					   $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\"$\r$\n\
					   de todos os usu�rios.'
${LangFileString} SecUnProgramFilesDescription "Desinstala LyX e todos os seus componentes."

