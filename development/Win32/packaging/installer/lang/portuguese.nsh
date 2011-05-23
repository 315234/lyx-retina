!insertmacro LANGFILE_EXT "Portuguese"

${LangFileString} WelcomePageText "Este assistente de instala��o ir� gui�-lo atrav�s da instala��o do LyX.$\r$\n\
					  $\r$\n\
					  Precisa de privil�gios de administrador para instalar o LyX.$\r$\n\
					  $\r$\n\
					  Recomenda-se fechar todas as outras aplica��es antes de iniciar o programa de configura��o. Isto torna possivel actualizar os ficheiros de sistema relevantes sem ter que reiniciar o computador.$\r$\n\
					  $\r$\n\
					  $_CLICK"

${LangFileString} FileTypeTitle "Documento LyX"

${LangFileString} SecInstGSviewTitle "GSview"
${LangFileString} SecInstJabRefTitle "JabRef"
${LangFileString} SecAllUsersTitle "Instalar para todos os utilizadores?"
${LangFileString} SecFileAssocTitle "Associa��o dos ficheiros"
${LangFileString} SecDesktopTitle "Icone do ambiente de trabalho"

${LangFileString} SecCoreDescription "Os ficheiros LyX."
${LangFileString} SecInstGSviewDescription "Programa para ver documentos em Postscript e PDF."
${LangFileString} SecInstJabRefDescription "Gestor de refer�ncias bibliogr�ficas e editor de ficheiros BibTeX."
${LangFileString} SecAllUsersDescription "Instalar o LyX para todos os utilizadores ou apenas para o presente utilizador."
${LangFileString} SecFileAssocDescription "Os ficheiros com a extens�o .lyx ir�o abrir automaticamente no LyX."
${LangFileString} SecDesktopDescription "Um icone do LyX no ambiente de trabalho."

${LangFileString} LangSelectHeader "Selec��o da linguagem dos menus no LyX"
${LangFileString} AvailableLang " L�nguas dispon�veis "

${LangFileString} EnterLaTeXHeader1 'Distribui��o de LaTeX'
${LangFileString} EnterLaTeXHeader2 'Escolher a distribui��o de LaTeX que o LyX dever� usar.'
${LangFileString} EnterLaTeXFolder 'Opcionalmente pode especificar o caminho do ficheiro $\"latex.exe$\" e posteriormente \
					   escolher a distribui��o de LaTeX que vai usar o LyX.$\r$\n\
					   Sem LaTeX o LyX n�o pode (pr�-)imprimir os documentos!$\r$\n\
					   $\r$\n\
					   O instalador detectou a distribui��o de LaTeX $\"$LaTeXName$\"\
					   no seu sistema, no caminho que se mostra abaixo.'
${LangFileString} EnterLaTeXFolderNone 'Especifique abaixo o caminho ao ficheiro $\"latex.exe$\". Posteriormente escolha \
					       a distribui��o de LaTeX que deve usar o LyX.$\r$\n\
					       Sem LaTeX o LyX n�o pode (pr�-)imprimir os documentos!$\r$\n\
					       $\r$\n\
					       O instalador n�o pudo achar nenhuma distribui��o de LaTeX no seu sistema.'
${LangFileString} PathName 'Caminho ao ficheiro $\"latex.exe$\"'
${LangFileString} DontUseLaTeX "N�o usar LaTeX"
${LangFileString} InvalidLaTeXFolder 'O ficheiro $\"latex.exe$\" n�o est� no caminho especificado.'

${LangFileString} LatexInfo 'Agora lan�ar-se-� o instalador da distribui��o de LaTeX $\"MiKTeX$\".$\r$\n\
			            Para instalar o programa carregue no bot�o $\"Pr�ximo$\" na janela de instala��o at� a instala��o come�ar.$\r$\n\
				    $\r$\n\
				    !!! Por favor use todas as op��es por defeito do instalador do MiKTeX !!!'
${LangFileString} LatexError1 'N�o foi encontrada nenhuma distribui��o de LaTeX!$\r$\n\
                      		      O LyX n�o pode ser usado sem uma distribui��o de LaTeX como o $\"MiKTeX$\"!$\r$\n\
				      A instala��o ser� por isso abortada.'

${LangFileString} GSviewInfo 'Agora lan�ar-se-� o instalador do programa $\"GSview$\".$\r$\n\
			             Para instalar carregue no bot�o $\"Setup$\" na primeira janela de instala��o$\r$\n\
				     escolha a l�ngua e depois carregue no bot�o $\"Pr�ximo$\" na janela seguinte do instalador.$\r$\n\
				     Pode usar todas as op��es por defeito no instalador do GSview.'
