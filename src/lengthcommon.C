/**
 * \file lengthcommon.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "lengthcommon.h"

#include "gettext.h"


namespace lyx {

using std::string;

// I am not sure if "mu" should be possible to select (Lgb)

// the latex units
char const * const unit_name[] = {
	"sp", "pt", "bp", "dd", "mm", "pc",
	"cc", "cm", "in", "ex", "em", "mu",
	"text%",  "col%", "page%", "line%",
	"theight%", "pheight%", "" };

int const num_units = int(sizeof(unit_name) / sizeof(unit_name[0]) - 1);

// the LyX gui units
char const * const unit_name_gui[] = {
	N_("sp"), N_("pt"), N_("bp"), N_("dd"), N_("mm"), N_("pc"),
	N_("cc"), N_("cm"), N_("in"), N_("ex"), N_("em"), N_("mu"),
	N_("Text Width %"), N_("Column Width %"), N_("Page Width %"), N_("Line Width %"),
	N_("Text Height %"), N_("Page Height %"), "" };

LyXLength::UNIT unitFromString(string const & data)
{
	int i = 0;
	while (i < num_units && data != unit_name[i])
		++i;
	return static_cast<LyXLength::UNIT>(i);
}


} // namespace lyx
