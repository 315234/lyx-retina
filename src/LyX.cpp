/**
 * \file LyX.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>
#include <version.h>

#include "LyX.h"

#include "AspellChecker.h"
#include "Buffer.h"
#include "BufferList.h"
#include "CmdDef.h"
#include "ColorSet.h"
#include "ConverterCache.h"
#include "Converter.h"
#include "CutAndPaste.h"
#include "EnchantChecker.h"
#include "Encoding.h"
#include "ErrorList.h"
#include "Format.h"
#include "FuncStatus.h"
#include "HunspellChecker.h"
#include "KeyMap.h"
#include "Language.h"
#include "LayoutFile.h"
#include "Lexer.h"
#include "LyX.h"
#include "LyXAction.h"
#include "LyXRC.h"
#include "ModuleList.h"
#include "Mover.h"
#include "Server.h"
#include "ServerSocket.h"
#include "Session.h"

#include "frontends/alert.h"
#include "frontends/Application.h"

#include "graphics/Previews.h"

#include "support/lassert.h"
#include "support/debug.h"
#include "support/environment.h"
#include "support/ExceptionMessage.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lstrings.h"
#include "support/Messages.h"
#include "support/os.h"
#include "support/Package.h"
#include "support/Path.h"
#include "support/Systemcall.h"

#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>

#include <algorithm>
#include <iostream>
#include <csignal>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace Alert = frontend::Alert;
namespace os = support::os;



// Are we using the GUI at all?  We default to true and this is changed
// to false when the export feature is used.

bool use_gui = true;


// Tell what files can be silently overwritten during batch export.
// Possible values are: NO_FILES, MAIN_FILE, ALL_FILES.

OverwriteFiles force_overwrite = NO_FILES;


namespace {

// Filled with the command line arguments "foo" of "-sysdir foo" or
// "-userdir foo".
string cl_system_support;
string cl_user_support;

string geometryArg;

LyX * singleton_ = 0;

void showFileError(string const & error)
{
	Alert::warning(_("Could not read configuration file"),
		       bformat(_("Error while reading the configuration file\n%1$s.\n"
			   "Please check your installation."), from_utf8(error)));
}


void reconfigureUserLyXDir()
{
	string const configure_command = package().configure_command();

	lyxerr << to_utf8(_("LyX: reconfiguring user directory")) << endl;
	PathChanger p(package().user_support());
	Systemcall one;
	one.startscript(Systemcall::Wait, configure_command);
	lyxerr << "LyX: " << to_utf8(_("Done!")) << endl;
}

} // namespace anon

/// The main application class private implementation.
struct LyX::Impl
{
	Impl() : spell_checker_(0), aspell_checker_(0), enchant_checker_(0), hunspell_checker_(0)
	{
		// Set the default User Interface language as soon as possible.
		// The language used will be derived from the environment
		// variables.
		messages_["GUI"] = Messages();
	}

	~Impl()
	{
		delete aspell_checker_;
		delete enchant_checker_;
		delete hunspell_checker_;
	}

	///
	BufferList buffer_list_;
	///
	KeyMap toplevel_keymap_;
	///
	CmdDef toplevel_cmddef_;
	///
	boost::scoped_ptr<Server> lyx_server_;
	///
	boost::scoped_ptr<ServerSocket> lyx_socket_;
	///
	boost::scoped_ptr<frontend::Application> application_;
	/// lyx session, containing lastfiles, lastfilepos, and lastopened
	boost::scoped_ptr<Session> session_;

	/// Files to load at start.
	vector<string> files_to_load_;

	/// The messages translators.
	map<string, Messages> messages_;

	/// The file converters.
	Converters converters_;

	// The system converters copy after reading lyxrc.defaults.
	Converters system_converters_;

	///
	Movers movers_;
	///
	Movers system_movers_;

	/// has this user started lyx for the first time?
	bool first_start;
	/// the parsed command line batch command if any
	vector<string> batch_commands;

	///
	graphics::Previews preview_;
	///
	SpellChecker * spell_checker_;
	///
	SpellChecker * aspell_checker_;
	///
	SpellChecker * enchant_checker_;
	///
	SpellChecker * hunspell_checker_;
};

///
frontend::Application * theApp()
{
	if (singleton_)
		return singleton_->pimpl_->application_.get();
	else
		return 0;
}


LyX::~LyX()
{
	singleton_ = 0;
	delete pimpl_;
}


void lyx_exit(int exit_code)
{
	if (exit_code)
		// Something wrong happened so better save everything, just in
		// case.
		emergencyCleanup();

#ifndef NDEBUG
	// Properly crash in debug mode in order to get a useful backtrace.
	abort();
#endif

	// In release mode, try to exit gracefully.
	if (theApp())
		theApp()->exit(exit_code);
	else
		exit(exit_code);
}


LyX::LyX()
	: first_start(false)
{
	singleton_ = this;
	pimpl_ = new Impl;
}


Messages & LyX::messages(string const & language)
{
	map<string, Messages>::iterator it = pimpl_->messages_.find(language);

	if (it != pimpl_->messages_.end())
		return it->second;

	pair<map<string, Messages>::iterator, bool> result =
			pimpl_->messages_.insert(make_pair(language, Messages(language)));

	LASSERT(result.second, /**/);
	return result.first->second;
}


