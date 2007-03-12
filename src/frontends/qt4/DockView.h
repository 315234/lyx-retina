// -*- C++ -*-
/**
 * \file DockView.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef DOCK_VIEW_H
#define DOCK_VIEW_H

#include "controllers/dialog.h"

#include <boost/scoped_ptr.hpp>

#include <QDockWidget>
#include <QMainWindow>

namespace lyx {
namespace frontend {

/// Dock Widget container for LyX dialogs.
/// This template class that encapsulates a given Widget inside a
/// DockWidget and presents a Dialog::View interface
template<class Controller, class Widget>
class DockView : public QDockWidget, public Dialog::View 
{
public:
	DockView(
		Dialog & dialog, ///< The (one) parent Dialog class.
		Controller * form, ///< Associated model/controller
		QMainWindow * parent, ///< the main window where to dock.
		docstring const & title ///< Window title (shown in the top title bar).
		)
		: QDockWidget(toqstr(title), parent), Dialog::View(dialog, title)
	{
		widget_.reset(new Widget(form));
		setWidget(widget_.get());
		parent->addDockWidget(Qt::LeftDockWidgetArea, this);
	}

	/// Dialog::View inherited methods
	//@{
	void apply() {}
	void hide()	{ QDockWidget::hide(); }
	void show()	{ QDockWidget::show(); }
	bool isVisible() const	
	{ return QDockWidget::isVisible(); }
	void redraw() {}
	void update()
	{
		widget_->update();
		QDockWidget::update();
	}
	//@}
private:
	/// The encapsulated widget.
	boost::scoped_ptr<Widget> widget_;
};

} // frontend
} // lyx

#endif // TOC_WIDGET_H
