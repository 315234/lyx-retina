/**
 * \file GuiCompleter.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Stefan Schimanski
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiWorkArea.h"

#include "Buffer.h"
#include "BufferView.h"
#include "Cursor.h"
#include "Dimension.h"
#include "FuncRequest.h"
#include "GuiView.h"
#include "LyXFunc.h"
#include "LyXRC.h"
#include "Paragraph.h"
#include "version.h"

#include "support/debug.h"

#include <QApplication>
#include <QAbstractListModel>
#include <QHeaderView>
#include <QPainter>
#include <QPixmapCache>
#include <QScrollBar>
#include <QItemDelegate>
#include <QTreeView>
#include <QTimer>

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

class RtlItemDelegate : public QItemDelegate {
public:
	explicit RtlItemDelegate(QObject * parent = 0)
		: QItemDelegate(parent) {}

	void setEnabled(bool enabled = true)
	{
		enabled_ = enabled;
	}
	
protected:
	virtual void drawDisplay(QPainter * painter,
		QStyleOptionViewItem const & option,
		QRect const & rect, QString const & text) const
	{
		if (!enabled_)
			return QItemDelegate::drawDisplay(painter, option, rect, text);

		// FIXME: do this more elegantly
		docstring stltext = qstring_to_ucs4(text);
		reverse(stltext.begin(), stltext.end());
		QItemDelegate::drawDisplay(painter, option, rect, toqstr(stltext));
	}
	
private:
	bool enabled_;
};


class PixmapItemDelegate : public QItemDelegate {
public:
	explicit PixmapItemDelegate(QObject *parent = 0)
	: QItemDelegate(parent) {}

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		   const QModelIndex &index) const
	{
		QStyleOptionViewItem opt = setOptions(index, option);
		QVariant value = index.data(Qt::DisplayRole);
		QPixmap pixmap = qvariant_cast<QPixmap>(value);
		
		// draw
		painter->save();
		drawBackground(painter, opt, index);
		if (!pixmap.isNull()) {
			const QSize size = pixmap.size();
			painter->drawPixmap(option.rect.left() + (16 - size.width()) / 2,
				option.rect.top() + (option.rect.height() - size.height()) / 2,
				pixmap);
		}
		drawFocus(painter, opt, option.rect);
		painter->restore();
	}
};


class GuiCompletionModel : public QAbstractListModel {
public:
	///
	GuiCompletionModel(QObject * parent,
		Inset::CompletionList const * l)
		: QAbstractListModel(parent), list_(l) {}
	///
	~GuiCompletionModel()
		{ delete list_; }
	///
	bool sorted() const
	{
		if (list_)
			return list_->sorted();
		else
			return false;
	}
	///
	int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const
	{
		return 2;
	}
	///
	int rowCount(const QModelIndex & /*parent*/ = QModelIndex()) const
	{
		if (list_ == 0)
			return 0;
		else
			return list_->size();
	}

	///
	QVariant data(const QModelIndex & index, int role) const
	{
		if (list_ == 0)
			return QVariant();

		if (index.row() < 0 || index.row() >= rowCount())
			return QVariant();

		if (role != Qt::DisplayRole && role != Qt::EditRole)
		    return QVariant();
		    
		if (index.column() == 0)
			return toqstr(list_->data(index.row()));
		else if (index.column() == 1) {
			// get icon from cache
			QPixmap scaled;
			QString const name = ":" + toqstr(list_->icon(index.row()));
			if (!QPixmapCache::find("completion" + name, scaled)) {
				// load icon from disk
				QPixmap p = QPixmap(name);
				if (!p.isNull()) {
					// scale it to 16x16 or smaller
					scaled = p.scaled(min(16, p.width()), min(16, p.height()), 
						Qt::KeepAspectRatio, Qt::SmoothTransformation);
				}

				QPixmapCache::insert("completion" + name, scaled);
			}
			return scaled;
		}
		return QVariant();
	}

private:
	///
	Inset::CompletionList const * list_;
};


