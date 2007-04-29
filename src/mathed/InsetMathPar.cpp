/**
 * \file InsetMathPar.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathPar.h"
#include "MathData.h"
#include "MathStream.h"
#include "support/std_ostream.h"


namespace lyx {


using std::auto_ptr;


InsetMathPar::InsetMathPar(MathData const & ar)
{
	cells_[0] = ar;
}


bool InsetMathPar::metrics(MetricsInfo & mi, Dimension & dim) const
{
	dim = dim_;
	FontSetChanger dummy1(mi.base, "textnormal");
	InsetMathGrid::metrics(mi);
	if (dim_ == dim)
		return false;
	dim = dim_;
	return true;
}


void InsetMathPar::draw(PainterInfo & pi, int x, int y) const
{
	FontSetChanger dummy1(pi.base, "textnormal");
	InsetMathGrid::draw(pi, x, y);
}


void InsetMathPar::write(WriteStream & os) const
{
	for (idx_type i = 0; i < nargs(); ++i)
		os << cell(i) << "\n";
}


void InsetMathPar::infoize(odocstream & os) const
{
	os << "Type: Paragraph ";
}


auto_ptr<Inset> InsetMathPar::doClone() const
{
	return auto_ptr<Inset>(new InsetMathPar(*this));
}


} // namespace lyx
