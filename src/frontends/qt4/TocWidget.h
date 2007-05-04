// -*- C++ -*-
/**
 * \file TocWidget.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TOC_WIDGET_H
#define TOC_WIDGET_H

#include "ui/TocUi.h"

#include <QWidget>

namespace lyx {
namespace frontend {

class QToc;

class TocWidget : public QWidget, public Ui::QTocUi {
	Q_OBJECT
public:
	TocWidget(QToc * form, QWidget * parent = 0);

	/// Update the display of the dialog whilst it is still visible.
	void update();

protected Q_SLOTS:
	/// Update Gui of the display.
	void updateGui();
	///
	void setTocModel(size_t type);
	///
	void select(QModelIndex const & index);
	///
	void selectionChanged(const QModelIndex & current,
		const QModelIndex & previous);

	void on_updateTB_clicked();
	void on_depthSL_valueChanged(int depth);
	void on_typeCO_activated(int value);
	void on_moveUpTB_clicked();
	void on_moveDownTB_clicked();
	void on_moveInTB_clicked();
	void on_moveOutTB_clicked();

protected:
	///
	void enableControls(bool enable = true);
	///
	int getIndexDepth(QModelIndex const & index, int depth = -1);
	///
	void setTreeDepth(int depth);

private:
	/// Reconnects the selection model change signal when TOC changed.
	void reconnectSelectionModel();

	QToc * form_;

	/// depth of list shown
	int depth_;
};

} // namespace frontend
} // namespace lyx

#endif // TOC_WIDGET_H
