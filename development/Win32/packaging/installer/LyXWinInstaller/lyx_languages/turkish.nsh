!ifndef _LYX_LANGUAGES_TURKISH_NSH_
!define _LYX_LANGUAGES_TURKISH_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_TURKISH}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "Bu sihirbaz size LyX program�n� kuracak.\r\n\
					  \r\n\
					  LyX kurabilmek i�in y�netici yetkilerine ihtiyac�n�z var.\r\n\
					  \r\n\
					  Kuruluma ba�lamadan �nce di�er programlar� kapatman�z tavsiye edilir. B�ylece bilgisayar�n�z� yeniden ba�latman�za gerek kalmadan ilgili sistem dosyalar� g�ncellenebilir.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "LyX-Document"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "T�m kullan�c�lar i�in kur?"
LangString SecFileAssocTitle "${LYX_LANG}" "Dosya e�le�meleri"
LangString SecDesktopTitle "${LYX_LANG}" "Masa�st� ikonu"

LangString SecCoreDescription "${LYX_LANG}" "LyX dosyalar�."
LangString SecInstGSviewDescription "${LYX_LANG}" "Postscript ve PDF g�sterici program."
LangString SecInstJabRefDescription "${LYX_LANG}" "Bibliography reference manager and editor for BibTeX files."
LangString SecAllUsersDescription "${LYX_LANG}" "LyX t�m kullan�c�lar i�in mi yoksa yaln�zca bu kullan�c�ya m� kurulacak."
LangString SecFileAssocDescription "${LYX_LANG}" "Uzant�s� .lyx olan dosyalar otomatik olarak LyX ile a��ls�n."
LangString SecDesktopDescription "${LYX_LANG}" "Masa�st�ne bir LyX ikonu koy."

LangString LangSelectHeader "${LYX_LANG}" "LyX arabirim dili se�imi"
LangString AvailableLang "${LYX_LANG}" " Mevcut Diller "

LangString MissProgHeader "${LYX_LANG}" "Gerekli programlar�n kontrol�"
LangString MissProgCap "${LYX_LANG}" "A�a��daki gerekli programlar da otomatik olarak kurulacak"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, bir LaTeX da��t�m�"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, bir PostScript ve PDF yorumlay�c�"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, bir resim d�n��t�r�c�"
LangString MissProgAspell "${LYX_LANG}" "Aspell, bir yaz�m denetleyici"
LangString MissProgMessage "${LYX_LANG}" 'Ek bir program kurmaya gerek bulunmuyor.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" 'LaTeX-da��t�m�'
LangString EnterLaTeXHeader2 "${LYX_LANG}" 'LyX in kullanaca�� LaTeX da��t�m�n� se�in.'
LangString EnterLaTeXFolder "${LYX_LANG}" 'You can optionally set here the path to the file "latex.exe" and therewith set the \
					   LaTeX-distribution that should be used by LyX.\r\n\
					   If you don$\'t use LaTeX, LyX cannot output documents!\r\n\
					   \r\n\
					   The installer has detected the LaTeX-distribution \
					   "$LaTeXName" on your system. Displayed below is its path.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" 'Set below the path to the file "latex.exe". Therewith you set which \
					       LaTeX-distribution should be used by LyX.\r\n\
					       If you don$\'t use LaTeX, LyX cannot output documents!\r\n\
					       \r\n\
					       The installer couldn$\'t find a LaTeX-distribution on your system.'
LangString PathName "${LYX_LANG}" 'Path to the file "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "LaTeX kullanma"
LangString InvalidLaTeXFolder "${LYX_LANG}" '"latex.exe" dosyas� belirtti�iniz dizinde bulunamad�.'

LangString LatexInfo "${LYX_LANG}" '�imdi LaTeX da��t�m� "MiKTeX" kurulumu ba�lat�lacak.$\r$\n\
			            Program� kurmak i�in kurulum ba�layana kadar "Devam" tu�uyla ilerleyin.$\r$\n\
				    $\r$\n\
				    !!! L�tfen MiKTeX kurucusunun �ntan�ml� ayarlar�n� de�i�tirmeyin !!!'
LangString LatexError1 "${LYX_LANG}" 'Hi� bir LaTeX da��t�m� bulunamad�!$\r$\n\
                      		      LyX "MiKTeX" gibi bir LaTeX da��t�m� olmadan kullan�lamaz!$\r$\n\
				      Bu nedenle kurulum iptal edilecek.'
			
LangString GSviewInfo "${LYX_LANG}" '�imdi "GSview" kurulum program� ba�lat�lacak.$\r$\n\
			             Program� kurmak i�in ile kurulum penceresinde "Setup" tu�una bas�p$\r$\n\
				     bir dil se�in, ve sonraki pencerelerde "Devam" tu�uyla ilerleyin.$\r$\n\
				     GSview kurulumunun t�m �ntan�ml� se�eneklerini kullanabilirsiniz.'
LangString GSviewError "${LYX_LANG}" '"GSview" kurulumu ba�ar�s�z oldu!$\r$\n\
		                      Kurulum gene de devam edecek.$\r$\n\
				      Daha sonra GSview kurmay� tekrar deneyin.'

