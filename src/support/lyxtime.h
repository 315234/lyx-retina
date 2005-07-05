// -*- C++ -*-
/**
 * \file lyxtime.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author J�rgen Spitzm�ller
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYXTIME_H
#define LYXTIME_H

#include <time.h>
#include <string>


namespace lyx {

typedef time_t time_type;

time_type current_time();

/** Returns a locale-dependent formatting of the date
 * �and time encoded in \c time. The \p fmt string
 *  holds the formatting arguments of \c strftime.
 */
std::string const formatted_time(time_type t, std::string const & fmt);

/** Returns a locale-dependent formatting of the date
 * �and time encoded in \c time. For the formatting,
 *  \c lyxrc.date_insert_format is being used.
 */
std::string const formatted_time(time_type t);

}; // namespace lyx

#endif // LYXTIME_H
