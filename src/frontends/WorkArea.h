// -*- C++ -*-
/**
 * \file WorkArea.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BASE_WORKAREA_H
#define BASE_WORKAREA_H

#include "frontends/KeyModifier.h"
#include "frontends/LyXView.h"

namespace lyx {

class BufferView;
class KeySymbol;

namespace frontend {

/**
 * The work area class represents the widget that provides the
 * view onto a document. It is owned by the BufferView, and
 * is responsible for handing events back to its owning BufferView.
 * It works in concert with the BaseScreen class to update the
 * widget view of a document.
 */
class WorkArea
{
public:
	///
	WorkArea() : lyx_view_(0) {}
	///
	virtual ~WorkArea() {}

	/// redraw the screen, without using existing pixmap
	virtual void redraw() = 0;

	/// close this work area.
	/// Slot for Buffer::closing signal.
	virtual void close() = 0;
	/// This function is called when the buffer readonly status change.
	virtual void setReadOnly(bool) = 0;

	/// Update window titles of all users.
	virtual void updateWindowTitle() = 0;

private:

	LyXView * lyx_view_;
};

} // namespace frontend
} // namespace lyx

#endif // BASE_WORKAREA_H
