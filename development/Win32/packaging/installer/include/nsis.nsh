/*

nsis.nsh

Configuration of standard NSIS header files

*/

#--------------------------------
# Multi-User settings

!define MULTIUSER_EXECUTIONLEVEL Highest
!define MULTIUSER_INSTALLMODE_COMMANDLINE
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_KEY "${APP_REGKEY}"
!define MULTIUSER_INSTALLMODE_DEFAULT_REGISTRY_VALUENAME ""

!define MULTIUSER_INSTALLMODE_INSTDIR "${APP_DIR}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_KEY "${APP_REGKEY}"
!define MULTIUSER_INSTALLMODE_INSTDIR_REGISTRY_VALUENAME ""

!define MULTIUSER_INSTALLMODE_FUNCTION InitUser
!define MULTIUSER_MUI

#--------------------------------
# Standard header files

!include MUI2.nsh
!include LogicLib.nsh
!include StrFunc.nsh
!include FileFunc.nsh
!include WordFunc.nsh
!include Sections.nsh
!include WinVer.nsh
!include LangFile.nsh
!include MultiUser.nsh
!include nsDialogs.nsh

#--------------------------------
# Include standard functions

!insertmacro RefreshShellIcons
!insertmacro WordFind2X