GuiCompleter::GuiCompleter(GuiWorkArea * gui, QObject * parent)
	: QCompleter(parent), gui_(gui), updateLock_(0),
	  inlineVisible_(false)
{
	// Setup the completion popup
	setModel(new GuiCompletionModel(this, 0));
	setCompletionMode(QCompleter::PopupCompletion);
	setWidget(gui_);
	
	// create the popup
	QTreeView *listView = new QTreeView;
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->header()->hide();
	listView->setIndentation(0);
	listView->setUniformRowHeights(true);
	setPopup(listView);
	popup()->setItemDelegateForColumn(1, new PixmapItemDelegate(this));
	rtlItemDelegate_ = new RtlItemDelegate(this);
	popup()->setItemDelegateForColumn(0, rtlItemDelegate_);

	// create timeout timers
	popup_timer_.setSingleShot(true);
	inline_timer_.setSingleShot(true);
	connect(this, SIGNAL(highlighted(const QString &)),
		this, SLOT(popupHighlighted(const QString &)));
	connect(this, SIGNAL(activated(const QString &)),
		this, SLOT(popupActivated(const QString &)));
	connect(&popup_timer_, SIGNAL(timeout()),
		this, SLOT(showPopup()));
	connect(&inline_timer_, SIGNAL(timeout()),
		this, SLOT(showInline()));
}


GuiCompleter::~GuiCompleter()
{
	popup()->hide();
}


bool GuiCompleter::eventFilter(QObject * watched, QEvent * e)
{
	// hijack back the tab key from the popup
	// (which stole it from the workspace before)
	if (e->type() == QEvent::KeyPress && popupVisible()) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(e);
		switch (ke->key()) {
		case Qt::Key_Tab:
			tab();
			ke->accept();
			return true;
		default: break;
		}
	}
	
	return QCompleter::eventFilter(watched, e);
}


bool GuiCompleter::popupPossible(Cursor const & cur) const
{
	return QApplication::activeWindow()
		&& gui_->hasFocus()
		&& cur.inset().completionSupported(cur);
}


bool GuiCompleter::inlinePossible(Cursor const & cur) const
{
	return cur.inset().inlineCompletionSupported(cur);
}


bool GuiCompleter::completionAvailable() const
{
	size_t n = popup()->model()->rowCount();

	// if there is exactly one, we have to check whether it is a 
	// real completion, i.e. longer than the current prefix.
	if (n == 1 && completionPrefix() == currentCompletion())
	    return false;

	return n > 0;
}


bool GuiCompleter::popupVisible() const
{
	return popup()->isVisible();
}


bool GuiCompleter::inlineVisible() const
{
	// In fact using BufferView::inlineCompletionPos.empty() should be
	// here. But unfortunately this information is not good enough
	// because destructive operations like backspace might invalidate
	// inlineCompletionPos. But then the completion should stay visible
	// (i.e. reshown on the next update). Hence be keep this information
	// in the inlineVisible_ variable.
	return inlineVisible_;
}


void GuiCompleter::updateVisibility(Cursor & cur, bool start, bool keep, bool cursorInView)
{
	// parameters which affect the completion
	bool moved = cur != old_cursor_;
	if (moved)
		old_cursor_ = cur;

	bool possiblePopupState = popupPossible(cur) && cursorInView;
	bool possibleInlineState = inlinePossible(cur) && cursorInView;

	// we moved or popup state is not ok for popup?
	if ((moved && !keep) || !possiblePopupState)
		hidePopup(cur);

	// we moved or inline state is not ok for inline completion?
	if ((moved && !keep) || !possibleInlineState)
		hideInline(cur);

	// we inserted something and are in a possible popup state?
	if (!popupVisible() && possiblePopupState && start
		&& cur.inset().automaticPopupCompletion())
		popup_timer_.start(int(lyxrc.completion_popup_delay * 1000));

	// we inserted something and are in a possible inline completion state?
	if (!inlineVisible() && possibleInlineState && start
		&& cur.inset().automaticInlineCompletion())
		inline_timer_.start(int(lyxrc.completion_inline_delay * 1000));

	// update prefix if any completion is possible
	bool modelActive = model()->rowCount() > 0;
	if (possiblePopupState || possibleInlineState) {
		if (modelActive)
			updatePrefix(cur);
		else
			updateAvailability();
	}
}


