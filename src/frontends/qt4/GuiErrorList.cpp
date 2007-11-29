/**
 * \file GuiErrorList.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiErrorList.h"

#include "Buffer.h"
#include "BufferView.h"
#include "support/debug.h"
#include "support/gettext.h"
#include "Text.h"
#include "ParIterator.h"

#include "qt_helpers.h"

#include "support/lstrings.h"

#include <QListWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QCloseEvent>

using std::string;


namespace lyx {
namespace frontend {

using support::bformat;

GuiErrorList::GuiErrorList(GuiView & lv)
	: GuiDialog(lv, "errorlist")
{
	setupUi(this);

	connect(closePB, SIGNAL(clicked()),
		this, SLOT(slotClose()));
	connect(errorsLW, SIGNAL(itemClicked(QListWidgetItem *)),
		this, SLOT(select(QListWidgetItem *)));

	bc().setPolicy(ButtonPolicy::OkCancelPolicy);
	bc().setCancel(closePB);
}


void GuiErrorList::closeEvent(QCloseEvent * e)
{
	slotClose();
	e->accept();
}


void GuiErrorList::showEvent(QShowEvent * e)
{
	errorsLW->setCurrentRow(0);
	select(errorsLW->item(0));
	e->accept();
}


void GuiErrorList::select(QListWidgetItem * wi)
{
	int const item = errorsLW->row(wi);
	goTo(item);
	descriptionTB->setPlainText(toqstr(errorList()[item].description));
}


void GuiErrorList::updateContents()
{
	setViewTitle(name_);
	errorsLW->clear();
	descriptionTB->setPlainText(QString());

	ErrorList::const_iterator it = errorList().begin();
	ErrorList::const_iterator end = errorList().end();
	for (; it != end; ++it)
		errorsLW->addItem(toqstr(it->error));
}


ErrorList const & GuiErrorList::errorList() const
{
	return bufferview()->buffer().errorList(error_type_);
}


bool GuiErrorList::initialiseParams(string const & error_type)
{
	error_type_ = error_type;
	Buffer const & buf = bufferview()->buffer();
	name_ = bformat(_("%1$s Errors (%2$s)"), _(error_type),
				     from_utf8(buf.absFileName()));
	return true;
}


void GuiErrorList::goTo(int item)
{
	ErrorItem const & err = errorList()[item];

	if (err.par_id == -1)
		return;

	Buffer & buf = buffer();
	ParIterator pit = buf.getParFromID(err.par_id);

	if (pit == buf.par_iterator_end()) {
		LYXERR0("par id " << err.par_id << " not found");
		return;
	}

	// Now make the selection.
	// This should be implemented using an LFUN. (Angus)
	// if pos_end is 0, this means it is end-of-paragraph
	pos_type const end = err.pos_end ? std::min(err.pos_end, pit->size())
					 : pit->size();
	pos_type const start = std::min(err.pos_start, end);
	pos_type const range = end - start;
	DocIterator const dit = makeDocIterator(pit, start);
	bufferview()->putSelectionAt(dit, range, false);
	// FIXME: If we used an LFUN, we would not need this line:
	bufferview()->processUpdateFlags(Update::Force | Update::FitCursor);
}


Dialog * createGuiErrorList(GuiView & lv) { return new GuiErrorList(lv); }

} // namespace frontend
} // namespace lyx


#include "GuiErrorList_moc.cpp"
