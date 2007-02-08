!ifndef _LYX_LANGUAGES_HUNGARIAN_NSH_
!define _LYX_LANGUAGES_HUNGARIAN_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_HUNGARIAN}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "A var�zsl� seg�ts�g�vel tudja telep�teni a LyX-et.\r\n\
					  \r\n\
					  A LyX telep�t�s�hez rendszergazdai jogra van sz�ks�ge.\r\n\
					  \r\n\
					  A telep�t�s megkezd�se el�tt, javasolt kil�pni minden fut� alkalmaz�sb�l. Ez a l�p�s teszi lehet�v�, hogy friss�ts�nk fontos rendszerf�jlokat �jraind�t�s sz�ks�gess�ge n�lk�l.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-dokumentum"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "Telep�t�s minden felhaszn�l�nak"
LangString SecFileAssocTitle "${LYX_LANG}" "F�jlt�rs�t�sok"
LangString SecDesktopTitle "${LYX_LANG}" "Parancsikon asztalra"

LangString SecCoreDescription "${LYX_LANG}" "A LyX futtat�s�hoz sz�ks�ges f�jlok."
LangString SecInstGSviewDescription "${LYX_LANG}" "Alkalmaz�s Postscript- �s PDF-f�jlok megjelen�t�s�hez."
LangString SecInstJabRefDescription "${LYX_LANG}" "Irodalomjegyz�k hivatkoz�s szerkeszt� �s kezel� BibTeX f�jlokhoz."
LangString SecAllUsersDescription "${LYX_LANG}" "Minden felhaszn�l�nak telep�tsem vagy csak az aktu�lisnak?"
LangString SecFileAssocDescription "${LYX_LANG}" "A .lyx kiterjeszt�ssel rendelkez� f�jlok megnyit�sa automatikusan a LyX-el t�rt�nik."
LangString SecDesktopDescription "${LYX_LANG}" "LyX-ikon elhelyez�se az asztalon."

LangString LangSelectHeader "${LYX_LANG}" "A LyX felhaszn�l�i fel�let nyelv�nek kiv�laszt�sa"
LangString AvailableLang "${LYX_LANG}" " V�laszthat� nyelvek "

LangString MissProgHeader "${LYX_LANG}" "M�k�d�shez sz�ks�ges programok ellen�rz�se"
LangString MissProgCap "${LYX_LANG}" "A k�vetkez� program(ok) telep�t�se fog m�g megt�rt�nni"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, LaTeX-disztrib�ci�"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, ami egy PostScript �s PDF �rtelmez�"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, ami egy k�p�talak�t�"
LangString MissProgAspell "${LYX_LANG}" "Aspell, ami egy helyes�r�s ellen�rz�"
LangString MissProgMessage "${LYX_LANG}" 'Nem kell tov�bbi programokat telep�tenie.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'LaTeX-disztrib�ci�'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'Adja meg a LaTeX-disztrib�ci�t, amit a LyX-nek haszn�lnia kell.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'Itt megadhatja az el�r�si �tvonalat a "latex.exe" f�jlhoz �s ezzel meg is adja \
					   melyik LaTeX disztrib�ci�t fogja haszn�lni a LyX.\r\n\
					   Amennyiben nem haszn�l LaTeX-et, a LyX nem tud kimenetet k�sz�teni!\r\n\
					   \r\n\
					  A telep�t� megtalt�lta az �n sz�m�t�g�p�n a "$LaTeXName" (LaTeX) disztrib�ci�t \ Lent l�thatja az el�r�si �tvonal�t.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Adja meg a haszn�land� "latex.exe" f�jl el�r�si �tvonal�t. Ezzel azt is megadja \
					       melyik LaTeX disztrib�ci�t fogja haszn�lni a LyX.\r\n\
					       Amennyiben nem haszn�l LaTeX-et, a LyX nem tud kimenetet k�sz�teni!\r\n\
					       \r\n\
					       A telep�t� nem tal�lt a sz�m�t�g�p�n LaTeX disztrib�ci�t!'
