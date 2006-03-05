// -*- C++ -*-
/**
 * \file QMathMatrixDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Edwin Leuven
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QMATHMATRIXDIALOG_H
#define QMATHMATRIXDIALOG_H

#include "ui/QMathMatrixUi.h"

#include <QDialog>

namespace lyx {
namespace frontend {

class QMathMatrix;

class QMathMatrixDialog : public QDialog, public Ui::QMathMatrixUi {
	Q_OBJECT
public:
	QMathMatrixDialog(QMathMatrix * form);
public slots:
	void slotOK();
	void slotClose();
protected slots:
	virtual void columnsChanged(int);
	virtual void rowsChanged(int);
	virtual void change_adaptor();
private:
	QMathMatrix * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QMATHMATRIXDIALOG_H
