// -*- C++ -*-
/**
 * \file QNoteDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QNOTEDIALOG_H
#define QNOTEDIALOG_H

#include "ui/NoteUi.h"

#include <QDialog>
#include <QCloseEvent>

namespace lyx {
namespace frontend {

class QNote;

class QNoteDialog : public QDialog, public Ui::QNoteUi {
	Q_OBJECT
public:
	QNoteDialog(QNote * form);
protected Q_SLOTS:
	virtual void change_adaptor();
protected:
	virtual void closeEvent(QCloseEvent * e);
private:
	QNote * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QNOTEDIALOG_H
