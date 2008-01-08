/**
 * \file BufferView.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 * \author Andr� P�nitz
 * \author J�rgen Vigna
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "BufferView.h"

#include "Buffer.h"
#include "buffer_funcs.h"
#include "BufferList.h"
#include "BufferParams.h"
#include "CoordCache.h"
#include "Cursor.h"
#include "CutAndPaste.h"
#include "DispatchResult.h"
#include "EmbeddedFiles.h"
#include "ErrorList.h"
#include "factory.h"
#include "FloatList.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "Intl.h"
#include "InsetIterator.h"
#include "Language.h"
#include "LaTeXFeatures.h"
#include "LyX.h"
#include "lyxfind.h"
#include "LyXFunc.h"
#include "Layout.h"
#include "LyXRC.h"
#include "MetricsInfo.h"
#include "Paragraph.h"
#include "paragraph_funcs.h"
#include "ParagraphParameters.h"
#include "ParIterator.h"
#include "Session.h"
#include "Text.h"
#include "TextClass.h"
#include "TextMetrics.h"
#include "TexRow.h"
#include "VSpace.h"
#include "WordLangTuple.h"

#include "insets/InsetBibtex.h"
#include "insets/InsetCommand.h" // ChangeRefs
#include "insets/InsetRef.h"
#include "insets/InsetText.h"

#include "frontends/alert.h"
#include "frontends/Application.h"
#include "frontends/Delegates.h"
#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"
#include "frontends/Selection.h"

#include "graphics/Previews.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/ExceptionMessage.h"
#include "support/FileFilterList.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lstrings.h"
#include "support/Package.h"
#include "support/types.h"

#include <cerrno>
#include <fstream>
#include <functional>
#include <iterator>
#include <vector>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace Alert = frontend::Alert;

namespace {

/// Return an inset of this class if it exists at the current cursor position
template <class T>
T * getInsetByCode(Cursor const & cur, InsetCode code)
{
	DocIterator it = cur;
	Inset * inset = it.nextInset();
	if (inset && inset->lyxCode() == code)
		return static_cast<T*>(inset);
	return 0;
}


bool findInset(DocIterator & dit, vector<InsetCode> const & codes,
	bool same_content);

bool findNextInset(DocIterator & dit, vector<InsetCode> const & codes,
	docstring const & contents)
{
	DocIterator tmpdit = dit;

	while (tmpdit) {
		Inset const * inset = tmpdit.nextInset();
		if (inset
		    && find(codes.begin(), codes.end(), inset->lyxCode()) != codes.end()
		    && (contents.empty() ||
		    static_cast<InsetCommand const *>(inset)->getFirstNonOptParam() == contents)) {
			dit = tmpdit;
			return true;
		}
		tmpdit.forwardInset();
	}

	return false;
}


/// Looks for next inset with one of the given codes.
bool findInset(DocIterator & dit, vector<InsetCode> const & codes,
	bool same_content)
{
	docstring contents;
	DocIterator tmpdit = dit;
	tmpdit.forwardInset();
	if (!tmpdit)
		return false;

	if (same_content) {
		Inset const * inset = tmpdit.nextInset();
		if (inset
		    && find(codes.begin(), codes.end(), inset->lyxCode()) != codes.end()) {
			contents = static_cast<InsetCommand const *>(inset)->getFirstNonOptParam();
		}
	}

	if (!findNextInset(tmpdit, codes, contents)) {
		if (dit.depth() != 1 || dit.pit() != 0 || dit.pos() != 0) {
			tmpdit  = doc_iterator_begin(tmpdit.bottom().inset());
			if (!findNextInset(tmpdit, codes, contents))
				return false;
		} else
			return false;
	}

	dit = tmpdit;
	return true;
}


/// Looks for next inset with the given code
void findInset(DocIterator & dit, InsetCode code, bool same_content)
{
	findInset(dit, vector<InsetCode>(1, code), same_content);
}


/// Moves cursor to the next inset with one of the given codes.
void gotoInset(BufferView * bv, vector<InsetCode> const & codes,
	       bool same_content)
{
	Cursor tmpcur = bv->cursor();
	if (!findInset(tmpcur, codes, same_content)) {
		bv->cursor().message(_("No more insets"));
		return;
	}

	tmpcur.clearSelection();
	bv->setCursor(tmpcur);
	bv->showCursor();
}


/// Moves cursor to the next inset with given code.
void gotoInset(BufferView * bv, InsetCode code, bool same_content)
{
	gotoInset(bv, vector<InsetCode>(1, code), same_content);
}


/// A map from a Text to the associated text metrics
typedef map<Text const *, TextMetrics> TextMetricsCache;

enum ScreenUpdateStrategy {
	NoScreenUpdate,
	SingleParUpdate,
	FullScreenUpdate,
	DecorationUpdate
};

} // anon namespace


/////////////////////////////////////////////////////////////////////
//
// BufferView
//
/////////////////////////////////////////////////////////////////////

struct BufferView::Private
{
	Private(BufferView & bv): wh_(0), cursor_(bv),
		anchor_pit_(0), anchor_ypos_(0),
		last_inset_(0), gui_(0)
	{}

	///
	ScrollbarParameters scrollbarParameters_;
	///
	ScreenUpdateStrategy update_strategy_;
	///
	CoordCache coord_cache_;

	/// Estimated average par height for scrollbar.
	int wh_;
	/// this is used to handle XSelection events in the right manner.
	struct {
		CursorSlice cursor;
		CursorSlice anchor;
		bool set;
	} xsel_cache_;
	///
	Cursor cursor_;
	///
	pit_type anchor_pit_;
	///
	int anchor_ypos_;
	///
	vector<int> par_height_;

	/// keyboard mapping object.
	Intl intl_;

	/// last visited inset.
	/** kept to send setMouseHover(false).
	  * Not owned, so don't delete.
	  */
	Inset * last_inset_;

	mutable TextMetricsCache text_metrics_;

	/// Whom to notify.
	/** Not owned, so don't delete.
	  */
	frontend::GuiBufferViewDelegate * gui_;
};


BufferView::BufferView(Buffer & buf)
	: width_(0), height_(0), buffer_(buf), d(new Private(*this))
{
	d->xsel_cache_.set = false;
	d->intl_.initKeyMapper(lyxrc.use_kbmap);

	d->cursor_.push(buffer_.inset());
	d->cursor_.resetAnchor();
	d->cursor_.setCurrentFont();

	if (graphics::Previews::status() != LyXRC::PREVIEW_OFF)
		graphics::Previews::get().generateBufferPreviews(buffer_);
}


BufferView::~BufferView()
{
	// current buffer is going to be switched-off, save cursor pos
	// Ideally, the whole cursor stack should be saved, but session
	// currently can only handle bottom (whole document) level pit and pos.
	// That is to say, if a cursor is in a nested inset, it will be
	// restore to the left of the top level inset.
	LastFilePosSection::FilePos fp;
	fp.pit = d->cursor_.bottom().pit();
	fp.pos = d->cursor_.bottom().pos();
	LyX::ref().session().lastFilePos().save(buffer_.fileName(), fp);

	delete d;
}


Intl & BufferView::getIntl()
{
	return d->intl_;
}


Intl const & BufferView::getIntl() const
{
	return d->intl_;
}


CoordCache & BufferView::coordCache()
{
	return d->coord_cache_;
}


CoordCache const & BufferView::coordCache() const
{
	return d->coord_cache_;
}


Buffer & BufferView::buffer()
{
	return buffer_;
}


Buffer const & BufferView::buffer() const
{
	return buffer_;
}


bool BufferView::fitCursor()
{
	if (cursorStatus(d->cursor_) == CUR_INSIDE) {
		frontend::FontMetrics const & fm =
			theFontMetrics(d->cursor_.getFont().fontInfo());
		int const asc = fm.maxAscent();
		int const des = fm.maxDescent();
		Point const p = getPos(d->cursor_, d->cursor_.boundary());
		if (p.y_ - asc >= 0 && p.y_ + des < height_)
			return false;
	}
	return true;
}


