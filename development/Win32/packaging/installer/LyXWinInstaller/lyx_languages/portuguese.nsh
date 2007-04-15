!ifndef _LYX_LANGUAGES_PORTUGUESE_NSH_
!define _LYX_LANGUAGES_PORTUGUESE_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_PORTUGUESE}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Este assistente de instala��o ir� gui�-lo atrav�s da instala��o do LyX.\r\n\
					  \r\n\
					  Precisa de privil�gios de administrador para instalar o LyX.\r\n\
					  \r\n\
					  Recomenda-se fechar todas as outras aplica��es antes de iniciar o programa de configura��o. Isto torna possivel actualizar os ficheiros de sistema relevantes sem ter que reiniciar o computador.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Documento LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Instalar para todos os utilizadores?"
LangString SecFileAssocTitle "${LYX_LANG}" "Associa��o dos ficheiros"
LangString SecDesktopTitle "${LYX_LANG}" "Icone do ambiente de trabalho"

LangString SecCoreDescription "${LYX_LANG}" "Os ficheiros LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Programa para ver documentos em Postscript e PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Gestor de refer�ncias bibliogr�ficas e editor de ficheiros BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "Instalar o LyX para todos os utilizadores ou apenas para o presente utilizador."
LangString SecFileAssocDescription "${LYX_LANG}" "Os ficheiros com a extens�o .lyx ir�o abrir automaticamente no LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Um icone do LyX no ambiente de trabalho."

LangString LangSelectHeader "${LYX_LANG}" "Selec��o da linguagem dos menus no LyX"
LangString AvailableLang "${LYX_LANG}" " L�nguas dispon�veis "

LangString MissProgHeader "${LYX_LANG}" "Verifica��o para os programas exigidos"
LangString MissProgCap "${LYX_LANG}" "O(s) seguinte(s) programa(s) exigidos ser�o instalados adicionalmente"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, uma distribui��o de LaTeX"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, um interpretador para PostScript e PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, um conversor de imagens"
LangString MissProgAspell "${LYX_LANG}" "Aspell, um corrector ortogr�fico"
LangString MissProgMessage "${LYX_LANG}" 'N�o ser� instalado nenhum programa adicional.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribui��o de LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Escolher a distribui��o de LaTeX que o LyX dever� usar.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Opcionalmente pode especificar o caminho do ficheiro "latex.exe" e posteriormente \
					   escolher a distribui��o de LaTeX que vai usar o LyX.\r\n\
					   Sem LaTeX o LyX n�o pode (pr�-)imprimir os documentos!\r\n\
					   \r\n\
					   O instalador detectou a distribui��o de LaTeX "$LaTeXName"\
					   no seu sistema, no caminho que se mostra abaixo.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Especifique abaixo o caminho ao ficheiro "latex.exe". Posteriormente escolha \
					       a distribui��o de LaTeX que deve usar o LyX.\r\n\
					       Sem LaTeX o LyX n�o pode (pr�-)imprimir os documentos!\r\n\
					       \r\n\
					       O instalador n�o pudo achar nenhuma distribui��o de LaTeX no seu sistema.'
LangString PathName "${LYX_LANG}" 'Caminho ao ficheiro "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "N�o usar LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'O ficheiro "latex.exe" n�o est� no caminho especificado.'

LangString LatexInfo "${LYX_LANG}" 'Agora lan�ar-se-� o instalador da distribui��o de LaTeX "MiKTeX".$\r$\n\
			            Para instalar o programa carregue no bot�o "Pr�ximo" na janela de instala��o at� a instala��o come�ar.$\r$\n\
				    $\r$\n\
				    !!! Por favor use todas as op��es por defeito do instalador do MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" 'N�o foi encontrada nenhuma distribui��o de LaTeX!$\r$\n\
                      		      O LyX n�o pode ser usado sem uma distribui��o de LaTeX como o "MiKTeX"!$\r$\n\
				      A instala��o ser� por isso abortada.'

LangString GSviewInfo "${LYX_LANG}" 'Agora lan�ar-se-� o instalador do programa "GSview".$\r$\n\
			             Para instalar carregue no bot�o "Setup" na primeira janela de instala��o$\r$\n\
				     escolha a l�ngua e depois carregue no bot�o "Pr�ximo" na janela seguinte do instalador.$\r$\n\
				     Pode usar todas as op��es por defeito no instalador do GSview.'
LangString GSviewError "${LYX_LANG}" 'N�o se conseguiu instalar o programa "GSview"!$\r$\n\
		                      A instala��o ir� continuar na mesma.$\r$\n\
				      Tente instalar o GSview outra vez mais tarde.'

LangString JabRefInfo "${LYX_LANG}" 'Agora lan�ar-se-� o instalador do programa "JabRef".$\r$\n\
				     Pode usar todas as op��es por defeito no instalador do JabRef.'
