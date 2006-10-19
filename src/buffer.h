// -*- C++ -*-
/**
 * \file buffer.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "errorlist.h"
#include "InsetList.h"

#include "dociterator.h"
#include "ParagraphList_fwd.h"

#include "support/limited_stack.h"
#include "support/types.h"
#include "support/docstring.h"
#include "support/docstream.h"

#include <boost/scoped_ptr.hpp>
#include <boost/signal.hpp>

#include <iosfwd>
#include <string>
#include <map>
#include <utility>
#include <vector>


class BufferParams;
class ErrorItem;
class FuncRequest;
class InsetBase;
class InsetText;
class LyXFont;
class LyXLex;
class LyXRC;
class LyXText;
class LyXVC;
class LaTeXFeatures;
class Language;
class MacroData;
class OutputParams;
class ParConstIterator;
class ParIterator;
class TeXErrors;
class TexRow;
class Undo;
class StableDocIterator;


/** The buffer object.
 * This is the buffer object. It contains all the informations about
 * a document loaded into LyX.
 * The buffer object owns the LyXText (wrapped in an InsetText), which
 * contains the individual paragraphs of the document.
 *
 *
 * I am not sure if the class is complete or
 * minimal, probably not.
 * \author Lars Gullik Bj�nnes
  */
class Buffer {
public:
	/// What type of log will \c getLogName() return?
	enum LogType {
		latexlog, ///< LaTeX log
		buildlog  ///< Literate build log
	};

	/** Constructor
	    \param file
	    \param b  optional \c false by default
	*/
	explicit Buffer(std::string const & file, bool b = false);

	/// Destructor
	~Buffer();

	/** High-level interface to buffer functionality.
	    This function parses a command string and executes it
	*/
	bool dispatch(std::string const & command, bool * result = 0);

	/// Maybe we know the function already by number...
	bool dispatch(FuncRequest const & func, bool * result = 0);

	/// Load the autosaved file.
	void loadAutoSaveFile();

	/// load a new file
	bool readFile(std::string const & filename);

	/// read the header, returns number of unknown tokens
	int readHeader(LyXLex & lex);

	/** Reads a file without header.
	    \param par if != 0 insert the file.
	    \return \c false if file is not completely read.
	*/
	bool readDocument(LyXLex &);

	///
	void insertStringAsLines(ParagraphList & plist,
		lyx::pit_type &, lyx::pos_type &,
		LyXFont const &, lyx::docstring const &, bool);
	///
	ParIterator getParFromID(int id) const;
	/// do we have a paragraph with this id?
	bool hasParWithID(int id) const;

	/// This signal is emitted when some parsing error shows up.
	boost::signal<void(std::string)> errors;
	/// This signal is emitted when some message shows up.
	boost::signal<void(lyx::docstring)> message;
	/// This signal is emitted when the buffer busy status change.
	boost::signal<void(bool)> busy;
	/// This signal is emitted when the buffer readonly status change.
	boost::signal<void(bool)> readonly;
	/// Update window titles of all users.
	boost::signal<void()> updateTitles;
	/// Reset autosave timers for all users.
	boost::signal<void()> resetAutosaveTimers;
	/// This signal is emitting if the buffer is being closed.
	boost::signal<void()> closing;


	/** Save file.
	    Takes care of auto-save files and backup file if requested.
	    Returns \c true if the save is successful, \c false otherwise.
	*/
	bool save() const;

	/// Write file. Returns \c false if unsuccesful.
	bool writeFile(std::string const &) const;

	/// Just a wrapper for the method below, first creating the ofstream.
	void makeLaTeXFile(std::string const & filename,
			   std::string const & original_path,
			   OutputParams const &,
			   bool output_preamble = true,
			   bool output_body = true);
	///
	void writeLaTeXSource(lyx::odocstream & os,
			   std::string const & original_path,
			   OutputParams const &,
			   bool output_preamble = true,
			   bool output_body = true);
	///
	void makeDocBookFile(std::string const & filename,
			     OutputParams const & runparams_in,
			     bool only_body = false);
	///
	void writeDocBookSource(lyx::odocstream & os, std::string const & filename,
			     OutputParams const & runparams_in,
			     bool only_body = false);
	/// returns the main language for the buffer (document)
	Language const * getLanguage() const;
	/// get l10n translated to the buffers language
	lyx::docstring const B_(std::string const & l10n) const;

	///
	int runChktex();
	/// return true if the main lyx file does not need saving
	bool isClean() const;
	///
	bool isBakClean() const;
	///
	bool isDepClean(std::string const & name) const;

	/// mark the main lyx file as not needing saving
	void markClean() const;

	///
	void markBakClean();

	///
	void markDepClean(std::string const & name);

	///
	void setUnnamed(bool flag = true);

	///
	bool isUnnamed() const;

	/// Mark this buffer as dirty.
	void markDirty();

	/// Returns the buffer's filename. It is always an absolute path.
	std::string const & fileName() const;

	/// Returns the the path where the buffer lives.
	/// It is always an absolute path.
	std::string const & filePath() const;

	/** A transformed version of the file name, adequate for LaTeX.
	    \param no_path optional if \c true then the path is stripped.
	*/
	std::string const getLatexName(bool no_path = true) const;

