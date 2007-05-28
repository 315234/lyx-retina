/**
 * \file RenderButton.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "RenderButton.h"

#include "Color.h"
#include "MetricsInfo.h"

#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"


namespace lyx {

using std::string;
using std::auto_ptr;


RenderButton::RenderButton()
	: editable_(false)
{}


auto_ptr<RenderBase> RenderButton::clone(Inset const *) const
{
	return auto_ptr<RenderBase>(new RenderButton(*this));
}


void RenderButton::update(docstring const & text, bool editable)
{
	text_ = text;
	editable_ = editable;
}


bool RenderButton::metrics(MetricsInfo &, Dimension & dim) const
{
	Font font(Font::ALL_SANE);
	font.decSize();
	frontend::FontMetrics const & fm =
		theFontMetrics(font);

	if (editable_)
		fm.buttonText(text_, dim.wid, dim.asc, dim.des);
	else
		fm.rectText(text_, dim.wid, dim.asc, dim.des);

	dim.wid += 4;
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void RenderButton::draw(PainterInfo & pi, int x, int y) const
{
	// Draw it as a box with the LaTeX text
	Font font(Font::ALL_SANE);
	font.setColor(Color::command);
	font.decSize();

	if (editable_) {
		pi.pain.buttonText(x + 2, y, text_, font, renderState());
	} else {
		pi.pain.rectText(x + 2, y, text_, font,
				 Color::commandbg, Color::commandframe);
	}
}


} // namespace lyx