void BufferView::processUpdateFlags(Update::flags flags)
{
	// last_inset_ points to the last visited inset. This pointer may become
	// invalid because of keyboard editing. Since all such operations
	// causes screen update(), I reset last_inset_ to avoid such a problem.
	d->last_inset_ = 0;
	// This is close to a hot-path.
	LYXERR(Debug::DEBUG, "BufferView::processUpdateFlags()"
		<< "[fitcursor = " << (flags & Update::FitCursor)
		<< ", forceupdate = " << (flags & Update::Force)
		<< ", singlepar = " << (flags & Update::SinglePar)
		<< "]  buffer: " << &buffer_);

	buffer_.updateMacros();

	// Now do the first drawing step if needed. This consists on updating
	// the CoordCache in updateMetrics().
	// The second drawing step is done in WorkArea::redraw() if needed.

	// Case when no explicit update is requested.
	if (!flags) {
		// no need to redraw anything.
		d->update_strategy_ = NoScreenUpdate;
		return;
	}

	if (flags == Update::Decoration) {
		d->update_strategy_ = DecorationUpdate;
		buffer_.changed();
		return;
	}

	if (flags == Update::FitCursor
		|| flags == (Update::Decoration | Update::FitCursor)) {
		// tell the frontend to update the screen if needed.
		if (fitCursor()) {
			showCursor();
			return;
		}
		if (flags & Update::Decoration) {
			d->update_strategy_ = DecorationUpdate;
			buffer_.changed();
			return;
		}
		// no screen update is needed.
		d->update_strategy_ = NoScreenUpdate;
		return;
	}

	bool const full_metrics = flags & Update::Force || !singleParUpdate();

	if (full_metrics)
		// We have to update the full screen metrics.
		updateMetrics();

	if (!(flags & Update::FitCursor)) {
		// Nothing to do anymore. Trigger a redraw and return
		buffer_.changed();
		return;
	}

	// updateMetrics() does not update paragraph position
	// This is done at draw() time. So we need a redraw!
	buffer_.changed();

	if (fitCursor()) {
		// The cursor is off screen so ensure it is visible.
		// refresh it:
		showCursor();
	}
}


void BufferView::updateScrollbar()
{
	if (height_ == 0)
		return;

	Text & t = buffer_.text();
	TextMetrics & tm = d->text_metrics_[&t];		

	LYXERR(Debug::GUI, " Updating scrollbar: height: "
		<< t.paragraphs().size()
		<< " curr par: " << d->cursor_.bottom().pit()
		<< " default height " << defaultRowHeight());

	int const parsize = int(t.paragraphs().size());
	if (d->par_height_.size() != parsize) {
		d->par_height_.clear();
		// FIXME: We assume a default paragraph height of 2 rows. This
		// should probably be pondered with the screen width.
		d->par_height_.resize(parsize, defaultRowHeight() * 2);
	}

	// It would be better to fix the scrollbar to understand
	// values in [0..1] and divide everything by wh

	// Look at paragraph heights on-screen
	pit_type first_visible_pit = -1;
	pair<pit_type, ParagraphMetrics const *> first = tm.first();
	pair<pit_type, ParagraphMetrics const *> last = tm.last();
	for (pit_type pit = first.first; pit <= last.first; ++pit) {
		ParagraphMetrics const & pm = tm.parMetrics(pit);
		d->par_height_[pit] = pm.height();
		if (first_visible_pit >= 0 || pm.position() + pm.descent() <= 0)
			continue;
		first_visible_pit = pit;
		LYXERR(Debug::SCROLLING, "first visible pit " << first_visible_pit);
		// FIXME: we should look for the first visible row within
		// the deepest inset!
		int row_pos = pm.position();
		size_t const nrows = pm.rows().size();
		for (size_t i = 0; i != nrows; ++i) {
			Row const & row = pm.rows()[i];
			if (row_pos >= 0) {
				LYXERR(Debug::SCROLLING, "first visible row " << i
					<< "(row pos = " << row_pos << ");");
				break;
			}
			row_pos += row.height();
		}
		d->scrollbarParameters_.position = row_pos;
	}

	d->scrollbarParameters_.height = 0;
	for (size_t i = 0; i != d->par_height_.size(); ++i) {
		if (i == first_visible_pit)
			d->scrollbarParameters_.position += d->scrollbarParameters_.height;
		d->scrollbarParameters_.height += d->par_height_[i];
	}

	// We prefer fixed size line scrolling.
	d->scrollbarParameters_.lineScrollHeight = defaultRowHeight();
}


ScrollbarParameters const & BufferView::scrollbarParameters() const
{
	return d->scrollbarParameters_;
}


docstring BufferView::toolTip(int x, int y) const
{
	// Get inset under mouse, if there is one.
	Inset const * covering_inset = getCoveringInset(buffer_.text(), x, y);
	if (!covering_inset)
		// No inset, no tooltip...
		return docstring();
	return covering_inset->toolTip(*this, x, y);
}


docstring BufferView::contextMenu(int x, int y) const
{
	// Get inset under mouse, if there is one.
	Inset const * covering_inset = getCoveringInset(buffer_.text(), x, y);
	if (covering_inset)
		return covering_inset->contextMenu(*this, x, y);

	// FIXME: Do something more elaborate here.
	return from_ascii("edit");
}


void BufferView::scrollDocView(int value)
{
	int const offset = value - d->scrollbarParameters_.position;
	// If the offset is less than 2 screen height, prefer to scroll instead.
	if (abs(offset) <= 2 * height_) {
		scroll(offset);
		return;
	}

	int par_pos = 0;
	for (size_t i = 0; i != d->par_height_.size(); ++i) {
		par_pos += d->par_height_[i];
		if (par_pos >= value) {
			d->anchor_pit_ = pit_type(i);
			break;
		}
	}

	LYXERR(Debug::SCROLLING, "value = " << value
		<< "\tanchor_ref_ = " << d->anchor_pit_
		<< "\tpar_pos = " << par_pos);

	d->anchor_ypos_ = par_pos - value;
	updateMetrics();
	buffer_.changed();
}


// FIXME: this method is not working well.
void BufferView::setCursorFromScrollbar()
{
	TextMetrics & tm = d->text_metrics_[&buffer_.text()];

	int const height = 2 * defaultRowHeight();
	int const first = height;
	int const last = height_ - height;
	Cursor & cur = d->cursor_;

	switch (cursorStatus(cur)) {
	case CUR_ABOVE:
		// We reset the cursor because cursorStatus() does not
		// work when the cursor is within mathed.
		cur.reset(buffer_.inset());
		tm.setCursorFromCoordinates(cur, 0, first);
		cur.clearSelection();
		break;
	case CUR_BELOW:
		// We reset the cursor because cursorStatus() does not
		// work when the cursor is within mathed.
		cur.reset(buffer_.inset());
		tm.setCursorFromCoordinates(cur, 0, last);
		cur.clearSelection();
		break;
	case CUR_INSIDE:
		int const y = getPos(cur, cur.boundary()).y_;
		int const newy = min(last, max(y, first));
		if (y != newy) {
			cur.reset(buffer_.inset());
			tm.setCursorFromCoordinates(cur, 0, newy);
		}
	}
}


Change const BufferView::getCurrentChange() const
{
	if (!d->cursor_.selection())
		return Change(Change::UNCHANGED);

	DocIterator dit = d->cursor_.selectionBegin();
	return dit.paragraph().lookupChange(dit.pos());
}


// this could be used elsewhere as well?
// FIXME: This does not work within mathed!
CursorStatus BufferView::cursorStatus(DocIterator const & dit) const
{
	Point const p = getPos(dit, dit.boundary());
	if (p.y_ < 0)
		return CUR_ABOVE;
	if (p.y_ > workHeight())
		return CUR_BELOW;
	return CUR_INSIDE;
}


