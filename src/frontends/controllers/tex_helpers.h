// -*- C++ -*-
/**
 * \file tex_helpers.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Herbert Voss
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TEX_HELPERS_H
#define TEX_HELPERS_H


#include "LString.h"
#include <utility>
#include <vector>


// build filelists of all availabe bst/cls/sty-files. done through
// kpsewhich and an external script, saved in *Files.lst
void rescanTexStyles();

/// rebuild the textree
void texhash();

/** Fill \c contents from one of the three texfiles.
 *  Each entry in the file list is returned as a name_with_path
 */
void getTexFileList(string const & filename, std::vector<string> & contents);

/// get the options of stylefile
string const getListOfOptions(string const & classname, string const & type);

/// get a class with full path from the list
string const getTexFileFromList(string const & classname, string const & type);

#endif // TEX_HELPERS_H
