// -*- C++ -*-
/**
 * \file lengthcommon.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LENGTH_COMMON_H
#define LENGTH_COMMON_H

#include "lyxlength.h"


/// the number of units possible
extern int const num_units;

/**
 * array of unit names
 *
 * FIXME: I am not sure if "mu" should be possible to select (Lgb)
 */
extern char const * const unit_name[];
extern char const * const unit_name_gui[];

/// return the unit given a string representation such as "cm"
LyXLength::UNIT unitFromString(std::string const & data);


#endif // LENGTH_COMMON_H