void BufferView::saveBookmark(unsigned int idx)
{
	// tenatively save bookmark, id and pos will be used to
	// acturately locate a bookmark in a 'live' lyx session.
	// pit and pos will be updated with bottom level pit/pos
	// when lyx exits.
	LyX::ref().session().bookmarks().save(
		buffer_.fileName(),
		d->cursor_.bottom().pit(),
		d->cursor_.bottom().pos(),
		d->cursor_.paragraph().id(),
		d->cursor_.pos(),
		idx
	);
	if (idx)
		// emit message signal.
		message(_("Save bookmark"));
}


bool BufferView::moveToPosition(pit_type bottom_pit, pos_type bottom_pos,
	int top_id, pos_type top_pos)
{
	bool success = false;
	DocIterator doc_it;

	d->cursor_.clearSelection();

	// if a valid par_id is given, try it first
	// This is the case for a 'live' bookmark when unique paragraph ID
	// is used to track bookmarks.
	if (top_id > 0) {
		ParIterator par = buffer_.getParFromID(top_id);
		if (par != buffer_.par_iterator_end()) {
			doc_it = makeDocIterator(par, min(par->size(), top_pos));
			// Some slices of the iterator may not be
			// reachable (e.g. closed collapsable inset)
			// so the dociterator may need to be
			// shortened. Otherwise, setCursor may crash
			// lyx when the cursor can not be set to these
			// insets.
			size_t const n = doc_it.depth();
			for (size_t i = 0; i < n; ++i)
				if (doc_it[i].inset().editable() != Inset::HIGHLY_EDITABLE) {
					doc_it.resize(i);
					break;
				}
			success = true;
		}
	}

	// if top_id == 0, or searching through top_id failed
	// This is the case for a 'restored' bookmark when only bottom
	// (document level) pit was saved. Because of this, bookmark
	// restoration is inaccurate. If a bookmark was within an inset,
	// it will be restored to the left of the outmost inset that contains
	// the bookmark.
	if (static_cast<size_t>(bottom_pit) < buffer_.paragraphs().size()) {
		doc_it = doc_iterator_begin(buffer_.inset());
		doc_it.pit() = bottom_pit;
		doc_it.pos() = min(bottom_pos, doc_it.paragraph().size());
		success = true;
	}

	if (success) {
		// Note: only bottom (document) level pit is set.
		setCursor(doc_it);
		// set the current font.
		d->cursor_.setCurrentFont();
		// To center the screen on this new position we need the
		// paragraph position which is computed at draw() time.
		// So we need a redraw!
		buffer_.changed();
		if (fitCursor())
			showCursor();
	}

	return success;
}


void BufferView::translateAndInsert(char_type c, Text * t, Cursor & cur)
{
	if (lyxrc.rtl_support) {
		if (d->cursor_.real_current_font.isRightToLeft()) {
			if (d->intl_.keymap == Intl::PRIMARY)
				d->intl_.keyMapSec();
		} else {
			if (d->intl_.keymap == Intl::SECONDARY)
				d->intl_.keyMapPrim();
		}
	}

	d->intl_.getTransManager().translateAndInsert(c, t, cur);
}


int BufferView::workWidth() const
{
	return width_;
}


void BufferView::showCursor()
{
	// We are not properly started yet, delay until resizing is
	// done.
	if (height_ == 0)
		return;

	LYXERR(Debug::SCROLLING, "recentering!");

	CursorSlice & bot = d->cursor_.bottom();
	TextMetrics & tm = d->text_metrics_[bot.text()];

	pos_type const max_pit = pos_type(bot.text()->paragraphs().size() - 1);
	int bot_pit = d->cursor_.bottom().pit();
	if (bot_pit > max_pit) {
		// FIXME: Why does this happen?
		LYXERR0("bottom pit is greater that max pit: "
			<< bot_pit << " > " << max_pit);
		bot_pit = max_pit;
	}

	if (bot_pit == tm.first().first - 1)
		tm.newParMetricsUp();
	else if (bot_pit == tm.last().first + 1)
		tm.newParMetricsDown();

	if (tm.has(bot_pit)) {
		ParagraphMetrics const & pm = tm.parMetrics(bot_pit);
		int offset = coordOffset(d->cursor_, d->cursor_.boundary()).y_;
		int ypos = pm.position() + offset;
		Dimension const & row_dim = d->cursor_.textRow().dimension();
		if (ypos - row_dim.ascent() < 0)
			scrollUp(- ypos + row_dim.ascent());
		else if (ypos + row_dim.descent() > height_)
			scrollDown(ypos - height_ + row_dim.descent());
		// else, nothing to do, the cursor is already visible so we just return.
		return;
	}

	tm.redoParagraph(bot_pit);
	ParagraphMetrics const & pm = tm.parMetrics(bot_pit);
	int offset = coordOffset(d->cursor_, d->cursor_.boundary()).y_;

	d->anchor_pit_ = bot_pit;
	Dimension const & row_dim = d->cursor_.textRow().dimension();

	if (d->anchor_pit_ == 0)
		d->anchor_ypos_ = offset + pm.ascent();
	else if (d->anchor_pit_ == max_pit)
		d->anchor_ypos_ = height_ - offset - row_dim.descent();
	else
		d->anchor_ypos_ = offset + pm.ascent() + height_ / 2;

	updateMetrics();
	buffer_.changed();
}


FuncStatus BufferView::getStatus(FuncRequest const & cmd)
{
	FuncStatus flag;

	Cursor & cur = d->cursor_;

	switch (cmd.action) {

	case LFUN_UNDO:
		flag.enabled(buffer_.undo().hasUndoStack());
		break;
	case LFUN_REDO:
		flag.enabled(buffer_.undo().hasRedoStack());
		break;
	case LFUN_FILE_INSERT:
	case LFUN_FILE_INSERT_PLAINTEXT_PARA:
	case LFUN_FILE_INSERT_PLAINTEXT:
	case LFUN_BOOKMARK_SAVE:
		// FIXME: Actually, these LFUNS should be moved to Text
		flag.enabled(cur.inTexted());
		break;
	case LFUN_FONT_STATE:
	case LFUN_LABEL_INSERT:
	case LFUN_INFO_INSERT:
	case LFUN_PARAGRAPH_GOTO:
	case LFUN_NOTE_NEXT:
	case LFUN_REFERENCE_NEXT:
	case LFUN_WORD_FIND:
	case LFUN_WORD_REPLACE:
	case LFUN_MARK_OFF:
	case LFUN_MARK_ON:
	case LFUN_MARK_TOGGLE:
	case LFUN_SCREEN_RECENTER:
	case LFUN_BIBTEX_DATABASE_ADD:
	case LFUN_BIBTEX_DATABASE_DEL:
	case LFUN_STATISTICS:
	case LFUN_NEXT_INSET_TOGGLE:
		flag.enabled(true);
		break;

	case LFUN_LABEL_GOTO: {
		flag.enabled(!cmd.argument().empty()
		    || getInsetByCode<InsetRef>(cur, REF_CODE));
		break;
	}

	case LFUN_CHANGES_TRACK:
		flag.enabled(true);
		flag.setOnOff(buffer_.params().trackChanges);
		break;

	case LFUN_CHANGES_OUTPUT:
		flag.enabled(true);
		flag.setOnOff(buffer_.params().outputChanges);
		break;

	case LFUN_CHANGES_MERGE:
	case LFUN_CHANGE_NEXT:
	case LFUN_ALL_CHANGES_ACCEPT:
	case LFUN_ALL_CHANGES_REJECT:
		// TODO: context-sensitive enabling of LFUNs
		// In principle, these command should only be enabled if there
		// is a change in the document. However, without proper
		// optimizations, this will inevitably result in poor performance.
		flag.enabled(true);
		break;

	case LFUN_BUFFER_TOGGLE_COMPRESSION: {
		flag.setOnOff(buffer_.params().compressed);
		break;
	}
	
	case LFUN_BUFFER_TOGGLE_EMBEDDING: {
		flag.setOnOff(buffer_.params().embedded);
		break;
	}

	case LFUN_SCREEN_UP:
	case LFUN_SCREEN_DOWN:
	case LFUN_SCROLL:
		flag.enabled(true);
		break;

	// FIXME: LFUN_SCREEN_DOWN_SELECT should be removed from
	// everywhere else before this can enabled:
	case LFUN_SCREEN_UP_SELECT:
	case LFUN_SCREEN_DOWN_SELECT:
		flag.enabled(false);
		break;

	case LFUN_LAYOUT_TABULAR:
		flag.enabled(cur.innerInsetOfType(TABULAR_CODE));
		break;

	case LFUN_LAYOUT:
	case LFUN_LAYOUT_PARAGRAPH:
		flag.enabled(cur.inset().forceDefaultParagraphs(cur.idx()));
		break;

	case LFUN_INSET_SETTINGS: {
		InsetCode code = cur.inset().lyxCode();
		bool enable = false;
		switch (code) {
			case TABULAR_CODE:
				enable = cmd.argument() == "tabular";
				break;
			case ERT_CODE:
				enable = cmd.argument() == "ert";
				break;
			case FLOAT_CODE:
				enable = cmd.argument() == "float";
				break;
			case WRAP_CODE:
				enable = cmd.argument() == "wrap";
				break;
			case NOTE_CODE:
				enable = cmd.argument() == "note";
				break;
			case BRANCH_CODE:
				enable = cmd.argument() == "branch";
				break;
			case BOX_CODE:
				enable = cmd.argument() == "box";
				break;
			case LISTINGS_CODE:
				enable = cmd.argument() == "listings";
				break;
			default:
				break;
		}
		flag.enabled(enable);
		break;
	}

	case LFUN_DIALOG_SHOW_NEW_INSET:
		flag.enabled(cur.inset().lyxCode() != ERT_CODE &&
			cur.inset().lyxCode() != LISTINGS_CODE);
		if (cur.inset().lyxCode() == CAPTION_CODE) {
			FuncStatus flag;
			if (cur.inset().getStatus(cur, cmd, flag))
				return flag;
		}
		break;

	default:
		flag.enabled(false);
	}

	return flag;
}