LangString JabRefInfo "${LYX_LANG}" 'Now the installer of the program "JabRef" will be launched.$\r$\n\
				     You can use all default options of the JabRef-installer.'
LangString JabRefError "${LYX_LANG}" 'The program "JabRef" could not successfully be installed!$\r$\n\
		                      The installer will continue anyway.$\r$\n\
				      Try to install JabRef again later.'

LangString LatexConfigInfo "${LYX_LANG}" "S�radaki LyX yap�land�rmas� biraz zaman alacak."

LangString AspellInfo "${LYX_LANG}" 'Now dictionaries for the spellchecker "Aspell" will be downloaded and installed.$\r$\n\
				     Every dictionary has a different license that will be displayed before the installation.'
LangString AspellDownloadFailed "${LYX_LANG}" "No Aspell spellchecker dictionary could be downloaded!"
LangString AspellInstallFailed "${LYX_LANG}" "No Aspell spellchecker dictionary could be installed!"
LangString AspellPartAnd "${LYX_LANG}" " and "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" 'There is already installed a dictionary of the language'
LangString AspellNone "${LYX_LANG}" 'No dictionary for the spellchecker "Aspell" has been installed.$\r$\n\
				     Dictionaries can be downloaded from$\r$\n\
				     ${AspellLocation}$\r$\n\
				     Do you want to download dictionaries now?'
LangString AspellPartStart "${LYX_LANG}" "There was successfully installed "
LangString AspellPart1 "${LYX_LANG}" "an english dictionary"
LangString AspellPart2 "${LYX_LANG}" "a dictionary of the language $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
				      dictionary of the language $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' for the spellchecker "Aspell".$\r$\n\
				      More dictionaries can be downloaded from$\r$\n\
				      ${AspellLocation}$\r$\n\
				      Do you want to download other dictionaries now?'

LangString MiKTeXPathInfo "${LYX_LANG}" "In order that every user is later able to customize MiKTeX for his needs$\r$\n\
					 it is necessary to set write permissions for all users to MiKTeX's installation folder$\r$\n\
					 $MiKTeXPath $\r$\n\
					 and its subfolders."
LangString MiKTeXInfo "${LYX_LANG}" 'The LaTeX-distribution "MiKTeX" will be used together with LyX.$\r$\n\
				     It is recommended to install available MiKTeX-updates using the program "MiKTeX Update Wizard"$\r$\n\
				     before you use LyX for the first time.$\r$\n\
				     Would you now check for MiKTeX updates?'
				
LangString ModifyingConfigureFailed "${LYX_LANG}" "Yap�land�rma program�nda 'path_prefix' ayarlanamad�"
LangString CreateCmdFilesFailed "${LYX_LANG}" "lyx.bat olu�turulamad�"
LangString RunConfigureFailed "${LYX_LANG}" "Yap�land�rma program� �al��t�r�lamad�"
LangString NotAdmin "${LYX_LANG}" "LyX kurabilmek i�in y�netici yetkileri gerekiyor!"
LangString InstallRunning "${LYX_LANG}" "Kurulum program� zaten �al���yor!"
LangString StillInstalled "${LYX_LANG}" "LyX kurulu zaten! �nce onu kald�r�n."

LangString FinishPageMessage "${LYX_LANG}" "Tebrikler! LyX ba�ar�yla kuruldu.\r\n\
					    \r\n\
					    (LyX in ilk a��l��� birka� saniye alabilir.)"
LangString FinishPageRun "${LYX_LANG}" "LyX Ba�lat"

LangString UnNotInRegistryLabel "${LYX_LANG}" "Sistem k�t���nde LyX bulunamad�.$\r$\n\
					       Ba�lat men�s� ve masa�st�n�zdeki k�sayollar silinemeyecek."
LangString UnInstallRunning "${LYX_LANG}" "�nce LyX i kapatmal�s�n�z!"
LangString UnNotAdminLabel "${LYX_LANG}" "LyX kald�rabilmek i�in y�netici yetkileri gerekiyor!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "LyX ve t�m bile�enlerini kald�rmak istedi�inize emin misiniz?"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX$\'s user preferences'
LangString UnGSview "${LYX_LANG}" 'L�tfen bir sonraki pencerede "Kald�r" d��mesine basarak$\r$\n\
				   "GSview" postscript ve pdf g�stericisini kald�r�n.'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "LyX bilgisayar�n�zdan ba�ar�yla kald�r�ld�."

LangString SecUnAspellDescription "${LYX_LANG}" "Uninstalls the spellchecker Aspell and all of its dictionaries."
LangString SecUnMiKTeXDescription "${LYX_LANG}" "Uninstalls the LaTeX-distribution MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "Uninstalls the bibliography manager JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" 'Deletes LyX$\'s configuration folder$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   for all users.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "Uninstall LyX and all of its components."

!undef LYX_LANG

!endif ; _LYX_LANGUAGES_TURKISH_NSH_