LangString PathName "${LYX_LANG}" 'A "latex.exe" f�jl el�r�si �tja'
LangString DontUseLaTeX "${LYX_LANG}" "Ne haszn�lja a LaTeX-et"
LangString InvalidLaTeXFolder "${LYX_LANG}" 'Nem tal�lom a "latex.exe" f�jlt, a megadott helyen.'

LangString LatexInfo "${LYX_LANG}" 'A "MiKteX" LaTeX-disztrib�ci� telep�t�j�nek ind�t�sa k�vetkezik.$\r$\n\
			            A program telep�t�s�hez addig nyomkodja a "K�vetkez�" gombot a telep�t� ablak�ban, am�g a telep�t�s el nem kezd�dik.$\r$\n\
				    $\r$\n\
				    !!! K�rem haszn�lja a MikTeX-telep�t� alap�rtelmezett opci�it!!!'
LangString LatexError1 "${LYX_LANG}" 'Nem tal�ltam LaTeX-disztrib�ci�t!$\r$\n\
                      		      A LyX nem haszn�lhat� egy LaTeX-disztib�ci� n�lk�l,(mint p�ld�ul a "MiKTeX")!$\r$\n\
				      Ez�rt a telep�t�s nem folytathat�!'

LangString GSviewInfo "${LYX_LANG}" 'A "GSview" program telep�t�j�nek ind�t�sa k�vetkezik.$\r$\n\
			             A program telep�t�s�hez, nyomja meg a "Telep�t"-gombot a telep�t� els� ablak�ban,$\r$\n\
				     azut�n v�lasszon nyelvet, majd nyomja meg a "K�vetkez�"-gombot a k�vetkez� telep�t� ablakokban.$\r$\n\
				     Haszn�lhatja a program �ltal javasolt be�ll�t�sokat.'
LangString GSviewError "${LYX_LANG}" 'A "GSview" programot nem siker�lt telep�teni!$\r$\n\
		                      Azonban a telep�t�s tov�bb fog folytat�dni.$\r$\n\
				      K�s�bb pr�b�lja meg �jra a GSview telep�t�s�t.'

LangString JabRefInfo "${LYX_LANG}" 'A "JabRef" telep�t�j�nek elind�t�sa k�vetkezik.$\r$\n\
				     Haszn�lhatja az alap�rtelmezett opci�kat.'
LangString JabRefError "${LYX_LANG}" 'Nem tudom telep�teni a "JabRef" programot!$\r$\n\
		                      Azonban a telep�t�s tov�bb fog folytat�dni.$\r$\n\
				      k�s�bb pr�b�lja meg �jra telep�teni a JabRef-et.'

LangString LatexConfigInfo "${LYX_LANG}" "A LyX telep�t�s ut�ni be�ll�t�sa hossz� id�t fog ig�nybe venni."

LangString AspellInfo "${LYX_LANG}" 'Az "Aspell" helyes�r�s-ellen�rz�h�z haszn�lhat� sz�t�rak let�lt�se �s telep�t�se k�vetkezik.$\r$\n\
				     Minden egyes sz�t�rnak k�l�nb�z� licence lehet, ez�rt a licenc minden telep�t�s el�tt meg lesz jelen�tve.'
LangString AspellDownloadFailed "${LYX_LANG}" "Nem tudtam Aspell sz�t�rat let�lteni!"
LangString AspellInstallFailed "${LYX_LANG}" "Nincs telep�tend� Aspell sz�t�r!"
LangString AspellPartAnd "${LYX_LANG}" " �s "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'M�r van telep�tve helyes�r�s-ellen�rz� sz�t�r a k�vetkez� nyelvhez'
LangString AspellNone "${LYX_LANG}" 'Nem lett sz�t�r telep�tve az "Aspell" helyes�r�s-ellen�rz�h�z.$\r$\n\
				     A sz�ks�ges sz�t�rat let�ltheti a k�vetkez� c�mr�l:$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Szeretne most sz�t�rakat let�lteni?'
LangString AspellPartStart "${LYX_LANG}" "Sikeresen telep�lt "
LangString AspellPart1 "${LYX_LANG}" "az angol sz�t�r"
LangString AspellPart2 "${LYX_LANG}" "a(z) $LangName nyelv sz�t�ra"
LangString AspellPart3 "${LYX_LANG}" "a(z)$\r$\n\
				      $LangNameSys nyelv sz�t�ra"
