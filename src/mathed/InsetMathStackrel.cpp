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


using std::max;
using std::auto_ptr;


InsetMathStackrel::InsetMathStackrel()
{}


auto_ptr<InsetBase> InsetMathStackrel::doClone() const
{
	return auto_ptr<InsetBase>(new InsetMathStackrel(*this));
}


bool InsetMathStackrel::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(1).metrics(mi);
	FracChanger dummy(mi.base);
	cell(0).metrics(mi);
	dim.wid = max(cell(0).width(), cell(1).width()) + 4;
	dim.asc = cell(1).ascent() + cell(0).height() + 4;
	dim.des = cell(1).descent();
	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathStackrel::draw(PainterInfo & pi, int x, int y) const
{
	int m  = x + dim_.width() / 2;
	int yo = y - cell(1).ascent() - cell(0).descent() - 1;
	cell(1).draw(pi, m - cell(1).width() / 2, y);
	FracChanger dummy(pi.base);
	cell(0).draw(pi, m - cell(0).width() / 2, yo);
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
