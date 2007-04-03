// -*- C++ -*-
/**
 * \file lyx_cb.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_CB_H
#define LYX_CB_H

#include "support/docstring.h"

namespace lyx {

class Buffer;
class BufferView;
class LyXView;

///
extern bool quitting;

///
bool menuWrite(Buffer * buffer);
/// write the given file, or ask if no name given
bool writeAs(Buffer * buffer, std::string const & filename = std::string());
///
void autoSave(BufferView * bv);
///
void newFile(BufferView * bv, std::string const & filename);
///
void insertPlaintextFile(BufferView * bv, std::string const & f, bool asParagraph);
/// read plain text file (if \p f is empty, prompt for a filename)
docstring const getContentsOfPlaintextFile(BufferView * bv,
		std::string const & f, bool asParagraph);
///
void reconfigure(LyXView & lv);

} // namespace lyx

#endif
