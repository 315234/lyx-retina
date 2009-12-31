// -*- C++ -*-
/**
 * \file InsetMathEnsureMath.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 * \author Enrico Forestieri
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_ENSUREMATHINSET_H
#define MATH_ENSUREMATHINSET_H

#include "InsetMathNest.h"


namespace lyx {


/// Inset for ensuring math mode
class InsetMathEnsureMath : public InsetMathNest {
public:
	InsetMathEnsureMath(Buffer * buf);
	///
	mode_type currentMode() const { return MATH_MODE; }
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void mathmlize(MathStream &) const;
	///
	void infoize(odocstream & os) const;
	///
	InsetCode lyxCode() const { return MATH_ENSUREMATH_CODE; }
private:
	virtual Inset * clone() const;
};


} // namespace lyx

#endif