bool BufferView::dispatch(FuncRequest const & cmd)
{
	//lyxerr << [ cmd = " << cmd << "]" << endl;

	// Make sure that the cached BufferView is correct.
	LYXERR(Debug::ACTION, " action[" << cmd.action << ']'
		<< " arg[" << to_utf8(cmd.argument()) << ']'
		<< " x[" << cmd.x << ']'
		<< " y[" << cmd.y << ']'
		<< " button[" << cmd.button() << ']');

	Cursor & cur = d->cursor_;

	switch (cmd.action) {

	case LFUN_UNDO:
		cur.message(_("Undo"));
		cur.clearSelection();
		if (!cur.textUndo())
			cur.message(_("No further undo information"));
		else
			processUpdateFlags(Update::Force | Update::FitCursor);
		break;

	case LFUN_REDO:
		cur.message(_("Redo"));
		cur.clearSelection();
		if (!cur.textRedo())
			cur.message(_("No further redo information"));
		else
			processUpdateFlags(Update::Force | Update::FitCursor);
		break;

	case LFUN_FONT_STATE:
		cur.message(cur.currentState());
		break;

	case LFUN_BOOKMARK_SAVE:
		saveBookmark(convert<unsigned int>(to_utf8(cmd.argument())));
		break;

	case LFUN_LABEL_GOTO: {
		docstring label = cmd.argument();
		if (label.empty()) {
			InsetRef * inset =
				getInsetByCode<InsetRef>(d->cursor_,
							 REF_CODE);
			if (inset) {
				label = inset->getParam("reference");
				// persistent=false: use temp_bookmark
				saveBookmark(0);
			}
		}

		if (!label.empty())
			gotoLabel(label);
		break;
	}

	case LFUN_PARAGRAPH_GOTO: {
		int const id = convert<int>(to_utf8(cmd.argument()));
		int i = 0;
		for (Buffer * b = &buffer_; i == 0 || b != &buffer_;
			b = theBufferList().next(b)) {

			ParIterator par = b->getParFromID(id);
			if (par == b->par_iterator_end()) {
				LYXERR(Debug::INFO, "No matching paragraph found! [" << id << "].");
			} else {
				LYXERR(Debug::INFO, "Paragraph " << par->id()
					<< " found in buffer `"
					<< b->absFileName() << "'.");

				if (b == &buffer_) {
					// Set the cursor
					setCursor(makeDocIterator(par, 0));
					showCursor();
				} else {
					// Switch to other buffer view and resend cmd
					theLyXFunc().dispatch(FuncRequest(
						LFUN_BUFFER_SWITCH, b->absFileName()));
					theLyXFunc().dispatch(cmd);
				}
				break;
			}
			++i;
		}
		break;
	}

	case LFUN_NOTE_NEXT:
		gotoInset(this, NOTE_CODE, false);
		break;

	case LFUN_REFERENCE_NEXT: {
		vector<InsetCode> tmp;
		tmp.push_back(LABEL_CODE);
		tmp.push_back(REF_CODE);
		gotoInset(this, tmp, true);
		break;
	}

	case LFUN_CHANGES_TRACK:
		buffer_.params().trackChanges = !buffer_.params().trackChanges;
		break;

	case LFUN_CHANGES_OUTPUT:
		buffer_.params().outputChanges = !buffer_.params().outputChanges;
		if (buffer_.params().outputChanges) {
			bool dvipost    = LaTeXFeatures::isAvailable("dvipost");
			bool xcolorsoul = LaTeXFeatures::isAvailable("soul") &&
					  LaTeXFeatures::isAvailable("xcolor");

			if (!dvipost && !xcolorsoul) {
				Alert::warning(_("Changes not shown in LaTeX output"),
					       _("Changes will not be highlighted in LaTeX output, "
						 "because neither dvipost nor xcolor/soul are installed.\n"
						 "Please install these packages or redefine "
						 "\\lyxadded and \\lyxdeleted in the LaTeX preamble."));
			} else if (!xcolorsoul) {
				Alert::warning(_("Changes not shown in LaTeX output"),
					       _("Changes will not be highlighted in LaTeX output "
						 "when using pdflatex, because xcolor and soul are not installed.\n"
						 "Please install both packages or redefine "
						 "\\lyxadded and \\lyxdeleted in the LaTeX preamble."));
			}
		}
		break;

	case LFUN_CHANGE_NEXT:
		findNextChange(this);
		break;

	case LFUN_CHANGES_MERGE:
		if (findNextChange(this))
			showDialog("changes");
		break;

	case LFUN_ALL_CHANGES_ACCEPT:
		// select complete document
		d->cursor_.reset(buffer_.inset());
		d->cursor_.selHandle(true);
		buffer_.text().cursorBottom(d->cursor_);
		// accept everything in a single step to support atomic undo
		buffer_.text().acceptOrRejectChanges(d->cursor_, Text::ACCEPT);
		break;

	case LFUN_ALL_CHANGES_REJECT:
		// select complete document
		d->cursor_.reset(buffer_.inset());
		d->cursor_.selHandle(true);
		buffer_.text().cursorBottom(d->cursor_);
		// reject everything in a single step to support atomic undo
		// Note: reject does not work recursively; the user may have to repeat the operation
		buffer_.text().acceptOrRejectChanges(d->cursor_, Text::REJECT);
		break;

	case LFUN_WORD_FIND:
		find(this, cmd);
		break;

	case LFUN_WORD_REPLACE: {
		bool has_deleted = false;
		if (cur.selection()) {
			DocIterator beg = cur.selectionBegin();
			DocIterator end = cur.selectionEnd();
			if (beg.pit() == end.pit()) {
				for (pos_type p = beg.pos() ; p < end.pos() ; ++p) {
					if (cur.paragraph().isDeleted(p))
						has_deleted = true;
				}
			}
		}
		replace(this, cmd, has_deleted);
		break;
	}

	case LFUN_MARK_OFF:
		cur.clearSelection();
		cur.resetAnchor();
		cur.message(from_utf8(N_("Mark off")));
		break;

	case LFUN_MARK_ON:
		cur.clearSelection();
		cur.mark() = true;
		cur.resetAnchor();
		cur.message(from_utf8(N_("Mark on")));
		break;

	case LFUN_MARK_TOGGLE:
		cur.clearSelection();
		if (cur.mark()) {
			cur.mark() = false;
			cur.message(from_utf8(N_("Mark removed")));
		} else {
			cur.mark() = true;
			cur.message(from_utf8(N_("Mark set")));
		}
		cur.resetAnchor();
		break;

	case LFUN_SCREEN_RECENTER:
		showCursor();
		break;

	case LFUN_BIBTEX_DATABASE_ADD: {
		Cursor tmpcur = d->cursor_;
		findInset(tmpcur, BIBTEX_CODE, false);
		InsetBibtex * inset = getInsetByCode<InsetBibtex>(tmpcur,
						BIBTEX_CODE);
		if (inset) {
			if (inset->addDatabase(to_utf8(cmd.argument())))
				buffer_.updateBibfilesCache();
		}
		break;
	}

	case LFUN_BIBTEX_DATABASE_DEL: {
		Cursor tmpcur = d->cursor_;
		findInset(tmpcur, BIBTEX_CODE, false);
		InsetBibtex * inset = getInsetByCode<InsetBibtex>(tmpcur,
						BIBTEX_CODE);
		if (inset) {
			if (inset->delDatabase(to_utf8(cmd.argument())))
				buffer_.updateBibfilesCache();
		}
		break;
	}

	case LFUN_STATISTICS: {
		DocIterator from, to;
		if (cur.selection()) {
			from = cur.selectionBegin();
			to = cur.selectionEnd();
		} else {
			from = doc_iterator_begin(buffer_.inset());
			to = doc_iterator_end(buffer_.inset());
		}
		int const words = countWords(from, to);
		int const chars = countChars(from, to, false);
		int const chars_blanks = countChars(from, to, true);
		docstring message;
		if (cur.selection())
			message = _("Statistics for the selection:\n");
		else
			message = _("Statistics for the document:\n");
		if (words != 1)
			message += bformat(_("\n%1$d words"), words);
		else
			message += _("\nOne word");
		if (chars_blanks != 1)
			message += bformat(_("\n%1$d characters (including blanks)"),
					  chars_blanks);
		else
			message += _("\nOne character (including blanks)");
		if (chars != 1)
			message += bformat(_("\n%1$d characters (excluding blanks)"),
					  chars);
		else
			message += _("\nOne character (excluding blanks)");

		Alert::information(_("Statistics"), message);
	}
		break;

	case LFUN_BUFFER_TOGGLE_COMPRESSION:
		// turn compression on/off
		buffer_.params().compressed = !buffer_.params().compressed;
		break;
	
	case LFUN_BUFFER_TOGGLE_EMBEDDING: {
		// turn embedding on/off
		try {
			buffer_.embeddedFiles().enable(!buffer_.params().embedded);
		} catch (ExceptionMessage const & message) {
			Alert::error(message.title_, message.details_);
		}
		break;
	}

	case LFUN_NEXT_INSET_TOGGLE: {
		// this is the real function we want to invoke
		FuncRequest tmpcmd = FuncRequest(LFUN_INSET_TOGGLE, cmd.origin);
		// if there is an inset at cursor, see whether it
		// wants to toggle.
		Inset * inset = cur.nextInset();
		if (inset) {
			if (inset->isActive()) {
				Cursor tmpcur = cur;
				tmpcur.pushBackward(*inset);
				inset->dispatch(tmpcur, tmpcmd);
				if (tmpcur.result().dispatched()) {
					cur.dispatched();
				}
			} else if (inset->editable() == Inset::IS_EDITABLE) {
				inset->edit(cur, true);
			}
		}
		// if it did not work, try the underlying inset.
		if (!cur.result().dispatched())
			cur.dispatch(tmpcmd);

		if (!cur.result().dispatched())
			// It did not work too; no action needed.
			break;
		cur.clearSelection();
		processUpdateFlags(Update::SinglePar | Update::FitCursor);
		break;
	}

	case LFUN_SCREEN_UP:
	case LFUN_SCREEN_DOWN: {
		Point p = getPos(cur, cur.boundary());
		if (p.y_ < 0 || p.y_ > height_) {
			// The cursor is off-screen so recenter before proceeding.
			showCursor();
			p = getPos(cur, cur.boundary());
		}
		scroll(cmd.action == LFUN_SCREEN_UP? - height_ : height_);
		cur.reset(buffer_.inset());
		d->text_metrics_[&buffer_.text()].editXY(cur, p.x_, p.y_);
		//FIXME: what to do with cur.x_target()?
		cur.finishUndo();
		break;
	}

	case LFUN_SCROLL:
		lfunScroll(cmd);
		break;

	case LFUN_SCREEN_UP_SELECT:
	case LFUN_SCREEN_DOWN_SELECT: {
		// Those two are not ready yet for consumption.
		return false;

		cur.selHandle(true);
		size_t initial_depth = cur.depth();
		Point const p = getPos(cur, cur.boundary());
		scroll(cmd.action == LFUN_SCREEN_UP_SELECT? - height_ : height_);
		// FIXME: We need to verify if the cursor stayed within an inset...
		//cur.reset(buffer_.inset());
		d->text_metrics_[&buffer_.text()].editXY(cur, p.x_, p.y_);
		cur.finishUndo();
		while (cur.depth() > initial_depth) {
			cur.forwardInset();
		}
		// FIXME: we need to do a redraw again because of the selection
		// But no screen update is needed.
		d->update_strategy_ = NoScreenUpdate;
		buffer_.changed();
		break;
	}

	default:
		return false;
	}

	return true;
}


