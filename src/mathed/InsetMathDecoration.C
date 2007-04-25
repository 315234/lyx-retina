/**
 * \file InsetMathDecoration.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathDecoration.h"
#include "MathData.h"
#include "MathParser.h"
#include "MathSupport.h"
#include "MathStream.h"

#include "LaTeXFeatures.h"
#include "debug.h"

#include "support/std_ostream.h"


namespace lyx {


InsetMathDecoration::InsetMathDecoration(latexkeys const * key)
	: InsetMathNest(1), key_(key)
{
//	lyxerr << " creating deco " << key->name << std::endl;
}


std::auto_ptr<InsetBase> InsetMathDecoration::doClone() const
{
	return std::auto_ptr<InsetBase>(new InsetMathDecoration(*this));
}


bool InsetMathDecoration::upper() const
{
	return key_->name.substr(0, 5) != "under";
}


bool InsetMathDecoration::isScriptable() const
{
	return
			key_->name == "overbrace" ||
			key_->name == "underbrace" ||
			key_->name == "overleftarrow" ||
			key_->name == "overrightarrow" ||
			key_->name == "overleftrightarrow" ||
			key_->name == "underleftarrow" ||
			key_->name == "underrightarrow" ||
			key_->name == "underleftrightarrow";
}


bool InsetMathDecoration::protect() const
{
	return
			key_->name == "overbrace" ||
			key_->name == "underbrace" ||
			key_->name == "overleftarrow" ||
			key_->name == "overrightarrow" ||
			key_->name == "overleftrightarrow" ||
			key_->name == "underleftarrow" ||
			key_->name == "underrightarrow" ||
			key_->name == "underleftrightarrow";
}


bool InsetMathDecoration::wide() const
{
	return
			key_->name == "overline" ||
			key_->name == "underline" ||
			key_->name == "overbrace" ||
			key_->name == "underbrace" ||
			key_->name == "overleftarrow" ||
			key_->name == "overrightarrow" ||
			key_->name == "overleftrightarrow" ||
			key_->name == "widehat" ||
			key_->name == "widetilde" ||
			key_->name == "underleftarrow" ||
			key_->name == "underrightarrow" ||
			key_->name == "underleftrightarrow";
}


bool InsetMathDecoration::ams() const
{
	return
			key_->name == "overleftrightarrow" ||
			key_->name == "underleftarrow" ||
			key_->name == "underrightarrow" ||
			key_->name == "underleftrightarrow";
}


bool InsetMathDecoration::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(0).metrics(mi, dim);

	dh_  = 6; //mathed_char_height(LM_TC_VAR, mi, 'I', ascent_, descent_);
	dw_  = 6; //mathed_char_width(LM_TC_VAR, mi, 'x');

	if (upper()) {
		dy_ = -dim.asc - dh_;
		dim.asc += dh_ + 1;
	} else {
		dy_ = dim.des + 1;
		dim.des += dh_ + 2;
	}

	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathDecoration::draw(PainterInfo & pi, int x, int y) const
{
	cell(0).draw(pi, x + 1, y);
	if (wide())
		mathed_draw_deco(pi, x + 1, y + dy_, cell(0).width(), dh_, key_->name);
	else
		mathed_draw_deco(pi, x + 1 + (cell(0).width() - dw_) / 2,
			y + dy_, dw_, dh_, key_->name);
	drawMarkers(pi, x, y);
	setPosCache(pi, x, y);
}


void InsetMathDecoration::write(WriteStream & os) const
{
	if (os.fragile() && protect())
		os << "\\protect";
	os << '\\' << key_->name << '{' << cell(0) << '}';
}


void InsetMathDecoration::normalize(NormalStream & os) const
{
	os << "[deco " << key_->name << ' ' <<  cell(0) << ']';
}


void InsetMathDecoration::infoize(odocstream & os) const
{
	os << "Deco: " << key_->name;
}


void InsetMathDecoration::validate(LaTeXFeatures & features) const
{
	if (ams())
		features.require("amsmath");
	InsetMathNest::validate(features);
}


} // namespace lyx
