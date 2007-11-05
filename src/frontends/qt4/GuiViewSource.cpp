/**
 * \file GuiViewSource.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Bo Peng
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiViewSource.h"
#include "LaTeXHighlighter.h"
#include "qt_helpers.h"

#include "Application.h"
#include "BufferView.h"
#include "Buffer.h"
#include "Cursor.h"
#include "gettext.h"
#include "Paragraph.h"
#include "TexRow.h"

#include "support/docstream.h"

#include <QTextCursor>
#include <QTextDocument>
#include <boost/tuple/tuple.hpp>

using std::string;

namespace lyx {
namespace frontend {

ViewSourceWidget::ViewSourceWidget(GuiViewSource & controller)
	:	controller_(controller), document_(new QTextDocument(this)),
		highlighter_(new LaTeXHighlighter(document_))
{
	setupUi(this);
	setWindowTitle(qt_("LaTeX Source"));

	connect(viewFullSourceCB, SIGNAL(clicked()),
		this, SLOT(updateView()));
	connect(autoUpdateCB, SIGNAL(toggled(bool)),
		updatePB, SLOT(setDisabled(bool)));
	connect(updatePB, SIGNAL(clicked()),
		this, SLOT(updateView()));

	// setting a document at this point trigger an assertion in Qt
	// so we disable the signals here:
	document_->blockSignals(true);
	viewSourceTV->setDocument(document_);
	document_->blockSignals(false);
	viewSourceTV->setReadOnly(true);
	///dialog_->viewSourceTV->setAcceptRichText(false);
	// this is personal. I think source code should be in fixed-size font
	QFont font(toqstr(theApp()->typewriterFontName()));
	font.setKerning(false);
	font.setFixedPitch(true);
	font.setStyleHint(QFont::TypeWriter);
	viewSourceTV->setFont(font);
	// again, personal taste
	viewSourceTV->setWordWrapMode(QTextOption::NoWrap);
}


void ViewSourceWidget::updateView()
{
	if (autoUpdateCB->isChecked())
		update(viewFullSourceCB->isChecked());

	int beg, end;
	boost::tie(beg, end) = controller_.getRows();
	QTextCursor c = QTextCursor(viewSourceTV->document());
	c.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, beg);
	c.select(QTextCursor::BlockUnderCursor);
	c.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor, end - beg + 1);
	viewSourceTV->setTextCursor(c);
}


void ViewSourceWidget::update(bool full_source)
{
	document_->setPlainText(controller_.getContent(full_source));
}


GuiViewSource::GuiViewSource(GuiView & parent, Qt::DockWidgetArea area, Qt::WindowFlags flags)
	: DockView(parent, "view-source", area, flags)
{
	widget_ = new ViewSourceWidget(*this);
	setWidget(widget_);
	setWindowTitle(widget_->windowTitle());
}


GuiViewSource::~GuiViewSource()
{
	delete widget_;
}


void GuiViewSource::updateView()
{
	widget_->updateView();
}


bool GuiViewSource::initialiseParams(string const & /*source*/)
{
	setWindowTitle(title());
	return true;
}


QString GuiViewSource::getContent(bool fullSource)
{
	// get the *top* level paragraphs that contain the cursor,
	// or the selected text
	pit_type par_begin;
	pit_type par_end;

	BufferView * view = bufferview();
	if (!view->cursor().selection()) {
		par_begin = view->cursor().bottom().pit();
		par_end = par_begin;
	} else {
		par_begin = view->cursor().selectionBegin().bottom().pit();
		par_end = view->cursor().selectionEnd().bottom().pit();
	}
	if (par_begin > par_end)
		std::swap(par_begin, par_end);
	odocstringstream ostr;
	view->buffer().getSourceCode(ostr, par_begin, par_end + 1, fullSource);
	return toqstr(ostr.str());
}


std::pair<int, int> GuiViewSource::getRows() const
{
	BufferView const * view = bufferview();
	CursorSlice beg = view->cursor().selectionBegin().bottom();
	CursorSlice end = view->cursor().selectionEnd().bottom();

	int begrow = view->buffer().texrow().
		getRowFromIdPos(beg.paragraph().id(), beg.pos());
	int endrow = view->buffer().texrow().
		getRowFromIdPos(end.paragraph().id(), end.pos());
	int nextendrow = view->buffer().texrow().
		getRowFromIdPos(end.paragraph().id(), end.pos() + 1);
	return std::make_pair(begrow, endrow == nextendrow ? endrow : (nextendrow - 1));
}


QString GuiViewSource::title() const
{
	switch (docType()) {
		case LATEX:
			return qt_("LaTeX Source");
		case DOCBOOK:
			return qt_("DocBook Source");
		case LITERATE:
			return qt_("Literate Source");
	}
	BOOST_ASSERT(false);
	return QString();
}


Dialog * createGuiViewSource(LyXView & lv)
{
	return new GuiViewSource(static_cast<GuiView &>(lv));
}


} // namespace frontend
} // namespace lyx

#include "GuiViewSource_moc.cpp"
