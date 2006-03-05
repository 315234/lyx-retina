// -*- C++ -*-
/**
 * \file QMathDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QMATHDIALOG_H
#define QMATHDIALOG_H

#include "ui/QMathUi.h"
#include <string>

#include <QDialog>

class IconPalette;

namespace lyx {
namespace frontend {

class QMath;

class QMathDialog : public QDialog, public Ui::QMathUi
{
	Q_OBJECT
public:
	QMathDialog(QMath * form);
public slots:
	virtual void delimiterClicked();
	virtual void expandClicked();
	virtual void fracClicked();
	virtual void functionSelected(const QString &);
	virtual void matrixClicked();
	virtual void subscriptClicked();
	virtual void superscriptClicked();
	virtual void equationClicked();
	void symbol_clicked(const std::string &);
	void insertSpace(int id);
	void insertRoot(int id);
	void insertStyle(int id);
	void insertFont(int id);

	/// about to show a symbol panel
	void showingPanel(int);
protected:
	//needed ? virtual void closeEvent(QCloseEvent * e);
private:
	/// make a symbol panel
	IconPalette * makePanel(QWidget * parent, char const ** entries);

	/// add a symbol panel
	void addPanel(int num);

	/// owning form
	QMath * form_;
};

} // namespace frontend
} // namespace lyx

#endif // QMATHDIALOG_H
