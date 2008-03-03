/**
 * \file InsetLabel.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetLabel.h"

#include "InsetRef.h"

#include "buffer_funcs.h"
#include "Buffer.h"
#include "BufferView.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "InsetIterator.h"
#include "ParIterator.h"
#include "sgml.h"
#include "Text.h"
#include "TocBackend.h"

#include "frontends/alert.h"

#include "support/convert.h"
#include "support/lyxalgo.h"
#include "support/gettext.h"
#include "support/lstrings.h"

using namespace std;
using namespace lyx::support;

namespace lyx {


InsetLabel::InsetLabel(InsetCommandParams const & p)
	: InsetCommand(p, "label")
{}


void InsetLabel::validate()
{
	update(getParam("name"));
}


void InsetLabel::update(docstring const & new_label)
{
	docstring const old_label = getParam("name");
	docstring label = new_label;
	int i = 1;
	while (buffer().insetLabel(label)) {
		label = new_label + '-' + convert<docstring>(i);
		++i;
	}

	if (label != new_label) {
		// Warn the user that the label has been changed to something else.
		frontend::Alert::warning(_("Label names must be unique!"),
			bformat(_("The label %1$s already exists,\n"
			"it will been changed to %2$s."), new_label, label));
	}

	setParam("name", label);

	Buffer::References const & refs = buffer().references(old_label);
	Buffer::References::const_iterator it = refs.begin();
	Buffer::References::const_iterator end = refs.end();
	for (; it != end; ++it)
		it->first->setParam("reference", label);

	// We need an update of the Buffer reference cache. This is achieved by
	// updateLabel().
	lyx::updateLabels(buffer());
}


ParamInfo const & InsetLabel::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("name", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


void InsetLabel::getLabelList(vector<docstring> & list) const
{
	list.push_back(getParam("name"));
}


docstring InsetLabel::screenLabel() const
{
	return getParam("name");
}


void InsetLabel::updateLabels(ParIterator const & it)
{
	docstring const & label = getParam("name");
	if (buffer().insetLabel(label))
		// Problem: We already have an InsetLabel with the same name!
		return;
	buffer().setInsetLabel(label, this);
}


void InsetLabel::addToToc(ParConstIterator const & cpit) const
{
	docstring const & label = getParam("name");
	Toc & toc = buffer().tocBackend().toc("label");
	if (buffer().insetLabel(label) != this) {
		toc.push_back(TocItem(cpit, 0, _("DUPLICATE: ") + label));
		return;
	}
	toc.push_back(TocItem(cpit, 0, label));
	Buffer::References const & refs = buffer().references(label);
	Buffer::References::const_iterator it = refs.begin();
	Buffer::References::const_iterator end = refs.end();
	for (; it != end; ++it) {
		ParConstIterator const ref_pit(it->second);
		toc.push_back(TocItem(ref_pit, 1, it->first->screenLabel()));
	}
}


void InsetLabel::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY: {
		InsetCommandParams p(LABEL_CODE);
		// FIXME UNICODE
		InsetCommandMailer::string2params("label", to_utf8(cmd.argument()), p);
		if (p.getCmdName().empty()) {
			cur.noUpdate();
			break;
		}
		update(p["name"]);
		break;
	}

	default:
		InsetCommand::doDispatch(cur, cmd);
		break;
	}
}


int InsetLabel::latex(odocstream & os, OutputParams const &) const
{
	os << escape(getCommand());
	return 0;
}


int InsetLabel::plaintext(odocstream & os, OutputParams const &) const
{
	docstring const str = getParam("name");
	os << '<' << str << '>';
	return 2 + str.size();
}


int InsetLabel::docbook(odocstream & os, OutputParams const & runparams) const
{
	os << "<!-- anchor id=\""
	   << sgml::cleanID(buffer(), runparams, getParam("name"))
	   << "\" -->";
	return 0;
}


} // namespace lyx
