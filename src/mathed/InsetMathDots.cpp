/**
 * \file InsetMathDots.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathDots.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "MathParser.h"

#include "frontends/FontMetrics.h"


namespace lyx {

InsetMathDots::InsetMathDots(latexkeys const * key)
	: key_(key)
{}


Inset * InsetMathDots::clone() const
{
	return new InsetMathDots(*this);
}


bool InsetMathDots::metrics(MetricsInfo & mi, Dimension & dim) const
{
	dim = theFontMetrics(mi.base.font).dimension('M');
	dh_ = 0;
	if (key_->name == "cdots" || key_->name == "dotsb"
			|| key_->name == "dotsm" || key_->name == "dotsi")
		dh_ = dim.asc / 2;
	else if (key_->name == "dotsc")
		dh_ = dim.asc / 4;
	else if (key_->name == "vdots") {
		dim.wid = (dim.wid / 2) + 1;
		dh_ = dim.asc;
	}
	else if (key_->name == "ddots")
		dh_ = dim.asc;
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathDots::draw(PainterInfo & pain, int x, int y) const
{
	mathed_draw_deco(pain, x + 2, y - dh_, dim_.width() - 2, dim_.ascent(),
		key_->name);
	if (key_->name == "vdots" || key_->name == "ddots")
		++x;
	if (key_->name != "vdots")
		--y;
	mathed_draw_deco(pain, x + 2, y - dh_, dim_.width() - 2, dim_.ascent(),
		key_->name);
	setPosCache(pain, x, y);
}


docstring InsetMathDots::name() const
{
	return key_->name;
}


} // namespace lyx