docstring const BufferView::requestSelection()
{
	Cursor & cur = d->cursor_;

	if (!cur.selection()) {
		d->xsel_cache_.set = false;
		return docstring();
	}

	if (!d->xsel_cache_.set ||
	    cur.top() != d->xsel_cache_.cursor ||
	    cur.anchor_.top() != d->xsel_cache_.anchor)
	{
		d->xsel_cache_.cursor = cur.top();
		d->xsel_cache_.anchor = cur.anchor_.top();
		d->xsel_cache_.set = cur.selection();
		return cur.selectionAsString(false);
	}
	return docstring();
}


void BufferView::clearSelection()
{
	d->cursor_.clearSelection();
	// Clear the selection buffer. Otherwise a subsequent
	// middle-mouse-button paste would use the selection buffer,
	// not the more current external selection.
	cap::clearSelection();
	d->xsel_cache_.set = false;
	// The buffer did not really change, but this causes the
	// redraw we need because we cleared the selection above.
	buffer_.changed();
}


void BufferView::resize(int width, int height)
{
	// Update from work area
	width_ = width;
	height_ = height;

	// Clear the paragraph height cache.
	d->par_height_.clear();

	updateMetrics();
}


Inset const * BufferView::getCoveringInset(Text const & text,
		int x, int y) const
{
	TextMetrics & tm = d->text_metrics_[&text];
	Inset * inset = tm.checkInsetHit(x, y);
	if (!inset)
		return 0;

	if (!inset->descendable())
		// No need to go further down if the inset is not
		// descendable.
		return inset;

	size_t cell_number = inset->nargs();
	// Check all the inner cell.
	for (size_t i = 0; i != cell_number; ++i) {
		Text const * inner_text = inset->getText(i);
		if (inner_text) {
			// Try deeper.
			Inset const * inset_deeper =
				getCoveringInset(*inner_text, x, y);
			if (inset_deeper)
				return inset_deeper;
		}
	}

	return inset;
}


