// -*- C++ -*-
/**
 * \file InsetMathTabular.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_TABULARINSET_H
#define MATH_TABULARINSET_H

#include "InsetMathGrid.h"


namespace lyx {


/// Inset for things like \begin{tabular}...\end{tabular}
class InsetMathTabular : public InsetMathGrid {
public:
	///
	InsetMathTabular(docstring const &, int m, int n);
	///
	InsetMathTabular(docstring const &, int m, int n,
		char valign, docstring const & halign);
	///
	InsetMathTabular(docstring const &, char valign, docstring const & halign);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	InsetMathTabular * asTabularInset() { return this; }
	///
	InsetMathTabular const * asTabularInset() const { return this; }

	///
	void write(WriteStream & os) const;
	///
	void infoize(odocstream & os) const;
	///
	void normalize(NormalStream &) const;
	///
	void maple(MapleStream &) const;

private:
	virtual Inset * clone() const;
	///
	docstring name_;
};

} // namespace lyx

#endif