	/// Get the name and type of the log.
	std::pair<LogType, std::string> const getLogName() const;

	/// Change name of buffer. Updates "read-only" flag.
	void setFileName(std::string const & newfile);

	/// Name of the document's parent
	void setParentName(std::string const &);

	/** Get the document's master (or \c this if this is not a
	    child document)
	 */
	Buffer const * getMasterBuffer() const;
	/** Get the document's master (or \c this if this is not a
	    child document)
	 */
	Buffer * getMasterBuffer();

	/// Is buffer read-only?
	bool isReadonly() const;

	/// Set buffer read-only flag
	void setReadonly(bool flag = true);

	/// returns \c true if the buffer contains a LaTeX document
	bool isLatex() const;
	/// returns \c true if the buffer contains a DocBook document
	bool isDocBook() const;
	/// returns \c true if the buffer contains a Wed document
	bool isLiterate() const;

	/** Validate a buffer for LaTeX.
	    This validates the buffer, and returns a struct for use by
	    #makeLaTeX# and others. Its main use is to figure out what
	    commands and packages need to be included in the LaTeX file.
	    It (should) also check that the needed constructs are there
	    (i.e. that the \refs points to coresponding \labels). It
	    should perhaps inset "error" insets to help the user correct
	    obvious mistakes.
	*/
	void validate(LaTeXFeatures &) const;

	/// return all bibkeys from buffer and its childs
	void fillWithBibKeys(std::vector<std::pair<std::string, std::string> > & keys) const;
	/// Update the cache with all bibfiles in use (including bibfiles
	/// of loaded child documents).
	void updateBibfilesCache();
	/// Return the cache with all bibfiles in use (including bibfiles
	/// of loaded child documents).
	std::vector<std::string> const & getBibfilesCache() const;
	///
	void getLabelList(std::vector<lyx::docstring> &) const;

	///
	void changeLanguage(Language const * from, Language const * to);

	///
	void updateDocLang(Language const * nlang);

	///
	bool isMultiLingual() const;

	/// Does this mean that this is buffer local?
	limited_stack<Undo> & undostack();
	limited_stack<Undo> const & undostack() const;

	/// Does this mean that this is buffer local?
	limited_stack<Undo> & redostack();
	limited_stack<Undo> const & redostack() const;

	///
	BufferParams & params();
	BufferParams const & params() const;

	/** The list of paragraphs.
	    This is a linked list of paragraph, this list holds the
	    whole contents of the document.
	 */
	ParagraphList & paragraphs();
	ParagraphList const & paragraphs() const;

	/// LyX version control object.
	LyXVC & lyxvc();
	LyXVC const & lyxvc() const;

	/// Where to put temporary files.
	std::string const & temppath() const;

	/// Used when typesetting to place errorboxes.
	TexRow & texrow();
	TexRow const & texrow() const;

	///
	ParIterator par_iterator_begin();
	///
	ParConstIterator par_iterator_begin() const;
	///
	ParIterator par_iterator_end();
	///
	ParConstIterator par_iterator_end() const;

	/** \returns true only when the file is fully loaded.
	 *  Used to prevent the premature generation of previews
	 *  and by the citation inset.
	 */
	bool fully_loaded() const;
	/// Set by buffer_funcs' newFile.
	void fully_loaded(bool);

	/// Our main text (inside the top InsetText)
	LyXText & text() const;

	/// Our top InsetText!
	InsetBase & inset() const;

	//
	// Macro handling
	//
	///
	void buildMacros();
	///
	bool hasMacro(std::string const & name) const;
	///
	MacroData const & getMacro(std::string const & name) const;
	///
	void insertMacro(std::string const & name, MacroData const & data);
	///
	void saveCursor(StableDocIterator cursor, StableDocIterator anchor);
	///
	StableDocIterator getCursor() const { return cursor_; }
	///
	StableDocIterator getAnchor() const { return anchor_; }
	///
	void changeRefsIfUnique(std::string const & from, std::string const & to, InsetBase::Code code);
	/// get source code (latex/docbook) for some paragraphs, or all paragraphs
	/// including preamble
	void getSourceCode(lyx::odocstream & os, lyx::pit_type par_begin, lyx::pit_type par_end, bool full_source);

	/// errorLists_ accessors.
	//@{
	ErrorList const & errorList(std::string const & type) const;
	ErrorList & errorList(std::string const & type);
	//@}

private:
	/** Inserts a file into a document
	    \return \c false if method fails.
	*/
	bool readFile(LyXLex &, std::string const & filename);

	bool do_writeFile(std::ostream & ofs) const;

	/// Use the Pimpl idiom to hide the internals.
	class Impl;
	/// The pointer never changes although *pimpl_'s contents may.
	boost::scoped_ptr<Impl> const pimpl_;

	/// Save the cursor Position on Buffer switch
	/// this would not be needed if every Buffer would have
	/// it's BufferView, this should be FIXED in future.
	StableDocIterator cursor_;
	StableDocIterator anchor_;
	/// A cache for the bibfiles (including bibfiles of loaded child
	/// documents), needed for appropriate update of natbib labels.
	std::vector<std::string> bibfilesCache_;

	/// Container for all sort of Buffer dependant errors.
	std::map<std::string, ErrorList> errorLists_;
};

#endif
