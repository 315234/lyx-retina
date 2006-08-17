// -*- C++ -*-
/**
 * \file qfontexample.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QT_QFONTEXAMPLE_H
#define QT_QFONTEXAMPLE_H

#include <qwidget.h>
#include <qfont.h>
//Added by qt3to4:
#include <QPaintEvent>

class QFontExample : public QWidget {

public:
	QFontExample(QWidget * parent)
		: QWidget(parent) {}

	void set(QFont const & font, QString const & text);

	virtual QSize sizeHint() const;

protected:
	virtual void paintEvent(QPaintEvent * p);

private:
	QFont font_;
	QString text_;
};

#endif
