/**
 * \file FuncRequest.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "FuncRequest.h"

#include <iostream>
#include <sstream>
#include <vector>


namespace lyx {

using std::getline;
using std::istringstream;
using std::vector;
using std::string;


FuncRequest::FuncRequest(Origin o)
	: action(LFUN_NOACTION), origin(o), x(0), y(0),
	  button_(mouse_button::none)
{}


FuncRequest::FuncRequest(kb_action act, Origin o)
	: action(act), origin(o), x(0), y(0), button_(mouse_button::none)
{}


FuncRequest::FuncRequest(kb_action act, docstring const & arg, Origin o)
	: action(act), argument_(arg), origin(o), x(0), y(0),
	  button_(mouse_button::none)
{}


FuncRequest::FuncRequest(kb_action act, string const & arg, Origin o)
	: action(act), argument_(from_utf8(arg)), origin(o), x(0), y(0),
	  button_(mouse_button::none)
{}


FuncRequest::FuncRequest(kb_action act, int ax, int ay,
			 mouse_button::state but, Origin o)
	: action(act), origin(o), x(ax), y(ay), button_(but)
{}


FuncRequest::FuncRequest(FuncRequest const & cmd, docstring const & arg, Origin o)
	: action(cmd.action), argument_(arg), origin(o),
	  x(cmd.x), y(cmd.y), button_(cmd.button_)
{}


FuncRequest::FuncRequest(FuncRequest const & cmd, string const & arg, Origin o)
	: action(cmd.action), argument_(from_utf8(arg)), origin(o),
	  x(cmd.x), y(cmd.y), button_(cmd.button_)
{}


mouse_button::state FuncRequest::button() const
{
	return button_;
}


void split(vector<string> & args, string const & str)
{
	istringstream is(str);
	while (is) {
		char c;
		string s;
		is >> c;
		if (is) {
			if (c == '"')
				getline(is, s, '"');
			else {
				is.putback(c);
				is >> s;
			}
			args.push_back(s);
		}
	}
}


string FuncRequest::getArg(unsigned int i) const
{
	vector<string> args;
	split(args, to_utf8(argument_));
	return i < args.size() ? args[i] : string();
}


bool operator==(FuncRequest const & lhs, FuncRequest const & rhs)
{
	return lhs.action == rhs.action && lhs.argument() == rhs.argument();
}


std::ostream & operator<<(std::ostream & os, FuncRequest const & cmd)
{
	return os
		<< " action: " << cmd.action
		<< " arg: '" << to_utf8(cmd.argument()) << "'"
		<< " x: " << cmd.x
		<< " y: " << cmd.y;
}


} // namespace lyx
