// -*- C++ -*-
/**
 * \file math_mboxinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_MBOXINSET_H
#define MATH_MBOXINSET_H

#include "math_diminset.h"
#include "lyxtext.h"


// almost a substitute for the real text inset...

class MathMBoxInset : public MathDimInset {
public:
	///
	MathMBoxInset(BufferView & bv);
	///
	std::auto_ptr<InsetBase> clone() const;
	/// this stores metrics information in cache_
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	/// draw according to cached metrics
	void draw(PainterInfo &, int x, int y) const;
	///
	void priv_dispatch(LCursor & cur, FuncRequest & cmd);

	///
	bool inMathed() const { return false; }
	///
	bool isActive() const { return true; }

	///
	void write(WriteStream & os) const;
	///
	int latex(Buffer const &, std::ostream & os,
			OutputParams const & runparams) const;
	///
	LyXText * getText(int) const;
	///
	void getCursorPos(CursorSlice const & cur, int & x, int & y) const;
protected:
	///
	mutable LyXText text_;
	///
	BufferView * const bv_;
};

#endif
