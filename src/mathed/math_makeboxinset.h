// -*- C++ -*-
/**
 * \file math_makeboxinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Ling Li
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_MAKEBOXINSET_H
#define MATH_MAKEBOXINSET_H

#include "math_nestinset.h"

/// Extra nesting: \\makebox.
// consolidate with MathFrameboxInset?

class MathMakeboxInset : public MathNestInset {
public:
	///
	MathMakeboxInset();
	///
	virtual std::auto_ptr<InsetBase> clone() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	/// write normalized content
	void normalize(NormalStream & ns) const;
	///
	mode_type currentMode() const { return TEXT_MODE; }
private:
	/// width of '[' in current font
	mutable int w_;
};

#endif
