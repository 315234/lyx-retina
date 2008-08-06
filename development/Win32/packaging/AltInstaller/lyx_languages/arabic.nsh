!ifndef _LYX_LANGUAGES_ARABIC_NSH_
!define _LYX_LANGUAGES_ARABIC_NSH_

!ifdef LYX_LANG
  !undef LYX_LANG
!endif
!define LYX_LANG ${LANG_ARABIC}

LicenseLangString LyXLicenseData ${LYX_LANG} "${PRODUCT_LICENSE_FILE}"

LangString WelcomePageText "${LYX_LANG}" "������ �������� ���� ����� ����� ������ ��� ���� ������.\r\n\
					  \r\n\
					  ��� ��� �� ���� ������� ������ ����� ����� �������.\r\n\
					  \r\n\
					  ��� ���� �� ����� ���� ������� ������� ��� ����� ������ �������� ���� ��� ��� ����� ����� ������ ��� ������ ������ ����� �������.\r\n\
					  \r\n\
					  $_CLICK"

LangString FileTypeTitle "${LYX_LANG}" "�����-���"

LangString SecInstGSviewTitle "${LYX_LANG}" "GSview"
LangString SecInstJabRefTitle "${LYX_LANG}" "JabRef"
LangString SecAllUsersTitle "${LYX_LANG}" "����� ��� ����������"
LangString SecFileAssocTitle "${LYX_LANG}" "����� ������"
LangString SecDesktopTitle "${LYX_LANG}" "��� ��� ������"

LangString SecCoreDescription "${LYX_LANG}" "����� ���."
LangString SecInstGSviewDescription "${LYX_LANG}" "������ ���� �������� �������� PDF."
LangString SecInstJabRefDescription "${LYX_LANG}" "���� ���� ����������� ������ ������ BibTeX."
LangString SecAllUsersDescription "${LYX_LANG}" "����� ������ ��� ��� ���������� �� �������� ������ ���."
LangString SecFileAssocDescription "${LYX_LANG}" "������� ��� �������� .lyx ���� ����� �������� ������� ���."
LangString SecDesktopDescription "${LYX_LANG}" "��� ��� ��� ��� ������."

LangString LangSelectHeader "${LYX_LANG}" "����� ��� ����� ���"
LangString AvailableLang "${LYX_LANG}" " ������ ������� "

LangString MissProgHeader "${LYX_LANG}" "������ �� ������� ��������"
LangString MissProgCap "${LYX_LANG}" "������� �������� ������� ���� �������"
LangString MissProgLatex "${LYX_LANG}" "MiKTeX, ������ ����"
LangString MissProgGhostscript "${LYX_LANG}" "Ghostscript, ���� �������� �������� PDF"
LangString MissProgImageMagick "${LYX_LANG}" "ImageMagick, ���� �����"
LangString MissProgAspell "${LYX_LANG}" "Aspell, ������ ��������"
LangString MissProgMessage "${LYX_LANG}" '�� ���� ������ ����� ������.'

LangString EnterLaTeXHeader1 "${LYX_LANG}" '������-����'
LangString EnterLaTeXHeader2 "${LYX_LANG}" '���� ������-���� �������� �������� �� ���.'
LangString EnterLaTeXFolder "${LYX_LANG}" '����� ��� ������ ������ �����  "latex.exe" ���� ����  \
					   ������-���� ���� ��� �������� �� ���.\r\n\
					   ��� ��� �� ������ ���� ��� ��� �� ������ ����� ���������!\r\n\
					   \r\n\
					   �� ������ ��� ������-���� \
					   "$LaTeXName" �� ����� ���� ����� �� ������.'
LangString EnterLaTeXFolderNone "${LYX_LANG}" '���� �� ���� �����  "latex.exe". ���� ���� \
					       ������-���� ���� ��� �������� �� ���.\r\n\
					       ��� ��� �� ������ ���� ��� ��� �� ������ ����� ���������!\r\n\
					       \r\n\
					       �� ��� ������ ��� ������-���� �� �����.'
LangString PathName "${LYX_LANG}" '���� �����  "latex.exe"'
LangString DontUseLaTeX "${LYX_LANG}" "�� ������ ����"
LangString InvalidLaTeXFolder "${LYX_LANG}" '����� "latex.exe" ���� �� ������ ������.'

LangString LatexInfo "${LYX_LANG}" '���� ����� ������ ������-���� ������� "MiKTeX".$\r$\n\
			            ���� ��� �� ������ ��������� �� ����� �������.$\r$\n\
				    $\r$\n\
				    !!! ����� ������ ���� �������� ���������� ��� ����� MiKTeX !!!'
LangString LatexError1 "${LYX_LANG}" '�� ��� ������ ��� ������-����!$\r$\n\
                      		      �� ���� ������� ��� ���� ������ ���� ��� MiKTeX!$\r$\n\
				      ���� ����� �������.'
			    
LangString GSviewInfo "${LYX_LANG}" '�� ��� ����� ������ GSview.$\r$\n\
			             ������ �������� ���� ��� �� ����� �� ������� ������ ����� ����� �������,$\r$\n\
				     ���� ����� �� ���� ������ �� ������� �������.$\r$\n\
				     ������ ������ �� �������� ����� ����� ������ GSview.'
LangString GSviewError "${LYX_LANG}" '�� ���� ����� ������ GSview!$\r$\n\
		                    ������ ����� �������.$\r$\n\
				      ���� ����� ������ GSview ������.'
				      
LangString JabRefInfo "${LYX_LANG}" '���� ���� ����� ������ JabRef.$\r$\n\
				     ������ ������ �� �������� ����� ����� ������ JabRef.'
