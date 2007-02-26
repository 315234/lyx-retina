/*

Uninstaller

*/

#--------------------------------
#Sections

Section "un.Program Files" un.SecProgramFiles

  #Binaries
  
  Delete "$INSTDIR\bin\lyx.exe"
  Delete "$INSTDIR\bin\lyxc.exe"
  !insertmacro FileListLyXBin Delete "$INSTDIR\bin\"
  !insertmacro FileListQtBin Delete "$INSTDIR\bin\"
  
  !insertmacro FileListDllMSVCBin Delete "$INSTDIR\bin\"
  !insertmacro FileListMSVCBin Delete "$INSTDIR\bin\"
  !insertmacro FileListMSVCManifest Delete "$INSTDIR\bin\"

  !insertmacro FileListNetpbmBin Delete "$INSTDIR\bin\"
  !insertmacro FileListDvipostBin Delete "$INSTDIR\bin\"
  !insertmacro FileListDTLBin Delete "$INSTDIR\bin\"
  !insertmacro FileListPDFToolsBin Delete "$INSTDIR\bin\"
  !insertmacro FileListPDFViewBin Delete "$INSTDIR\bin\"
  
  #Icons
  
  !insertmacro FileListLyXIcons Delete "$INSTDIR\bin\"  
  
  #Resources
  
  RMDir /r "$INSTDIR\Resources"
  
  #Components of Python
  
  RMDir /r "$INSTDIR\python"
  
  #Aiksaurus Data
  
  !insertmacro FileListAiksaurusData Delete "$INSTDIR\aiksaurus\"
  RMDir "$INSTDIR\aiksaurus"
  
  #Helper DLLs for NSIS-based tools

  !insertmacro FileListNSISPluginsStandard Delete "$INSTDIR\bin\"
  !insertmacro FileListNSISPlugins Delete "$INSTDIR\bin\"   
  
  #Shortcuts
  
  Delete "$SMPROGRAMS\${APP_NAME} ${APP_SERIES_NAME}.lnk"
  Delete "$DESKTOP\${APP_NAME} ${APP_SERIES_NAME}.lnk"

  #Uninstaller itself
  
  Delete "$INSTDIR\${SETUP_UNINSTALLER}"
  RMDir "$INSTDIR\bin"
  
  #Application folder
  
  SetOutPath "$PROGRAMFILES"
  RMDir "$INSTDIR"
  
  #Registry keys
  
  DeleteRegKey SHELL_CONTEXT "${APP_REGKEY_SETUP}"
  DeleteRegKey SHELL_CONTEXT "${APP_REGKEY}"
  DeleteRegKey SHELL_CONTEXT "Software\Classes\${APP_REGNAME_DOC}"
  DeleteRegKey SHELL_CONTEXT "Software\Microsoft\Windows\CurrentVersion\Uninstall\${SETUP_UNINSTALLER_KEY}"
  
  #File associations
  
  ReadRegStr $R0 SHELL_CONTEXT "Software\Classes\${APP_EXT}" ""
  
  ${if} $R0 == "${APP_REGNAME_DOC}"
     DeleteRegKey SHELL_CONTEXT "Software\Classes\${APP_EXT}"
  ${endif}
  
SectionEnd

Section "un.User Preferences and Custom Files" un.SecUserFiles

  SetShellVarContext current
  RMDir /r "$APPDATA\${APP_DIR_USERDATA}"
  
SectionEnd
