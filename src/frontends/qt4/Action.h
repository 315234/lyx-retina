// -*- C++ -*-
/**
 * \file Action.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef ACTION_H
#define ACTION_H

#include <QAction>

class QIcon;

namespace lyx {

class FuncRequest;

namespace frontend {

/**
 * Action - Qt interface with LyX' FuncRequest.
 *
 * Action can be used in LyX menubar and/or toolbars.
 */
class Action : public QAction
{
	Q_OBJECT

public:
	Action(QIcon const & icon, QString const & text,
		FuncRequest const & func, QString const & tooltip, QObject * parent);

	void update();

Q_SIGNALS:
	/// the current action is triggered
	void triggered(QAction *);

private Q_SLOTS:
	void action();

private:
	FuncRequest const & func_ ;
};


} // namespace frontend
} // namespace lyx

#endif // ACTION_H
