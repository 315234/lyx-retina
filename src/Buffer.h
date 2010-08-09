// -*- C++ -*-
/**
 * \file Buffer.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "OutputEnums.h"

#include "insets/InsetCode.h"

#include "support/strfwd.h"
#include "support/types.h"

#include <list>
#include <set>
#include <string>
#include <vector>


namespace lyx {

class BiblioInfo;
class BufferParams;
class BufferSet;
class DispatchResult;
class DocIterator;
class docstring_list;
class ErrorList;
class FuncRequest;
class FuncStatus;
class Inset;
class InsetLabel;
class InsetRef;
class Font;
class Format;
class Lexer;
class Text;
class LyXVC;
class LaTeXFeatures;
class Language;
class MacroData;
class MacroNameSet;
class MacroSet;
class OutputParams;
class Paragraph;
class ParConstIterator;
class ParIterator;
class ParagraphList;
class TeXErrors;
class TexRow;
class TocBackend;
class Undo;
class WordLangTuple;

namespace frontend {
class GuiBufferDelegate;
class WorkAreaManager;
}

namespace support {
class FileName;
class FileNameList;
}

/** The buffer object.
 * This is the buffer object. It contains all the informations about
 * a document loaded into LyX.
 * The buffer object owns the Text (wrapped in an InsetText), which
 * contains the individual paragraphs of the document.
 *
 *
 * I am not sure if the class is complete or
 * minimal, probably not.
 * \author Lars Gullik Bjønnes
 */
class Buffer {
public:
	/// What type of log will \c getLogName() return?
	enum LogType {
		latexlog, ///< LaTeX log
		buildlog  ///< Literate build log
	};

	/// Result of \c readFile()
	enum ReadStatus {
		failure, ///< The file could not be read
		success, ///< The file could not be read
		wrongversion ///< The version of the file does not match ours
	};


	/// Method to check if a file is externally modified, used by
	/// isExternallyModified()
	/**
	 * timestamp is fast but inaccurate. For example, the granularity
	 * of timestamp on a FAT filesystem is 2 second. Also, various operations
	 * may touch the timestamp of a file even when its content is unchanged.
	 *
	 * checksum is accurate but slow, which can be a problem when it is
	 * frequently used, or used for a large file on a slow (network) file
	 * system.
	 *
	 * FIXME: replace this method with support/FileMonitor.
	 */
	enum CheckMethod {
		checksum_method,  ///< Use file checksum
		timestamp_method, ///< Use timestamp, and checksum if timestamp has changed
	};

	///
	enum UpdateScope {
		UpdateMaster,
		UpdateChildOnly
	};

	/// Constructor
	explicit Buffer(std::string const & file, bool readonly = false,
		Buffer const * cloned_buffer = 0);

	/// Destructor
	~Buffer();

	///
	Buffer * clone() const;
	///
	bool isClone() const;

	/** High-level interface to buffer functionality.
	    This function parses a command string and executes it.
	*/
	void dispatch(std::string const & command, DispatchResult & result);

	/// Maybe we know the function already by number...
	void dispatch(FuncRequest const & func, DispatchResult & result);

	/// Can this function be exectued?
	/// \return true if we made a decision
	bool getStatus(FuncRequest const & cmd, FuncStatus & flag);

	/// read a new document from a string
	bool readString(std::string const &);
	/// load a new file
	bool readFile(support::FileName const & filename);

	/// read the header, returns number of unknown tokens
	int readHeader(Lexer & lex);

	/** Reads a file without header.
	    \param par if != 0 insert the file.
	    \return \c true if file is not completely read.
	*/
	bool readDocument(Lexer &);

	///
	DocIterator getParFromID(int id) const;
	/// do we have a paragraph with this id?
	bool hasParWithID(int id) const;

	///
	frontend::WorkAreaManager & workAreaManager() const;

	/** Save file.
	    Takes care of auto-save files and backup file if requested.
	    Returns \c true if the save is successful, \c false otherwise.
	*/
	bool save() const;

	/// Write document to stream. Returns \c false if unsuccesful.
	bool write(std::ostream &) const;
	/// save emergency file
	/// \return a status message towards the user.
	docstring emergencyWrite();
	/// Write file. Returns \c false if unsuccesful.
	bool writeFile(support::FileName const &) const;

