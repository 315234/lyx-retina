/**
 * \file InsetMath.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMath.h"
#include "MathData.h"
#include "MathStream.h"
#include "gettext.h"
#include "debug.h"

#include "support/lstrings.h"
#include "support/textutils.h"

#include <boost/current_function.hpp>

using std::endl;

namespace lyx {



MathData & InsetMath::cell(idx_type)
{
	static MathData dummyCell;
	lyxerr << BOOST_CURRENT_FUNCTION << ": I don't have any cell" << endl;
	return dummyCell;
}


MathData const & InsetMath::cell(idx_type) const
{
	static MathData dummyCell;
	lyxerr << BOOST_CURRENT_FUNCTION << ": I don't have any cell" << endl;
	return dummyCell;
}


void InsetMath::dump() const
{
	lyxerr << "---------------------------------------------" << endl;
	odocstringstream os;
	WriteStream wi(os, false, true);
	write(wi);
	lyxerr << to_utf8(os.str());
	lyxerr << "\n---------------------------------------------" << endl;
}


void InsetMath::metricsT(TextMetricsInfo const &, Dimension &) const
{
#ifdef WITH_WARNINGS
	lyxerr << "InsetMath::metricsT(Text) called directly!" << endl;
#endif
}


void InsetMath::drawT(TextPainter &, int, int) const
{
#ifdef WITH_WARNINGS
	lyxerr << "InsetMath::drawT(Text) called directly!" << endl;
#endif
}



void InsetMath::write(WriteStream & os) const
{
	docstring const s = name();
	os << "\\" << s;
	// We need an extra ' ' unless this is a single-char-non-ASCII name
	// or anything non-ASCII follows
	if (s.size() != 1 || isAlphaASCII(s[0]))
		os.pendingSpace(true);
}


int InsetMath::plaintext(Buffer const &, odocstream &,
			 OutputParams const &) const
{
	// all math plain text output shall take place in InsetMathHull
	BOOST_ASSERT(false);
	return 0;
}


void InsetMath::normalize(NormalStream & os) const
{
	os << '[' << name() << "] ";
}


void InsetMath::octave(OctaveStream & os) const
{
	NormalStream ns(os.os());
	normalize(ns);
}


void InsetMath::maple(MapleStream & os) const
{
	NormalStream ns(os.os());
	normalize(ns);
}


void InsetMath::maxima(MaximaStream & os) const
{
	MapleStream ns(os.os());
	maple(ns);
}


void InsetMath::mathematica(MathematicaStream & os) const
{
	NormalStream ns(os.os());
	normalize(ns);
}


void InsetMath::mathmlize(MathStream & os) const
{
	NormalStream ns(os.os());
	normalize(ns);
}


HullType InsetMath::getType() const
{
	return hullNone;
}


std::ostream & operator<<(std::ostream & os, MathAtom const & at)
{
	odocstringstream oss;
	WriteStream wi(oss, false, false);
	at->write(wi);
	return os << to_utf8(oss.str());
}


odocstream & operator<<(odocstream & os, MathAtom const & at)
{
	WriteStream wi(os, false, false);
	at->write(wi);
	return os;
}


} // namespace lyx
