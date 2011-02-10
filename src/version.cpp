/* -*- C++ -*- */
/**
 * \file version.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jean-Marc Lasgouttes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

///
char const * lyx_version = PACKAGE_VERSION;
///
const int lyx_version_major = LYX_MAJOR_VERSION;
const int lyx_version_minor = LYX_MINOR_VERSION;
///
char const * lyx_release_date = LYX_DATE;

/// Package identifier (lyx[-<version-suffix>])
char const * lyx_package = PACKAGE;

/// This is the version information shown by 'lyx --version'
char const * lyx_version_info = VERSION_INFO;