void GuiCompleter::updateVisibility(bool start, bool keep)
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	updateVisibility(cur, start, keep);
	
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::updatePrefix(Cursor & cur)
{
	// get new prefix. Do nothing if unchanged
	QString newPrefix = toqstr(cur.inset().completionPrefix(cur));
	if (newPrefix == completionPrefix())
		return;
	
	// value which should be kept selected
	QString old = currentCompletion();
	if (old.length() == 0)
		old = last_selection_;
	
	// update completer to new prefix
	setCompletionPrefix(newPrefix);

	// update popup because its size might have changed
	if (popupVisible())
		updatePopup(cur);

	// restore old selection
	setCurrentCompletion(old);
	
	// if popup is not empty, the new selection will
	// be our last valid one
	QString const & s = currentCompletion();
	if (popupVisible() || inlineVisible()) {
		if (s.length() > 0)
			last_selection_ = s;
		else
			last_selection_ = old;
	}

	// update inline completion because the default
	// completion string might have changed
	if (inlineVisible())
		updateInline(cur, s);
}


void GuiCompleter::updateInline(Cursor & cur, QString const & completion)
{
	if (!cur.inset().inlineCompletionSupported(cur))
		return;
	
	// compute postfix
	docstring prefix = cur.inset().completionPrefix(cur);
	docstring postfix = from_utf8(fromqstr(completion.mid(prefix.length())));
	
	// shorten it if necessary
	if (lyxrc.completion_inline_dots != -1
	    && postfix.size() > unsigned(lyxrc.completion_inline_dots))
		postfix = postfix.substr(0, lyxrc.completion_inline_dots - 1) + "...";

	// set inline completion at cursor position
	size_t uniqueTo = max(longestUniqueCompletion().size(), prefix.size());
	gui_->bufferView().setInlineCompletion(cur, cur, postfix, uniqueTo - prefix.size());
	inlineVisible_ = true;
}


void GuiCompleter::updatePopup(Cursor & cur)
{
	if (!cur.inset().completionSupported(cur))
		return;
	
	if (completionCount() == 0)
		return;
	
	// get dimensions of completion prefix
	Dimension dim;
	int x;
	int y;
	cur.inset().completionPosAndDim(cur, x, y, dim);
	
	// and calculate the rect of the popup
	QRect rect;
	if (popup()->layoutDirection() == Qt::RightToLeft)
		rect = QRect(x + dim.width() - 200, y - dim.ascent() - 3, 200, dim.height() + 6);
	else
		rect = QRect(x, y - dim.ascent() - 3, 200, dim.height() + 6);
	
	// Resize the columns in the popup.
	// This should really be in the constructor. But somehow the treeview
	// has a bad memory about it and we have to tell him again and again.
	QTreeView * listView = static_cast<QTreeView *>(popup());
	listView->header()->setStretchLastSection(false);
	listView->header()->setResizeMode(0, QHeaderView::Stretch);
	listView->header()->setResizeMode(1, QHeaderView::Fixed);
	listView->header()->resizeSection(1, 22);
	
	// show/update popup
	complete(rect);
}


void GuiCompleter::updateAvailability()
{
	// this should really only be of interest if no completion is
	// visible yet, i.e. especially if automatic completion is disabled.
	if (inlineVisible() || popupVisible())
		return;
	Cursor & cur = gui_->bufferView().cursor();
	if (!popupPossible(cur) && !inlinePossible(cur))
		return;
	
	updateModel(cur, false, false);
}
	

void GuiCompleter::updateModel(Cursor & cur, bool popupUpdate, bool inlineUpdate)
{
	// value which should be kept selected
	QString old = currentCompletion();
	if (old.length() == 0)
		old = last_selection_;

	// set whether rtl
	bool rtl = false;
	if (cur.inTexted()) {
		Paragraph const & par = cur.paragraph();
		Font const font =
		par.getFontSettings(cur.bv().buffer().params(), cur.pos());
		rtl = font.isVisibleRightToLeft();
	}
	popup()->setLayoutDirection(rtl ? Qt::RightToLeft : Qt::LeftToRight);

	// turn the direction of the strings in the popup.
	// Qt does not do that itself.
	rtlItemDelegate_->setEnabled(rtl);

	// set new model
	Inset::CompletionList const * list = cur.inset().createCompletionList(cur);
	setModel(new GuiCompletionModel(this, list));
	if (list->sorted())
		setModelSorting(QCompleter::CaseSensitivelySortedModel);
	else
		setModelSorting(QCompleter::UnsortedModel);

	// set prefix
	QString newPrefix = toqstr(cur.inset().completionPrefix(cur));
	if (newPrefix != completionPrefix())
		setCompletionPrefix(newPrefix);

	// show popup
	if (popupUpdate)
		updatePopup(cur);

	// restore old selection
	setCurrentCompletion(old);
	
	// if popup is not empty, the new selection will
	// be our last valid one
	if (popupVisible() || inlineVisible()) {
		QString const & s = currentCompletion();
		if (s.length() > 0)
			last_selection_ = s;
		else
			last_selection_ = old;
	}

	// show inline completion
	if (inlineUpdate)
		updateInline(cur, currentCompletion());
}


