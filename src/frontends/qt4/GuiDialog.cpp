/**
 * \file Dialog.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiDialog.h"
#include "GuiView.h"
#include "qt_helpers.h"

#include "support/debug.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QSettings>
#include <QShowEvent>

using namespace std;

namespace lyx {
namespace frontend {

GuiDialog::GuiDialog(GuiView & lv, string const & name)
	:  QDialog(&lv), Dialog(lv, name), is_closing_(false)
{}


void GuiDialog::setViewTitle(docstring const & title)
{
	setWindowTitle("LyX: " + toqstr(title));
}


void GuiDialog::setButtonsValid(bool valid)
{
	bc().setValid(valid);
}


void GuiDialog::slotApply()
{
	apply();
	bc().apply();
}


void GuiDialog::slotOK()
{
	is_closing_ = true;
	apply();
	is_closing_ = false;
	QDialog::hide();
	bc().ok();
}


void GuiDialog::slotClose()
{
	QDialog::hide();
	bc().cancel();
}


void GuiDialog::slotRestore()
{
	// Tell the controller that a request to refresh the dialog's contents
	// has been received. It's up to the controller to supply the necessary
	// info by calling GuiDialog::updateView().
	updateDialog();
	bc().restore();
}


void GuiDialog::changed()
{
	if (updating_)
		return;
	bc().setValid(isValid());
}


void GuiDialog::enableView(bool enable)
{
	bc().setReadOnly(!enable);
	bc().setValid(enable);
	Dialog::enableView(enable);
}


void GuiDialog::updateView()
{
	setUpdatesEnabled(false);

	bc().setReadOnly(isBufferReadonly());
	// protect the BC from unwarranted state transitions
	updating_ = true;
	updateContents();
	updating_ = false;
	// The widgets may not be valid, so refresh the button controller
	bc().refresh();

	setUpdatesEnabled(true);
}

} // namespace frontend
} // namespace lyx


/////////////////////////////////////////////////////////////////////
//
// Command based dialogs
//
/////////////////////////////////////////////////////////////////////

#include "FuncRequest.h"
#include "insets/InsetCommand.h"

using namespace std;

namespace lyx {
namespace frontend {

GuiCommand::GuiCommand(GuiView & lv, string const & name)
	: GuiDialog(lv, name), params_(insetCode(name)), lfun_name_(name)
{
}


bool GuiCommand::initialiseParams(string const & data)
{
	// The name passed with LFUN_INSET_APPLY is also the name
	// used to identify the mailer.
	InsetCommandMailer::string2params(lfun_name_, data, params_);
	return true;
}


void GuiCommand::dispatchParams()
{
	if (lfun_name_.empty())
		return;

	string const lfun = 
		InsetCommandMailer::params2string(lfun_name_, params_);
	dispatch(FuncRequest(getLfun(), lfun));
}

} // namespace frontend
} // namespace lyx

#include "GuiDialog_moc.cpp"
