// -*- C++ -*-
/**
 * \file path.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef PATH_H
#define PATH_H

#include <boost/utility.hpp>

#include <string>

namespace lyx {
namespace support {

/**
 * Path - utility closs for stackable working directories
 *
 * You can use a local variable of this type to temporarily
 * change to a directory as the cwd, for example :
 *
 * if (blah) {
 *	Path p("/tmp/blah");
 *	...
 * }
 *
 * At the end of p's scope the cwd is reset to its previous value.
 */
class Path : boost::noncopyable {
public:
	/// change to the given directory
	explicit Path(std::string const & path);

	/// set cwd to the previous value if needed
	~Path();

	/// set cwd to the previous value if needed
	int pop();
private:
	/// whether we are in the new cwd or not
	bool popped_;
	/// the previous cwd
	std::string pushedDir_;
};

// To avoid the wrong usage:
// Path("/tmp");   // wrong
// Path p("/tmp");  // right
// we add this macro:
///
#ifndef PATH_C
#define Path(x) unnamed_Path;
#endif
// Tip gotten from Bobby Schmidt's column in C/C++ Users Journal

} // namespace support
} // namespace lyx

#endif // PATH_H
