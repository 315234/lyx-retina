!ifndef _LYX_LANGUAGES_CATALAN_NSH_
!define _LYX_LANGUAGES_CATALAN_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_CATALAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Aquest assistent us guiar� en la instal�laci� del LyX.\r\n\
					  \r\n\
					  Necessiteu drets d'administrador per instal�lar el LyX.\r\n\
					  \r\n\
					  Abans de comen�ar la instal�laci�, �s recomenable tancar totes les altres aplicacions. Aix� permet actualitzar alguns fitxerr del sistema sense haver de reiniciar l'ordinador.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "Document LyX"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Voleu instal�lar-ho per a tots els usuaris?"
LangString SecFileAssocTitle "${LYX_LANG}" "Associaci� de fitxers"
LangString SecDesktopTitle "${LYX_LANG}" "Icona a l'escriptori"

LangString SecCoreDescription "${LYX_LANG}" "Els fitxers del LyX."
LangString SecInstGSviewDescription "${LYX_LANG}" "Programa per visualitzar documents en format Postscript i PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "Gestor i editor de refer�ncies bibliogr�fiques per a fitxers BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "Instal�la el LyX per a tots els usuaris o nom�s per a l'usuari actual."
LangString SecFileAssocDescription "${LYX_LANG}" "Els fitxers amb extensi� .lyx s'obriran autom�ticament amb el LyX."
LangString SecDesktopDescription "${LYX_LANG}" "Una icona del LyX a l'escriptori."

LangString LangSelectHeader "${LYX_LANG}" "Selecci� de la llengua de men�s del LyX"
LangString AvailableLang "${LYX_LANG}" " Lleng�es disponibles "

LangString MissProgHeader "${LYX_LANG}" "Verificaci� dels programes requerits"
LangString MissProgCap "${LYX_LANG}" "Addicionalment, s'instal�laran els programes seg�ents"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, una distribuci� de LaTeX"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, un int�rpret per a documents PostScript i PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, un conversor d'imatges"
LangString MissProgAspell "${LYX_LANG}" "Aspell, un corrector ortogr�fic"
LangString MissProgMessage "${LYX_LANG}" 'No �s necessari instal�lar cap programa addicional.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'Distribuci� LaTeX'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Establiu la distribuci� LaTeX que ha de fer servir el LyX.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Opcionalment podeu establir aqu� el cam� al fitxer "latex.exe" i posteriorment establir la \
					   distribuci� LaTeX que el LyX ha de fer servir.\r\n\
					   Si no useu LaTeX, el LyX no podr� generar documents!\r\n\
					   \r\n\
					   L$\'instal�lador ha detectat la distribuci� LaTeX \
					   "$LaTeXName" al sistema, al cam� que es mostra avall.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Indiqueu el cam� al fitxer "latex.exe". Posteriorment establiu \
					       quina distribuci� LaTeX ha de fer servir el LyX.\r\n\
					       Si no useu LaTeX, el LyX no podr� generar documents!\r\n\
					       \r\n\
					       L$\'instal�lador no ha trobat cap distribuci� LaTeX al sistema.'
LangString PathName "${LYX_LANG}" 'Cam� al fitxer "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "No usis LaTeX"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'El fitxer "latex.exe" no es troba al cam� indicat.'

LangString LatexInfo "${LYX_LANG}" 'Ara s$\'executar� l$\'instal�lador de la distribuci� LaTeX "MiKTeX".$\r$\n\
			            Per instal�lar el programa pitgeu el bot� "Seg�ent" a les finestres de l$\'instal�lador fins que la instal�laci� comenci.$\r$\n\
				    $\r$\n\
				    Si us plau, useu les opcions predeterminades de l$\'instal�lador MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" 'No s$\'ha trobat cap distribuci� LaTeX!$\r$\n\
                      		      No podeu usar el LyX sense cap distribuci� LaTeX, com ara "MiKTeX"!$\r$\n\
				      Es cancel�lar� la instal�laci�.'
			    
LangString GSviewInfo "${LYX_LANG}" 'Ara s$\'executar� l$\'instal�lador del programa "GSview".$\r$\n\
			             Per instal�lar el programa pitgeu el bot� "Setup" a la primera finestra de l$\'instal�lador,$\r$\n\
				     seleccioneu la llengua i pitgeu el bot� "Seg�ent" a la finstres seg�ent de l$\'instal�lador.$\r$\n\
				     Podeu usar totes les opcions predeterminades de l$\'instal�lador del GSview.'
LangString GSviewError "${LYX_LANG}" 'El programa "GSview" no s$\'ha instal�lat correctament!$\r$\n\
		                      L$\'instal�lador continuar� igualment.$\r$\n\
				      Intenteu instal�lar el GSview altre cop m�s tard.'
				      
LangString JabRefInfo "${LYX_LANG}" 'Ara s$\'executar� l$\'instal�lador del programa "JabRef".$\r$\n\
				     Podeu usar totes les opcions predeterminades de l$\'instal�lador del JabRef.'
