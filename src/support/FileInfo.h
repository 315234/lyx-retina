// -*- C++ -*-
/**
 * \file FileInfo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/utility.hpp>
#include "LString.h"

namespace lyx {
namespace support {

/** Use objects of this class to get information about files.
 *
 * Users must make sure to check fi.isOK() before any operations
 * requiring the file to exist such as fi.isDir()
 */
class FileInfo : boost::noncopyable {
public:
	///
	FileInfo();

	/** Get information about file.
	    If link is true, the information is about the link itself, not
	    the file that is obtained by tracing the links. */
	explicit FileInfo(string const & path, bool link = false);

	/// File descriptor
	explicit FileInfo(int fildes);

	/// Query a new file
	FileInfo & newFile(string const & path, bool link = false);

	/// Query a new file descriptor
	FileInfo & newFile(int fildes);

	/// Returns a character describing file type (ls -F)
	char typeIndicator() const;

	/// File protection mode
	mode_t getMode() const;

	/// Constructs standard mode string (ls style)
	string modeString() const;

	///
	time_t getModificationTime() const;

	///
	time_t getAccessTime() const;

	///
	time_t getStatusChangeTime() const;

	/// Total file size in bytes
	off_t getSize() const;

	/// Number of hard links
	nlink_t getNumberOfLinks() const;

	/// User ID of owner
	uid_t getUid() const;

	/// Group ID of owner
	gid_t getGid() const;

	/// Is the file information correct? Did the query succeed?
	bool isOK() const;

	/// Permission flags
	enum perm_test {
		/// test for read permission
		rperm = R_OK,
		/// test for write permission
		wperm = W_OK,
		/// test for execute (search) permission
		xperm = X_OK,
		/// test for existence of file
		eperm = F_OK
	};
	/// Test whether the current user has a given set of permissions
	bool access(int p) const;
	/// Is the file writable for the current user?
	bool writable() const { return access(FileInfo::wperm); }
	/// Is the file readable for the current user?
	bool readable() const { return access(FileInfo::rperm); }
	/// Is the file executable for the current user?
	bool executable() const { return access(FileInfo::xperm); }
	/// Does the file exist?
	bool exist() const { return access(FileInfo::eperm); }
	///
	bool isLink() const;
	///
	bool isRegular() const;
	///
	bool isDir() const;
	///
	bool isChar() const;
	///
	bool isBlock() const;
	///
	bool isFifo() const;
	///
	bool isSocket() const;
	///
	int getError() const;
	///
	enum Err {
		///
		NoErr = -1
	};
private:
	///
	void init();
	///
	void dostat(bool);

	///
	struct stat buf_;
	///
	int status_;
	///
	int err_;
	///
	string fname_;
};

} // namespace support
} // namespace lyx

#endif