	/// Loads LyX file \c filename into buffer, *  and return success
	bool loadLyXFile(support::FileName const & s);
	/// Reloads the LyX file
	bool reload();

	/// Fill in the ErrorList with the TeXErrors
	void bufferErrors(TeXErrors const &, ErrorList &) const;

	/// Just a wrapper for writeLaTeXSource, first creating the ofstream.
	bool makeLaTeXFile(support::FileName const & filename,
			   std::string const & original_path,
			   OutputParams const &,
			   bool output_preamble = true,
			   bool output_body = true) const;
	/** Export the buffer to LaTeX.
	    If \p os is a file stream, and params().inputenc is "auto" or
	    "default", and the buffer contains text in different languages
	    with more than one encoding, then this method will change the
	    encoding associated to \p os. Therefore you must not call this
	    method with a string stream if the output is supposed to go to a
	    file. \code
	    ofdocstream ofs;
	    ofs.open("test.tex");
	    writeLaTeXSource(ofs, ...);
	    ofs.close();
	    \endcode is NOT equivalent to \code
	    odocstringstream oss;
	    writeLaTeXSource(oss, ...);
	    ofdocstream ofs;
	    ofs.open("test.tex");
	    ofs << oss.str();
	    ofs.close();
	    \endcode
	 */
	void writeLaTeXSource(odocstream & os,
			   std::string const & original_path,
			   OutputParams const &,
			   bool output_preamble = true,
			   bool output_body = true) const;
	///
	void makeDocBookFile(support::FileName const & filename,
			     OutputParams const & runparams_in,
			     bool only_body = false) const;
	///
	void writeDocBookSource(odocstream & os, std::string const & filename,
			     OutputParams const & runparams_in,
			     bool only_body = false) const;
	///
	void makeLyXHTMLFile(support::FileName const & filename,
			     OutputParams const & runparams_in,
			     bool only_body = false) const;
	///
	void writeLyXHTMLSource(odocstream & os,
			     OutputParams const & runparams_in,
			     bool only_body = false) const;
	/// returns the main language for the buffer (document)
	Language const * language() const;
	/// get l10n translated to the buffers language
	docstring const B_(std::string const & l10n) const;

	///
	int runChktex();
	/// return true if the main lyx file does not need saving
	bool isClean() const;
	///
	bool isDepClean(std::string const & name) const;

	/// whether or not disk file has been externally modified
	bool isExternallyModified(CheckMethod method) const;

	/// save timestamp and checksum of the given file.
	void saveCheckSum(support::FileName const & file) const;

	/// mark the main lyx file as not needing saving
	void markClean() const;

	///
	void markDepClean(std::string const & name);

	///
	void setUnnamed(bool flag = true);

	/// Whether or not a filename has been assigned to this buffer
	bool isUnnamed() const;

	/// Whether or not this buffer is internal.
	///
	/// An internal buffer does not contain a real document, but some auxiliary text segment.
	/// It is not associated with a filename, it is never saved, thus it does not need to be
	/// automatically saved, nor it needs to trigger any "do you want to save ?" question.
	bool isInternal() const;

	/// Mark this buffer as dirty.
	void markDirty();

	/// Returns the buffer's filename. It is always an absolute path.
	support::FileName fileName() const;

	/// Returns the buffer's filename. It is always an absolute path.
	std::string absFileName() const;

	/// Returns the the path where the buffer lives.
	/// It is always an absolute path.
	std::string filePath() const;

	/** A transformed version of the file name, adequate for LaTeX.
	    \param no_path optional if \c true then the path is stripped.
	*/
	std::string latexName(bool no_path = true) const;

	/// Get the name and type of the log.
	std::string logName(LogType * type = 0) const;

	/// Change name of buffer. Updates "read-only" flag.
	void setFileName(std::string const & newfile);

	/// Set document's parent Buffer.
	void setParent(Buffer const *);
	Buffer const * parent() const;

	// Collect all relative buffer
	std::vector<Buffer const *> allRelatives() const;

	/** Get the document's master (or \c this if this is not a
	    child document)
	 */
	Buffer const * masterBuffer() const;