void GuiCompleter::showPopup(Cursor & cur)
{
	if (!popupPossible(cur))
		return;
	
	updateModel(cur, true, inlineVisible());
}


void GuiCompleter::hidePopup(Cursor & cur)
{
	// hide popup asynchronously because we might be here inside of
	// LFUN dispatchers. Hiding a popup can trigger a focus event on the 
	// workarea which then redisplays the cursor. But the metrics are not
	// yet up to date such that the coord cache has not all insets yet. The
	// cursorPos methods would triggers asserts in the coord cache then.
	QTimer::singleShot(0, popup(), SLOT(hide()));
	
	if (popup_timer_.isActive())
		popup_timer_.stop();
	
	if (!inlineVisible())
		setModel(new GuiCompletionModel(this, 0));
}


void GuiCompleter::showInline(Cursor & cur)
{
	if (!inlinePossible(cur))
		return;
	
	updateModel(cur, popupVisible(), true);
}


void GuiCompleter::hideInline(Cursor & cur)
{
	gui_->bufferView().setInlineCompletion(cur, DocIterator(), docstring());
	inlineVisible_ = false;
	
	if (!popupVisible())
		setModel(new GuiCompletionModel(this, 0));
}


void GuiCompleter::showPopup()
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	showPopup(cur);

	// redraw if needed
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::showInline()
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	showInline(cur);

	// redraw if needed
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::hidePopup()
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	hidePopup(cur);
	
	// redraw if needed
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::hideInline()
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	hideInline(cur);
	
	// redraw if needed
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::activate()
{
	if (!popupVisible() && !inlineVisible())
		return;

	popupActivated(currentCompletion());
}


