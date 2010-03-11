Function ConfigureLyX
 # configures LyX

  # create the PathPrefix
  StrCpy $PathPrefix "$INSTDIR\bin"
  ${if} $PythonPath != ""
    StrCpy $PathPrefix "$PathPrefix;$PythonPath"
  ${endif}
  ${if} $LatexPath != ""
    StrCpy $PathPrefix "$PathPrefix;$LatexPath"
  ${endif}
  ${if} $GhostscriptPath != ""
    StrCpy $PathPrefix "$PathPrefix;$GhostscriptPath"
  ${endif}
  ${if} $ImageMagickPath != ""
    StrCpy $PathPrefix "$PathPrefix;$ImageMagickPath"
  ${endif}
  ${if} $PSVPath != ""
    StrCpy $PathPrefix "$PathPrefix;$PSVPath"
  ${endif}
  ${if} $EditorPath != ""
    StrCpy $PathPrefix "$PathPrefix;$EditorPath"
  ${endif}
  ${if} $ImageEditorPath != ""
    StrCpy $PathPrefix "$PathPrefix;$ImageEditorPath"
  ${endif}
  ${if} $SVGPath != ""
   StrCpy $PathPrefix "$PathPrefix;$SVGPath"
  ${endif}
  ${if} $WMFPath != ""
   StrCpy $PathPrefix "$PathPrefix;$WMFPath"
  ${endif}
  
  # Create a batch file to start LyX with the environment variables set
  ClearErrors
  Delete "${PRODUCT_BAT}"
  FileOpen $R1 "${PRODUCT_BAT}" w
  FileWrite $R1 '@echo off$\r$\n\
		 SET AIK_DATA_DIR=${AiksaurusDir}$\r$\n\
		 start "${PRODUCT_NAME}" "${LAUNCHER_EXE}" %*$\r$\n'
  FileClose $R1
  IfErrors 0 +2
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(CreateCmdFilesFailed)"
   
  # Set the path prefix in lyxrc.dist
  ClearErrors
  Delete "$INSTDIR\Resources\lyxrc.dist"
  FileOpen $R1 "$INSTDIR\Resources\lyxrc.dist" w
  # set some general things
  FileWrite $R1 '\screen_zoom "120"$\r$\n\
                 \accept_compound true$\r$\n\
                 \gui_language $LangNameLyX$\r$\n'
  ${if} "$PathPrefix" != ""
   FileWrite $R1 '\path_prefix "$PathPrefix"$\r$\n'
  ${endif}
  # if Acrobat or Adobe Reader is used
  ${if} $Acrobat == "Yes" # used for Acrobat / Adobe Reader
   FileWrite $R1 '\format "pdf3" "pdf" "PDF (dvipdfm)" "m" "pdfview" "" "document,vector"$\r$\n\
   		  \format "pdf2" "pdf" "PDF (pdflatex)" "F" "pdfview" "" "document,vector"$\r$\n\
		  \format "pdf" "pdf" "PDF (ps2pdf)" "P" "pdfview" "" "document,vector"$\r$\n'
  ${endif}
  # if a SVG to PDF converter ws found (e.g. Inkscape)
  ${if} $SVGPath != ""
   FileWrite $R1 '\format "svg" "svg" "SVG" "" "inkscape --file=$$$$i" "inkscape --file=$$$$i" "vector"$\r$\n\
   		  \converter "svg" "png" "inkscape --without-gui --file=$$$$i --export-png=$$$$o" ""$\r$\n\
		  \converter "svg" "pdf" "inkscape --file=$$$$p/$$$$i --export-area-drawing --without-gui --export-pdf=$$$$p/$$$$o" ""$\r$\n\
		  \converter "svg" "eps" "inkscape --file=$$$$p/$$$$i --export-area-drawing --without-gui --export-eps=$$$$p/$$$$o" ""'
  ${endif}
  FileClose $R1
  IfErrors 0 +2
   MessageBox MB_OK|MB_ICONEXCLAMATION "$(ModifyingConfigureFailed)"
  ClearErrors
  
  # register LyX
  ${if} $CreateFileAssociations == "true"
   WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "${PRODUCT_EXE}"
  ${endif}
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "RootKey" "$ProductRootKey"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "UninstallString" "${PRODUCT_UNINSTALL_EXE}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "DisplayIcon" "${PRODUCT_EXE}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "StartMenu" "$SMPROGRAMS\$StartmenuFolder"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "URLUpdateInfo" "${PRODUCT_INFO_URL}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_ABOUT_URL}"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "Publisher" "LyX Team"
  WriteRegStr SHCTX "${PRODUCT_UNINST_KEY}" "HelpLink" "${PRODUCT_HELP_LINK}"
  WriteRegDWORD SHCTX "${PRODUCT_UNINST_KEY}" "NoModify" 0x00000001
  WriteRegDWORD SHCTX "${PRODUCT_UNINST_KEY}" "NoRepair" 0x00000001

  # create start menu entry
  SetOutPath "$INSTDIR\bin"
  CreateDirectory "$SMPROGRAMS\$StartmenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartmenuFolder\${PRODUCT_NAME}.lnk" "${PRODUCT_BAT}" "" "${PRODUCT_EXE}"
  SetOutPath "$INSTDIR"
  CreateShortCut "$SMPROGRAMS\$StartmenuFolder\Uninstall.lnk" "${PRODUCT_UNINSTALL_EXE}"

  # create desktop icon
  ${if} $CreateDesktopIcon == "true"
   SetOutPath "$INSTDIR\bin"
   CreateShortCut "$DESKTOP\LyX ${PRODUCT_VERSION}.lnk" "${PRODUCT_BAT}" "" "${PRODUCT_EXE}"
  ${endif}

  # register the extension .lyx
  ${if} $CreateFileAssociations == "true"
   WriteRegStr SHCTX "${PRODUCT_DIR_REGKEY}" "OnlyWithLyX" "Yes${PRODUCT_VERSION_SHORT}" # special entry to test if they were registered by this LyX version
   # write informations about file type
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_REGNAME}" "" "${PRODUCT_NAME} Document"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_REGNAME}\DefaultIcon" "" "${PRODUCT_EXE}"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_REGNAME}\Shell\open\command" "" '"${PRODUCT_BAT}" "%1"'
   # write informations about file extensions
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}" "" "${PRODUCT_REGNAME}"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}" "Content Type" "${PRODUCT_MIME_TYPE}"
   # .lyx13
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}13" "" "${PRODUCT_REGNAME}"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}13" "Content Type" "${PRODUCT_MIME_TYPE}"
   # .lyx14
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}14" "" "${PRODUCT_REGNAME}"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}14" "Content Type" "${PRODUCT_MIME_TYPE}"
   # .lyx15 don't set this, because this is designed to be opened with LyX 1.5.x
   #WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}15" "" "${PRODUCT_REGNAME}"
   #WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}15" "Content Type" "${PRODUCT_MIME_TYPE}"
   # .lyx16
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}16" "" "${PRODUCT_REGNAME}"
   WriteRegStr SHCTX "Software\Classes\${PRODUCT_EXT}16" "Content Type" "${PRODUCT_MIME_TYPE}"
   # refresh shell
   System::Call 'shell32.dll::SHChangeNotify(i, i, i, i) (${SHCNE_ASSOCCHANGED}, ${SHCNF_IDLIST}, 0, 0)'
  ${endif}

  # delete unnecessary files
  ${if} $DelPythonFiles == "True"
   Delete $INSTDIR\bin\python.exe
   Delete $INSTDIR\bin\python26.dll
   Delete $INSTDIR\bin\Python-License.txt
   RMDir /r $INSTDIR\bin\Lib
   RMDir /r $INSTDIR\bin\DLLs
  ${endif}
  RMDir /r $INSTDIR\external

  # create a bat-file to start configure in a console window so that the user see the progress
  # of the configuration and to have a signal when the configuration is ready to start LyX
  # this is important when LyX is installed together with MiKTeX or when LyX is installed for the first
  # time on a computer, because the installation of missing LaTeX-files required by LyX could last minutes
  # a batch file is needed because simply calling
  # ExecWait '"$PythonPath\python.exe" "$INSTDIR\Resources\configure.py"'
  # creates the config files in $PythonPath
  ${if} $PythonPath == ""
   StrCpy $PythonPath "$INSTDIR\bin"
  ${endif}
  StrCpy $1 $INSTDIR 2 # get drive letter
  FileOpen $R1 "$INSTDIR\Resources\configLyX.bat" w
  FileWrite $R1 '$1$\r$\n\
  		 cd $INSTDIR\Resources\$\r$\n\
  		 "$PythonPath\python.exe" configure.py'
  FileClose $R1

FunctionEnd

