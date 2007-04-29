/**
 * \file ParagraphParameters.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Angus Leeming
 * \author John Levon
 * \author Andr� P�nitz
 * \author J�rgen Vigna
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "ParagraphParameters.h"

#include "Buffer.h"
#include "gettext.h"
#include "Layout.h"
#include "Lexer.h"
#include "LyXText.h"
#include "Paragraph.h"
#include "tex-strings.h"

#include "support/lstrings.h"

#include <sstream>

namespace lyx {

using support::rtrim;

using std::istringstream;
using std::ostream;
using std::ostringstream;
using std::string;


static int findToken(char const * const str[], string const & search_token)
{
	return search_token == "default" ?
		0 :
		support::findToken(str, search_token);
}


ParagraphParameters::ParagraphParameters()
	: noindent_(false),
		start_of_appendix_(false), appendix_(false),
	  align_(LYX_ALIGN_LAYOUT), depth_(0)
{}


void ParagraphParameters::clear()
{
	operator=(ParagraphParameters());
}


depth_type ParagraphParameters::depth() const
{
	return depth_;
}


bool ParagraphParameters::sameLayout(ParagraphParameters const & pp) const
{
	return align_ == pp.align_
		&& spacing_ == pp.spacing_
		&& noindent_ == pp.noindent_
		&& depth_ == pp.depth_;
}


Spacing const & ParagraphParameters::spacing() const
{
	return spacing_;
}


void ParagraphParameters::spacing(Spacing const & s)
{
	spacing_ = s;
}


bool ParagraphParameters::noindent() const
{
	return noindent_;
}


void ParagraphParameters::noindent(bool ni)
{
	noindent_ = ni;
}


LyXAlignment ParagraphParameters::align() const
{
	return align_;
}


void ParagraphParameters::align(LyXAlignment la)
{
	align_ = la;
}


void ParagraphParameters::depth(depth_type d)
{
	depth_ = d;
}


bool ParagraphParameters::startOfAppendix() const
{
	return start_of_appendix_;
}


void ParagraphParameters::startOfAppendix(bool soa)
{
	start_of_appendix_ = soa;
}


bool ParagraphParameters::appendix() const
{
	return appendix_;
}


void ParagraphParameters::appendix(bool a)
{
	appendix_ = a;
}


docstring const & ParagraphParameters::labelString() const
{
	return labelstring_;
}


void ParagraphParameters::labelString(docstring const & ls)
{
	labelstring_ = ls;
}


docstring const & ParagraphParameters::labelWidthString() const
{
	return labelwidthstring_;
}


void ParagraphParameters::labelWidthString(docstring const & lws)
{
	labelwidthstring_ = lws;
}


Length const & ParagraphParameters::leftIndent() const
{
	return leftindent_;
}


void ParagraphParameters::leftIndent(Length const & li)
{
	leftindent_ = li;
}


void ParagraphParameters::read(Lexer & lex)
{
	while (lex.isOK()) {
		lex.nextToken();
		string const token = lex.getString();

		if (token.empty())
			continue;

		if (token[0] != '\\') {
			lex.pushToken(token);
			break;
		}

		if (token == "\\noindent") {
			noindent(true);
		} else if (token == "\\leftindent") {
			lex.next();
			Length value(lex.getString());
			leftIndent(value);
		} else if (token == "\\start_of_appendix") {
			startOfAppendix(true);
		} else if (token == "\\paragraph_spacing") {
			lex.next();
			string const tmp = rtrim(lex.getString());
			if (tmp == "single") {
				spacing(Spacing(Spacing::Single));
			} else if (tmp == "onehalf") {
				spacing(Spacing(Spacing::Onehalf));
			} else if (tmp == "double") {
				spacing(Spacing(Spacing::Double));
			} else if (tmp == "other") {
				lex.next();
				spacing(Spacing(Spacing::Other,
						 lex.getString()));
			} else {
				lex.printError("Unknown spacing token: '$$Token'");
			}
		} else if (token == "\\align") {
			lex.next();
			int tmpret = findToken(string_align, lex.getString());
			if (tmpret == -1)
				++tmpret;
			align(LyXAlignment(1 << tmpret));
		} else if (token == "\\labelwidthstring") {
			lex.eatLine();
			labelWidthString(lex.getDocString());
		} else {
			lex.pushToken(token);
			break;
		}
	}
}


void ParagraphParameters::write(ostream & os) const
{
	// Maybe the paragraph has special spacing
	spacing().writeFile(os, true);

	// The labelwidth string used in lists.
	if (!labelWidthString().empty())
		os << "\\labelwidthstring "
		   << to_utf8(labelWidthString()) << '\n';

	// Start of appendix?
	if (startOfAppendix())
		os << "\\start_of_appendix\n";

	// Noindent?
	if (noindent())
		os << "\\noindent\n";

	// Do we have a manual left indent?
	if (!leftIndent().zero())
		os << "\\leftindent " << leftIndent().asString() << '\n';

	// Alignment?
	if (align() != LYX_ALIGN_LAYOUT) {
		int h = 0;
		switch (align()) {
		case LYX_ALIGN_LEFT: h = 1; break;
		case LYX_ALIGN_RIGHT: h = 2; break;
		case LYX_ALIGN_CENTER: h = 3; break;
		default: h = 0; break;
		}
		os << "\\align " << string_align[h] << '\n';
	}
}


void params2string(Paragraph const & par, string & data)
{
	// A local copy
	ParagraphParameters params = par.params();

	// This needs to be done separately
	params.labelWidthString(par.getLabelWidthString());

	// Alignment
	Layout_ptr const & layout = par.layout();
	if (params.align() == LYX_ALIGN_LAYOUT)
		params.align(layout->align);

	ostringstream os;
	params.write(os);

	// Is alignment possible
	os << "\\alignpossible " << layout->alignpossible << '\n';

	/// set default alignment
	os << "\\aligndefault " << layout->align << '\n';

	/// paragraph is always in inset. This is redundant.
	os << "\\ininset " << 1 << '\n';

	data = os.str();
}


/*
bool operator==(ParagraphParameeters const & ps1,
		ParagraphParameeters const & ps2)
{
	return
		   ps1.spacing == ps2.spacing
		&& ps1.noindent == ps2.noindent
		&& ps1.align == ps2.align
		&& ps1.depth == ps2.depth
		&& ps1.start_of_appendix == ps2.start_of_appendix
		&& ps1.appendix == ps2.appendix
		&& ps1.labelstring == ps2.labelstring
		&& ps1.labelwidthstring == ps2.labelwidthstring
		&& ps1.leftindent == ps2.leftindent;
}
*/


} // namespace lyx
