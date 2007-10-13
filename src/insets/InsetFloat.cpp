/**
 * \file InsetFloat.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetFloat.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Counters.h"
#include "Cursor.h"
#include "debug.h"
#include "DispatchResult.h"
#include "Floating.h"
#include "FloatList.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "gettext.h"
#include "LaTeXFeatures.h"
#include "Lexer.h"
#include "OutputParams.h"

#include "support/lstrings.h"
#include "support/convert.h"


namespace lyx {

using std::endl;
using std::string;
using std::istringstream;
using std::ostream;
using std::ostringstream;


// With this inset it will be possible to support the latex package
// float.sty, and I am sure that with this and some additional support
// classes we can support similar functionality in other formats
// (read DocBook).
// By using float.sty we will have the same handling for all floats, both
// for those already in existance (table and figure) and all user created
// ones�. So suddenly we give the users the possibility of creating new
// kinds of floats on the fly. (and with a uniform look)
//
// API to float.sty:
//   \newfloat{type}{placement}{ext}[within]
//     type      - The "type" of the new class of floats, like program or
//                 algorithm. After the appropriate \newfloat, commands
//                 such as \begin{program} or \end{algorithm*} will be
//                 available.
//     placement - The default placement for the given class of floats.
//                 They are like in standard LaTeX: t, b, p and h for top,
//                 bottom, page, and here, respectively. On top of that
//                 there is a new type, H, which does not really correspond
//                 to a float, since it means: put it "here" and nowhere else.
//                 Note, however that the H specifier is special and, because
//                 of implementation details cannot be used in the second
//                 argument of \newfloat.
//     ext       - The file name extension of an auxiliary file for the list
//                 of figures (or whatever). LaTeX writes the captions to
//                 this file.
//     within    - This (optional) argument determines whether floats of this
//                 class will be numbered within some sectional unit of the
//                 document. For example, if within is equal to chapter, the
//                 floats will be numbered within chapters.
//   \floatstyle{style}
//     style -  plain, boxed, ruled
//   \floatname{float}{floatname}
//     float     -
//     floatname -
//   \floatplacement{float}{placement}
//     float     -
//     placement -
//   \restylefloat{float}
//     float -
//   \listof{type}{title}
//     title -

// � the algorithm float is defined using the float.sty package. Like this
//   \floatstyle{ruled}
//   \newfloat{algorithm}{htbp}{loa}[<sect>]
//   \floatname{algorithm}{Algorithm}
//
// The intention is that floats should be definable from two places:
//          - layout files
//          - the "gui" (i.e. by the user)
//
// From layout files.
// This should only be done for floats defined in a documentclass and that
// does not need any additional packages. The two most known floats in this
// category is "table" and "figure". Floats defined in layout files are only
// stored in lyx files if the user modifies them.
//
// By the user.
// There should be a gui dialog (and also a collection of lyxfuncs) where
// the user can modify existing floats and/or create new ones.
//
// The individual floats will also have some settable
// variables: wide and placement.
//
// Lgb


InsetFloat::InsetFloat(BufferParams const & bp, string const & type)
	: InsetCollapsable(bp), name_(from_utf8(type))
{
	setLabel(_("float: ") + floatName(type, bp));
	Font font(Font::ALL_SANE);
	font.decSize();
	font.decSize();
	font.setColor(Color::collapsable);
	setLabelFont(font);
	params_.type = type;
}


InsetFloat::~InsetFloat()
{
	InsetFloatMailer(*this).hideDialog();
}


void InsetFloat::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY: {
		InsetFloatParams params;
		InsetFloatMailer::string2params(to_utf8(cmd.argument()), params);
		params_.placement = params.placement;
		params_.wide      = params.wide;
		params_.sideways  = params.sideways;
		wide(params_.wide, cur.buffer().params());
		sideways(params_.sideways, cur.buffer().params());
		break;
	}

	case LFUN_INSET_DIALOG_UPDATE: {
		InsetFloatMailer(*this).updateDialog(&cur.bv());
		break;
	}

	case LFUN_MOUSE_RELEASE: {
		if (cmd.button() == mouse_button::button3 && hitButton(cmd)) {
			InsetFloatMailer(*this).showDialog(&cur.bv());
			break;
		}
		InsetCollapsable::doDispatch(cur, cmd);
		break;
	}

	default:
		InsetCollapsable::doDispatch(cur, cmd);
		break;
	}
}


bool InsetFloat::getStatus(Cursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY:
	case LFUN_INSET_DIALOG_UPDATE:
		flag.enabled(true);
		return true;

	default:
		return InsetCollapsable::getStatus(cur, cmd, flag);
	}
}


void InsetFloat::updateLabels(Buffer const & buf, ParIterator const & it)
{
	Counters & cnts = buf.params().getTextClass().counters();
	string const saveflt = cnts.current_float();

	// Tell to captions what the current float is
	cnts.current_float(params().type);

	InsetCollapsable::updateLabels(buf, it);

	//reset afterwards
	cnts.current_float(saveflt);
}


void InsetFloatParams::write(ostream & os) const
{
	os << "Float " << type << '\n';

	if (!placement.empty())
		os << "placement " << placement << "\n";

	if (wide)
		os << "wide true\n";
	else
		os << "wide false\n";

	if (sideways)
		os << "sideways true\n";
	else
		os << "sideways false\n";
}


void InsetFloatParams::read(Lexer & lex)
{
	string token;
	lex >> token;
	if (token == "placement") {
		lex >> placement;
	} else {
		// take countermeasures
		lex.pushToken(token);
	}
	lex >> token;
	if (token == "wide") {
		lex >> wide;
	} else {
		lyxerr << "InsetFloat::Read:: Missing wide!"
		<< endl;
		// take countermeasures
		lex.pushToken(token);
	}
	lex >> token;
	if (token == "sideways") {
		lex >> sideways;
	} else {
		lyxerr << "InsetFloat::Read:: Missing sideways!"
		<< endl;
		// take countermeasures
		lex.pushToken(token);
	}
}


void InsetFloat::write(Buffer const & buf, ostream & os) const
{
	params_.write(os);
	InsetCollapsable::write(buf, os);
}


void InsetFloat::read(Buffer const & buf, Lexer & lex)
{
	params_.read(lex);
	wide(params_.wide, buf.params());
	sideways(params_.sideways, buf.params());
	InsetCollapsable::read(buf, lex);
}


void InsetFloat::validate(LaTeXFeatures & features) const
{
	if (support::contains(params_.placement, 'H')) {
		features.require("float");
	}

	if (params_.sideways)
		features.require("rotating");

	features.useFloat(params_.type);
	InsetCollapsable::validate(features);
}


Inset * InsetFloat::clone() const
{
	return new InsetFloat(*this);
}


docstring const InsetFloat::editMessage() const
{
	return _("Opened Float Inset");
}


int InsetFloat::latex(Buffer const & buf, odocstream & os,
		      OutputParams const & runparams) const
{
	FloatList const & floats = buf.params().getTextClass().floats();
	string tmptype = (params_.wide ? params_.type + "*" : params_.type);
	if (params_.sideways) {
		if (params_.type == "table")
			tmptype = "sidewaystable";
		else if (params_.type == "figure")
			tmptype = "sidewaysfigure";
	}
	// Figure out the float placement to use.
	// From lowest to highest:
	// - float default placement
	// - document wide default placement
	// - specific float placement
	string placement;
	string const buf_placement = buf.params().float_placement;
	string const def_placement = floats.defaultPlacement(params_.type);
	if (!params_.placement.empty()
	    && params_.placement != def_placement) {
		placement = params_.placement;
	} else if (params_.placement.empty()
		   && !buf_placement.empty()
		   && buf_placement != def_placement) {
		placement = buf_placement;
	}

	// The \n is used to force \begin{<floatname>} to appear in a new line.
	// The % is needed to prevent two consecutive \n chars in the case
	// when the current output line is empty.
	os << "%\n\\begin{" << from_ascii(tmptype) << '}';
	// We only output placement if different from the def_placement.
	// sidewaysfloats always use their own page
	if (!placement.empty() && !params_.sideways) {
		os << '[' << from_ascii(placement) << ']';
	}
	os << '\n';

	int const i = InsetText::latex(buf, os, runparams);

	// The \n is used to force \end{<floatname>} to appear in a new line.
	// In this case, we do not case if the current output line is empty.
	os << "\n\\end{" << from_ascii(tmptype) << "}\n";

	return i + 4;
}


int InsetFloat::plaintext(Buffer const & buf, odocstream & os,
			  OutputParams const & runparams) const
{
	os << '[' << buf.B_("float") << ' ' << floatName(params_.type, buf.params()) << ":\n";
	InsetText::plaintext(buf, os, runparams);
	os << "\n]";

	return PLAINTEXT_NEWLINE + 1; // one char on a separate line
}


int InsetFloat::docbook(Buffer const & buf, odocstream & os,
			OutputParams const & runparams) const
{
	// FIXME UNICODE
	os << '<' << from_ascii(params_.type) << '>';
	int const i = InsetText::docbook(buf, os, runparams);
	os << "</" << from_ascii(params_.type) << '>';

	return i;
}


bool InsetFloat::insetAllowed(InsetCode code) const
{
	return code != FLOAT_CODE
	    && code != FOOT_CODE
	    && code != MARGIN_CODE;
}


bool InsetFloat::showInsetDialog(BufferView * bv) const
{
	if (!InsetText::showInsetDialog(bv))
		InsetFloatMailer(const_cast<InsetFloat &>(*this)).showDialog(bv);
	return true;
}


void InsetFloat::wide(bool w, BufferParams const & bp)
{
	params_.wide = w;
	docstring lab = _("float: ") + floatName(params_.type, bp);
	if (params_.wide)
		lab += '*';
	setLabel(lab);
}


void InsetFloat::sideways(bool s, BufferParams const & bp)
{
	params_.sideways = s;
	docstring lab = _("float: ") + floatName(params_.type, bp);
	if (params_.sideways)
		lab += _(" (sideways)");
	setLabel(lab);
}


string const InsetFloatMailer::name_("float");

InsetFloatMailer::InsetFloatMailer(InsetFloat & inset)
	: inset_(inset)
{}


string const InsetFloatMailer::inset2string(Buffer const &) const
{
	return params2string(inset_.params());
}


void InsetFloatMailer::string2params(string const & in,
				     InsetFloatParams & params)
{
	params = InsetFloatParams();
	if (in.empty())
		return;

	istringstream data(in);
	Lexer lex(0,0);
	lex.setStream(data);

	string name;
	lex >> name;
	if (!lex || name != name_)
		return print_mailer_error("InsetFloatMailer", in, 1, name_);

	// This is part of the inset proper that is usually swallowed
	// by Text::readInset
	string id;
	lex >> id;
	if (!lex || id != "Float")
		return print_mailer_error("InsetBoxMailer", in, 2, "Float");

	// We have to read the type here!
	lex >> params.type;
	params.read(lex);
}


string const InsetFloatMailer::params2string(InsetFloatParams const & params)
{
	ostringstream data;
	data << name_ << ' ';
	params.write(data);
	return data.str();
}


} // namespace lyx