LangString AspellPart4 "${LYX_LANG}" ' az "Aspell" helyes�r�s-ellen�rz�h�z.$\r$\n\
				      Sz�t�rat m�s nyelvekhez a ${AspellLocation}$\r$\n\
				      c�mr�l t�lthet le.$\r$\n\
				      Szeretne let�lteni egy�b sz�t�rakat??'

LangString MiKTeXPathInfo "${LYX_LANG}" "Ahhoz, hogy a t�bbi felhaszn�l� is testre tudja haszn�lni a MiKTeX-et$\r$\n\
					 �r�s jogot kell adni minden felhaszn�l� r�sz�re a MiKTeX alk�nyvt�rhoz.$\r$\n\
					 A $MiKTeXPath $\r$\n\
					 valamint alk�nyvt�raihoz."
LangString MiKTeXInfo "${LYX_LANG}" 'A "MiKTeX" LaTeX-disztib�ci� lesz haszn�lva a LyX �ltal.$\r$\n\
				     Javasolt telep�teni a MiKTeX friss�t�seket a "MiKTeX Update Wizard" seg�ts�g�vel,$\r$\n\
				     a LyX els� ind�t�sa el�tt.$\r$\n\
				     Szeretn� egy ilyen friss�t�st most?'

LangString ModifyingConfigureFailed "${LYX_LANG}" "Nem tudom be�ll�tani a 'path_prefix'-et a configure parancsf�jlban!"
LangString CreateCmdFilesFailed "${LYX_LANG}" "Nem tudom letrehozni a lyx.bat f�jlt."
LangString RunConfigureFailed "${LYX_LANG}" "Nem tudom v�grehajtani a configure parancsf�jlt!"
LangString NotAdmin "${LYX_LANG}" "A(z) LyX telep�t�s�hez rendszergazdai jogok sz�ks�gesek!"
LangString InstallRunning "${LYX_LANG}" "A telep�t� m�r fut!"
LangString StillInstalled "${LYX_LANG}" "A LyX m�r teleptve van! El�sz�r t�vol�tsa el a telep�tettet."

LangString FinishPageMessage "${LYX_LANG}" "Gratul�lok! Sikeresen telep�tette a LyX-et.\r\n\
					    \r\n\
					    (Az program els� ind�t�sa egy kis id�t vesz ig�nybe...)"
LangString FinishPageRun "${LYX_LANG}" "LyX ind�t�sa"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Nem tal�lom a LyX-et a regisztriben.$\r$\n\
					       Az asztalon �s a Start Men�ben tal�lhat� parancsikonok nem lesznek elt�vol�tva!."
LangString UnInstallRunning "${LYX_LANG}" "El�sz�r be kell z�rnia a LyX-et!"
LangString UnNotAdminLabel "${LYX_LANG}" "A(z) LyX elt�vol�t�shoz rendszergazdai jogokkal kell rendelkeznie!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "Biztosan abban, hogy el akarja t�vol�tani a LyX-t, minden tartoz�k�val egy�tt?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX felhaszn�l�i be�ll�t�sok'
LangString UnGSview "${LYX_LANG}" 'K�rem nyomja meg az "Elt�vol�t�s" gombot, a PostScript �s a\r$\n\
				   PDF megjelen�t� "GSview" elt�vol�t�s�hoz a k�vetkez� ablakban.'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX elt�vol�t�sa a sz�m�t�g�p�r�l z�kken�mentesen befejez�d�tt."

LangString SecUnAspellDescription "${LYX_LANG}" "Az Aspell �s minden sz�t�r�nak elt�vol�t�sa."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "A MikTeX LaTeX-disztib�ci� elt�vol�t�sa."
LangString SecUnJabRefDescription "${LYX_LANG}" "A JabRef irodalomjegyz�k kezel� elt�vol�t�sa."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'A  LyX be�ll�t�sok mappa t�rl�se$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   minden felhaszn�l�n�l.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "A LyX �s minden komponens�nek elt�vol�t�sa."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_HUNGARIAN_NSH_
