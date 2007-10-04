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

#include "GuiApplication.h"
#include "GuiPainter.h"
#include "GuiKeySymbol.h"
#include "qt_helpers.h"

#include "frontends/LyXView.h"

#include "BufferView.h"
#include "Color.h"
#include "debug.h"
#include "FuncRequest.h"
#include "LyXRC.h"
#include "version.h"

#include "support/filetools.h" // LibFileSearch

#include "graphics/GraphicsImage.h"
#include "graphics/GraphicsLoader.h"

#include <QInputContext>
#include <QLayout>
#include <QMainWindow>
#include <QPainter>
#include <QScrollBar>
#include <QTimer>

#include <boost/bind.hpp>
#include <boost/current_function.hpp>

#ifdef Q_WS_X11
#include <QX11Info>
extern "C" int XEventsQueued(Display *display, int mode);
#endif

#ifdef Q_WS_WIN
int const CursorWidth = 2;
#else
int const CursorWidth = 1;
#endif

#undef KeyPress
#undef NoModifier 

using std::endl;
using std::string;

namespace lyx {

using support::FileName;

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
		if (show_ && rect_.isValid()) {
			switch (shape_) {
			case L_SHAPE:
				painter.fillRect(rect_.x(), rect_.y(), CursorWidth, rect_.height(), color_);
				painter.setPen(color_);
				painter.drawLine(rect_.bottomLeft().x() + CursorWidth, rect_.bottomLeft().y(),
												 rect_.bottomRight().x(), rect_.bottomLeft().y());
				break;
			
			case REVERSED_L_SHAPE:
				painter.fillRect(rect_.x() + rect_.height() / 3, rect_.y(), CursorWidth, rect_.height(), color_);
				painter.setPen(color_);
				painter.drawLine(rect_.bottomRight().x() - CursorWidth, rect_.bottomLeft().y(),
													 rect_.bottomLeft().x(), rect_.bottomLeft().y());
				break;
					
			default:
				painter.fillRect(rect_, color_);
				break;
			}
		}
	}

	void update(int x, int y, int h, CursorShape shape)
	{
		color_ = guiApp->colorCache().get(Color::cursor);
		shape_ = shape;
		switch (shape) {
		case L_SHAPE:
			rect_ = QRect(x, y, CursorWidth + h / 3, h);
			break;
		case REVERSED_L_SHAPE:
			rect_ = QRect(x - h / 3, y, CursorWidth + h / 3, h);
			break;
		default: 
			rect_ = QRect(x, y, CursorWidth, h);
			break;
		}
	}

	void show(bool set_show = true) { show_ = set_show; }
	void hide() { show_ = false; }

	QRect const & rect() { return rect_; }

private:
	///
	CursorShape shape_;
	///
	bool show_;
	///
	QColor color_;
	///
	QRect rect_;
};


// This is a 'heartbeat' generating synthetic mouse move events when the
// cursor is at the top or bottom edge of the viewport. One scroll per 0.2 s
SyntheticMouseEvent::SyntheticMouseEvent()
	: timeout(200), restart_timeout(true),
	  x_old(-1), y_old(-1), scrollbar_value_old(-1.0)
{}


GuiWorkArea::GuiWorkArea(Buffer & buf, LyXView & lv)
	: WorkArea(buf, lv), need_resize_(false), schedule_redraw_(false),
	  preedit_lines_(1)
{
	screen_ = QPixmap(viewport()->width(), viewport()->height());
	cursor_ = new frontend::CursorWidget();
	cursor_->hide();

	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setAcceptDrops(true);
	setMouseTracking(true);
	setMinimumSize(100, 70);

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
	QObject::connect(verticalScrollBar(), SIGNAL(actionTriggered(int)),
		this, SLOT(adjustViewWithScrollBar(int)));

	// disable context menu for the scrollbar
	verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

	// PageStep only depends on the viewport height.
	verticalScrollBar()->setPageStep(viewport()->height());

	LYXERR(Debug::GUI) << BOOST_CURRENT_FUNCTION
		<< "\n Area width\t" << width()
		<< "\n Area height\t" << height()
		<< "\n viewport width\t" << viewport()->width()
		<< "\n viewport height\t" << viewport()->height()
		<< endl;

	// Enables input methods for asian languages.
	// Must be set when creating custom text editing widgets.
	setAttribute(Qt::WA_InputMethodEnabled, true);
}


