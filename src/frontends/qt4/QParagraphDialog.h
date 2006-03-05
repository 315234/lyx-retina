// -*- C++ -*-
/**
 * \file QParagraphDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Edwin Leuven
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QPARAGRAPHDIALOG_H
#define QPARAGRAPHDIALOG_H

#include "ui/QParagraphUi.h"

#include <QDialog>
#include <QCloseEvent>

namespace lyx {
namespace frontend {

class QParagraph;

class QParagraphDialog : public QDialog, public Ui::QParagraphUi {
	Q_OBJECT
public:
	QParagraphDialog(QParagraph * form);
protected:
	void closeEvent (QCloseEvent * e);
private:
	QParagraph * form_;
protected slots:
	void change_adaptor();
	void enableLinespacingValue(int);
};

} // namespace frontend
} // namespace lyx

#endif // QPARAGRAPHDIALOG_H
