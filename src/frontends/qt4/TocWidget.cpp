/**
 * \file TocWidget.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "TocWidget.h"

#include "GuiApplication.h"
#include "GuiView.h"
#include "qt_helpers.h"
#include "TocModel.h"

#include "Buffer.h"
#include "BufferView.h"
#include "CutAndPaste.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "LyX.h"
#include "Menus.h"
#include "TocBackend.h"

#include "insets/InsetCommand.h"
#include "insets/InsetRef.h"

#include "support/debug.h"
#include "support/lassert.h"

#include <QHeaderView>
#include <QMenu>
#include <QTimer>

#include <vector>

using namespace std;

namespace lyx {
namespace frontend {

TocWidget::TocWidget(GuiView & gui_view, QWidget * parent)
	: QWidget(parent), depth_(0), persistent_(false), gui_view_(gui_view)
{
	setupUi(this);

	moveOutTB->setIcon(QIcon(getPixmap("images/", "promote", "png")));
	moveInTB->setIcon(QIcon(getPixmap("images/", "demote", "png")));
	moveUpTB->setIcon(QIcon(getPixmap("images/", "up", "png")));
	moveDownTB->setIcon(QIcon(getPixmap("images/", "down", "png")));
	updateTB->setIcon(QIcon(getPixmap("images/", "reload", "png")));

	// avoid flickering
	tocTV->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	tocTV->showColumn(0);

	// hide the pointless QHeader for now
	// in the future, new columns may appear
	// like labels, bookmarks, etc...
	// tocTV->header()->hide();
	tocTV->header()->setVisible(false);

	// Only one item selected at a time.
	tocTV->setSelectionMode(QAbstractItemView::SingleSelection);
	setFocusProxy(tocTV);

	// The toc types combo won't change its model.
	typeCO->setModel(gui_view_.tocModels().nameModel());

	// Make sure the buttons are disabled when first shown without a loaded
	// Buffer.
	enableControls(false);

	// make us responsible for the context menu of the tabbar
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));
	connect(tocTV, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(showContextMenu(const QPoint &)));
	connect(filterLE, SIGNAL(textEdited(QString)), 
		this, SLOT(filterContents()));

	init(QString());
}


void TocWidget::showContextMenu(const QPoint & pos)
{
	std::string name = "context-toc-" + fromqstr(current_type_);
	QMenu * menu = guiApp->menus().menu(toqstr(name), gui_view_);
	if (!menu)
		return;	
	menu->exec(mapToGlobal(pos));
}


Inset * TocWidget::itemInset() const
{
	QModelIndex const & index = tocTV->currentIndex();
	TocItem const & item =
		gui_view_.tocModels().currentItem(current_type_, index);
	DocIterator const & dit = item.dit();
	
	Inset * inset = 0;
	if (current_type_ == "label" 
		  || current_type_ == "graphics"
		  || current_type_ == "citation"
		  || current_type_ == "child")
		inset = dit.nextInset();

	else if (current_type_ == "branch"
			 || current_type_ == "index"
			 || current_type_ == "change")
		inset = &dit.inset();

	else if (current_type_ == "table" 
		     || current_type_ == "listing"
		     || current_type_ == "figure") {
		DocIterator tmp_dit(dit);
		tmp_dit.pop_back();
		inset = &tmp_dit.inset();
	}
	return inset;
}


bool TocWidget::getStatus(Cursor & cur, FuncRequest const & cmd,
	FuncStatus & status) const
{
	Inset * inset = itemInset();
	FuncRequest tmpcmd(cmd);

	QModelIndex const & index = tocTV->currentIndex();
	TocItem const & item =
		gui_view_.tocModels().currentItem(current_type_, index);

	switch (cmd.action())
	{
	case LFUN_CHANGE_ACCEPT:
	case LFUN_CHANGE_REJECT:
	case LFUN_OUTLINE_UP:
	case LFUN_OUTLINE_DOWN:
	case LFUN_OUTLINE_IN:
	case LFUN_OUTLINE_OUT:
	case LFUN_SECTION_SELECT:
		status.setEnabled(true);
		return true;

	case LFUN_LABEL_COPY_AS_REF: {
		// For labels in math, we need to supply the label as a string
		FuncRequest label_copy(LFUN_LABEL_COPY_AS_REF, item.asString());
		if (inset)
			return inset->getStatus(cur, label_copy, status);
	}

	default:
		if (inset)
			return inset->getStatus(cur, tmpcmd, status);
	}

	return false;
}


void TocWidget::doDispatch(Cursor & cur, FuncRequest const & cmd)
{
	Inset * inset = itemInset();
	FuncRequest tmpcmd(cmd);

	QModelIndex const & index = tocTV->currentIndex();
	TocItem const & item =
		gui_view_.tocModels().currentItem(current_type_, index);

	// Start an undo group.
	cur.beginUndoGroup();

	switch (cmd.action())
	{
	case LFUN_CHANGE_ACCEPT:
	case LFUN_CHANGE_REJECT:
	case LFUN_SECTION_SELECT:
		dispatch(item.action());
		cur.dispatch(tmpcmd);
		break;

	case LFUN_LABEL_COPY_AS_REF: {
		// For labels in math, we need to supply the label as a string
		FuncRequest label_copy(LFUN_LABEL_COPY_AS_REF, item.asString());
		if (inset)
			inset->dispatch(cur, label_copy);
		break;
	}
	
	case LFUN_OUTLINE_UP:
	case LFUN_OUTLINE_DOWN:
	case LFUN_OUTLINE_IN:
	case LFUN_OUTLINE_OUT:
		outline(cmd.action());
		break;

	default:
		if (inset)
			inset->dispatch(cur, tmpcmd);
	}
	cur.endUndoGroup();
}


void TocWidget::on_tocTV_activated(QModelIndex const & index)
{
	goTo(index);
}


void TocWidget::on_tocTV_pressed(QModelIndex const & index)
{
	Qt::MouseButtons const button = QApplication::mouseButtons();
	if (button & Qt::LeftButton) {
		goTo(index);
		gui_view_.setFocus();
	}
}


void TocWidget::goTo(QModelIndex const & index)
{
	LYXERR(Debug::GUI, "goto " << index.row()
		<< ", " << index.column());

	gui_view_.tocModels().goTo(current_type_, index);
}


void TocWidget::on_updateTB_clicked()
{
	// The backend update can take some time so we disable
	// the controls while waiting.
	enableControls(false);
	gui_view_.tocModels().updateBackend();
}


void TocWidget::on_sortCB_stateChanged(int state)
{
	gui_view_.tocModels().sort(current_type_, state == Qt::Checked);
	updateView();
}


void TocWidget::on_persistentCB_stateChanged(int state)
{
	persistent_ = state == Qt::Checked;
}


/* FIXME (Ugras 17/11/06):
I have implemented a indexDepth function to get the model indices. In my
opinion, somebody should derive a new qvariant class for tocModelItem
which saves the string data and depth information. That will save the
depth calculation.  */

