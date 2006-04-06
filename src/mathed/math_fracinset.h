// -*- C++ -*-
/**
 * \file math_fracinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_FRACINSET_H
#define MATH_FRACINSET_H

#include "math_fracbase.h"


/// Fraction like objects (frac, binom)
class MathFracInset : public MathFracbaseInset {
public:
	///
	enum Kind {
		FRAC,
		ATOP,
		NICEFRAC
	};

	///
	explicit MathFracInset(Kind kind = FRAC);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter &, int x, int y) const;
	/// identifies FracInsets
	MathFracInset * asFracInset();
	/// identifies FracInsets
	MathFracInset const * asFracInset() const;
	///
	std::string name() const;

	///
	void write(WriteStream & os) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void validate(LaTeXFeatures & features) const;
public:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	Kind kind_;
};

#endif
