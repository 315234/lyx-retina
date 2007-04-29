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

#include "Buffer.h"
#include "BufferView.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "InsetList.h"
#include "Text.h"
#include "Paragraph.h"
#include "ParIterator.h"
#include "sgml.h"

#include "support/lstrings.h"
#include "support/lyxalgo.h"
#include "support/std_ostream.h"


namespace lyx {

using support::escape;

using std::string;
using std::ostream;
using std::vector;


InsetLabel::InsetLabel(InsetCommandParams const & p)
	: InsetCommand(p, "label")
{}


std::auto_ptr<Inset> InsetLabel::doClone() const
{
	return std::auto_ptr<Inset>(new InsetLabel(params()));
}


void InsetLabel::getLabelList(Buffer const &, std::vector<docstring> & list) const
{
	list.push_back(getParam("name"));
}


docstring const InsetLabel::getScreenLabel(Buffer const &) const
{
	return getParam("name");
}


void InsetLabel::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY: {
		InsetCommandParams p("label");
		// FIXME UNICODE
		InsetCommandMailer::string2params("label", to_utf8(cmd.argument()), p);
		if (p.getCmdName().empty()) {
			cur.noUpdate();
			break;
		}
		if (p["name"] != params()["name"])
			cur.bv().buffer()->changeRefsIfUnique(params()["name"],
					p["name"], Inset::REF_CODE);
		setParams(p);
		break;
	}

	default:
		InsetCommand::doDispatch(cur, cmd);
		break;
	}
}


int InsetLabel::latex(Buffer const &, odocstream & os,
                      OutputParams const &) const
{
	os << escape(getCommand());
	return 0;
}


int InsetLabel::plaintext(Buffer const &, odocstream & os,
                          OutputParams const &) const
{
	docstring const str = getParam("name");
	os << '<' << str << '>';
	return 2 + str.size();
}


int InsetLabel::docbook(Buffer const & buf, odocstream & os,
                        OutputParams const & runparams) const
{
	os << "<!-- anchor id=\""
           << sgml::cleanID(buf, runparams, getParam("name"))
           << "\" -->";
	return 0;
}


} // namespace lyx
