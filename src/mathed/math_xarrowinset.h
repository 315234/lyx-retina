// -*- C++ -*-
#ifndef MATH_XARROWINSET_H
#define MATH_XARROWINSET_H

#include "math_fracbase.h"
#include "LString.h"


/** Wide arrows like \xrightarrow
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
 */

class MathXArrowInset : public MathFracbaseInset {
public:
	///
	explicit MathXArrowInset(string const & name);
	///
	MathInset * clone() const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void normalize(NormalStream & os) const;

private:
	///
	bool upper() const;
	///
	string const name_;
};
#endif
