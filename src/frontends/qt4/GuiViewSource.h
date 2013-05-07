// -*- C++ -*-
/**
 * \file GuiViewSource.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Bo Peng
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIVIEWSOURCE_H
#define GUIVIEWSOURCE_H

#include "ui_ViewSourceUi.h"

#include "DockView.h"
#include "GuiView.h"
#include "qt_helpers.h"

#include <QDockWidget>
#include <QString>

class QTextDocument;

namespace lyx {
namespace frontend {

class GuiViewSource;
class LaTeXHighlighter;

class ViewSourceWidget : public QWidget, public Ui::ViewSourceUi
{
	Q_OBJECT

public:
	ViewSourceWidget();
	///
	void setBufferView(BufferView const * bv);

protected:
	///
	void resizeEvent (QResizeEvent * event);

public Q_SLOTS:
	/// update content
	void updateView();
	///
	void setViewFormat();
	///
	void updateDefaultFormat();
	///
	void contentsChanged();

private:
	///
	BufferView const * bv_;
	///
	QTextDocument * document_;
	/// LaTeX syntax highlighter
	LaTeXHighlighter * highlighter_;
	///
	bool force_getcontent_;
	///
	QString view_format_;
};


class GuiViewSource : public DockView
{
	Q_OBJECT

public:
	GuiViewSource(
		GuiView & parent, ///< the main window where to dock.
		Qt::DockWidgetArea area = Qt::BottomDockWidgetArea, ///< Position of the dock (and also drawer)
		Qt::WindowFlags flags = 0);

	~GuiViewSource();

	/// Controller inherited method.
	///@{
	bool initialiseParams(std::string const & source);
	void clearParams() {}
	void dispatchParams() {}
	bool isBufferDependent() const { return true; }
	bool canApply() const { return true; }
	bool canApplyToReadOnly() const { return true; }
	void updateView();
	void enableView(bool enable);
	void saveSession() const;
	void restoreSession();
	bool wantInitialFocus() const { return false; }
	///@}

	/// The title displayed by the dialog reflects source type.
	QString title() const;

private:
	/// The encapsulated widget.
	ViewSourceWidget * widget_;
};

} // namespace frontend
} // namespace lyx

#endif // GUIVIEWSOURCE_H
