// -*- C++ -*-
/**
 * \file GuiWorkArea.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef WORKAREA_H
#define WORKAREA_H

#include "frontends/WorkArea.h"
#include "frontends/qt4/GuiCompleter.h"

#include "DocIterator.h"
#include "FuncRequest.h"
#include "qt_helpers.h"
#include "support/docstring.h"
#include "support/Timeout.h"

#include <QAbstractScrollArea>
#include <QMouseEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QTabWidget>
#include <QTimer>

class QContextMenuEvent;
class QDragEnterEvent;
class QDropEvent;
class QKeyEvent;
class QWheelEvent;
class QPaintEvent;
class QWidget;

#ifdef CursorShape
#undef CursorShape
#endif

namespace lyx {

class Buffer;

namespace frontend {

class GuiView;
class GuiWorkArea;

/// for emulating triple click
class DoubleClick {
public:
	///
	DoubleClick() : state(Qt::NoButton), active(false) {}
	///
	DoubleClick(QMouseEvent * e) : state(e->button()), active(true) {}
	///
	bool operator==(QMouseEvent const & e) { return state == e.button(); }
	///
public:
	///
	Qt::MouseButton state;
	///
	bool active;
};

/** Qt only emits mouse events when the mouse is being moved, but
 *  we want to generate 'pseudo' mouse events when the mouse button is
 *  pressed and the mouse cursor is below the bottom, or above the top
 *  of the work area. In this way, we'll be able to continue scrolling
 *  (and selecting) the text.
 *
 *  This class stores all the parameters needed to make this happen.
 */
class SyntheticMouseEvent
{
public:
	SyntheticMouseEvent();

	FuncRequest cmd;
	Timeout timeout;
	bool restart_timeout;
	int x_old;
	int y_old;
	double scrollbar_value_old;
};


/**
 * Implementation of the work area (buffer view GUI)
*/
class CursorWidget;

class GuiWorkArea : public QAbstractScrollArea, public WorkArea
{
	Q_OBJECT

public:
	///
	GuiWorkArea(Buffer & buffer, GuiView & lv);
	///
	~GuiWorkArea();

	///
	void setFullScreen(bool full_screen);
	/// is LyXView in fullscreen mode?
	bool isFullScreen();
	///
	void scheduleRedraw() { schedule_redraw_ = true; }
	///
	BufferView & bufferView();
	///
	BufferView const & bufferView() const;
	///
	void redraw();
	///
	void stopBlinkingCursor();
	///
	void startBlinkingCursor();
	/// Process Key pressed event.
	/// This needs to be public because it is accessed externally by GuiView.
	void processKeySym(KeySymbol const & key, KeyModifier mod);
	///
	void resizeBufferView();

	///
	GuiCompleter & completer() { return completer_; }
	
Q_SIGNALS:
	///
	void titleChanged(GuiWorkArea *);

private Q_SLOTS:
	/// Scroll the BufferView.
	/**
	  * This is a slot for the valueChanged() signal of the vertical scrollbar.
	  * \p value value of the scrollbar.
	*/
	void scrollTo(int value);
	/// timer to limit triple clicks
	void doubleClickTimeout();
	/// toggle the cursor's visibility
	void toggleCursor();
	/// close this work area.
	/// Slot for Buffer::closing signal.
	void close();

private:
	friend class GuiCompleter;

	/// update the passed area.
	void update(int x, int y, int w, int h);
	///
	void updateScreen();

	/// paint the cursor and store the background
	virtual void showCursor(int x, int y, int h,
		bool l_shape, bool rtl, bool completable);

	/// hide the cursor
	virtual void removeCursor();

	/// This function is called when the buffer readonly status change.
	void setReadOnly(bool);

	/// Update window titles of all users.
	void updateWindowTitle();
	///
	bool event(QEvent *);
	///
	void contextMenuEvent(QContextMenuEvent *);
	///
	void focusInEvent(QFocusEvent *);
	///
	void focusOutEvent(QFocusEvent *);
	/// repaint part of the widget
	void paintEvent(QPaintEvent * ev);
	/// widget has been resized
	void resizeEvent(QResizeEvent * ev);
	/// mouse button press
	void mousePressEvent(QMouseEvent * ev);
	/// mouse button release
	void mouseReleaseEvent(QMouseEvent * ev);
	/// mouse double click of button
	void mouseDoubleClickEvent(QMouseEvent * ev);
	/// mouse motion
	void mouseMoveEvent(QMouseEvent * ev);
	/// wheel event
	void wheelEvent(QWheelEvent * ev);
	/// key press
	void keyPressEvent(QKeyEvent * ev);
	/// IM events
	void inputMethodEvent(QInputMethodEvent * ev);
	/// IM query
	QVariant inputMethodQuery(Qt::InputMethodQuery query) const;

	/// The slot connected to SyntheticMouseEvent::timeout.
	void generateSyntheticMouseEvent();
	///
	void dispatch(FuncRequest const & cmd0, KeyModifier = NoModifier);
	/// hide the visible cursor, if it is visible
	void hideCursor();
	/// show the cursor if it is not visible
	void showCursor();
	///
	void updateScrollbar();

	///
	BufferView * buffer_view_;
	///
	GuiView * lyx_view_;
	/// is the cursor currently displayed
	bool cursor_visible_;

	///
	QTimer cursor_timeout_;
	///
	SyntheticMouseEvent synthetic_mouse_event_;
	///
	DoubleClick dc_event_;

	///
	CursorWidget * cursor_;
	///
	QPixmap screen_;
	///
	bool need_resize_;
	///
	bool schedule_redraw_;
	///
	int preedit_lines_;

	///
	GuiCompleter completer_;
}; // GuiWorkArea


/// A tabbed set of GuiWorkAreas.
class TabWorkArea : public QTabWidget
{
	Q_OBJECT
public:
	TabWorkArea(QWidget * parent = 0);

	///
	void setFullScreen(bool full_screen);
	void showBar(bool show);
	void closeAll();
	bool setCurrentWorkArea(GuiWorkArea *);
	GuiWorkArea * addWorkArea(Buffer & buffer, GuiView & view);
	bool removeWorkArea(GuiWorkArea *);
	GuiWorkArea * currentWorkArea();
	GuiWorkArea * workArea(Buffer & buffer);

Q_SIGNALS:
	///
	void currentWorkAreaChanged(GuiWorkArea *);
	///
	void lastWorkAreaRemoved();

public Q_SLOTS:
	///
	void on_currentTabChanged(int index);
	///
	void closeCurrentBuffer();
	///
	void closeCurrentTab();
	///
	void updateTabText(GuiWorkArea *);
}; // TabWorkArea

} // namespace frontend
} // namespace lyx

#endif // WORKAREA_H
