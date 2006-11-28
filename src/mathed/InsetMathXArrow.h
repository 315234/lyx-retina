// -*- C++ -*-
/**
 * \file InsetMathXArrow.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_XARROWINSET_H
#define MATH_XARROWINSET_H

#include "InsetMathFracBase.h"


namespace lyx {


/// Wide arrows like \xrightarrow
class InsetMathXArrow : public InsetMathFracBase {
public:
	///
	explicit InsetMathXArrow(docstring const & name);
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void normalize(NormalStream & os) const;
	///
	void validate(LaTeXFeatures & features) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	bool upper() const;
	///
	docstring const name_;
};


} // namespace lyx
#endif
