// -*- C++ -*-
/**
 * \file userinfo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef USERINFO_H
#define USERINFO_H

#include "support/std_string.h"

namespace lyx {
namespace support {

/// return the current user's real name
string const user_name();

/// return the current user's e-mail address
string const user_email();

} // namespace support
} // namespace lyx

#endif // USERINFO_H
