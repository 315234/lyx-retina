// -*- C++ -*-
/**
 * \file ForkedcallsController.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup Nielsen
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 *
 * A class for the control of child processes launched using
 * fork() and execvp().
 */

#ifndef FORKEDCALLSCONTROLLER_H
#define FORKEDCALLSCONTROLLER_H

#include <boost/shared_ptr.hpp>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include <list>
#include <string>
#include <vector>


namespace lyx {
namespace support {

class ForkedProcess;

class ForkedcallsController {
public:
	/// Get hold of the only controller that can exist inside the process.
	static ForkedcallsController & get();

	/// Add a new child process to the list of controlled processes.
	void addCall(ForkedProcess const &);

	/** Those child processes that are found to have finished are removed
	 *  from the list and their callback function is passed the final
	 *  return state.
	 */
	void handleCompletedProcesses();

	/** Kill this process prematurely and remove it from the list.
	 *  The process is killed within tolerance secs.
	 *  See forkedcall.[Ch] for details.
	 */
	void kill(pid_t, int tolerance = 5);

private:
	ForkedcallsController();
	ForkedcallsController(ForkedcallsController const &);
	~ForkedcallsController();

	typedef boost::shared_ptr<ForkedProcess> ForkedProcessPtr;
	typedef std::list<ForkedProcessPtr> ListType;
	typedef ListType::iterator iterator;

	iterator find_pid(pid_t);

	/// The child processes
	ListType forkedCalls;
};


#if defined(_WIN32)
// a wrapper for GetLastError() and FormatMessage().
std::string const getChildErrorMessage();
#endif

} // namespace support
} // namespace lyx

#endif // FORKEDCALLSCONTROLLER_H
