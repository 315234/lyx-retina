// -*- C++ -*-
/**
 * \file updatableinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef UPDATABLEINSET_H
#define UPDATABLEINSET_H

//  Updatable Insets. These insets can receive directly user interaction.
//  Note that all pure methods from Inset class are pure here too.
//  [Alejandro 080596]

#include "inset.h"

class UpdatableInset : public InsetOld {
public:
	///
	UpdatableInset() : scx(0) {}
	///
	virtual EDITABLE editable() const;
	/// identification as text inset in a cursor slice
	UpdatableInset * asUpdatableInset() { return this; }

	/// return the cursor dim
	virtual void getCursorDim(int &, int &) const;
	///
	virtual bool showInsetDialog(BufferView *) const { return false; }

protected:
	///  An updatable inset could handle lyx editing commands
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);
	/// scrolls to absolute position in maxwidth * sx units
	void setScroll(int maxwidth, double sx) const;
	/// scrolls offset pixels
	void setScroll(int maxwidth, int offset) const;
	/// returns the actual scroll value
	virtual int scroll(bool recursive = true) const;
	///
	mutable int scx;
};

#endif
