/**
 * \file InsetMathStackrel.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathStackrel.h"

#include "LaTeXFeatures.h"
#include "MathData.h"
#include "MathStream.h"

using namespace std;

namespace lyx {

InsetMathStackrel::InsetMathStackrel(Buffer * buf) : InsetMathFracBase(buf)
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
	dim.wid = max(dim0.width(), dim1.width()) + 4;
	dim.asc = dim1.ascent() + dim0.height() + 4;
	dim.des = dim1.descent();
	metricsMarkers(dim);
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
	MathEnsurer ensurer(os);
	os << "\\stackrel{" << cell(0) << "}{" << cell(1) << '}';
}


void InsetMathStackrel::normalize(NormalStream & os) const
{
	os << "[stackrel " << cell(0) << ' ' << cell(1) << ']';
}


void InsetMathStackrel::mathmlize(MathStream & ms) const
{
	ms << "<mover accent='false'>" << cell(1) << cell(0) << "</mover>";
}


void InsetMathStackrel::htmlize(HtmlStream & os) const
{
	// at the moment, this is exactly the same as overset
	os << MTag("span", "class='overset'")
		 << MTag("span", "class='top'") << cell(0) << ETag("span")
		 << MTag("span") << cell(1) << ETag("span")
		 << ETag("span");
}


void InsetMathStackrel::validate(LaTeXFeatures & features) const
{
	// from overset
	if (features.runparams().math_flavor == OutputParams::MathAsHTML)
		features.addPreambleSnippet("<style type=\"text/css\">\n"
			"span.overset{display: inline-block; vertical-align: bottom; text-align:center;}\n"
			"span.overset span {display: block;}\n"
			"span.top{font-size: 66%;}\n"
			"</style>");

	InsetMathNest::validate(features);
}

} // namespace lyx