void setRcGuiLanguage()
{
	LASSERT(singleton_, /**/);
	if (lyxrc.gui_language == "auto")
		return;
	Language const * language = languages.getLanguage(lyxrc.gui_language);
	if (language) {
		LYXERR(Debug::LOCALE, "Setting LANGUAGE to " << language->code());
		if (!setEnv("LANGUAGE", language->code()))
			LYXERR(Debug::LOCALE, "\t... failed!");
	}
	LYXERR(Debug::LOCALE, "Setting LC_ALL to en_US");
	if (!setEnv("LC_ALL", "en_US"))
		LYXERR(Debug::LOCALE, "\t... failed!");
	Messages::init();
	singleton_->pimpl_->messages_["GUI"] = Messages();
}


int LyX::exec(int & argc, char * argv[])
{
	// Here we need to parse the command line. At least
	// we need to parse for "-dbg" and "-help"
	easyParse(argc, argv);

	try {
		init_package(os::utf8_argv(0),
			      cl_system_support, cl_user_support,
			      top_build_dir_is_one_level_up);
	} catch (ExceptionMessage const & message) {
		if (message.type_ == ErrorException) {
			Alert::error(message.title_, message.details_);
			lyx_exit(1);
		} else if (message.type_ == WarningException) {
			Alert::warning(message.title_, message.details_);
		}
	}

	// Reinit the messages machinery in case package() knows
	// something interesting about the locale directory.
	Messages::init();

	if (!use_gui) {
		// FIXME: create a ConsoleApplication
		int exit_status = init(argc, argv);
		if (exit_status) {
			prepareExit();
			return exit_status;
		}

		// this is correct, since return values are inverted.
		exit_status = !loadFiles();

		if (pimpl_->batch_commands.empty() || pimpl_->buffer_list_.empty()) {
			prepareExit();
			return exit_status;
		}

		BufferList::iterator begin = pimpl_->buffer_list_.begin();

		bool final_success = false;
		for (BufferList::iterator I = begin; I != pimpl_->buffer_list_.end(); ++I) {
			Buffer * buf = *I;
			if (buf != buf->masterBuffer())
				continue;
			vector<string>::const_iterator bcit  = pimpl_->batch_commands.begin();
			vector<string>::const_iterator bcend = pimpl_->batch_commands.end();
			DispatchResult dr;
			for (; bcit != bcend; bcit++) {
				LYXERR(Debug::ACTION, "Buffer::dispatch: cmd: " << *bcit);
				buf->dispatch(*bcit, dr);
				final_success |= !dr.error();
			}
		}
		prepareExit();
		return !final_success;
	}

	// Let the frontend parse and remove all arguments that it knows
	pimpl_->application_.reset(createApplication(argc, argv));

	// Reestablish our defaults, as Qt overwrites them
	// after createApplication()
	locale_init();

	// Parse and remove all known arguments in the LyX singleton
	// Give an error for all remaining ones.
	int exit_status = init(argc, argv);
	if (exit_status) {
		// Kill the application object before exiting.
		pimpl_->application_.reset();
		use_gui = false;
		prepareExit();
		return exit_status;
	}
 
	// FIXME
	/* Create a CoreApplication class that will provide the main event loop
	* and the socket callback registering. With Qt4, only QtCore
	* library would be needed.
	* When this is done, a server_mode could be created and the following two
	* line would be moved out from here.
	*/
	// Note: socket callback must be registered after init(argc, argv)
	// such that package().temp_dir() is properly initialized.
	pimpl_->lyx_server_.reset(new Server(lyxrc.lyxpipes));
	pimpl_->lyx_socket_.reset(new ServerSocket(
			FileName(package().temp_dir().absFileName() + "/lyxsocket")));

	// Start the real execution loop.
	exit_status = pimpl_->application_->exec();

	prepareExit();

	return exit_status;
}


