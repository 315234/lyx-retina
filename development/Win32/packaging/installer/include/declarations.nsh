/*

declaration.nsh

Configuration of LyX installer

*/

SetCompressor /SOLID lzma

#--------------------------------
# File locations

!define FILES_LICENSE "license.rtf"
!define FILES_ICONS "..\icons"
!define FILES_PDFVIEW "${FILES_DEPS}\bin"
!define FILES_MSVC "${FILES_DEPS}\bin"
!define FILES_PYTHON "${FILES_DEPS}\python"
!define FILES_IMAGEMAGICK "${FILES_DEPS}\imagemagick"
!define FILES_GHOSTSCRIPT "${FILES_DEPS}\ghostscript"
!define FILES_NETPBM "${FILES_DEPS}\bin"
!define FILES_DTL "${FILES_DEPS}\bin"
!define FILES_DVIPOST "${FILES_DEPS}\bin"
!define FILES_DVIPOST_PKG "${FILES_DEPS}\tex"
!define FILES_PDFTOOLS "${FILES_DEPS}\bin"
!define FILES_METAFILE2EPS "${FILES_DEPS}\bin"

#--------------------------------
# Locations of components to download

# CTAN and SourceForge select a mirror automatically

!define DOWNLOAD_LATEX "http://mirrors.ctan.org/systems/win32/miktex/setup/${SETUPFILE_LATEX}"

#--------------------------------
# Locations of setup files for components (for bundled setup)

!define INSTALL_LATEX "${SETUPFILE_LATEX}"

#--------------------------------
# Names and version

!define APP_NAME "LyX"
!define APP_VERSION_NUMBER "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_REVISION}.${APP_VERSION_BUILD}"
!define APP_SERIES_NAME "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}"
!define APP_SERIES_KEY "${APP_VERSION_MAJOR}${APP_VERSION_MINOR}"
!define APP_DIR "${APP_NAME}${APP_SERIES_KEY}"
!define APP_DIR_USERDATA "${APP_NAME}${APP_SERIES_KEY}"
!define APP_INFO "${APP_NAME} - The Document Processor"
!define APP_COPYRIGHT "LyX is Copyright � 1995 by Matthias Ettrich, 1995-2011 LyX Team"

!define APP_RUN "bin\LyX.exe"

!define APP_REGKEY "Software\${APP_NAME}${APP_SERIES_KEY}"
!define APP_REGKEY_SETUP "Software\${APP_NAME}${APP_SERIES_KEY}\Setup"
!define APP_REGKEY_SETTINGS "Software\${APP_NAME}${APP_SERIES_KEY}\Settings"

!define APP_REGNAME_DOC "LyX.Document"

!define APP_EXT ".lyx"
!define APP_MIME_TYPE "application/lyx"

#--------------------------------
# Setup settings

# Output file name can be configured using command line paramaters like
# /DExeFile=/path/to/installer

!ifndef SETUPTYPE
  !define SETUPTYPE STANDARD
!endif
 
!if ${SETUPTYPE} == STANDARD

  !define SETUP_EXE ${ExeFile}

!else if ${SETUPTYPE} == BUNDLE

  !define SETUP_EXE ${BundleExeFile}
  !define BUNDLESETUP_LATEX

!endif

!define SETUP_ICON "${FILES_ICONS}\lyx.ico"
!define SETUP_HEADERIMAGE "graphics\header.bmp"
!define SETUP_WIZARDIMAGE "graphics\wizard.bmp"
!define SETUP_UNINSTALLER "Uninstall-${APP_NAME}.exe"
!define SETUP_UNINSTALLER_KEY "${APP_NAME}"

#--------------------------------
# Names of binaries to identify compontents

!define BIN_LATEX "tex.exe"
!define BIN_BIBTEXEDITOR "JabRef.exe"

#--------------------------------
# Custom NSIS plug-ins

!addplugindir "${FILES_NSISPLUGINS}"
