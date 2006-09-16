// -*- C++ -*-
/**
 * \file math_fboxinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_FBOXINSET_H
#define MATH_FBOXINSET_H

#include "math_nestinset.h"


/// Non-AMS-style frame
class InsetMathFBox : public InsetMathNest {
public:
	///
	InsetMathFBox();
	///
	mode_type currentMode() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	/// write normalized content
	void normalize(NormalStream & ns) const;
	///
	void infoize(std::ostream & os) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
};

#endif
