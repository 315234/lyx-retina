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


namespace lyx {


/** Some inset that "is" a number
 *  mainly for math-extern
 */
class InsetMathNumber : public InsetMath {
public:
	///
	explicit InsetMathNumber(docstring const & s);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	docstring str() const { return str_; }
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
	void mathmlize(MathStream &) const;
	///
	void write(WriteStream & os) const;

private:
	virtual std::auto_ptr<Inset> doClone() const;
	/// the number as string
	docstring str_;
};


} // namespace lyx
#endif
