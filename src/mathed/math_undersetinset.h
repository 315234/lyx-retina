// -*- C++ -*-

/** Underset objects
 *  \author Andr� P�nitz
 *
 *  Full author contact details are available in file CREDITS
 */

#ifndef MATH_UNDERSETINSET_H
#define MATH_UNDERSETINSET_H


#include "math_fracbase.h"

/// Inset for underset
class MathUndersetInset : public MathFracbaseInset {
public:
	///
	MathUndersetInset();
	///
	MathInset * clone() const;
	///
	void metrics(MetricsInfo & st) const;
	///
	void draw(PainterInfo &, int x, int y) const;

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
};

#endif
