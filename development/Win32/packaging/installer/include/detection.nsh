/*

Detection functions for all components

*/

;--------------------------------
;Macros

!macro SearchMiKTeX25

  ;Detect location of MiKTeX installation using initexmf
  
  Push $R0
  
  nsExec::ExecToStack "initexmf.exe --report"
  Pop $R0 ;Return value
  Pop $R0 ;Output
  
  ${WordFind2X} $R0 "BinDir: " "$\r" "+1" $PathLaTeX
  
  Pop $R0

!macroend

!macro SearchMiKTeX24 ROOTKEY

  ReadRegStr $PathLaTeX ${ROOTKEY} "Software\MiK\MiKTeX\CurrentVersion\MiKTeX" "Install Root"
  
  ${if} $PathLaTeX != ""
    !insertmacro callfunc TrimBackslash $PathLaTeX $PathLaTeX ;Just in case it's installed in a root directory
    StrCpy $PathLaTeX "$PathLaTeX\miktex\bin"
  ${endif}
  
!macroend

!macro IfKeyExists ROOT MAIN_KEY KEY

  Push $R0
  Push $R1
 
  !define Index 'Line${__LINE__}'
 
  StrCpy $R1 0
 
  "${Index}-Loop:"
  ;Check for key
  EnumRegKey $R0 ${ROOT} "${MAIN_KEY}" "$R1"
  StrCmp $R0 "" "${Index}-False"
  IntOp $R1 $R1 + 1
  StrCmp $R0 "${KEY}" "${Index}-True" "${Index}-Loop"
 
  "${Index}-True:"
  ;Found
  Push ${TRUE}
  Goto "${Index}-End"
 
  "${Index}-False:"
  ;Not found
  Push ${FALSE}
  goto "${Index}-End"
 
  "${Index}-End:"
  !undef Index
  
  Exch 2
  Pop $R0
  Pop $R1

!macroend

;--------------------------------
;Functions

Function SearchAll

  Call SearchLaTeX
  Call SearchGhostscript
  Call SearchImageMagick
  
  IfSilent +2
    Call SearchViewer

FunctionEnd

Function TrimBackslash

  ;Trim a trailing backslash of a directory

  Exch $R0
  Push $R1
  
  StrCpy $R1 $R0 1 -1
  
  ${if} $R1 == "\"
    StrLen $R1 $R0
    IntOp $R1 $R1 - 1
    StrCpy $R0 $R0 $R1
  ${endif}
  
  Pop $R1
  Exch $R0
  
FunctionEnd

Function GetPathPrefix

  Push $R0

  StrCpy $R0 "$INSTDIR\bin;$INSTDIR\python"
  
  ${if} $PathLaTeX != ""
    StrCpy $R0 "$R0;$PathLaTeX"
  ${endif}
  
  ${if} $PathGhostscript != ""
    StrCpy $R0 "$R0;$PathGhostscript"
  ${endif}
  
  ${if} $PathImageMagick != ""
    StrCpy $R0 "$R0;$PathImageMagick"
  ${endif}
  
  Exch $R0
  
FunctionEnd

Function SearchViewer

  Push $R0
  Push $R1

  !insertmacro CallFunc DetectViewerByExtension "pdf" $R0
  !insertmacro CallFunc DetectViewerByExtension "ps" $R1

  StrCpy $PathViewer ""
  
  ${if} $R0 != ""
    ${if} $R1 != ""
      StrCpy $PathViewer "associated"
    ${endif}      
  ${endif}
  
  Pop $R1
  Pop $R0
  
FunctionEnd  

Function DetectViewerByExtension

  ;Input on stack: file extension without dot

  Exch $R0
  Push $R1
  Push $R2
  
  InitPluginsDir
  
  StrCpy $R1 "$PLUGINSDIR\ViewerDetect.$R0"
  FileOpen $R2 $R1 w
  FileClose $R2
  
  StrCpy $R0 ""

  System::Call "shell32::FindExecutableA(t R1, n, t .R0)"
  
  Delete $R1
  
  Pop $R2
  Pop $R1
  Exch $R0
  
FunctionEnd

