/**
 * \file Alert_pimpl.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS
 */

#include "debug.h"

// GUI-specific implementations

void alert_pimpl(string const & s1, string const & s2, string const & s3);

int prompt_pimpl(string const & title, string const & question,
           int default_button,
	   string const & b1, string const & b2, string const & b3);

std::pair<bool, string> const askForText_pimpl(string const & msg, string const & dflt);