static int indexDepth(QModelIndex const & index, int depth = -1)
{
	++depth;
	return index.parent() == QModelIndex()
		? depth : indexDepth(index.parent(), depth);
}


void TocWidget::on_depthSL_valueChanged(int depth)
{
	if (depth == depth_)
		return;
	setTreeDepth(depth);
	gui_view_.setFocus();
}


void TocWidget::setTreeDepth(int depth)
{
	depth_ = depth;
	if (!tocTV->model())
		return;

#if QT_VERSION >= 0x040300
	// this should be faster than our own code below
	if (depth == 0)
		tocTV->collapseAll();
	else
		tocTV->expandToDepth(depth - 1);
#else
	// expanding and then collapsing is probably better,
	// but my qt 4.1.2 doesn't have expandAll()..
	//tocTV->expandAll();
	QModelIndexList indices = tocTV->model()->match(
		tocTV->model()->index(0, 0),
		Qt::DisplayRole, "*", -1,
		Qt::MatchFlags(Qt::MatchWildcard|Qt::MatchRecursive));

	int size = indices.size();
	for (int i = 0; i < size; i++) {
		QModelIndex index = indices[i];
		tocTV->setExpanded(index, indexDepth(index) < depth_);
	}
#endif
}


void TocWidget::on_typeCO_currentIndexChanged(int index)
{
	if (index == -1)
		return;
	current_type_ = typeCO->itemData(index).toString();
	updateView();
	if (typeCO->hasFocus())
		gui_view_.setFocus();
}


void TocWidget::outline(FuncCode func_code)
{
	QModelIndexList const & list = tocTV->selectionModel()->selectedIndexes();
	if (list.isEmpty())
		return;
	enableControls(false);
	goTo(list[0]);
	dispatch(FuncRequest(func_code));
	enableControls(true);
	gui_view_.setFocus();
}


void TocWidget::on_moveUpTB_clicked()
{
	outline(LFUN_OUTLINE_UP);
}


void TocWidget::on_moveDownTB_clicked()
{
	outline(LFUN_OUTLINE_DOWN);
}


void TocWidget::on_moveInTB_clicked()
{
	outline(LFUN_OUTLINE_IN);
}


