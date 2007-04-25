/**
 * \file InsetMathXArrow.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathXArrow.h"
#include "MathArray.h"
#include "MathStream.h"
#include "MathStream.h"
#include "MathSupport.h"

#include "LaTeXFeatures.h"


namespace lyx {

using std::string;
using std::auto_ptr;


InsetMathXArrow::InsetMathXArrow(docstring const & name)
	: InsetMathFracBase(), name_(name)
{}


auto_ptr<InsetBase> InsetMathXArrow::doClone() const
{
	return auto_ptr<InsetBase>(new InsetMathXArrow(*this));
}


bool InsetMathXArrow::metrics(MetricsInfo & mi, Dimension & dim) const
{
	ScriptChanger dummy(mi.base);
	cell(0).metrics(mi);
	cell(1).metrics(mi);
	dim.wid = std::max(cell(0).width(), cell(1).width()) + 10;
	dim.asc = cell(0).height() + 10;
	dim.des = cell(1).height();
	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathXArrow::draw(PainterInfo & pi, int x, int y) const
{
	ScriptChanger dummy(pi.base);
	cell(0).draw(pi, x + 5, y - 10);
	cell(1).draw(pi, x + 5, y + cell(1).height());
	mathed_draw_deco(pi, x + 1, y - 7, width() - 2, 5, name_);
	drawMarkers(pi, x, y);
}


void InsetMathXArrow::write(WriteStream & os) const
{
	os << '\\' << name_;
	if (cell(1).size())
		os << '[' << cell(1) << ']';
	os << '{' << cell(0) << '}';
}


void InsetMathXArrow::normalize(NormalStream & os) const
{
	os << "[xarrow " << name_ << ' ' <<  cell(0) << ' ' << cell(1) << ']';
}


void InsetMathXArrow::validate(LaTeXFeatures & features) const
{
	features.require("amsmath");
	InsetMathNest::validate(features);
}


} // namespace lyx
