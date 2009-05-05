/**
 * \file InsetCollapsable.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Jürgen Vigna
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetCollapsable.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Cursor.h"
#include "Dimension.h"
#include "DispatchResult.h"
#include "FloatList.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "InsetLayout.h"
#include "Language.h"
#include "LaTeXFeatures.h"
#include "Lexer.h"
#include "MetricsInfo.h"
#include "paragraph_funcs.h"
#include "ParagraphParameters.h"
#include "sgml.h"
#include "TextClass.h"

#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"

#include "support/debug.h"
#include "support/docstream.h"
#include "support/gettext.h"
#include "support/lassert.h"

using namespace std;


namespace lyx {

InsetCollapsable::CollapseStatus InsetCollapsable::status(BufferView const & bv) const
{
	if (decoration() == InsetLayout::CONGLOMERATE)
		return status_;
	return auto_open_[&bv] ? Open : status_;
}


InsetCollapsable::Geometry InsetCollapsable::geometry(BufferView const & bv) const
{
	switch (decoration()) {
	case InsetLayout::CLASSIC:
		if (status(bv) == Open)
			return openinlined_ ? LeftButton : TopButton;
		return ButtonOnly;

	case InsetLayout::MINIMALISTIC:
		return status(bv) == Open ? NoButton : ButtonOnly ;

	case InsetLayout::CONGLOMERATE:
		return status(bv) == Open ? SubLabel : Corners ;

	case InsetLayout::DEFAULT:
		break; // this shouldn't happen
	}

	// dummy return value to shut down a warning,
	// this is dead code.
	return NoButton;
}


InsetCollapsable::Geometry InsetCollapsable::geometry() const
{
	switch (decoration()) {
	case InsetLayout::CLASSIC:
		if (status_ == Open)
			return openinlined_ ? LeftButton : TopButton;
		return ButtonOnly;

	case InsetLayout::MINIMALISTIC:
		return status_ == Open ? NoButton : ButtonOnly ;

	case InsetLayout::CONGLOMERATE:
		return status_ == Open ? SubLabel : Corners ;

	case InsetLayout::DEFAULT:
		break; // this shouldn't happen
	}

	// dummy return value to shut down a warning,
	// this is dead code.
	return NoButton;
}


InsetCollapsable::InsetCollapsable(Buffer const & buf, InsetText::UsePlain ltype)
	: InsetText(buf, ltype), status_(Inset::Open),
	  openinlined_(false), mouse_hover_(false)
{
	setLayout(&buf.params().documentClass());
	setAutoBreakRows(true);
	setDrawFrame(true);
	setFrameColor(Color_collapsableframe);
}


InsetCollapsable::InsetCollapsable(InsetCollapsable const & rhs)
	: InsetText(rhs),
	  status_(rhs.status_),
	  layout_(rhs.layout_),
	  labelstring_(rhs.labelstring_),
	  button_dim(rhs.button_dim),
	  openinlined_(rhs.openinlined_),
	  auto_open_(rhs.auto_open_),
	  // the sole purpose of this copy constructor
	  mouse_hover_(false)
{
}


docstring InsetCollapsable::toolTip(BufferView const & bv, int x, int y) const
{
	Dimension dim = dimensionCollapsed(bv);
	if (geometry(bv) == NoButton)
		return translateIfPossible(layout_->labelstring());
	if (x > xo(bv) + dim.wid || y > yo(bv) + dim.des || isOpen(bv))
		return docstring();

	OutputParams rp(&buffer().params().encoding());
	odocstringstream ods;
	InsetText::plaintext(ods, rp);
	docstring content_tip = ods.str();
	// shorten it if necessary
	if (content_tip.size() > 200)
		content_tip = content_tip.substr(0, 200) + "...";
	return content_tip;
}


void InsetCollapsable::setLayout(BufferParams const & bp)
{
	setLayout(bp.documentClassPtr());
}


void InsetCollapsable::setLayout(DocumentClass const * const dc)
{
	if (dc) {
		layout_ = &(dc->insetLayout(name()));
		labelstring_ = translateIfPossible(layout_->labelstring());
	} else {
		layout_ = &DocumentClass::plainInsetLayout();
		labelstring_ = _("UNDEFINED");
	}

	setButtonLabel();
}


void InsetCollapsable::write(ostream & os) const
{
	os << "status ";
	switch (status_) {
	case Open:
		os << "open";
		break;
	case Collapsed:
		os << "collapsed";
		break;
	}
	os << "\n";
	text().write(buffer(), os);
}


void InsetCollapsable::read(Lexer & lex)
{
	lex.setContext("InsetCollapsable::read");
	string tmp_token;
	status_ = Collapsed;
	lex >> "status" >> tmp_token;
	if (tmp_token == "open")
		status_ = Open;

	// this must be set before we enter InsetText::read()
	setLayout(buffer().params());
	InsetText::read(lex);
	// set button label again as the inset contents was not read yet at
	// setLayout() time.
	setButtonLabel();

	// Force default font, if so requested
	// This avoids paragraphs in buffer language that would have a
	// foreign language after a document language change, and it ensures
	// that all new text in ERT and similar gets the "latex" language,
	// since new text inherits the language from the last position of the
	// existing text.  As a side effect this makes us also robust against
	// bugs in LyX that might lead to font changes in ERT in .lyx files.
	resetParagraphsFont();
}


Dimension InsetCollapsable::dimensionCollapsed(BufferView const & bv) const
{
	LASSERT(layout_, /**/);
	Dimension dim;
	theFontMetrics(layout_->labelfont()).buttonText(
		buttonLabel(bv), dim.wid, dim.asc, dim.des);
	return dim;
}


