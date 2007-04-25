/**
 * \file InsetFootlike.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetFootlike.h"

#include "buffer.h"
#include "BufferView.h"
#include "bufferparams.h"
#include "LColor.h"
#include "metricsinfo.h"
#include "paragraph.h"


namespace lyx {

InsetFootlike::InsetFootlike(BufferParams const & bp)
	: InsetCollapsable(bp)
{
	LyXFont font(LyXFont::ALL_SANE);
	font.decSize();
	font.decSize();
	font.setColor(LColor::collapsable);
	setLabelFont(font);
}


InsetFootlike::InsetFootlike(InsetFootlike const & in)
	: InsetCollapsable(in)
{
	LyXFont font(LyXFont::ALL_SANE);
	font.decSize();
	font.decSize();
	font.setColor(LColor::collapsable);
	setLabelFont(font);
}


bool InsetFootlike::metrics(MetricsInfo & mi, Dimension & dim) const
{
	LyXFont tmpfont = mi.base.font;
	mi.base.font = mi.base.bv->buffer()->params().getFont();
	InsetCollapsable::metrics(mi, dim);
	mi.base.font = tmpfont;
	bool const changed = dim_ != dim;
	dim_ = dim;
	return changed;
}


void InsetFootlike::draw(PainterInfo & pi, int x, int y) const
{
	LyXFont tmpfont = pi.base.font;
	pi.base.font = pi.base.bv->buffer()->params().getFont();
	InsetCollapsable::draw(pi, x, y);
	pi.base.font = tmpfont;
}


void InsetFootlike::write(Buffer const & buf, std::ostream & os) const
{
	os << to_utf8(getInsetName()) << "\n";
	InsetCollapsable::write(buf, os);
}


bool InsetFootlike::insetAllowed(InsetBase::Code code) const
{
	if (code == InsetBase::FOOT_CODE || code == InsetBase::MARGIN_CODE
	    || code == InsetBase::FLOAT_CODE)
		return false;
	return InsetCollapsable::insetAllowed(code);
}


} // namespace lyx
