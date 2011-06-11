/*

NSIS Script - LyX 2.0 Installer for Win32
Authors: Joost Verburg, Angus Leeming, Uwe St�hr
Compatible with NSIS 2.46

*/

CRCCheck force
SetCompressor /SOLID lzma

# installer settings like version numbers
!include settings.nsh 

# declarations of LyX's registry keys and installer variant
!include include\declarations.nsh

# definitions of global variables
!include include\variables.nsh

# configuration of standard NSIS header files
!include include\nsis.nsh

# list of files to be included in the installer
!include include\filelist.nsh

# detect third-party programs like Inkscape and LaTeX
!include include\detection.nsh

# set up the installer pages
!include include\gui.nsh

# 
!include include\init.nsh

# install LyX and needed third-party programs like Python etc.
!include setup\install.nsh

# uninstall LyX and all programs that were installed together with LyX
!include setup\uninstall.nsh

# configure LyX (set start menu and write registry entries)
!include setup\configure.nsh

# 
!include gui\external.nsh

#--------------------------------
# Output file

Outfile "${SETUP_EXE}"
