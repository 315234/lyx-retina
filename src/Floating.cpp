/**
 * \file Floating.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Floating.h"

using namespace std;


namespace lyx {


Floating::Floating()
{}


Floating::Floating(string const & type, string const & placement,
		   string const & ext, string const & within,
		   string const & style, string const & name,
		   string const & listName, string const & htmlTag,
		   string const & htmlClass, string const & htmlStyle,
		   bool builtin)
	: type_(type), placement_(placement), ext_(ext), within_(within),
	  style_(style), name_(name), listName_(listName), htmlTag_(htmlTag),
	  htmlClass_(htmlClass), htmlStyle_(htmlStyle), builtin_(builtin)
{}


string const & Floating::type() const
{
	return type_;
}


string const & Floating::placement() const
{
	return placement_;
}


string const & Floating::ext() const
{
	return ext_;
}


string const & Floating::within() const
{
	return within_;
}


string const & Floating::style() const
{
	return style_;
}


string const & Floating::name() const
{
	return name_;
}


string const & Floating::listName() const
{
	return listName_;
}


string const & Floating::htmlStyle() const
{
	return htmlStyle_;
}


string const & Floating::htmlClass() const
{
	if (htmlClass_.empty())
		htmlClass_ = "float-" + type_;
	return htmlClass_;
}


string const & Floating::htmlTag() const
{
	if (htmlTag_.empty())
		htmlTag_ = "div";
	return htmlTag_;
}


bool Floating::builtin() const
{
	return builtin_;
}


} // namespace lyx
