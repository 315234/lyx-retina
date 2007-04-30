// -*- C++ -*-
/**
 * \file InsetMathDots.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_DOTSINSET_H
#define MATH_DOTSINSET_H

#include "InsetMath.h"


namespace lyx {

class latexkeys;

/// The different kinds of ellipsis
class InsetMathDots : public InsetMath {
public:
	///
	explicit InsetMathDots(latexkeys const * l);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	docstring name() const;
protected:
	/// cache for the thing's height
	mutable int dh_;
	///
	latexkeys const * key_;
private:
	virtual std::auto_ptr<Inset> doClone() const;
};

} // namespace lyx

#endif
