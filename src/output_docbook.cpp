/**
 * \file output_docbook.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jos� Matos
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "output_docbook.h"

#include "Buffer.h"
#include "buffer_funcs.h"
#include "BufferParams.h"
#include "Counters.h"
#include "support/debug.h"
#include "Layout.h"
#include "OutputParams.h"
#include "Paragraph.h"
#include "paragraph_funcs.h"
#include "ParagraphList.h"
#include "ParagraphParameters.h"
#include "sgml.h"
#include "TextClass.h"

#include "support/lstrings.h"
#include "support/lyxlib.h"
#include "support/convert.h"
#include "support/types.h"

#include <boost/next_prior.hpp>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace {

ParagraphList::const_iterator searchParagraph(
	ParagraphList::const_iterator p,
  ParagraphList::const_iterator const & pend)
{
	for (++p; p != pend && p->layout()->latextype == LATEX_PARAGRAPH; ++p)
		;

	return p;
}


ParagraphList::const_iterator searchCommand(
		ParagraphList::const_iterator p,
		ParagraphList::const_iterator const & pend)
{
	LayoutPtr const & bstyle = p->layout();

	for (++p; p != pend; ++p) {
		LayoutPtr const & style = p->layout();
		if (style->latextype == LATEX_COMMAND
				&& style->commanddepth <= bstyle->commanddepth)
			return p;
	}
	return pend;
}


ParagraphList::const_iterator searchEnvironment(
		ParagraphList::const_iterator p,
		ParagraphList::const_iterator const & pend)
{
	LayoutPtr const & bstyle = p->layout();
	size_t const depth = p->params().depth();
	for (++p; p != pend; ++p) {
		LayoutPtr const & style = p->layout();
		if (style->latextype == LATEX_COMMAND)
			return p;

		if (style->latextype == LATEX_PARAGRAPH) {
			if (p->params().depth() > depth)
				continue;
			return p;
		}

		if (p->params().depth() < depth)
			return p;

		if (style->latexname() != bstyle->latexname()
				&& p->params().depth() == depth)
			return p;
	}
	return pend;
}


ParagraphList::const_iterator makeParagraph(Buffer const & buf,
					    odocstream & os,
					    OutputParams const & runparams,
					    ParagraphList const & paragraphs,
					    ParagraphList::const_iterator const & pbegin,
					    ParagraphList::const_iterator const & pend)
{
	LayoutPtr const & defaultstyle =
		buf.params().getTextClass().defaultLayout();
	for (ParagraphList::const_iterator par = pbegin; par != pend; ++par) {
		if (par != pbegin)
			os << '\n';
		if (par->layout() == defaultstyle && par->emptyTag()) {
			par->simpleDocBookOnePar(buf, os, runparams, outerFont(std::distance(paragraphs.begin(), par), paragraphs));
		} else {
			sgml::openTag(buf, os, runparams, *par);
			par->simpleDocBookOnePar(buf, os, runparams, outerFont(std::distance(paragraphs.begin(), par), paragraphs));
			sgml::closeTag(os, *par);
		}
	}
	return pend;
}


ParagraphList::const_iterator makeEnvironment(Buffer const & buf,
					      odocstream & os,
					      OutputParams const & runparams,
					      ParagraphList const & paragraphs,
					      ParagraphList::const_iterator const & pbegin,
					      ParagraphList::const_iterator const & pend) {
	ParagraphList::const_iterator par = pbegin;

	LayoutPtr const & defaultstyle = buf.params().getTextClass().defaultLayout();
	LayoutPtr const & bstyle = par->layout();
	string item_tag;

	// Opening outter tag
	sgml::openTag(buf, os, runparams, *pbegin);
	os << '\n';
	if (bstyle->latextype == LATEX_ENVIRONMENT && bstyle->pass_thru)
		os << "<![CDATA[";

	while (par != pend) {
		LayoutPtr const & style = par->layout();
		ParagraphList::const_iterator send;
		string id = par->getID(buf, runparams);
		string wrapper = "";
		pos_type sep = 0;

		// Opening inner tag
		switch (bstyle->latextype) {
		case LATEX_ENVIRONMENT:
			if (!bstyle->innertag().empty()) {
				sgml::openTag(os, bstyle->innertag(), id);
			}
			break;

		case LATEX_ITEM_ENVIRONMENT:
			if (!bstyle->labeltag().empty()) {
				sgml::openTag(os, bstyle->innertag(), id);
				sgml::openTag(os, bstyle->labeltag());
				sep = par->getFirstWord(buf, os, runparams) + 1;
				sgml::closeTag(os, bstyle->labeltag());
			}
			wrapper = defaultstyle->latexname();
			// If a sub list (embedded list) appears next with a
			// different depth, then there is no need to open
			// another tag at the current depth.
			if(par->params().depth() == pbegin->params().depth()) {
				sgml::openTag(os, bstyle->itemtag());
			}
			break;
		default:
			break;
		}

		switch (style->latextype) {
		case LATEX_ENVIRONMENT:
		case LATEX_ITEM_ENVIRONMENT: {
			if (par->params().depth() == pbegin->params().depth()) {
				sgml::openTag(os, wrapper);
				par->simpleDocBookOnePar(buf, os, runparams, outerFont(std::distance(paragraphs.begin(), par), paragraphs), sep);
				sgml::closeTag(os, wrapper);
				++par;
			}
			else {
				send = searchEnvironment(par, pend);
				par = makeEnvironment(buf, os, runparams, paragraphs, par,send);
			}
			break;
		}
		case LATEX_PARAGRAPH:
			send = searchParagraph(par, pend);
			par = makeParagraph(buf, os, runparams, paragraphs, par,send);
			break;
		default:
			break;
		}

		// Closing inner tag
		switch (bstyle->latextype) {
		case LATEX_ENVIRONMENT:
			if (!bstyle->innertag().empty()) {
				sgml::closeTag(os, bstyle->innertag());
				os << '\n';
			}
			break;
		case LATEX_ITEM_ENVIRONMENT:
			// If a sub list (embedded list) appears next, then
			// there is no need to close the current tag.
			// par should have already been incremented to the next
			// element. So we can compare the depth of the next
			// element with pbegin.
			// We need to be careful, that we don't dereference par
			// when par == pend but at the same time that the
			// current tag is closed.
			if((par != pend && par->params().depth() == pbegin->params().depth()) || par == pend) {
				sgml::closeTag(os, bstyle->itemtag());
			}
			if (!bstyle->labeltag().empty())
				sgml::closeTag(os, bstyle->innertag());
			break;
		default:
			break;
		}
	}

	if (bstyle->latextype == LATEX_ENVIRONMENT && bstyle->pass_thru)
		os << "]]>";

	// Closing outter tag
	sgml::closeTag(os, *pbegin);

	return pend;
}


ParagraphList::const_iterator makeCommand(Buffer const & buf,
					  odocstream & os,
					  OutputParams const & runparams,
					  ParagraphList const & paragraphs,
					  ParagraphList::const_iterator const & pbegin,
					  ParagraphList::const_iterator const & pend)
{
	ParagraphList::const_iterator par = pbegin;
	LayoutPtr const & bstyle = par->layout();

	//Open outter tag
	sgml::openTag(buf, os, runparams, *pbegin);
	os << '\n';

	// Label around sectioning number:
	if (!bstyle->labeltag().empty()) {
		sgml::openTag(os, bstyle->labeltag());
		// We don't care about appendix in DOCBOOK.
		os << par->expandLabel(bstyle, buf.params(), false);
		sgml::closeTag(os, bstyle->labeltag());
	}

	// Opend inner tag and	close inner tags
	sgml::openTag(os, bstyle->innertag());
	par->simpleDocBookOnePar(buf, os, runparams,  outerFont(std::distance(paragraphs.begin(), par), paragraphs));
	sgml::closeTag(os, bstyle->innertag());
	os << '\n';

	++par;
	while (par != pend) {
		LayoutPtr const & style = par->layout();
		ParagraphList::const_iterator send;

		switch (style->latextype) {
		case LATEX_COMMAND: {
			send = searchCommand(par, pend);
			par = makeCommand(buf, os, runparams, paragraphs, par,send);
			break;
		}
		case LATEX_ENVIRONMENT:
		case LATEX_ITEM_ENVIRONMENT: {
			send = searchEnvironment(par, pend);
			par = makeEnvironment(buf, os, runparams, paragraphs, par,send);
			break;
		}
		case LATEX_PARAGRAPH:
			send = searchParagraph(par, pend);
			par = makeParagraph(buf, os, runparams, paragraphs, par,send);
			break;
		default:
			break;
		}
	}
	// Close outter tag
	sgml::closeTag(os, *pbegin);

	return pend;
}

} // end anonym namespace


void docbookParagraphs(ParagraphList const & paragraphs,
		       Buffer const & buf,
		       odocstream & os,
		       OutputParams const & runparams)
{
	ParagraphList::const_iterator par = paragraphs.begin();
	ParagraphList::const_iterator pend = paragraphs.end();

	BOOST_ASSERT(runparams.par_begin <= runparams.par_end);
	// if only part of the paragraphs will be outputed
	if (runparams.par_begin !=  runparams.par_end) {
		par = boost::next(paragraphs.begin(), runparams.par_begin);
		pend = boost::next(paragraphs.begin(), runparams.par_end);
		// runparams will be passed to nested paragraphs, so
		// we have to reset the range parameters.
		const_cast<OutputParams&>(runparams).par_begin = 0;
		const_cast<OutputParams&>(runparams).par_end = 0;
	}

	while (par != pend) {
		LayoutPtr const & style = par->layout();
		ParagraphList::const_iterator lastpar = par;
		ParagraphList::const_iterator send;

		switch (style->latextype) {
		case LATEX_COMMAND: {
			send = searchCommand(par, pend);
			par = makeCommand(buf, os, runparams, paragraphs, par,send);
			break;
		}
		case LATEX_ENVIRONMENT:
		case LATEX_ITEM_ENVIRONMENT: {
			send = searchEnvironment(par, pend);
			par = makeEnvironment(buf, os, runparams, paragraphs, par,send);
			break;
		}
		case LATEX_PARAGRAPH:
			send = searchParagraph(par, pend);
			par = makeParagraph(buf, os, runparams, paragraphs, par,send);
			break;
		default:
			break;
		}
		// makeEnvironment may process more than one paragraphs and bypass pend
		if (std::distance(lastpar, par) >= std::distance(lastpar, pend))
			break;
	}
}


} // namespace lyx