Function SearchLaTeX

  ;Search where MikTeX is installed
  
  !insertmacro SearchMiKTeX25
  
  ${unless} ${FileExists} "$PathLaTeX\${BIN_LATEX}"
    !insertmacro SearchMiKTeX24 HKCU
  ${endif}

  ${unless} ${FileExists} "$PathLaTeX\${BIN_LATEX}"
    !insertmacro SearchMiKTeX24 HKLM
  ${endif}

  ${unless} ${FileExists} "$PathLaTeX\${BIN_LATEX}"
    StrCpy $PathLatex ""
  ${endif}

FunctionEnd

Function SearchLaTeXLocalRoot

  ;Search for the MikTeX local root
  ;Returns value on stack

  Push $R0
  
  ;MikTeX 2.5
  
  !insertmacro IfKeyExists HKCU "Software\MiKTeX.org\MiKTeX" "2.5"
  Pop $R0
  
  ${if} $R0 == ${FALSE}
    !insertmacro IfKeyExists HKLM "Software\MiKTeX.org\MiKTeX" "2.5"
    Pop $R0
  ${endif}
  
  ${if} $R0 == ${TRUE}
    StrCpy $R0 "$APPDATA\MiKTeX\2.5"
  ${else}
  
    ;MiKTeX 2.4
    
    ReadRegStr $R0 HKCU "Software\MiK\MiKTeX\CurrentVersion\MiKTeX" "Local Root"
    
    ${unless} ${FileExists} $R0
      ReadRegStr $R0 HKLM "Software\MiK\MiKTeX\CurrentVersion\MiKTeX" "Local Root"
    ${endif}
    
  ${endif}
  
  Exch $R0

FunctionEnd

Function SearchGhostscript

  ;Search where Ghostscript is installed
  
  Push $R0 ;Temp
  Push $R1 ;Counter
  Push $R2 ;Enum return
  Push $R3 ;AFPL version
  Push $R4 ;GPL version

  StrCpy $R3 ""
  StrCpy $R4 ""

  ;Check the latest version of AFPL Ghostscript installed
  
  StrCpy $R1 0
  
  ${do}
  
    EnumRegKey $R2 HKLM "Software\AFPL Ghostscript" $R1
    
    ${if} $R2 != ""
      ${VersionCompare} $R2 $R3 $R0
      ${if} $R0 == "1"
        StrCpy $R3 $R2
      ${endif}
      IntOp $R1 $R1 + 1
    ${endif}
    
  ${loopuntil} $R2 == ""
    
  ;The same for GPL Ghostscript
    
  StrCpy $R1 0
  
  ${do}

    EnumRegKey $R2 HKLM "Software\GPL Ghostscript" $R1
    
    ${if} $R2 != ""
      ${VersionCompare} $R2 $R4 $R0
      ${if} $R0 == "1"
        StrCpy $R4 $R2
      ${endif}
      IntOp $R1 $R1 + 1
    ${endif}    
  
  ${loopuntil} $R2 == ""  
  
  ;Take the latest one
  ${VersionCompare} $R3 $R4 $R0

  ${if} $R0 == "1"
    ;AFPL is newer
    ReadRegStr $PathGhostscript HKLM "Software\AFPL Ghostscript\$R3" "GS_DLL"
  ${else}
    ;GPL is newer or equal
    ReadRegStr $PathGhostscript HKLM "Software\GPL Ghostscript\$R4" "GS_DLL"
  ${endif}
  
  ;Trim the DLL filename to get the path
  ${GetParent} $PathGhostscript $PathGhostscript
  
  ${unless} ${FileExists} "$PathGhostscript\${BIN_GHOSTSCRIPT}"
    StrCpy $PathGhostscript ""  
  ${endif}
    
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R1
  Pop $R0
  
FunctionEnd

Function SearchImageMagick

  ;Search where ImageMagick is installed
  ReadRegStr $PathImageMagick HKLM "Software\ImageMagick\Current" "BinPath"
  
  ${unless} ${FileExists} "$PathImageMagick\${BIN_IMAGEMAGICK}"
    StrCpy $PathImageMagick ""  
  ${endif}

FunctionEnd