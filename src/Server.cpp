/**
 * \file Server.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 * \author John Levon
 * \author Enrico Forestieri
 *
 * Full author contact details are available in file CREDITS.
 */

/**
  Docu   : To use the lyxserver define the name of the pipe in your
	   lyxrc:
	   \serverpipe "/home/myhome/.lyxpipe"
	   Then use .lyxpipe.in and .lyxpipe.out to communicate to LyX.
	   Each message consists of a single line in ASCII. Input lines
	   (client -> LyX) have the following format:
	    "LYXCMD:<clientname>:<functionname>:<argument>"
	   Answers from LyX look like this:
	   "INFO:<clientname>:<functionname>:<data>"
 [asierra970531] Or like this in case of error:
	   "ERROR:<clientname>:<functionname>:<error message>"
	   where <clientname> and <functionname> are just echoed.
	   If LyX notifies about a user defined extension key-sequence,
	   the line looks like this:
	   "NOTIFY:<key-sequence>"
 [asierra970531] New server-only messages to implement a simple protocol
	   "LYXSRV:<clientname>:<protocol message>"
	   where <protocol message> can be "hello" or "bye". If hello is
	   received LyX will inform the client that it's listening its
	   messages, and 'bye' will inform that lyx is closing.

	   See development/server_monitor.c for an example client.
  Purpose: implement a client/server lib for LyX
*/

#include <config.h>

#include "Server.h"
#include "FuncRequest.h"
#include "LyXAction.h"
#include "LyXFunc.h"

#include "frontends/Application.h"

#include "support/debug.h"
#include "support/FileName.h"
#include "support/lstrings.h"
#include "support/os.h"

#include <boost/bind.hpp>

#ifdef _WIN32
#include <QCoreApplication>
#endif

#include <cerrno>
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#include <fcntl.h>

using namespace std;
using namespace lyx::support;
using os::external_path;

namespace lyx {

/////////////////////////////////////////////////////////////////////
//
// LyXComm
//
/////////////////////////////////////////////////////////////////////

#if defined(_WIN32)

class PipeEvent : public QEvent {
public:
	///
	PipeEvent(HANDLE inpipe) : QEvent(QEvent::User), inpipe_(inpipe)
	{}
	///
	HANDLE pipe() const { return inpipe_; }

private:
	HANDLE inpipe_;
};

namespace {

bool closing_ = false;

void closing()
{
	closing_ = true;
}


char * errormsg()
{
	void * msgbuf;
	DWORD error = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		      FORMAT_MESSAGE_FROM_SYSTEM |
		      FORMAT_MESSAGE_IGNORE_INSERTS,
		      NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      (LPTSTR) &msgbuf, 0, NULL);
	return static_cast<char *>(msgbuf);
}


extern "C" {

DWORD WINAPI pipeServerWrapper(void * arg)
{
	LyXComm * lyxcomm = reinterpret_cast<LyXComm *>(arg);
	lyxcomm->pipeServer();
	return 1;
}

}

} // namespace anon

LyXComm::LyXComm(string const & pip, Server * cli, ClientCallbackfct ccb)
	: pipename_(pip), client_(cli), clientcb_(ccb)
{
	// Ask Qt to notify us on quit.
	qAddPostRoutine(closing);
	ready_ = false;
	openConnection();
}


