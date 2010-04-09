/**
 * \file InsetMathSubstack.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathSubstack.h"

#include "LaTeXFeatures.h"
#include "MathData.h"
#include "MathStream.h"

#include "FuncRequest.h"
#include "FuncStatus.h"
#include "support/gettext.h"

#include "support/lstrings.h"

#include <ostream>

using namespace std;

namespace lyx {

using support::bformat;


InsetMathSubstack::InsetMathSubstack(Buffer * buf)
	: InsetMathGrid(buf, 1, 1)
{}


Inset * InsetMathSubstack::clone() const
{
	return new InsetMathSubstack(*this);
}


void InsetMathSubstack::metrics(MetricsInfo & mi, Dimension & dim) const
{
	if (mi.base.style == LM_ST_DISPLAY) {
		StyleChanger dummy(mi.base, LM_ST_TEXT);
		InsetMathGrid::metrics(mi, dim);
	} else {
		InsetMathGrid::metrics(mi, dim);
	}
}


void InsetMathSubstack::draw(PainterInfo & pi, int x, int y) const
{
	InsetMathGrid::draw(pi, x + 1, y);
}


bool InsetMathSubstack::getStatus(Cursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const
{
	switch (cmd.action_) {
	case LFUN_INSET_MODIFY: {
		istringstream is(to_utf8(cmd.argument()));
		string s;
		is >> s;
		if (s != "tabular")
			break;
		is >> s;
		string const name = "substack";
		if (s == "add-vline-left" || s == "add-vline-right") {
			flag.message(bformat(
				from_utf8(N_("Can't add vertical grid lines in '%1$s'")),
				from_utf8(name)));
			flag.setEnabled(false);
			return true;
		}
		break;
	}
	default:
		break;
	}
	return InsetMathGrid::getStatus(cur, cmd, flag);
}


void InsetMathSubstack::infoize(odocstream & os) const
{
	os << "Substack ";
}


void InsetMathSubstack::write(WriteStream & os) const
{
	MathEnsurer ensurer(os);
	os << "\\substack{";
	InsetMathGrid::write(os);
	os << "}\n";
}


void InsetMathSubstack::normalize(NormalStream & os) const
{
	os << "[substack ";
	InsetMathGrid::normalize(os);
	os << ']';
}


void InsetMathSubstack::maple(MapleStream & os) const
{
	os << "substack(";
	InsetMathGrid::maple(os);
	os << ')';
}


void InsetMathSubstack::htmlize(HtmlStream & os) const
{
	os << MTag("span", "class='substack'");
	for (row_type row = 0; row < nrows(); ++row) 
		os << MTag("span") << cell(index(row, 0)) << ETag("span");
	os << ETag("span");
}

	
void InsetMathSubstack::validate(LaTeXFeatures & features) const
{
	if (features.runparams().isLaTeX())
		features.require("amsmath");
	else if (features.runparams().math_flavor == OutputParams::MathAsHTML)
		features.addPreambleSnippet("<style type=\"text/css\">\n"
			"span.substack{display: inline-block; vertical-align: middle; text-align:center; font-size: 75%;}\n"
			"span.substack span{display: block;}\n"
			"</style>");
	
	InsetMathGrid::validate(features);
}


} // namespace lyx
