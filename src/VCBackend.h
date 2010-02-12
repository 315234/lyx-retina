// -*- C++ -*-
/**
 * \file VCBackend.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Pavel Sanda
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef VC_BACKEND_H
#define VC_BACKEND_H

#include "support/FileName.h"

#include <string>

#include "LyXVC.h"


namespace lyx {

class Buffer;

/// A simple version control system interface
class VCS {
public:
	/// the status of the managed file
	enum VCStatus {
		UNLOCKED,
		LOCKED,
		NOLOCKING
	};

	virtual ~VCS() {}

	/// register a file for version control
	virtual void registrer(std::string const & msg) = 0;
	/// check in the current revision, returns log
	virtual std::string checkIn(std::string const & msg) = 0;
	// can be this operation processed in the current RCS?
	virtual bool checkInEnabled() = 0;
	/// check out for editing, returns log
	virtual std::string checkOut() = 0;
	// can be this operation processed in the current RCS?
	virtual bool checkOutEnabled() = 0;
	/// synchronize with repository, returns log
	virtual std::string repoUpdate() = 0;
	// can be this operation processed in the current RCS?
	virtual bool repoUpdateEnabled() = 0;
	// toggle locking property of the file
	virtual std::string lockingToggle() = 0;
	// can be this operation processed in the current RCS?
	virtual bool lockingToggleEnabled() = 0;
	/// revert current edits
	virtual void revert() = 0;
	/// FIXME
	virtual void undoLast() = 0;
	// can be this operation processed in the current RCS?
	virtual bool undoLastEnabled() = 0;
	/**
	 * getLog - read the revision log into the given file
	 * @param fname file name to read into
	 */
	virtual void getLog(support::FileName const &) = 0;
	/// return the current version description
	virtual std::string const versionString() const = 0;
	/// set the owning buffer
	void owner(Buffer * b) { owner_ = b; }
	/// return the owning buffer
	Buffer * owner() const { return owner_; }
	/// return the lock status of this file
	VCStatus status() const { return vcstatus; }
	/// do we need special handling for read-only toggling?
	/// (also used for check-out operation)
	virtual bool toggleReadOnlyEnabled() = 0;
	/// Return revision info specified by the argument.
	virtual std::string revisionInfo(LyXVC::RevisionInfo const info) = 0;
protected:
	/// parse information from the version file
	virtual void scanMaster() = 0;

	// GUI container for doVCCommandCall
	int doVCCommand(std::string const & cmd, support::FileName const & path);
	/**
	 * doVCCommandCall - call out to the version control utility
	 * @param cmd the command to execute
	 * @param path the path from which to execute
	 * @return exit status
	 */
	static int doVCCommandCall(std::string const & cmd, support::FileName const & path);

	/**
	 * The master VC file. For RCS this is *,v or RCS/ *,v. master should
	 * have full path.
	 */
	support::FileName master_;

	/// The status of the VC controlled file.
	VCStatus vcstatus;

	/// The buffer using this VC
	Buffer * owner_;
};


///
class RCS : public VCS {
public:

	explicit
	RCS(support::FileName const & m);

	/// return the revision file for the given file, if found
	static support::FileName const findFile(support::FileName const & file);

	static void retrieve(support::FileName const & file);

	virtual void registrer(std::string const & msg);

	virtual std::string checkIn(std::string const & msg);

	virtual bool checkInEnabled();

	virtual std::string checkOut();

	virtual bool checkOutEnabled();

	virtual std::string repoUpdate();

	virtual bool repoUpdateEnabled();

	virtual std::string lockingToggle();

	virtual bool lockingToggleEnabled();

	virtual void revert();

	virtual void undoLast();

	virtual bool undoLastEnabled();

	virtual void getLog(support::FileName const &);

	virtual std::string const versionString() const {
		return "RCS: " + version_;
	}

	virtual bool toggleReadOnlyEnabled();

