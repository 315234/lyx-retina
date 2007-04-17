// -*- C++ -*-
/**
 * \file InsetMathBrace.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_BRACEINSET_H
#define MATH_BRACEINSET_H

#include "InsetMathNest.h"


namespace lyx {


/// Extra nesting
class InsetMathBrace : public InsetMathNest {
public:
	///
	InsetMathBrace();
	///
	InsetMathBrace(MathArray const & ar);
	/// we write extra braces in any case...
	bool extraBraces() const { return true; }
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	void write(WriteStream & os) const;
	/// write normalized content
	void normalize(NormalStream & ns) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void mathmlize(MathStream &) const;
	///
	void infoize(odocstream & os) const;
	
	/// identifies brace insets
	InsetMathBrace * asBraceInset() { return this; }
	/// identifies brace insets
	InsetMathBrace const * asBraceInset() const { return this; }
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};


} // namespace lyx

#endif
