// -*- C++ -*-
/**
 * \file GuiDelimiterDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIDELIMITERDIALOG_H
#define GUIDELIMITERDIALOG_H

#include "GuiDialog.h"
#include "ControlMath.h"
#include "ui_DelimiterUi.h"

class QListWidgetItem;

namespace lyx {
namespace frontend {

class GuiDelimiterDialog : public GuiDialog, public Ui::DelimiterUi
{
	Q_OBJECT

public:
	GuiDelimiterDialog(LyXView & lv);

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
	/// parent controller
	ControlMath & controller() const;

	/// TeX code that will be inserted.
	QString tex_code_;
};

} // namespace frontend
} // namespace lyx

#endif // GUIDELIMITERDIALOG_H
