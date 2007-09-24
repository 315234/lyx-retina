/**
 * \file InsetMathStackrel.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathStackrel.h"
#include "MathData.h"
#include "MathStream.h"


namespace lyx {

InsetMathStackrel::InsetMathStackrel()
{}


Inset * InsetMathStackrel::clone() const
{
	return new InsetMathStackrel(*this);
}


void InsetMathStackrel::metrics(MetricsInfo & mi, Dimension & dim) const
{
	Dimension dim1;
	cell(1).metrics(mi, dim1);
	FracChanger dummy(mi.base);
	Dimension dim0;
	cell(0).metrics(mi, dim0);
	dim.wid = std::max(dim0.width(), dim1.width()) + 4;
	dim.asc = dim1.ascent() + dim0.height() + 4;
	dim.des = dim1.descent();
	metricsMarkers(dim);
	// Cache the inset dimension. 
	setDimCache(mi, dim);
}


void InsetMathStackrel::draw(PainterInfo & pi, int x, int y) const
{
	Dimension const dim = dimension(*pi.base.bv);
	Dimension const & dim0 = cell(0).dimension(*pi.base.bv);
	Dimension const & dim1 = cell(1).dimension(*pi.base.bv);
	int m  = x + dim.width() / 2;
	int yo = y - dim1.ascent() - dim0.descent() - 1;
	cell(1).draw(pi, m - dim1.width() / 2, y);
	FracChanger dummy(pi.base);
	cell(0).draw(pi, m - dim0.width() / 2, yo);
	drawMarkers(pi, x, y);
}


void InsetMathStackrel::write(WriteStream & os) const
{
	os << "\\stackrel{" << cell(0) << "}{" << cell(1) << '}';
}


void InsetMathStackrel::normalize(NormalStream & os) const
{
	os << "[stackrel " << cell(0) << ' ' << cell(1) << ']';
}


} // namespace lyx
