// -*- C++ -*-
/**
 * \file iconpalette.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef ICONPALETTE_H
#define ICONPALETTE_H

#include <QWidget>
#include <QMenu>
#include "Action.h"

namespace lyx {
namespace frontend {

/**
 * For holding an arbitrary set of icons.
 */
class IconPalette : public QWidget {
	Q_OBJECT
public:
	IconPalette(QWidget * parent);
	void addButton(QAction *);

public Q_SLOTS:
	void updateParent();

Q_SIGNALS:
	void triggered(QAction *);
	void visible(bool);

protected:
	void showEvent(QShowEvent * event);
	void hideEvent(QHideEvent * event);
	void paintEvent(QPaintEvent * event);

private Q_SLOTS:
	virtual void clicked(QAction *);

private:
	QGridLayout * layout_;
	QList<QAction *> actions_;
};

/**
 * Popup menu for a toolbutton.
 * We need this to keep track whether
 * it is necessary to enable/disable
 * the toolbutton
 */
class ButtonMenu : public QMenu {
	Q_OBJECT
public:
	ButtonMenu(const QString & title, QWidget * parent = 0 );
	void add(QAction *);

public Q_SLOTS:
	void updateParent();

private:
	QList<QAction *> actions_;
};


} // namespace frontend
} // namespace lyx

#endif // ICONPALETTE_H
