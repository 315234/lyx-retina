// -*- C++ -*-
/**
 * \file InsetMathTFrac.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_TFRACINSET_H
#define MATH_TFRACINSET_H

#include "InsetMathFrac.h"


namespace lyx {


/// \dfrac support
class InsetMathTFrac : public InsetMathFrac {
public:
	///
	InsetMathTFrac();
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	docstring name() const;
	///
	void mathmlize(MathStream &) const;
	///
	void validate(LaTeXFeatures & features) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};



} // namespace lyx
#endif
