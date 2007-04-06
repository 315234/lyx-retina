// -*- C++ -*-
/**
 * \file lyxsocket.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author Jo�o Luis M. Assirati
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYXSOCKET_H
#define LYXSOCKET_H

#include "support/filename.h"
#include "support/socktools.h"
#include "lyxfunc.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <map>


namespace lyx {

class LyXServerSocket;
class LyXDataSocket;


/** Sockets can be in two states: listening and connected.
 *  Connected sockets are used to transfer data, and will therefore
 *  be called Data Sockets. Listening sockets are used to create
 *  Data Sockets when clients connect, and therefore will be called
 * Server Sockets.

 * This class encapsulates local (unix) server socket operations and
 * manages LyXDataSockets objects that are created when clients connect.
 */
class LyXServerSocket {
public:
	///
	LyXServerSocket(LyXFunc *, support::FileName const &);
	///
	~LyXServerSocket();
	/// Address of the local socket
	std::string const address() const;
	/// To be called when there is activity in the server socket
	void serverCallback();
	/// To be called when there is activity in the data socket
	void dataCallback(int fd);
private:
	///
	void writeln(std::string const &);
	///
	LyXFunc * func;
	/// File descriptor for the server socket
	int fd_;
	/// Stores the socket filename
	support::FileName address_;
	/// Maximum number of simultaneous clients
	enum {
		MAX_CLIENTS = 10
	};
	/// All connections
	std::map<int, boost::shared_ptr<LyXDataSocket> > clients;
};


/** This class encapsulates data socket operations.
 *  It provides read and write IO operations on the socket.
 */
class LyXDataSocket {
public:
	///
	LyXDataSocket(int fd);
	///
	~LyXDataSocket();
	/// Connection status
	bool connected() const;
	/// Line buffered input from the socket
	bool readln(std::string &);
	/// Write the string + '\n' to the socket
	void writeln(std::string const &);
private:
	/// File descriptor for the data socket
	int fd_;
	/// True if the connection is up
	bool connected_;
	/// buffer for input data
	std::string buffer_;
};

/// Implementation is in lyx_main.C
extern LyXServerSocket & theLyXServerSocket();


} // namespace lyx

#endif // LYXSOCKET_H
