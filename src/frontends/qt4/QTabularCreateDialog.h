// -*- C++ -*-
/**
 * \file QTabularCreateDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Edwin Leuven
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QTABULARCREATEDIALOG_H
#define QTABULARCREATEDIALOG_H

#include "ui/QTabularCreateUi.h"

#include <QDialog>

namespace lyx {
namespace frontend {

class QTabularCreate;

class QTabularCreateDialog : public QDialog, public Ui::QTabularCreateUi {
	Q_OBJECT
public:
	QTabularCreateDialog(QTabularCreate * form);
protected Q_SLOTS:
	virtual void columnsChanged(int);
	virtual void rowsChanged(int);
private:
	QTabularCreate * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QTABULARCREATEDIALOG_H
