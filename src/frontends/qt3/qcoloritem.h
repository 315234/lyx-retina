// -*- C++ -*-
/**
 * \file qcoloritem.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QT_QCOLORITEM_H
#define QT_QCOLORITEM_H

#include <qlistbox.h>
#include <qcolor.h>


namespace lyx {

class QColorItem : public QListBoxItem {

public:
	QColorItem(QColor c, QString const & text);

	QColor color() {
		return color_;
	}

	void color(QColor c) {
		color_ = c;
	}

	virtual int height(const QListBox *) const {
		return height_;
	}

	virtual int width(const QListBox *) const {
		return width_;
	}

protected:
	virtual void paint(QPainter * p);

private:
	int ascent_;

	int height_;

	int width_;

	QColor color_;

	QString text_;
};


} // namespace lyx

#endif
