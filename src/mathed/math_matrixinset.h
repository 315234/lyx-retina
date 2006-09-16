// -*- C++ -*-
/**
 * \file math_matrixinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_MATRIXINSET_H
#define MATH_MATRIXINSET_H

#include "math_gridinset.h"


// "shortcut" for DelimInset("(",ArrayInset,")")

class InsetMathMatrix : public InsetMathGrid {
public:
	///
	explicit InsetMathMatrix(InsetMathGrid const &);
	///
	explicit InsetMathMatrix(std::string const & str);
	/// identifies MatrixInsets
	InsetMathMatrix const * asMatrixInset() const { return this; }

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void maxima(MaximaStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void octave(OctaveStream &) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};

#endif