${LangFileString} GSviewError 'N�o se conseguiu instalar o programa $\"GSview$\"!$\r$\n\
		                      A instala��o ir� continuar na mesma.$\r$\n\
				      Tente instalar o GSview outra vez mais tarde.'

${LangFileString} JabRefInfo 'Agora lan�ar-se-� o instalador do programa $\"JabRef$\".$\r$\n\
				     Pode usar todas as op��es por defeito no instalador do JabRef.'
${LangFileString} JabRefError 'N�o se conseguiu instalar o programa $\"JabRef$\"!$\r$\n\
		                      A instala��o ir� continuar na mesma.$\r$\n\
				      Tente instalar o JabRef outra vez mais tarde.'

${LangFileString} LatexConfigInfo "A configura��o seguinte do LyX ir� demorar um bocado."

${LangFileString} MiKTeXPathInfo "Para que cada utilizador possa customizar posteriormente o MiKTeX segundo as suas$\r$\n\
					 necesidades, � preciso a pasta onde foi instalado o MiKTeX$\r$\n\
					 $MiKTeXPath $\r$\n\
					 e as suas subpastas terem permiss�o de escritura para todos os utilizadores."
${LangFileString} MiKTeXInfo 'Junto com o LyX vai-se utilizar a distribui��o de LaTeX $\"MiKTeX$\".$\r$\n\
				     Antes de usar o LyX pela primeira vez, recomenda-se instalar as actualiza��es dispon�veis$\r$\n\
				     do MiKTeX com o instalador $\"MiKTeX Update Wizard$\"$\r$\n\
				     Deseja comprovar agora se h� actualiza��es do MiKTeX?'

${LangFileString} UpdateNotAllowed "This update package can only update ${PRODUCT_VERSION_OLD}!"
${LangFileString} ModifyingConfigureFailed "N�o conseguimos establecer o 'path_prefix' no script de configura��o"
${LangFileString} CreateCmdFilesFailed "N�o foi poss�vel criar o lyx.bat"
${LangFileString} RunConfigureFailed "N�o foi poss�vel executar o script de configura��o"
${LangFileString} NotAdmin "Precisa de privil�gios de administrador para instalar o LyX!"
${LangFileString} InstallRunning "O instalador j� est� a correr!"
${LangFileString} StillInstalled "O LyX j� est� instalado! Desinstale o LyX primeiro."

${LangFileString} FinishPageMessage "Parab�ns! O LyX foi instalado com sucesso.$\r$\n\
					    $\r$\n\
					    (O primeiro in�cio do LyX pode levar alguns segundos.)"
${LangFileString} FinishPageRun "Lan�ar o LyX"

${LangFileString} UnNotInRegistryLabel "Incapaz de encontrar o LyX no registry.$\r$\n\
					       Os atalhos para o ambiente de trabalho no menu Start n�o ser�o removidos."
${LangFileString} UnInstallRunning "Deve fechar o LyX em primeiro lugar!"
${LangFileString} UnNotAdminLabel "Precisa de privil�gios de administrador para desinstalar o LyX!"
${LangFileString} UnReallyRemoveLabel "Tem a certeza que quer remover completamente o LyX e todas as suas componentes?"
${LangFileString} UnLyXPreferencesTitle 'Prefer�ncias de utilizador do LyX'
${LangFileString} UnGSview 'Por favor carregue no bot�o $\"Desinstalar$\" na pr�xima janela para desinstalar$\r$\n\
				   o visualisador de Postscript e PDF $\"GSview$\".'
${LangFileString} UnRemoveSuccessLabel "LyX foi removido com sucesso do seu computador."

${LangFileString} SecUnMiKTeXDescription "Desinstala a distribui��o de LaTeX MiKTeX."
${LangFileString} SecUnJabRefDescription "Desinstala o gestor de refer�ncias bibliogr�ficas JabRef."
${LangFileString} SecUnPreferencesDescription 'Apaga as pastas de configura��o do  LyX$\r$\n\
					   $\"$AppPre\username\$AppSuff\${APP_DIR_USERDATA}$\"$\r$\n\
					   de todos os utilizadores.'
${LangFileString} SecUnProgramFilesDescription "Desinstala LyX e todas as suas componentes."

