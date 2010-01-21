/**
 * \file InsetMathSize.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathSize.h"

#include "MathData.h"
#include "MathParser.h"
#include "MathStream.h"
#include "output_xhtml.h"

#include "support/convert.h"

#include <string>
#include <ostream>

using namespace std;

namespace lyx {

InsetMathSize::InsetMathSize(Buffer * buf, latexkeys const * l)
	: InsetMathNest(buf, 1), key_(l), style_(Styles(convert<int>(l->extra)))
{}


Inset * InsetMathSize::clone() const
{
	return new InsetMathSize(*this);
}


void InsetMathSize::metrics(MetricsInfo & mi, Dimension & dim) const
{
	StyleChanger dummy(mi.base, style_);
	cell(0).metrics(mi, dim);
	metricsMarkers(dim);
}


void InsetMathSize::draw(PainterInfo & pi, int x, int y) const
{
	StyleChanger dummy(pi.base, style_);
	cell(0).draw(pi, x + 1, y);
	drawMarkers(pi, x, y);
}


void InsetMathSize::write(WriteStream & os) const
{
	MathEnsurer ensurer(os);
	os << "{\\" << key_->name << ' ' << cell(0) << '}';
}


// From the MathML documentation:
//	MathML uses two attributes, displaystyle and scriptlevel, to control 
//	orthogonal presentation features that TeX encodes into one "style" 
//	attribute with values \displaystyle, \textstyle, \scriptstyle, and 
//	\scriptscriptstyle. The corresponding values of displaystyle and scriptlevel 
//	for those TeX styles would be "true" and "0", "false" and "0", "false" and "1", 
//	and "false" and "2", respectively. 
void InsetMathSize::mathmlize(MathStream & ms) const
{
	string const & name = to_utf8(key_->name);
	bool dispstyle = (name == "displaystyle");
	int scriptlevel = 0;
	if (name == "scriptstyle")
		scriptlevel = 1;
	else if (name == "scriptscriptstyle")
		scriptlevel = 2;
	stringstream attrs;
	attrs << "displaystyle='" << (dispstyle ? "true" : "false")
		<< "' scriptlevel='" << scriptlevel << "'";
	ms << MTag("mstyle", attrs.str()) << ">"
	   << cell(0) << ETag("mstyle");
}


void InsetMathSize::normalize(NormalStream & os) const
{
	os << '[' << key_->name << ' ' << cell(0) << ']';
}


void InsetMathSize::infoize(odocstream & os) const
{
	os << "Size: " << key_->name;
}


} // namespace lyx
