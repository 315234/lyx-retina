// -*- C++ -*-
/**
 * \file InsetMathBoldSymbol.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_BOLDSYMBOLINSET_H
#define MATH_BOLDSYMBOLINSET_H

#include "InsetMathNest.h"


namespace lyx {


/// Inset for AMSTeX's \boldsymbol
class InsetMathBoldSymbol : public InsetMathNest {
public:
	enum Kind {
		BOLD,
		HEAVY
	};
	///
	InsetMathBoldSymbol(Kind kind = BOLD);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter & pi, int x, int y) const;
	///
	void validate(LaTeXFeatures & features) const;
	///
	void write(WriteStream & os) const;
	///
	void infoize(odocstream & os) const;
	///
	Kind kind_;
private:
	virtual Inset * clone() const;
};


} // namespace lyx

#endif
