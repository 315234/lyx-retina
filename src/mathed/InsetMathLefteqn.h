// -*- C++ -*-
/**
 * \file InsetMathLefteqn.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_LEFTEQNINSET_H
#define MATH_LEFTEQNINSET_H

#include "InsetMathNest.h"


namespace lyx {


/// Support for LaTeX's \\lefteqn command

class InsetMathLefteqn : public InsetMathNest {
public:
	///
	InsetMathLefteqn();
	///
	docstring name() const;
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void infoize(odocstream & os) const;
private:
	virtual std::auto_ptr<Inset> doClone() const;
};


} // namespace lyx
#endif