void InsetCollapsable::metrics(MetricsInfo & mi, Dimension & dim) const
{
	LASSERT(layout_, /**/);

	auto_open_[mi.base.bv] =  mi.base.bv->cursor().isInside(this);

	FontInfo tmpfont = mi.base.font;
	mi.base.font = layout_->font();
	mi.base.font.realize(tmpfont);

	BufferView const & bv = *mi.base.bv;

	switch (geometry(bv)) {
	case NoButton:
		InsetText::metrics(mi, dim);
		break;
	case Corners:
		InsetText::metrics(mi, dim);
		dim.des -= 3;
		dim.asc -= 1;
		break;
	case SubLabel: {
		InsetText::metrics(mi, dim);
		// consider width of the inset label
		FontInfo font(layout_->labelfont());
		font.realize(sane_font);
		font.decSize();
		font.decSize();
		int w = 0;
		int a = 0;
		int d = 0;
		theFontMetrics(font).rectText(buttonLabel(bv), w, a, d);
		dim.des += a + d;
		break;
		}
	case TopButton:
	case LeftButton:
	case ButtonOnly:
		dim = dimensionCollapsed(bv);
		if (geometry(bv) == TopButton 
			  || geometry(bv) == LeftButton) {
			Dimension textdim;
			InsetText::metrics(mi, textdim);
			openinlined_ = (textdim.wid + dim.wid) < mi.base.textwidth;
			if (openinlined_) {
				// Correct for button width.
				dim.wid += textdim.wid;
				dim.des = max(dim.des - textdim.asc + dim.asc, textdim.des);
				dim.asc = textdim.asc;
			} else {
				dim.des += textdim.height() + TEXT_TO_INSET_OFFSET;
				dim.wid = max(dim.wid, textdim.wid);
			}
		}
		break;
	}

	mi.base.font = tmpfont;
}


bool InsetCollapsable::setMouseHover(bool mouse_hover)
{
	mouse_hover_ = mouse_hover;
	return true;
}


