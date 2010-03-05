/**
 * \file InsetFloatList.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetFloatList.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "DispatchResult.h"
#include "Floating.h"
#include "FloatList.h"
#include "FuncRequest.h"
#include "LaTeXFeatures.h"
#include "Lexer.h"
#include "MetricsInfo.h"
#include "Paragraph.h"
#include "output_xhtml.h"
#include "TextClass.h"
#include "TocBackend.h"

#include "support/debug.h"
#include "support/gettext.h"
#include "support/lstrings.h"

#include <ostream>

using namespace std;
using namespace lyx::support;

namespace lyx {


InsetFloatList::InsetFloatList(Buffer * buf)
	: InsetCommand(buf, InsetCommandParams(FLOAT_LIST_CODE), "toc")
{}


InsetFloatList::InsetFloatList(Buffer * buf, string const & type)
	: InsetCommand(buf, InsetCommandParams(FLOAT_LIST_CODE), "toc")
{
	setParam("type", from_ascii(type));
}


ParamInfo const & InsetFloatList::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("type", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


//HACK
bool InsetFloatList::isCompatibleCommand(string const & s)
{
	string str = s.substr(0, 6);
	return str == "listof";
}


docstring InsetFloatList::screenLabel() const
{
	FloatList const & floats = buffer().params().documentClass().floats();
	FloatList::const_iterator it = floats[to_ascii(getParam("type"))];
	if (it != floats.end())
		return buffer().B_(it->second.listName());
	else
		return _("ERROR: Nonexistent float type!");
}


void InsetFloatList::write(ostream & os) const
{
	os << "FloatList " << to_ascii(getParam("type")) << "\n";
}


void InsetFloatList::read(Lexer & lex)
{
	lex.setContext("InsetFloatList::read");
	FloatList const & floats = buffer().params().documentClass().floats();
	string token;

	if (lex.eatLine()) {
		setParam("type", lex.getDocString());
		LYXERR(Debug::INSETS, "FloatList::float_type: "
				      << to_ascii(getParam("type")));
		if (!floats.typeExist(to_ascii(getParam("type"))))
			lex.printError("Unknown float type");
	} else {
		lex.printError("Parse error");
	}

	while (lex.isOK()) {
		lex.next();
		token = lex.getString();
		if (token == "\\end_inset")
			break;
	}
	if (token != "\\end_inset") {
		lex.printError("Missing \\end_inset at this point.");
	}
}


int InsetFloatList::latex(odocstream & os, OutputParams const &) const
{
	FloatList const & floats = buffer().params().documentClass().floats();
	FloatList::const_iterator cit = floats[to_ascii(getParam("type"))];

	if (cit != floats.end()) {
		if (!cit->second.needsFloatPkg()) {
			// Only two different types allowed here:
			string const type = cit->second.floattype();
			if (type == "table") {
				os << "\\listoftables\n";
			} else if (type == "figure") {
				os << "\\listoffigures\n";
			} else {
				os << "%% unknown builtin float\n";
			}
		} else {
			os << "\\listof{" << getParam("type") << "}{"
			   << buffer().B_(cit->second.listName()) << "}\n";
		}
	} else {
		os << "%%\\listof{" << getParam("type") << "}{"
		   << bformat(_("List of %1$s"), from_utf8(cit->second.name()))
		   << "}\n";
	}
	return 1;
}


int InsetFloatList::plaintext(odocstream & os, OutputParams const &) const
{
	os << screenLabel() << "\n\n";

	buffer().tocBackend().writePlaintextTocList(to_ascii(getParam("type")), os);

	return PLAINTEXT_NEWLINE;
}


docstring InsetFloatList::xhtml(XHTMLStream &, OutputParams const &) const {
	FloatList const & floats = buffer().params().documentClass().floats();
	FloatList::const_iterator cit = floats[to_ascii(getParam("type"))];

	if (cit == floats.end()) {
		LYXERR0("Unknown float type `" << getParam("type") << "' in IFL::xhtml.");
		return docstring();
	}

	string toctype;
	docstring toclabel;
	if (!cit->second.needsFloatPkg()) {
		// Only two different types allowed here:
		string const type = cit->second.floattype();
		if (type == "table") {
			toctype = "table";
			toclabel = _("List of Tables");
		} else if (type == "figure") {
			toctype = "figure";
			toclabel = _("List of Figures");
		} else {
			LYXERR0("Unknown Builtin Float!");
			return docstring();
		}
	} else {
		toctype = to_utf8(getParam("type"));
		toclabel = buffer().B_(cit->second.listName());
	}

	// FIXME Do we need to check if it exists? If so, we need a new
	// routine in TocBackend to do that.
	Toc const & toc = buffer().tocBackend().toc(toctype);
	if (toc.empty())
		return docstring();

	// we want to look like a chapter, section, or whatever.
	// so we're going to look for the layout with the minimum toclevel
	// number > 0---because we don't want Part. 
	// we'll take the first one, just because.
	// FIXME This could be specified in the layout file.
	DocumentClass const & dc = buffer().params().documentClass();
	TextClass::LayoutList::const_iterator lit = dc.begin();
	TextClass::LayoutList::const_iterator len = dc.end();
	int minlevel = 1000;
	Layout const * lay = NULL;
	for (; lit != len; ++lit) {
		int const level = lit->toclevel;
		if (level > 0 && (level == Layout::NOT_IN_TOC || level >= minlevel))
			continue;
		lay = &*lit;
		minlevel = level;
	}
	
	string const tocclass = lay ? " " + lay->defaultCSSClass(): "";
	string const tocattr = "class='tochead + toc-" + toctype + " " + tocclass + "'";
	
	// we'll use our own stream, because we are going to defer everything.
	// that's how we deal with the fact that we're probably inside a standard
	// paragraph, and we don't want to be.
	odocstringstream ods;
	XHTMLStream xs(ods);

	xs << html::StartTag("div", "class='toc'");
	xs << html::StartTag("div", tocattr) 
		 << toclabel 
		 << html::EndTag("div");
	
	Toc::const_iterator it = toc.begin();
	Toc::const_iterator const en = toc.end();
	for (; it != en; ++it) {
		Paragraph const & par = it->dit().innerParagraph();
		string const attr = "class='lyxtoc-" + toctype + "'";
		Font const dummy;
		xs << html::StartTag("div", attr);
		string const parattr = "href='#" + par.magicLabel() + "' class='tocarrow'";
		xs << it->str() << " "
		   << html::StartTag("a", parattr)
		   // FIXME XHTML 
		   // There ought to be a simple way to customize this.
		   << XHTMLStream::NextRaw() << "&seArr;"
		   << html::EndTag("a");
		xs << html::EndTag("div");
	}
	xs << html::EndTag("div");
	return ods.str();
}


void InsetFloatList::validate(LaTeXFeatures & features) const
{
	features.useFloat(to_ascii(getParam("type")));
}


} // namespace lyx
