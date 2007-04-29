/**
 * \file InsetMathNumber.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathNumber.h"
#include "MathStream.h"
#include "MathStream.h"
#include "MathSupport.h"


namespace lyx {

using std::string;
using std::auto_ptr;
using std::vector;

InsetMathNumber::InsetMathNumber(docstring const & s)
	: str_(s)
{}


auto_ptr<Inset> InsetMathNumber::doClone() const
{
	return auto_ptr<Inset>(new InsetMathNumber(*this));
}


bool InsetMathNumber::metrics(MetricsInfo & mi, Dimension & dim) const
{
	mathed_string_dim(mi.base.font, str_, dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathNumber::draw(PainterInfo & pi, int x, int y) const
{
	pi.draw(x, y, str_);
}


void InsetMathNumber::normalize(NormalStream & os) const
{
	os << "[number " << str_ << ']';
}


void InsetMathNumber::maple(MapleStream & os) const
{
	os << str_;
}


void InsetMathNumber::mathematica(MathematicaStream & os) const
{
	os << str_;
}


void InsetMathNumber::octave(OctaveStream & os) const
{
	os << str_;
}


void InsetMathNumber::mathmlize(MathStream & os) const
{
	os << "<mi> " << str_ << " </mi>";
}


void InsetMathNumber::write(WriteStream & os) const
{
	os << str_;
}


} // namespace lyx
