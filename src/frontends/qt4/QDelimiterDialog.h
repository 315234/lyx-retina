// -*- C++ -*-
/**
 * \file QDelimiterDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QDELIMITERDIALOG_H
#define QDELIMITERDIALOG_H

#include "QDialogView.h"

#include "ui/DelimiterUi.h"

#include <string>

class QListWidgetItem;

namespace lyx {
namespace frontend {

class ControlMath;
class QMathDelimiter;

class QDelimiterDialog : public QDialog, public Ui::QDelimiterUi {
	Q_OBJECT
public:
	QDelimiterDialog(QMathDelimiter * form, QWidget * parent);
public Q_SLOTS:
	void on_leftLW_itemActivated(QListWidgetItem *);
	void on_rightLW_itemActivated(QListWidgetItem *);
	void on_leftLW_currentRowChanged(int);
	void on_rightLW_currentRowChanged(int);
	void on_matchCB_stateChanged(int);
	void on_insertPB_clicked();
	void on_sizeCO_activated(int);
private:
	///
	char_type doMatch(char_type const symbol) const;
	///
	void updateTeXCode(int size);
	/// owning form
	QMathDelimiter * form_;
	/// TeX code that will be inserted.
	QString tex_code_;
};


class QMathDelimiter : public QController<ControlMath, QView<QDelimiterDialog> > {
public:
	friend class QDelimiterDialog;

	QMathDelimiter(Dialog &);

private:
	virtual void apply() {}
	virtual void update_contents() {}
	/// Build the dialog.
	virtual void build_dialog();
};


} // namespace frontend
} // namespace lyx

#endif // QDELIMITERDIALOG_H
