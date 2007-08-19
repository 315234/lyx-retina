/**
 * \file QCommandEdit.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "QCommandEdit.h"

#include <QKeyEvent>
#include <QEvent>

#undef KeyPress

namespace lyx {
namespace frontend {

QCommandEdit::QCommandEdit(QWidget * parent)
	: QLineEdit(parent)
{
	setFocusPolicy(Qt::ClickFocus);
}


void QCommandEdit::keyPressEvent(QKeyEvent * e)
{
	switch (e->key()) {
	case Qt::Key_Escape:
		// emit signal
		escapePressed();
		break;

	case Qt::Key_Up:
		// emit signal
		upPressed();
		break;

	case Qt::Key_Down:
		// emit signal
		downPressed();
		break;

	case Qt::Key_X:
		if (e->modifiers() == Qt::AltModifier
		   || e->modifiers() == Qt::MetaModifier) {
			// emit signal
			hidePressed();
			break;
		}

	default:
		QLineEdit::keyPressEvent(e);
		break;
	}
}


bool QCommandEdit::event(QEvent * e)
{
	if (e->type() != QEvent::KeyPress)
		return QLineEdit::event(e);

	QKeyEvent * ev = (QKeyEvent *)e;

	if (ev->key() != Qt::Key_Tab)
		return QLineEdit::event(e);

	// emit signal
	tabPressed();
	return true;
}

} // namespace frontend
} // namespace lyx

#include "QCommandEdit_moc.cpp"
