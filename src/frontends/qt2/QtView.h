// -*- C++ -*-
/**
 * \file QtView.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjornes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef QTVIEW_H
#define QTVIEW_H


#include "frontends/LyXView.h"

#include <qmainwindow.h>
#include <qtimer.h>

class QCommandBuffer;

/**
 * QtView - Qt implementation of LyXView
 *
 * Qt-private implementation of the main LyX window.
 */
class QtView : public QMainWindow, public LyXView {
	Q_OBJECT
public:
	/// create a main window of the given dimensions
	QtView(unsigned int w, unsigned int h);

	~QtView();

	/// show - display the top-level window
	void show();

	/// show busy cursor
	virtual void busy(bool) const;

	/// display a status message
	virtual void message(string const & str);
public slots:
	/// menu item has been selected
	void activated(int id);

	/// idle timeout
	void update_view_state_qt();
protected:
	/// make sure we quit cleanly
	virtual void closeEvent(QCloseEvent * e);
private:
	/// focus the command buffer widget
	void focus_command_widget();

	/// update status bar
	void update_view_state();

	/**
	 * setWindowTitle - set title of window
	 * @param t main window title
	 * @param it iconified (short) title
	 */
	virtual void setWindowTitle(string const & t, string const & it);

	/// idle timer
	QTimer idle_timer_;

	/// command buffer
	QCommandBuffer * commandbuffer_;
};

#endif // QTVIEW_H
