// -*- C++ -*-
/* This file is part of
 * ======================================================
 *
 *           LyX, The Document Processor
 *
 *           Copyright 2000-2001 The LyX Team.
 *
 *           @author: J�rgen Vigna
 *
 * ======================================================
 */

#ifndef TABULAR_FUNCS_H
#define TABULAR_FUNCS_H

#include "tabular.h"
#include "support/tostr.h"

#include <iosfwd>

// Perfect case for a template... (Lgb)
// or perhaps not...
template<class T>
string const write_attribute(string const & name, T const & t)
{
	if (tostr(t).empty())
		return string();

	string str = " " + name + "=\"" + tostr(t) + "\"";
	return str;
}
template<>
string const write_attribute(string const & name, bool const & b);
template<>
string const write_attribute(string const & name, int const & b);
template<>
string const write_attribute(string const & name, LyXLength const & value);
string const tostr(LyXAlignment const & num);
string const tostr(LyXTabular::VAlignment const & num);
string const tostr(LyXTabular::BoxType const & num);

// I would have liked a fromstr template a lot better. (Lgb)
bool string2type(string const str, LyXAlignment & num);
bool string2type(string const str, LyXTabular::VAlignment & num);
bool string2type(string const str, LyXTabular::BoxType & num);
bool string2type(string const str, bool & num);
bool getTokenValue(string const & str, char const * token, string &ret);
bool getTokenValue(string const & str, char const * token, int & num);
bool getTokenValue(string const & str, char const * token,
			  LyXAlignment & num);
bool getTokenValue(string const & str, char const * token,
			  LyXTabular::VAlignment & num);
bool getTokenValue(string const & str, char const * token,
			  LyXTabular::BoxType & num);
bool getTokenValue(string const & str, char const * token, bool & flag);
bool getTokenValue(string const & str, char const * token,
			  LyXLength & len);
void l_getline(std::istream & is, string & str);

#endif
