// -*- C++ -*-
/**
 * \file InsetMathNumber.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_NUMBERINSET_H
#define MATH_NUMBERINSET_H

#include "InsetMath.h"


/** Some inset that "is" a number
 *  mainly for math-extern
 */
class InsetMathNumber : public InsetMath {
public:
	///
	explicit InsetMathNumber(std::string const & s);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	std::string str() const { return str_; }
	///
	InsetMathNumber * asNumberInset() { return this; }

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
	virtual std::auto_ptr<InsetBase> doClone() const;
	/// the number as string
	std::string str_;
};
#endif