void GuiCompleter::tab()
{
	BufferView * bv = &gui_->bufferView();
	Cursor cur = bv->cursor();
	cur.updateFlags(Update::None);
	
	// check that inline completion is active
	if (!inlineVisible()) {
		// try to activate the inline completion
		if (cur.inset().inlineCompletionSupported(cur)) {
			showInline();
			
			// show popup without delay because the completion was not unique
			if (lyxrc.completion_popup_after_complete
			    && !popupVisible()
			    && popup()->model()->rowCount() > 1)
				popup_timer_.start(0);

			return;
		}
		// or try popup
		if (!popupVisible() && cur.inset().completionSupported(cur)) {
			showPopup();
			return;
		}
		
		return;
	}
	
	// If completion is active, at least complete by one character
	docstring prefix = cur.inset().completionPrefix(cur);
	docstring completion = from_utf8(fromqstr(currentCompletion()));
	if (completion.size() <= prefix.size()) {
		// finalize completion
		cur.inset().insertCompletion(cur, docstring(), true);
		
		// hide popup and inline completion
		hidePopup(cur);
		hideInline(cur);
		updateVisibility(false, false);
		return;
	}
	docstring nextchar = completion.substr(prefix.size(), 1);
	if (!cur.inset().insertCompletion(cur, nextchar, false))
		return;
	updatePrefix(cur);

	// try to complete as far as it is unique
	docstring longestCompletion = longestUniqueCompletion();
	prefix = cur.inset().completionPrefix(cur);
	docstring postfix = longestCompletion.substr(min(longestCompletion.size(), prefix.size()));
	cur.inset().insertCompletion(cur, postfix, false);
	old_cursor_ = bv->cursor();
	updatePrefix(cur);

	// show popup without delay because the completion was not unique
	if (lyxrc.completion_popup_after_complete
	    && !popupVisible()
	    && popup()->model()->rowCount() > 1)
		popup_timer_.start(0);

	// redraw if needed
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


QString GuiCompleter::currentCompletion() const
{
	if (!popup()->selectionModel()->hasSelection())
		return QString();

	// Not sure if this is bug in Qt: currentIndex() always 
	// return the first element in the list.
	QModelIndex idx = popup()->currentIndex();
	return popup()->model()->data(idx, Qt::EditRole).toString();
}


void GuiCompleter::setCurrentCompletion(QString const & s)
{	
	QAbstractItemModel const & model = *popup()->model();
	size_t n = model.rowCount();
	if (n == 0)
		return;

	// select the first if s is empty
	if (s.length() == 0) {
		updateLock_++;
		popup()->setCurrentIndex(model.index(0, 0));
		updateLock_--;
		return;
	}

	// find old selection in model
	size_t i;
	if (modelSorting() == QCompleter::UnsortedModel) {
		// In unsorted models, iterate through list until the s is found
		for (i = 0; i < n; ++i) {
			QString const & is
			= model.data(model.index(i, 0), Qt::EditRole).toString();
			if (is == s)
				break;
		}
	} else {
		// In sorted models, do binary search for s.
		int l = 0;
		int r = n - 1;
		while (r >= l && l < int(n)) {
			size_t mid = (r + l) / 2;
			QString const & mids
			= model.data(model.index(mid, 0),
				     Qt::EditRole).toString();

			// left or right?
			// FIXME: is this really the same order that the docstring
			// from the CompletionList has?
			int c = s.compare(mids, Qt::CaseSensitive);
			if (c == 0) {
				l = mid;
				break;
			} else if (l == r) {
				l = n;
				break;
			} else if (c > 0)
				// middle is not far enough
				l = mid + 1;
			else
				// middle is too far
				r = mid - 1;
		}

		// loop was left without finding anything
		if (r < l)
			i = n;
		else
			i = l;
		BOOST_ASSERT(0 <= i && i <= n);
	}

	// select the first if none was found
	if (i == n)
		i = 0;

	updateLock_++;
	popup()->setCurrentIndex(model.index(i, 0));
	updateLock_--;
}


size_t commonPrefix(QString const & s1, QString const & s2)
{
	// find common prefix
	size_t j;
	size_t n1 = s1.length();
	size_t n2 = s2.length();
	for (j = 0; j < n1 && j < n2; ++j) {
		if (s1.at(j) != s2.at(j))
			break;
	}
	return j;
}


docstring GuiCompleter::longestUniqueCompletion() const
{
	QAbstractItemModel const & model = *popup()->model();
	size_t n = model.rowCount();
	if (n == 0)
		return docstring();
	QString s = model.data(model.index(0, 0), Qt::EditRole).toString();
	
	if (modelSorting() == QCompleter::UnsortedModel) {
		// For unsorted model we cannot do more than iteration.
		// Iterate through the completions and cut off where s differs
		for (size_t i = 0; i < n && s.length() > 0; ++i) {
			QString const & is
			= model.data(model.index(i, 0), Qt::EditRole).toString();

			s = s.left(commonPrefix(is, s));
		}
	} else {
		// For sorted models we can do binary search multiple times,
		// each time to find the first string which has s not as prefix.
		size_t i = 0;
		while (i < n && s.length() > 0) {
			// find first string that does not have s as prefix
			// via binary search in [i,n-1]
			size_t r = n - 1;
			do {
				// get common prefix with the middle string
				size_t mid = (r + i) / 2;
				QString const & mids
				= model.data(model.index(mid, 0), 
					Qt::EditRole).toString();
				size_t oldLen = s.length();
				size_t len = commonPrefix(mids, s);
				s = s.left(len);

				// left or right?
				if (oldLen == len) {
					// middle is not far enough
					i = mid + 1;
				} else {
					// middle is maybe too far
					r = mid;
				}
			} while (r - i > 0 && i < n);
		}
	}

	return from_utf8(fromqstr(s));
}


void GuiCompleter::popupActivated(const QString & completion)
{
	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	docstring prefix = cur.inset().completionPrefix(cur);
	docstring postfix = from_utf8(fromqstr(completion.mid(prefix.length())));
	cur.inset().insertCompletion(cur, postfix, true);
	hidePopup(cur);
	hideInline(cur);
	
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}


void GuiCompleter::popupHighlighted(const QString & completion)
{
	if (updateLock_ > 0)
		return;

	Cursor cur = gui_->bufferView().cursor();
	cur.updateFlags(Update::None);
	
	if (inlineVisible())
		updateInline(cur, completion);
	
	if (cur.disp_.update())
		gui_->bufferView().processUpdateFlags(cur.disp_.update());
}

} // namespace frontend
} // namespace lyx

#include "GuiCompleter_moc.cpp"
