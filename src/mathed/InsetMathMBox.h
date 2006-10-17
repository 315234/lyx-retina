// -*- C++ -*-
/**
 * \file InsetMathMBox.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_MBOXINSET_H
#define MATH_MBOXINSET_H

#include "InsetMathDim.h"
#include "lyxtext.h"


// almost a substitute for the real text inset...

class InsetMathMBox : public InsetMathDim {
public:
	///
	explicit InsetMathMBox(BufferView & bv);
	/// this stores metrics information in cache_
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	/// draw according to cached metrics
	void draw(PainterInfo &, int x, int y) const;
	///
	void drawSelection(PainterInfo & pi, int x, int y) const;
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
	void cursorPos(BufferView const & bv, CursorSlice const & sl,
		bool boundary, int & x, int & y) const;
protected:
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);

	///
	mutable LyXText text_;
	///
	BufferView * const bv_;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};

#endif
