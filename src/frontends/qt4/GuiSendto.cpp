/**
 * \file GuiSendto.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiSendto.h"
#include "qt_helpers.h"

#include "Format.h"

#include <QListWidget>
#include <QPushButton>
#include <QCloseEvent>

using std::vector;
using std::string;


namespace lyx {
namespace frontend {

/////////////////////////////////////////////////////////////////////
//
// GuiSendtoDialog
//
/////////////////////////////////////////////////////////////////////

GuiSendtoDialog::GuiSendtoDialog(GuiSendto * form)
	: form_(form)
{
	setupUi(this);

	connect(okPB, SIGNAL(clicked()),
		form, SLOT(slotOK()));
	connect(applyPB, SIGNAL(clicked()),
		form, SLOT(slotApply()));
	connect(closePB, SIGNAL(clicked()),
		form, SLOT(slotClose()));

	connect( formatLW, SIGNAL( itemClicked(QListWidgetItem *) ),
		this, SLOT( slotFormatHighlighted(QListWidgetItem *) ) );
	connect( formatLW, SIGNAL( itemActivated(QListWidgetItem *) ),
		this, SLOT( slotFormatSelected(QListWidgetItem *) ) );
	connect( formatLW, SIGNAL( itemClicked(QListWidgetItem *) ),
		this, SLOT( changed_adaptor() ) );
	connect( commandCO, SIGNAL( textChanged(const QString&) ),
		this, SLOT( changed_adaptor() ) );
}


void GuiSendtoDialog::changed_adaptor()
{
	form_->changed();
}


void GuiSendtoDialog::closeEvent(QCloseEvent * e)
{
	form_->slotWMHide();
	e->accept();
}


/////////////////////////////////////////////////////////////////////
//
// GuiSendto
//
/////////////////////////////////////////////////////////////////////


GuiSendto::GuiSendto(GuiDialog & parent)
	: GuiView<GuiSendtoDialog>(parent, _("Send Document to Command"))
{
}


void GuiSendto::build_dialog()
{
	dialog_.reset(new GuiSendtoDialog(this));

	// Manage the ok, apply, restore and cancel/close buttons
	bc().setOK(dialog_->okPB);
	bc().setApply(dialog_->applyPB);
	bc().setCancel(dialog_->closePB);
}


void GuiSendto::update_contents()
{
	all_formats_ = controller().allFormats();

	// Check whether the current contents of the browser will be
	// changed by loading the contents of formats
	vector<string> keys;
	keys.resize(all_formats_.size());

	vector<string>::iterator result = keys.begin();
	vector<Format const *>::const_iterator it  = all_formats_.begin();
	vector<Format const *>::const_iterator end = all_formats_.end();
	for (; it != end; ++it, ++result) {
		*result = (*it)->prettyname();
	}

	// Reload the browser
	dialog_->formatLW->clear();

	for (vector<string>::const_iterator it = keys.begin();
	     it < keys.end(); ++it) {
		dialog_->formatLW->addItem(toqstr(*it));
	}

	dialog_->commandCO->addItem(toqstr(controller().getCommand()));
}


void GuiSendto::applyView()
{
	int const line(dialog_->formatLW->currentRow());

	if (line < 0 || line > int(dialog_->formatLW->count()))
		return;

	string const cmd(fromqstr(dialog_->commandCO->currentText()));

	controller().setFormat(all_formats_[line]);
	controller().setCommand(cmd);
}


bool GuiSendto::isValid()
{
	int const line(dialog_->formatLW->currentRow());

	if (line < 0 || line > int(dialog_->formatLW->count()))
		return false;

	else return dialog_->formatLW->count() != 0 &&
		!dialog_->commandCO->currentText().isEmpty();
}

} // namespace frontend
} // namespace lyx

#include "GuiSendto_moc.cpp"
