// -*- C++ -*-
/**
 * \file LyXAction.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYXACTION_H
#define LYXACTION_H

#include "lfuns.h"

#include <boost/utility.hpp>

#include <map>
#include <string>

class FuncRequest;

/**
 * This class is a container for LyX actions. It also
 * stores and managers "pseudo-actions". Pseudo-actions
 * are not part of the kb_action enum, but are created
 * dynamically, for encapsulating a real action and an
 * argument. They are used for things like the menus.
 */
class LyXAction : boost::noncopyable {
private:
	/// information for an action
	struct func_info {
		/// the action name
		std::string name;
		/// the func_attrib values set
		unsigned int attrib;
	};

public:
	/// type for map between a function name and its action
	typedef std::map<std::string, kb_action> func_map;
	/// type for map between an action and its info
	typedef std::map<kb_action, func_info> info_map;

	/// possible "permissions" for an action
	enum func_attrib {
		Noop = 0, //< nothing special about this func
		ReadOnly = 1, //< can be used in RO mode (perhaps this should change)
		NoBuffer = 2, //< Can be used when there is no document open
		Argument = 4 //< Requires argument
	};

	LyXAction();

	/**
	 * Returns an pseudoaction from a string
	 * If you include arguments in func_name, a new pseudoaction
	 * will be created if needed.
	 */
	FuncRequest lookupFunc(std::string const & func_name) const;

	/// Return the name (and argument) associated with the given (pseudo) action
	std::string const getActionName(kb_action action) const;

	/// True if the command has `flag' set
	bool funcHasFlag(kb_action action, func_attrib flag) const;

	/// iterator across all real actions
	typedef func_map::const_iterator const_func_iterator;

	/// return an iterator to the start of all real actions
	const_func_iterator func_begin() const;

	/// return an iterator to the end of all real actions
	const_func_iterator func_end() const;

private:
	/// populate the action container with our actions
	void init();
	/// add the given action
	void newFunc(kb_action, std::string const & name, unsigned int attrib);

	/**
	 * This is a list of all the LyXFunc names with the
	 * coresponding action number. It is usually only used by the
	 * minibuffer or when assigning commands to keys during init.
	 */
	func_map lyx_func_map;

	/**
	 * This is a mapping from action number to an object holding
	 * info about this action. f.ex. command name (string),
	 * command attributes (ro)
	 */
	info_map lyx_info_map;
};

/// singleton instance
extern LyXAction lyxaction;

#endif // LYXACTION_H
