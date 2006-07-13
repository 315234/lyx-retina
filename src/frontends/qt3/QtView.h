// -*- C++ -*-
/**
 * \file QtView.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjornes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QTVIEW_H
#define QTVIEW_H

#include <config.h>

#include "frontends/LyXView.h"

#include <qmainwindow.h>
#include <qtimer.h>

class FuncRequest;

namespace lyx {
namespace frontend {

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
	QtView(Gui & owner);

	~QtView();

	/// initialise the object members (menubars, toolbars, etc..)
	void init();

	/// show - display the top-level window
	void show();

	/// show busy cursor
	virtual void busy(bool) const;

	/// display a status message
	virtual void message(std::string const & str);

	/// clear status message
	virtual void clearMessage();

	/// add the command buffer
	void addCommandBuffer(QWidget * parent);

	/// menu item has been selected
	void activated(FuncRequest const &);

	// returns true if this view has the focus.
	virtual bool hasFocus() const;

	///
	void initFloatingGeometry(QRect const &);

public slots:
	/// idle timeout
	void update_view_state_qt();

	///
	virtual void resizeEvent(QResizeEvent * e);

	///
	virtual void moveEvent(QMoveEvent * e);

protected:
	/// make sure we quit cleanly
	virtual void closeEvent(QCloseEvent * e);
	/// update status bar
	void updateStatusBar();
private:
	/// focus the command buffer widget
	void focus_command_widget();

	/**
	 * setWindowTitle - set title of window
	 * @param t main window title
	 * @param it iconified (short) title
	 */
	virtual void setWindowTitle(std::string const & t, std::string const & it);

	QTimer statusbar_timer_;

	/// command buffer
	QCommandBuffer * commandbuffer_;
	
	///
	void updateFloatingGeometry();
	///
	QRect floatingGeometry_;
	///
	int maxWidth;
};

} // namespace frontend
} // namespace lyx

#endif // QTVIEW_H
