// -*- C++ -*-
/**
 * \file InsetMathArray.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_ARRAYINSET_H
#define MATH_ARRAYINSET_H

#include "InsetMathGrid.h"


namespace lyx {


/// Inset for things like \begin{array}...\end{array}
class InsetMathArray : public InsetMathGrid {
public:
	///
	InsetMathArray(docstring const &, int m, int n);
	///
	InsetMathArray(docstring const &, int m, int n,
		char valign, docstring const & halign);
	///
	InsetMathArray(docstring const &, char valign, docstring const & halign);
	/// convenience constructor from whitespace/newline separated data
	InsetMathArray(docstring const &, docstring const & str);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	InsetMathArray * asArrayInset() { return this; }
	///
	InsetMathArray const * asArrayInset() const { return this; }

	///
	void write(WriteStream & os) const;
	///
	void infoize(odocstream & os) const;
	///
	void normalize(NormalStream & os) const;
	///
	void maple(MapleStream & os) const;
	///
	void validate(LaTeXFeatures & features) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	docstring name_;
};


} // namespace lyx

#endif
