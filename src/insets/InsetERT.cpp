/**
 * \file InsetERT.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetERT.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Cursor.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "Language.h"
#include "Layout.h"
#include "LyXAction.h"
#include "Lexer.h"
#include "TextClass.h"
#include "MetricsInfo.h"
#include "ParagraphParameters.h"
#include "Paragraph.h"

#include "frontends/alert.h"

#include "support/debug.h"
#include "support/gettext.h"
#include "support/lstrings.h"

#include <sstream>

using namespace std;
using namespace lyx::support;

namespace lyx {


InsetERT::InsetERT(BufferParams const & bp, CollapseStatus status)
	: InsetCollapsable(bp, status)
{}


InsetERT::InsetERT(InsetERT const & in)
	: InsetCollapsable(in)
{}


Inset * InsetERT::clone() const
{
	return new InsetERT(*this);
}


InsetERT::~InsetERT()
{
	InsetERTMailer(*this).hideDialog();
}


void InsetERT::write(Buffer const & buf, ostream & os) const
{
	os << "ERT" << "\n";
	InsetCollapsable::write(buf, os);
}


docstring const InsetERT::editMessage() const
{
	return _("Opened ERT Inset");
}


int InsetERT::latex(Buffer const & buf, odocstream & os,
		    OutputParams const & op) const
{
	return InsetCollapsable::latex(buf, os, op);
}


int InsetERT::plaintext(Buffer const &, odocstream &,
			OutputParams const &) const
{
	return 0; // do not output TeX code
}


int InsetERT::docbook(Buffer const &, odocstream & os,
		      OutputParams const &) const
{
	// FIXME can we do the same thing here as for LaTeX?
	ParagraphList::const_iterator par = paragraphs().begin();
	ParagraphList::const_iterator end = paragraphs().end();

	int lines = 0;
	while (par != end) {
		pos_type siz = par->size();
		for (pos_type i = 0; i < siz; ++i)
			os.put(par->getChar(i));
		++par;
		if (par != end) {
			os << "\n";
			++lines;
		}
	}

	return lines;
}


void InsetERT::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	BufferParams const & bp = cur.buffer().params();
	LayoutPtr const layout =
			bp.textClass().emptyLayout();
	//lyxerr << "\nInsetERT::doDispatch (begin): cmd: " << cmd << endl;
	switch (cmd.action) {

	case LFUN_MOUSE_PRESS:
		if (cmd.button() != mouse_button::button3)
			InsetCollapsable::doDispatch(cur, cmd);
		else
			// This makes the cursor leave the
			// inset when it collapses on mouse-3
			cur.undispatched();
		break;

	case LFUN_QUOTE_INSERT: {
		// We need to bypass the fancy quotes in Text
		FuncRequest f(LFUN_SELF_INSERT, "\"");
		dispatch(cur, f);
		break;
	}
	case LFUN_INSET_MODIFY: {
		InsetCollapsable::CollapseStatus st;
		InsetERTMailer::string2params(to_utf8(cmd.argument()), st);
		setStatus(cur, st);
		break;
	}
	default:
		// Force any new text to latex_language
		// FIXME: This should not be necessary but
		// new paragraphs that are created by pressing enter at the
		// start of an existing paragraph get the buffer language
		// and not latex_language, so we take this brute force
		// approach.
		cur.current_font.fontInfo() = layout->font;
		cur.real_current_font.fontInfo() = layout->font;
		InsetCollapsable::doDispatch(cur, cmd);
		break;
	}
}


bool InsetERT::getStatus(Cursor & cur, FuncRequest const & cmd,
	FuncStatus & status) const
{
	switch (cmd.action) {
		case LFUN_CLIPBOARD_PASTE:
		case LFUN_INSET_MODIFY:
		case LFUN_PASTE:
		case LFUN_PRIMARY_SELECTION_PASTE:
		case LFUN_QUOTE_INSERT:
			status.enabled(true);
			return true;

		// this one is difficult to get right. As a half-baked
		// solution, we consider only the first action of the sequence
		case LFUN_COMMAND_SEQUENCE: {
			// argument contains ';'-terminated commands
			string const firstcmd = token(to_utf8(cmd.argument()), ';', 0);
			FuncRequest func(lyxaction.lookupFunc(firstcmd));
			func.origin = cmd.origin;
			return getStatus(cur, func, status);
		}

		default:
			return InsetCollapsable::getStatus(cur, cmd, status);
	}
}


void InsetERT::setButtonLabel()
{
	if (decoration() == InsetLayout::Classic)
		setLabel(isOpen() ? _("ERT") : getNewLabel(_("ERT")));
	else
		setLabel(getNewLabel(_("ERT")));
}


bool InsetERT::insetAllowed(InsetCode /* code */) const
{
	return false;
}


void InsetERT::draw(PainterInfo & pi, int x, int y) const
{
	const_cast<InsetERT &>(*this).setButtonLabel();
	InsetCollapsable::draw(pi, x, y);
}


bool InsetERT::showInsetDialog(BufferView * bv) const
{
	InsetERTMailer(const_cast<InsetERT &>(*this)).showDialog(bv);
	return true;
}


string const InsetERTMailer::name_("ert");

InsetERTMailer::InsetERTMailer(InsetERT & inset)
	: inset_(inset)
{}


string const InsetERTMailer::inset2string(Buffer const &) const
{
	return params2string(inset_.status());
}


void InsetERTMailer::string2params(string const & in,
				   InsetCollapsable::CollapseStatus & status)
{
	status = InsetCollapsable::Collapsed;
	if (in.empty())
		return;

	istringstream data(in);
	Lexer lex(0,0);
	lex.setStream(data);

	string name;
	lex >> name;
	if (name != name_)
		return print_mailer_error("InsetERTMailer", in, 1, name_);

	int s;
	lex >> s;
	if (lex)
		status = static_cast<InsetCollapsable::CollapseStatus>(s);
}


string const
InsetERTMailer::params2string(InsetCollapsable::CollapseStatus status)
{
	ostringstream data;
	data << name_ << ' ' << status;
	return data.str();
}


} // namespace lyx
