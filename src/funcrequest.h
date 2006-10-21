// -*- C++ -*-
/**
 * \file funcrequest.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef FUNCREQUEST_H
#define FUNCREQUEST_H

#include "lfuns.h"
#include "frontends/mouse_state.h"

#include "support/docstring.h"

#include <iosfwd>


namespace lyx {


/**
 * This class encapsulates a LyX action and its argument
 * in order to pass it around easily.
 */
class FuncRequest {
public:
	/// Where the request came from
	enum Origin {
		INTERNAL,
		UI, // The menu or the toolbar
		KEYBOARD, // a keyboard binding
		COMMANDBUFFER
	};

	/// just for putting these things in std::container
	explicit FuncRequest(Origin o = INTERNAL);
	/// actions without extra argument
	explicit FuncRequest(kb_action act, Origin o = INTERNAL);
	/// actions without extra argument
	FuncRequest(kb_action act, int x, int y, mouse_button::state button,
		    Origin o = INTERNAL);
	/// actions with extra argument
	FuncRequest(kb_action act, docstring const & arg,
		    Origin o = INTERNAL);
	/// actions with extra argument. FIXME: remove this
	FuncRequest(kb_action act, std::string const & arg,
		    Origin o = INTERNAL);
	/// for changing requests a bit
	FuncRequest(FuncRequest const & cmd, docstring const & arg,
		    Origin o = INTERNAL);
	/// for changing requests a bit. FIXME: remove this
	FuncRequest(FuncRequest const & cmd, std::string const & arg,
		    Origin o = INTERNAL);

	/// access to button
	mouse_button::state button() const;

	/// argument parsing, extract argument i as std::string
	std::string getArg(unsigned int i) const;

	/// access the whole argument
	docstring const & argument() const { return argument_; }

public:  // should be private
	/// the action
	kb_action action;
private:
	/// the action's string argument
	docstring argument_;
public:  // should be private
	/// who initiated the action
	Origin origin;
	/// the x coordinate of a mouse press
	int x;
	/// the y coordinate of a mouse press
	int y;
	/// some extra information (like button number)
	mouse_button::state button_;
};


bool operator==(FuncRequest const & lhs, FuncRequest const & rhs);

std::ostream & operator<<(std::ostream &, FuncRequest const &);


} // namespace lyx

#endif // FUNCREQUEST_H
