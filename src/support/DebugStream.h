// -*- C++ -*-
/**
 * \file DebugStream.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef DEBUGSTREAM_H
#define DEBUGSTREAM_H

#include "LOstream.h"

#ifdef TEST_DEBUGSTREAM
#include <string>
struct Debug {
	enum type {
		NONE = 0,
		INFO       = (1 << 0),   // 1
		WARN       = (1 << 1),   // 2
		CRIT       = (1 << 2)   // 4
	};
	static const type ANY = type(INFO | WARN | CRIT);
	static Debug::type value(string const & val) {
		if (val == "NONE") return Debug::NONE;
		if (val == "INFO") return Debug::INFO;
		if (val == "WARN") return Debug::WARN;
		if (val == "CRIT") return Debug::CRIT;
		return Debug::NONE;
	}
};
#endif

/** DebugStream is a ostream intended for debug output.
    It has also support for a logfile. Debug output is output to cerr
    and if the logfile is set, to the logfile.

    Example of Usage:
    DebugStream debug;
    debug.level(Debug::INFO);
    debug.debug(Debug::WARN) << "WARN\n";
    debug[Debug::INFO] << "INFO\n";
    debug << "Always\n";

    Will output:
    INFO
    Always

    If you want to have debug output from time critical code you should
    use this construct:
    if (debug.debugging(Debug::INFO)) {
	 debug << "...debug output...\n";
    }

    To give debug info even if no debug (NONE) is requested:
    debug << "... always output ...\n";

    To give debug output regardless of what debug level is set (!NONE):
    debug.debug() << "...on debug output...\n";
    debug[Debug::ANY] << "...on debug output...\n";

    To give debug output when a specific debug level is set (INFO):
    debug.debug(Debug::INFO) << "...info...\n";
    debug[Debug::INFO] << "...info...\n";

    To give debug output when either on of debug levels is set (INFO or CRIT):
    debug.debug(Debug::type(Debug::INFO | Debug::CRIT)) << "...info/crit...\n";
    debug[Debug::type(Debug::INFO | Debug::CRIT)] << "...info/crit...\n";

*/
class DebugStream : public std::ostream
{
public:
	/// Constructor, sets the debug level to t.
	explicit DebugStream(Debug::type t = Debug::NONE);

	/// Constructor, sets the log file to f, and the debug level to t.
	explicit
	DebugStream(char const * f, Debug::type t = Debug::NONE);

	///
	~DebugStream();

	/// Sets the debug level to t.
	void level(Debug::type t) {
		dt = Debug::type(t & Debug::ANY);
	}

	/// Returns the current debug level.
	Debug::type level() const {
		return dt;
	}

	/// Adds t to the current debug level.
	void addLevel(Debug::type t) {
		dt = Debug::type(dt | t);
	}

	/// Deletes t from the current debug level.
	void delLevel(Debug::type t) {
		dt = Debug::type(dt & ~t);
	}

	/// Sets the debugstreams' logfile to f.
	void logFile(char const * f);

	/// Returns true if t is part of the current debug level.
	bool debugging(Debug::type t = Debug::ANY) const
	{
		if (dt & t) return true;
		return false;
	}


	/** Returns the no-op stream if t is not part of the
	    current debug level otherwise the real debug stream
	    is used.
	*/
	std::ostream & debug(Debug::type t = Debug::ANY) {
		if (dt & t) return *this;
		return nullstream;
	}


	/** This is an operator to give a more convenient use:
	    dbgstream[Debug::INFO] << "Info!\n";
	*/
	std::ostream & operator[](Debug::type t) {
		return debug(t);
	}
private:
	/// The current debug level
	Debug::type dt;
	/// The no-op stream.
	std::ostream nullstream;
	///
	struct debugstream_internal;
	///
	debugstream_internal * internal;
};

#endif
