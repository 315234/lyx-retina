// -*- C++ -*-
/**
 * \file QRefDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Kalle Dalheimer
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QREFDIALOG_H
#define QREFDIALOG_H

#include "ui/QRefUi.h"

#include <QDialog>
#include <QCloseEvent>

class QListWidgetItem;

namespace lyx {
namespace frontend {

class QRef;

class QRefDialog : public QDialog, public Ui::QRefUi {
	Q_OBJECT
public:
	QRefDialog(QRef * form);

	virtual void show();

public slots:
	void changed_adaptor();
	void gotoClicked();
	void refHighlighted(QListWidgetItem *);
	void refSelected(QListWidgetItem *);
	void sortToggled(bool);
	void updateClicked();

protected:
	void closeEvent(QCloseEvent * e);
private:
	QRef * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QREFDIALOG_H