LangString JabRefError "${LYX_LANG}" 'N�o se conseguiu instalar o programa "JabRef"!$\r$\n\
		                      A instala��o ir� continuar na mesma.$\r$\n\
				      Tente instalar o JabRef outra vez mais tarde.'

LangString LatexConfigInfo "${LYX_LANG}" "A configura��o seguinte do LyX ir� demorar um bocado."

LangString AspellInfo "${LYX_LANG}" 'Agora se v�o descarregar e instalar dicion�rios do corrector ortogr�fico "Aspell".$\r$\n\
				     Cada dicion�rio tem uma licen�a diferente, que se mostrar� antes da instala��o.'
LangString AspellDownloadFailed "${LYX_LANG}" "N�o foi poss�vel descarregar nenhum dicion�rio do Aspell!"
LangString AspellInstallFailed "${LYX_LANG}" "N�o foi poss�vel instalar nenhum dicion�rio do Aspell!"
LangString AspellPartAnd "${LYX_LANG}" " e "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'J� h� instalado um dicion�rio desta l�ngua'
LangString AspellNone "${LYX_LANG}" 'N�o se instalou nenhum dicion�rio do "Aspell".$\r$\n\
				     Os dicion�rios podem descarregar-se de$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Deseja descarregar dicion�rios agora?'
LangString AspellPartStart "${LYX_LANG}" "Instalou-se com sucesso "
LangString AspellPart1 "${LYX_LANG}" "um dicion�rio de ingl�s"
LangString AspellPart2 "${LYX_LANG}" "um dicion�rio de $LangName"
LangString AspellPart3 "${LYX_LANG}" "um$\r$\n\
				      dicion�rio de $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' para o corrector ortogr�fico "Aspell".$\r$\n\
				      � pos�vel descarregar mais dicion�rios de$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Deseja descarregar outros dicion�rios agora?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Para que cada utilizador possa customizar posteriormente o MiKTeX segundo as suas$\r$\n\
					 necesidades, � preciso a pasta onde foi instalado o MiKTeX$\r$\n\
					 $MiKTeXPath $\r$\n\
					 e as suas subpastas terem permiss�o de escritura para todos os utilizadores."
LangString MiKTeXInfo "${LYX_LANG}" 'Junto com o LyX vai-se utilizar a distribui��o de LaTeX "MiKTeX".$\r$\n\
				     Antes de usar o LyX pela primeira vez, recomenda-se instalar as actualiza��es dispon�veis$\r$\n\
				     do MiKTeX com o instalador "MiKTeX Update Wizard"$\r$\n\
				     Deseja comprovar agora se h� actualiza��es do MiKTeX?'

LangString UpdateNotAllowed "${LYX_LANG}" "This update package can only update ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "N�o conseguimos establecer o 'path_prefix' no script de configura��o"
LangString CreateCmdFilesFailed "${LYX_LANG}" "N�o foi poss�vel criar o lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "N�o foi poss�vel executar o script de configura��o"
LangString NotAdmin "${LYX_LANG}" "Precisa de privil�gios de administrador para instalar o LyX!"
LangString InstallRunning "${LYX_LANG}" "O instalador j� est� a correr!"
LangString StillInstalled "${LYX_LANG}" "O LyX j� est� instalado! Desinstale o LyX primeiro."

LangString FinishPageMessage "${LYX_LANG}" "Parab�ns! O LyX foi instalado com sucesso.\r\n\
					    \r\n\
					    (O primeiro in�cio do LyX pode levar alguns segundos.)"
LangString FinishPageRun "${LYX_LANG}" "Lan�ar o LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Incapaz de encontrar o LyX no registry.$\r$\n\
					       Os atalhos para o ambiente de trabalho no menu Start n�o ser�o removidos."
LangString UnInstallRunning "${LYX_LANG}" "Deve fechar o LyX em primeiro lugar!"
LangString UnNotAdminLabel "${LYX_LANG}" "Precisa de privil�gios de administrador para desinstalar o LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Tem a certeza que quer remover completamente o LyX e todas as suas componentes?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Prefer�ncias de utilizador do LyX'
LangString UnGSview "${LYX_LANG}" 'Por favor carregue no bot�o "Desinstalar" na pr�xima janela para desinstalar$\r$\n\
				   o visualisador de Postscript e PDF "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX foi removido com sucesso do seu computador."

LangString SecUnAspellDescription "${LYX_LANG}" "Desinstala o corrector ortogr�fico Aspell e todos os seus dicion�rios."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Desinstala a distribui��o de LaTeX MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Desinstala o gestor de refer�ncias bibliogr�ficas JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Apaga as pastas de configura��o do  LyX$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   de todos os utilizadores.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Desinstala LyX e todas as suas componentes."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_PORTUGUESE_NSH_
