// -*- C++ -*-
/**
 * \file gnome/Timeout_pimpl.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Baruch Even
 *
 * Full author contact details are available in file CREDITS
 */
#ifndef TIMEOUTPIMPL_H
#define TIMEOUTPIMPL_H

#include "frontends/Timeout.h"
#include "glib.h" // for gint

#include <sigc++/sigc++.h>


/**
 * This class executes the callback when the timeout expires
 * using Gnome mechanisms
 */
struct Timeout::Pimpl : public SigC::Object {
public:
	///
	Pimpl(Timeout * owner_);
	/// start the timer
	void start();
	/// stop the timer
	void stop();
	/// reset
	void reset();
	/// Is the timer running?
	bool running() const;


public:
	/// The timeout signal, this gets called when the timeout passed.
	bool timeoutEvent();
private:
	/// the owning timer
	Timeout * owner_;
	/// Timer connection
	SigC::Connection conn_;
	/// Used for running as SigC::Connection::connected() isn't const
	bool running_;
};

#endif
