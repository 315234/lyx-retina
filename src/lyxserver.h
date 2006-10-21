// -*- C++ -*-
/**
 * \file lyxserver.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYXSERVER_H
#define LYXSERVER_H

#include <boost/signals/trackable.hpp>


namespace lyx {

class LyXFunc;
class LyXServer;

/* --- i/o pipes --------------------------------------------------------- */

/** This class managed the pipes used for communicating with clients.
 Usage: Initialize with pipe-filename-base, client class to receive
 messages, and callback-function that will be called with the messages.
 When you want to send, use "send()".
 This class encapsulates all the dirty communication and thus provides
 a clean string interface.
 */
class LyXComm : public boost::signals::trackable {
public:
	/** When we receive a message, we send it to a client.
	  This is one of the small things that would have been a lot
	  cleaner with a Signal/Slot thing.
	 */
	typedef void (*ClientCallbackfct)(LyXServer *, std::string const &);

	/// Construct with pipe-basename and callback to receive messages
	LyXComm(std::string const & pip, LyXServer * cli, ClientCallbackfct ccb = 0)
		: pipename(pip), client(cli), clientcb(ccb) {
		ready = false;
		openConnection();
	}

	///
	~LyXComm() {
		closeConnection();
	}

	/// clean up in emergency
	void emergencyCleanup();

	/// Send message
	void send(std::string const &);

	/// asynch ready-to-be-read notification
	void read_ready();

private:
	/// the filename of the in pipe
	std::string const inPipeName() const;

	/// the filename of the out pipe
	std::string const outPipeName() const;

	/// Open pipes
	void openConnection();

	/// Close pipes
	void closeConnection();

	/// start a pipe
	int startPipe(std::string const &, bool);

	/// finish a pipe
	void endPipe(int &, std::string const &, bool);

	/// This is -1 if not open
	int infd;

	/// This is -1 if not open
	int outfd;

	/// Are we up and running?
	bool ready;

	/// Base of pipename including path
	std::string pipename;

	/// The client
	LyXServer * client;

	/// The client callback function
	ClientCallbackfct clientcb;
};


/* --- prototypes -------------------------------------------------------- */
///
class LyXServer {
public:
	// FIXME IN 0.13
	// Hack! This should be changed in 0.13

	// The lyx server should not take an argument "LyXFunc" but this is
	// how it will be done for 0.12. In 0.13 we must write a non-gui
	// bufferview.
	// IMO lyxserver is atypical, and for the moment the only one, non-gui
	// bufferview. We just have to find a way to handle situations like if
	// lyxserver is using a buffer that is being edited with a bufferview.
	// With a common buffer list this is not a problem, maybe. (Alejandro)
	///
	LyXServer(LyXFunc * f, std::string const & pip)
		: numclients(0), func(f), pipes(pip, (this), callback) {}
	///
	~LyXServer();
	///
	void notifyClient(std::string const &);

	/// whilst crashing etc.
	void emergencyCleanup() {
		pipes.emergencyCleanup();
	}

private:
	///
	static void callback(LyXServer *, std::string const & msg);
	/// Names and number of current clients
	enum {
		///
		MAX_CLIENTS = 10
	};
	///
	std::string clients[MAX_CLIENTS];
	///
	int numclients;
	///
	LyXFunc * func;
	///
	LyXComm pipes;
};

/// Implementation is in lyx_main.C
extern LyXServer & theLyXServer();

#endif /* _LYXSERVER_H_ */


} // namespace lyx

/* === End of File: lyxserver.h ========================================== */
