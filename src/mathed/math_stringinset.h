// -*- C++ -*-
#ifndef MATH_STRINGINSET_H
#define MATH_STRINGINSET_H

#include "math_diminset.h"


/** Some collection of chars with similar properties
 *  maily for math-extern
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
 */

class MathStringInset : public MathDimInset {
public:
	///
	explicit MathStringInset(string const & s);
	///
	MathInset * clone() const;
	///
	void metrics(MathMetricsInfo & mi) const;
	///
	void draw(MathPainterInfo & pi, int x, int y) const;
	///
	string str() const { return str_; }
	///
	MathStringInset * asStringInset() { return this; }

	///
	void normalize(NormalStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void write(WriteStream & os) const;

private:
	/// the string
	string str_;
};
#endif
