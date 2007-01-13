// -*- C++ -*-
/**
 * \file Clipboard.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BASE_CLIPBOARD_H
#define BASE_CLIPBOARD_H

#include "support/docstring.h"

namespace lyx {
namespace frontend {

/**
 * A Clipboard class manages the clipboard.
 */
class Clipboard
{
public:
	virtual ~Clipboard() {}

	/**
	 * Get the system clipboard contents. The format is as written in
	 * .lyx files (may even be an older version than ours if it comes
	 * from an older LyX).
	 * Does not convert plain text to LyX if only plain text is available.
	 * This should be called when the user requests to paste from the
	 * clipboard.
	 */
	virtual std::string const getAsLyX() const = 0;
	/// Get the contents of the window system clipboard in plain text format.
	virtual docstring const getAsText() const = 0;
	/**
	 * Fill the system clipboard. The format of \p lyx is as written in
	 * .lyx files, the format of \p text is plain text.
	 * We put the clipboard contents in LyX format and plain text into
	 * the system clipboard if supported, so that it is useful for other
	 * applications as well as other instances of LyX.
	 * This should be called when the user requests to cut or copy to
	 * the clipboard.
	 */
	virtual void put(std::string const & lyx, docstring const & text) = 0;

	/// Does the clipboard contain LyX contents?
	virtual bool hasLyXContents() const = 0;
	/// state of clipboard.
	/// \retval true if the system clipboard has been set within LyX.
	virtual bool isInternal() const = 0;
	/// Is the clipboard empty?
	virtual bool empty() const = 0;
};

} // namespace frontend

/// Implementation is in Application.C
extern frontend::Clipboard & theClipboard();

} // namespace lyx


#endif // BASE_CLIPBOARD_H
