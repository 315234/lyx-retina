// -*- C++ -*-
/**
 * \file InsetMathUnderset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_UNDERSETINSET_H
#define MATH_UNDERSETINSET_H


#include "InsetMathFracBase.h"


namespace lyx {

/// Inset for underset
class InsetMathUnderset : public InsetMathFracBase {
public:
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	bool idxFirst(Cursor & cur) const;
	///
	bool idxLast(Cursor & cur) const;
	///
	bool idxUpDown(Cursor & cur, bool up) const;
	///
	void write(WriteStream & ws) const;
	///
	void normalize(NormalStream & ns) const;
	///
	void validate(LaTeXFeatures & features) const;
private:
	virtual Inset * clone() const;
};



} // namespace lyx
#endif