void TocWidget::on_moveOutTB_clicked()
{
	outline(LFUN_OUTLINE_OUT);
}


void TocWidget::select(QModelIndex const & index)
{
	if (!index.isValid()) {
		LYXERR(Debug::GUI, "TocWidget::select(): QModelIndex is invalid!");
		return;
	}

	tocTV->scrollTo(index);
	tocTV->clearSelection();
	tocTV->setCurrentIndex(index);
}


void TocWidget::enableControls(bool enable)
{
	updateTB->setEnabled(enable);

	if (!canOutline())
		enable = false;

	moveUpTB->setEnabled(enable);
	moveDownTB->setEnabled(enable);
	moveInTB->setEnabled(enable);
	moveOutTB->setEnabled(enable);
}


void TocWidget::updateView()
{
	if (!gui_view_.documentBufferView()) {
		tocTV->setModel(0);
		depthSL->setMaximum(0);
		depthSL->setValue(0);
		setEnabled(false);
		return;
	}
	setEnabled(true);
	bool const is_sortable = isSortable();
	sortCB->setEnabled(is_sortable);
	bool focus_ = tocTV->hasFocus();
	tocTV->setEnabled(false);
	tocTV->setUpdatesEnabled(false);

	QAbstractItemModel * toc_model = 
			gui_view_.tocModels().model(current_type_);
	if (tocTV->model() != toc_model) {
		tocTV->setModel(toc_model);
		tocTV->setEditTriggers(QAbstractItemView::NoEditTriggers);
		if (persistent_)
			setTreeDepth(depth_);
	}

	sortCB->blockSignals(true);
	sortCB->setChecked(is_sortable
		&& gui_view_.tocModels().isSorted(current_type_));
	sortCB->blockSignals(false);

	bool const can_navigate_ = canNavigate();
	persistentCB->setEnabled(can_navigate_);

	bool controls_enabled = toc_model && toc_model->rowCount() > 0
		&& !gui_view_.documentBufferView()->buffer().isReadonly();
	enableControls(controls_enabled);

	depthSL->setMaximum(gui_view_.tocModels().depth(current_type_));
	depthSL->setValue(depth_);
	if (!persistent_ && can_navigate_)
		setTreeDepth(depth_);
	if (can_navigate_) {
		persistentCB->setChecked(persistent_);
		select(gui_view_.tocModels().currentIndex(current_type_));
	}
	filterContents();
	tocTV->setEnabled(true);
	tocTV->setUpdatesEnabled(true);
	if (focus_)
		tocTV->setFocus();
}


void TocWidget::filterContents()
{
	if (!tocTV->model())
		return;

	QModelIndexList indices = tocTV->model()->match(
		tocTV->model()->index(0, 0),
		Qt::DisplayRole, "*", -1,
		Qt::MatchFlags(Qt::MatchWildcard|Qt::MatchRecursive));

	int size = indices.size();
	for (int i = 0; i < size; i++) {
		QModelIndex index = indices[i];
		bool const matches =
			index.data().toString().contains(
				filterLE->text(), Qt::CaseInsensitive);
		tocTV->setRowHidden(index.row(), index.parent(), !matches);
	}
	// recursively unhide parents of unhidden children 
	for (int i = size - 1; i >= 0; i--) {
		QModelIndex index = indices[i];
		if (!tocTV->isRowHidden(index.row(), index.parent())
		    && index.parent() != QModelIndex())
			tocTV->setRowHidden(index.parent().row(),
					    index.parent().parent(), false);
	}
}


static QString decodeType(QString const & str)
{
	QString type = str;
	if (type.contains("tableofcontents")) {
		type = "tableofcontents";
	} else if (type.contains("floatlist")) {
		if (type.contains("\"figure"))
			type = "figure";
		else if (type.contains("\"table"))
			type = "table";
		else if (type.contains("\"algorithm"))
			type = "algorithm";
	}
	return type;
}


void TocWidget::init(QString const & str)
{
	int new_index;
	if (str.isEmpty())
		new_index = typeCO->findData(current_type_);
	else
		new_index = typeCO->findData(decodeType(str));

	// If everything else fails, settle on the table of contents which is
	// guaranteed to exist.
	if (new_index == -1) {
		current_type_ = "tableofcontents";
		new_index = typeCO->findData(current_type_);
	} else {
		current_type_ = typeCO->itemData(new_index).toString();
	}

	typeCO->blockSignals(true);
	typeCO->setCurrentIndex(new_index);
	typeCO->blockSignals(false);
}

} // namespace frontend
} // namespace lyx

#include "moc_TocWidget.cpp"
