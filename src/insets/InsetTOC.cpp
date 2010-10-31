/**
 * \file InsetTOC.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetTOC.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "DispatchResult.h"
#include "Font.h"
#include "Language.h"
#include "LaTeXFeatures.h"
#include "OutputParams.h"
#include "output_xhtml.h"
#include "Paragraph.h"
#include "ParagraphParameters.h"
#include "TextClass.h"
#include "TocBackend.h"

#include "support/debug.h"
#include "support/gettext.h"

#include <ostream>

using namespace std;

namespace lyx {


InsetTOC::InsetTOC(Buffer * buf, InsetCommandParams const & p)
	: InsetCommand(buf, p)
{}


ParamInfo const & InsetTOC::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("type", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


docstring InsetTOC::screenLabel() const
{
	if (getCmdName() == "tableofcontents")
		return buffer().B_("Table of Contents");
	return _("Unknown TOC type");
}


int InsetTOC::plaintext(odocstream & os, OutputParams const &) const
{
	os << screenLabel() << "\n\n";
	buffer().tocBackend().writePlaintextTocList(getCmdName(), os);
	return PLAINTEXT_NEWLINE;
}


int InsetTOC::docbook(odocstream & os, OutputParams const &) const
{
	if (getCmdName() == "tableofcontents")
		os << "<toc></toc>";
	return 0;
}


docstring InsetTOC::xhtml(XHTMLStream &, OutputParams const & op) const
{
	Layout const & lay = buffer().params().documentClass().htmlTOCLayout();
	string const & tocclass = lay.defaultCSSClass();
	string const tocattr = "class='tochead " + tocclass + "'";
	
	// we'll use our own stream, because we are going to defer everything.
	// that's how we deal with the fact that we're probably inside a standard
	// paragraph, and we don't want to be.
	odocstringstream ods;
	XHTMLStream xs(ods);

	Toc const & toc = buffer().tocBackend().toc("tableofcontents");
	if (toc.empty())
		return docstring();

	xs << html::StartTag("div", "class='toc'");

	// Title of TOC
	Language const * lang = buffer().params().language;
	static string toctitle = N_("Table of Contents");
	docstring title = lang 
			? translateIfPossible(from_ascii(toctitle), lang->code())
			: translateIfPossible(from_ascii(toctitle));
	xs << html::StartTag("div", tocattr)
		 << title
		 << html::EndTag("div");

	// Output of TOC
	Toc::const_iterator it = toc.begin();
	Toc::const_iterator const en = toc.end();
	int lastdepth = 0;
	for (; it != en; ++it) {
		// First, we need to manage increases and decreases of depth
		int const depth = it->depth();
		
		// Ignore stuff above the tocdepth
		if (depth > buffer().params().tocdepth)
			continue;
		
		if (depth > lastdepth) {
			xs.cr();
			// open as many tags as we need to open to get to this level
			// this includes the tag for the current level
			for (int i = lastdepth + 1; i <= depth; ++i) {
				stringstream attr;
				attr << "class='lyxtoc-" << i << "'";
				xs << html::StartTag("div", attr.str());
			}
			lastdepth = depth;
		}
		else if (depth < lastdepth) {
			// close as many as we have to close to get back to this level
			// this includes closing the last tag at this level
			for (int i = lastdepth; i >= depth; --i) 
				xs << html::EndTag("div");
			// now open our tag
			stringstream attr;
			attr << "class='lyxtoc-" << depth << "'";
			xs << html::StartTag("div", attr.str());
			lastdepth = depth;
		} else {
			// no change of level, so close and open
			xs << html::EndTag("div");
			stringstream attr;
			attr << "class='lyxtoc-" << depth << "'";
			xs << html::StartTag("div", attr.str());
		}
		
		// Now output TOC info for this entry
		Paragraph const & par = it->dit().innerParagraph();
		// First the label, if there is one
		docstring const & label = par.params().labelString();
		if (!label.empty())
			xs << label << " ";
		// Now the content of the TOC entry, taken from the paragraph itself
		OutputParams ours = op;
		ours.for_toc = true;
		Font const dummy;
		par.simpleLyXHTMLOnePar(buffer(), xs, ours, dummy);
		xs << " ";
		// Now a link to that paragraph
		string const parattr = "href='#" + par.magicLabel() + "' class='tocarrow'";
		xs << html::StartTag("a", parattr);
		// FIXME XHTML 
		// There ought to be a simple way to customize this.
		// Maybe if we had an InsetLayout for TOC...
		xs << XHTMLStream::NextRaw() << "&gt;";
		xs << html::EndTag("a");		
	}
	for (int i = lastdepth; i > 0; --i) 
		xs << html::EndTag("div");
	xs << html::EndTag("div");
	return ods.str();
}


} // namespace lyx
