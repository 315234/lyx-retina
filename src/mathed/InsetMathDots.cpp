/**
 * \file InsetMathDots.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathDots.h"

#include "LaTeXFeatures.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "MathParser.h"
#include "MetricsInfo.h"

#include "frontends/FontMetrics.h"
#include "support/lassert.h"

namespace lyx {

InsetMathDots::InsetMathDots(latexkeys const * key)
	: key_(key)
{}


Inset * InsetMathDots::clone() const
{
	return new InsetMathDots(*this);
}


void InsetMathDots::metrics(MetricsInfo & mi, Dimension & dim) const
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
	else if (key_->name == "ddots" || key_->name == "adots" || key_->name == "iddots")
		dh_ = dim.asc;
}


void InsetMathDots::draw(PainterInfo & pain, int x, int y) const
{
	Dimension const dim = dimension(*pain.base.bv);
	if (key_->name == "adots" || key_->name == "iddots")
		--y;
	mathed_draw_deco(pain, x + 2, y - dh_, dim.width() - 2, dim.ascent(),
			key_->name);
	if (key_->name == "vdots" || key_->name == "ddots" || key_->name == "adots" || key_->name == "iddots")
		++x;
	if (key_->name == "adots" || key_->name == "iddots")
		++y;
	else if (key_->name != "vdots")
		--y;
	mathed_draw_deco(pain, x + 2, y - dh_, dim.width() - 2, dim.ascent(),
		key_->name);
	setPosCache(pain, x, y);
}


docstring InsetMathDots::name() const
{
	return key_->name;
}


void InsetMathDots::validate(LaTeXFeatures & features) const
{
	if (!key_->requires.empty())
		features.require(to_utf8(key_->requires));
}


void InsetMathDots::mathmlize(MathStream & os) const
{
	// which symbols we support is decided by what is listed in
	// lib/symbols as generating a dots inset
	docstring const & n = key_->name;
	std::string ent;
	if (n == "dots" || n == "dotsc" || n == "dotso" || n == "ldots")
		ent = "&hellip;";
	else if (n == "adots")
		ent = "&utdot;";
	else if (n == "cdots" || n == "dotsb" || n == "dotsi" || n == "dotsm")
		ent = "&ctdot;";
	else if (n == "ddots")
		ent = "&dtdot;";
	else if (n == "iddots")
		ent = "&utdot;";
	else if (n == "vdots")
		ent = "&vellip;";
	else
		LASSERT(false, ent = "&hellip;");
	os << from_ascii(ent);
}

} // namespace lyx
