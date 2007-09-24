/**
 * \file InsetMathBrace.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathBrace.h"
#include "MathData.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "Color.h"

#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"

#include "support/std_ostream.h"

namespace lyx {

InsetMathBrace::InsetMathBrace()
	: InsetMathNest(1)
{}


InsetMathBrace::InsetMathBrace(MathData const & ar)
	: InsetMathNest(1)
{
	cell(0) = ar;
}


Inset * InsetMathBrace::clone() const
{
	return new InsetMathBrace(*this);
}


void InsetMathBrace::metrics(MetricsInfo & mi, Dimension & dim) const
{
	Dimension dim0;
	cell(0).metrics(mi, dim0);
	Dimension t = theFontMetrics(mi.base.font).dimension('{');
	dim.asc = std::max(dim0.asc, t.asc);
	dim.des = std::max(dim0.des, t.des);
	dim.wid = dim0.width() + 2 * t.wid;
	metricsMarkers(dim);
	// Cache the inset dimension. 
	setDimCache(mi, dim);
}


void InsetMathBrace::draw(PainterInfo & pi, int x, int y) const
{
	Font font = pi.base.font;
	font.setColor(Color::latex);
	Dimension t = theFontMetrics(font).dimension('{');
	pi.pain.text(x, y, '{', font);
	cell(0).draw(pi, x + t.wid, y);
	Dimension const & dim0 = cell(0).dimension(*pi.base.bv);
	pi.pain.text(x + t.wid + dim0.width(), y, '}', font);
	drawMarkers(pi, x, y);
}


void InsetMathBrace::write(WriteStream & os) const
{
	os << '{' << cell(0) << '}';
}


void InsetMathBrace::normalize(NormalStream & os) const
{
	os << "[block " << cell(0) << ']';
}


void InsetMathBrace::maple(MapleStream & os) const
{
	os << cell(0);
}


void InsetMathBrace::octave(OctaveStream & os) const
{
	os << cell(0);
}


void InsetMathBrace::mathmlize(MathStream & os) const
{
	os << MTag("mrow") << cell(0) << ETag("mrow");
}


void InsetMathBrace::mathematica(MathematicaStream & os) const
{
	os << cell(0);
}


void InsetMathBrace::infoize(odocstream & os) const
{
	os << "Nested Block: ";
}


} // namespace lyx