void LyX::prepareExit()
{
	// Clear the clipboard and selection stack:
	cap::clearCutStack();
	cap::clearSelection();

	// Write the index file of the converter cache
	ConverterCache::get().writeIndex();

	// close buffers first
	pimpl_->buffer_list_.closeAll();

	// register session changes and shutdown server and socket
	if (use_gui) {
		if (pimpl_->session_)
			pimpl_->session_->writeFile();
		pimpl_->session_.reset();
		pimpl_->lyx_server_.reset();
		pimpl_->lyx_socket_.reset();
	}

	// do any other cleanup procedures now
	if (package().temp_dir() != package().system_temp_dir()) {
		string const abs_tmpdir = package().temp_dir().absFileName();
		if (!contains(package().temp_dir().absFileName(), "lyx_tmpdir")) {
			docstring const msg =
				bformat(_("%1$s does not appear like a LyX created temporary directory."),
				from_utf8(abs_tmpdir));
			Alert::warning(_("Cannot remove temporary directory"), msg);
		} else {
			LYXERR(Debug::INFO, "Deleting tmp dir "
				<< package().temp_dir().absFileName());
			if (!package().temp_dir().destroyDirectory()) {
				docstring const msg =
					bformat(_("Unable to remove the temporary directory %1$s"),
					from_utf8(package().temp_dir().absFileName()));
				Alert::warning(_("Unable to remove temporary directory"), msg);
			}
		}
	}

	// Kill the application object before exiting. This avoids crashes
	// when exiting on Linux.
	if (pimpl_->application_)
		pimpl_->application_.reset();
}


void LyX::earlyExit(int status)
{
	LASSERT(pimpl_->application_.get(), /**/);
	// LyX::pimpl_::application_ is not initialised at this
	// point so it's safe to just exit after some cleanup.
	prepareExit();
	exit(status);
}


int LyX::init(int & argc, char * argv[])
{
	// check for any spurious extra arguments
	// other than documents
	for (int argi = 1; argi < argc ; ++argi) {
		if (argv[argi][0] == '-') {
			lyxerr << to_utf8(
				bformat(_("Wrong command line option `%1$s'. Exiting."),
				from_utf8(os::utf8_argv(argi)))) << endl;
			return EXIT_FAILURE;
		}
	}

	// Initialization of LyX (reads lyxrc and more)
	LYXERR(Debug::INIT, "Initializing LyX::init...");
	bool success = init();
	LYXERR(Debug::INIT, "Initializing LyX::init...done");
	if (!success)
		return EXIT_FAILURE;

	// Remaining arguments are assumed to be files to load.
	for (int argi = argc - 1; argi >= 1; --argi)
		pimpl_->files_to_load_.push_back(os::utf8_argv(argi));

	if (first_start) {
		pimpl_->files_to_load_.push_back(
			i18nLibFileSearch("examples", "splash.lyx").absFileName());
	}

	return EXIT_SUCCESS;
}


bool LyX::loadFiles()
{
	LASSERT(!use_gui, /**/);
	bool success = true;
	vector<string>::const_iterator it = pimpl_->files_to_load_.begin();
	vector<string>::const_iterator end = pimpl_->files_to_load_.end();

	for (; it != end; ++it) {
		// get absolute path of file and add ".lyx" to
		// the filename if necessary
		FileName fname = fileSearch(string(), os::internal_path(*it), "lyx",
			may_not_exist);

		if (fname.empty())
			continue;

		Buffer * buf = pimpl_->buffer_list_.newBuffer(fname.absFileName(), false);
		if (buf->loadLyXFile(fname)) {
			ErrorList const & el = buf->errorList("Parse");
			if (!el.empty())
				for_each(el.begin(), el.end(),
				boost::bind(&LyX::printError, this, _1));
		}
		else {
			pimpl_->buffer_list_.release(buf);
			success = false;
		}
	}
	return success;
}


void execBatchCommands()
{
	LASSERT(singleton_, /**/);
	singleton_->execCommands();
}


