// -*- C++ -*-
/**
 * \file math_stackrelinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_STACKRELINSET_H
#define MATH_STACKRELINSET_H

#include "math_fracbase.h"


/** Stackrel objects
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */
class InsetMathStackrel : public InsetMathFracBase {
public:
	///
	InsetMathStackrel();
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};

#endif
