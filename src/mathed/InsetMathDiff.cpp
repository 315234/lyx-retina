/**
 * \file InsetMathDiff.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathDiff.h"
#include "MathData.h"
#include "MathStream.h"
#include "debug.h"


namespace lyx {

using std::endl;


InsetMathDiff::InsetMathDiff()
	: InsetMathNest(1)
{}


Inset * InsetMathDiff::clone() const
{
	return new InsetMathDiff(*this);
}


void InsetMathDiff::addDer(MathData const & der)
{
	cells_.push_back(der);
}


void InsetMathDiff::normalize(NormalStream & os) const
{
	os << "[diff";
	for (idx_type idx = 0; idx < nargs(); ++idx)
		os << ' ' << cell(idx);
	os << ']';
}


void InsetMathDiff::metrics(MetricsInfo &, Dimension &) const
{
	lyxerr << "should not happen" << endl;
}


void InsetMathDiff::draw(PainterInfo &, int, int) const
{
	lyxerr << "should not happen" << endl;
}


void InsetMathDiff::maple(MapleStream & os) const
{
	os << "diff(";
	for (idx_type idx = 0; idx < nargs(); ++idx) {
		if (idx != 0)
			os << ',';
		os << cell(idx);
	}
	os << ')';
}


void InsetMathDiff::maxima(MaximaStream & os) const
{
	os << "diff(";
	for (idx_type idx = 0; idx < nargs(); ++idx) {
		if (idx != 0)
			os << ',';
		os << cell(idx);
		if (idx != 0)
			os << ",1";
	}
	os << ')';
}


void InsetMathDiff::mathematica(MathematicaStream & os) const
{
	os << "D[";
	for (idx_type idx = 0; idx < nargs(); ++idx) {
		if (idx != 0)
			os << ',';
		os << cell(idx);
	}
	os << ']';
}


void InsetMathDiff::mathmlize(MathStream & os) const
{
	os << "diff(";
	for (idx_type idx = 0; idx < nargs(); ++idx) {
		if (idx != 0)
			os << ',';
		os << cell(idx);
	}
	os << ')';
}


void InsetMathDiff::write(WriteStream &) const
{
	lyxerr << "should not happen" << endl;
}


} // namespace lyx
