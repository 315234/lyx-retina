// -*- C++ -*-
/**
 * \file lyxcursor.h
 * Copyright 1995-2001 the LyX Team
 * Read the file COPYING
 *
 * \author Matthias Ettrich
 */

#ifndef LYXCURSOR_H
#define LYXCURSOR_H

#include "ParagraphList.h"
#include "support/types.h"

/**
 * The cursor class describes the position of a cursor within a document.
 * Several cursors exist within LyX; for example, when locking an inset,
 * the position of the cursor in the containing inset is stored.
 *
 * FIXME: true ?
 */
class LyXCursor {
public:
	LyXCursor();
	/// set the paragraph that contains this cursor
	void par(ParagraphList::iterator pit);
	/// return the paragraph this cursor is in
	ParagraphList::iterator par() const;
	/// set the position within the paragraph
	void pos(lyx::pos_type p);
	/// return the position within the paragraph
	lyx::pos_type pos() const;
	/// FIXME
	void boundary(bool b);
	/// FIXME
	bool boundary() const;
	/// set the x position in pixels
	void x(int i);
	/// return the x position in pixels
	int x() const;
	/// set the stored next-line position when at the end of a row
	void ix(int i);
	/**
	 * Return the x position of the start of the next row, when this
	 * cursor is at the end of the previous row, for insets that take
	 * a full row.
	 *
	 * FIXME: explain why we need this ?
	 */
	int ix() const;
	/// set the cached x position
	void x_fix(int i);
	/**
	 * Return the cached x position of the cursor. This is used for when
	 * we have text like :
	 *
	 * blah blah blah blah| blah blah blah
	 * blah blah blah
	 * blah blah blah blah blah blah
	 *
	 * When we move onto row 3, we would like to be vertically aligned
	 * with where we were in row 1, despite the fact that row 2 is
	 * shorter than x()
	 */
	int x_fix() const;
	/// set the y position in pixels
	void y(int i);
	/// return the y position in pixels
	int y() const;
	/// set the stored next-line y position when at the end of a row
	void iy(int i);
	/**
	 * Return the y position of the start of the next row, when this
	 * cursor is at the end of the previous row, for insets that take
	 * a full row.
	 *
	 * FIXME: explain why we need this ? especially for y...
	 */
	int iy() const;
private:
	/// The paragraph the cursor is in.
	ParagraphList::iterator par_;
	/// The position inside the paragraph
	lyx::pos_type pos_;
	/**
	 * When the cursor position is i, is the cursor is after the i-th char
	 * or before the i+1-th char ? Normally, these two interpretations are
	 * equivalent, except when the fonts of the i-th and i+1-th char
	 * differ.
	 * We use boundary_ to distinguish between the two options:
	 * If boundary_=true, then the cursor is after the i-th char
	 * and if boundary_=false, then the cursor is before the i+1-th char.
	 *
	 * We currently use the boundary only when the language direction of
	 * the i-th char is different than the one of the i+1-th char.
	 * In this case it is important to distinguish between the two
	 * cursor interpretations, in order to give a reasonable behavior to
	 * the user.
	 */
	bool boundary_;
	/// the pixel x position
	int x_;
	/// the stored next-row x position
	int ix_;
	/// the cached x position
	int x_fix_;
	/// the pixel y position
	int y_;
	/// the stored next-row y position
	int iy_;
};

/// 
bool operator==(LyXCursor const & a, LyXCursor const & b);
///
bool operator!=(LyXCursor const & a, LyXCursor const & b);

#endif // LYXCURSOR_H
