// -*- C++ -*-
/**
 * \file lyx_main.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_MAIN_H
#define LYX_MAIN_H

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <list>
#include <string>

class Buffer;
class ErrorItem;
class InsetBase;
class LyXView;
class kb_keymap;
namespace lyx {
	class Session;
}


/// initial startup
class LyX : boost::noncopyable {
public:
	static void exec(int & argc, char * argv[]);
	static LyX & ref();
	static LyX const & cref();

	/// in the case of failure
	void emergencyCleanup() const;

	lyx::Session & session();
	lyx::Session const & session() const;

	void addLyXView(boost::shared_ptr<LyXView> const & lyxview);

	/** redraw \c inset in all the BufferViews in which it is currently
	 *  visible. If successful return a pointer to the owning Buffer.
	 */
	Buffer const * const updateInset(InsetBase const *) const;

private:
	static boost::scoped_ptr<LyX> singleton_;

	LyX();
	void priv_exec(int & argc, char * argv[]);

	/// initial LyX set up
	void init(bool);
	/// set up the default key bindings
	void defaultKeyBindings(kb_keymap * kbmap);
	/// set up the default dead key bindings if requested
	void deadKeyBindings(kb_keymap * kbmap);
	/** Check for the existence of the user's support directory and,
	 *  if not present, create it. Exits the program if the directory
	 *  cannot be created.
	 *  \returns true if the user-side configuration script
	 *  (lib/configure) should be re-run in this directory.
	 */
	bool queryUserLyXDir(bool explicit_userdir);
	/// read lyxrc/preferences
	void readRcFile(std::string const & name);
	/// read the given ui (menu/toolbar) file
	void readUIFile(std::string const & name);
	/// read the given languages file
	void readLanguagesFile(std::string const & name);
	/// read the given encodings file
	void readEncodingsFile(std::string const & name);
	/// parsing of non-gui LyX options. Returns true if gui
	bool easyParse(int & argc, char * argv[]);
	/// shows up a parsing error on screen
	void printError(ErrorItem const &);

	/// has this user started lyx for the first time?
	bool first_start;
	/// the parsed command line batch command if any
	std::string batch_command;

	/// lyx session, containing lastfiles, lastfilepos, and lastopened
	boost::scoped_ptr<lyx::Session> session_;
	///
	typedef std::list<boost::shared_ptr<LyXView> > ViewList;
	ViewList views_;
};

#endif // LYX_MAIN_H
