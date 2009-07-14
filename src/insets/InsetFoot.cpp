/**
 * \file InsetFoot.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jürgen Vigna
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetFoot.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "Counters.h"
#include "Language.h"
#include "Layout.h"
#include "OutputParams.h"
#include "ParIterator.h"
#include "TextClass.h"
#include "TocBackend.h"

#include "support/debug.h"
#include "support/docstream.h"
#include "support/gettext.h"
#include "support/lstrings.h"

using namespace std;

namespace lyx {

using support::bformat;

InsetFoot::InsetFoot(Buffer const & buf)
	: InsetFootlike(buf)
{}


void InsetFoot::updateLabels(ParIterator const & it)
{
	BufferParams const & bp = buffer().masterBuffer()->params();
	Counters & cnts = bp.documentClass().counters();
	docstring const foot = from_ascii("footnote");
	Paragraph const & outer =  it.paragraph();
	if (!outer.layout().intitle && cnts.hasCounter(foot)) {
		cnts.step(foot);
		// FIXME: the counter should format itself.
		custom_label_= bformat(from_utf8("%1$s %2$s"),
				       translateIfPossible(getLayout().labelstring()),
				       cnts.theCounter(foot, outer.getParLanguage(bp)->code()));
		setLabel(custom_label_);
	
	}
	InsetCollapsable::updateLabels(it);
}


void InsetFoot::addToToc(DocIterator const & cpit)
{
	DocIterator pit = cpit;
	pit.push_back(CursorSlice(*this));

	Toc & toc = buffer().tocBackend().toc("footnote");
	docstring str;
	str = custom_label_ + ": " + getNewLabel(str);
	toc.push_back(TocItem(pit, 0, str));
	// Proceed with the rest of the inset.
	InsetFootlike::addToToc(cpit);
}


docstring InsetFoot::toolTip(BufferView const & bv, int x, int y) const
{
	docstring default_tip = InsetCollapsable::toolTip(bv, x, y);
	if (!isOpen(bv))
		return custom_label_ + "\n" + default_tip;
	return default_tip;
}


int InsetFoot::latex(odocstream & os, OutputParams const & runparams_in) const
{
	OutputParams runparams = runparams_in;
	// footnotes in titling commands like \title have moving arguments
	runparams.moving_arg |= runparams_in.intitle;

	// in titling commands, \thanks should be used instead of \footnote.
	// some classes (e.g. memoir) do not understand \footnote.
	if (runparams_in.intitle)
		os << "%\n\\thanks{";
	else
		os << "%\n\\footnote{";

	int const i = InsetText::latex(os, runparams);
	os << "%\n}";
	runparams_in.encoding = runparams.encoding;

	return i + 2;
}


int InsetFoot::plaintext(odocstream & os, OutputParams const & runparams) const
{
	os << '[' << buffer().B_("footnote") << ":\n";
	InsetText::plaintext(os, runparams);
	os << "\n]";

	return PLAINTEXT_NEWLINE + 1; // one char on a separate line
}


int InsetFoot::docbook(odocstream & os, OutputParams const & runparams) const
{
	os << "<footnote>";
	int const i = InsetText::docbook(os, runparams);
	os << "</footnote>";

	return i;
}

} // namespace lyx
