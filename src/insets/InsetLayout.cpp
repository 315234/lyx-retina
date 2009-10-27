// -*- C++ -*-
/**
 * \file InsetLayout.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Martin Vermeer
 * \author Richard Heck
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetLayout.h"

#include "ColorSet.h"
#include "Lexer.h"
#include "TextClass.h"

#include "support/debug.h"
#include "support/lstrings.h"

#include <vector>

using std::string;
using std::set;
using std::vector;

namespace lyx {

InsetLayout::InsetLayout() :
	name_(from_ascii("undefined")), lyxtype_(STANDARD),
	labelstring_(from_ascii("UNDEFINED")), decoration_(DEFAULT),
	latextype_(NOLATEXTYPE), font_(sane_font), 
	labelfont_(sane_font), bgcolor_(Color_error), 
	htmlforcecss_ (false), htmlisblock_(true),
	multipar_(false), custompars_(true), forceplain_(false), 
	passthru_(false), needprotect_(false), freespacing_(false), 
	keepempty_(false), forceltr_(false), intoc_(false)
{ 
	labelfont_.setColor(Color_error);
}


namespace {

InsetLayout::InsetDecoration translateDecoration(std::string const & str) 
{
	if (support::compare_ascii_no_case(str, "classic") == 0)
		return InsetLayout::CLASSIC;
	if (support::compare_ascii_no_case(str, "minimalistic") == 0)
		return InsetLayout::MINIMALISTIC;
	if (support::compare_ascii_no_case(str, "conglomerate") == 0)
		return InsetLayout::CONGLOMERATE;
	return InsetLayout::DEFAULT;
}

InsetLayout::InsetLaTeXType translateLaTeXType(std::string const & str)
{
	if (support::compare_ascii_no_case(str, "command") == 0)
		return InsetLayout::COMMAND;
	if (support::compare_ascii_no_case(str, "environment") == 0)
		return InsetLayout::ENVIRONMENT;
	if (support::compare_ascii_no_case(str, "none") == 0)
		return InsetLayout::NOLATEXTYPE;
	return InsetLayout::ILT_ERROR;
}

} // namespace anon


bool InsetLayout::read(Lexer & lex, TextClass const & tclass)
{
	enum {
		IL_BGCOLOR,
		IL_COPYSTYLE,
		IL_COUNTER,
		IL_CUSTOMPARS,
		IL_DECORATION,
		IL_FONT,
		IL_FORCELTR,
		IL_FORCEPLAIN,
		IL_FREESPACING,
		IL_HTMLTAG,
		IL_HTMLATTR,
		IL_HTMLFORCECSS,
		IL_HTMLINNERTAG,
		IL_HTMLINNERATTR,
		IL_HTMLISBLOCK,
		IL_HTMLLABEL,
		IL_HTMLSTYLE,
		IL_HTMLPREAMBLE,
		IL_INTOC,
		IL_LABELFONT,
		IL_LABELSTRING,
		IL_LATEXNAME,
		IL_LATEXPARAM,
		IL_LATEXTYPE,
		IL_LYXTYPE,
		IL_KEEPEMPTY,
		IL_MULTIPAR,
		IL_NEEDPROTECT,
		IL_PASSTHRU,
		IL_PREAMBLE,
		IL_REQUIRES,
		IL_END
	};


	LexerKeyword elementTags[] = {
		{ "bgcolor", IL_BGCOLOR },
		{ "copystyle", IL_COPYSTYLE }, 
		{ "counter", IL_COUNTER},
		{ "custompars", IL_CUSTOMPARS },
		{ "decoration", IL_DECORATION },
		{ "end", IL_END },
		{ "font", IL_FONT },
		{ "forceltr", IL_FORCELTR },
		{ "forceplain", IL_FORCEPLAIN },
		{ "freespacing", IL_FREESPACING },
		{ "htmlattr", IL_HTMLATTR },
		{ "htmlforcecss", IL_HTMLFORCECSS },
		{ "htmlinnerattr", IL_HTMLINNERATTR},
		{ "htmlinnertag", IL_HTMLINNERTAG},
		{ "htmlisblock", IL_HTMLISBLOCK},
		{ "htmllabel", IL_HTMLLABEL },
		{ "htmlpreamble", IL_HTMLPREAMBLE },
		{ "htmlstyle", IL_HTMLSTYLE },
		{ "htmltag", IL_HTMLTAG },
		{ "intoc", IL_INTOC },
		{ "keepempty", IL_KEEPEMPTY },
		{ "labelfont", IL_LABELFONT },
		{ "labelstring", IL_LABELSTRING },
		{ "latexname", IL_LATEXNAME },
		{ "latexparam", IL_LATEXPARAM },
		{ "latextype", IL_LATEXTYPE },
		{ "lyxtype", IL_LYXTYPE },
		{ "multipar", IL_MULTIPAR },
		{ "needprotect", IL_NEEDPROTECT },
		{ "passthru", IL_PASSTHRU },
		{ "preamble", IL_PREAMBLE },
		{ "requires", IL_REQUIRES }
	};

	lex.pushTable(elementTags);

	FontInfo font = inherit_font;
	labelfont_ = inherit_font;
	bgcolor_ = Color_none;
	bool getout = false;
	// whether we've read the CustomPars or ForcePlain tag
	// for issuing a warning in case MultiPars comes later
	bool readCustomOrPlain = false;

	string tmp;	
	while (!getout && lex.isOK()) {
		int le = lex.lex();
		switch (le) {
		case Lexer::LEX_UNDEF:
			lex.printError("Unknown InsetLayout tag");
			continue;
		default:
			break;
		}
		switch (le) {
		// FIXME
		// Perhaps a more elegant way to deal with the next two would be the
		// way this sort of thing is handled in Layout::read(), namely, by
		// using the Lexer.
		case IL_LYXTYPE: {
			string lt;
			lex >> lt;
			lyxtype_ = translateLyXType(lt);
			if (lyxtype_  == NOLYXTYPE)
				LYXERR0("Unknown LyXType `" << lt << "'.");
			break;
		}
		case IL_LATEXTYPE:  {
			string lt;
			lex >> lt;
			latextype_ = translateLaTeXType(lt);
			if (latextype_  == ILT_ERROR)
				LYXERR0("Unknown LaTeXType `" << lt << "'.");
			break;
		}
		case IL_LABELSTRING:
			lex >> labelstring_;
			break;
		case IL_DECORATION:
			lex >> tmp;
			decoration_ = translateDecoration(tmp);
			break;
		case IL_LATEXNAME:
			lex >> latexname_;
			break;
		case IL_LATEXPARAM:
			lex >> tmp;
			latexparam_ = support::subst(tmp, "&quot;", "\"");
			break;
		case IL_LABELFONT:
			labelfont_ = lyxRead(lex, inherit_font);
			break;
		case IL_FORCELTR:
			lex >> forceltr_;
			break;
		case IL_INTOC:
			lex >> intoc_;
			break;
		case IL_MULTIPAR:
			lex >> multipar_;
			// the defaults for these depend upon multipar_
			if (readCustomOrPlain)
				LYXERR0("Warning: Read MultiPar after CustomPars or ForcePlain. "
				        "Previous value may be overwritten!");
			readCustomOrPlain = false;
			custompars_ = multipar_;
			forceplain_ = !multipar_;
			break;
		case IL_COUNTER:
			lex >> counter_;
			break;
		case IL_CUSTOMPARS:
			lex >> custompars_;
			readCustomOrPlain = true;
			break;
		case IL_FORCEPLAIN:
			lex >> forceplain_;
			break;
		case IL_PASSTHRU:
			lex >> passthru_;
			readCustomOrPlain = true;
			break;
		case IL_KEEPEMPTY:
			lex >> keepempty_;
			break;
		case IL_FREESPACING:
			lex >> freespacing_;
			break;
		case IL_NEEDPROTECT:
			lex >> needprotect_;
			break;
		case IL_COPYSTYLE: {     // initialize with a known style
			docstring style;
			lex >> style;
			style = support::subst(style, '_', ' ');

			// We don't want to apply the algorithm in DocumentClass::insetLayout()
			// here. So we do it the long way.
			TextClass::InsetLayouts::const_iterator it = 
					tclass.insetLayouts().find(style);
			if (it != tclass.insetLayouts().end()) {
				docstring const tmpname = name_;
				this->operator=(it->second);
				name_ = tmpname;
			} else {
				LYXERR0("Cannot copy unknown InsetLayout `"
					<< style << "'\n"
					<< "All InsetLayouts so far:");
				TextClass::InsetLayouts::const_iterator lit = 
						tclass.insetLayouts().begin();
				TextClass::InsetLayouts::const_iterator len = 
						tclass.insetLayouts().end();
				for (; lit != len; ++lit)
					lyxerr << lit->second.name() << "\n";
			}
			break;
		}

		case IL_FONT: {
			font_ = lyxRead(lex, inherit_font);
			// If you want to define labelfont, you need to do so after
			// font is defined.
			labelfont_ = font_;
			break;
		}
		case IL_BGCOLOR:
			lex >> tmp;
			bgcolor_ = lcolor.getFromLyXName(tmp);
			break;
		case IL_PREAMBLE:
			preamble_ = from_utf8(lex.getLongString("EndPreamble"));
			break;
		case IL_HTMLTAG:
			lex >> htmltag_;
			break;
		case IL_HTMLATTR:
			lex >> htmlattr_;
			break;
		case IL_HTMLFORCECSS:
			lex >> htmlforcecss_;
			break;
		case IL_HTMLINNERTAG:
			lex >> htmlinnertag_;
			break;
		case IL_HTMLINNERATTR:
			lex >> htmlinnerattr_;
			break;
		case IL_HTMLLABEL:
			lex >> htmllabel_;
			break;
		case IL_HTMLISBLOCK:
			lex >> htmlisblock_;
			break;
		case IL_HTMLSTYLE:
			htmlstyle_ = from_utf8(lex.getLongString("EndHTMLStyle"));
			break;
		case IL_HTMLPREAMBLE:
			htmlpreamble_ = from_utf8(lex.getLongString("EndPreamble"));
			break;
		case IL_REQUIRES: {
			lex.eatLine();
			vector<string> const req 
				= support::getVectorFromString(lex.getString());
			requires_.insert(req.begin(), req.end());
			break;
		}
		case IL_END:
			getout = true;
			break;
		}
	}

	// Here add element to list if getout == true
	if (!getout)
		return false;
	
	// The label font is generally used as-is without
	// any realization against a given context.
	labelfont_.realize(sane_font);

	lex.popTable();
	return true;
}


InsetLayout::InsetLyXType translateLyXType(std::string const & str) 
{
	
	if (support::compare_ascii_no_case(str, "charstyle") == 0)
		return InsetLayout::CHARSTYLE;
	if (support::compare_ascii_no_case(str, "custom") == 0)
		return InsetLayout::CUSTOM;
	if (support::compare_ascii_no_case(str, "element") == 0)
		return InsetLayout::ELEMENT;
	if (support::compare_ascii_no_case(str, "end") == 0)
		return InsetLayout::END;
	if (support::compare_ascii_no_case(str, "standard") == 0)
		return InsetLayout::STANDARD;
	return InsetLayout::NOLYXTYPE;
}


string const & InsetLayout::htmlattr() const
{
	if (htmlattr_.empty())
		htmlattr_ = "class=\"" + defaultCSSClass() + "\"";
	return htmlattr_; 
}


string const & InsetLayout::htmlinnerattr() const
{
	if (htmlinnerattr_.empty())
		htmlinnerattr_ = "class=\"" + defaultCSSClass() + "_inner\"";
	return htmlinnerattr_; 
}


string InsetLayout::defaultCSSClass() const
{ 
	if (!defaultcssclass_.empty())
		return defaultcssclass_;
	string d;
	string n = to_utf8(name());
	string::const_iterator it = n.begin();
	string::const_iterator en = n.end();
	for (; it != en; ++it) {
		if (!isalpha(*it))
			d += "_";
		else if (islower(*it))
			d += *it;
		else
			d += support::lowercase(*it);
	}
	// are there other characters we need to remove?
	defaultcssclass_ = d;
	return defaultcssclass_;
}


void InsetLayout::makeDefaultCSS() const
{
	if (!htmldefaultstyle_.empty()) 
		return;
	if (!htmltag_.empty()) {
		docstring const mainfontCSS = font_.asCSS();
		if (!mainfontCSS.empty())
			htmldefaultstyle_ = 
					from_ascii(htmltag() + "." + defaultCSSClass() + " {\n") +
					mainfontCSS + from_ascii("\n}\n");
	}
	/* 
	At present, we do not have default tags, etc, for the label.
	if (labelfont_ == font_)
			return;
	docstring const labelfontCSS = labelfont_.asCSS();
	if (!labelfontCSS.empty())
		htmldefaultstyle_ +=
.				from_ascii(htmllabeltag() + "." + defaultCSSLabelClass() + " {\n") +
				labelfontCSS + from_ascii("\n}\n");
	*/
}

docstring InsetLayout::htmlstyle() const 
{ 
	if (!htmlstyle_.empty() && !htmlforcecss_)
		return htmlstyle_;
	if (htmldefaultstyle_.empty()) 
		makeDefaultCSS();
	docstring retval = htmldefaultstyle_;
	if (!htmlstyle_.empty())
		retval += '\n' + htmlstyle_;
	return retval;
}


} //namespace lyx
