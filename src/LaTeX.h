// -*- C++ -*-
/* This file is part of
 * ======================================================
 *
 *           LyX, The Document Processor
 *	     Copyright 1995 Matthias Ettrich
 *           Copyright 1995-2001 The Lyx Team
 *
 *           This file is Copyright 1996-2001
 *           Lars Gullik Bj�nnes
 *
 * ======================================================
 */

#ifndef LATEX_H
#define LATEX_H

#include "LString.h"
#include "DepTable.h"
#include <vector>
#include <set>

#include <boost/utility.hpp>

class LyXFunc;

///
class TeXErrors {
private:
	///
	struct Error {
		///
		Error () : error_in_line(0) {}
		///
		Error(int line, string const & desc, string const & text)
			: error_in_line(line),
			  error_desc(desc),
			  error_text(text) {}
		/// what line in the TeX file the error occured in
		int error_in_line;
		/// The kind of error
		string error_desc;
		/// The line/cmd that caused the error.
		string error_text;
	};
public:
	///
	typedef std::vector<Error> Errors;
	///
	Errors::const_iterator begin() const { return errors.begin(); }
	///
	Errors::const_iterator end() const { return errors.end(); }
	///
	void insertError(int line, string const & error_desc,
			 string const & error_text);
private:
	///
	Errors errors;
};

class Aux_Info {
public:
	///
	Aux_Info() {}
	///
	string aux_file;
	///
	std::set<string> citations;
	///
	std::set<string> databases;
	///
	std::set<string> styles;
};


///
inline
bool operator==(Aux_Info const & a, Aux_Info const & o)
{
	return a.aux_file == o.aux_file &&
		a.citations == o.citations &&
		a.databases == o.databases &&
		a.styles == o.styles;
}


///
inline
bool operator!=(Aux_Info const & a, Aux_Info const & o)
{
	return !(a == o);
}


///
class LaTeX : boost::noncopyable {
public:
	/** Return values from scanLogFile() and run() (to come)

	    This enum should be enlarged a bit so that one could
	    get more feedback from the LaTeX run.
	*/
	enum log_status {
		///
		NO_ERRORS = 0,
		///
		NO_LOGFILE = 1,
		///
		NO_OUTPUT = 2,
		///
		UNDEF_REF = 4, // Reference '...' on page ... undefined.
		///
		UNDEF_CIT = 8, // Citation '...' on page ... undefined.
		///
		RERUN = 16, // Label(s) may have changed. Rerun to get...
		///
		TEX_ERROR = 32,
		///
		TEX_WARNING = 64,
		///
		LATEX_ERROR = 128,
		///
		LATEX_WARNING = 256,
		///
		PACKAGE_WARNING = 512,
		///
		NO_FILE = 1024,
		///
		NO_CHANGE = 2048,
		///
		TOO_MANY_ERRORS = 4096,
		///
		ERROR_RERUN = 8192,
		///
		ERRORS = TEX_ERROR + LATEX_ERROR,
		///
		WARNINGS = TEX_WARNING + LATEX_WARNING + PACKAGE_WARNING
	};


	/**
	   cmd = the latex command, file = name of the (temporary) latex file,
	   path = name of the files original path.
	*/
	LaTeX(string const & cmd, string const & file, string const & path);

	///
	virtual ~LaTeX() {}

	/// runs LaTeX several times
	int run(TeXErrors &, LyXFunc *);

	///
	int getNumErrors() { return num_errors;}

	/// use this for running LaTeX once
	int operator() ();

	///
	int scanLogFile(TeXErrors &);

protected:
	/// The dependency file.
	string depfile;

	///
	void deplog(DepTable & head);

	///
	bool runMakeIndex(string const &);

	///
	std::vector<Aux_Info> const scanAuxFiles(string const &);

	///
	Aux_Info const scanAuxFile(string const &);

	///
	void scanAuxFile(string const &, Aux_Info &);

	///
	void updateBibtexDependencies(DepTable &,
				      std::vector<Aux_Info> const &);

	///
	bool runBibTeX(std::vector<Aux_Info> const &);

	///
	void deleteFilesOnError() const;

	///
	string cmd;

	///
	string file;

	///
	string path;

	/// used by scanLogFile
	int num_errors;

	/// The name of the final output file.
	string output_file;
};

#endif
