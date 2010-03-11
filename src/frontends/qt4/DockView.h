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

#include "Dialog.h"
#include "GuiView.h"

#include <QDockWidget>

namespace lyx {
namespace frontend {

/// Dock Widget container for LyX dialogs.
/**
 * This template class that encapsulates a given Widget inside a
 * QDockWidget and presents a Dialog interface
 * FIXME: create a DockView.cpp file
 **/
class DockView : public QDockWidget, public Dialog
{
public:
	DockView(
		GuiView & parent, ///< the main window where to dock.
		QString const & name, ///< dialog identifier.
		QString const & title, ///< dialog title.
		Qt::DockWidgetArea area = Qt::LeftDockWidgetArea, ///< Position of the dock (and also drawer)
		Qt::WindowFlags flags = 0
	)
		: QDockWidget(&parent, flags), Dialog(parent, name, title)
	{
		setObjectName(name);
		parent.addDockWidget(area, this);
		hide();
	}

	virtual ~DockView() {}

	virtual QWidget * asQWidget() { return this; }
	virtual QWidget const * asQWidget() const { return this; }

	/// We don't want to restore geometry session for dock widgets.
	void restoreSession() {}

	/// Dialog inherited methods
	//@{
	void applyView() {}
	bool isClosing() const { return false; }
	//@}
};

} // frontend
} // lyx

#endif // DOCK_VIEW_H