void GuiWorkArea::setScrollbarParams(int h, int scroll_pos, int scroll_line_step)
{
	if (verticalScrollBarPolicy() != Qt::ScrollBarAlwaysOn)
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	verticalScrollBar()->setTracking(false);

	// do what cursor movement does (some grey)
	h += height() / 4;
	int scroll_max_ = std::max(0, h - height());

	verticalScrollBar()->setRange(0, scroll_max_);
	verticalScrollBar()->setSliderPosition(scroll_pos);
	verticalScrollBar()->setSingleStep(scroll_line_step);
	verticalScrollBar()->setValue(scroll_pos);

	verticalScrollBar()->setTracking(true);
}


void GuiWorkArea::adjustViewWithScrollBar(int action)
{
	stopBlinkingCursor();
	if (action == QAbstractSlider::SliderPageStepAdd)
		buffer_view_->scrollDown(viewport()->height());
	else if (action == QAbstractSlider::SliderPageStepSub)
		buffer_view_->scrollUp(viewport()->height());
	else
		buffer_view_->scrollDocView(verticalScrollBar()->sliderPosition());

	if (lyxrc.cursor_follows_scrollbar) {
		buffer_view_->setCursorFromScrollbar();
		lyx_view_->updateLayoutChoice();
	}
	// Show the cursor immediately after any operation.
	startBlinkingCursor();
	QApplication::syncX();
}


void GuiWorkArea::focusInEvent(QFocusEvent * /*event*/)
{
	// Repaint the whole screen.
	// Note: this is different from redraw() as only the backing pixmap
	// will be redrawn, which is cheap.
	viewport()->repaint();

	startBlinkingCursor();
}


void GuiWorkArea::focusOutEvent(QFocusEvent * /*event*/)
{
	stopBlinkingCursor();
}


void GuiWorkArea::mousePressEvent(QMouseEvent * e)
{
	if (dc_event_.active && dc_event_ == *e) {
		dc_event_.active = false;
		FuncRequest cmd(LFUN_MOUSE_TRIPLE,
			e->x(), e->y(),
			q_button_state(e->button()));
		dispatch(cmd);
		return;
	}

	inputContext()->reset();

	FuncRequest const cmd(LFUN_MOUSE_PRESS, e->x(), e->y(),
		q_button_state(e->button()));
	dispatch(cmd, q_key_state(e->modifiers()));
}


void GuiWorkArea::mouseReleaseEvent(QMouseEvent * e)
{
	if (synthetic_mouse_event_.timeout.running())
		synthetic_mouse_event_.timeout.stop();

	FuncRequest const cmd(LFUN_MOUSE_RELEASE, e->x(), e->y(),
			      q_button_state(e->button()));
	dispatch(cmd);
}


void GuiWorkArea::mouseMoveEvent(QMouseEvent * e)
{
	// we kill the triple click if we move
	doubleClickTimeout();
	FuncRequest cmd(LFUN_MOUSE_MOTION, e->x(), e->y(),
			      q_motion_state(e->buttons()));

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
		else {
			synthetic_mouse_event_.restart_timeout = true;
			synthetic_mouse_event_.timeout.start();
			// Fall through to handle this event...
		}

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
	if (e->x() != synthetic_mouse_event_.x_old ||
	    e->y() != synthetic_mouse_event_.y_old ||
	    scrollbar_value != synthetic_mouse_event_.scrollbar_value_old) {
		// Yes it has. Store the params used to check this.
		synthetic_mouse_event_.x_old = e->x();
		synthetic_mouse_event_.y_old = e->y();
		synthetic_mouse_event_.scrollbar_value_old = scrollbar_value;

		// ... and dispatch the event to the LyX core.
		dispatch(cmd);
	}
}


void GuiWorkArea::wheelEvent(QWheelEvent * e)
{
	// Wheel rotation by one notch results in a delta() of 120 (see
	// documentation of QWheelEvent)
	int const lines = qApp->wheelScrollLines() * e->delta() / 120;
	verticalScrollBar()->setValue(verticalScrollBar()->value() -
			lines *  verticalScrollBar()->singleStep());
	adjustViewWithScrollBar();
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
	// do nothing if there are other events
	// (the auto repeated events come too fast)
	// \todo FIXME: remove hard coded Qt keys, process the key binding
#ifdef Q_WS_X11
	if (XEventsQueued(QX11Info::display(), 0) > 1 && ev->isAutoRepeat() 
			&& (Qt::Key_PageDown || Qt::Key_PageUp)) {
		LYXERR(Debug::KEY)	
			<< BOOST_CURRENT_FUNCTION << endl
			<< "system is busy: scroll key event ignored" << endl;
		ev->ignore();
		return;
	}
#endif

	LYXERR(Debug::KEY) << BOOST_CURRENT_FUNCTION
		<< " count=" << ev->count()
		<< " text=" << fromqstr(ev->text())
		<< " isAutoRepeat=" << ev->isAutoRepeat()
		<< " key=" << ev->key()
		<< endl;

	KeySymbol sym;
	setKeySymbol(&sym, ev);
	processKeySym(sym, q_key_state(ev->modifiers()));
}