LangString JabRefError "${LYX_LANG}" 'El programa "JabRef" no s$\'ha instal�lat correctament!$\r$\n\
		                      L$\'instal�lador continuar� igualment.$\r$\n\
				      Intenteu instal�lar el JabRef altre cop m�s tard.'
				      
LangString LatexConfigInfo "${LYX_LANG}" "La configuraci� seg�ent del LyX pot trigar una mica."

LangString AspellInfo "${LYX_LANG}" 'Ara es descarregaran i instal�laran els diccionaris pel corrector ortogr�fic "Aspell".$\r$\n\
				     Cada diccionari t� una llic�ncia diferent que es mostrar� abans de la instal�laci�.'
LangString AspellDownloadFailed "${LYX_LANG}" "No s$\'ha pogut descarregar cap diccionari de l$\'Aspell!"
LangString AspellInstallFailed "${LYX_LANG}" "No s$\'ha pogut instal�lar cap diccionari de l$\'Aspell!"
LangString AspellPartAnd "${LYX_LANG}" " i "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'Ja es troba instal�lat un diccionari per a la llengua'
LangString AspellNone "${LYX_LANG}" 'No s$\'ha instal�lat el diccionari de l$\'Aspell.$\r$\n\
				     Podeu descarregar els diccionaris des de$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Voleu descarregar els diccionaris ara?'
LangString AspellPartStart "${LYX_LANG}" "S'ha instal�lat correctament "
LangString AspellPart1 "${LYX_LANG}" "un diccionari per a la llengua anglesa"
LangString AspellPart2 "${LYX_LANG}" "un diccionari per a la llengua $LangName"
LangString AspellPart3 "${LYX_LANG}" "un$\r$\n\
				      diccionari per a la llengua $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' pel corrector ortogr�fic "Aspell".$\r$\n\
				      Podeu descarregar m�s diccionaris des de$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Voleu descarregar altres diccionaris ara?'

LangString MiKTeXPathInfo "${LYX_LANG}" "Per a que cada usuari pugui personalitzar el MiKTeX segons les seves necessitats\r$\n\
					 �s necessari que la carpeta on es troba instal�lat el MiKTeK$\r$\n\
					 $MiKTeXPath $\r$\n\
					 i a les seves subcarpetes tinguin permissos d'escriptura per a tots els usuaris."
LangString MiKTeXInfo "${LYX_LANG}" 'La distribuci� LaTeX "MiKTeX" conjuntament amb el LyX.$\r$\n\
				     �s recomenable instal�lar les actualitzacions MiKTeX fent servir el programa "MiKTeX Update Wizard"$\r$\n\
				     abans d$\'executar el LyX per primer cop.$\r$\n\
				     Voleu comprobar ara si hi ha actualitzacions del MiKTeX?'

LangString UpdateNotAllowed "${LYX_LANG}" "Aquest paquet d'actualitzaci� nom�s actualitza ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "No es pot establir 'path_prefix' durant el programa de configuraci�"
LangString CreateCmdFilesFailed "${LYX_LANG}" "No es pot crear el fitxer lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "No es pot executar el programa de configuraci�"
LangString NotAdmin "${LYX_LANG}" "Necessiteu drets d'administrador per instal�lar el LyX!"
LangString InstallRunning "${LYX_LANG}" "L'instal�lador ja s'est� executant!"
LangString StillInstalled "${LYX_LANG}" "El LyX ja es troba instal�lat! Desinstal�leu-lo primer."

LangString FinishPageMessage "${LYX_LANG}" "Felicitats! Heu instal�lat correctament el LyX.\r\n\
					    \r\n\
					    (La primera execuci� del LyX pot trigar alguns segons.)"
LangString FinishPageRun "${LYX_LANG}" "Executa el LyX"

LangString UnNotInRegistryLabel "${LYX_LANG}" "No es possible trobar el LyX al registre.$\r$\n\
					       No se suprimiran les dreceres de l'escriptori i del men� inici."
LangString UnInstallRunning "${LYX_LANG}" "Primer heu de tancar el LyX!"
LangString UnNotAdminLabel "${LYX_LANG}" "Necessiteu drets d'administrador per desinstal�lar el LyX!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Esteu segur de voler suprimir completament el LyX i tots els seus components?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'Prefer�ncies d$\'usuari del LyX'
LangString UnGSview "${LYX_LANG}" 'Pitgeu el bot� "Uninstall" a la finestra seg�ent per desinstal�lar\r$\n\
				   el visor de fitxers Postscript i PDF "GSview".'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "El LyX s'ha desinstal�lat correctament de l'ordinador."

LangString SecUnAspellDescription "${LYX_LANG}" 'Desinstal�la el corrector ortogr�fic Aspell i tots els seus diccionaris.'
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Desintal�la la distribuci� de LaTeX MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Desinstal�la el gestor de bibliografia JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Suprimeix les carptes de configuraci� del LyX$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   de tots els usuaris.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Desinstal�la el LyX i tots els seus components."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_CATALAN_NSH_
