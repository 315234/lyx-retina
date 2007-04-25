// -*- C++ -*-
/**
 * \file QInclude.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QINCLUDE_H
#define QINCLUDE_H

#include "QDialogView.h"

#include "ui/IncludeUi.h"

#include <QDialog>

namespace lyx {
namespace frontend {

class QInclude;

class QIncludeDialog : public QDialog, public Ui::QIncludeUi {
	Q_OBJECT
public:
	QIncludeDialog(QInclude * form);

	void updateLists();

	virtual void show();
protected Q_SLOTS:
	virtual void change_adaptor();
	virtual void loadClicked();
	virtual void browseClicked();
	virtual void typeChanged(int v);
protected:
	virtual void closeEvent(QCloseEvent * e);
private:
	QInclude * form_;
};


class ControlInclude;

///
class QInclude : public QController<ControlInclude, QView<QIncludeDialog> >
{
public:
	///
	friend class QIncludeDialog;
	///
	QInclude(Dialog &);
protected:
	virtual bool isValid();
private:
	/// Apply changes
	virtual void apply();
	/// update
	virtual void update_contents();
	/// build the dialog
	virtual void build_dialog();

	/// load the file
	void load();

	/// browse for a file
	void browse();
};

} // namespace frontend
} // namespace lyx

#endif // QINCLUDE_H