	/// \return true if \p child is a child of this \c Buffer.
	bool isChild(Buffer * child) const;
	
	/// return a vector with all children (and grandchildren)
	std::vector<Buffer *> getChildren(bool grand_children = true) const;

	/// Add all children (and grandchildren) to supplied vector
	void getChildren(std::vector<Buffer *> & children, bool grand_children = true) const;

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

	/// Return the list with all bibfiles in use (including bibfiles
	/// of loaded child documents).
	support::FileNameList const & 
		getBibfilesCache(UpdateScope scope = UpdateMaster) const;
	/// Information from BibTeX databases is cached in the Buffer, so
	/// we do not have to read the file over and over. 
	/// Calling this method invalidates the cache and so requires a
	/// re-read.
	void invalidateBibinfoCache();
	/// This invalidates the cache of files we need to check.
	void invalidateBibfileCache();
	/// Updates the cached bibliography information.
	/// Note that you MUST call this method to update the cache. It will
	/// not happen otherwise. (Currently, it is called at the start of
	/// updateBuffer() and from GuiCitation.)
	void checkBibInfoCache() const;
	/// \return the bibliography information for this buffer's master,
	/// or just for it, if it isn't a child.
	BiblioInfo const & masterBibInfo() const;
	/// \return the bibliography information for this buffer ONLY.
	BiblioInfo const & localBibInfo() const;
	///
	void getLabelList(std::vector<docstring> &) const;

	///
	void changeLanguage(Language const * from, Language const * to);

	///
	bool isMultiLingual() const;
	///
	std::set<Language const *> getLanguages() const;

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
	std::string const temppath() const;

	/// Used when typesetting to place errorboxes.
	TexRow const & texrow() const;
	TexRow & texrow();

	///
	ParIterator par_iterator_begin();
	///
	ParConstIterator par_iterator_begin() const;
	///
	ParIterator par_iterator_end();
	///
	ParConstIterator par_iterator_end() const;

	// Position of the child buffer where it appears first in the master.
	DocIterator firstChildPosition(Buffer const * child);

	/** \returns true only when the file is fully loaded.
	 *  Used to prevent the premature generation of previews
	 *  and by the citation inset.
	 */
	bool isFullyLoaded() const;
	/// Set by buffer_funcs' newFile.
	void setFullyLoaded(bool);

	/// Our main text (inside the top InsetText)
	Text & text() const;

	/// Our top InsetText
	Inset & inset() const;

	//
	// Macro handling
	//
	/// Collect macro definitions in paragraphs
	void updateMacros() const;
	/// Iterate through the whole buffer and try to resolve macros
	void updateMacroInstances() const;

	/// List macro names of this buffer, the parent and the children
	void listMacroNames(MacroNameSet & macros) const;
	/// Collect macros of the parent and its children in front of this buffer.
	void listParentMacros(MacroSet & macros, LaTeXFeatures & features) const;

	/// Return macro defined before pos (or in the master buffer)
	MacroData const * getMacro(docstring const & name, DocIterator const & pos, bool global = true) const;
	/// Return macro defined anywhere in the buffer (or in the master buffer)
	MacroData const * getMacro(docstring const & name, bool global = true) const;
	/// Return macro defined before the inclusion of the child
	MacroData const * getMacro(docstring const & name, Buffer const & child, bool global = true) const;

	/// Collect user macro names at loading time
	typedef std::set<docstring> UserMacroSet;
	UserMacroSet usermacros;

	/// Replace the inset contents for insets which InsetCode is equal
	/// to the passed \p inset_code.
	void changeRefsIfUnique(docstring const & from, docstring const & to,
		InsetCode code);

	/// get source code (latex/docbook) for some paragraphs, or all paragraphs
	/// including preamble
	void getSourceCode(odocstream & os, pit_type par_begin, pit_type par_end,
		bool full_source) const;

	/// Access to error list.
	/// This method is used only for GUI visualisation of Buffer related
	/// errors (like parsing or LateX compilation). This method is const
	/// because modifying the returned ErrorList does not touch the document
	/// contents.
	ErrorList & errorList(std::string const & type) const;

	/// The Toc backend.
	/// This is useful only for screen visualisation of the Buffer. This
	/// method is const because modifying this backend does not touch
	/// the document contents.
	TocBackend & tocBackend() const;