void LyXComm::pipeServer()
{
	int const bufsize = 1024;
	HANDLE inpipe;

	outpipe_ = CreateNamedPipe(external_path(outPipeName()).c_str(),
				   PIPE_ACCESS_OUTBOUND, PIPE_NOWAIT,
				   PIPE_UNLIMITED_INSTANCES,
				   bufsize, 0, 0, NULL);
	if (outpipe_ == INVALID_HANDLE_VALUE) {
		lyxerr << "LyXComm: Could not create pipe "
		       << outPipeName() << '\n' << errormsg() << endl;
		return;
	}
	ConnectNamedPipe(outpipe_, NULL);
	// Now change to blocking mode
	DWORD mode = PIPE_WAIT;
	SetNamedPipeHandleState(outpipe_, &mode, NULL, NULL);

	while (!checkStopServerEvent()) {
		inpipe = CreateNamedPipe(external_path(inPipeName()).c_str(),
					 PIPE_ACCESS_INBOUND, PIPE_WAIT,
					 PIPE_UNLIMITED_INSTANCES,
					 0, bufsize, 0, NULL);
		if (inpipe == INVALID_HANDLE_VALUE) {
			lyxerr << "LyXComm: Could not create pipe "
			       << inPipeName() << '\n' << errormsg() << endl;
			break;
		}

		BOOL connected = ConnectNamedPipe(inpipe, NULL);
		// Check whether we are signaled to shutdown the pipe server.
		if (checkStopServerEvent()) {
			CloseHandle(inpipe);
			break;
		}
		if (connected || GetLastError() == ERROR_PIPE_CONNECTED) {
			PipeEvent * event = new PipeEvent(inpipe);
			QCoreApplication::postEvent(this,
					static_cast<QEvent *>(event));
		} else
			CloseHandle(inpipe);
	}
	CloseHandle(outpipe_);
}


bool LyXComm::event(QEvent * e)
{
	if (e->type() == QEvent::User) {
		read_ready(static_cast<PipeEvent *>(e)->pipe());
		return true;
	}
	return false;
}


BOOL LyXComm::checkStopServerEvent()
{
	return WaitForSingleObject(stopserver_, 0) == WAIT_OBJECT_0 || closing_;
}


void LyXComm::openConnection()
{
	LYXERR(Debug::LYXSERVER, "LyXComm: Opening connection");

	// If we are up, that's an error
	if (ready_) {
		lyxerr << "LyXComm: Already connected" << endl;
		return;
	}
	// We assume that we don't make it
	ready_ = false;

	if (pipename_.empty()) {
		LYXERR(Debug::LYXSERVER, "LyXComm: server is disabled, nothing to do");
		return;
	}

	stopserver_ = CreateEvent(NULL, TRUE, FALSE, NULL);
	DWORD tid;
	HANDLE thread = CreateThread(NULL, 0, pipeServerWrapper,
				     static_cast<void *>(this), 0, &tid);
	if (!thread) {
		lyxerr << "LyXComm: Could not create pipe server thread: "
		       << errormsg() << endl;
		return;
	} else
		CloseHandle(thread);

	// We made it!
	ready_ = true;
	LYXERR(Debug::LYXSERVER, "LyXComm: Connection established");
}


/// Close pipes
void LyXComm::closeConnection()
{
	LYXERR(Debug::LYXSERVER, "LyXComm: Closing connection");

	if (pipename_.empty()) {
		LYXERR(Debug::LYXSERVER, "LyXComm: server is disabled, nothing to do");
		return;
	}

	if (!ready_) {
		LYXERR0("LyXComm: Already disconnected");
		return;
	}

	SetEvent(stopserver_);
	HANDLE hpipe = CreateFile(external_path(inPipeName()).c_str(),
				  GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
				  FILE_ATTRIBUTE_NORMAL, NULL);
	if (hpipe != INVALID_HANDLE_VALUE)
		CloseHandle(hpipe);

	ResetEvent(stopserver_);
	CloseHandle(stopserver_);
	ready_ = false;
}


void LyXComm::emergencyCleanup()
{
	if (!pipename_.empty()) {
		SetEvent(stopserver_);
		HANDLE hpipe = CreateFile(external_path(inPipeName()).c_str(),
					  GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL, NULL);
		if (hpipe != INVALID_HANDLE_VALUE)
			CloseHandle(hpipe);

		ResetEvent(stopserver_);
		CloseHandle(stopserver_);
	}
}

