// -*- C++ -*-
/**
 * \file QGraphicsDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Herbert Vo�
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QGRAPHICSDIALOG_H
#define QGRAPHICSDIALOG_H

#include "ui/QGraphicsUi.h"

#include <QCloseEvent>
#include <QDialog>

namespace lyx {
namespace frontend {

class QGraphics;

class QGraphicsDialog : public QDialog, public Ui::QGraphicsUi {
	Q_OBJECT
public:
	QGraphicsDialog(QGraphics * form);

	virtual void show();
protected slots:
	virtual void change_adaptor();
	virtual void change_bb();
	virtual void change_WUnit();
	virtual void browse_clicked();
	virtual void getBB_clicked();
	virtual void edit_clicked();
protected:
	virtual void closeEvent(QCloseEvent * e);
private:
	QGraphics * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QGRAPHICSDIALOG_H