void LyX::execCommands()
{
	// The advantage of doing this here is that the event loop
	// is already started. So any need for interaction will be
	// aknowledged.

	// if reconfiguration is needed.
	if (LayoutFileList::get().empty()) {
		switch (Alert::prompt(
			_("No textclass is found"),
			_("LyX will have minimal functionality because no textclasses "
				"have been found. You can either try to reconfigure LyX normally, "
				"try to reconfigure using only the defaults, or continue."),
			0, 2,
			_("&Reconfigure"),
			_("&Use Defaults"),
			_("&Continue")))
		{
		case 0:
			// regular reconfigure
			lyx::dispatch(FuncRequest(LFUN_RECONFIGURE, ""));
			break;
		case 1:
			// reconfigure --without-latex-config
			lyx::dispatch(FuncRequest(LFUN_RECONFIGURE,
				" --without-latex-config"));
			break;
		default:
			break;
		}
	}
	
	// create the first main window
	lyx::dispatch(FuncRequest(LFUN_WINDOW_NEW, geometryArg));

	if (!pimpl_->files_to_load_.empty()) {
		// if some files were specified at command-line we assume that the
		// user wants to edit *these* files and not to restore the session.
		for (size_t i = 0; i != pimpl_->files_to_load_.size(); ++i) {
			lyx::dispatch(
				FuncRequest(LFUN_FILE_OPEN, pimpl_->files_to_load_[i]));
		}
		// clear this list to save a few bytes of RAM
		pimpl_->files_to_load_.clear();
	} else
		pimpl_->application_->restoreGuiSession();

	// Execute batch commands if available
	if (pimpl_->batch_commands.empty())
		return;

	vector<string>::const_iterator bcit  = pimpl_->batch_commands.begin();
	vector<string>::const_iterator bcend = pimpl_->batch_commands.end();
	for (; bcit != bcend; bcit++) {
		LYXERR(Debug::INIT, "About to handle -x '" << *bcit << '\'');
		lyx::dispatch(lyxaction.lookupFunc(*bcit));
	}
}


