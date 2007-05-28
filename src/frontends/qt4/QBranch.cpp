/**
 * \file QBranch.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "QBranch.h"
#include "Qt2BC.h"
#include "qt_helpers.h"

#include "BranchList.h"

#include "controllers/ControlBranch.h"

#include "insets/InsetBranch.h"

#include <QPushButton>
#include <QCloseEvent>

namespace lyx {
namespace frontend {

/////////////////////////////////////////////////////////////////////
//
// QBranchDialog
//
/////////////////////////////////////////////////////////////////////

QBranchDialog::QBranchDialog(QBranch * form)
	: form_(form)
{
	setupUi(this);
	connect(okPB, SIGNAL(clicked()),
		form, SLOT(slotOK()));
	connect(closePB, SIGNAL(clicked()),
		form, SLOT(slotClose()));
	connect(branchCO, SIGNAL( activated(int) ),
		this, SLOT( change_adaptor() ) );
}


void QBranchDialog::closeEvent(QCloseEvent * e)
{
	form_->slotWMHide();
	e->accept();
}


void QBranchDialog::change_adaptor()
{
	form_->changed();
}


/////////////////////////////////////////////////////////////////////
//
// QBranch
//
/////////////////////////////////////////////////////////////////////

typedef QController<ControlBranch, QView<QBranchDialog> > branch_base_class;


QBranch::QBranch(Dialog & parent)
	: branch_base_class(parent, _("Branch Settings"))
{}


void QBranch::build_dialog()
{
	dialog_.reset(new QBranchDialog(this));

	bcview().setOK(dialog_->okPB);
	bcview().setCancel(dialog_->closePB);
}


void QBranch::update_contents()
{
	typedef BranchList::const_iterator const_iterator;

	BranchList const & branchlist = controller().branchlist();
	docstring const cur_branch = controller().params().branch;

	dialog_->branchCO->clear();

	const_iterator const begin = branchlist.begin();
	const_iterator const end = branchlist.end();
	int id = 0;
	int count = 0;
	for (const_iterator it = begin; it != end; ++it, ++count) {
		docstring const & branch = it->getBranch();
		dialog_->branchCO->addItem(toqstr(branch));

		if (cur_branch == branch)
			id = count;
	}
	dialog_->branchCO->setCurrentIndex(id);
}


void QBranch::apply()
{
	docstring const type = qstring_to_ucs4(dialog_->branchCO->currentText());
	controller().params().branch = type;
}

} // namespace frontend
} // namespace lyx

#include "QBranch_moc.cpp"
