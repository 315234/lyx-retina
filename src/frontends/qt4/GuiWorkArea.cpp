/**
 * \file GuiWorkArea.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiWorkArea.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "CoordCache.h"
#include "Cursor.h"
#include "Font.h"
#include "FuncRequest.h"
#include "GuiApplication.h"
#include "GuiKeySymbol.h"
#include "GuiPainter.h"
#include "GuiPopupMenu.h"
#include "GuiView.h"
#include "KeySymbol.h"
#include "Language.h"
#include "LyXFunc.h"
#include "LyXRC.h"
#include "MetricsInfo.h"
#include "qt_helpers.h"
#include "version.h"

#include "graphics/GraphicsImage.h"
#include "graphics/GraphicsLoader.h"

#include "support/debug.h"
#include "support/gettext.h"
#include "support/FileName.h"

#include "frontends/Application.h"
#include "frontends/FontMetrics.h"
#include "frontends/WorkAreaManager.h"

#include <QContextMenuEvent>
#include <QInputContext>
#include <QHelpEvent>
#include <QMainWindow>
#include <QPainter>
#include <QPalette>
#include <QScrollBar>
#include <QTabBar>
#include <QTimer>
#include <QToolButton>
#include <QToolTip>

#include <boost/bind.hpp>

#ifdef Q_WS_X11
#include <QX11Info>
extern "C" int XEventsQueued(Display *display, int mode);
#endif

#ifdef Q_WS_WIN
int const CursorWidth = 2;
#else
int const CursorWidth = 1;
#endif
int const TabIndicatorWidth = 4;

#undef KeyPress
#undef NoModifier 

using namespace std;
using namespace lyx::support;

namespace lyx {


/// return the LyX mouse button state from Qt's
static mouse_button::state q_button_state(Qt::MouseButton button)
{
	mouse_button::state b = mouse_button::none;
	switch (button) {
		case Qt::LeftButton:
			b = mouse_button::button1;
			break;
		case Qt::MidButton:
			b = mouse_button::button2;
			break;
		case Qt::RightButton:
			b = mouse_button::button3;
			break;
		default:
			break;
	}
	return b;
}


/// return the LyX mouse button state from Qt's
mouse_button::state q_motion_state(Qt::MouseButtons state)
{
	mouse_button::state b = mouse_button::none;
	if (state & Qt::LeftButton)
		b |= mouse_button::button1;
	if (state & Qt::MidButton)
		b |= mouse_button::button2;
	if (state & Qt::RightButton)
		b |= mouse_button::button3;
	return b;
}


namespace frontend {

class CursorWidget {
public:
	CursorWidget() {}

	void draw(QPainter & painter)
	{
		if (!show_ || !rect_.isValid())
			return;
		
		int l = x_ - rect_.left();
		int r = rect_.right() - x_;
		int h = rect_.height();

		painter.fillRect(x_, y_, CursorWidth, h, color_);
		painter.setPen(color_);
		if (l_shape_) {
			if (rtl_)
				painter.drawLine(x_, y_ + h, x_ - l, y_ + h);
			else
				painter.drawLine(x_ + CursorWidth, y_ + h, 
					x_ + CursorWidth + r, y_ + h);
		}
		
		if (completable_) {
			int m = y_ + h / 2;
			int d = TabIndicatorWidth - 1;
			if (rtl_) {
				painter.drawLine(x_ - 1 , m - d, x_ - d - 1, m);
				painter.drawLine(x_ - 1 , m + d, x_ - d - 1, m);
			} else {
				painter.drawLine(x_ + CursorWidth , m - d, x_ + d + CursorWidth, m);
				painter.drawLine(x_ + CursorWidth , m + d, x_ + d + CursorWidth, m);
			}
		}
	}

	void update(int x, int y, int h, bool l_shape,
		bool rtl, bool completable)
	{
		color_ = guiApp->colorCache().get(Color_cursor);
		l_shape_ = l_shape;
		rtl_ = rtl;
		completable_ = completable;
		x_ = x;
		y_ = y;
		int l = 0;
		int r = 0;

		if (l_shape_) {
			if (rtl)
				l += h / 3;
			else
				r += h / 3;
		}
		
		if (completable_) {
			if (rtl)
				l = max(r, TabIndicatorWidth);
			else
				r = max(l, TabIndicatorWidth);
		}

		rect_ = QRect(x - l, y, CursorWidth + r + l, h);
	}

	void show(bool set_show = true) { show_ = set_show; }
	void hide() { show_ = false; }

	QRect const & rect() { return rect_; }

private:
	///
	bool rtl_;
	///
	bool l_shape_;
	///
	bool completable_;
	///
	bool show_;
	///
	QColor color_;
	///
	QRect rect_;
	///
	int x_;
	///
	int y_;
};


// This is a 'heartbeat' generating synthetic mouse move events when the
// cursor is at the top or bottom edge of the viewport. One scroll per 0.2 s
SyntheticMouseEvent::SyntheticMouseEvent()
	: timeout(200), restart_timeout(true),
	  x_old(-1), y_old(-1), scrollbar_value_old(-1.0)
{}



GuiWorkArea::GuiWorkArea(Buffer & buffer, GuiView & lv)
	: buffer_view_(new BufferView(buffer)), lyx_view_(&lv),
	cursor_visible_(false),
	need_resize_(false), schedule_redraw_(false),
	preedit_lines_(1), completer_(this)
{
	buffer.workAreaManager().add(this);
	// Setup the signals
	connect(&cursor_timeout_, SIGNAL(timeout()),
		this, SLOT(toggleCursor()));
	
	int const time = QApplication::cursorFlashTime() / 2;
	if (time > 0) {
		cursor_timeout_.setInterval(time);
		cursor_timeout_.start();
	} else
		// let's initialize this just to be safe
		cursor_timeout_.setInterval(500);

	screen_ = QPixmap(viewport()->width(), viewport()->height());
	cursor_ = new frontend::CursorWidget();
	cursor_->hide();

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setAcceptDrops(true);
	setMouseTracking(true);
	setMinimumSize(100, 70);
	updateWindowTitle();

	viewport()->setAutoFillBackground(false);
	// We don't need double-buffering nor SystemBackground on
	// the viewport because we have our own backing pixmap.
	viewport()->setAttribute(Qt::WA_NoSystemBackground);

	setFocusPolicy(Qt::WheelFocus);

	viewport()->setCursor(Qt::IBeamCursor);

	synthetic_mouse_event_.timeout.timeout.connect(
		boost::bind(&GuiWorkArea::generateSyntheticMouseEvent,
					this));

	// Initialize the vertical Scroll Bar
	QObject::connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
		this, SLOT(scrollTo(int)));

	LYXERR(Debug::GUI, "viewport width: " << viewport()->width()
		<< "  viewport height: " << viewport()->height());

	// Enables input methods for asian languages.
	// Must be set when creating custom text editing widgets.
	setAttribute(Qt::WA_InputMethodEnabled, true);
}


GuiWorkArea::~GuiWorkArea()
{
	buffer_view_->buffer().workAreaManager().remove(this);
	delete buffer_view_;
	delete cursor_;
}


void GuiWorkArea::close()
{
	lyx_view_->removeWorkArea(this);
}


void GuiWorkArea::setFullScreen(bool full_screen)
{
	buffer_view_->setFullScreen(full_screen);
	if (full_screen) {
		setFrameStyle(QFrame::NoFrame);
		if (lyxrc.full_screen_scrollbar)
			setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	} else {
		setFrameStyle(QFrame::Box);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	}
}


BufferView & GuiWorkArea::bufferView()
{
	return *buffer_view_;
}


BufferView const & GuiWorkArea::bufferView() const
{
	return *buffer_view_;
}


void GuiWorkArea::stopBlinkingCursor()
{
	cursor_timeout_.stop();
	hideCursor();
}


void GuiWorkArea::startBlinkingCursor()
{
	showCursor();
	//we're not supposed to cache this value.
	int const time = QApplication::cursorFlashTime() / 2;
	if (time <= 0)
		return;
	cursor_timeout_.setInterval(time);
	cursor_timeout_.start();
}


void GuiWorkArea::redraw()
{
	if (!isVisible())
		// No need to redraw in this case.
		return;

	// No need to do anything if this is the current view. The BufferView
	// metrics are already up to date.
	if (lyx_view_ != guiApp->currentView()
		|| lyx_view_->currentWorkArea() != this) {
		// FIXME: it would be nice to optimize for the off-screen case.
		buffer_view_->updateMetrics();
		buffer_view_->cursor().fixIfBroken();
	}

	// update cursor position, because otherwise it has to wait until
	// the blinking interval is over
	if (cursor_visible_) {
		hideCursor();
		showCursor();
	}
	
	LYXERR(Debug::WORKAREA, "WorkArea::redraw screen");
	updateScreen();
	update(0, 0, viewport()->width(), viewport()->height());

	/// \warning: scrollbar updating *must* be done after the BufferView is drawn
	/// because \c BufferView::updateScrollbar() is called in \c BufferView::draw().
	updateScrollbar();
	lyx_view_->updateStatusBar();

	if (lyxerr.debugging(Debug::WORKAREA))
		buffer_view_->coordCache().dump();
}


void GuiWorkArea::processKeySym(KeySymbol const & key, KeyModifier mod)
{
	// In order to avoid bad surprise in the middle of an operation,
	// we better stop the blinking cursor...
	// the cursor gets restarted in GuiView::restartCursor()
	stopBlinkingCursor();

	theLyXFunc().setLyXView(lyx_view_);
	theLyXFunc().processKeySym(key, mod);
	
}


void GuiWorkArea::dispatch(FuncRequest const & cmd0, KeyModifier mod)
{
	// Handle drag&drop
	if (cmd0.action == LFUN_FILE_OPEN) {
		lyx_view_->dispatch(cmd0);
		return;
	}

	theLyXFunc().setLyXView(lyx_view_);

	FuncRequest cmd;

	if (cmd0.action == LFUN_MOUSE_PRESS) {
		if (mod == ShiftModifier)
			cmd = FuncRequest(cmd0, "region-select");
		else if (mod == ControlModifier)
			cmd = FuncRequest(cmd0, "paragraph-select");
		else
			cmd = cmd0;
	}
	else
		cmd = cmd0;

	bool const notJustMovingTheMouse = 
		cmd.action != LFUN_MOUSE_MOTION || cmd.button() != mouse_button::none;
	
	// In order to avoid bad surprise in the middle of an operation, we better stop
	// the blinking cursor.
	if (notJustMovingTheMouse)
		stopBlinkingCursor();

	buffer_view_->mouseEventDispatch(cmd);

	// Skip these when selecting
	if (cmd.action != LFUN_MOUSE_MOTION) {
		completer_.updateVisibility(false, false);
		lyx_view_->updateLayoutList();
		lyx_view_->updateToolbars();
	}

	// GUI tweaks except with mouse motion with no button pressed.
	if (notJustMovingTheMouse) {
		// Slight hack: this is only called currently when we
		// clicked somewhere, so we force through the display
		// of the new status here.
		lyx_view_->clearMessage();

		// Show the cursor immediately after any operation
		startBlinkingCursor();
	}
}


void GuiWorkArea::resizeBufferView()
{
	// WARNING: Please don't put any code that will trigger a repaint here!
	// We are already inside a paint event.
	lyx_view_->setBusy(true);
	buffer_view_->resize(viewport()->width(), viewport()->height());
	updateScreen();

	// Update scrollbars which might have changed due different
	// BufferView dimension. This is especially important when the 
	// BufferView goes from zero-size to the real-size for the first time,
	// as the scrollbar paramters are then set for the first time.
	updateScrollbar();
	
	lyx_view_->updateLayoutList();
	lyx_view_->setBusy(false);
	need_resize_ = false;
}


void GuiWorkArea::showCursor()
{
	if (cursor_visible_)
		return;

	// RTL or not RTL
	bool l_shape = false;
	Font const & realfont = buffer_view_->cursor().real_current_font;
	BufferParams const & bp = buffer_view_->buffer().params();
	bool const samelang = realfont.language() == bp.language;
	bool const isrtl = realfont.isVisibleRightToLeft();

	if (!samelang || isrtl != bp.language->rightToLeft())
		l_shape = true;

	// The ERT language hack needs fixing up
	if (realfont.language() == latex_language)
		l_shape = false;

	Font const font = buffer_view_->cursor().getFont();
	FontMetrics const & fm = theFontMetrics(font);
	int const asc = fm.maxAscent();
	int const des = fm.maxDescent();
	int h = asc + des;
	int x = 0;
	int y = 0;
	Cursor & cur = buffer_view_->cursor();
	cur.getPos(x, y);
	y -= asc;

	// if it doesn't touch the screen, don't try to show it
	bool cursorInView = true;
	if (y + h < 0 || y >= viewport()->height())
		cursorInView = false;

	// show cursor on screen
	bool completable = completer_.completionAvailable();
	if (cursorInView) {
		cursor_visible_ = true;
		showCursor(x, y, h, l_shape, isrtl, completable);
	}
}


void GuiWorkArea::hideCursor()
{
	if (!cursor_visible_)
		return;

	cursor_visible_ = false;
	removeCursor();
}


void GuiWorkArea::toggleCursor()
{
	if (cursor_visible_)
		hideCursor();
	else
		showCursor();
}


void GuiWorkArea::updateScrollbar()
{
	ScrollbarParameters const & scroll_ = buffer_view_->scrollbarParameters();

	verticalScrollBar()->setRange(scroll_.min, scroll_.max);
	verticalScrollBar()->setPageStep(scroll_.page_step);
	verticalScrollBar()->setSingleStep(scroll_.single_step);
	// Block the scrollbar signal to prevent recursive signal/slot calling.
	verticalScrollBar()->blockSignals(true);
	verticalScrollBar()->setValue(scroll_.position);
	verticalScrollBar()->setSliderPosition(scroll_.position);
	verticalScrollBar()->blockSignals(false);
}


void GuiWorkArea::scrollTo(int value)
{
	stopBlinkingCursor();
	buffer_view_->scrollDocView(value);

	if (lyxrc.cursor_follows_scrollbar) {
		buffer_view_->setCursorFromScrollbar();
		lyx_view_->updateLayoutList();
	}
	// Show the cursor immediately after any operation.
	startBlinkingCursor();
	QApplication::syncX();
}


bool GuiWorkArea::event(QEvent * e)
{
	switch (e->type()) {
	case QEvent::ToolTip: {
		QHelpEvent * helpEvent = static_cast<QHelpEvent *>(e);
		if (lyxrc.use_tooltip) {
			QPoint pos = helpEvent->pos();
			if (pos.x() < viewport()->width()) {
				QString s = toqstr(buffer_view_->toolTip(pos.x(), pos.y()));
				QToolTip::showText(helpEvent->globalPos(), s);
			}
			else
				QToolTip::hideText();
		}
		// Don't forget to accept the event!
		e->accept();
		return true;
	}

	case QEvent::ShortcutOverride: {
		// We catch this event in order to catch the Tab or Shift+Tab key press
		// which are otherwise reserved to focus switching between controls
		// within a dialog.
		QKeyEvent * ke = static_cast<QKeyEvent*>(e);
		if ((ke->key() != Qt::Key_Tab && ke->key() != Qt::Key_Backtab)
			|| ke->modifiers() & Qt::ControlModifier)
			return QAbstractScrollArea::event(e);
		keyPressEvent(ke);
		return true;
	}

	default:
		return QAbstractScrollArea::event(e);
	}
	return false;
}


void GuiWorkArea::contextMenuEvent(QContextMenuEvent * e)
{
	QPoint pos = e->pos();
	docstring name = buffer_view_->contextMenu(pos.x(), pos.y());
	if (name.empty()) {
		QAbstractScrollArea::contextMenuEvent(e);
		return;
	}
	QMenu * menu = guiApp->menus().menu(toqstr(name));
	if (!menu) {
		QAbstractScrollArea::contextMenuEvent(e);
		return;
	}
	// Position the menu to the right.
	// FIXME: menu position should be different for RTL text.
	menu->exec(e->globalPos());
	e->accept();
}


void GuiWorkArea::focusInEvent(QFocusEvent * e)
{
	lyx_view_->setCurrentWorkArea(this);
	// Repaint the whole screen.
	// Note: this is different from redraw() as only the backing pixmap
	// will be redrawn, which is cheap.
	viewport()->repaint();

	startBlinkingCursor();
	QAbstractScrollArea::focusInEvent(e);
}


void GuiWorkArea::focusOutEvent(QFocusEvent * e)
{
	stopBlinkingCursor();
	QAbstractScrollArea::focusOutEvent(e);
}


void GuiWorkArea::mousePressEvent(QMouseEvent * e)
{
	if (dc_event_.active && dc_event_ == *e) {
		dc_event_.active = false;
		FuncRequest cmd(LFUN_MOUSE_TRIPLE, e->x(), e->y(),
			q_button_state(e->button()));
		dispatch(cmd);
		e->accept();
		return;
	}

	inputContext()->reset();

	FuncRequest const cmd(LFUN_MOUSE_PRESS, e->x(), e->y(),
		q_button_state(e->button()));
	dispatch(cmd, q_key_state(e->modifiers()));
	e->accept();
}


void GuiWorkArea::mouseReleaseEvent(QMouseEvent * e)
{
	if (synthetic_mouse_event_.timeout.running())
		synthetic_mouse_event_.timeout.stop();

	FuncRequest const cmd(LFUN_MOUSE_RELEASE, e->x(), e->y(),
			      q_button_state(e->button()));
	dispatch(cmd);
	e->accept();
}


void GuiWorkArea::mouseMoveEvent(QMouseEvent * e)
{
	// we kill the triple click if we move
	doubleClickTimeout();
	FuncRequest cmd(LFUN_MOUSE_MOTION, e->x(), e->y(),
		q_motion_state(e->buttons()));

	e->accept();

	// If we're above or below the work area...
	if (e->y() <= 20 || e->y() >= viewport()->height() - 20) {
		// Make sure only a synthetic event can cause a page scroll,
		// so they come at a steady rate:
		if (e->y() <= 20)
			// _Force_ a scroll up:
			cmd.y = -40;
		else
			cmd.y = viewport()->height();
		// Store the event, to be handled when the timeout expires.
		synthetic_mouse_event_.cmd = cmd;

		if (synthetic_mouse_event_.timeout.running())
			// Discard the event. Note that it _may_ be handled
			// when the timeout expires if
			// synthetic_mouse_event_.cmd has not been overwritten.
			// Ie, when the timeout expires, we handle the
			// most recent event but discard all others that
			// occurred after the one used to start the timeout
			// in the first place.
			return;

		synthetic_mouse_event_.restart_timeout = true;
		synthetic_mouse_event_.timeout.start();
		// Fall through to handle this event...

	} else if (synthetic_mouse_event_.timeout.running()) {
		// Store the event, to be possibly handled when the timeout
		// expires.
		// Once the timeout has expired, normal control is returned
		// to mouseMoveEvent (restart_timeout = false).
		// This results in a much smoother 'feel' when moving the
		// mouse back into the work area.
		synthetic_mouse_event_.cmd = cmd;
		synthetic_mouse_event_.restart_timeout = false;
		return;
	}

	// Has anything changed on-screen since the last QMouseEvent
	// was received?
	double const scrollbar_value = verticalScrollBar()->value();
	if (e->x() == synthetic_mouse_event_.x_old
		&& e->y() == synthetic_mouse_event_.y_old
		&& scrollbar_value == synthetic_mouse_event_.scrollbar_value_old) {
		// Nothing changed on-screen since the last QMouseEvent.
		return;
	}

	// Yes something has changed. Store the params used to check this.
	synthetic_mouse_event_.x_old = e->x();
	synthetic_mouse_event_.y_old = e->y();
	synthetic_mouse_event_.scrollbar_value_old = scrollbar_value;

	// ... and dispatch the event to the LyX core.
	dispatch(cmd);
}


void GuiWorkArea::wheelEvent(QWheelEvent * e)
{
	// Wheel rotation by one notch results in a delta() of 120 (see
	// documentation of QWheelEvent)
	double const lines = qApp->wheelScrollLines()
		* lyxrc.mouse_wheel_speed
		* e->delta() / 120.0;
	LYXERR(Debug::SCROLLING, "wheelScrollLines = " << qApp->wheelScrollLines()
		<< " delta = " << e->delta()
		<< " lines = " << lines);
	verticalScrollBar()->setValue(verticalScrollBar()->value() -
		int(lines *  verticalScrollBar()->singleStep()));
	e->accept();
}


void GuiWorkArea::generateSyntheticMouseEvent()
{
	// Set things off to generate the _next_ 'pseudo' event.
	if (synthetic_mouse_event_.restart_timeout)
		synthetic_mouse_event_.timeout.start();

	// Has anything changed on-screen since the last timeout signal
	// was received?
	double const scrollbar_value = verticalScrollBar()->value();
	if (scrollbar_value != synthetic_mouse_event_.scrollbar_value_old) {
		// Yes it has. Store the params used to check this.
		synthetic_mouse_event_.scrollbar_value_old = scrollbar_value;

		// ... and dispatch the event to the LyX core.
		dispatch(synthetic_mouse_event_.cmd);
	}
}


void GuiWorkArea::keyPressEvent(QKeyEvent * ev)
{
	// intercept some keys if completion popup is visible
	if (completer_.popupVisible()) {
		switch (ev->key()) {
		case Qt::Key_Enter:
		case Qt::Key_Return:
			completer_.activate();
			ev->accept();
			return;
		}
	}
	
	// intercept keys for the completion
	if (ev->key() == Qt::Key_Tab) {
		completer_.tab();
		ev->accept();
		return;
	} 

	if (completer_.popupVisible() && ev->key() == Qt::Key_Escape) {
		completer_.hidePopup();
		ev->accept();
		return;
	}

	if (completer_.inlineVisible() && ev->key() == Qt::Key_Escape) {
		completer_.hideInline();
		ev->accept();
		return;
	}

	// do nothing if there are other events
	// (the auto repeated events come too fast)
	// \todo FIXME: remove hard coded Qt keys, process the key binding
#ifdef Q_WS_X11
	if (XEventsQueued(QX11Info::display(), 0) > 1 && ev->isAutoRepeat() 
			&& (Qt::Key_PageDown || Qt::Key_PageUp)) {
		LYXERR(Debug::KEY, "system is busy: scroll key event ignored");
		ev->ignore();
		return;
	}
#endif

	LYXERR(Debug::KEY, " count: " << ev->count()
		<< " text: " << fromqstr(ev->text())
		<< " isAutoRepeat: " << ev->isAutoRepeat() << " key: " << ev->key());

	KeySymbol sym;
	setKeySymbol(&sym, ev);
	processKeySym(sym, q_key_state(ev->modifiers()));
	ev->accept();
}


void GuiWorkArea::doubleClickTimeout()
{
	dc_event_.active = false;
}


void GuiWorkArea::mouseDoubleClickEvent(QMouseEvent * ev)
{
	dc_event_ = DoubleClick(ev);
	QTimer::singleShot(QApplication::doubleClickInterval(), this,
			   SLOT(doubleClickTimeout()));
	FuncRequest cmd(LFUN_MOUSE_DOUBLE,
			ev->x(), ev->y(),
			q_button_state(ev->button()));
	dispatch(cmd);
	ev->accept();
}


void GuiWorkArea::resizeEvent(QResizeEvent * ev)
{
	QAbstractScrollArea::resizeEvent(ev);
	need_resize_ = true;
	ev->accept();
}


void GuiWorkArea::update(int x, int y, int w, int h)
{
	viewport()->repaint(x, y, w, h);
}


void GuiWorkArea::paintEvent(QPaintEvent * ev)
{
	QRect const rc = ev->rect();
	// LYXERR(Debug::PAINTING, "paintEvent begin: x: " << rc.x()
	//	<< " y: " << rc.y() << " w: " << rc.width() << " h: " << rc.height());

	if (need_resize_) {
		screen_ = QPixmap(viewport()->width(), viewport()->height());
		resizeBufferView();
		hideCursor();
		showCursor();
	}

	QPainter pain(viewport());
	pain.drawPixmap(rc, screen_, rc);
	cursor_->draw(pain);
	ev->accept();
}


void GuiWorkArea::updateScreen()
{
	GuiPainter pain(&screen_);
	buffer_view_->draw(pain);
}


void GuiWorkArea::showCursor(int x, int y, int h,
	bool l_shape, bool rtl, bool completable)
{
	if (schedule_redraw_) {
		buffer_view_->updateMetrics();
		updateScreen();
		viewport()->update(QRect(0, 0, viewport()->width(), viewport()->height()));
		schedule_redraw_ = false;
		// Show the cursor immediately after the update.
		hideCursor();
		toggleCursor();
		return;
	}

	cursor_->update(x, y, h, l_shape, rtl, completable);
	cursor_->show();
	viewport()->update(cursor_->rect());
}


void GuiWorkArea::removeCursor()
{
	cursor_->hide();
	//if (!qApp->focusWidget())
		viewport()->update(cursor_->rect());
}


void GuiWorkArea::inputMethodEvent(QInputMethodEvent * e)
{
	QString const & commit_string = e->commitString();
	docstring const & preedit_string
		= qstring_to_ucs4(e->preeditString());

	if (!commit_string.isEmpty()) {

		LYXERR(Debug::KEY, "preeditString: " << fromqstr(e->preeditString())
			<< " commitString: " << fromqstr(e->commitString()));

		int key = 0;

		// FIXME Iwami 04/01/07: we should take care also of UTF16 surrogates here.
		for (int i = 0; i != commit_string.size(); ++i) {
			QKeyEvent ev(QEvent::KeyPress, key, Qt::NoModifier, commit_string[i]);
			keyPressEvent(&ev);
		}
	}

	// Hide the cursor during the kana-kanji transformation.
	if (preedit_string.empty())
		startBlinkingCursor();
	else
		stopBlinkingCursor();

	// last_width : for checking if last preedit string was/wasn't empty.
	static bool last_width = false;
	if (!last_width && preedit_string.empty()) {
		// if last_width is last length of preedit string.
		e->accept();
		return;
	}

	GuiPainter pain(&screen_);
	buffer_view_->updateMetrics();
	buffer_view_->draw(pain);
	FontInfo font = buffer_view_->cursor().getFont().fontInfo();
	FontMetrics const & fm = theFontMetrics(font);
	int height = fm.maxHeight();
	int cur_x = cursor_->rect().left();
	int cur_y = cursor_->rect().bottom();

	// redraw area of preedit string.
	update(0, cur_y - height, viewport()->width(),
		(height + 1) * preedit_lines_);

	if (preedit_string.empty()) {
		last_width = false;
		preedit_lines_ = 1;
		e->accept();
		return;
	}
	last_width = true;

	// att : stores an IM attribute.
	QList<QInputMethodEvent::Attribute> const & att = e->attributes();

	// get attributes of input method cursor.
	// cursor_pos : cursor position in preedit string.
	size_t cursor_pos = 0;
	bool cursor_is_visible = false;
	for (int i = 0; i != att.size(); ++i) {
		if (att.at(i).type == QInputMethodEvent::Cursor) {
			cursor_pos = att.at(i).start;
			cursor_is_visible = att.at(i).length != 0;
			break;
		}
	}

	size_t preedit_length = preedit_string.length();

	// get position of selection in input method.
	// FIXME: isn't there a way to do this simplier?
	// rStart : cursor position in selected string in IM.
	size_t rStart = 0;
	// rLength : selected string length in IM.
	size_t rLength = 0;
	if (cursor_pos < preedit_length) {
		for (int i = 0; i != att.size(); ++i) {
			if (att.at(i).type == QInputMethodEvent::TextFormat) {
				if (att.at(i).start <= int(cursor_pos)
					&& int(cursor_pos) < att.at(i).start + att.at(i).length) {
						rStart = att.at(i).start;
						rLength = att.at(i).length;
						if (!cursor_is_visible)
							cursor_pos += rLength;
						break;
				}
			}
		}
	}
	else {
		rStart = cursor_pos;
		rLength = 0;
	}

	int const right_margin = buffer_view_->rightMargin();
	Painter::preedit_style ps;
	// Most often there would be only one line:
	preedit_lines_ = 1;
	for (size_t pos = 0; pos != preedit_length; ++pos) {
		char_type const typed_char = preedit_string[pos];
		// reset preedit string style
		ps = Painter::preedit_default;

		// if we reached the right extremity of the screen, go to next line.
		if (cur_x + fm.width(typed_char) > viewport()->width() - right_margin) {
			cur_x = right_margin;
			cur_y += height + 1;
			++preedit_lines_;
		}
		// preedit strings are displayed with dashed underline
		// and partial strings are displayed white on black indicating
		// that we are in selecting mode in the input method.
		// FIXME: rLength == preedit_length is not a changing condition
		// FIXME: should be put out of the loop.
		if (pos >= rStart
			&& pos < rStart + rLength
			&& !(cursor_pos < rLength && rLength == preedit_length))
			ps = Painter::preedit_selecting;

		if (pos == cursor_pos
			&& (cursor_pos < rLength && rLength == preedit_length))
			ps = Painter::preedit_cursor;

		// draw one character and update cur_x.
		cur_x += pain.preeditText(cur_x, cur_y, typed_char, font, ps);
	}

	// update the preedit string screen area.
	update(0, cur_y - preedit_lines_*height, viewport()->width(),
		(height + 1) * preedit_lines_);

	// Don't forget to accept the event!
	e->accept();
}


QVariant GuiWorkArea::inputMethodQuery(Qt::InputMethodQuery query) const
{
	QRect cur_r(0,0,0,0);
	switch (query) {
		// this is the CJK-specific composition window position.
		case Qt::ImMicroFocus:
			cur_r = cursor_->rect();
			if (preedit_lines_ != 1)
				cur_r.moveLeft(10);
			cur_r.moveBottom(cur_r.bottom() + cur_r.height() * preedit_lines_);
			// return lower right of cursor in LyX.
			return cur_r;
		default:
			return QWidget::inputMethodQuery(query);
	}
}


void GuiWorkArea::updateWindowTitle()
{
	docstring maximize_title;
	docstring minimize_title;

	Buffer & buf = buffer_view_->buffer();
	FileName const fileName = buf.fileName();
	if (!fileName.empty()) {
		maximize_title = fileName.displayName(30);
		minimize_title = from_utf8(fileName.onlyFileName());
		if (!buf.isClean()) {
			maximize_title += _(" (changed)");
			minimize_title += char_type('*');
		}
		if (buf.isReadonly())
			maximize_title += _(" (read only)");
	}

	QString title = windowTitle();
	QString new_title = toqstr(maximize_title);
	if (title == new_title)
		return;

	QWidget::setWindowTitle(new_title);
	QWidget::setWindowIconText(toqstr(minimize_title));
	titleChanged(this);
}


void GuiWorkArea::setReadOnly(bool)
{
	updateWindowTitle();
	if (this == lyx_view_->currentWorkArea())
		lyx_view_->updateBufferDependent(false);
}


bool GuiWorkArea::isFullScreen()
{
	return lyx_view_ && lyx_view_->isFullScreen();
}


////////////////////////////////////////////////////////////////////
//
// TabWorkArea 
//
////////////////////////////////////////////////////////////////////

TabWorkArea::TabWorkArea(QWidget * parent) : QTabWidget(parent)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Active, QPalette::Button,
		pal.color(QPalette::Active, QPalette::Window));
	pal.setColor(QPalette::Disabled, QPalette::Button,
		pal.color(QPalette::Disabled, QPalette::Window));
	pal.setColor(QPalette::Inactive, QPalette::Button,
		pal.color(QPalette::Inactive, QPalette::Window));

	QObject::connect(this, SIGNAL(currentChanged(int)),
		this, SLOT(on_currentTabChanged(int)));

	QToolButton * closeBufferButton = new QToolButton(this);
    closeBufferButton->setPalette(pal);
	// FIXME: rename the icon to closebuffer.png
	closeBufferButton->setIcon(QIcon(":/images/closetab.png"));
	closeBufferButton->setText("Close File");
	closeBufferButton->setAutoRaise(true);
	closeBufferButton->setCursor(Qt::ArrowCursor);
	closeBufferButton->setToolTip(qt_("Close File"));
	closeBufferButton->setEnabled(true);
	QObject::connect(closeBufferButton, SIGNAL(clicked()),
		this, SLOT(closeCurrentBuffer()));
	setCornerWidget(closeBufferButton, Qt::TopRightCorner);

	QToolButton * closeTabButton = new QToolButton(this);
    closeTabButton->setPalette(pal);
	closeTabButton->setIcon(QIcon(":/images/hidetab.png"));
	closeTabButton->setText("Hide tab");
	closeTabButton->setAutoRaise(true);
	closeTabButton->setCursor(Qt::ArrowCursor);
	closeTabButton->setToolTip(qt_("Hide tab"));
	closeTabButton->setEnabled(true);
	QObject::connect(closeTabButton, SIGNAL(clicked()),
		this, SLOT(closeCurrentTab()));
	setCornerWidget(closeTabButton, Qt::TopLeftCorner);

	setUsesScrollButtons(true);
}


void TabWorkArea::setFullScreen(bool full_screen)
{
	for (int i = 0; i != count(); ++i) {
		if (GuiWorkArea * wa = dynamic_cast<GuiWorkArea *>(widget(i)))
			wa->setFullScreen(full_screen);
	}

	if (lyxrc.full_screen_tabbar)
		showBar(!full_screen && count()>1);
}


void TabWorkArea::showBar(bool show)
{
	tabBar()->setEnabled(show);
	tabBar()->setVisible(show);
}


GuiWorkArea * TabWorkArea::currentWorkArea()
{
	if (count() == 0)
		return 0;

	GuiWorkArea * wa = dynamic_cast<GuiWorkArea *>(currentWidget()); 
	BOOST_ASSERT(wa);
	return wa;
}


GuiWorkArea * TabWorkArea::workArea(Buffer & buffer)
{
	for (int i = 0; i != count(); ++i) {
		GuiWorkArea * wa = dynamic_cast<GuiWorkArea *>(widget(i));
		BOOST_ASSERT(wa);
		if (&wa->bufferView().buffer() == &buffer)
			return wa;
	}
	return 0;
}


void TabWorkArea::closeAll()
{
	while (count()) {
		GuiWorkArea * wa = dynamic_cast<GuiWorkArea *>(widget(0));
		BOOST_ASSERT(wa);
		removeTab(0);
		delete wa;
	}
}


bool TabWorkArea::setCurrentWorkArea(GuiWorkArea * work_area)
{
	BOOST_ASSERT(work_area);
	int index = indexOf(work_area);
	if (index == -1)
		return false;

	if (index == currentIndex())
		// Make sure the work area is up to date.
		on_currentTabChanged(index);
	else
		// Switch to the work area.
		setCurrentIndex(index);
	work_area->setFocus();

	return true;
}


GuiWorkArea * TabWorkArea::addWorkArea(Buffer & buffer, GuiView & view)
{
	GuiWorkArea * wa = new GuiWorkArea(buffer, view);
	wa->setUpdatesEnabled(false);
	// Hide tabbar if there's no tab (avoid a resize and a flashing tabbar
	// when hiding it again below).
	if (!(currentWorkArea() && currentWorkArea()->isFullScreen()))
		showBar(count() > 0);
	addTab(wa, wa->windowTitle());
	QObject::connect(wa, SIGNAL(titleChanged(GuiWorkArea *)),
		this, SLOT(updateTabText(GuiWorkArea *)));
	if (currentWorkArea() && currentWorkArea()->isFullScreen())
		setFullScreen(true);
	else
		// Hide tabbar if there's only one tab.
		showBar(count() > 1);

	return wa;
}


bool TabWorkArea::removeWorkArea(GuiWorkArea * work_area)
{
	BOOST_ASSERT(work_area);
	int index = indexOf(work_area);
	if (index == -1)
		return false;

	work_area->setUpdatesEnabled(false);
	removeTab(index);
	delete work_area;

	if (count()) {
		// make sure the next work area is enabled.
		currentWidget()->setUpdatesEnabled(true);
		if ((currentWorkArea() && currentWorkArea()->isFullScreen()))
			setFullScreen(true);
		else
			// Hide tabbar if there's only one tab.
			showBar(count() > 1);
	}
	return true;
}


void TabWorkArea::on_currentTabChanged(int i)
{
	GuiWorkArea * wa = dynamic_cast<GuiWorkArea *>(widget(i));
	BOOST_ASSERT(wa);
	BufferView & bv = wa->bufferView();
	bv.cursor().fixIfBroken();
	bv.updateMetrics();
	wa->setUpdatesEnabled(true);
	wa->redraw();
	wa->setFocus();
	///
	currentWorkAreaChanged(wa);

	LYXERR(Debug::GUI, "currentTabChanged " << i
		<< "File" << bv.buffer().absFileName());
}


void TabWorkArea::closeCurrentBuffer()
{
	lyx::dispatch(FuncRequest(LFUN_BUFFER_CLOSE));
}


void TabWorkArea::closeCurrentTab()
{
	removeWorkArea(currentWorkArea());
}


void TabWorkArea::updateTabText(GuiWorkArea * wa)
{
	int const i = indexOf(wa);
	if (i < 0)
		return;
	setTabText(i, wa->windowTitle());
}

} // namespace frontend
} // namespace lyx

#include "GuiWorkArea_moc.cpp"
