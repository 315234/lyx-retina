/**
 * \file InsetSeparator.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Enrico Forestieri
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetSeparator.h"

#include "Cursor.h"
#include "Dimension.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "Lexer.h"
#include "MetricsInfo.h"
#include "OutputParams.h"
#include "output_xhtml.h"

#include "frontends/Application.h"
#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"

#include "support/debug.h"
#include "support/docstream.h"
#include "support/docstring.h"

using namespace std;

namespace lyx {

InsetSeparator::InsetSeparator() : Inset(0)
{}


InsetSeparator::InsetSeparator(InsetSeparatorParams const & params)
	: Inset(0), params_(params)
{}


void InsetSeparatorParams::write(ostream & os) const
{
	string command;
	switch (kind) {
	case InsetSeparatorParams::PLAIN:
		os <<  "plain";
		break;
	case InsetSeparatorParams::PARBREAK:
		os <<  "parbreak";
		break;
	}
}


void InsetSeparatorParams::read(Lexer & lex)
{
	string token;
	lex.setContext("InsetSeparatorParams::read");
	lex >> token;	
	if (token == "plain")
		kind = InsetSeparatorParams::PLAIN;
	else if (token == "parbreak")
		kind = InsetSeparatorParams::PARBREAK;
	else
		lex.printError("Unknown kind: `$$Token'");
}


void InsetSeparator::write(ostream & os) const
{
	os << "Separator ";
	params_.write(os);
}


void InsetSeparator::read(Lexer & lex)
{
	params_.read(lex);
	lex >> "\\end_inset";
}


void InsetSeparator::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action()) {

	case LFUN_INSET_MODIFY: {
		InsetSeparatorParams params;
		cur.recordUndo();
		string2params(to_utf8(cmd.argument()), params);
		params_.kind = params.kind;
		break;
	}

	default:
		Inset::doDispatch(cur, cmd);
		break;
	}
}


bool InsetSeparator::getStatus(Cursor & cur, FuncRequest const & cmd,
	FuncStatus & status) const
{
	switch (cmd.action()) {
	// we handle these
	case LFUN_INSET_MODIFY:
		if (cmd.getArg(0) == "plain") {
			InsetSeparatorParams params;
			string2params(to_utf8(cmd.argument()), params);
			status.setOnOff(params_.kind == params.kind);
		}
		status.setEnabled(true);
		return true;
	default:
		return Inset::getStatus(cur, cmd, status);
	}
}


ColorCode InsetSeparator::ColorName() const
{
	switch (params_.kind) {
		case InsetSeparatorParams::PLAIN:
			return Color_latex;
			break;
		case InsetSeparatorParams::PARBREAK:
			return Color_pagebreak;
			break;
	}
	// not really useful, but to avoids gcc complaints
	return Color_latex;
}


void InsetSeparator::latex(otexstream & os, OutputParams const &) const
{
	switch (params_.kind) {
		case InsetSeparatorParams::PLAIN:
			os << breakln << "%\n";
			break;
		case InsetSeparatorParams::PARBREAK:
			os << breakln << "\n";
			break;
		default:
			os << breakln << "%\n";
			break;
	}
}


int InsetSeparator::plaintext(odocstringstream & os,
        OutputParams const &, size_t) const
{
	os << '\n';
	return PLAINTEXT_NEWLINE;
}


int InsetSeparator::docbook(odocstream & os, OutputParams const &) const
{
	os << '\n';
	return 0;
}


docstring InsetSeparator::xhtml(XHTMLStream & xs, OutputParams const &) const
{
	xs << html::CR() << html::CompTag("br") << html::CR();
	return docstring();
}


void InsetSeparator::metrics(MetricsInfo & mi, Dimension & dim) const
{
	frontend::FontMetrics const & fm = theFontMetrics(mi.base.font);
	dim.asc = fm.maxAscent();
	dim.des = fm.maxDescent();
	dim.wid = fm.width('m');
	if (params_.kind == InsetSeparatorParams::PLAIN)
		dim.wid *= 5;
}


void InsetSeparator::draw(PainterInfo & pi, int x, int y) const
{
	FontInfo font;
	font.setColor(ColorName());

	frontend::FontMetrics const & fm = theFontMetrics(pi.base.font);
	int const wid = fm.width('m');
	int const asc = fm.maxAscent();

	int xp[3];
	int yp[3];

	if (params_.kind == InsetSeparatorParams::PLAIN) {
		yp[0] = int(y - 0.500 * asc * 0.75);
		yp[1] = int(y - 0.500 * asc * 0.75);

		xp[0] = int(x);
		xp[1] = int(x + wid * 5);

		pi.pain.lines(xp, yp, 2, ColorName());
	} else {
		yp[0] = int(y - 0.875 * asc * 0.75);
		yp[1] = int(y - 0.500 * asc * 0.75);
		yp[2] = int(y - 0.125 * asc * 0.75);

		if (pi.ltr_pos) {
			xp[0] = int(x + wid * 0.375);
			xp[1] = int(x);
			xp[2] = int(x + wid * 0.375);
		} else {
			xp[0] = int(x + wid * 0.625);
			xp[1] = int(x + wid);
			xp[2] = int(x + wid * 0.625);
		}

		pi.pain.lines(xp, yp, 3, ColorName());

		yp[0] = int(y - 0.500 * asc * 0.75);
		yp[1] = int(y - 0.500 * asc * 0.75);
		yp[2] = int(y - asc * 0.75);

		if (pi.ltr_pos) {
			xp[0] = int(x);
			xp[1] = int(x + wid);
			xp[2] = int(x + wid);
		} else {
			xp[0] = int(x + wid);
			xp[1] = int(x);
			xp[2] = int(x);
		}

		pi.pain.lines(xp, yp, 3, ColorName());
	}
}


string InsetSeparator::contextMenuName() const
{
	return "context-separator";
}


void InsetSeparator::string2params(string const & in, InsetSeparatorParams & params)
{
	params = InsetSeparatorParams();
	if (in.empty())
		return;
	istringstream data(in);
	Lexer lex;
	lex.setStream(data);
	lex.setContext("InsetSeparator::string2params");
	lex >> "separator";
	params.read(lex);
}


string InsetSeparator::params2string(InsetSeparatorParams const & params)
{
	ostringstream data;
	data << "separator" << ' ';
	params.write(data);
	return data.str();
}


} // namespace lyx