void InsetCollapsable::draw(PainterInfo & pi, int x, int y) const
{
	LASSERT(layout_, /**/);
	BufferView const & bv = *pi.base.bv;

	auto_open_[&bv] =  bv.cursor().isInside(this);

	FontInfo tmpfont = pi.base.font;
	pi.base.font = layout_->font();
	pi.base.font.realize(tmpfont);

	// Draw button first -- top, left or only
	Dimension dimc = dimensionCollapsed(bv);

	if (geometry(*pi.base.bv) == TopButton ||
	    geometry(*pi.base.bv) == LeftButton ||
	    geometry(*pi.base.bv) == ButtonOnly) {
		button_dim.x1 = x + 0;
		button_dim.x2 = x + dimc.width();
		button_dim.y1 = y - dimc.asc;
		button_dim.y2 = y + dimc.des;

		pi.pain.buttonText(x, y, buttonLabel(bv), layout_->labelfont(),
			mouse_hover_);
	} else {
		button_dim.x1 = 0;
		button_dim.y1 = 0;
		button_dim.x2 = 0;
		button_dim.y2 = 0;
	}

	Dimension const textdim = InsetText::dimension(bv);
	int const baseline = y;
	int textx, texty;
	switch (geometry(bv)) {
	case LeftButton:
		textx = x + dimc.width();
		texty = baseline;
		InsetText::draw(pi, textx, texty);
		break;
	case TopButton:
		textx = x;
		texty = baseline + dimc.des + textdim.asc;
		InsetText::draw(pi, textx, texty);
		break;
	case ButtonOnly:
		break;
	case NoButton:
		textx = x;
		texty = baseline;
		InsetText::draw(pi, textx, texty);
		break;
	case SubLabel:
	case Corners:
		textx = x;
		texty = baseline;
		const_cast<InsetCollapsable *>(this)->setDrawFrame(false);
		InsetText::draw(pi, textx, texty);
		const_cast<InsetCollapsable *>(this)->setDrawFrame(true);

		int desc = textdim.descent();
		if (geometry(bv) == Corners)
			desc -= 3;

		const int xx1 = x + TEXT_TO_INSET_OFFSET - 1;
		const int xx2 = x + textdim.wid - TEXT_TO_INSET_OFFSET + 1;
		pi.pain.line(xx1, y + desc - 4, 
			     xx1, y + desc, 
			layout_->labelfont().color());
		if (status_ == Open)
			pi.pain.line(xx1, y + desc, 
				xx2, y + desc,
				layout_->labelfont().color());
		else {
			// Make status_ value visible:
			pi.pain.line(xx1, y + desc,
				xx1 + 4, y + desc,
				layout_->labelfont().color());
			pi.pain.line(xx2 - 4, y + desc,
				xx2, y + desc,
				layout_->labelfont().color());
		}
		pi.pain.line(x + textdim.wid - 3, y + desc, x + textdim.wid - 3, 
			y + desc - 4, layout_->labelfont().color());

		// the label below the text. Can be toggled.
		if (geometry(bv) == SubLabel) {
			FontInfo font(layout_->labelfont());
			font.realize(sane_font);
			font.decSize();
			font.decSize();
			int w = 0;
			int a = 0;
			int d = 0;
			theFontMetrics(font).rectText(buttonLabel(bv), w, a, d);
			int const ww = max(textdim.wid, w);
			pi.pain.rectText(x + (ww - w) / 2, y + desc + a,
				buttonLabel(bv), font, Color_none, Color_none);
			desc += d;
		}

		// a visual cue when the cursor is inside the inset
		Cursor const & cur = bv.cursor();
		if (cur.isInside(this)) {
			y -= textdim.asc;
			y += 3;
			pi.pain.line(xx1, y + 4, xx1, y, layout_->labelfont().color());
			pi.pain.line(xx1 + 4, y, xx1, y, layout_->labelfont().color());
			pi.pain.line(xx2, y + 4, xx2, y,
				layout_->labelfont().color());
			pi.pain.line(xx2 - 4, y, xx2, y,
				layout_->labelfont().color());
		}
		break;
	}

	pi.base.font = tmpfont;
}


