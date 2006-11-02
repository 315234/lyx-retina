// -*- C++ -*-
/**
 * \file ToolbarBackend.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TOOLBAR_BACKEND_H
#define TOOLBAR_BACKEND_H

#include "funcrequest.h"

#include <vector>


namespace lyx {


class LyXLex;


///
class ToolbarBackend {
public:
	/// The special toolbar actions
	enum ItemType {
		/// the command buffer
		MINIBUFFER = -3,
		/// adds space between buttons in the toolbar
		SEPARATOR = -2,
		/// a special combox insead of a button
		LAYOUTS = -1,
	};

	/// action, tooltip
	typedef std::pair<FuncRequest, docstring> Item;

	/// the toolbar items
	typedef std::vector<Item> Items;

	/// toolbar flags
	enum Flags {
		ON = 1, //< always shown
		OFF = 2, //< never shown
		MATH = 4, //< shown when in math
		TABLE = 8, //< shown when in table
		TOP = 16, //< show at top
		BOTTOM = 32, //< show at bottom
		LEFT = 64, //< show at left
		RIGHT = 128, //< show at right
		REVIEW = 256, //< shown when change tracking is enabled
		AUTO = 512  //< only if AUTO is set, will MATH, TABLE and REIVEW is used
	};

	/// a toolbar
	struct Toolbar {
		/// toolbar name
		std::string name;
		/// toolbar GUI name
		std::string gui_name;
		/// toolbar contents
		Items items;
		/// flags
		Flags flags;
	};

	typedef std::vector<Toolbar> Toolbars;

	typedef Items::const_iterator item_iterator;

	ToolbarBackend();

	/// iterator for all toolbars
	Toolbars::const_iterator begin() const { return usedtoolbars.begin(); }

	Toolbars::const_iterator end() const { return usedtoolbars.end(); }

	Toolbars::iterator begin() { return usedtoolbars.begin(); }

	Toolbars::iterator end() { return usedtoolbars.end(); }

	/// read a toolbar from the file
	void read(LyXLex &);

	/// read the used toolbars
	void readToolbars(LyXLex &);

	/// return a full path of an XPM for the given action
	static std::string const getIcon(FuncRequest const &);

private:
	/// add the given lfun with tooltip if relevant
	void add(Toolbar & tb, FuncRequest const &,
		 docstring const & tooltip = docstring());

	/// all the toolbars
	Toolbars toolbars;

	/// toolbars listed
	Toolbars usedtoolbars;
};

/// The global instance
extern ToolbarBackend toolbarbackend;



} // namespace lyx

#endif // TOOLBAR_BACKEND_H
