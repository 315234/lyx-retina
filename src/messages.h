// -*- C++ -*-
/* \file messages.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MESSAGES_H
#define MESSAGES_H

#include "support/docstring.h"

#include <boost/scoped_ptr.hpp>
#include <string>

///
class Messages {
public:
	/// messages in the language defined by the environment
	Messages();
	/// messages in the language \p l
	Messages(std::string const & l);
	///
	~Messages();
	///
	lyx::docstring const get(std::string const & msg) const;
private:
	class Pimpl;
	boost::scoped_ptr<Pimpl> pimpl_;
};

#endif
