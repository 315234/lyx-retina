// -*- C++ -*-
/**
 * \file BufferView_pimpl.h
 * Copyright 2002 the LyX Team
 * Read the file COPYING
 *
 * \author various
 */

#ifndef BUFFERVIEW_PIMPL_H
#define BUFFERVIEW_PIMPL_H

#include "BufferView.h"
#include "frontends/Timeout.h"
#include "frontends/key_state.h"
#include "frontends/LyXKeySym.h"
#include "support/types.h"

#include <boost/scoped_ptr.hpp>
#include <boost/signals/trackable.hpp>


class Change;
class LyXView;
class WorkArea;
class LyXScreen;
class FuncRequest;

///
struct BufferView::Pimpl : public boost::signals::trackable {
	///
	Pimpl(BufferView * i, LyXView * o,
	      int xpos, int ypos, int width, int height);
	///
	Painter & painter() const;
	/// return the screen for this bview
	LyXScreen & screen() const;
	///
	void buffer(Buffer *);
	/// Return true if the cursor was fitted.
	bool fitCursor();
	///
	void redoCurrentBuffer();
	///
	int resizeCurrentBuffer();
	///
	void update();
	//
	void update(LyXText *, BufferView::UpdateCodes);
	/// update the toplevel lyx text
	void update(BufferView::UpdateCodes);
	/**
	 * Repaint pixmap. Used for when we've made a visible
	 * change but don't need the full update() logic
	 */
	void repaint();
	///
	void workAreaResize();
	///
	void updateScrollbar();
	///
	void scrollDocView(int value);
	/**
	 * Wheel mouse scroll, move by multiples of text->defaultRowHeight().
	 */
	void scroll(int lines);
	///
	void workAreaKeyPress(LyXKeySymPtr key, key_modifier::state state);
	///
	void selectionRequested();
	///
	void selectionLost();
	///
	void cursorToggle();
	///
	bool available() const;
	/// get the change at the cursor position
	Change const getCurrentChange();
	///
	void beforeChange(LyXText *);
	///
	void savePosition(unsigned int i);
	///
	void restorePosition(unsigned int i);
	///
	bool isSavedPosition(unsigned int i);
	///
	void switchKeyMap();
	///
	void insetUnlock();
	///
	void showCursor();
	///
	void hideCursor();
	///
	void toggleSelection(bool = true);
	///
	void toggleToggle();
	///
	void center();
	///
	bool insertInset(Inset * inset, string const & lout = string());
	///
	void updateInset(Inset * inset);
	///
	bool dispatch(FuncRequest const & ev);
private:
	/// track changes for the document
	void trackChanges();

	///
	friend class BufferView;

	///
	BufferView * bv_;
	///
	LyXView * owner_;
	///
	Buffer * buffer_;
	///
	boost::scoped_ptr<LyXScreen> screen_;
	///
	boost::scoped_ptr<WorkArea> workarea_;
	///
	Timeout cursor_timeout;
	///
	void stuffClipboard(string const &) const;
	///
	bool using_xterm_cursor;
	///
	struct Position {
		/// Filename
		string filename;
		/// Cursor paragraph Id
		int par_id;
		/// Cursor position
		lyx::pos_type par_pos;
		///
		Position() : par_id(0), par_pos(0) {}
		///
		Position(string const & f, int id, lyx::pos_type pos)
			: filename(f), par_id(id), par_pos(pos) {}
	};
	///
	std::vector<Position> saved_positions;
	/// Get next inset of this class from current cursor position
	Inset * getInsetByCode(Inset::Code code);
	///
	void MenuInsertLyXFile(string const & filen);
	/// our workarea
	WorkArea & workarea() const;
};
#endif // BUFFERVIEW_PIMPL_H
