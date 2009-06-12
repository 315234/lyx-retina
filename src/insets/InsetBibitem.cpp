/**
 * \file InsetBibitem.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>
#include <algorithm>

#include "InsetBibitem.h"

#include "BiblioInfo.h"
#include "Buffer.h"
#include "buffer_funcs.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Counters.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "InsetIterator.h"
#include "InsetList.h"
#include "Lexer.h"
#include "output_xhtml.h"
#include "Paragraph.h"
#include "ParagraphList.h"
#include "TextClass.h"

#include "frontends/alert.h"

#include "support/convert.h"
#include "support/docstream.h"
#include "support/gettext.h"
#include "support/lstrings.h"

using namespace std;
using namespace lyx::support;

namespace lyx {


int InsetBibitem::key_counter = 0;
docstring const key_prefix = from_ascii("key-");


InsetBibitem::InsetBibitem(Buffer const & buf, InsetCommandParams const & p)
	: InsetCommand(p, "bibitem")
{
	Inset::setBuffer(const_cast<Buffer &>(buf));
	buffer_->invalidateBibinfoCache();
	if (getParam("key").empty())
		setParam("key", key_prefix + convert<docstring>(++key_counter));
}


InsetBibitem::~InsetBibitem()
{
	if (isBufferValid())
		buffer_->invalidateBibinfoCache();
}


void InsetBibitem::initView()
{
	updateCommand(getParam("key"));
}


void InsetBibitem::updateCommand(docstring const & new_key, bool)
{
	docstring const old_key = getParam("key");
	docstring key = new_key;

	vector<docstring> bibkeys = buffer().masterBibInfo().getKeys();

	int i = 1;

	if (find(bibkeys.begin(), bibkeys.end(), key) != bibkeys.end()) {
		// generate unique label
		key = new_key + '-' + convert<docstring>(i);
		while (find(bibkeys.begin(), bibkeys.end(), key) != bibkeys.end()) {
			++i;
			key = new_key + '-' + convert<docstring>(i);
		}
		frontend::Alert::warning(_("Keys must be unique!"),
			bformat(_("The key %1$s already exists,\n"
			"it will be changed to %2$s."), new_key, key));
	}
	setParam("key", key);

	buffer().updateLabels();
}


ParamInfo const & InsetBibitem::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("label", ParamInfo::LATEX_OPTIONAL);
		param_info_.add("key", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


void InsetBibitem::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY: {
		InsetCommandParams p(BIBITEM_CODE);
		InsetCommand::string2params("bibitem", to_utf8(cmd.argument()), p);
		if (p.getCmdName().empty()) {
			cur.noUpdate();
			break;
		}
		docstring const & old_key = params()["key"];
		setParam("label", p["label"]);
		if (p["key"] != old_key) {
			updateCommand(p["key"]);
			cur.bv().buffer().changeRefsIfUnique(old_key,
				params()["key"], CITE_CODE);
		}
		buffer_->invalidateBibinfoCache();
		break;
	}

	default:
		InsetCommand::doDispatch(cur, cmd);
		break;
	}
}


void InsetBibitem::read(Lexer & lex)
{
	InsetCommand::read(lex);

	if (prefixIs(getParam("key"), key_prefix)) {
		int const key = convert<int>(getParam("key").substr(key_prefix.length()));
		key_counter = max(key_counter, key);
	}
}


docstring InsetBibitem::bibLabel() const
{
	docstring const & label = getParam("label");
	return label.empty() ? autolabel_ : label;
}


docstring InsetBibitem::screenLabel() const
{
	return getParam("key") + " [" + bibLabel() + ']';
}


int InsetBibitem::plaintext(odocstream & os, OutputParams const &) const
{
	odocstringstream oss;
	oss << '[' << bibLabel() << "] ";

	docstring const str = oss.str();
	os << str;

	return str.size();
}


// ale070405
docstring bibitemWidest(Buffer const & buffer)
{
	int w = 0;

	InsetBibitem const * bitem = 0;

	// FIXME: this font is used unitialized for now but should  be set to
	// a proportional font. Here is what Georg Baum has to say about it:
	/*
	bibitemWidest() is supposed to find the bibitem with the widest label in the
	output, because that is needed as an argument of the bibliography
	environment to determine the correct indentation. To be 100% correct we
	would need the metrics of the font that is used in the output, but usually
	we don't have access to these.
	In practice, any proportional font is probably good enough, since we don't
	need to know the final with, we only need to know the which label is the
	widest.
	Unless there is an easy way to get the metrics of the output font I suggest
	to use a hardcoded font like "Times" or so.

	It is very important that the result of this function is the same both with
	and without GUI. After thinking about this it is clear that no Font
	metrics should be used here, since these come from the gui. If we can't
	easily get the LaTeX font metrics we should make our own poor mans font
	metrics replacement, e.g. by hardcoding the metrics of the standard TeX
	font.
	*/

	docstring lbl;

	ParagraphList::const_iterator it = buffer.paragraphs().begin();
	ParagraphList::const_iterator end = buffer.paragraphs().end();

	for (; it != end; ++it) {
		if (it->insetList().empty())
			continue;
		Inset * inset = it->insetList().begin()->inset;
		if (inset->lyxCode() != BIBITEM_CODE)
			continue;

		bitem = static_cast<InsetBibitem const *>(inset);
		docstring const label = bitem->bibLabel();

		// FIXME: we can't be sure using the following that the GUI
		// version and the command-line version will give the same
		// result.
		//
		//int const wx = use_gui?
		//	theFontMetrics(font).width(label): label.size();
		//
		// So for now we just use the label size in order to be sure
		// that GUI and no-GUI gives the same bibitem (even if that is
		// potentially the wrong one.
		int const wx = label.size();

		if (wx > w) {
			w = wx;
			lbl = label;
		}
	}

	if (!lbl.empty())
		return lbl;

	return from_ascii("99");
}


void InsetBibitem::fillWithBibKeys(BiblioInfo & keys, InsetIterator const & it) const
{
	docstring const key = getParam("key");
	BibTeXInfo keyvalmap(false);
	keyvalmap.label(bibLabel());
	DocIterator doc_it(it); 
	doc_it.forwardPos();
	keyvalmap[from_ascii("ref")] = doc_it.paragraph().asString();
	keys[key] = keyvalmap;
}


// Update the counters of this inset and of its contents
void InsetBibitem::updateLabels(ParIterator const &) 
{
	Counters & counters = buffer().masterBuffer()->params().documentClass().counters();
	docstring const bibitem = from_ascii("bibitem");
	if (counters.hasCounter(bibitem) && getParam("label").empty()) {
		counters.step(bibitem);
		autolabel_ = counters.theCounter(bibitem);
	} else {
		autolabel_ = from_ascii("??");
	}
}


void InsetBibitem::xhtml(odocstream & os, OutputParams const &) const
{
	os << "<a name='" << html::htmlize(getParam("key")) << "'></a>";
	os << "<span class='biblabel'>" << bibLabel() << "</span> "; 
}


} // namespace lyx