/*
Signals and Windows
===================
The SIGHUP signal does not exist on Windows and does not need to be handled.

Windows handles SIGFPE and SIGSEGV signals as expected.

Ctrl+C interrupts (mapped to SIGINT by Windows' POSIX compatability layer)
cause a new thread to be spawned. This may well result in unexpected
behaviour by the single-threaded LyX.

SIGTERM signals will come only from another process actually sending
that signal using 'raise' in Windows' POSIX compatability layer. It will
not come from the general "terminate process" methods that everyone
actually uses (and which can't be trapped). Killing an app 'politely' on
Windows involves first sending a WM_CLOSE message, something that is
caught already by the Qt frontend.

For more information see:

http://aspn.activestate.com/ASPN/Mail/Message/ActiveTcl/2034055
...signals are mostly useless on Windows for a variety of reasons that are
Windows specific...

'UNIX Application Migration Guide, Chapter 9'
http://msdn.microsoft.com/library/en-us/dnucmg/html/UCMGch09.asp

'How To Terminate an Application "Cleanly" in Win32'
http://support.microsoft.com/default.aspx?scid=kb;en-us;178893
*/
extern "C" {

static void error_handler(int err_sig)
{
	// Throw away any signals other than the first one received.
	static sig_atomic_t handling_error = false;
	if (handling_error)
		return;
	handling_error = true;

	// We have received a signal indicating a fatal error, so
	// try and save the data ASAP.
	emergencyCleanup();

	// These lyxerr calls may or may not work:

	// Signals are asynchronous, so the main program may be in a very
	// fragile state when a signal is processed and thus while a signal
	// handler function executes.
	// In general, therefore, we should avoid performing any
	// I/O operations or calling most library and system functions from
	// signal handlers.

	// This shouldn't matter here, however, as we've already invoked
	// emergencyCleanup.
	docstring msg;
	switch (err_sig) {
#ifdef SIGHUP
	case SIGHUP:
		msg = _("SIGHUP signal caught!\nBye.");
		break;
#endif
	case SIGFPE:
		msg = _("SIGFPE signal caught!\nBye.");
		break;
	case SIGSEGV:
		msg = _("SIGSEGV signal caught!\n"
			  "Sorry, you have found a bug in LyX, "
			  "hope you have not lost any data.\n"
			  "Please read the bug-reporting instructions "
			  "in 'Help->Introduction' and send us a bug report, "
			  "if necessary. Thanks !\nBye.");
		break;
	case SIGINT:
	case SIGTERM:
		// no comments
		break;
	}

	if (!msg.empty()) {
		lyxerr << "\nlyx: " << msg << endl;
		// try to make a GUI message
		Alert::error(_("LyX crashed!"), msg);
	}

	// Deinstall the signal handlers
#ifdef SIGHUP
	signal(SIGHUP, SIG_DFL);
#endif
	signal(SIGINT, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGTERM, SIG_DFL);

#ifdef SIGHUP
	if (err_sig == SIGSEGV ||
		(err_sig != SIGHUP && !getEnv("LYXDEBUG").empty())) {
#else
	if (err_sig == SIGSEGV || !getEnv("LYXDEBUG").empty()) {
#endif 
#ifdef _MSC_VER
		// with abort() it crashes again.
		exit(err_sig);
#else
		abort();
#endif
	}

	exit(0);
}

}


void LyX::printError(ErrorItem const & ei)
{
	docstring tmp = _("LyX: ") + ei.error + char_type(':')
		+ ei.description;
	cerr << to_utf8(tmp) << endl;
}


bool LyX::init()
{
#ifdef SIGHUP
	signal(SIGHUP, error_handler);
#endif
	signal(SIGFPE, error_handler);
	signal(SIGSEGV, error_handler);
	signal(SIGINT, error_handler);
	signal(SIGTERM, error_handler);
	// SIGPIPE can be safely ignored.

	lyxrc.tempdir_path = package().temp_dir().absFileName();
	lyxrc.document_path = package().document_dir().absFileName();

	if (lyxrc.example_path.empty()) {
		lyxrc.example_path = addPath(package().system_support().absFileName(),
					      "examples");
	}
	if (lyxrc.template_path.empty()) {
		lyxrc.template_path = addPath(package().system_support().absFileName(),
					      "templates");
	}

	//
	// Read configuration files
	//

	// This one may have been distributed along with LyX.
	if (!readRcFile("lyxrc.dist"))
		return false;

	// Set the language defined by the distributor.
	setRcGuiLanguage();

	// Set the PATH correctly.
#if !defined (USE_POSIX_PACKAGING)
	// Add the directory containing the LyX executable to the path
	// so that LyX can find things like tex2lyx.
	if (package().build_support().empty())
		prependEnvPath("PATH", package().binary_dir().absFileName());
#endif
	if (!lyxrc.path_prefix.empty())
		prependEnvPath("PATH", lyxrc.path_prefix);

	// Check that user LyX directory is ok.
	if (queryUserLyXDir(package().explicit_user_support()))
		reconfigureUserLyXDir();

	// no need for a splash when there is no GUI
	if (!use_gui) {
		first_start = false;
	}

	// This one is generated in user_support directory by lib/configure.py.
	if (!readRcFile("lyxrc.defaults"))
		return false;

	// Query the OS to know what formats are viewed natively
	formats.setAutoOpen();

	// Read lyxrc.dist again to be able to override viewer auto-detection.
	readRcFile("lyxrc.dist");

	// Set again the language defined by the distributor.
	setRcGuiLanguage();

	system_lyxrc = lyxrc;
	system_formats = formats;
	pimpl_->system_converters_ = pimpl_->converters_;
	pimpl_->system_movers_ = pimpl_->movers_;
	system_lcolor = lcolor;

	// This one is edited through the preferences dialog.
	if (!readRcFile("preferences"))
		return false;

	if (!readEncodingsFile("encodings", "unicodesymbols"))
		return false;
	if (!readLanguagesFile("languages"))
		return false;

	// Set the language defined by the user.
	setRcGuiLanguage();

	LYXERR(Debug::INIT, "Reading layouts...");
	// Load the layouts
	LayoutFileList::get().read();
	//...and the modules
	theModuleList.read();

	// read keymap and ui files in batch mode as well
	// because InsetInfo needs to know these to produce
	// the correct output

	// Set up command definitions
	pimpl_->toplevel_cmddef_.read(lyxrc.def_file);

	// Set up bindings
	pimpl_->toplevel_keymap_.read("site");
	pimpl_->toplevel_keymap_.read(lyxrc.bind_file);
	// load user bind file user.bind
	pimpl_->toplevel_keymap_.read("user", 0, KeyMap::MissingOK);

	if (lyxerr.debugging(Debug::LYXRC))
		lyxrc.print();

	os::windows_style_tex_paths(lyxrc.windows_style_tex_paths);
	if (!lyxrc.path_prefix.empty())
		prependEnvPath("PATH", lyxrc.path_prefix);

	FileName const document_path(lyxrc.document_path);
	if (document_path.exists() && document_path.isDirectory())
		package().document_dir() = document_path;

	package().set_temp_dir(createLyXTmpDir(FileName(lyxrc.tempdir_path)));
	if (package().temp_dir().empty()) {
		Alert::error(_("Could not create temporary directory"),
			     bformat(_("Could not create a temporary directory in\n"
						       "\"%1$s\"\n"
							   "Make sure that this path exists and is writable and try again."),
				     from_utf8(lyxrc.tempdir_path)));
		// createLyXTmpDir() tries sufficiently hard to create a
		// usable temp dir, so the probability to come here is
		// close to zero. We therefore don't try to overcome this
		// problem with e.g. asking the user for a new path and
		// trying again but simply exit.
		return false;
	}

	LYXERR(Debug::INIT, "LyX tmp dir: `"
			    << package().temp_dir().absFileName() << '\'');

	LYXERR(Debug::INIT, "Reading session information '.lyx/session'...");
	pimpl_->session_.reset(new Session(lyxrc.num_lastfiles));

	// This must happen after package initialization and after lyxrc is
	// read, therefore it can't be done by a static object.
	ConverterCache::init();
		
	return true;
}


void emergencyCleanup()
{
	// what to do about tmpfiles is non-obvious. we would
	// like to delete any we find, but our lyxdir might
	// contain documents etc. which might be helpful on
	// a crash

	singleton_->pimpl_->buffer_list_.emergencyWriteAll();
	if (use_gui) {
		if (singleton_->pimpl_->lyx_server_)
			singleton_->pimpl_->lyx_server_->emergencyCleanup();
		singleton_->pimpl_->lyx_server_.reset();
		singleton_->pimpl_->lyx_socket_.reset();
	}
}


// return true if file does not exist or is older than configure.py.
static bool needsUpdate(string const & file)
{
	// We cannot initialize configure_script directly because the package
	// is not initialized yet when  static objects are constructed.
	static FileName configure_script;
	static bool firstrun = true;
	if (firstrun) {
		configure_script =
			FileName(addName(package().system_support().absFileName(),
				"configure.py"));
		firstrun = false;
	}

	FileName absfile = 
		FileName(addName(package().user_support().absFileName(), file));
	return !absfile.exists()
		|| configure_script.lastModified() > absfile.lastModified();
}


bool LyX::queryUserLyXDir(bool explicit_userdir)
{
	// Does user directory exist?
	FileName const sup = package().user_support();
	if (sup.exists() && sup.isDirectory()) {
		first_start = false;

		return needsUpdate("lyxrc.defaults")
			|| needsUpdate("lyxmodules.lst")
			|| needsUpdate("textclass.lst")
			|| needsUpdate("packages.lst");
	}

	first_start = !explicit_userdir;

	// If the user specified explicitly a directory, ask whether
	// to create it. If the user says "no", then exit.
	if (explicit_userdir &&
	    Alert::prompt(
		    _("Missing user LyX directory"),
		    bformat(_("You have specified a non-existent user "
					   "LyX directory, %1$s.\n"
					   "It is needed to keep your own configuration."),
			    from_utf8(package().user_support().absFileName())),
		    1, 0,
		    _("&Create directory"),
		    _("&Exit LyX"))) {
		lyxerr << to_utf8(_("No user LyX directory. Exiting.")) << endl;
		earlyExit(EXIT_FAILURE);
	}

	lyxerr << to_utf8(bformat(_("LyX: Creating directory %1$s"),
			  from_utf8(sup.absFileName()))) << endl;

	if (!sup.createDirectory(0755)) {
		// Failed, so let's exit.
		lyxerr << to_utf8(_("Failed to create directory. Exiting."))
		       << endl;
		earlyExit(EXIT_FAILURE);
	}

	return true;
}


bool LyX::readRcFile(string const & name)
{
	LYXERR(Debug::INIT, "About to read " << name << "... ");

	FileName const lyxrc_path = libFileSearch(string(), name);
	if (!lyxrc_path.empty()) {
		LYXERR(Debug::INIT, "Found in " << lyxrc_path);
		if (lyxrc.read(lyxrc_path) < 0) {
			showFileError(name);
			return false;
		}
	} else {
		LYXERR(Debug::INIT, "Not found." << lyxrc_path);
	}
	return true;
}

// Read the languages file `name'
bool LyX::readLanguagesFile(string const & name)
{
	LYXERR(Debug::INIT, "About to read " << name << "...");

	FileName const lang_path = libFileSearch(string(), name);
	if (lang_path.empty()) {
		showFileError(name);
		return false;
	}
	languages.read(lang_path);
	return true;
}


// Read the encodings file `name'
bool LyX::readEncodingsFile(string const & enc_name,
			    string const & symbols_name)
{
	LYXERR(Debug::INIT, "About to read " << enc_name << " and "
			    << symbols_name << "...");

	FileName const symbols_path = libFileSearch(string(), symbols_name);
	if (symbols_path.empty()) {
		showFileError(symbols_name);
		return false;
	}

	FileName const enc_path = libFileSearch(string(), enc_name);
	if (enc_path.empty()) {
		showFileError(enc_name);
		return false;
	}
	encodings.read(enc_path, symbols_path);
	return true;
}


namespace {

/// return the the number of arguments consumed
typedef boost::function<int(string const &, string const &, string &)> cmd_helper;

int parse_dbg(string const & arg, string const &, string &)
{
	if (arg.empty()) {
		lyxerr << to_utf8(_("List of supported debug flags:")) << endl;
		Debug::showTags(lyxerr);
		exit(0);
	}
	lyxerr << to_utf8(bformat(_("Setting debug level to %1$s"), from_utf8(arg))) << endl;

	lyxerr.setLevel(Debug::value(arg));
	Debug::showLevel(lyxerr, lyxerr.level());
	return 1;
}


int parse_help(string const &, string const &, string &)
{
	lyxerr <<
		to_utf8(_("Usage: lyx [ command line switches ] [ name.lyx ... ]\n"
		  "Command line switches (case sensitive):\n"
		  "\t-help              summarize LyX usage\n"
		  "\t-userdir dir       set user directory to dir\n"
		  "\t-sysdir dir        set system directory to dir\n"
		  "\t-geometry WxH+X+Y  set geometry of the main window\n"
		  "\t-dbg feature[,feature]...\n"
		  "                  select the features to debug.\n"
		  "                  Type `lyx -dbg' to see the list of features\n"
		  "\t-x [--execute] command\n"
		  "                  where command is a lyx command.\n"
		  "\t-e [--export] fmt\n"
		  "                  where fmt is the export format of choice.\n"
		  "                  Look on Tools->Preferences->File formats->Format\n"
		  "                  to get an idea which parameters should be passed.\n"
		  "                  Note that the order of -e and -x switches matters.\n"
		  "\t-i [--import] fmt file.xxx\n"
		  "                  where fmt is the import format of choice\n"
		  "                  and file.xxx is the file to be imported.\n"
		  "\t-f [--force-overwrite] what\n"
		  "                  where what is either `all' or `main'.\n"
		  "                  Using `all', all files are overwritten during\n"
		  "                  a batch export, otherwise only the main file will be.\n"
		  "                  Anything else is equivalent to `all', but is not consumed.\n"
		  "\t-batch          execute commands without launching GUI and exit.\n"
		  "\t-version        summarize version and build info\n"
			       "Check the LyX man page for more details.")) << endl;
	exit(0);
	return 0;
}


int parse_version(string const &, string const &, string &)
{
	lyxerr << "LyX " << lyx_version
	       << " (" << lyx_release_date << ")" << endl;
	lyxerr << "Built on " << __DATE__ << ", " << __TIME__ << endl;

	lyxerr << lyx_version_info << endl;
	exit(0);
	return 0;
}


int parse_sysdir(string const & arg, string const &, string &)
{
	if (arg.empty()) {
		Alert::error(_("No system directory"),
			_("Missing directory for -sysdir switch"));
		exit(1);
	}
	cl_system_support = arg;
	return 1;
}


int parse_userdir(string const & arg, string const &, string &)
{
	if (arg.empty()) {
		Alert::error(_("No user directory"),
			_("Missing directory for -userdir switch"));
		exit(1);
	}
	cl_user_support = arg;
	return 1;
}


int parse_execute(string const & arg, string const &, string & batch)
{
	if (arg.empty()) {
		Alert::error(_("Incomplete command"),
			_("Missing command string after --execute switch"));
		exit(1);
	}
	batch = arg;
	return 1;
}


int parse_export(string const & type, string const &, string & batch)
{
	if (type.empty()) {
		lyxerr << to_utf8(_("Missing file type [eg latex, ps...] after "
					 "--export switch")) << endl;
		exit(1);
	}
	batch = "buffer-export " + type;
	use_gui = false;
	return 1;
}


int parse_import(string const & type, string const & file, string & batch)
{
	if (type.empty()) {
		lyxerr << to_utf8(_("Missing file type [eg latex, ps...] after "
					 "--import switch")) << endl;
		exit(1);
	}
	if (file.empty()) {
		lyxerr << to_utf8(_("Missing filename for --import")) << endl;
		exit(1);
	}

	batch = "buffer-import " + type + ' ' + file;
	return 2;
}


int parse_geometry(string const & arg1, string const &, string &)
{
	geometryArg = arg1;
	// don't remove "-geometry", it will be pruned out later in the
	// frontend if need be.
	return -1;
}


int parse_batch(string const &, string const &, string &) 
{
	use_gui = false;
	return 0;
}


int parse_force(string const & arg, string const &, string &) 
{
	if (arg == "all") {
		force_overwrite = ALL_FILES;
		return 1;
	} else if (arg == "main") {
		force_overwrite = MAIN_FILE;
		return 1;
	}
	force_overwrite = ALL_FILES;
	return 0;
}


} // namespace anon


void LyX::easyParse(int & argc, char * argv[])
{
	map<string, cmd_helper> cmdmap;

	cmdmap["-dbg"] = parse_dbg;
	cmdmap["-help"] = parse_help;
	cmdmap["--help"] = parse_help;
	cmdmap["-version"] = parse_version;
	cmdmap["--version"] = parse_version;
	cmdmap["-sysdir"] = parse_sysdir;
	cmdmap["-userdir"] = parse_userdir;
	cmdmap["-x"] = parse_execute;
	cmdmap["--execute"] = parse_execute;
	cmdmap["-e"] = parse_export;
	cmdmap["--export"] = parse_export;
	cmdmap["-i"] = parse_import;
	cmdmap["--import"] = parse_import;
	cmdmap["-geometry"] = parse_geometry;
	cmdmap["-batch"] = parse_batch;
	cmdmap["-f"] = parse_force;
	cmdmap["--force-overwrite"] = parse_force;

	for (int i = 1; i < argc; ++i) {
		map<string, cmd_helper>::const_iterator it
			= cmdmap.find(argv[i]);

		// don't complain if not found - may be parsed later
		if (it == cmdmap.end())
			continue;

		string const arg =
			(i + 1 < argc) ? os::utf8_argv(i + 1) : string();
		string const arg2 =
			(i + 2 < argc) ? os::utf8_argv(i + 2) : string();

		string batch;
		int const remove = 1 + it->second(arg, arg2, batch);
		if (!batch.empty())
			pimpl_->batch_commands.push_back(batch);

		// Now, remove used arguments by shifting
		// the following ones remove places down.
		if (remove > 0) {
			os::remove_internal_args(i, remove);
			argc -= remove;
			for (int j = i; j < argc; ++j)
				argv[j] = argv[j + remove];
			--i;
		}
	}
}


FuncStatus getStatus(FuncRequest const & action)
{
	LASSERT(theApp(), /**/);
	return theApp()->getStatus(action);
}


void dispatch(FuncRequest const & action)
{
	LASSERT(theApp(), /**/);
	return theApp()->dispatch(action);
}


void dispatch(FuncRequest const & action, DispatchResult & dr)
{
	LASSERT(theApp(), /**/);
	return theApp()->dispatch(action, dr);
}


BufferList & theBufferList()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->buffer_list_;
}


