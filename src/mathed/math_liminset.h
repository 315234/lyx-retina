// -*- C++ -*-
#ifndef MATH_LIMINSET_H
#define MATH_LIMINSET_H


// lim_{x->x0} f(x) in one block
// for interfacing external programs

#include "math_nestinset.h"

class MathLimInset : public MathNestInset {
public:
	///
	MathLimInset(MathArray const & f, MathArray const & x, MathArray const & x0);
	///
	MathInset * clone() const;
	///
	void metrics(MathMetricsInfo & mi) const;
	///
	void draw(MathPainterInfo & pi, int x, int y) const;

	///
	void normalize(NormalStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void write(WriteStream & os) const;
};

#endif
