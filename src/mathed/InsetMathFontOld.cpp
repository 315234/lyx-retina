/**
 * \file InsetMathFontOld.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathFontOld.h"
#include "MathData.h"
#include "MathStream.h"
#include "MathParser.h"
#include "MathStream.h"
#include "support/std_ostream.h"


namespace lyx {

using std::auto_ptr;


InsetMathFontOld::InsetMathFontOld(latexkeys const * key)
	: InsetMathNest(1), key_(key)
{
	//lock(true);
}


auto_ptr<Inset> InsetMathFontOld::doClone() const
{
	return auto_ptr<Inset>(new InsetMathFontOld(*this));
}


bool InsetMathFontOld::metrics(MetricsInfo & mi, Dimension & dim) const
{
	FontSetChanger dummy(mi.base, key_->name.c_str());
	cell(0).metrics(mi, dim);
	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathFontOld::draw(PainterInfo & pi, int x, int y) const
{
	FontSetChanger dummy(pi.base, key_->name.c_str());
	cell(0).draw(pi, x + 1, y);
	drawMarkers(pi, x, y);
}


void InsetMathFontOld::metricsT(TextMetricsInfo const & mi, Dimension & dim) const
{
	cell(0).metricsT(mi, dim);
}


void InsetMathFontOld::drawT(TextPainter & pain, int x, int y) const
{
	cell(0).drawT(pain, x, y);
}


void InsetMathFontOld::write(WriteStream & os) const
{
	os << "{\\" << key_->name << ' ' << cell(0) << '}';
}


void InsetMathFontOld::normalize(NormalStream & os) const
{
	os << "[font " << key_->name << ' ' << cell(0) << ']';
}


void InsetMathFontOld::infoize(odocstream & os) const
{
	os << "Font: " << key_->name;
}


} // namespace lyx