void GuiWorkArea::doubleClickTimeout()
{
	dc_event_.active = false;
}


void GuiWorkArea::mouseDoubleClickEvent(QMouseEvent * ev)
{
	dc_event_ = double_click(ev);
	QTimer::singleShot(QApplication::doubleClickInterval(), this,
			   SLOT(doubleClickTimeout()));
	FuncRequest cmd(LFUN_MOUSE_DOUBLE,
			ev->x(), ev->y(),
			q_button_state(ev->button()));
	dispatch(cmd);
}


void GuiWorkArea::resizeEvent(QResizeEvent * ev)
{
	QAbstractScrollArea::resizeEvent(ev);
	need_resize_ = true;
}


void GuiWorkArea::update(int x, int y, int w, int h)
{
	viewport()->repaint(x, y, w, h);
}


void GuiWorkArea::paintEvent(QPaintEvent * ev)
{
	QRect const rc = ev->rect();
	/*
	LYXERR(Debug::PAINTING) << "paintEvent begin: x: " << rc.x()
		<< " y: " << rc.y()
		<< " w: " << rc.width()
		<< " h: " << rc.height() << endl;
	*/

	if (need_resize_) {
		verticalScrollBar()->setPageStep(viewport()->height());
		screen_ = QPixmap(viewport()->width(), viewport()->height());
		resizeBufferView();
		updateScreen();
		WorkArea::hideCursor();
		WorkArea::showCursor();
		need_resize_ = false;
	}

	QPainter pain(viewport());
	pain.drawPixmap(rc, screen_, rc);
	cursor_->draw(pain);
}


void GuiWorkArea::expose(int x, int y, int w, int h)
{
	updateScreen();
	update(x, y, w, h);
}


void GuiWorkArea::updateScreen()
{
	GuiPainter pain(&screen_);
	verticalScrollBar()->show();
	buffer_view_->draw(pain);
}


void GuiWorkArea::showCursor(int x, int y, int h, CursorShape shape)
{
	if (schedule_redraw_) {
		buffer_view_->update(Update::Force);
		updateScreen();
		viewport()->update(QRect(0, 0, viewport()->width(), viewport()->height()));
		schedule_redraw_ = false;
		// Show the cursor immediately after the update.
		hideCursor();
		toggleCursor();
		return;
	}

	cursor_->update(x, y, h, shape);
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

		LYXERR(Debug::KEY) << BOOST_CURRENT_FUNCTION
			<< " preeditString =" << fromqstr(e->preeditString())
			<< " commitString  =" << fromqstr(e->commitString())
			<< endl;

		int key = 0;

		// FIXME Iwami 04/01/07: we should take care also of UTF16 surrogates here.
		for (int i = 0; i < commit_string.size(); ++i) {
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
	buffer_view_->updateMetrics(false);
	buffer_view_->draw(pain);
	Font font = buffer_view_->cursor().getFont();
	FontMetrics const & fm = theFontMetrics(font);
	int height = fm.maxHeight();
	int cur_x = cursor_->rect().left();
	int cur_y = cursor_->rect().bottom();

	// redraw area of preedit string.
	update(0, cur_y - height, GuiWorkArea::width(),
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
	for (int i = 0; i < att.size(); ++i) {
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
		for (int i = 0; i < att.size(); ++i) {
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

	int const right_margin = rightMargin();
	Painter::preedit_style ps;
	// Most often there would be only one line:
	preedit_lines_ = 1;
	for (size_t pos = 0; pos != preedit_length; ++pos) {
		char_type const typed_char = preedit_string[pos];
		// reset preedit string style
		ps = Painter::preedit_default;

		// if we reached the right extremity of the screen, go to next line.
		if (cur_x + fm.width(typed_char) > GuiWorkArea::width() - right_margin) {
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
	update(0, cur_y - preedit_lines_*height, GuiWorkArea::width(),
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

} // namespace frontend
} // namespace lyx

#include "GuiWorkArea_moc.cpp"
