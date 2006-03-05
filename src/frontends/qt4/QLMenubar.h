// -*- C++ -*-
/**
 * \file qt2/QLMenubar.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QLMENUBAR_H
#define QLMENUBAR_H

#include "frontends/Menubar.h"

#include <map>

#include <QObject>

class QMenuBar;
class QMenu;
class LyXView;
class MenuBackend;
class Menu;
class MenuItem;

namespace lyx {


namespace frontend {

class QtView;

class QLMenubar : public QObject, public Menubar {
	Q_OBJECT
public:
	QLMenubar(LyXView *, MenuBackend &);

	/// opens a top-level submenu given its name
	void openByName(std::string const &);

	/// return the owning view
	QtView * view();

	/// return the menu controller
	MenuBackend const & backend();

	/// The QMenuBar used by LyX
	QMenuBar * menuBar() const;

	/// update the state of the menuitems - not needed
	void update();

public slots:
	/// populate a toplevel menu and all its children on demand
//	void updateMenu();

private:
	/// Initialize specific MACOS X menubar
	void QLMenubar::macxMenuBarInit();

	/// owning view
	QtView * owner_;

	/// menu controller
	MenuBackend & menubackend_;

	typedef std::map<std::string, QMenu *> NameMap;

	/// name to menu for openByName
	NameMap name_map_;

#ifdef Q_WS_MACX
	boost::scoped_ptr<QMenuBar> menubar_;
#endif
};

} // namespace frontend
} // namespace lyx

#endif // QLMENUBAR_H