void BufferView::mouseEventDispatch(FuncRequest const & cmd0)
{
	//lyxerr << "[ cmd0 " << cmd0 << "]" << endl;

	// This is only called for mouse related events including
	// LFUN_FILE_OPEN generated by drag-and-drop.
	FuncRequest cmd = cmd0;

	Cursor cur(*this);
	cur.push(buffer_.inset());
	cur.selection() = d->cursor_.selection();

	// Either the inset under the cursor or the
	// surrounding Text will handle this event.

	// make sure we stay within the screen...
	cmd.y = min(max(cmd.y, -1), height_);

	if (cmd.action == LFUN_MOUSE_MOTION && cmd.button() == mouse_button::none) {

		// Get inset under mouse, if there is one.
		Inset const * covering_inset =
			getCoveringInset(buffer_.text(), cmd.x, cmd.y);
		if (covering_inset == d->last_inset_)
			// Same inset, no need to do anything...
			return;

		bool need_redraw = false;
		// const_cast because of setMouseHover().
		Inset * inset = const_cast<Inset *>(covering_inset);
		if (d->last_inset_)
			// Remove the hint on the last hovered inset (if any).
			need_redraw |= d->last_inset_->setMouseHover(false);
		if (inset)
			// Highlighted the newly hovered inset (if any).
			need_redraw |= inset->setMouseHover(true);
		d->last_inset_ = inset;
		if (!need_redraw)
			return;

		LYXERR(Debug::PAINTING, "Mouse hover detected at: ("
			<< cmd.x << ", " << cmd.y << ")");

		d->update_strategy_ = DecorationUpdate;

		// This event (moving without mouse click) is not passed further.
		// This should be changed if it is further utilized.
		buffer_.changed();
		return;
	}

	// Build temporary cursor.
	Inset * inset = d->text_metrics_[&buffer_.text()].editXY(cur, cmd.x, cmd.y);

	// Put anchor at the same position.
	cur.resetAnchor();

	// Try to dispatch to an non-editable inset near this position
	// via the temp cursor. If the inset wishes to change the real
	// cursor it has to do so explicitly by using
	//  cur.bv().cursor() = cur;  (or similar)
	if (inset)
		inset->dispatch(cur, cmd);

	// Now dispatch to the temporary cursor. If the real cursor should
	// be modified, the inset's dispatch has to do so explicitly.
	if (!cur.result().dispatched())
		cur.dispatch(cmd);

	//Do we have a selection?
	theSelection().haveSelection(cursor().selection());

	// If the command has been dispatched,
	if (cur.result().dispatched()
		// an update is asked,
		&& cur.result().update())
		processUpdateFlags(cur.result().update());
}


void BufferView::lfunScroll(FuncRequest const & cmd)
{
	string const scroll_type = cmd.getArg(0);
	int const scroll_step = 
		(scroll_type == "line")? d->scrollbarParameters_.lineScrollHeight
		: (scroll_type == "page")? height_ : 0;
	if (scroll_step == 0)
		return;
	string const scroll_quantity = cmd.getArg(1);
	if (scroll_quantity == "up")
		scrollUp(scroll_step);
	else if (scroll_quantity == "down")
		scrollDown(scroll_step);
	else {
		int const scroll_value = convert<int>(scroll_quantity);
		if (scroll_value)
			scroll(scroll_step * scroll_value);
	}
}


void BufferView::scroll(int y)
{
	if (y > 0)
		scrollDown(y);
	else if (y < 0)
		scrollUp(-y);
}


void BufferView::scrollDown(int offset)
{
	Text * text = &buffer_.text();
	TextMetrics & tm = d->text_metrics_[text];
	int ymax = height_ + offset;
	while (true) {
		pair<pit_type, ParagraphMetrics const *> last = tm.last();
		int bottom_pos = last.second->position() + last.second->descent();
		if (last.first + 1 == int(text->paragraphs().size())) {
			if (bottom_pos <= height_)
				return;
			offset = min(offset, bottom_pos - height_);
			break;
		}
		if (bottom_pos > ymax)
			break;
		tm.newParMetricsDown();
	}
	d->anchor_ypos_ -= offset;
	updateMetrics();
	buffer_.changed();
}


void BufferView::scrollUp(int offset)
{
	Text * text = &buffer_.text();
	TextMetrics & tm = d->text_metrics_[text];
	int ymin = - offset;
	while (true) {
		pair<pit_type, ParagraphMetrics const *> first = tm.first();
		int top_pos = first.second->position() - first.second->ascent();
		if (first.first == 0) {
			if (top_pos >= 0)
				return;
			offset = min(offset, - top_pos);
			break;
		}
		if (top_pos < ymin)
			break;
		tm.newParMetricsUp();
	}
	d->anchor_ypos_ += offset;
	updateMetrics();
	buffer_.changed();
}


void BufferView::setCursorFromRow(int row)
{
	int tmpid = -1;
	int tmppos = -1;

	buffer_.texrow().getIdFromRow(row, tmpid, tmppos);

	d->cursor_.reset(buffer_.inset());
	if (tmpid == -1)
		buffer_.text().setCursor(d->cursor_, 0, 0);
	else
		buffer_.text().setCursor(d->cursor_, buffer_.getParFromID(tmpid).pit(), tmppos);
}


void BufferView::gotoLabel(docstring const & label)
{
	for (InsetIterator it = inset_iterator_begin(buffer_.inset()); it; ++it) {
		vector<docstring> labels;
		it->getLabelList(buffer_, labels);
		if (find(labels.begin(), labels.end(), label) != labels.end()) {
			setCursor(it);
			showCursor();
			return;
		}
	}
}


TextMetrics const & BufferView::textMetrics(Text const * t) const
{
	return const_cast<BufferView *>(this)->textMetrics(t);
}


TextMetrics & BufferView::textMetrics(Text const * t)
{
	TextMetricsCache::iterator tmc_it  = d->text_metrics_.find(t);
	if (tmc_it == d->text_metrics_.end()) {
		tmc_it = d->text_metrics_.insert(
			make_pair(t, TextMetrics(this, const_cast<Text *>(t)))).first;
	}
	return tmc_it->second;
}


ParagraphMetrics const & BufferView::parMetrics(Text const * t,
		pit_type pit) const
{
	return textMetrics(t).parMetrics(pit);
}


int BufferView::workHeight() const
{
	return height_;
}


void BufferView::setCursor(DocIterator const & dit)
{
	size_t const n = dit.depth();
	for (size_t i = 0; i < n; ++i)
		dit[i].inset().edit(d->cursor_, true);

	d->cursor_.setCursor(dit);
	d->cursor_.selection() = false;
}


bool BufferView::checkDepm(Cursor & cur, Cursor & old)
{
	// Would be wrong to delete anything if we have a selection.
	if (cur.selection())
		return false;

	bool need_anchor_change = false;
	bool changed = d->cursor_.text()->deleteEmptyParagraphMechanism(cur, old,
		need_anchor_change);

	if (need_anchor_change)
		cur.resetAnchor();

	if (!changed)
		return false;

	d->cursor_ = cur;

	updateLabels(buffer_);

	updateMetrics();
	buffer_.changed();
	return true;
}


