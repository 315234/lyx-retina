// -*- C++ -*-
/**
 * \file InsetMathRoot.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_ROOT_H
#define MATH_ROOT_H

#include "InsetMathNest.h"


namespace lyx {


/// The general n-th root inset.
class InsetMathRoot : public InsetMathNest {
public:
	///
	InsetMathRoot();
	///
	bool idxUpDown(LCursor & cur, bool up) const;
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
	///
	void mathmlize(MathStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void octave(OctaveStream &) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};



} // namespace lyx
#endif
