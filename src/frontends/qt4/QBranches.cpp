/**
 * \file QBranches.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "QBranches.h"

#include "GuiApplication.h"
#include "Validator.h"
#include "qt_helpers.h"

#include "bufferparams.h"
#include "controllers/ControlDocument.h"

#include "support/lstrings.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QIcon>
#include <QColor>
#include <QColorDialog>


namespace lyx {
namespace frontend {


QBranches::QBranches(QWidget * parent, Qt::WFlags f)
	: QWidget(parent, f)
{
	setupUi(this);
	branchesTW->setColumnCount(3);
	branchesTW->headerItem()->setText(0, qt_("Branch"));
	branchesTW->headerItem()->setText(1, qt_("Activated"));
	branchesTW->headerItem()->setText(2, qt_("Color"));
	branchesTW->setSortingEnabled(true);
}

void QBranches::update(BufferParams const & params)
{
	branchlist_ = params.branchlist();
	update();
}

void QBranches::update()
{
	// store the selected branch
	QTreeWidgetItem * item = branchesTW->currentItem();
	QString sel_branch;
	if (item != 0)
		sel_branch = item->text(0);

	branchesTW->clear();

	BranchList::const_iterator it = branchlist_.begin();
	BranchList::const_iterator const end = branchlist_.end();
	for (; it != end; ++it) {
		QTreeWidgetItem * newItem =
			new QTreeWidgetItem(branchesTW);

		QString const bname = toqstr(it->getBranch());
		newItem->setText(0, bname);

		QString const sel = it->getSelected() ? qt_("Yes") : qt_("No");
		newItem->setText(1, sel);

		QColor const itemcolor = rgb2qcolor(it->getColor());
		if (itemcolor.isValid()) {
			QPixmap coloritem(30, 10);
			coloritem.fill(itemcolor);
			newItem->setIcon(2, QIcon(coloritem));
		}
		// restore selected branch
		if (bname == sel_branch) {
			branchesTW->setCurrentItem(newItem);
			branchesTW->setItemSelected(newItem, true);
		}
	}
	// emit signal
	changed();
}

void QBranches::apply(BufferParams & params) const
{
	params.branchlist() = branchlist_;
}

void QBranches::on_addBranchPB_pressed()
{
	QString const new_branch = newBranchLE->text();
	if (!new_branch.isEmpty()) {
		branchlist_.add(qstring_to_ucs4(new_branch));
		newBranchLE->clear();
		update();
	}
}


void QBranches::on_removePB_pressed()
{
	QTreeWidgetItem * selItem =
		branchesTW->currentItem();
	QString sel_branch;
	if (selItem != 0)
		sel_branch = selItem->text(0);
	if (!sel_branch.isEmpty()) {
		branchlist_.remove(qstring_to_ucs4(sel_branch));
		newBranchLE->clear();
		update();
	}
}


void QBranches::on_activatePB_pressed()
{
	toggleBranch(branchesTW->currentItem());
}


void QBranches::on_branchesTW_itemDoubleClicked(QTreeWidgetItem * item, int col)
{
	if (col < 2)
		toggleBranch(item);
	else
		toggleColor(item);
}


void QBranches::toggleBranch(QTreeWidgetItem * item)
{
	if (item == 0)
		return;

	QString sel_branch = item->text(0);
	if (!sel_branch.isEmpty()) {
		bool const selected = item->text(1) == qt_("Yes");
		Branch * branch = branchlist_.find(qstring_to_ucs4(sel_branch));
		if (branch && branch->setSelected(!selected)) {
			newBranchLE->clear();
			update();
		}
	}
}


void QBranches::on_colorPB_clicked()
{
	toggleColor(branchesTW->currentItem());
}


void QBranches::toggleColor(QTreeWidgetItem * item)
{
	if (item == 0)
		return;

	QString sel_branch = item->text(0);
	if (!sel_branch.isEmpty()) {
		docstring current_branch = qstring_to_ucs4(sel_branch);
		Branch * branch =
			branchlist_.find(current_branch);
		if (!branch)
			return;

		QColor const initial = rgb2qcolor(branch->getColor());
		QColor ncol(QColorDialog::getColor(initial, qApp->focusWidget()));
		if (ncol.isValid()){
			// add the color to the branchlist
			branch->setColor(fromqstr(ncol.name()));
			newBranchLE->clear();
			update();
		}
	}
}

} // namespace frontend
} // namespace lyx

#include "QBranches_moc.cpp"
