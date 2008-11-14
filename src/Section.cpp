/**
 * \file Section.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Section.h"

using namespace std;

namespace lyx {


string const & Section::name() const
{
	return name_;
}


int Section::level() const
{
	return level_;
}


string const & Section::indent() const
{
	return indent_;
}


string const & Section::beforeskip() const
{
	return beforeskip_;
}


string const & Section::afterskip() const
{
	return afterskip_;
}


Font const & Section::style() const
{
	return style_;
}


bool Section::display() const
{
	// If afterskip is negative it is a display section.
	if (!afterskip_.empty() && afterskip_[0] == '-')
		return false;
	return true;
}


} // namespace lyx