void LyXComm::read_ready(HANDLE inpipe)
{
	string read_buffer_;
	read_buffer_.erase();

	int const charbuf_size = 100;
	char charbuf[charbuf_size];

	DWORD status;
	while (ReadFile(inpipe, charbuf, charbuf_size - 1, &status, NULL)) {
		if (status > 0) {
			charbuf[status] = '\0'; // turn it into a c string
			read_buffer_ += rtrim(charbuf, "\r");
			// commit any commands read
			while (read_buffer_.find('\n') != string::npos) {
				// split() grabs the entire string if
				// the delim /wasn't/ found. ?:-P
				string cmd;
				read_buffer_= split(read_buffer_, cmd, '\n');
				cmd = rtrim(cmd, "\r");
				LYXERR(Debug::LYXSERVER, "LyXComm: status:" << status
					<< ", read_buffer_:" << read_buffer_
					<< ", cmd:" << cmd);
				if (!cmd.empty())
					clientcb_(client_, cmd);
					//\n or not \n?
			}
		}
	}
	if (GetLastError() != ERROR_BROKEN_PIPE) {
		// An error occurred
		LYXERR0("LyXComm: " << errormsg());
		if (!read_buffer_.empty()) {
			LYXERR0("LyXComm: truncated command: " << read_buffer_);
			read_buffer_.erase();
		}
	}
	// Client closed the pipe, so disconnect and close.
	DisconnectNamedPipe(inpipe);
	CloseHandle(inpipe);
	FlushFileBuffers(outpipe_);
	DisconnectNamedPipe(outpipe_);
	// Temporarily change to non-blocking mode otherwise
	// ConnectNamedPipe() would block waiting for a connection.
	DWORD mode = PIPE_NOWAIT;
	SetNamedPipeHandleState(outpipe_, &mode, NULL, NULL);
	ConnectNamedPipe(outpipe_, NULL);
	mode = PIPE_WAIT;
	SetNamedPipeHandleState(outpipe_, &mode, NULL, NULL);
}


void LyXComm::send(string const & msg)
{
	if (msg.empty()) {
		LYXERR0("LyXComm: Request to send empty string. Ignoring.");
		return;
	}

	LYXERR(Debug::LYXSERVER, "LyXComm: Sending '" << msg << '\'');

	if (pipename_.empty()) return;

	if (!ready_) {
		LYXERR0("LyXComm: Pipes are closed. Could not send " << msg);
		return;
	}

	bool success;
	int count = 0;
	do {
		DWORD sent;
		success = WriteFile(outpipe_, msg.c_str(), msg.length(), &sent, NULL);
		if (!success) {
			DWORD error = GetLastError();
			if (error == ERROR_NO_DATA) {
				DisconnectNamedPipe(outpipe_);
				DWORD mode = PIPE_NOWAIT;
				SetNamedPipeHandleState(outpipe_, &mode, NULL, NULL);
				ConnectNamedPipe(outpipe_, NULL);
				mode = PIPE_WAIT;
				SetNamedPipeHandleState(outpipe_, &mode, NULL, NULL);
			} else if (error != ERROR_PIPE_LISTENING)
				break;
		}
		Sleep(100);
		++count;
	} while (!success && count < 100 && !checkStopServerEvent());

	if (!success) {
		lyxerr << "LyXComm: Error sending message: " << msg
		       << '\n' << errormsg()
		       << "\nLyXComm: Resetting connection" << endl;
		closeConnection();
		if (!checkStopServerEvent())
			openConnection();
	}
}


#elif !defined (HAVE_MKFIFO)
// We provide a stub class that disables the lyxserver.

LyXComm::LyXComm(string const &, Server *, ClientCallbackfct)
{}

void LyXComm::openConnection()
{}


void LyXComm::closeConnection()
{}


int LyXComm::startPipe(string const & filename, bool write)
{
	return -1;
}


void LyXComm::endPipe(int & fd, string const & filename, bool write)
{}


void LyXComm::emergencyCleanup()
{}

void LyXComm::read_ready()
{}


void LyXComm::send(string const & msg)
{}


#else // defined (HAVE_MKFIFO)


LyXComm::LyXComm(string const & pip, Server * cli, ClientCallbackfct ccb)
	: pipename_(pip), client_(cli), clientcb_(ccb)
{
	ready_ = false;
	openConnection();
}


