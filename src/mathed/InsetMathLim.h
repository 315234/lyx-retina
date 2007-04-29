// -*- C++ -*-
/**
 * \file InsetMathLim.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_LIMINSET_H
#define MATH_LIMINSET_H


// lim_{x->x0} f(x) in one block
// for interfacing external programs

#include "InsetMathNest.h"


namespace lyx {

class InsetMathLim : public InsetMathNest {
public:
	///
	InsetMathLim(MathData const & f, MathData const & x, MathData const & x0);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;

	///
	void normalize(NormalStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void maxima(MaximaStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathStream &) const;
	///
	void write(WriteStream & os) const;
private:
	virtual std::auto_ptr<Inset> doClone() const;
};



} // namespace lyx
#endif
