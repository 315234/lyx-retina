// -*- C++ -*-
/**
 * \file text_funcs.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 *
 * This file contains some utility functions for actually mutating
 * the text contents of a document
 */

#ifndef TEXT_FUNCS_H
#define TEXT_FUNCS_H

#include "ParagraphList_fwd.h"
#include "support/types.h"

class LyXCursor;
class LyXText;


// do no use LyXText or BufferView here


///
bool transposeChars(LyXText &, LyXCursor const & cursor);
///
void cursorLeftOneWord(LyXText &, LyXCursor &, ParagraphList const &);
///
void cursorRightOneWord(LyXText &, LyXCursor &, ParagraphList const &);

// Select current word. This depends on behaviour of
// CursorLeftOneWord(), so it is patched as well.
void getWord(LyXText &, LyXCursor & from, LyXCursor & to, lyx::word_location const loc,
	ParagraphList const & pars);

#endif // TEXT_FUNCS_H
