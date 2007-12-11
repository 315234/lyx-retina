// -*- C++ -*-
/**
 * \file qt4/GuiSelection.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiSelection.h"

#include "qt_helpers.h"
#include "support/debug.h"

#include "support/lstrings.h"

#include <QApplication>
#include <QClipboard>
#include <QString>


namespace lyx {
namespace frontend {

using support::internalLineEnding;
using support::externalLineEnding;


GuiSelection::GuiSelection()
	: selection_supported_(qApp->clipboard()->supportsSelection())
{
	connect(qApp->clipboard(), SIGNAL(dataChanged()),
		this, SLOT(on_dataChanged()));
	// initialize clipboard status.
	on_dataChanged();
}


void GuiSelection::haveSelection(bool own)
{
	if (!qApp->clipboard()->supportsSelection())
		return;

	// Tell qt that we have a selection by setting a dummy selection.
	// We don't use the interface provided by Qt for setting the
	// selection for performance reasons (see documentation of
	// Selection::put()). Instead we only tell here that we have a
	// selection by setting the selection to the empty string.
	// The real selection is set in GuiApplication::x11EventFilter when
	// an application actually requests it.
	// This way calling Selection::have() is cheap and we can do it as
	// often as we want.
	if (own)
		qApp->clipboard()->setText(QString(), QClipboard::Selection);
	// We don't need to do anything if own = false, as this case is
	// handled by QT.
	// FIXME (gb): This is wrong. What is missing here is rather a call of
	//else
	//	qApp->clipboard()->clear(QClipboard::Selection);
	// Since we do not issue this call we rather implement
	// "persistent selections" as far as X is concerned.
}


docstring const GuiSelection::get() const
{
	QString const str = qApp->clipboard()->text(QClipboard::Selection)
				.normalized(QString::NormalizationForm_C);
	LYXERR(Debug::ACTION, "GuiSelection::get: " << fromqstr(str));
	if (str.isNull())
		return docstring();

	return internalLineEnding(qstring_to_ucs4(str));
}


void GuiSelection::put(docstring const & str)
{
	LYXERR(Debug::ACTION, "GuiSelection::put: " << to_utf8(str));

	qApp->clipboard()->setText(toqstr(externalLineEnding(str)),
				   QClipboard::Selection);
}


void GuiSelection::on_dataChanged()
{
	text_selection_empty_ = qApp->clipboard()->
		text(QClipboard::Selection).isEmpty();
}


bool GuiSelection::empty() const
{
	if (!selection_supported_)
		return true;

	LYXERR(Debug::ACTION, "GuiSelection::empty: " << text_selection_empty_);

	return text_selection_empty_;
}

} // namespace frontend
} // namespace lyx

#include "GuiSelection_moc.cpp"
