// -*- C++ -*-
/**
 * \file InsetMathFrac.h
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

#include "InsetMathFracBase.h"


namespace lyx {


/// Fraction like objects (frac, binom)
class InsetMathFrac : public InsetMathFracBase {
public:
	///
	enum Kind {
		FRAC,
		OVER,
		ATOP,
		NICEFRAC
	};

	///
	explicit InsetMathFrac(Kind kind = FRAC);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter &, int x, int y) const;
	/// identifies FracInsets
	InsetMathFrac * asFracInset();
	/// identifies FracInsets
	InsetMathFrac const * asFracInset() const;
	///
	std::string name() const;
	///
	bool extraBraces() const;

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



} // namespace lyx
#endif
