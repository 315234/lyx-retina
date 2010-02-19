// -*- C++ -*-
/**
 * \file InsetParamsDialog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_PARAMS_DIALOG_H
#define INSET_PARAMS_DIALOG_H

#include "DialogView.h"
#include "ui_InsetParamsUi.h"

#include "qt_i18n.h"

namespace lyx {

class Inset;

namespace frontend {

class InsetParamsWidget;

class InsetParamsDialog : public DialogView, public Ui::InsetParamsUi
{
	Q_OBJECT
public:
	InsetParamsDialog(GuiView & lv, InsetParamsWidget * widget);
	~InsetParamsDialog();

	///
	void setInsetParamsWidget(InsetParamsWidget * widget);

protected Q_SLOTS:
	void onWidget_changed();
	void applyView();
	void on_restorePB_clicked();
	void on_okPB_clicked();
	void on_applyPB_clicked();
	void on_closePB_clicked();
	void on_synchronizedViewCB_stateChanged(int state);

private:
	/// \name DialogView inherited methods
	//@{
	void updateView();
	void dispatchParams() {}
	bool isBufferDependent() const { return true; }
	bool canApply() const { return true; }
	//@}
	///
	void newInset();
	///
	void updateView(bool update_widget);
	///
	docstring checkWidgets(bool synchronized_view);
	/// pimpl
	struct Private;
	Private * d;
};

} // namespace frontend
} // namespace lyx

#endif // INSET_PARAMS_DIALOG_H