LangString JabRefError "${LYX_LANG}" '�� ���� ����� ������ JabRef!$\r$\n\
		                      ������ ����� �������.$\r$\n\
				      ���� ����� JabRef ������.'
				      
LangString LatexConfigInfo "${LYX_LANG}" "������� ������� ������� ��� �����."

LangString AspellInfo "${LYX_LANG}" '�� ��� ����� ������ �� ������ ������ �������� Aspell.$\r$\n\
				     �� ����� ���� ���� ������ ���� ����� ��� �������.'
LangString AspellDownloadFailed "${LYX_LANG}" "�� ��� ����� ����� ������ �������� Aspell!"
LangString AspellInstallFailed "${LYX_LANG}" "�� ��� ����� ����� ������ �������� Aspell!"
LangString AspellPartAnd "${LYX_LANG}" " � "
LangString AspellPartSep "${LYX_LANG}" ", "
LangString AspellAll "${LYX_LANG}" '�� ������ ����� ����� ����'
LangString AspellNone "${LYX_LANG}" '�� ��� ����� ����� ������ �������� Aspell.$\r$\n\
				     ���� ����� �������� �� $\r$\n\
				     ${AspellLocation}$\r$\n\
				     �� ���� ������ �������� ����'
LangString AspellPartStart "${LYX_LANG}" "�� ������� ����� "
LangString AspellPart1 "${LYX_LANG}" "����� �������"
LangString AspellPart2 "${LYX_LANG}" "����� ���  $LangName"
LangString AspellPart3 "${LYX_LANG}" "a$\r$\n\
				      ����� ����� $LangNameSys"
LangString AspellPart4 "${LYX_LANG}" ' ������ �������� Aspell.$\r$\n\
				      ���� ����� ������ ���� �� $\r$\n\
				      ${AspellLocation}$\r$\n\
				      �� ���� ������ ������ ���� ����'

LangString MiKTeXPathInfo "${LYX_LANG}" "��� ���� �� ������ ����� ��� ����� miktex ������ ��� ��������� $\r$\n\
					 �� ������� ����� ������� ���������� �� ���� ����� MiKTeX$\r$\n\
					 $MiKTeXPath $\r$\n\
					 ��� �������� �������."
LangString MiKTeXInfo "${LYX_LANG}" '������-���� MiKTeX ������ ����� �� ��� ���.$\r$\n\
				     ��� ������ �� ������ ��������� ������� �� miktex - ������� ������ "����� ����� miktex"$\r$\n\
				     ��� ������� ��� ���� ���.$\r$\n\
				     �� ���� ������� �� ��������� ����'

LangString UpdateNotAllowed "${LYX_LANG}" "��� ������� ��� ��� �� ${PRODUCT_VERSION_OLD}!"
LangString ModifyingConfigureFailed "${LYX_LANG}" "Could not set 'path_prefix' in the configure script"
LangString CreateCmdFilesFailed "${LYX_LANG}" "���� ����� lyx.bat"
LangString RunConfigureFailed "${LYX_LANG}" "�� ���� ����� ��� configure script"
LangString NotAdmin "${LYX_LANG}" "��� �� ���� ������� ������ ��� ������ ������ �������!"
LangString InstallRunning "${LYX_LANG}" "�� ������ ����� ������ �������!"
LangString StillInstalled "${LYX_LANG}" "�� ������ ����� ������ ��� ��� ���� ����� ����� ����� �������������."

LangString FinishPageMessage "${LYX_LANG}" "�������! �� ����� ��� �����.\r\n\
					    \r\n\
					    (����� ��� ���� ��� �� ���� ��� �����.)"
LangString FinishPageRun "${LYX_LANG}" "����� ���"

LangString UnNotInRegistryLabel "${LYX_LANG}" "�� ��� ������ ��� ��� �� ���������.$\r$\n\
					       �� ��� ��� �������� ��� �� ��� ������ ������ �������."
LangString UnInstallRunning "${LYX_LANG}" "��� ����� ��� �����!"
LangString UnNotAdminLabel "${LYX_LANG}" "��� �� ���� ������� ������ ����� ������ �����!"
LangString UnReallyRemoveLabel "${LYX_LANG}" "�� ���� ���� ������ ��� ������� �� �� ��������� ����� ��"
LangString UnLyXPreferencesTitle "${LYX_LANG}" 'LyX$\'s ������� ��������'
LangString UnGSview "${LYX_LANG}" '����� ���� ��� �� ������ ����� ������ ����� ���$\r$\n\
				   ������ GSview ����� ���� ���������� �������� PDF.'
LangString UnRemoveSuccessLabel "${LYX_LANG}" "�� ��� ������ ��� �����."

LangString SecUnAspellDescription "${LYX_LANG}" '�� ����� ������ �������� Aspell ��� �������� ������ ��.'
LangString SecUnMiKTeXDescription "${LYX_LANG}" "�� ����� ����- ���� MiKTeX."
LangString SecUnJabRefDescription "${LYX_LANG}" "�� ����� ���� ������������� JabRef."
LangString SecUnPreferencesDescription "${LYX_LANG}" '��� ���� ����� ���$\r$\n\
					   "$AppPre\username\$AppSuff\${PRODUCT_SUBFOLDER}"$\r$\n\
					   ��� ����������.'
LangString SecUnProgramFilesDescription "${LYX_LANG}" "����� ��� ��� ��������� ������ ��."

!undef LYX_LANG

!endif # _LYX_LANGUAGES_ARABIC_NSH_
