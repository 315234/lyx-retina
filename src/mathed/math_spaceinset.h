// -*- C++ -*-
/**
 * \file math_spaceinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_SPACEINSET_H
#define MATH_SPACEINSET_H

#include "math_inset.h"


/// Smart spaces
class InsetMathSpace : public InsetMath {
public:
	///
	explicit InsetMathSpace(int sp);
	///
	explicit InsetMathSpace(std::string const & name);
	///
	InsetMathSpace const * asSpaceInset() const { return this; }
	///
	InsetMathSpace * asSpaceInset() { return this; }
	///
	void incSpace();
	///
	int ascent() const;
	///
	int descent() const;
	///
	int width() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;

	///
	void normalize(NormalStream &) const;
	///
	void validate(LaTeXFeatures & features) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void write(WriteStream & os) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	int space_;
};
#endif