	///
	Undo & undo();

	/// This function is called when the buffer is changed.
	void changed(bool update_metrics) const;
	///
	void setChild(DocIterator const & dit, Buffer * child);
	///
	void updateTocItem(std::string const &, DocIterator const &) const;
	/// This function is called when the buffer structure is changed.
	void structureChanged() const;
	/// This function is called when some parsing error shows up.
	void errors(std::string const & err, bool from_master = false) const;
	/// This function is called when the buffer busy status change.
	void setBusy(bool on) const;
	/// Update window titles of all users.
	void updateTitles() const;
	/// Reset autosave timers for all users.
	void resetAutosaveTimers() const;
	///
	void message(docstring const & msg) const;

	///
	void setGuiDelegate(frontend::GuiBufferDelegate * gui);
	///
	bool hasGuiDelegate() const;

	///
	void autoSave() const;
	///
	void removeAutosaveFile() const;
	///
	void moveAutosaveFile(support::FileName const & old) const;
	///
	support::FileName getAutosaveFileName() const;

	/// return the format of the buffer on a string
	std::string bufferFormat() const;
	/// return the default output format of the current backend
	std::string getDefaultOutputFormat() const;

	///
	bool doExport(std::string const & format, bool put_in_tempdir,
		bool includeall, std::string & result_file) const;
	///
	bool doExport(std::string const & format, bool put_in_tempdir,
		      bool includeall = false) const;
	///
	bool preview(std::string const & format, bool includeall = false) const;
	///
	bool isExportable(std::string const & format) const;
	///
	std::vector<Format const *> exportableFormats(bool only_viewable) const;
	///
	bool isExportableFormat(std::string const & format) const;
	/// mark the buffer as busy exporting something, or not
	void setExportStatus(bool e) const;
	///
	bool isExporting() const;

	///
	typedef std::vector<std::pair<Inset *, ParIterator> > References;
	References & references(docstring const & label);
	References const & references(docstring const & label) const;
	void clearReferenceCache() const;
	void setInsetLabel(docstring const & label, InsetLabel const * il);
	InsetLabel const * insetLabel(docstring const & label) const;

	/// return a list of all used branches (also in children)
	void getUsedBranches(std::list<docstring> &, bool const from_master = false) const;

	/// sets the buffer_ member for every inset in this buffer.
	// FIXME This really shouldn't be needed, but at the moment it's not
	// clear how to do it just for the individual pieces we need.
	void setBuffersForInsets() const;
	/// Updates screen labels and some other information associated with
	/// insets and paragraphs. Actually, it's more like a general "recurse
	/// through the Buffer" routine, that visits all the insets and paragraphs.
	void updateBuffer() const { updateBuffer(UpdateMaster, InternalUpdate); }
	/// \param scope: whether to start with the master document or just
	/// do this one.
	/// \param output: whether we are preparing for output.
	void updateBuffer(UpdateScope scope, UpdateType utype) const;
	/// 
	void updateBuffer(ParIterator & parit, UpdateType utype) const;

	/// Spellcheck starting from \p from.
	/// \p from initial position, will then points to the next misspelled
	///    word.
	/// \p to will points to the end of the next misspelled word.
	/// \p word_lang will contain the found misspelled word.
	/// \return progress if a new word was found.
	int spellCheck(DocIterator & from, DocIterator & to,
		WordLangTuple & word_lang, docstring_list & suggestions) const;
	///
	void checkChildBuffers();

private:
	///
	bool readFileHelper(support::FileName const & s);
	///
	std::vector<std::string> backends() const;
	/** Inserts a file into a document
	    \return \c false if method fails.
	*/
	ReadStatus readFile(Lexer &, support::FileName const & filename,
			    bool fromString = false);
	///
	void getLanguages(std::set<Language const *> &) const;
	/// Update the list of all bibfiles in use (including bibfiles
	/// of loaded child documents).
	void updateBibfilesCache(UpdateScope scope = UpdateMaster) const;

	/// Use the Pimpl idiom to hide the internals.
	class Impl;
	/// The pointer never changes although *pimpl_'s contents may.
	Impl * const d;
};


} // namespace lyx

#endif