bool BufferView::mouseSetCursor(Cursor & cur, bool select)
{
	BOOST_ASSERT(&cur.bv() == this);

	if (!select)
		// this event will clear selection so we save selection for
		// persistent selection
		cap::saveSelection(cursor());

	// Has the cursor just left the inset?
	bool badcursor = false;
	bool leftinset = (&d->cursor_.inset() != &cur.inset());
	if (leftinset)
		badcursor = notifyCursorLeaves(d->cursor_, cur);

	// FIXME: shift-mouse selection doesn't work well across insets.
	bool do_selection = select && &d->cursor_.anchor().inset() == &cur.inset();

	// do the dEPM magic if needed
	// FIXME: (1) move this to InsetText::notifyCursorLeaves?
	// FIXME: (2) if we had a working InsetText::notifyCursorLeaves,
	// the leftinset bool would not be necessary (badcursor instead).
	bool update = leftinset;
	if (!do_selection && !badcursor && d->cursor_.inTexted())
		update |= checkDepm(cur, d->cursor_);

	// if the cursor was in an empty script inset and the new
	// position is in the nucleus of the inset, notifyCursorLeaves
	// will kill the script inset itself. So we check all the
	// elements of the cursor to make sure that they are correct.
	// For an example, see bug 2933:
	// http://bugzilla.lyx.org/show_bug.cgi?id=2933
	// The code below could maybe be moved to a DocIterator method.
	//lyxerr << "cur before " << cur <<endl;
	DocIterator dit(cur.inset());
	dit.push_back(cur.bottom());
	size_t i = 1;
	while (i < cur.depth() && dit.nextInset() == &cur[i].inset()) {
		dit.push_back(cur[i]);
		++i;
	}
	//lyxerr << "5 cur after" << dit <<endl;

	d->cursor_.setCursor(dit);
	d->cursor_.boundary(cur.boundary());
	if (do_selection)
		d->cursor_.setSelection();
	else
		d->cursor_.clearSelection();

	d->cursor_.finishUndo();
	d->cursor_.setCurrentFont();
	return update;
}


void BufferView::putSelectionAt(DocIterator const & cur,
				int length, bool backwards)
{
	d->cursor_.clearSelection();

	setCursor(cur);

	if (length) {
		if (backwards) {
			d->cursor_.pos() += length;
			d->cursor_.setSelection(d->cursor_, -length);
		} else
			d->cursor_.setSelection(d->cursor_, length);
	}
}


Cursor & BufferView::cursor()
{
	return d->cursor_;
}


Cursor const & BufferView::cursor() const
{
	return d->cursor_;
}


pit_type BufferView::anchor_ref() const
{
	return d->anchor_pit_;
}


bool BufferView::singleParUpdate()
{
	Text & buftext = buffer_.text();
	pit_type const bottom_pit = d->cursor_.bottom().pit();
	TextMetrics & tm = textMetrics(&buftext);
	int old_height = tm.parMetrics(bottom_pit).height();

	// In Single Paragraph mode, rebreak only
	// the (main text, not inset!) paragraph containing the cursor.
	// (if this paragraph contains insets etc., rebreaking will
	// recursively descend)
	tm.redoParagraph(bottom_pit);
	ParagraphMetrics const & pm = tm.parMetrics(bottom_pit);		
	if (pm.height() != old_height)
		// Paragraph height has changed so we cannot proceed to
		// the singlePar optimisation.
		return false;

	d->update_strategy_ = SingleParUpdate;

	LYXERR(Debug::PAINTING, "\ny1: " << pm.position() - pm.ascent()
		<< " y2: " << pm.position() + pm.descent()
		<< " pit: " << bottom_pit
		<< " singlepar: 1");
	return true;
}


void BufferView::updateMetrics()
{
	Text & buftext = buffer_.text();
	pit_type const npit = int(buftext.paragraphs().size());

	// Clear out the position cache in case of full screen redraw,
	d->coord_cache_.clear();

	// Clear out paragraph metrics to avoid having invalid metrics
	// in the cache from paragraphs not relayouted below
	// The complete text metrics will be redone.
	d->text_metrics_.clear();

	TextMetrics & tm = textMetrics(&buftext);

	// Rebreak anchor paragraph.
	tm.redoParagraph(d->anchor_pit_);
	ParagraphMetrics & anchor_pm = tm.par_metrics_[d->anchor_pit_];
	anchor_pm.setPosition(d->anchor_ypos_);

	LYXERR(Debug::PAINTING, "metrics: "
		<< " anchor pit = " << d->anchor_pit_
		<< " anchor ypos = " << d->anchor_ypos_);

	// Redo paragraphs above anchor if necessary.
	int y1 = d->anchor_ypos_ - anchor_pm.ascent();
	// We are now just above the anchor paragraph.
	pit_type pit1 = d->anchor_pit_ - 1;
	for (; pit1 >= 0 && y1 >= 0; --pit1) {
		tm.redoParagraph(pit1);
		ParagraphMetrics & pm = tm.par_metrics_[pit1];
		y1 -= pm.descent();
		// Save the paragraph position in the cache.
		pm.setPosition(y1);
		y1 -= pm.ascent();
	}

	// Redo paragraphs below the anchor if necessary.
	int y2 = d->anchor_ypos_ + anchor_pm.descent();
	// We are now just below the anchor paragraph.
	pit_type pit2 = d->anchor_pit_ + 1;
	for (; pit2 < npit && y2 <= height_; ++pit2) {
		tm.redoParagraph(pit2);
		ParagraphMetrics & pm = tm.par_metrics_[pit2];
		y2 += pm.ascent();
		// Save the paragraph position in the cache.
		pm.setPosition(y2);
		y2 += pm.descent();
	}

	LYXERR(Debug::PAINTING, "Metrics: "
		<< " anchor pit = " << d->anchor_pit_
		<< " anchor ypos = " << d->anchor_ypos_
		<< " y1 = " << y1
		<< " y2 = " << y2
		<< " pit1 = " << pit1
		<< " pit2 = " << pit2);

	d->update_strategy_ = FullScreenUpdate;

	if (lyxerr.debugging(Debug::WORKAREA)) {
		LYXERR(Debug::WORKAREA, "BufferView::updateMetrics");
		d->coord_cache_.dump();
	}
}


void BufferView::insertLyXFile(FileName const & fname)
{
	BOOST_ASSERT(d->cursor_.inTexted());

	// Get absolute path of file and add ".lyx"
	// to the filename if necessary
	FileName filename = fileSearch(string(), fname.absFilename(), "lyx");

	docstring const disp_fn = makeDisplayPath(filename.absFilename());
	// emit message signal.
	message(bformat(_("Inserting document %1$s..."), disp_fn));

	docstring res;
	Buffer buf("", false);
	if (buf.loadLyXFile(filename)) {
		ErrorList & el = buffer_.errorList("Parse");
		// Copy the inserted document error list into the current buffer one.
		el = buf.errorList("Parse");
		buffer_.undo().recordUndo(d->cursor_);
		cap::pasteParagraphList(d->cursor_, buf.paragraphs(),
					     buf.params().getTextClassPtr(), el);
		res = _("Document %1$s inserted.");
	} else {
		res = _("Could not insert document %1$s");
	}

	updateMetrics();
	buffer_.changed();
	// emit message signal.
	message(bformat(res, disp_fn));
	buffer_.errors("Parse");
}


