/**
 * \file os_unix.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Ruurd A. Reitsma
 *
 * Full author contact details are available in file CREDITS.
 *
 * Various OS specific functions
 */

#include <config.h>

#include "support/os.h"
#include "support/docstring.h"
#include "support/FileName.h"
#include "support/lstrings.h"

#include <limits.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#endif

using namespace std;

namespace lyx {
namespace support {
namespace os {

namespace {

string stdoutdev_ = "/dev/stdout";
string stderrdev_ = "/dev/stderr";

} // namespace anon

void init(int, char *[])
{
	// Check whether /dev/stdout and /dev/stderr are available,
	// otherwise default to /dev/tty.
	if (access(stdoutdev_.c_str(), W_OK) != 0
	    || access(stderrdev_.c_str(), W_OK) != 0) {
		stdoutdev_ = "/dev/tty";
		stderrdev_ = "/dev/tty";
	}
}


string current_root()
{
	return "/";
}


bool isFilesystemCaseSensitive()
{
#ifdef __APPLE__
	return false;
#else
	return true;
#endif
}


docstring::size_type common_path(docstring const & p1, docstring const & p2)
{
	docstring::size_type i = 0;
	docstring::size_type const p1_len = p1.length();
	docstring::size_type const p2_len = p2.length();
	while (i < p1_len && i < p2_len && p1[i] == p2[i])
		++i;
	if ((i < p1_len && i < p2_len)
	    || (i < p1_len && p1[i] != '/' && i == p2_len)
	    || (i < p2_len && p2[i] != '/' && i == p1_len))
	{
		if (i)
			--i;     // here was the last match
		while (i && p1[i] != '/')
			--i;
	}
	return i;
}


string external_path(string const & p)
{
	return p;
}


string internal_path(string const & p)
{
	return p;
}


string external_path_list(string const & p)
{
	return p;
}


string internal_path_list(string const & p)
{
	return p;
}


string latex_path(string const & p)
{
	return p;
}


bool is_valid_strftime(string const & p)
{
	string::size_type pos = p.find_first_of('%');
	while (pos != string::npos) {
		if (pos + 1 == string::npos)
			break;
		if (!containsOnly(p.substr(pos + 1, 1),
			"aAbBcCdDeEFgGhHIjklmMnOpPrRsStTuUVwWxXyYzZ%+"))
			return false;
		if (pos + 2 == string::npos)
		      break;
		pos = p.find_first_of('%', pos + 2);
	}
	return true;
}


char const * popen_read_mode()
{
	return "r";
}


string const & nulldev()
{
	static string const nulldev_ = "/dev/null";
	return nulldev_;
}


string const & stdoutdev()
{
	return stdoutdev_;
}


string const & stderrdev()
{
	return stderrdev_;
}


bool is_terminal(io_channel channel)
{
	return isatty(channel);
}


shell_type shell()
{
	return UNIX;
}


char path_separator()
{
	return ':';
}


void windows_style_tex_paths(bool)
{}

bool canAutoOpenFile(string const & ext, auto_open_mode const mode)
{
#ifdef __APPLE__
// Reference: http://developer.apple.com/documentation/Carbon/Reference/LaunchServicesReference/
	CFStringRef cfs_ext = CFStringCreateWithBytes(kCFAllocatorDefault,
					(UInt8 *) ext.c_str(), ext.length(),
					kCFStringEncodingISOLatin1, false);
	// this is what we would like to do but it seems that the
	// viewer for PDF is often quicktime...
	//LSRolesMask role = (mode == VIEW) ? kLSRolesViewer :  kLSRolesEditor;
	(void)mode;
	LSRolesMask role = kLSRolesAll;
	FSRef outAppRef;
	OSStatus status =
		LSGetApplicationForInfo(kLSUnknownType, kLSUnknownCreator,
					cfs_ext, role, &outAppRef, NULL);
	CFRelease(cfs_ext);

	return status != kLSApplicationNotFoundErr;
#else
	// silence compiler warnings
	(void)ext;
	(void)mode;

	// currently, no default viewer is tried for non-windows system
	// support for KDE/Gnome/Macintosh may be added later
	return false;
#endif
}


bool autoOpenFile(string const & filename, auto_open_mode const mode)
{
#ifdef __APPLE__
// Reference: http://developer.apple.com/documentation/Carbon/Reference/LaunchServicesReference/
	FSRef fileref;
	OSStatus status =
		FSPathMakeRef((UInt8 *) filename.c_str(), &fileref, NULL);
	if (status != 0)
		return false;

	// this is what we would like to do but it seems that the
	// viewer for PDF is often quicktime...
	//LSRolesMask role = (mode == VIEW) ? kLSRolesViewer :  kLSRolesEditor;
	(void)mode;
	LSRolesMask role = kLSRolesAll;
	FSRef outAppRef;

	status = LSGetApplicationForItem(&fileref, role, &outAppRef, NULL);
	if (status == kLSApplicationNotFoundErr)
		return false;

	LSLaunchFSRefSpec inLaunchSpec;
	inLaunchSpec.appRef = &outAppRef;
	inLaunchSpec.numDocs = 1;
	inLaunchSpec.itemRefs = &fileref;
	inLaunchSpec.passThruParams = NULL;
	inLaunchSpec.launchFlags = kLSLaunchDefaults;
	inLaunchSpec.asyncRefCon = NULL;
	status = LSOpenFromRefSpec(&inLaunchSpec, NULL);

	return status != kLSApplicationNotFoundErr;
#else
	// silence compiler warnings
	(void)filename;
	(void)mode;

	// currently, no default viewer is tried for non-windows system
	// support for KDE/Gnome/Macintosh may be added later
	return false;
#endif
}


string real_path(string const & path)
{
	char rpath[PATH_MAX + 1];
	char * result = realpath(path.c_str(), rpath);
	return FileName::fromFilesystemEncoding(result ? rpath : path).absFilename();
}

} // namespace os
} // namespace support
} // namespace lyx