Server & theServer()
{
	// FIXME: this should not be use_gui dependent
	LASSERT(use_gui, /**/);
	LASSERT(singleton_, /**/);
	return *singleton_->pimpl_->lyx_server_.get();
}


ServerSocket & theServerSocket()
{
	// FIXME: this should not be use_gui dependent
	LASSERT(use_gui, /**/);
	LASSERT(singleton_, /**/);
	return *singleton_->pimpl_->lyx_socket_.get();
}


KeyMap & theTopLevelKeymap()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->toplevel_keymap_;
}


Converters & theConverters()
{
	LASSERT(singleton_, /**/);
	return  singleton_->pimpl_->converters_;
}


Converters & theSystemConverters()
{
	LASSERT(singleton_, /**/);
	return  singleton_->pimpl_->system_converters_;
}


Movers & theMovers()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->movers_;
}


Mover const & getMover(string  const & fmt)
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->movers_(fmt);
}


void setMover(string const & fmt, string const & command)
{
	LASSERT(singleton_, /**/);
	singleton_->pimpl_->movers_.set(fmt, command);
}


Movers & theSystemMovers()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->system_movers_;
}


Messages const & getMessages(string const & language)
{
	LASSERT(singleton_, /**/);
	return singleton_->messages(language);
}


Messages const & getGuiMessages()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->messages_["GUI"];
}