Point BufferView::coordOffset(DocIterator const & dit, bool boundary) const
{
	int x = 0;
	int y = 0;
	int lastw = 0;

	// Addup contribution of nested insets, from inside to outside,
 	// keeping the outer paragraph for a special handling below
	for (size_t i = dit.depth() - 1; i >= 1; --i) {
		CursorSlice const & sl = dit[i];
		int xx = 0;
		int yy = 0;
		
		// get relative position inside sl.inset()
		sl.inset().cursorPos(*this, sl, boundary && (i + 1 == dit.depth()), xx, yy);
		
		// Make relative position inside of the edited inset relative to sl.inset()
		x += xx;
		y += yy;
		
		// In case of an RTL inset, the edited inset will be positioned to the left
		// of xx:yy
		if (sl.text()) {
			bool boundary_i = boundary && i + 1 == dit.depth();
			bool rtl = textMetrics(sl.text()).isRTL(sl, boundary_i);
			if (rtl)
				x -= lastw;
		}

		// remember width for the case that sl.inset() is positioned in an RTL inset
		if (i && dit[i - 1].text()) {
			// If this Inset is inside a Text Inset, retrieve the Dimension
			// from the containing text instead of using Inset::dimension() which
			// might not be implemented.
			// FIXME (Abdel 23/09/2007): this is a bit messy because of the
			// elimination of Inset::dim_ cache. This coordOffset() method needs
			// to be rewritten in light of the new design.
			Dimension const & dim = parMetrics(dit[i - 1].text(),
				dit[i - 1].pit()).insetDimension(&sl.inset());
			lastw = dim.wid;
		} else {
			Dimension const dim = sl.inset().dimension(*this);
			lastw = dim.wid;
		}
		
		//lyxerr << "Cursor::getPos, i: "
		// << i << " x: " << xx << " y: " << y << endl;
	}

	// Add contribution of initial rows of outermost paragraph
	CursorSlice const & sl = dit[0];
	TextMetrics const & tm = textMetrics(sl.text());
	ParagraphMetrics const & pm = tm.parMetrics(sl.pit());
	BOOST_ASSERT(!pm.rows().empty());
	y -= pm.rows()[0].ascent();
#if 1
	// FIXME: document this mess
	size_t rend;
	if (sl.pos() > 0 && dit.depth() == 1) {
		int pos = sl.pos();
		if (pos && boundary)
			--pos;
//		lyxerr << "coordOffset: boundary:" << boundary << " depth:" << dit.depth() << " pos:" << pos << " sl.pos:" << sl.pos() << endl;
		rend = pm.pos2row(pos);
	} else
		rend = pm.pos2row(sl.pos());
#else
	size_t rend = pm.pos2row(sl.pos());
#endif
	for (size_t rit = 0; rit != rend; ++rit)
		y += pm.rows()[rit].height();
	y += pm.rows()[rend].ascent();
	
	TextMetrics const & bottom_tm = textMetrics(dit.bottom().text());
	
	// Make relative position from the nested inset now bufferview absolute.
	int xx = bottom_tm.cursorX(dit.bottom(), boundary && dit.depth() == 1);
	x += xx;
	
	// In the RTL case place the nested inset at the left of the cursor in 
	// the outer paragraph
	bool boundary_1 = boundary && 1 == dit.depth();
	bool rtl = bottom_tm.isRTL(dit.bottom(), boundary_1);
	if (rtl)
		x -= lastw;
	
	return Point(x, y);
}


Point BufferView::getPos(DocIterator const & dit, bool boundary) const
{
	CursorSlice const & bot = dit.bottom();
	TextMetrics const & tm = textMetrics(bot.text());
	if (!tm.has(bot.pit()))
		return Point(-1, -1);

	Point p = coordOffset(dit, boundary); // offset from outer paragraph
	p.y_ += tm.parMetrics(bot.pit()).position();
	return p;
}


void BufferView::draw(frontend::Painter & pain)
{
	LYXERR(Debug::PAINTING, "\t\t*** START DRAWING ***");
	Text & text = buffer_.text();
	TextMetrics const & tm = d->text_metrics_[&text];
	int const y = tm.first().second->position();
	PainterInfo pi(this, pain);

	switch (d->update_strategy_) {

	case NoScreenUpdate:
		// If no screen painting is actually needed, only some the different
		// coordinates of insets and paragraphs needs to be updated.
		pi.full_repaint = true;
		pi.pain.setDrawingEnabled(false);
 		tm.draw(pi, 0, y);
		break;

	case SingleParUpdate:
		pi.full_repaint = false;
		// In general, only the current row of the outermost paragraph
		// will be redrawn. Particular cases where selection spans
		// multiple paragraph are correctly detected in TextMetrics.
 		tm.draw(pi, 0, y);
		break;

	case DecorationUpdate:
		// FIXME: We should also distinguish DecorationUpdate to avoid text
		// drawing if possible. This is not possible to do easily right now
		// because of the single backing pixmap.

	case FullScreenUpdate:
		// The whole screen, including insets, will be refreshed.
		pi.full_repaint = true;

		// Clear background.
		pain.fillRectangle(0, 0, width_, height_,
			buffer_.inset().backgroundColor());

		// Draw everything.
		tm.draw(pi, 0, y);

		// and possibly grey out below
		pair<pit_type, ParagraphMetrics const *> lastpm = tm.last();
		int const y2 = lastpm.second->position() + lastpm.second->descent();
		if (y2 < height_)
			pain.fillRectangle(0, y2, width_, height_ - y2, Color_bottomarea);
		break;
	}
	LYXERR(Debug::PAINTING, "\n\t\t*** END DRAWING  ***");

	// The scrollbar needs an update.
	updateScrollbar();

	// Normalize anchor for next time
	pair<pit_type, ParagraphMetrics const *> firstpm = tm.first();
	pair<pit_type, ParagraphMetrics const *> lastpm = tm.last();
	for (pit_type pit = firstpm.first; pit <= lastpm.first; ++pit) {
		ParagraphMetrics const & pm = tm.parMetrics(pit);
		if (pm.position() + pm.descent() > 0) {
			d->anchor_pit_ = pit;
			d->anchor_ypos_ = pm.position();
			break;
		}
	}
	LYXERR(Debug::PAINTING, "Found new anchor pit = " << d->anchor_pit_
		<< "  anchor ypos = " << d->anchor_ypos_);
}


void BufferView::message(docstring const & msg)
{
	if (d->gui_)
		d->gui_->message(msg);
}


void BufferView::showDialog(string const & name)
{
	if (d->gui_)
		d->gui_->showDialog(name, string());
}


void BufferView::showDialog(string const & name,
	string const & data, Inset * inset)
{
	if (d->gui_)
		d->gui_->showDialog(name, data, inset);
}


void BufferView::updateDialog(string const & name, string const & data)
{
	if (d->gui_)
		d->gui_->updateDialog(name, data);
}


void BufferView::setGuiDelegate(frontend::GuiBufferViewDelegate * gui)
{
	d->gui_ = gui;
}


// FIXME: Move this out of BufferView again
docstring BufferView::contentsOfPlaintextFile(FileName const & fname)
{
	if (!fname.isReadableFile()) {
		docstring const error = from_ascii(strerror(errno));
		docstring const file = makeDisplayPath(fname.absFilename(), 50);
		docstring const text =
		  bformat(_("Could not read the specified document\n"
			    "%1$s\ndue to the error: %2$s"), file, error);
		Alert::error(_("Could not read file"), text);
		return docstring();
	}

	if (!fname.isReadableFile()) {
		docstring const file = makeDisplayPath(fname.absFilename(), 50);
		docstring const text =
		  bformat(_("%1$s\n is not readable."), file);
		Alert::error(_("Could not open file"), text);
		return docstring();
	}

	// FIXME UNICODE: We don't know the encoding of the file
	docstring file_content = fname.fileContents("UTF-8");
	if (file_content.empty()) {
		Alert::error(_("Reading not UTF-8 encoded file"),
			     _("The file is not UTF-8 encoded.\n"
			       "It will be read as local 8Bit-encoded.\n"
			       "If this does not give the correct result\n"
			       "then please change the encoding of the file\n"
			       "to UTF-8 with a program other than LyX.\n"));
		file_content = fname.fileContents("local8bit");
	}

	return normalize_c(file_content);
}


void BufferView::insertPlaintextFile(FileName const & f, bool asParagraph)
{
	docstring const tmpstr = contentsOfPlaintextFile(f);

	if (tmpstr.empty())
		return;

	Cursor & cur = cursor();
	cap::replaceSelection(cur);
	buffer_.undo().recordUndo(cur);
	if (asParagraph)
		cur.innerText()->insertStringAsParagraphs(cur, tmpstr);
	else
		cur.innerText()->insertStringAsLines(cur, tmpstr);

	updateMetrics();
	buffer_.changed();
}

} // namespace lyx