	virtual std::string revisionInfo(LyXVC::RevisionInfo const info);

protected:
	virtual void scanMaster();
private:
	/**
	 * The version of the VC file. I am not sure if this can be a
	 * string or if it must be a float/int.
	 */
	std::string version_;
	/// The user currently keeping the lock on the VC file (or "Unlocked").
	std::string locker_;
};


///
class CVS : public VCS {
public:
	///
	explicit
	CVS(support::FileName const & m, support::FileName const & f);

	/// return the revision file for the given file, if found
	static support::FileName const findFile(support::FileName const & file);

	virtual void registrer(std::string const & msg);

	virtual std::string checkIn(std::string const & msg);

	virtual bool checkInEnabled();

	virtual std::string checkOut();

	virtual bool checkOutEnabled();

	virtual std::string repoUpdate();

	virtual bool repoUpdateEnabled();

	virtual std::string lockingToggle();

	virtual bool lockingToggleEnabled();

	virtual void revert();

	virtual void undoLast();

	virtual bool undoLastEnabled();

	virtual void getLog(support::FileName const &);

	virtual std::string const versionString() const {
		return "CVS: " + version_;
	}

	virtual bool toggleReadOnlyEnabled();

	virtual std::string revisionInfo(LyXVC::RevisionInfo const info);

protected:
	virtual void scanMaster();

private:
	support::FileName file_;
	// revision number from scanMaster
	std::string version_;
	/// The user currently keeping the lock on the VC file.
	std::string locker_;
};


///
class SVN : public VCS {
public:
	///
	explicit
	SVN(support::FileName const & m, support::FileName const & f);

	/// return the revision file for the given file, if found
	static support::FileName const findFile(support::FileName const & file);

	virtual void registrer(std::string const & msg);

	virtual std::string checkIn(std::string const & msg);

	virtual bool checkInEnabled();

	virtual std::string checkOut();

	virtual bool checkOutEnabled();

	virtual std::string repoUpdate();

	virtual bool repoUpdateEnabled();

	virtual std::string lockingToggle();

	virtual bool lockingToggleEnabled();

	virtual void revert();

	virtual void undoLast();

	virtual bool undoLastEnabled();

	virtual void getLog(support::FileName const &);

	virtual std::string const versionString() const {
		return "SVN: " + rev_file_cache_;
	}

	virtual bool toggleReadOnlyEnabled();

	virtual std::string revisionInfo(LyXVC::RevisionInfo const info);

protected:
	virtual void scanMaster();
	/// Check for messages in svn output. Returns error.
	std::string scanLogFile(support::FileName const & f, std::string & status);
	/// checks locking policy and setup locked_mode_
	bool checkLockMode();
	/// is the loaded file locked?
	bool isLocked() const;
	/// acquire/release write lock for the current file
	void fileLock(bool lock, support::FileName const & tmpf, std::string & status);
	/**
	 * Prepare revision rev of the file into newly created temporary file
	 * and save the filename into parameter f.
	 * Parameter rev can be either revision number or negative number
	 * which is interpreted as how many revision back from the current
	 * one do we want. rev=0 is reserved for the last (committed) revision.
	 */
	bool prepareFileRevision(int rev, std::string & f);

private:
	support::FileName file_;
	/// is the loaded file under locking policy?
	bool locked_mode_;
	/**
	 * Real code for obtaining file revision info. Fills all file-related caches
	 * and returns true if successfull.
	 * "?" is stored in rev_file_cache_ as a signal if request for obtaining info
	 * was already unsuccessful.
	 */
	bool getFileRevisionInfo();
	/// cache for file revision number, "?" if already unsuccessful, isNumber==true
	std::string rev_file_cache_;
	/// cache for author of last commit
	std::string rev_author_cache_;
	/// cache for date of last commit
	std::string rev_date_cache_;
	/// cache for time of last commit
	std::string rev_time_cache_;
	/// fills rev_tree_cache_, returns true if successfull.
	bool getTreeRevisionInfo();
	/// cache for tree revision number, "?" if already unsuccessful
	std::string rev_tree_cache_;
};

} // namespace lyx

#endif // VCBACKEND_H