graphics::Previews & thePreviews()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->preview_;
}


Session & theSession()
{
	LASSERT(singleton_, /**/);
	return *singleton_->pimpl_->session_.get();
}


CmdDef & theTopLevelCmdDef()
{
	LASSERT(singleton_, /**/);
	return singleton_->pimpl_->toplevel_cmddef_;
}


SpellChecker * theSpellChecker()
{
	if (!singleton_->pimpl_->spell_checker_)
		setSpellChecker();
	return singleton_->pimpl_->spell_checker_;
}


void setSpellChecker()
{
#if defined(USE_ASPELL)
	if (lyxrc.spellchecker == "aspell") {
		if (!singleton_->pimpl_->aspell_checker_)
			singleton_->pimpl_->aspell_checker_ = new AspellChecker();
		singleton_->pimpl_->spell_checker_ = singleton_->pimpl_->aspell_checker_;
		return;
	}
#endif
#if defined(USE_ENCHANT)
	if (lyxrc.spellchecker == "enchant") {
		if (!singleton_->pimpl_->enchant_checker_)
			singleton_->pimpl_->enchant_checker_ = new EnchantChecker();
		singleton_->pimpl_->spell_checker_ = singleton_->pimpl_->enchant_checker_;
		return;
	}
#endif
#if defined(USE_HUNSPELL)
	if (lyxrc.spellchecker == "hunspell") {
		if (!singleton_->pimpl_->hunspell_checker_)
			singleton_->pimpl_->hunspell_checker_ = new HunspellChecker();
		singleton_->pimpl_->spell_checker_ = singleton_->pimpl_->hunspell_checker_;
		return;
	}
#endif
	singleton_->pimpl_->spell_checker_ = 0;
}

} // namespace lyx