void LyXComm::openConnection()
{
	LYXERR(Debug::LYXSERVER, "LyXComm: Opening connection");

	// If we are up, that's an error
	if (ready_) {
		lyxerr << "LyXComm: Already connected" << endl;
		return;
	}
	// We assume that we don't make it
	ready_ = false;

	if (pipename_.empty()) {
		LYXERR(Debug::LYXSERVER, "LyXComm: server is disabled, nothing to do");
		return;
	}

	infd_ = startPipe(inPipeName(), false);
	if (infd_ == -1)
		return;

	outfd_ = startPipe(outPipeName(), true);
	if (outfd_ == -1) {
		endPipe(infd_, inPipeName(), false);
		return;
	}

	if (fcntl(outfd_, F_SETFL, O_NONBLOCK) < 0) {
		lyxerr << "LyXComm: Could not set flags on pipe " << outPipeName()
		       << '\n' << strerror(errno) << endl;
		return;
	}

	// We made it!
	ready_ = true;
	LYXERR(Debug::LYXSERVER, "LyXComm: Connection established");
}


/// Close pipes
void LyXComm::closeConnection()
{
	LYXERR(Debug::LYXSERVER, "LyXComm: Closing connection");

	if (pipename_.empty()) {
		LYXERR(Debug::LYXSERVER, "LyXComm: server is disabled, nothing to do");
		return;
	}

	if (!ready_) {
		LYXERR0("LyXComm: Already disconnected");
		return;
	}

	endPipe(infd_, inPipeName(), false);
	endPipe(outfd_, outPipeName(), true);

	ready_ = false;
}


int LyXComm::startPipe(string const & file, bool write)
{
	static bool stalepipe = false;
	FileName const filename(file);
	if (filename.exists()) {
		if (!write) {
			// Let's see whether we have a stale pipe.
			int fd = ::open(filename.toFilesystemEncoding().c_str(),
					O_WRONLY | O_NONBLOCK);
			if (fd >= 0) {
				// Another LyX instance is using it.
				::close(fd);
			} else if (errno == ENXIO) {
				// No process is reading from the other end.
				stalepipe = true;
				LYXERR(Debug::LYXSERVER,
					"LyXComm: trying to remove "
					<< filename);
				filename.removeFile();
			}
		} else if (stalepipe) {
			LYXERR(Debug::LYXSERVER, "LyXComm: trying to remove "
				<< filename);
			filename.removeFile();
			stalepipe = false;
		}
		if (filename.exists()) {
			lyxerr << "LyXComm: Pipe " << filename
			       << " already exists.\nIf no other LyX program"
			          " is active, please delete the pipe by hand"
				  " and try again."
			       << endl;
			pipename_.erase();
			return -1;
		}
	}

	if (::mkfifo(filename.toFilesystemEncoding().c_str(), 0600) < 0) {
		lyxerr << "LyXComm: Could not create pipe " << filename << '\n'
		       << strerror(errno) << endl;
		return -1;
	}
	int const fd = ::open(filename.toFilesystemEncoding().c_str(),
			      write ? (O_RDWR) : (O_RDONLY|O_NONBLOCK));

	if (fd < 0) {
		lyxerr << "LyXComm: Could not open pipe " << filename << '\n'
		       << strerror(errno) << endl;
		filename.removeFile();
		return -1;
	}

	if (!write) {
		theApp()->registerSocketCallback(fd,
			boost::bind(&LyXComm::read_ready, this));
	}

	return fd;
}


void LyXComm::endPipe(int & fd, string const & filename, bool write)
{
	if (fd < 0)
		return;

	if (!write)
		theApp()->unregisterSocketCallback(fd);

	if (::close(fd) < 0) {
		lyxerr << "LyXComm: Could not close pipe " << filename
		       << '\n' << strerror(errno) << endl;
	}

	if (FileName(filename).removeFile() < 0) {
		lyxerr << "LyXComm: Could not remove pipe " << filename
		       << '\n' << strerror(errno) << endl;
	}

	fd = -1;
}


