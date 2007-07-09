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

#include "FuncRequest.h"

#include <vector>


namespace lyx {


class Lexer;

class ToolbarItem {
public:
	enum Type {
		/// command/action
		COMMAND,
		/// the command buffer
		MINIBUFFER,
		/// adds space between buttons in the toolbar
		SEPARATOR,
		/// a special combox insead of a button
		LAYOUTS,
		/// a special widget to insert tabulars
		TABLEINSERT,
		///
		POPUPMENU,
		///
		ICONPALETTE
	};

	ToolbarItem(Type type,
		 FuncRequest const & func,
		 docstring const & label = docstring());

	ToolbarItem(Type type,
		 std::string const & name = std::string(),
		 docstring const & label = docstring());

	~ToolbarItem();

	/// item type
	Type type_;
	/// action
	FuncRequest func_;
	/// label/tooltip
	docstring label_;
	/// name
	std::string name_;
};


///
class ToolbarInfo {
public:
	/// toolbar flags
	enum Flags {
		ON = 1, //< show
		OFF = 2, //< do not show
		MATH = 4, //< show when in math
		TABLE = 8, //< show when in table
		TOP = 16, //< show at top
		BOTTOM = 32, //< show at bottom
		LEFT = 64, //< show at left
		RIGHT = 128, //< show at right
		REVIEW = 256, //< show when change tracking is enabled
		AUTO = 512  //< only if AUTO is set, when MATH, TABLE and REVIEW is used
	};
	/// the toolbar items
	typedef std::vector<ToolbarItem> Items;

	typedef Items::const_iterator item_iterator;

	explicit ToolbarInfo(std::string const & name = std::string())
		: name(name) {}

	/// toolbar name
	std::string name;
	/// toolbar GUI name
	std::string gui_name;
	/// toolbar contents
	Items items;
	/// flags
	Flags flags;

	/// read a toolbar from the file
	ToolbarInfo & read(Lexer &);

private:
	/// add toolbar item
	void add(ToolbarItem const &);
};


///
class ToolbarBackend {
public:
	typedef std::vector<ToolbarInfo> Toolbars;

	ToolbarBackend();

	/// iterator for all toolbars
	Toolbars::const_iterator begin() const { return usedtoolbars.begin(); }

	Toolbars::const_iterator end() const { return usedtoolbars.end(); }

	Toolbars::iterator begin() { return usedtoolbars.begin(); }

	Toolbars::iterator end() { return usedtoolbars.end(); }

	/// read toolbars from the file
	void readToolbars(Lexer &);

	/// read ui toolbar settings
	void readToolbarSettings(Lexer &);

	///
	ToolbarInfo const * getDefinedToolbarInfo(std::string const & name) const;
	///
	ToolbarInfo * getUsedToolbarInfo(std::string const & name);

private:
	/// all the defined toolbars
	Toolbars toolbars;

	/// toolbars listed
	Toolbars usedtoolbars;
};

/// The global instance
extern ToolbarBackend toolbarbackend;



} // namespace lyx

#endif // TOOLBAR_BACKEND_H
