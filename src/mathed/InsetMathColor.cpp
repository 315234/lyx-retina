/**
 * \file InsetMathColor.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathColor.h"
#include "MathData.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "LaTeXFeatures.h"

#include "support/std_ostream.h"


namespace lyx {

using std::auto_ptr;
using std::string;

InsetMathColor::InsetMathColor(bool oldstyle, Color_color const & color)
	: InsetMathNest(1), oldstyle_(oldstyle),
	  color_(from_utf8(lcolor.getLaTeXName(color)))
{}


InsetMathColor::InsetMathColor(bool oldstyle, docstring const & color)
	: InsetMathNest(1), oldstyle_(oldstyle), color_(color)
{}


auto_ptr<Inset> InsetMathColor::doClone() const
{
	return auto_ptr<Inset>(new InsetMathColor(*this));
}


bool InsetMathColor::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(0).metrics(mi, dim);
	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathColor::draw(PainterInfo & pi, int x, int y) const
{
	Color_color origcol = pi.base.font.color();
	pi.base.font.setColor(lcolor.getFromLaTeXName(to_utf8(color_)));
	cell(0).draw(pi, x + 1, y);
	pi.base.font.setColor(origcol);
	drawMarkers(pi, x, y);
	setPosCache(pi, x, y);
}


/// color "none" (reset to default) needs special treatment
static bool normalcolor(docstring const & color)
{
	return color == "none";
}


void InsetMathColor::validate(LaTeXFeatures & features) const
{
	InsetMathNest::validate(features);
	if (!normalcolor(color_))
		features.require("color");
}


void InsetMathColor::write(WriteStream & os) const
{
	if (normalcolor(color_))
		// reset to default color inside another color inset
		os << "{\\normalcolor " << cell(0) << '}';
	else if (oldstyle_)
		os << "{\\color{" << color_ << '}' << cell(0) << '}';
	else
		os << "\\textcolor{" << color_ << "}{" << cell(0) << '}';
}


void InsetMathColor::normalize(NormalStream & os) const
{
	os << "[color " << color_ << ' ' << cell(0) << ']';
}


void InsetMathColor::infoize(odocstream & os) const
{
	os << "Color: " << color_;
}


} // namespace lyx