void LyXComm::emergencyCleanup()
{
	if (!pipename_.empty()) {
		endPipe(infd_, inPipeName(), false);
		endPipe(outfd_, outPipeName(), true);
	}
}


// Receives messages and sends then to client
void LyXComm::read_ready()
{
	// FIXME: make read_buffer_ a class-member for multiple sessions
	static string read_buffer_;
	read_buffer_.erase();

	int const charbuf_size = 100;
	char charbuf[charbuf_size];

	// As O_NONBLOCK is set, until no data is available for reading,
	// read() doesn't block but returns -1 and set errno to EAGAIN.
	// After a client that opened the pipe for writing, closes it
	// (and no other client is using the pipe), read() would always
	// return 0 and thus the connection has to be reset.

	errno = 0;
	int status;
	// the single = is intended here.
	while ((status = ::read(infd_, charbuf, charbuf_size - 1))) {

		if (status > 0) {
			charbuf[status] = '\0'; // turn it into a c string
			read_buffer_ += rtrim(charbuf, "\r");
			// commit any commands read
			while (read_buffer_.find('\n') != string::npos) {
				// split() grabs the entire string if
				// the delim /wasn't/ found. ?:-P
				string cmd;
				read_buffer_= split(read_buffer_, cmd,'\n');
				LYXERR(Debug::LYXSERVER, "LyXComm: status:" << status
					<< ", read_buffer_:" << read_buffer_
					<< ", cmd:" << cmd);
				if (!cmd.empty())
					clientcb_(client_, cmd);
					//\n or not \n?
			}
		} else {
			if (errno == EAGAIN) {
				// Nothing to read, continue
				errno = 0;
				return;
			}
			// An error occurred, better bailing out
			LYXERR0("LyXComm: " << strerror(errno));
			if (!read_buffer_.empty()) {
				LYXERR0("LyXComm: truncated command: " << read_buffer_);
				read_buffer_.erase();
			}
			break; // reset connection
		}
	}

	// The connection gets reset when read() returns 0 (meaning that the
	// last client closed the pipe) or an error occurred, in which case
	// read() returns -1 and errno != EAGAIN.
	closeConnection();
	openConnection();
	errno = 0;
}


void LyXComm::send(string const & msg)
{
	if (msg.empty()) {
		LYXERR0("LyXComm: Request to send empty string. Ignoring.");
		return;
	}

	LYXERR(Debug::LYXSERVER, "LyXComm: Sending '" << msg << '\'');

	if (pipename_.empty()) return;

	if (!ready_) {
		LYXERR0("LyXComm: Pipes are closed. Could not send " << msg);
	} else if (::write(outfd_, msg.c_str(), msg.length()) < 0) {
		lyxerr << "LyXComm: Error sending message: " << msg
		       << '\n' << strerror(errno)
		       << "\nLyXComm: Resetting connection" << endl;
		closeConnection();
		openConnection();
	}
}

#endif // defined (HAVE_MKFIFO)


string const LyXComm::inPipeName() const
{
	return pipename_ + ".in";
}


string const LyXComm::outPipeName() const
{
	return pipename_ + ".out";
}


/////////////////////////////////////////////////////////////////////
//
// Server
//
/////////////////////////////////////////////////////////////////////

void ServerCallback(Server * server, string const & msg)
{
	server->callback(msg);
}

Server::Server(LyXFunc * f, string const & pipes)
	: numclients_(0), func_(f), pipes_(pipes, this, &ServerCallback)
{}


Server::~Server()
{
	// say goodbye to clients so they stop sending messages
	// send as many bye messages as there are clients,
	// each with client's name.
	string message;
	for (int i = 0; i != numclients_; ++i) {
		message = "LYXSRV:" + clients_[i] + ":bye\n";
		pipes_.send(message);
	}
}


int compare(char const * a, char const * b, unsigned int len)
{
	using namespace std;
	return strncmp(a, b, len);
}


