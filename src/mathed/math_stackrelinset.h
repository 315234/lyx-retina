// -*- C++ -*-
#ifndef MATH_STACKRELINSET_H
#define MATH_STACKRELINSET_H

#include "math_fracbase.h"


/** Stackrel objects
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
 */
class MathStackrelInset : public MathFracbaseInset {
public:
	///
	MathStackrelInset();
	///
	MathInset * clone() const;
	///
	void metrics(MathMetricsInfo & mi) const;
	///
	void draw(MathPainterInfo & pi, int x, int y) const;

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
};

#endif
