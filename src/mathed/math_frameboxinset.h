// -*- C++ -*-
#ifndef MATH_FRAMEBOXINSET_H
#define MATH_FRAMEBOXINSET_H

#include "math_nestinset.h"


/** Extra nesting
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
*/

class MathFrameboxInset : public MathNestInset {
public:
	///
	MathFrameboxInset();
	///
	MathInset * clone() const;
	///
	Dimension metrics(MetricsInfo & mi) const;
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