void InsetCollapsable::cursorPos(BufferView const & bv,
		CursorSlice const & sl, bool boundary, int & x, int & y) const
{
	if (geometry(bv) == ButtonOnly)
		status_ = Open;
	LASSERT(geometry(bv) != ButtonOnly, /**/);

	InsetText::cursorPos(bv, sl, boundary, x, y);
	Dimension const textdim = InsetText::dimension(bv);

	switch (geometry(bv)) {
	case LeftButton:
		x += dimensionCollapsed(bv).wid;
		break;
	case TopButton: {
		y += dimensionCollapsed(bv).des + textdim.asc;
		break;
	}
	case NoButton:
	case SubLabel:
	case Corners:
		// Do nothing
		break;
	case ButtonOnly:
		// Cannot get here
		break;
	}
}


bool InsetCollapsable::editable() const
{
	return geometry() != ButtonOnly;
}


bool InsetCollapsable::descendable() const
{
	return geometry() != ButtonOnly;
}


bool InsetCollapsable::hitButton(FuncRequest const & cmd) const
{
	return button_dim.contains(cmd.x, cmd.y);
}


docstring const InsetCollapsable::getNewLabel(docstring const & l) const
{
	docstring label;
	pos_type const max_length = 15;
	pos_type const p_siz = paragraphs().begin()->size();
	pos_type const n = min(max_length, p_siz);
	pos_type i = 0;
	pos_type j = 0;
	for (; i < n && j < p_siz; ++j) {
		if (paragraphs().begin()->isInset(j))
			continue;
		label += paragraphs().begin()->getChar(j);
		++i;
	}
	if (paragraphs().size() > 1 || (i > 0 && j < p_siz)) {
		label += "...";
	}
	return label.empty() ? l : label;
}


void InsetCollapsable::edit(Cursor & cur, bool front, EntryDirection entry_from)
{
	//lyxerr << "InsetCollapsable: edit left/right" << endl;
	cur.push(*this);
	InsetText::edit(cur, front, entry_from);
}


Inset * InsetCollapsable::editXY(Cursor & cur, int x, int y)
{
	//lyxerr << "InsetCollapsable: edit xy" << endl;
	if (geometry(cur.bv()) == ButtonOnly
	 || (button_dim.contains(x, y) 
	  && geometry(cur.bv()) != NoButton))
		return this;
	cur.push(*this);
	return InsetText::editXY(cur, x, y);
}


void InsetCollapsable::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	//lyxerr << "InsetCollapsable::doDispatch (begin): cmd: " << cmd
	//	<< " cur: " << cur << " bvcur: " << cur.bv().cursor() << endl;

	switch (cmd.action) {
	case LFUN_MOUSE_PRESS:
		if (hitButton(cmd)) {
			switch (cmd.button()) {
			case mouse_button::button1:
			case mouse_button::button3:
				// Pass the command to the enclosing InsetText,
				// so that the cursor gets set.
				cur.undispatched();
				break;
			case mouse_button::none:
			case mouse_button::button2:
			case mouse_button::button4:
			case mouse_button::button5:
				// Nothing to do.
				cur.noUpdate();
				break;
			}
		} else if (geometry(cur.bv()) != ButtonOnly)
			InsetText::doDispatch(cur, cmd);
		else
			cur.undispatched();
		break;

	case LFUN_MOUSE_MOTION:
	case LFUN_MOUSE_DOUBLE:
	case LFUN_MOUSE_TRIPLE:
		if (hitButton(cmd)) 
			cur.noUpdate();
		else if (geometry(cur.bv()) != ButtonOnly)
			InsetText::doDispatch(cur, cmd);
		else
			cur.undispatched();
		break;

	case LFUN_MOUSE_RELEASE:
		if (!hitButton(cmd)) {
			// The mouse click has to be within the inset!
			if (geometry(cur.bv()) != ButtonOnly)
				InsetText::doDispatch(cur, cmd);
			else
				cur.undispatched();			
			break;
		}
		if (cmd.button() != mouse_button::button1) {
			// Nothing to do.
			cur.noUpdate();
			break;
		}
		// if we are selecting, we do not want to
		// toggle the inset.
		if (cur.selection())
			break;
		// Left button is clicked, the user asks to
		// toggle the inset visual state.
		cur.dispatched();
		cur.updateFlags(Update::Force | Update::FitCursor);
		if (geometry(cur.bv()) == ButtonOnly) {
			setStatus(cur, Open);
			edit(cur, true);
		}
		else
			setStatus(cur, Collapsed);
		cur.bv().cursor() = cur;
		break;

	case LFUN_INSET_TOGGLE:
		if (cmd.argument() == "open")
			setStatus(cur, Open);
		else if (cmd.argument() == "close")
			setStatus(cur, Collapsed);
		else if (cmd.argument() == "toggle" || cmd.argument().empty())
			if (status_ == Open) {
				setStatus(cur, Collapsed);
				if (geometry(cur.bv()) == ButtonOnly)
					cur.top().forwardPos();
			} else
				setStatus(cur, Open);
		else // if assign or anything else
			cur.undispatched();
		cur.dispatched();
		break;

	case LFUN_PASTE:
	case LFUN_CLIPBOARD_PASTE:
	case LFUN_PRIMARY_SELECTION_PASTE: {
		InsetText::doDispatch(cur, cmd);
		// Since we can only store plain text, we must reset all
		// attributes.
		// FIXME: Change only the pasted paragraphs

		resetParagraphsFont();
		break;
	}

	default:
		if (layout_ && layout_->isForceLtr()) {
			// Force any new text to latex_language
			// FIXME: This should only be necessary in constructor, but
			// new paragraphs that are created by pressing enter at the
			// start of an existing paragraph get the buffer language
			// and not latex_language, so we take this brute force
			// approach.
			cur.current_font.setLanguage(latex_language);
			cur.real_current_font.setLanguage(latex_language);
		}
		InsetText::doDispatch(cur, cmd);
		break;
	}
}