// Handle data gotten from communication, called by LyXComm
void Server::callback(string const & msg)
{
	LYXERR(Debug::LYXSERVER, "Server: Received: '" << msg << '\'');

	char const * p = msg.c_str();

	// --- parse the string --------------------------------------------
	//
	//  Format: LYXCMD:<client>:<func>:<argstring>\n
	//
	bool server_only = false;
	while (*p) {
		// --- 1. check 'header' ---

		if (compare(p, "LYXSRV:", 7) == 0) {
			server_only = true;
		} else if (0 != compare(p, "LYXCMD:", 7)) {
			lyxerr << "Server: Unknown request \""
			       << p << '"' << endl;
			return;
		}
		p += 7;

		// --- 2. for the moment ignore the client name ---
		string client;
		while (*p && *p != ':')
			client += char(*p++);
		if (*p == ':')
			++p;
		if (!*p)
			return;

		// --- 3. get function name ---
		string cmd;
		while (*p && *p != ':')
			cmd += char(*p++);

		// --- 4. parse the argument ---
		string arg;
		if (!server_only && *p == ':' && *(++p)) {
			while (*p && *p != '\n')
				arg += char(*p++);
			if (*p) ++p;
		}

		LYXERR(Debug::LYXSERVER, "Server: Client: '" << client
			<< "' Command: '" << cmd << "' Argument: '" << arg << '\'');

		// --- lookup and exec the command ------------------

		if (server_only) {
			string buf;
			// return the greeting to inform the client that
			// we are listening.
			if (cmd == "hello") {
				// One more client
				if (numclients_ == MAX_CLIENTS) { //paranoid check
					LYXERR(Debug::LYXSERVER, "Server: too many clients...");
					return;
				}
				int i = 0;
				while (!clients_[i].empty() && i < numclients_)
					++i;
				clients_[i] = client;
				++numclients_;
				buf = "LYXSRV:" + client + ":hello\n";
				LYXERR(Debug::LYXSERVER, "Server: Greeting " << client);
				pipes_.send(buf);
			} else if (cmd == "bye") {
				// If clients_ == 0 maybe we should reset the pipes
				// to prevent fake callbacks
				int i = 0; //look if client is registered
				for (; i < numclients_; ++i) {
					if (clients_[i] == client)
						break;
				}
				if (i < numclients_) {
					--numclients_;
					clients_[i].erase();
					LYXERR(Debug::LYXSERVER, "Server: Client "
						<< client << " said goodbye");
				} else {
					LYXERR(Debug::LYXSERVER,
						"Server: ignoring bye messge from unregistered client" << client);
				}
			} else {
				LYXERR0("Server: Undefined server command " << cmd << '.');
			}
			return;
		}

		if (!cmd.empty()) {
			// which lyxfunc should we let it connect to?
			// The correct solution would be to have a
			// specialized (non-gui) BufferView. But how do
			// we do it now? Probably we should just let it
			// connect to the lyxfunc in the single LyXView we
			// support currently. (Lgb)

			func_->dispatch(FuncRequest(lyxaction.lookupFunc(cmd), arg));
			string const rval = to_utf8(func_->getMessage());

			// all commands produce an INFO or ERROR message
			// in the output pipe, even if they do not return
			// anything. See chapter 4 of Customization doc.
			string buf;
			if (func_->errorStat())
				buf = "ERROR:";
			else
				buf = "INFO:";
			buf += client + ':' + cmd + ':' +  rval + '\n';
			pipes_.send(buf);

			// !!! we don't do any error checking -
			//  if the client won't listen, the
			//  message is lost and others too
			//  maybe; so the client should empty
			//  the outpipe before issuing a request.

			// not found
		}
	}  // while *p
}


// Send a notify message to a client, called by WorkAreaKeyPress
void Server::notifyClient(string const & s)
{
	pipes_.send("NOTIFY:" + s + "\n");
}


} // namespace lyx

#ifdef _WIN32
#include "moc_Server.cpp"
#endif
