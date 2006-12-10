// -*- C++ -*-
/**
 * \file lyxfind.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 * \author J�rgen Vigna
 * \author Alfredo Braunstein
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYXFIND_H
#define LYXFIND_H

#include "support/docstring.h"

namespace lyx {

class BufferView;
class FuncRequest;
class LyXText;

/** Encode the parameters needed to find \c search as a string
 *  that can be dispatched to the LyX core in a FuncRequest wrapper.
 */
docstring const find2string(docstring const & search,
			      bool casesensitive,
			      bool matchword,
			      bool forward);

/** Encode the parameters needed to replace \c search with \c replace
 *  as a string that can be dispatched to the LyX core in a FuncRequest
 *  wrapper.
 */
docstring const replace2string(docstring const & search,
				 docstring const & replace,
				 bool casesensitive,
				 bool matchword,
				 bool all,
				 bool forward);

/** Parse the string encoding of the find request that is found in
 *  \c ev.argument and act on it.
 * The string is encoded by \c find2string.
 */
void find(BufferView * bv, FuncRequest const & ev);

/** Parse the string encoding of the replace request that is found in
 *  \c ev.argument and act on it.
 * The string is encoded by \c replace2string.
 */
void replace(BufferView * bv, FuncRequest const &);

/// find the next change in the buffer
bool findNextChange(BufferView * bv);

} // namespace lyx

#endif // LYXFIND_H