bool InsetCollapsable::allowMultiPar() const
{
	return layout_->isMultiPar();
}


void InsetCollapsable::resetParagraphsFont()
{
	Font font(inherit_font, buffer().params().language);
	if (layout_->isForceLtr())
		font.setLanguage(latex_language);
	if (layout_->isPassThru()) {
		ParagraphList::iterator par = paragraphs().begin();
		ParagraphList::iterator const end = paragraphs().end();
		while (par != end) {
			par->resetFonts(font);
			par->params().clear();
			++par;
		}
	}
}


bool InsetCollapsable::getStatus(Cursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const
{
	switch (cmd.action) {
	// FIXME At present, these are being enabled and disabled according to
	// whether PASSTHRU has been set in the InsetLayout. This makes some
	// sense, but there are other checks that should really be done. E.g.,
	// one should not be able to inset IndexPrint inside an optional argument!!
	case LFUN_ACCENT_ACUTE:
	case LFUN_ACCENT_BREVE:
	case LFUN_ACCENT_CARON:
	case LFUN_ACCENT_CEDILLA:
	case LFUN_ACCENT_CIRCLE:
	case LFUN_ACCENT_CIRCUMFLEX:
	case LFUN_ACCENT_DOT:
	case LFUN_ACCENT_GRAVE:
	case LFUN_ACCENT_HUNGARIAN_UMLAUT:
	case LFUN_ACCENT_MACRON:
	case LFUN_ACCENT_OGONEK:
	case LFUN_ACCENT_TIE:
	case LFUN_ACCENT_TILDE:
	case LFUN_ACCENT_UMLAUT:
	case LFUN_ACCENT_UNDERBAR:
	case LFUN_ACCENT_UNDERDOT:
	case LFUN_APPENDIX:
	case LFUN_BOX_INSERT:
	case LFUN_BRANCH_INSERT:
	case LFUN_NEWLINE_INSERT:
	case LFUN_CAPTION_INSERT:
	case LFUN_DEPTH_DECREMENT:
	case LFUN_DEPTH_INCREMENT:
	case LFUN_ERT_INSERT:
	case LFUN_FILE_INSERT:
	case LFUN_FLEX_INSERT:
	case LFUN_FLOAT_INSERT:
	case LFUN_FLOAT_LIST_INSERT:
	case LFUN_FLOAT_WIDE_INSERT:
	case LFUN_FONT_BOLD:
	case LFUN_FONT_BOLDSYMBOL:
	case LFUN_FONT_TYPEWRITER:
	case LFUN_FONT_DEFAULT:
	case LFUN_FONT_EMPH:
	case LFUN_TEXTSTYLE_APPLY:
	case LFUN_TEXTSTYLE_UPDATE:
	case LFUN_FONT_NOUN:
	case LFUN_FONT_ROMAN:
	case LFUN_FONT_SANS:
	case LFUN_FONT_FRAK:
	case LFUN_FONT_ITAL:
	case LFUN_FONT_SIZE:
	case LFUN_FONT_STATE:
	case LFUN_FONT_UNDERLINE:
	case LFUN_FONT_STRIKEOUT:
	case LFUN_FONT_UULINE:
	case LFUN_FONT_UWAVE:
	case LFUN_FOOTNOTE_INSERT:
	case LFUN_HYPERLINK_INSERT:
	case LFUN_INDEX_INSERT:
	case LFUN_INDEX_PRINT:
	case LFUN_INSET_INSERT:
	case LFUN_LABEL_GOTO:
	case LFUN_LABEL_INSERT:
	case LFUN_LINE_INSERT:
	case LFUN_NEWPAGE_INSERT:
	case LFUN_LAYOUT_TABULAR:
	case LFUN_MARGINALNOTE_INSERT:
	case LFUN_MATH_DISPLAY:
	case LFUN_MATH_INSERT:
	case LFUN_MATH_MATRIX:
	case LFUN_MATH_MODE:
	case LFUN_MENU_OPEN:
	case LFUN_NOACTION:
	case LFUN_NOMENCL_INSERT:
	case LFUN_NOMENCL_PRINT:
	case LFUN_NOTE_INSERT:
	case LFUN_NOTE_NEXT:
	case LFUN_OPTIONAL_INSERT:
	case LFUN_PHANTOM_INSERT:
	case LFUN_REFERENCE_NEXT:
	case LFUN_SERVER_GOTO_FILE_ROW:
	case LFUN_SERVER_NOTIFY:
	case LFUN_SERVER_SET_XY:
	case LFUN_SPACE_INSERT:
	case LFUN_SPECIALCHAR_INSERT:
	case LFUN_TABULAR_INSERT:
	case LFUN_TOC_INSERT:
	case LFUN_WRAP_INSERT:
		if (layout_->isPassThru()) {
			flag.setEnabled(false);
			return true;
		}
		return InsetText::getStatus(cur, cmd, flag);

	case LFUN_INSET_TOGGLE:
		if (cmd.argument() == "open")
			flag.setEnabled(status_ != Open);
		else if (cmd.argument() == "close")
			flag.setEnabled(status_ == Open);
		else if (cmd.argument() == "toggle" || cmd.argument().empty()) {
			flag.setEnabled(true);
			flag.setOnOff(status_ == Open);
		} else
			flag.setEnabled(false);
		return true;

	case LFUN_LANGUAGE:
		flag.setEnabled(!layout_->isForceLtr());
		return InsetText::getStatus(cur, cmd, flag);

	case LFUN_BREAK_PARAGRAPH:
		flag.setEnabled(layout_->isMultiPar());
		return true;

	default:
		return InsetText::getStatus(cur, cmd, flag);
	}
}


void InsetCollapsable::setLabel(docstring const & l)
{
	labelstring_ = l;
}


void InsetCollapsable::setStatus(Cursor & cur, CollapseStatus status)
{
	status_ = status;
	setButtonLabel();
	if (status_ == Collapsed) {
		cur.leaveInset(*this);
		mouse_hover_ = false;
	}
}


docstring InsetCollapsable::floatName(
		string const & type, BufferParams const & bp) const
{
	FloatList const & floats = bp.documentClass().floats();
	FloatList::const_iterator it = floats[type];
	// FIXME UNICODE
	return (it == floats.end()) ? from_ascii(type) : bp.B_(it->second.name());
}


InsetLayout::InsetDecoration InsetCollapsable::decoration() const
{
	if (!layout_)
		return InsetLayout::CLASSIC;
	InsetLayout::InsetDecoration const dec = layout_->decoration();
	switch (dec) {
	case InsetLayout::CLASSIC:
	case InsetLayout::MINIMALISTIC:
	case InsetLayout::CONGLOMERATE:
		return dec;
	case InsetLayout::DEFAULT:
		break;
	}
	if (lyxCode() == FLEX_CODE)
		return InsetLayout::CONGLOMERATE;
	return InsetLayout::CLASSIC;
}


int InsetCollapsable::latex(odocstream & os,
			  OutputParams const & runparams) const
{
	// FIXME: What should we do layout_ is 0?
	// 1) assert
	// 2) throw an error
	if (!layout_)
		return 0;

	// This implements the standard way of handling the LaTeX output of
	// a collapsable inset, either a command or an environment. Standard 
	// collapsable insets should not redefine this, non-standard ones may
	// call this.
	if (!layout_->latexname().empty()) {
		if (layout_->latextype() == InsetLayout::COMMAND) {
			// FIXME UNICODE
			if (runparams.moving_arg)
				os << "\\protect";
			os << '\\' << from_utf8(layout_->latexname());
			if (!layout_->latexparam().empty())
				os << from_utf8(layout_->latexparam());
			os << '{';
		} else if (layout_->latextype() == InsetLayout::ENVIRONMENT) {
			os << "%\n\\begin{" << from_utf8(layout_->latexname()) << "}\n";
			if (!layout_->latexparam().empty())
				os << from_utf8(layout_->latexparam());
		}
	}
	OutputParams rp = runparams;
	if (layout_->isPassThru())
		rp.verbatim = true;
	if (layout_->isNeedProtect())
		rp.moving_arg = true;
	int i = InsetText::latex(os, rp);
	if (!layout_->latexname().empty()) {
		if (layout_->latextype() == InsetLayout::COMMAND) {
			os << "}";
		} else if (layout_->latextype() == InsetLayout::ENVIRONMENT) {
			os << "\n\\end{" << from_utf8(layout_->latexname()) << "}\n";
			i += 4;
		}
	}
	return i;
}


// FIXME It seems as if it ought to be possible to do this more simply,
// maybe by calling InsetText::docbook() in the middle there.
int InsetCollapsable::docbook(odocstream & os, OutputParams const & runparams) const
{
	ParagraphList::const_iterator const beg = paragraphs().begin();
	ParagraphList::const_iterator par = paragraphs().begin();
	ParagraphList::const_iterator const end = paragraphs().end();

	if (!undefined())
		sgml::openTag(os, getLayout().latexname(),
			      par->getID(buffer(), runparams) + getLayout().latexparam());

	for (; par != end; ++par) {
		par->simpleDocBookOnePar(buffer(), os, runparams,
					 outerFont(distance(beg, par),
						   paragraphs()));
	}

	if (!undefined())
		sgml::closeTag(os, getLayout().latexname());

	return 0;
}


void InsetCollapsable::validate(LaTeXFeatures & features) const
{
	string const preamble = getLayout().preamble();
	if (!preamble.empty())
		features.addPreambleSnippet(preamble);
	features.require(getLayout().requires());
	InsetText::validate(features);
}


bool InsetCollapsable::undefined() const
{
	docstring const & n = getLayout().name();
	return n.empty() || n == DocumentClass::plainInsetLayout().name();
}


docstring InsetCollapsable::contextMenu(BufferView const & bv, int x,
	int y) const
{
	if (decoration() == InsetLayout::CONGLOMERATE)
		return from_ascii("context-conglomerate");

	if (geometry(bv) == NoButton)
		return from_ascii("context-collapsable");

	Dimension dim = dimensionCollapsed(bv);
	if (x < xo(bv) + dim.wid && y < yo(bv) + dim.des)
		return from_ascii("context-collapsable");

	return InsetText::contextMenu(bv, x, y);
}

void InsetCollapsable::tocString(odocstream & os) const
{
	if (!getLayout().isInToc())
		return;
	os << text().asString(0, 1, AS_STR_LABEL | AS_STR_INSETS);
}


} // namespace lyx
