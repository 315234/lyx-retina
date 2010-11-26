/**
 * \file Buffer.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Stefan Schimanski
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Buffer.h"

#include "Author.h"
#include "LayoutFile.h"
#include "BiblioInfo.h"
#include "BranchList.h"
#include "buffer_funcs.h"
#include "BufferList.h"
#include "BufferParams.h"
#include "Bullet.h"
#include "Chktex.h"
#include "Converter.h"
#include "Counters.h"
#include "DispatchResult.h"
#include "DocIterator.h"
#include "Encoding.h"
#include "ErrorList.h"
#include "Exporter.h"
#include "Format.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "IndicesList.h"
#include "InsetIterator.h"
#include "InsetList.h"
#include "Language.h"
#include "LaTeXFeatures.h"
#include "LaTeX.h"
#include "Layout.h"
#include "Lexer.h"
#include "LyXAction.h"
#include "LyX.h"
#include "LyXRC.h"
#include "LyXVC.h"
#include "output_docbook.h"
#include "output.h"
#include "output_latex.h"
#include "output_xhtml.h"
#include "output_plaintext.h"
#include "Paragraph.h"
#include "ParagraphParameters.h"
#include "ParIterator.h"
#include "PDFOptions.h"
#include "SpellChecker.h"
#include "sgml.h"
#include "TexRow.h"
#include "TexStream.h"
#include "Text.h"
#include "TextClass.h"
#include "TocBackend.h"
#include "Undo.h"
#include "VCBackend.h"
#include "version.h"
#include "WordLangTuple.h"
#include "WordList.h"

#include "insets/InsetBibitem.h"
#include "insets/InsetBibtex.h"
#include "insets/InsetBranch.h"
#include "insets/InsetInclude.h"
#include "insets/InsetTabular.h"
#include "insets/InsetText.h"

#include "mathed/InsetMathHull.h"
#include "mathed/MacroTable.h"
#include "mathed/MathMacroTemplate.h"
#include "mathed/MathSupport.h"

#include "frontends/alert.h"
#include "frontends/Delegates.h"
#include "frontends/WorkAreaManager.h"

#include "graphics/Previews.h"

#include "support/lassert.h"
#include "support/convert.h"
#include "support/debug.h"
#include "support/docstring_list.h"
#include "support/ExceptionMessage.h"
#include "support/FileName.h"
#include "support/FileNameList.h"
#include "support/filetools.h"
#include "support/ForkedCalls.h"
#include "support/gettext.h"
#include "support/gzstream.h"
#include "support/lstrings.h"
#include "support/lyxalgo.h"
#include "support/os.h"
#include "support/Package.h"
#include "support/Path.h"
#include "support/Systemcall.h"
#include "support/textutils.h"
#include "support/types.h"

#include "support/bind.h"
#include "support/shared_ptr.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <vector>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace Alert = frontend::Alert;
namespace os = support::os;

namespace {

// Do not remove the comment below, so we get merge conflict in
// independent branches. Instead add your own.
int const LYX_FORMAT = 410; // rgh: dummy format for list->labeling

typedef map<string, bool> DepClean;
typedef map<docstring, pair<InsetLabel const *, Buffer::References> > RefCache;

void showPrintError(string const & name)
{
	docstring str = bformat(_("Could not print the document %1$s.\n"
					    "Check that your printer is set up correctly."),
			     makeDisplayPath(name, 50));
	Alert::error(_("Print document failed"), str);
}

} // namespace anon


class Buffer::Impl
{
public:
	Impl(Buffer * owner, FileName const & file, bool readonly, Buffer const * cloned_buffer);

	~Impl()
	{
		if (wa_) {
			wa_->closeAll();
			delete wa_;
		}
		delete inset;
	}
	
	/// search for macro in local (buffer) table or in children
	MacroData const * getBufferMacro(docstring const & name,
		DocIterator const & pos) const;

	/// Update macro table starting with position of it \param it in some
	/// text inset.
	void updateMacros(DocIterator & it, DocIterator & scope);
	///
	void setLabel(ParIterator & it, UpdateType utype) const;

	/** If we have branches that use the file suffix
	    feature, return the file name with suffix appended.
	*/
	support::FileName exportFileName() const;

	Buffer * owner_;

	BufferParams params;
	LyXVC lyxvc;
	FileName temppath;
	mutable TexRow texrow;

	/// need to regenerate .tex?
	DepClean dep_clean;

	/// is save needed?
	mutable bool lyx_clean;

	/// is autosave needed?
	mutable bool bak_clean;

	/// is this an unnamed file (New...)?
	bool unnamed;

	/// buffer is r/o
	bool read_only;

	/// name of the file the buffer is associated with.
	FileName filename;

	/** Set to true only when the file is fully loaded.
	 *  Used to prevent the premature generation of previews
	 *  and by the citation inset.
	 */
	bool file_fully_loaded;

	///
	mutable TocBackend toc_backend;

	/// macro tables
	typedef pair<DocIterator, MacroData> ScopeMacro;
	typedef map<DocIterator, ScopeMacro> PositionScopeMacroMap;
	typedef map<docstring, PositionScopeMacroMap> NamePositionScopeMacroMap;
	/// map from the macro name to the position map,
	/// which maps the macro definition position to the scope and the MacroData.
	NamePositionScopeMacroMap macros;
	/// This seem to change the way Buffer::getMacro() works
	mutable bool macro_lock;

	/// positions of child buffers in the buffer
	typedef map<Buffer const * const, DocIterator> BufferPositionMap;
	typedef pair<DocIterator, Buffer const *> ScopeBuffer;
	typedef map<DocIterator, ScopeBuffer> PositionScopeBufferMap;
	/// position of children buffers in this buffer
	BufferPositionMap children_positions;
	/// map from children inclusion positions to their scope and their buffer
	PositionScopeBufferMap position_to_children;

	/// Container for all sort of Buffer dependant errors.
	map<string, ErrorList> errorLists;

	/// timestamp and checksum used to test if the file has been externally
	/// modified. (Used to properly enable 'File->Revert to saved', bug 4114).
	time_t timestamp_;
	unsigned long checksum_;

	///
	frontend::WorkAreaManager * wa_;
	///
	frontend::GuiBufferDelegate * gui_;

	///
	Undo undo_;

	/// A cache for the bibfiles (including bibfiles of loaded child
	/// documents), needed for appropriate update of natbib labels.
	mutable support::FileNameList bibfiles_cache_;

	// FIXME The caching mechanism could be improved. At present, we have a
	// cache for each Buffer, that caches all the bibliography info for that
	// Buffer. A more efficient solution would be to have a global cache per
	// file, and then to construct the Buffer's bibinfo from that.
	/// A cache for bibliography info
	mutable BiblioInfo bibinfo_;
	/// whether the bibinfo cache is valid
	mutable bool bibinfo_cache_valid_;
	/// whether the bibfile cache is valid
	mutable bool bibfile_cache_valid_;
	/// Cache of timestamps of .bib files
	map<FileName, time_t> bibfile_status_;

	mutable RefCache ref_cache_;

	/// our Text that should be wrapped in an InsetText
	InsetText * inset;

	/// This is here to force the test to be done whenever parent_buffer
	/// is accessed.
	Buffer const * parent() const { 
		// if parent_buffer is not loaded, then it has been unloaded,
		// which means that parent_buffer is an invalid pointer. So we
		// set it to null in that case.
		// however, the BufferList doesn't know about cloned buffers, so
		// they will always be regarded as unloaded. in that case, we hope
		// for the best.
		if (!cloned_buffer_ && !theBufferList().isLoaded(parent_buffer))
			parent_buffer = 0;
		return parent_buffer; 
	}
	
	///
	void setParent(Buffer const * pb) {
		if (parent_buffer == pb)
			// nothing to do
			return;
		if (!cloned_buffer_ && parent_buffer && pb)
			LYXERR0("Warning: a buffer should not have two parents!");
		parent_buffer = pb;
		if (!cloned_buffer_ && parent_buffer) {
			parent_buffer->invalidateBibfileCache();
			parent_buffer->invalidateBibinfoCache();
		}
	}

	/// If non zero, this buffer is a clone of existing buffer \p cloned_buffer_
	/// This one is useful for preview detached in a thread.
	Buffer const * cloned_buffer_;
	/// are we in the process of exporting this buffer?
	mutable bool doing_export;
	
private:
	/// So we can force access via the accessors.
	mutable Buffer const * parent_buffer;

};


/// Creates the per buffer temporary directory
static FileName createBufferTmpDir()
{
	static int count;
	// We are in our own directory.  Why bother to mangle name?
	// In fact I wrote this code to circumvent a problematic behaviour
	// (bug?) of EMX mkstemp().
	FileName tmpfl(package().temp_dir().absFileName() + "/lyx_tmpbuf" +
		convert<string>(count++));

	if (!tmpfl.createDirectory(0777)) {
		throw ExceptionMessage(WarningException, _("Disk Error: "), bformat(
			_("LyX could not create the temporary directory '%1$s' (Disk is full maybe?)"),
			from_utf8(tmpfl.absFileName())));
	}
	return tmpfl;
}


Buffer::Impl::Impl(Buffer * owner, FileName const & file, bool readonly_,
	Buffer const * cloned_buffer)
	: owner_(owner), lyx_clean(true), bak_clean(true), unnamed(false),
	  read_only(readonly_), filename(file), file_fully_loaded(false),
	  toc_backend(owner), macro_lock(false), timestamp_(0),
	  checksum_(0), wa_(0), gui_(0), undo_(*owner), bibinfo_cache_valid_(false),
		bibfile_cache_valid_(false), cloned_buffer_(cloned_buffer), 
		doing_export(false), parent_buffer(0)
{
	if (!cloned_buffer_) {
		temppath = createBufferTmpDir();
		lyxvc.setBuffer(owner_);
		if (use_gui)
			wa_ = new frontend::WorkAreaManager;
		return;
	}
	temppath = cloned_buffer_->d->temppath;
	file_fully_loaded = true;
	params = cloned_buffer_->d->params;
	bibfiles_cache_ = cloned_buffer_->d->bibfiles_cache_;
	bibinfo_ = cloned_buffer_->d->bibinfo_;
	bibinfo_cache_valid_ = cloned_buffer_->d->bibinfo_cache_valid_;
	bibfile_cache_valid_ = cloned_buffer_->d->bibfile_cache_valid_;
	bibfile_status_ = cloned_buffer_->d->bibfile_status_;
}


Buffer::Buffer(string const & file, bool readonly, Buffer const * cloned_buffer)
	: d(new Impl(this, FileName(file), readonly, cloned_buffer))
{
	LYXERR(Debug::INFO, "Buffer::Buffer()");
	if (cloned_buffer) {
		d->inset = new InsetText(*cloned_buffer->d->inset);
		d->inset->setBuffer(*this);
		// FIXME: optimize this loop somewhat, maybe by creating a new
		// general recursive Inset::setId().
		DocIterator it = doc_iterator_begin(this);
		DocIterator cloned_it = doc_iterator_begin(cloned_buffer);
		for (; !it.atEnd(); it.forwardPar(), cloned_it.forwardPar())
			it.paragraph().setId(cloned_it.paragraph().id());
	} else
		d->inset = new InsetText(this);
	d->inset->setAutoBreakRows(true);
	d->inset->getText(0)->setMacrocontextPosition(par_iterator_begin());
}


Buffer::~Buffer()
{
	LYXERR(Debug::INFO, "Buffer::~Buffer()");
	// here the buffer should take care that it is
	// saved properly, before it goes into the void.

	// GuiView already destroyed
	d->gui_ = 0;

	if (isInternal()) {
		// No need to do additional cleanups for internal buffer.
		delete d;
		return;
	}

	// loop over children
	Impl::BufferPositionMap::iterator it = d->children_positions.begin();
	Impl::BufferPositionMap::iterator end = d->children_positions.end();
	for (; it != end; ++it) {
		Buffer * child = const_cast<Buffer *>(it->first);
		if (d->cloned_buffer_)
			delete child;
		// The child buffer might have been closed already.
		else if (theBufferList().isLoaded(child))
			theBufferList().releaseChild(this, child);
	}

	if (!isClean()) {
		docstring msg = _("LyX attempted to close a document that had unsaved changes!\n");
		msg += emergencyWrite();
		Alert::warning(_("Attempting to close changed document!"), msg);
	}
		
	// clear references to children in macro tables
	d->children_positions.clear();
	d->position_to_children.clear();

	if (!d->cloned_buffer_ && !d->temppath.destroyDirectory()) {
		Alert::warning(_("Could not remove temporary directory"),
			bformat(_("Could not remove the temporary directory %1$s"),
			from_utf8(d->temppath.absFileName())));
	}

	if (!isClone())
		removePreviews();

	delete d;
}


Buffer * Buffer::clone() const
{
	Buffer * buffer_clone = new Buffer(fileName().absFileName(), false, this);
	buffer_clone->d->macro_lock = true;
	buffer_clone->d->children_positions.clear();
	// FIXME (Abdel 09/01/2010): this is too complicated. The whole children_positions and
	// math macro caches need to be rethought and simplified.
	// I am not sure wether we should handle Buffer cloning here or in BufferList.
	// Right now BufferList knows nothing about buffer clones.
	Impl::BufferPositionMap::iterator it = d->children_positions.begin();
	Impl::BufferPositionMap::iterator end = d->children_positions.end();
	for (; it != end; ++it) {
		DocIterator dit = it->second.clone(buffer_clone);
		dit.setBuffer(buffer_clone);
		Buffer * child = const_cast<Buffer *>(it->first);
		Buffer * child_clone = child->clone();
		Inset * inset = dit.nextInset();
		LASSERT(inset && inset->lyxCode() == INCLUDE_CODE, continue);
		InsetInclude * inset_inc = static_cast<InsetInclude *>(inset);
		inset_inc->setChildBuffer(child_clone);
		child_clone->d->setParent(buffer_clone);
		buffer_clone->setChild(dit, child_clone);
	}
	buffer_clone->d->macro_lock = false;
	return buffer_clone;
}


bool Buffer::isClone() const
{
	return d->cloned_buffer_;
}


void Buffer::changed(bool update_metrics) const
{
	if (d->wa_)
		d->wa_->redrawAll(update_metrics);
}


frontend::WorkAreaManager & Buffer::workAreaManager() const
{
	LASSERT(d->wa_, /**/);
	return *d->wa_;
}


Text & Buffer::text() const
{
	return d->inset->text();
}


Inset & Buffer::inset() const
{
	return *d->inset;
}


BufferParams & Buffer::params()
{
	return d->params;
}


BufferParams const & Buffer::params() const
{
	return d->params;
}


ParagraphList & Buffer::paragraphs()
{
	return text().paragraphs();
}


ParagraphList const & Buffer::paragraphs() const
{
	return text().paragraphs();
}


LyXVC & Buffer::lyxvc()
{
	return d->lyxvc;
}


LyXVC const & Buffer::lyxvc() const
{
	return d->lyxvc;
}


string const Buffer::temppath() const
{
	return d->temppath.absFileName();
}


TexRow & Buffer::texrow()
{
	return d->texrow;
}


TexRow const & Buffer::texrow() const
{
	return d->texrow;
}


TocBackend & Buffer::tocBackend() const
{
	return d->toc_backend;
}


Undo & Buffer::undo()
{
	return d->undo_;
}


void Buffer::setChild(DocIterator const & dit, Buffer * child)
{
	d->children_positions[child] = dit;
}


string Buffer::latexName(bool const no_path) const
{
	FileName latex_name =
		makeLatexName(d->exportFileName());
	return no_path ? latex_name.onlyFileName()
		: latex_name.absFileName();
}


FileName Buffer::Impl::exportFileName() const
{
	docstring const branch_suffix =
		params.branchlist().getFileNameSuffix();
	if (branch_suffix.empty())
		return filename;

	string const name = filename.onlyFileNameWithoutExt()
		+ to_utf8(branch_suffix);
	FileName res(filename.onlyPath().absFileName() + "/" + name);
	res.changeExtension(filename.extension());

	return res;
}


string Buffer::logName(LogType * type) const
{
	string const filename = latexName(false);

	if (filename.empty()) {
		if (type)
			*type = latexlog;
		return string();
	}

	string const path = temppath();

	FileName const fname(addName(temppath(),
				     onlyFileName(changeExtension(filename,
								  ".log"))));

	// FIXME: how do we know this is the name of the build log?
	FileName const bname(
		addName(path, onlyFileName(
			changeExtension(filename,
					formats.extension(bufferFormat()) + ".out"))));

	// Also consider the master buffer log file
	FileName masterfname = fname;
	LogType mtype;
	if (masterBuffer() != this) {
		string const mlogfile = masterBuffer()->logName(&mtype);
		masterfname = FileName(mlogfile);
	}

	// If no Latex log or Build log is newer, show Build log
	if (bname.exists() &&
	    ((!fname.exists() && !masterfname.exists())
	     || (fname.lastModified() < bname.lastModified()
	         && masterfname.lastModified() < bname.lastModified()))) {
		LYXERR(Debug::FILES, "Log name calculated as: " << bname);
		if (type)
			*type = buildlog;
		return bname.absFileName();
	// If we have a newer master file log or only a master log, show this
	} else if (fname != masterfname
		   && (!fname.exists() && (masterfname.exists()
		   || fname.lastModified() < masterfname.lastModified()))) {
		LYXERR(Debug::FILES, "Log name calculated as: " << masterfname);
		if (type)
			*type = mtype;
		return masterfname.absFileName();
	}
	LYXERR(Debug::FILES, "Log name calculated as: " << fname);
	if (type)
			*type = latexlog;
	return fname.absFileName();
}


void Buffer::setReadonly(bool const flag)
{
	if (d->read_only != flag) {
		d->read_only = flag;
		changed(false);
	}
}


void Buffer::setFileName(FileName const & fname)
{
	d->filename = fname;
	setReadonly(d->filename.isReadOnly());
	saveCheckSum();
	updateTitles();
}


int Buffer::readHeader(Lexer & lex)
{
	int unknown_tokens = 0;
	int line = -1;
	int begin_header_line = -1;

	// Initialize parameters that may be/go lacking in header:
	params().branchlist().clear();
	params().preamble.erase();
	params().options.erase();
	params().master.erase();
	params().float_placement.erase();
	params().paperwidth.erase();
	params().paperheight.erase();
	params().leftmargin.erase();
	params().rightmargin.erase();
	params().topmargin.erase();
	params().bottommargin.erase();
	params().headheight.erase();
	params().headsep.erase();
	params().footskip.erase();
	params().columnsep.erase();
	params().fontsCJK.erase();
	params().listings_params.clear();
	params().clearLayoutModules();
	params().clearRemovedModules();
	params().clearIncludedChildren();
	params().pdfoptions().clear();
	params().indiceslist().clear();
	params().backgroundcolor = lyx::rgbFromHexName("#ffffff");
	params().isbackgroundcolor = false;
	params().fontcolor = lyx::rgbFromHexName("#000000");
	params().isfontcolor = false;
	params().notefontcolor = lyx::rgbFromHexName("#cccccc");
	params().boxbgcolor = lyx::rgbFromHexName("#ff0000");
	params().html_latex_start.clear();
	params().html_latex_end.clear();
	params().html_math_img_scale = 1.0;
	params().output_sync_macro.erase();

	for (int i = 0; i < 4; ++i) {
		params().user_defined_bullet(i) = ITEMIZE_DEFAULTS[i];
		params().temp_bullet(i) = ITEMIZE_DEFAULTS[i];
	}

	ErrorList & errorList = d->errorLists["Parse"];

	while (lex.isOK()) {
		string token;
		lex >> token;

		if (token.empty())
			continue;

		if (token == "\\end_header")
			break;

		++line;
		if (token == "\\begin_header") {
			begin_header_line = line;
			continue;
		}

		LYXERR(Debug::PARSER, "Handling document header token: `"
				      << token << '\'');

		string unknown = params().readToken(lex, token, d->filename.onlyPath());
		if (!unknown.empty()) {
			if (unknown[0] != '\\' && token == "\\textclass") {
				Alert::warning(_("Unknown document class"),
		       bformat(_("Using the default document class, because the "
					      "class %1$s is unknown."), from_utf8(unknown)));
			} else {
				++unknown_tokens;
				docstring const s = bformat(_("Unknown token: "
									"%1$s %2$s\n"),
							 from_utf8(token),
							 lex.getDocString());
				errorList.push_back(ErrorItem(_("Document header error"),
					s, -1, 0, 0));
			}
		}
	}
	if (begin_header_line) {
		docstring const s = _("\\begin_header is missing");
		errorList.push_back(ErrorItem(_("Document header error"),
			s, -1, 0, 0));
	}

	params().makeDocumentClass();

	return unknown_tokens;
}


// Uwe C. Schroeder
// changed to be public and have one parameter
// Returns true if "\end_document" is not read (Asger)
bool Buffer::readDocument(Lexer & lex)
{
	ErrorList & errorList = d->errorLists["Parse"];
	errorList.clear();

	// remove dummy empty par
	paragraphs().clear();

	if (!lex.checkFor("\\begin_document")) {
		docstring const s = _("\\begin_document is missing");
		errorList.push_back(ErrorItem(_("Document header error"),
			s, -1, 0, 0));
	}

	readHeader(lex);

	if (params().outputChanges) {
		bool dvipost    = LaTeXFeatures::isAvailable("dvipost");
		bool xcolorulem = LaTeXFeatures::isAvailable("ulem") &&
				  LaTeXFeatures::isAvailable("xcolor");

		if (!dvipost && !xcolorulem) {
			Alert::warning(_("Changes not shown in LaTeX output"),
				       _("Changes will not be highlighted in LaTeX output, "
					 "because neither dvipost nor xcolor/ulem are installed.\n"
					 "Please install these packages or redefine "
					 "\\lyxadded and \\lyxdeleted in the LaTeX preamble."));
		} else if (!xcolorulem) {
			Alert::warning(_("Changes not shown in LaTeX output"),
				       _("Changes will not be highlighted in LaTeX output "
					 "when using pdflatex, because xcolor and ulem are not installed.\n"
					 "Please install both packages or redefine "
					 "\\lyxadded and \\lyxdeleted in the LaTeX preamble."));
		}
	}

	if (!params().master.empty()) {
		FileName const master_file = makeAbsPath(params().master,
			   onlyPath(absFileName()));
		if (isLyXFileName(master_file.absFileName())) {
			Buffer * master = 
				checkAndLoadLyXFile(master_file, true);
			if (master) {
				// necessary e.g. after a reload
				// to re-register the child (bug 5873)
				// FIXME: clean up updateMacros (here, only
				// child registering is needed).
				master->updateMacros();
				// set master as master buffer, but only
				// if we are a real child
				if (master->isChild(this))
					setParent(master);
				// if the master is not fully loaded
				// it is probably just loading this
				// child. No warning needed then.
				else if (master->isFullyLoaded())
					LYXERR0("The master '"
						<< params().master
						<< "' assigned to this document ("
						<< absFileName()
						<< ") does not include "
						"this document. Ignoring the master assignment.");
			}
		}
	}
	
	// assure we have a default index
	params().indiceslist().addDefault(B_("Index"));

	// read main text
	bool const res = text().read(lex, errorList, d->inset);

	usermacros.clear();
	updateMacros();
	updateMacroInstances();
	return res;
}


bool Buffer::readString(string const & s)
{
	params().compressed = false;

	Lexer lex;
	istringstream is(s);
	lex.setStream(is);
	FileName const fn = FileName::tempName("Buffer_readString");

	int file_format;
	ReadStatus const ret_plf = parseLyXFormat(lex, fn, file_format);
	if (ret_plf != ReadSuccess)
		return ret_plf;

	if (file_format != LYX_FORMAT) {
		// We need to call lyx2lyx, so write the input to a file
		ofstream os(fn.toFilesystemEncoding().c_str());
		os << s;
		os.close();
		// lyxvc in readFile
		return readFile(fn) == ReadSuccess;
	}

	if (readDocument(lex))
		return false;
	return true;
}


Buffer::ReadStatus Buffer::readFile(FileName const & fn)
{
	FileName fname(fn);
	Lexer lex;
	lex.setFile(fname);

	int file_format;
	ReadStatus const ret_plf = parseLyXFormat(lex, fn, file_format);
	if (ret_plf != ReadSuccess)
		return ret_plf;

	if (file_format != LYX_FORMAT) {
		FileName tmpFile;
		ReadStatus const ret_clf = convertLyXFormat(fn, tmpFile, file_format);
		if (ret_clf != ReadSuccess)
			return ret_clf;
		return readFile(tmpFile);
	}

	// FIXME: InsetInfo needs to know whether the file is under VCS 
	// during the parse process, so this has to be done before.
	lyxvc().file_found_hook(d->filename);

	if (readDocument(lex)) {
		Alert::error(_("Document format failure"),
			bformat(_("%1$s ended unexpectedly, which means"
				" that it is probably corrupted."),
					from_utf8(fn.absFileName())));
		return ReadDocumentFailure;
	}

	d->file_fully_loaded = true;
	d->read_only = !d->filename.isWritable();
	params().compressed = d->filename.isZippedFile();
	saveCheckSum();
	return ReadSuccess;
}


bool Buffer::isFullyLoaded() const
{
	return d->file_fully_loaded;
}


void Buffer::setFullyLoaded(bool value)
{
	d->file_fully_loaded = value;
}


void Buffer::updatePreviews() const
{
	if (graphics::Previews::status() != LyXRC::PREVIEW_OFF)
		thePreviews().generateBufferPreviews(*this);
}


void Buffer::removePreviews() const
{
	thePreviews().removeLoader(*this);
}


Buffer::ReadStatus Buffer::parseLyXFormat(Lexer & lex,
	FileName const & fn, int & file_format) const
{
	if(!lex.checkFor("\\lyxformat")) {
		Alert::error(_("Document format failure"),
			bformat(_("%1$s is not a readable LyX document."),
				from_utf8(fn.absFileName())));
		return ReadNoLyXFormat;
	}	

	string tmp_format;
	lex >> tmp_format;

	// LyX formats 217 and earlier were written as 2.17. This corresponds
	// to files from LyX versions < 1.1.6.3. We just remove the dot in
	// these cases. See also: www.lyx.org/trac/changeset/1313.
	size_t dot = tmp_format.find_first_of(".,");
	if (dot != string::npos)
		tmp_format.erase(dot, 1);

	file_format = convert<int>(tmp_format);
	return ReadSuccess;
}


Buffer::ReadStatus Buffer::convertLyXFormat(FileName const & fn, 
	FileName & tmpfile, int from_format)
{
	tmpfile = FileName::tempName("Buffer_convertLyXFormat");
	if(tmpfile.empty()) {
		Alert::error(_("Conversion failed"),
			bformat(_("%1$s is from a different"
				" version of LyX, but a temporary"
				" file for converting it could"
				" not be created."),
				from_utf8(fn.absFileName())));
		return LyX2LyXNoTempFile;
	}

	FileName const lyx2lyx = libFileSearch("lyx2lyx", "lyx2lyx");
	if (lyx2lyx.empty()) {
		Alert::error(_("Conversion script not found"),
		     bformat(_("%1$s is from a different"
			       " version of LyX, but the"
			       " conversion script lyx2lyx"
			       " could not be found."),
			       from_utf8(fn.absFileName())));
		return LyX2LyXNotFound;
	}

	// Run lyx2lyx:
	//   $python$ "$lyx2lyx$" -t $LYX_FORMAT$ -o "$tempfile$" "$filetoread$"
	ostringstream command;
	command << os::python()
		<< ' ' << quoteName(lyx2lyx.toFilesystemEncoding())
		<< " -t " << convert<string>(LYX_FORMAT)
		<< " -o " << quoteName(tmpfile.toFilesystemEncoding())
		<< ' ' << quoteName(fn.toSafeFilesystemEncoding());
	string const command_str = command.str();

	LYXERR(Debug::INFO, "Running '" << command_str << '\'');

	cmd_ret const ret = runCommand(command_str);
	if (ret.first != 0) {
		if (from_format < LYX_FORMAT) {
			Alert::error(_("Conversion script failed"),
				bformat(_("%1$s is from an older version"
					" of LyX and the lyx2lyx script"
					" failed to convert it."),
					from_utf8(fn.absFileName())));
			return LyX2LyXOlderFormat;
		} else {
			Alert::error(_("Conversion script failed"),
				bformat(_("%1$s is from a newer version"
					" of LyX and the lyx2lyx script"
					" failed to convert it."),
					from_utf8(fn.absFileName())));
			return LyX2LyXNewerFormat;
		}
	}
	return ReadSuccess;
}


// Should probably be moved to somewhere else: BufferView? GuiView?
bool Buffer::save() const
{
	docstring const file = makeDisplayPath(absFileName(), 20);
	d->filename.refresh();

	// check the read-only status before moving the file as a backup
	if (d->filename.exists()) {
		bool const read_only = !d->filename.isWritable();
		if (read_only) {
			Alert::warning(_("File is read-only"),
				bformat(_("The file %1$s cannot be written because it "
				"is marked as read-only."), file));
			return false;
		}
	}

	// ask if the disk file has been externally modified (use checksum method)
	if (fileName().exists() && isExternallyModified(checksum_method)) {
		docstring text = 
			bformat(_("Document %1$s has been externally modified. "
				"Are you sure you want to overwrite this file?"), file);
		int const ret = Alert::prompt(_("Overwrite modified file?"),
			text, 1, 1, _("&Overwrite"), _("&Cancel"));
		if (ret == 1)
			return false;
	}

	// We don't need autosaves in the immediate future. (Asger)
	resetAutosaveTimers();

	FileName backupName;
	bool madeBackup = false;

	// make a backup if the file already exists
	if (lyxrc.make_backup && fileName().exists()) {
		backupName = FileName(absFileName() + '~');
		if (!lyxrc.backupdir_path.empty()) {
			string const mangledName =
				subst(subst(backupName.absFileName(), '/', '!'), ':', '!');
			backupName = FileName(addName(lyxrc.backupdir_path,
						      mangledName));
		}
		// do not copy because of #6587
		if (fileName().moveTo(backupName)) {
			madeBackup = true;
		} else {
			Alert::error(_("Backup failure"),
				     bformat(_("Cannot create backup file %1$s.\n"
					       "Please check whether the directory exists and is writeable."),
					     from_utf8(backupName.absFileName())));
			//LYXERR(Debug::DEBUG, "Fs error: " << fe.what());
		}
	}

	if (writeFile(d->filename)) {
		markClean();
		return true;
	} else {
		// Saving failed, so backup is not backup
		if (madeBackup)
			backupName.moveTo(d->filename);
		return false;
	}
}


bool Buffer::writeFile(FileName const & fname) const
{
	if (d->read_only && fname == d->filename)
		return false;

	bool retval = false;

	docstring const str = bformat(_("Saving document %1$s..."),
		makeDisplayPath(fname.absFileName()));
	message(str);

	string const encoded_fname = fname.toSafeFilesystemEncoding(os::CREATE);

	if (params().compressed) {
		gz::ogzstream ofs(encoded_fname.c_str(), ios::out|ios::trunc);
		retval = ofs && write(ofs);
	} else {
		ofstream ofs(encoded_fname.c_str(), ios::out|ios::trunc);
		retval = ofs && write(ofs);
	}

	if (!retval) {
		message(str + _(" could not write file!"));
		return false;
	}

	// see bug 6587
	// removeAutosaveFile();

	saveCheckSum();
	message(str + _(" done."));

	return true;
}


docstring Buffer::emergencyWrite()
{
	// No need to save if the buffer has not changed.
	if (isClean())
		return docstring();

	string const doc = isUnnamed() ? onlyFileName(absFileName()) : absFileName();

	docstring user_message = bformat(
		_("LyX: Attempting to save document %1$s\n"), from_utf8(doc));

	// We try to save three places:
	// 1) Same place as document. Unless it is an unnamed doc.
	if (!isUnnamed()) {
		string s = absFileName();
		s += ".emergency";
		LYXERR0("  " << s);
		if (writeFile(FileName(s))) {
			markClean();
			user_message += bformat(_("  Saved to %1$s. Phew.\n"), from_utf8(s));
			return user_message;
		} else {
			user_message += _("  Save failed! Trying again...\n");
		}
	}

	// 2) In HOME directory.
	string s = addName(package().home_dir().absFileName(), absFileName());
	s += ".emergency";
	lyxerr << ' ' << s << endl;
	if (writeFile(FileName(s))) {
		markClean();
		user_message += bformat(_("  Saved to %1$s. Phew.\n"), from_utf8(s));
		return user_message;
	}

	user_message += _("  Save failed! Trying yet again...\n");

	// 3) In "/tmp" directory.
	// MakeAbsPath to prepend the current
	// drive letter on OS/2
	s = addName(package().temp_dir().absFileName(), absFileName());
	s += ".emergency";
	lyxerr << ' ' << s << endl;
	if (writeFile(FileName(s))) {
		markClean();
		user_message += bformat(_("  Saved to %1$s. Phew.\n"), from_utf8(s));
		return user_message;
	}

	user_message += _("  Save failed! Bummer. Document is lost.");
	// Don't try again.
	markClean();
	return user_message;
}


bool Buffer::write(ostream & ofs) const
{
#ifdef HAVE_LOCALE
	// Use the standard "C" locale for file output.
	ofs.imbue(locale::classic());
#endif

	// The top of the file should not be written by params().

	// write out a comment in the top of the file
	ofs << "#LyX " << lyx_version
	    << " created this file. For more info see http://www.lyx.org/\n"
	    << "\\lyxformat " << LYX_FORMAT << "\n"
	    << "\\begin_document\n";

	/// For each author, set 'used' to true if there is a change
	/// by this author in the document; otherwise set it to 'false'.
	AuthorList::Authors::const_iterator a_it = params().authors().begin();
	AuthorList::Authors::const_iterator a_end = params().authors().end();
	for (; a_it != a_end; ++a_it)
		a_it->setUsed(false);

	ParIterator const end = const_cast<Buffer *>(this)->par_iterator_end();
	ParIterator it = const_cast<Buffer *>(this)->par_iterator_begin();
	for ( ; it != end; ++it)
		it->checkAuthors(params().authors());

	// now write out the buffer parameters.
	ofs << "\\begin_header\n";
	params().writeFile(ofs);
	ofs << "\\end_header\n";

	// write the text
	ofs << "\n\\begin_body\n";
	text().write(ofs);
	ofs << "\n\\end_body\n";

	// Write marker that shows file is complete
	ofs << "\\end_document" << endl;

	// Shouldn't really be needed....
	//ofs.close();

	// how to check if close went ok?
	// Following is an attempt... (BE 20001011)

	// good() returns false if any error occured, including some
	//        formatting error.
	// bad()  returns true if something bad happened in the buffer,
	//        which should include file system full errors.

	bool status = true;
	if (!ofs) {
		status = false;
		lyxerr << "File was not closed properly." << endl;
	}

	return status;
}


bool Buffer::makeLaTeXFile(FileName const & fname,
			   string const & original_path,
			   OutputParams const & runparams_in,
			   bool output_preamble, bool output_body) const
{
	OutputParams runparams = runparams_in;

	string const encoding = runparams.encoding->iconvName();
	LYXERR(Debug::LATEX, "makeLaTeXFile encoding: " << encoding << "...");

	ofdocstream ofs;
	try { ofs.reset(encoding); }
	catch (iconv_codecvt_facet_exception & e) {
		lyxerr << "Caught iconv exception: " << e.what() << endl;
		Alert::error(_("Iconv software exception Detected"), bformat(_("Please "
			"verify that the support software for your encoding (%1$s) is "
			"properly installed"), from_ascii(encoding)));
		return false;
	}
	if (!openFileWrite(ofs, fname))
		return false;

	//TexStream ts(ofs.rdbuf(), &texrow());
	ErrorList & errorList = d->errorLists["Export"];
	errorList.clear();
	bool failed_export = false;
	try {
		d->texrow.reset();
		writeLaTeXSource(ofs, original_path,
		      runparams, output_preamble, output_body);
	}
	catch (EncodingException & e) {
		odocstringstream ods;
		ods.put(e.failed_char);
		ostringstream oss;
		oss << "0x" << hex << e.failed_char << dec;
		docstring msg = bformat(_("Could not find LaTeX command for character '%1$s'"
					  " (code point %2$s)"),
					  ods.str(), from_utf8(oss.str()));
		errorList.push_back(ErrorItem(msg, _("Some characters of your document are probably not "
				"representable in the chosen encoding.\n"
				"Changing the document encoding to utf8 could help."),
				e.par_id, e.pos, e.pos + 1));
		failed_export = true;
	}
	catch (iconv_codecvt_facet_exception & e) {
		errorList.push_back(ErrorItem(_("iconv conversion failed"),
			_(e.what()), -1, 0, 0));
		failed_export = true;
	}
	catch (exception const & e) {
		errorList.push_back(ErrorItem(_("conversion failed"),
			_(e.what()), -1, 0, 0));
		failed_export = true;
	}
	catch (...) {
		lyxerr << "Caught some really weird exception..." << endl;
		lyx_exit(1);
	}

	ofs.close();
	if (ofs.fail()) {
		failed_export = true;
		lyxerr << "File '" << fname << "' was not closed properly." << endl;
	}

	errors("Export");
	return !failed_export;
}


void Buffer::writeLaTeXSource(odocstream & os,
			   string const & original_path,
			   OutputParams const & runparams_in,
			   bool const output_preamble, bool const output_body) const
{
	// The child documents, if any, shall be already loaded at this point.

	OutputParams runparams = runparams_in;

	// Classify the unicode characters appearing in math insets
	Encodings::initUnicodeMath(*this);

	// validate the buffer.
	LYXERR(Debug::LATEX, "  Validating buffer...");
	LaTeXFeatures features(*this, params(), runparams);
	validate(features);
	LYXERR(Debug::LATEX, "  Buffer validation done.");

	// The starting paragraph of the coming rows is the
	// first paragraph of the document. (Asger)
	if (output_preamble && runparams.nice) {
		os << "%% LyX " << lyx_version << " created this file.  "
			"For more info, see http://www.lyx.org/.\n"
			"%% Do not edit unless you really know what "
			"you are doing.\n";
		d->texrow.newline();
		d->texrow.newline();
	}
	LYXERR(Debug::INFO, "lyx document header finished");

	// Don't move this behind the parent_buffer=0 code below,
	// because then the macros will not get the right "redefinition"
	// flag as they don't see the parent macros which are output before.
	updateMacros();

	// fold macros if possible, still with parent buffer as the
	// macros will be put in the prefix anyway.
	updateMacroInstances();

	// There are a few differences between nice LaTeX and usual files:
	// usual is \batchmode and has a
	// special input@path to allow the including of figures
	// with either \input or \includegraphics (what figinsets do).
	// input@path is set when the actual parameter
	// original_path is set. This is done for usual tex-file, but not
	// for nice-latex-file. (Matthias 250696)
	// Note that input@path is only needed for something the user does
	// in the preamble, included .tex files or ERT, files included by
	// LyX work without it.
	if (output_preamble) {
		if (!runparams.nice) {
			// code for usual, NOT nice-latex-file
			os << "\\batchmode\n"; // changed
			// from \nonstopmode
			d->texrow.newline();
		}
		if (!original_path.empty()) {
			// FIXME UNICODE
			// We don't know the encoding of inputpath
			docstring const inputpath = from_utf8(support::latex_path(original_path));
			docstring uncodable_glyphs;
			Encoding const * const enc = runparams.encoding;
			if (enc) {
				for (size_t n = 0; n < inputpath.size(); ++n) {
					docstring const glyph =
						docstring(1, inputpath[n]);
					if (enc->latexChar(inputpath[n], true) != glyph) {
						LYXERR0("Uncodable character '"
							<< glyph
							<< "' in input path!");
						uncodable_glyphs += glyph;
					}
				}
			}

			// warn user if we found uncodable glyphs.
			if (!uncodable_glyphs.empty()) {
				frontend::Alert::warning(_("Uncodable character in file path"),
						support::bformat(_("The path of your document\n"
						  "(%1$s)\n"
						  "contains glyphs that are unknown in the\n"
						  "current document encoding (namely %2$s).\n"
						  "This will likely result in incomplete output.\n\n"
						  "Choose an appropriate document encoding (such as utf8)\n"
						  "or change the file path name."), inputpath, uncodable_glyphs));
			} else {
				os << "\\makeatletter\n"
				   << "\\def\\input@path{{"
				   << inputpath << "/}}\n"
				   << "\\makeatother\n";
				d->texrow.newline();
				d->texrow.newline();
				d->texrow.newline();
			}
		}

		// get parent macros (if this buffer has a parent) which will be
		// written at the document begin further down.
		MacroSet parentMacros;
		listParentMacros(parentMacros, features);

		runparams.use_polyglossia = features.usePolyglossia();
		// Write the preamble
		runparams.use_babel = params().writeLaTeX(os, features,
							  d->texrow,
							  d->filename.onlyPath());

		runparams.use_japanese = features.isRequired("japanese");

		if (!output_body)
			return;

		// make the body.
		os << "\\begin{document}\n";
		d->texrow.newline();

		// output the parent macros
		MacroSet::iterator it = parentMacros.begin();
		MacroSet::iterator end = parentMacros.end();
		for (; it != end; ++it) {
			int num_lines = (*it)->write(os, true);
			d->texrow.newlines(num_lines);
		}
		
	} // output_preamble

	d->texrow.start(paragraphs().begin()->id(), 0);

	LYXERR(Debug::INFO, "preamble finished, now the body.");

	// if we are doing a real file with body, even if this is the
	// child of some other buffer, let's cut the link here.
	// This happens for example if only a child document is printed.
	Buffer const * save_parent = 0;
	if (output_preamble) {
		save_parent = d->parent();
		d->setParent(0);
	}

	// the real stuff
	latexParagraphs(*this, text(), os, d->texrow, runparams);

	// Restore the parenthood if needed
	if (output_preamble)
		d->setParent(save_parent);

	// add this just in case after all the paragraphs
	os << endl;
	d->texrow.newline();

	if (output_preamble) {
		os << "\\end{document}\n";
		d->texrow.newline();
		LYXERR(Debug::LATEX, "makeLaTeXFile...done");
	} else {
		LYXERR(Debug::LATEX, "LaTeXFile for inclusion made.");
	}
	runparams_in.encoding = runparams.encoding;

	// Just to be sure. (Asger)
	d->texrow.newline();

	//for (int i = 0; i<d->texrow.rows(); i++) {
	// int id,pos;
	// if (d->texrow.getIdFromRow(i+1,id,pos) && id>0)
	//	lyxerr << i+1 << ":" << id << ":" << getParFromID(id).paragraph().asString()<<"\n";
	//}

	LYXERR(Debug::INFO, "Finished making LaTeX file.");
	LYXERR(Debug::INFO, "Row count was " << d->texrow.rows() - 1 << '.');
}


bool Buffer::isLatex() const
{
	return params().documentClass().outputType() == LATEX;
}


bool Buffer::isLiterate() const
{
	return params().documentClass().outputType() == LITERATE;
}


bool Buffer::isDocBook() const
{
	return params().documentClass().outputType() == DOCBOOK;
}


void Buffer::makeDocBookFile(FileName const & fname,
			      OutputParams const & runparams,
			      bool const body_only) const
{
	LYXERR(Debug::LATEX, "makeDocBookFile...");

	ofdocstream ofs;
	if (!openFileWrite(ofs, fname))
		return;

	writeDocBookSource(ofs, fname.absFileName(), runparams, body_only);

	ofs.close();
	if (ofs.fail())
		lyxerr << "File '" << fname << "' was not closed properly." << endl;
}


void Buffer::writeDocBookSource(odocstream & os, string const & fname,
			     OutputParams const & runparams,
			     bool const only_body) const
{
	LaTeXFeatures features(*this, params(), runparams);
	validate(features);

	d->texrow.reset();

	DocumentClass const & tclass = params().documentClass();
	string const top_element = tclass.latexname();

	if (!only_body) {
		if (runparams.flavor == OutputParams::XML)
			os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

		// FIXME UNICODE
		os << "<!DOCTYPE " << from_ascii(top_element) << ' ';

		// FIXME UNICODE
		if (! tclass.class_header().empty())
			os << from_ascii(tclass.class_header());
		else if (runparams.flavor == OutputParams::XML)
			os << "PUBLIC \"-//OASIS//DTD DocBook XML//EN\" "
			    << "\"http://www.oasis-open.org/docbook/xml/4.2/docbookx.dtd\"";
		else
			os << " PUBLIC \"-//OASIS//DTD DocBook V4.2//EN\"";

		docstring preamble = from_utf8(params().preamble);
		if (runparams.flavor != OutputParams::XML ) {
			preamble += "<!ENTITY % output.print.png \"IGNORE\">\n";
			preamble += "<!ENTITY % output.print.pdf \"IGNORE\">\n";
			preamble += "<!ENTITY % output.print.eps \"IGNORE\">\n";
			preamble += "<!ENTITY % output.print.bmp \"IGNORE\">\n";
		}

		string const name = runparams.nice
			? changeExtension(absFileName(), ".sgml") : fname;
		preamble += features.getIncludedFiles(name);
		preamble += features.getLyXSGMLEntities();

		if (!preamble.empty()) {
			os << "\n [ " << preamble << " ]";
		}
		os << ">\n\n";
	}

	string top = top_element;
	top += " lang=\"";
	if (runparams.flavor == OutputParams::XML)
		top += params().language->code();
	else
		top += params().language->code().substr(0, 2);
	top += '"';

	if (!params().options.empty()) {
		top += ' ';
		top += params().options;
	}

	os << "<!-- " << ((runparams.flavor == OutputParams::XML)? "XML" : "SGML")
	    << " file was created by LyX " << lyx_version
	    << "\n  See http://www.lyx.org/ for more information -->\n";

	params().documentClass().counters().reset();

	updateMacros();

	sgml::openTag(os, top);
	os << '\n';
	docbookParagraphs(text(), *this, os, runparams);
	sgml::closeTag(os, top_element);
}


void Buffer::makeLyXHTMLFile(FileName const & fname,
			      OutputParams const & runparams,
			      bool const body_only) const
{
	LYXERR(Debug::LATEX, "makeLyXHTMLFile...");

	ofdocstream ofs;
	if (!openFileWrite(ofs, fname))
		return;

	writeLyXHTMLSource(ofs, runparams, body_only);

	ofs.close();
	if (ofs.fail())
		lyxerr << "File '" << fname << "' was not closed properly." << endl;
}


void Buffer::writeLyXHTMLSource(odocstream & os,
			     OutputParams const & runparams,
			     bool const only_body) const
{
	LaTeXFeatures features(*this, params(), runparams);
	validate(features);
	updateBuffer(UpdateMaster, OutputUpdate);
	d->bibinfo_.makeCitationLabels(*this);
	updateMacros();
	updateMacroInstances();

	if (!only_body) {
		os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		   << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1 plus MathML 2.0//EN\" \"http://www.w3.org/TR/MathML2/dtd/xhtml-math11-f.dtd\">\n"
		   // FIXME Language should be set properly.
		   << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
		   << "<head>\n"
		   << "<meta name=\"GENERATOR\" content=\"" << PACKAGE_STRING << "\" />\n"
		   // FIXME Presumably need to set this right
		   << "<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\" />\n";

		docstring const & doctitle = features.htmlTitle();
		os << "<title>"
		   << (doctitle.empty() ? from_ascii("LyX Document") : doctitle)
		   << "</title>\n";

		os << "\n<!-- Text Class Preamble -->\n"
		   << features.getTClassHTMLPreamble()
		   << "\n<!-- Preamble Snippets -->\n"
		   << from_utf8(features.getPreambleSnippets());

		os << "\n<!-- Layout-provided Styles -->\n";
		docstring const styleinfo = features.getTClassHTMLStyles();
		if (!styleinfo.empty()) {
			os << "<style type='text/css'>\n"
				<< styleinfo
				<< "</style>\n";
		}
		os << "</head>\n<body>\n";
	}

	XHTMLStream xs(os);
	params().documentClass().counters().reset();
	xhtmlParagraphs(text(), *this, xs, runparams);
	if (!only_body)
		os << "</body>\n</html>\n";
}


// chktex should be run with these flags disabled: 3, 22, 25, 30, 38(?)
// Other flags: -wall -v0 -x
int Buffer::runChktex()
{
	setBusy(true);

	// get LaTeX-Filename
	FileName const path(temppath());
	string const name = addName(path.absFileName(), latexName());
	string const org_path = filePath();

	PathChanger p(path); // path to LaTeX file
	message(_("Running chktex..."));

	// Generate the LaTeX file if neccessary
	OutputParams runparams(&params().encoding());
	runparams.flavor = OutputParams::LATEX;
	runparams.nice = false;
	runparams.linelen = lyxrc.plaintext_linelen;
	makeLaTeXFile(FileName(name), org_path, runparams);

	TeXErrors terr;
	Chktex chktex(lyxrc.chktex_command, onlyFileName(name), filePath());
	int const res = chktex.run(terr); // run chktex

	if (res == -1) {
		Alert::error(_("chktex failure"),
			     _("Could not run chktex successfully."));
	} else if (res > 0) {
		ErrorList & errlist = d->errorLists["ChkTeX"];
		errlist.clear();
		bufferErrors(terr, errlist);
	}

	setBusy(false);

	errors("ChkTeX");

	return res;
}


void Buffer::validate(LaTeXFeatures & features) const
{
	params().validate(features);

	updateMacros();

	for_each(paragraphs().begin(), paragraphs().end(),
		 bind(&Paragraph::validate, _1, ref(features)));

	if (lyxerr.debugging(Debug::LATEX)) {
		features.showStruct();
	}
}


void Buffer::getLabelList(vector<docstring> & list) const
{
	// If this is a child document, use the master's list instead.
	if (parent()) {
		masterBuffer()->getLabelList(list);
		return;
	}

	list.clear();
	Toc & toc = d->toc_backend.toc("label");
	TocIterator toc_it = toc.begin();
	TocIterator end = toc.end();
	for (; toc_it != end; ++toc_it) {
		if (toc_it->depth() == 0)
			list.push_back(toc_it->str());
	}
}


void Buffer::updateBibfilesCache(UpdateScope scope) const
{
	// FIXME This is probably unnecssary, given where we call this.
	// If this is a child document, use the parent's cache instead.
	if (parent() && scope != UpdateChildOnly) {
		masterBuffer()->updateBibfilesCache();
		return;
	}

	d->bibfiles_cache_.clear();
	for (InsetIterator it = inset_iterator_begin(inset()); it; ++it) {
		if (it->lyxCode() == BIBTEX_CODE) {
			InsetBibtex const & inset = static_cast<InsetBibtex const &>(*it);
			support::FileNameList const bibfiles = inset.getBibFiles();
			d->bibfiles_cache_.insert(d->bibfiles_cache_.end(),
				bibfiles.begin(),
				bibfiles.end());
		} else if (it->lyxCode() == INCLUDE_CODE) {
			InsetInclude & inset = static_cast<InsetInclude &>(*it);
			Buffer const * const incbuf = inset.getChildBuffer();
			if (!incbuf)
				continue;
			support::FileNameList const & bibfiles =
					incbuf->getBibfilesCache(UpdateChildOnly);
			if (!bibfiles.empty()) {
				d->bibfiles_cache_.insert(d->bibfiles_cache_.end(),
					bibfiles.begin(),
					bibfiles.end());
			}
		}
	}
	d->bibfile_cache_valid_ = true;
	d->bibinfo_cache_valid_ = false;
}


void Buffer::invalidateBibinfoCache() const
{
	d->bibinfo_cache_valid_ = false;
	// also invalidate the cache for the parent buffer
	Buffer const * const pbuf = d->parent();
	if (pbuf)
		pbuf->invalidateBibinfoCache();
}


void Buffer::invalidateBibfileCache() const
{
	d->bibfile_cache_valid_ = false;
	d->bibinfo_cache_valid_ = false;
	// also invalidate the cache for the parent buffer
	Buffer const * const pbuf = d->parent();
	if (pbuf)
		pbuf->invalidateBibfileCache();
}


support::FileNameList const & Buffer::getBibfilesCache(UpdateScope scope) const
{
	// FIXME This is probably unnecessary, given where we call this.
	// If this is a child document, use the master's cache instead.
	Buffer const * const pbuf = masterBuffer();
	if (pbuf != this && scope != UpdateChildOnly)
		return pbuf->getBibfilesCache();

	if (!d->bibfile_cache_valid_)
		this->updateBibfilesCache(scope);

	return d->bibfiles_cache_;
}


BiblioInfo const & Buffer::masterBibInfo() const
{
	Buffer const * const tmp = masterBuffer();
	if (tmp != this)
		return tmp->masterBibInfo();
	return d->bibinfo_;
}


void Buffer::checkBibInfoCache() const 
{
	// use the master's cache
	Buffer const * const tmp = masterBuffer();
	if (tmp != this) {
		tmp->checkBibInfoCache();
		return;
	}

	// this will also reload the cache if it is invalid 
	support::FileNameList const & bibfiles_cache = getBibfilesCache();
	
	// compare the cached timestamps with the actual ones.
	support::FileNameList::const_iterator ei = bibfiles_cache.begin();
	support::FileNameList::const_iterator en = bibfiles_cache.end();
	for (; ei != en; ++ ei) {
		time_t lastw = ei->lastModified();
		time_t prevw = d->bibfile_status_[*ei];
		if (lastw != prevw) {
			d->bibinfo_cache_valid_ = false;
			d->bibfile_status_[*ei] = lastw;
		}
	}
	
	// if not valid, then reload the info
	if (!d->bibinfo_cache_valid_) {
		d->bibinfo_.clear();
		fillWithBibKeys(d->bibinfo_);
		d->bibinfo_cache_valid_ = true;
	}
}


void Buffer::fillWithBibKeys(BiblioInfo & keys) const
{
	for (InsetIterator it = inset_iterator_begin(inset()); it; ++it)
		it->fillWithBibKeys(keys, it);
}


bool Buffer::isDepClean(string const & name) const
{
	DepClean::const_iterator const it = d->dep_clean.find(name);
	if (it == d->dep_clean.end())
		return true;
	return it->second;
}


void Buffer::markDepClean(string const & name)
{
	d->dep_clean[name] = true;
}


bool Buffer::isExportableFormat(string const & format) const
{
		typedef vector<Format const *> Formats;
		Formats formats;
		formats = exportableFormats(true);
		Formats::const_iterator fit = formats.begin();
		Formats::const_iterator end = formats.end();
		for (; fit != end ; ++fit) {
			if ((*fit)->name() == format)
				return true;
		}
		return false;
}


bool Buffer::getStatus(FuncRequest const & cmd, FuncStatus & flag)
{
	if (isInternal()) {
		// FIXME? if there is an Buffer LFUN that can be dispatched even
		// if internal, put a switch '(cmd.action)' here.
		return false;
	}

	bool enable = true;

	switch (cmd.action()) {

		case LFUN_BUFFER_TOGGLE_READ_ONLY:
			flag.setOnOff(isReadonly());
			break;

		// FIXME: There is need for a command-line import.
		//case LFUN_BUFFER_IMPORT:

		case LFUN_BUFFER_AUTO_SAVE:
			break;

		case LFUN_BUFFER_EXPORT_CUSTOM:
			// FIXME: Nothing to check here?
			break;

		case LFUN_BUFFER_EXPORT: {
			docstring const arg = cmd.argument();
			enable = arg == "custom" || isExportable(to_utf8(arg));
			if (!enable)
				flag.message(bformat(
					_("Don't know how to export to format: %1$s"), arg));
			break;
		}

		case LFUN_BUFFER_CHKTEX:
			enable = isLatex() && !lyxrc.chktex_command.empty();
			break;

		case LFUN_BUILD_PROGRAM:
			enable = isExportable("program");
			break;

		case LFUN_BRANCH_ACTIVATE: 
		case LFUN_BRANCH_DEACTIVATE: {
			BranchList const & branchList = params().branchlist();
			docstring const branchName = cmd.argument();
			enable = !branchName.empty() && branchList.find(branchName);
			break;
		}

		case LFUN_BRANCH_ADD:
		case LFUN_BRANCHES_RENAME:
		case LFUN_BUFFER_PRINT:
			// if no Buffer is present, then of course we won't be called!
			break;

		case LFUN_BUFFER_LANGUAGE:
			enable = !isReadonly();
			break;

		default:
			return false;
	}
	flag.setEnabled(enable);
	return true;
}


void Buffer::dispatch(string const & command, DispatchResult & result)
{
	return dispatch(lyxaction.lookupFunc(command), result);
}


// NOTE We can end up here even if we have no GUI, because we are called
// by LyX::exec to handled command-line requests. So we may need to check 
// whether we have a GUI or not. The boolean use_gui holds this information.
void Buffer::dispatch(FuncRequest const & func, DispatchResult & dr)
{
	if (isInternal()) {
		// FIXME? if there is an Buffer LFUN that can be dispatched even
		// if internal, put a switch '(cmd.action())' here.
		dr.dispatched(false);
		return;
	}
	string const argument = to_utf8(func.argument());
	// We'll set this back to false if need be.
	bool dispatched = true;
	undo().beginUndoGroup();

	switch (func.action()) {
	case LFUN_BUFFER_TOGGLE_READ_ONLY:
		if (lyxvc().inUse())
			lyxvc().toggleReadOnly();
		else
			setReadonly(!isReadonly());
		break;

	case LFUN_BUFFER_EXPORT: {
		bool success = doExport(argument, false, false);
		dr.setError(!success);
		if (!success)
			dr.setMessage(bformat(_("Error exporting to format: %1$s."), 
					      func.argument()));
		break;
	}

	case LFUN_BUILD_PROGRAM:
		doExport("program", true, false);
		break;

	case LFUN_BUFFER_CHKTEX:
		runChktex();
		break;

	case LFUN_BUFFER_EXPORT_CUSTOM: {
		string format_name;
		string command = split(argument, format_name, ' ');
		Format const * format = formats.getFormat(format_name);
		if (!format) {
			lyxerr << "Format \"" << format_name
				<< "\" not recognized!"
				<< endl;
			break;
		}

		// The name of the file created by the conversion process
		string filename;

		// Output to filename
		if (format->name() == "lyx") {
			string const latexname = latexName(false);
			filename = changeExtension(latexname,
				format->extension());
			filename = addName(temppath(), filename);

			if (!writeFile(FileName(filename)))
				break;

		} else {
			doExport(format_name, true, false, filename);
		}

		// Substitute $$FName for filename
		if (!contains(command, "$$FName"))
			command = "( " + command + " ) < $$FName";
		command = subst(command, "$$FName", filename);

		// Execute the command in the background
		Systemcall call;
		call.startscript(Systemcall::DontWait, command);
		break;
	}

	// FIXME: There is need for a command-line import.
	/*
	case LFUN_BUFFER_IMPORT:
		doImport(argument);
		break;
	*/

	case LFUN_BUFFER_AUTO_SAVE:
		autoSave();
		resetAutosaveTimers();
		break;

	case LFUN_BRANCH_ADD: {
		docstring branch_name = func.argument();
		if (branch_name.empty()) {
			dispatched = false;
			break;
		}
		BranchList & branch_list = params().branchlist();
		vector<docstring> const branches =
			getVectorFromString(branch_name, branch_list.separator());
		docstring msg;
		for (vector<docstring>::const_iterator it = branches.begin();
		     it != branches.end(); ++it) {
			branch_name = *it;
			Branch * branch = branch_list.find(branch_name);
			if (branch) {
				LYXERR0("Branch " << branch_name << " already exists.");
				dr.setError(true);
				if (!msg.empty())
					msg += ("\n");
				msg += bformat(_("Branch \"%1$s\" already exists."), branch_name);
			} else {
				branch_list.add(branch_name);
				branch = branch_list.find(branch_name);
				string const x11hexname = X11hexname(branch->color());
				docstring const str = branch_name + ' ' + from_ascii(x11hexname);
				lyx::dispatch(FuncRequest(LFUN_SET_COLOR, str));
				dr.setError(false);
				dr.screenUpdate(Update::Force);
			}
		}
		if (!msg.empty())
			dr.setMessage(msg);
		break;
	}

	case LFUN_BRANCH_ACTIVATE:
	case LFUN_BRANCH_DEACTIVATE: {
		BranchList & branchList = params().branchlist();
		docstring const branchName = func.argument();
		// the case without a branch name is handled elsewhere
		if (branchName.empty()) {
			dispatched = false;
			break;
		}
		Branch * branch = branchList.find(branchName);
		if (!branch) {
			LYXERR0("Branch " << branchName << " does not exist.");
			dr.setError(true);
			docstring const msg = 
				bformat(_("Branch \"%1$s\" does not exist."), branchName);
			dr.setMessage(msg);
		} else {
			branch->setSelected(func.action() == LFUN_BRANCH_ACTIVATE);
			dr.setError(false);
			dr.screenUpdate(Update::Force);
			dr.forceBufferUpdate();
		}
		break;
	}

	case LFUN_BRANCHES_RENAME: {
		if (func.argument().empty())
			break;

		docstring const oldname = from_utf8(func.getArg(0));
		docstring const newname = from_utf8(func.getArg(1));
		InsetIterator it  = inset_iterator_begin(inset());
		InsetIterator const end = inset_iterator_end(inset());
		bool success = false;
		for (; it != end; ++it) {
			if (it->lyxCode() == BRANCH_CODE) {
				InsetBranch & ins = static_cast<InsetBranch &>(*it);
				if (ins.branch() == oldname) {
					undo().recordUndo(it);
					ins.rename(newname);
					success = true;
					continue;
				}
			}
			if (it->lyxCode() == INCLUDE_CODE) {
				// get buffer of external file
				InsetInclude const & ins =
					static_cast<InsetInclude const &>(*it);
				Buffer * child = ins.getChildBuffer();
				if (!child)
					continue;
				child->dispatch(func, dr);
			}
		}

		if (success) {
			dr.screenUpdate(Update::Force);
			dr.forceBufferUpdate();
		}
		break;
	}

	case LFUN_BUFFER_PRINT: {
		// we'll assume there's a problem until we succeed
		dr.setError(true); 
		string target = func.getArg(0);
		string target_name = func.getArg(1);
		string command = func.getArg(2);

		if (target.empty()
		    || target_name.empty()
		    || command.empty()) {
			LYXERR0("Unable to parse " << func.argument());
			docstring const msg = 
				bformat(_("Unable to parse \"%1$s\""), func.argument());
			dr.setMessage(msg);
			break;
		}
		if (target != "printer" && target != "file") {
			LYXERR0("Unrecognized target \"" << target << '"');
			docstring const msg = 
				bformat(_("Unrecognized target \"%1$s\""), from_utf8(target));
			dr.setMessage(msg);
			break;
		}

		bool const update_unincluded =
				params().maintain_unincluded_children
				&& !params().getIncludedChildren().empty();
		if (!doExport("dvi", true, update_unincluded)) {
			showPrintError(absFileName());
			dr.setMessage(_("Error exporting to DVI."));
			break;
		}

		// Push directory path.
		string const path = temppath();
		// Prevent the compiler from optimizing away p
		FileName pp(path);
		PathChanger p(pp);

		// there are three cases here:
		// 1. we print to a file
		// 2. we print directly to a printer
		// 3. we print using a spool command (print to file first)
		Systemcall one;
		int res = 0;
		string const dviname = changeExtension(latexName(true), "dvi");

		if (target == "printer") {
			if (!lyxrc.print_spool_command.empty()) {
				// case 3: print using a spool
				string const psname = changeExtension(dviname,".ps");
				command += ' ' + lyxrc.print_to_file
					+ quoteName(psname)
					+ ' '
					+ quoteName(dviname);

				string command2 = lyxrc.print_spool_command + ' ';
				if (target_name != "default") {
					command2 += lyxrc.print_spool_printerprefix
						+ target_name
						+ ' ';
				}
				command2 += quoteName(psname);
				// First run dvips.
				// If successful, then spool command
				res = one.startscript(Systemcall::Wait, command);

				if (res == 0) {
					// If there's no GUI, we have to wait on this command. Otherwise,
					// LyX deletes the temporary directory, and with it the spooled
					// file, before it can be printed!!
					Systemcall::Starttype stype = use_gui ?
						Systemcall::DontWait : Systemcall::Wait;
					res = one.startscript(stype, command2);
				}
			} else {
				// case 2: print directly to a printer
				if (target_name != "default")
					command += ' ' + lyxrc.print_to_printer + target_name + ' ';
				// as above....
				Systemcall::Starttype stype = use_gui ?
					Systemcall::DontWait : Systemcall::Wait;
				res = one.startscript(stype, command + quoteName(dviname));
			}

		} else {
			// case 1: print to a file
			FileName const filename(makeAbsPath(target_name, filePath()));
			FileName const dvifile(makeAbsPath(dviname, path));
			if (filename.exists()) {
				docstring text = bformat(
					_("The file %1$s already exists.\n\n"
					  "Do you want to overwrite that file?"),
					makeDisplayPath(filename.absFileName()));
				if (Alert::prompt(_("Overwrite file?"),
						  text, 0, 1, _("&Overwrite"), _("&Cancel")) != 0)
					break;
			}
			command += ' ' + lyxrc.print_to_file
				+ quoteName(filename.toFilesystemEncoding())
				+ ' '
				+ quoteName(dvifile.toFilesystemEncoding());
			// as above....
			Systemcall::Starttype stype = use_gui ?
				Systemcall::DontWait : Systemcall::Wait;
			res = one.startscript(stype, command);
		}

		if (res == 0) 
			dr.setError(false);
		else {
			dr.setMessage(_("Error running external commands."));
			showPrintError(absFileName());
		}
		break;
	}

	case LFUN_BUFFER_LANGUAGE: {
		Language const * oldL = params().language;
		Language const * newL = languages.getLanguage(argument);
		if (!newL || oldL == newL)
			break;
		if (oldL->rightToLeft() == newL->rightToLeft() && !isMultiLingual()) {
			changeLanguage(oldL, newL);
			dr.forceBufferUpdate();
		}
		break;
	}

	default:
		dispatched = false;
		break;
	}
	dr.dispatched(dispatched);
	undo().endUndoGroup();
}


void Buffer::changeLanguage(Language const * from, Language const * to)
{
	LASSERT(from, /**/);
	LASSERT(to, /**/);

	for_each(par_iterator_begin(),
		 par_iterator_end(),
		 bind(&Paragraph::changeLanguage, _1, params(), from, to));
}


bool Buffer::isMultiLingual() const
{
	ParConstIterator end = par_iterator_end();
	for (ParConstIterator it = par_iterator_begin(); it != end; ++it)
		if (it->isMultiLingual(params()))
			return true;

	return false;
}


std::set<Language const *> Buffer::getLanguages() const
{
	std::set<Language const *> languages;
	getLanguages(languages);
	return languages;
}


void Buffer::getLanguages(std::set<Language const *> & languages) const
{
	ParConstIterator end = par_iterator_end();
	// add the buffer language, even if it's not actively used
	languages.insert(language());
	// iterate over the paragraphs
	for (ParConstIterator it = par_iterator_begin(); it != end; ++it)
		it->getLanguages(languages);
	// also children
	ListOfBuffers clist = getDescendents();
	ListOfBuffers::const_iterator cit = clist.begin();
	ListOfBuffers::const_iterator const cen = clist.end();
	for (; cit != cen; ++cit)
		(*cit)->getLanguages(languages);
}


DocIterator Buffer::getParFromID(int const id) const
{
	Buffer * buf = const_cast<Buffer *>(this);
	if (id < 0) {
		// John says this is called with id == -1 from undo
		lyxerr << "getParFromID(), id: " << id << endl;
		return doc_iterator_end(buf);
	}

	for (DocIterator it = doc_iterator_begin(buf); !it.atEnd(); it.forwardPar())
		if (it.paragraph().id() == id)
			return it;

	return doc_iterator_end(buf);
}


bool Buffer::hasParWithID(int const id) const
{
	return !getParFromID(id).atEnd();
}


ParIterator Buffer::par_iterator_begin()
{
	return ParIterator(doc_iterator_begin(this));
}


ParIterator Buffer::par_iterator_end()
{
	return ParIterator(doc_iterator_end(this));
}


ParConstIterator Buffer::par_iterator_begin() const
{
	return ParConstIterator(doc_iterator_begin(this));
}


ParConstIterator Buffer::par_iterator_end() const
{
	return ParConstIterator(doc_iterator_end(this));
}


Language const * Buffer::language() const
{
	return params().language;
}


docstring const Buffer::B_(string const & l10n) const
{
	return params().B_(l10n);
}


bool Buffer::isClean() const
{
	return d->lyx_clean;
}


bool Buffer::isExternallyModified(CheckMethod method) const
{
	LASSERT(d->filename.exists(), /**/);
	// if method == timestamp, check timestamp before checksum
	return (method == checksum_method
		|| d->timestamp_ != d->filename.lastModified())
		&& d->checksum_ != d->filename.checksum();
}


void Buffer::saveCheckSum() const
{
	FileName const & file = d->filename;
	if (file.exists()) {
		d->timestamp_ = file.lastModified();
		d->checksum_ = file.checksum();
	} else {
		// in the case of save to a new file.
		d->timestamp_ = 0;
		d->checksum_ = 0;
	}
}


void Buffer::markClean() const
{
	if (!d->lyx_clean) {
		d->lyx_clean = true;
		updateTitles();
	}
	// if the .lyx file has been saved, we don't need an
	// autosave
	d->bak_clean = true;
	d->undo_.markDirty();
}


void Buffer::setUnnamed(bool flag)
{
	d->unnamed = flag;
}


bool Buffer::isUnnamed() const
{
	return d->unnamed;
}


/// \note
/// Don't check unnamed, here: isInternal() is used in
/// newBuffer(), where the unnamed flag has not been set by anyone
/// yet. Also, for an internal buffer, there should be no need for
/// retrieving fileName() nor for checking if it is unnamed or not.
bool Buffer::isInternal() const
{
	return fileName().extension() == "internal";
}


void Buffer::markDirty()
{
	if (d->lyx_clean) {
		d->lyx_clean = false;
		updateTitles();
	}
	d->bak_clean = false;

	DepClean::iterator it = d->dep_clean.begin();
	DepClean::const_iterator const end = d->dep_clean.end();

	for (; it != end; ++it)
		it->second = false;
}


FileName Buffer::fileName() const
{
	return d->filename;
}


string Buffer::absFileName() const
{
	return d->filename.absFileName();
}


string Buffer::filePath() const
{
	return d->filename.onlyPath().absFileName() + "/";
}


bool Buffer::isReadonly() const
{
	return d->read_only;
}


void Buffer::setParent(Buffer const * buffer)
{
	// Avoids recursive include.
	d->setParent(buffer == this ? 0 : buffer);
	updateMacros();
}


Buffer const * Buffer::parent() const
{
	return d->parent();
}


ListOfBuffers Buffer::allRelatives() const
{
	ListOfBuffers lb = masterBuffer()->getDescendents();
	lb.push_front(const_cast<Buffer *>(this));
	return lb;
}


Buffer const * Buffer::masterBuffer() const
{
	// FIXME Should be make sure we are not in some kind
	// of recursive include? A -> B -> A will crash this.
	Buffer const * const pbuf = d->parent();
	if (!pbuf)
		return this;

	return pbuf->masterBuffer();
}


bool Buffer::isChild(Buffer * child) const
{
	return d->children_positions.find(child) != d->children_positions.end();
}


DocIterator Buffer::firstChildPosition(Buffer const * child)
{
	Impl::BufferPositionMap::iterator it;
	it = d->children_positions.find(child);
	if (it == d->children_positions.end())
		return DocIterator(this);
	return it->second;
}


bool Buffer::hasChildren() const
{
	return !d->children_positions.empty();	
}


void Buffer::collectChildren(ListOfBuffers & clist, bool grand_children) const
{
	// loop over children
	Impl::BufferPositionMap::iterator it = d->children_positions.begin();
	Impl::BufferPositionMap::iterator end = d->children_positions.end();
	for (; it != end; ++it) {
		Buffer * child = const_cast<Buffer *>(it->first);
		// No duplicates
		ListOfBuffers::const_iterator bit = find(clist.begin(), clist.end(), child);
		if (bit != clist.end())
			continue;
		clist.push_back(child);
		if (grand_children) 
			// there might be grandchildren
			child->collectChildren(clist, true);
	}
}


ListOfBuffers Buffer::getChildren() const
{
	ListOfBuffers v;
	collectChildren(v, false);
	return v;
}


ListOfBuffers Buffer::getDescendents() const
{
	ListOfBuffers v;
	collectChildren(v, true);
	return v;
}


template<typename M>
typename M::const_iterator greatest_below(M & m, typename M::key_type const & x)
{
	if (m.empty())
		return m.end();

	typename M::const_iterator it = m.lower_bound(x);
	if (it == m.begin())
		return m.end();

	it--;
	return it;
}


MacroData const * Buffer::Impl::getBufferMacro(docstring const & name,
					 DocIterator const & pos) const
{
	LYXERR(Debug::MACROS, "Searching for " << to_ascii(name) << " at " << pos);

	// if paragraphs have no macro context set, pos will be empty
	if (pos.empty())
		return 0;

	// we haven't found anything yet
	DocIterator bestPos = owner_->par_iterator_begin();
	MacroData const * bestData = 0;

	// find macro definitions for name
	NamePositionScopeMacroMap::const_iterator nameIt = macros.find(name);
	if (nameIt != macros.end()) {
		// find last definition in front of pos or at pos itself
		PositionScopeMacroMap::const_iterator it
			= greatest_below(nameIt->second, pos);
		if (it != nameIt->second.end()) {
			while (true) {
				// scope ends behind pos?
				if (pos < it->second.first) {
					// Looks good, remember this. If there
					// is no external macro behind this,
					// we found the right one already.
					bestPos = it->first;
					bestData = &it->second.second;
					break;
				}

				// try previous macro if there is one
				if (it == nameIt->second.begin())
					break;
				it--;
			}
		}
	}

	// find macros in included files
	PositionScopeBufferMap::const_iterator it
		= greatest_below(position_to_children, pos);
	if (it == position_to_children.end())
		// no children before
		return bestData;

	while (true) {
		// do we know something better (i.e. later) already?
		if (it->first < bestPos )
			break;

		// scope ends behind pos?
		if (pos < it->second.first) {
			// look for macro in external file
			macro_lock = true;
			MacroData const * data
				= it->second.second->getMacro(name, false);
			macro_lock = false;
			if (data) {
				bestPos = it->first;
				bestData = data;
				break;
			}
		}

		// try previous file if there is one
		if (it == position_to_children.begin())
			break;
		--it;
	}

	// return the best macro we have found
	return bestData;
}


MacroData const * Buffer::getMacro(docstring const & name,
	DocIterator const & pos, bool global) const
{
	if (d->macro_lock)
		return 0;

	// query buffer macros
	MacroData const * data = d->getBufferMacro(name, pos);
	if (data != 0)
		return data;

	// If there is a master buffer, query that
	Buffer const * const pbuf = d->parent();
	if (pbuf) {
		d->macro_lock = true;
		MacroData const * macro	= pbuf->getMacro(
			name, *this, false);
		d->macro_lock = false;
		if (macro)
			return macro;
	}

	if (global) {
		data = MacroTable::globalMacros().get(name);
		if (data != 0)
			return data;
	}

	return 0;
}


MacroData const * Buffer::getMacro(docstring const & name, bool global) const
{
	// set scope end behind the last paragraph
	DocIterator scope = par_iterator_begin();
	scope.pit() = scope.lastpit() + 1;

	return getMacro(name, scope, global);
}


MacroData const * Buffer::getMacro(docstring const & name,
	Buffer const & child, bool global) const
{
	// look where the child buffer is included first
	Impl::BufferPositionMap::iterator it = d->children_positions.find(&child);
	if (it == d->children_positions.end())
		return 0;

	// check for macros at the inclusion position
	return getMacro(name, it->second, global);
}


void Buffer::Impl::updateMacros(DocIterator & it, DocIterator & scope)
{
	pit_type const lastpit = it.lastpit();

	// look for macros in each paragraph
	while (it.pit() <= lastpit) {
		Paragraph & par = it.paragraph();

		// iterate over the insets of the current paragraph
		InsetList const & insets = par.insetList();
		InsetList::const_iterator iit = insets.begin();
		InsetList::const_iterator end = insets.end();
		for (; iit != end; ++iit) {
			it.pos() = iit->pos;

			// is it a nested text inset?
			if (iit->inset->asInsetText()) {
				// Inset needs its own scope?
				InsetText const * itext = iit->inset->asInsetText();
				bool newScope = itext->isMacroScope();

				// scope which ends just behind the inset
				DocIterator insetScope = it;
				++insetScope.pos();

				// collect macros in inset
				it.push_back(CursorSlice(*iit->inset));
				updateMacros(it, newScope ? insetScope : scope);
				it.pop_back();
				continue;
			}
			
			if (iit->inset->asInsetTabular()) {
				CursorSlice slice(*iit->inset);
				size_t const numcells = slice.nargs();
				for (; slice.idx() < numcells; slice.forwardIdx()) {
					it.push_back(slice);
					updateMacros(it, scope);
					it.pop_back();
				}
				continue;
			}

			// is it an external file?
			if (iit->inset->lyxCode() == INCLUDE_CODE) {
				// get buffer of external file
				InsetInclude const & inset =
					static_cast<InsetInclude const &>(*iit->inset);
				macro_lock = true;
				Buffer * child = inset.getChildBuffer();
				macro_lock = false;
				if (!child)
					continue;

				// register its position, but only when it is
				// included first in the buffer
				if (children_positions.find(child) ==
					children_positions.end())
						children_positions[child] = it;

				// register child with its scope
				position_to_children[it] = Impl::ScopeBuffer(scope, child);
				continue;
			}

			InsetMath * im = iit->inset->asInsetMath();
			if (doing_export && im)  {
				InsetMathHull * hull = im->asHullInset();
				if (hull)
					hull->recordLocation(it);
			}

			if (iit->inset->lyxCode() != MATHMACRO_CODE)
				continue;

			// get macro data
			MathMacroTemplate & macroTemplate =
				*iit->inset->asInsetMath()->asMacroTemplate();
			MacroContext mc(owner_, it);
			macroTemplate.updateToContext(mc);

			// valid?
			bool valid = macroTemplate.validMacro();
			// FIXME: Should be fixNameAndCheckIfValid() in fact,
			// then the BufferView's cursor will be invalid in
			// some cases which leads to crashes.
			if (!valid)
				continue;

			// register macro
			// FIXME (Abdel), I don't understandt why we pass 'it' here
			// instead of 'macroTemplate' defined above... is this correct?
			macros[macroTemplate.name()][it] =
				Impl::ScopeMacro(scope, MacroData(const_cast<Buffer *>(owner_), it));
		}

		// next paragraph
		it.pit()++;
		it.pos() = 0;
	}
}


void Buffer::updateMacros() const
{
	if (d->macro_lock)
		return;

	LYXERR(Debug::MACROS, "updateMacro of " << d->filename.onlyFileName());

	// start with empty table
	d->macros.clear();
	d->children_positions.clear();
	d->position_to_children.clear();

	// Iterate over buffer, starting with first paragraph
	// The scope must be bigger than any lookup DocIterator
	// later. For the global lookup, lastpit+1 is used, hence
	// we use lastpit+2 here.
	DocIterator it = par_iterator_begin();
	DocIterator outerScope = it;
	outerScope.pit() = outerScope.lastpit() + 2;
	d->updateMacros(it, outerScope);
}


void Buffer::getUsedBranches(std::list<docstring> & result, bool const from_master) const
{
	InsetIterator it  = inset_iterator_begin(inset());
	InsetIterator const end = inset_iterator_end(inset());
	for (; it != end; ++it) {
		if (it->lyxCode() == BRANCH_CODE) {
			InsetBranch & br = static_cast<InsetBranch &>(*it);
			docstring const name = br.branch();
			if (!from_master && !params().branchlist().find(name))
				result.push_back(name);
			else if (from_master && !masterBuffer()->params().branchlist().find(name))
				result.push_back(name);
			continue;
		}
		if (it->lyxCode() == INCLUDE_CODE) {
			// get buffer of external file
			InsetInclude const & ins =
				static_cast<InsetInclude const &>(*it);
			Buffer * child = ins.getChildBuffer();
			if (!child)
				continue;
			child->getUsedBranches(result, true);
		}
	}
	// remove duplicates
	result.unique();
}


void Buffer::updateMacroInstances() const
{
	LYXERR(Debug::MACROS, "updateMacroInstances for "
		<< d->filename.onlyFileName());
	DocIterator it = doc_iterator_begin(this);
	it.forwardInset();
	DocIterator const end = doc_iterator_end(this);
	for (; it != end; it.forwardInset()) {
 		// look for MathData cells in InsetMathNest insets
		InsetMath * minset = it.nextInset()->asInsetMath();
		if (!minset)
			continue;

		// update macro in all cells of the InsetMathNest
		DocIterator::idx_type n = minset->nargs();
		MacroContext mc = MacroContext(this, it);
		for (DocIterator::idx_type i = 0; i < n; ++i) {
			MathData & data = minset->cell(i);
			data.updateMacros(0, mc);
		}
	}
}


void Buffer::listMacroNames(MacroNameSet & macros) const
{
	if (d->macro_lock)
		return;

	d->macro_lock = true;

	// loop over macro names
	Impl::NamePositionScopeMacroMap::iterator nameIt = d->macros.begin();
	Impl::NamePositionScopeMacroMap::iterator nameEnd = d->macros.end();
	for (; nameIt != nameEnd; ++nameIt)
		macros.insert(nameIt->first);

	// loop over children
	Impl::BufferPositionMap::iterator it = d->children_positions.begin();
	Impl::BufferPositionMap::iterator end = d->children_positions.end();
	for (; it != end; ++it)
		it->first->listMacroNames(macros);

	// call parent
	Buffer const * const pbuf = d->parent();
	if (pbuf)
		pbuf->listMacroNames(macros);

	d->macro_lock = false;
}


void Buffer::listParentMacros(MacroSet & macros, LaTeXFeatures & features) const
{
	Buffer const * const pbuf = d->parent();
	if (!pbuf)
		return;

	MacroNameSet names;
	pbuf->listMacroNames(names);

	// resolve macros
	MacroNameSet::iterator it = names.begin();
	MacroNameSet::iterator end = names.end();
	for (; it != end; ++it) {
		// defined?
		MacroData const * data =
		pbuf->getMacro(*it, *this, false);
		if (data) {
			macros.insert(data);

			// we cannot access the original MathMacroTemplate anymore
			// here to calls validate method. So we do its work here manually.
			// FIXME: somehow make the template accessible here.
			if (data->optionals() > 0)
				features.require("xargs");
		}
	}
}


Buffer::References & Buffer::references(docstring const & label)
{
	if (d->parent())
		return const_cast<Buffer *>(masterBuffer())->references(label);

	RefCache::iterator it = d->ref_cache_.find(label);
	if (it != d->ref_cache_.end())
		return it->second.second;

	static InsetLabel const * dummy_il = 0;
	static References const dummy_refs;
	it = d->ref_cache_.insert(
		make_pair(label, make_pair(dummy_il, dummy_refs))).first;
	return it->second.second;
}


Buffer::References const & Buffer::references(docstring const & label) const
{
	return const_cast<Buffer *>(this)->references(label);
}


void Buffer::setInsetLabel(docstring const & label, InsetLabel const * il)
{
	masterBuffer()->d->ref_cache_[label].first = il;
}


InsetLabel const * Buffer::insetLabel(docstring const & label) const
{
	return masterBuffer()->d->ref_cache_[label].first;
}


void Buffer::clearReferenceCache() const
{
	if (!d->parent())
		d->ref_cache_.clear();
}


void Buffer::changeRefsIfUnique(docstring const & from, docstring const & to,
	InsetCode code)
{
	//FIXME: This does not work for child documents yet.
	LASSERT(code == CITE_CODE, /**/);
	// Check if the label 'from' appears more than once
	vector<docstring> labels;
	string paramName;
	checkBibInfoCache();
	BiblioInfo const & keys = masterBibInfo();
	BiblioInfo::const_iterator bit  = keys.begin();
	BiblioInfo::const_iterator bend = keys.end();

	for (; bit != bend; ++bit)
		// FIXME UNICODE
		labels.push_back(bit->first);
	paramName = "key";

	if (count(labels.begin(), labels.end(), from) > 1)
		return;

	for (InsetIterator it = inset_iterator_begin(inset()); it; ++it) {
		if (it->lyxCode() == code) {
			InsetCommand * inset = it->asInsetCommand();
			if (!inset)
				continue;
			docstring const oldValue = inset->getParam(paramName);
			if (oldValue == from)
				inset->setParam(paramName, to);
		}
	}
}


void Buffer::getSourceCode(odocstream & os, pit_type par_begin,
	pit_type par_end, bool full_source) const
{
	OutputParams runparams(&params().encoding());
	runparams.nice = true;
	runparams.flavor = params().useNonTeXFonts ? 
		OutputParams::XETEX : OutputParams::LATEX;
	runparams.linelen = lyxrc.plaintext_linelen;
	// No side effect of file copying and image conversion
	runparams.dryrun = true;

	if (full_source) {
		os << "% " << _("Preview source code") << "\n\n";
		d->texrow.reset();
		d->texrow.newline();
		d->texrow.newline();
		if (isDocBook())
			writeDocBookSource(os, absFileName(), runparams, false);
		else
			// latex or literate
			writeLaTeXSource(os, string(), runparams, true, true);
	} else {
		runparams.par_begin = par_begin;
		runparams.par_end = par_end;
		if (par_begin + 1 == par_end) {
			os << "% "
			   << bformat(_("Preview source code for paragraph %1$d"), par_begin)
			   << "\n\n";
		} else {
			os << "% "
			   << bformat(_("Preview source code from paragraph %1$s to %2$s"),
					convert<docstring>(par_begin),
					convert<docstring>(par_end - 1))
			   << "\n\n";
		}
		TexRow texrow;
		texrow.reset();
		texrow.newline();
		texrow.newline();
		// output paragraphs
		if (isDocBook())
			docbookParagraphs(text(), *this, os, runparams);
		else 
			// latex or literate
			latexParagraphs(*this, text(), os, texrow, runparams);
	}
}


ErrorList & Buffer::errorList(string const & type) const
{
	static ErrorList emptyErrorList;
	map<string, ErrorList>::iterator it = d->errorLists.find(type);
	if (it == d->errorLists.end())
		return emptyErrorList;

	return it->second;
}


void Buffer::updateTocItem(std::string const & type,
	DocIterator const & dit) const
{
	if (d->gui_)
		d->gui_->updateTocItem(type, dit);
}


void Buffer::structureChanged() const
{
	if (d->gui_)
		d->gui_->structureChanged();
}


void Buffer::errors(string const & err, bool from_master) const
{
	if (d->gui_)
		d->gui_->errors(err, from_master);
}


void Buffer::message(docstring const & msg) const
{
	if (d->gui_)
		d->gui_->message(msg);
}


void Buffer::setBusy(bool on) const
{
	if (d->gui_)
		d->gui_->setBusy(on);
}


void Buffer::updateTitles() const
{
	if (d->wa_)
		d->wa_->updateTitles();
}


void Buffer::resetAutosaveTimers() const
{
	if (d->gui_)
		d->gui_->resetAutosaveTimers();
}


bool Buffer::hasGuiDelegate() const
{
	return d->gui_;
}


void Buffer::setGuiDelegate(frontend::GuiBufferDelegate * gui)
{
	d->gui_ = gui;
}



namespace {

class AutoSaveBuffer : public ForkedProcess {
public:
	///
	AutoSaveBuffer(Buffer const & buffer, FileName const & fname)
		: buffer_(buffer), fname_(fname) {}
	///
	virtual shared_ptr<ForkedProcess> clone() const
	{
		return shared_ptr<ForkedProcess>(new AutoSaveBuffer(*this));
	}
	///
	int start()
	{
		command_ = to_utf8(bformat(_("Auto-saving %1$s"),
						 from_utf8(fname_.absFileName())));
		return run(DontWait);
	}
private:
	///
	virtual int generateChild();
	///
	Buffer const & buffer_;
	FileName fname_;
};


int AutoSaveBuffer::generateChild()
{
#if defined(__APPLE__)
	/* FIXME fork() is not usable for autosave on Mac OS X 10.6 (snow leopard) 
	 *   We should use something else like threads.
	 *
	 * Since I do not know how to determine at run time what is the OS X
	 * version, I just disable forking altogether for now (JMarc)
	 */
	pid_t const pid = -1;
#else
	// tmp_ret will be located (usually) in /tmp
	// will that be a problem?
	// Note that this calls ForkedCalls::fork(), so it's
	// ok cross-platform.
	pid_t const pid = fork();
	// If you want to debug the autosave
	// you should set pid to -1, and comment out the fork.
	if (pid != 0 && pid != -1)
		return pid;
#endif

	// pid = -1 signifies that lyx was unable
	// to fork. But we will do the save
	// anyway.
	bool failed = false;
	FileName const tmp_ret = FileName::tempName("lyxauto");
	if (!tmp_ret.empty()) {
		buffer_.writeFile(tmp_ret);
		// assume successful write of tmp_ret
		if (!tmp_ret.moveTo(fname_))
			failed = true;
	} else
		failed = true;

	if (failed) {
		// failed to write/rename tmp_ret so try writing direct
		if (!buffer_.writeFile(fname_)) {
			// It is dangerous to do this in the child,
			// but safe in the parent, so...
			if (pid == -1) // emit message signal.
				buffer_.message(_("Autosave failed!"));
		}
	}

	if (pid == 0) // we are the child so...
		_exit(0);

	return pid;
}

} // namespace anon


FileName Buffer::getEmergencyFileName() const
{
	return FileName(d->filename.absFileName() + ".emergency");
}


FileName Buffer::getAutosaveFileName() const
{
	// if the document is unnamed try to save in the backup dir, else
	// in the default document path, and as a last try in the filePath, 
	// which will most often be the temporary directory
	string fpath;
	if (isUnnamed())
		fpath = lyxrc.backupdir_path.empty() ? lyxrc.document_path
			: lyxrc.backupdir_path;
	if (!isUnnamed() || fpath.empty() || !FileName(fpath).exists())
		fpath = filePath();

	string const fname = "#" + d->filename.onlyFileName() + "#";

	return makeAbsPath(fname, fpath);
}


void Buffer::removeAutosaveFile() const
{
	FileName const f = getAutosaveFileName();
	if (f.exists())
		f.removeFile();
}


void Buffer::moveAutosaveFile(support::FileName const & oldauto) const
{
	FileName const newauto = getAutosaveFileName();
	oldauto.refresh();
	if (newauto != oldauto && oldauto.exists())
		if (!oldauto.moveTo(newauto))
			LYXERR0("Unable to move autosave file `" << oldauto << "'!");
}


bool Buffer::autoSave() const 
{
	Buffer const * buf = d->cloned_buffer_ ? d->cloned_buffer_ : this;
	if (buf->d->bak_clean || isReadonly())
		return true;

	message(_("Autosaving current document..."));
	buf->d->bak_clean = true;
	
	FileName const fname = getAutosaveFileName();
	if (d->cloned_buffer_) {
		// If this buffer is cloned, we assume that
		// we are running in a separate thread already.
		FileName const tmp_ret = FileName::tempName("lyxauto");
		if (!tmp_ret.empty()) {
			writeFile(tmp_ret);
			// assume successful write of tmp_ret
			if (tmp_ret.moveTo(fname))
				return true;
		}
		// failed to write/rename tmp_ret so try writing direct
		return writeFile(fname);
	} else {	
		/// This function is deprecated as the frontend needs to take care
		/// of cloning the buffer and autosaving it in another thread. It
		/// is still here to allow (QT_VERSION < 0x040400).
		AutoSaveBuffer autosave(*this, fname);
		autosave.start();
		return true;
	}
}


string Buffer::bufferFormat() const
{
	string format = params().documentClass().outputFormat();
	if (format == "latex") {
		if (params().useNonTeXFonts)
			return "xetex";
		if (params().encoding().package() == Encoding::japanese)
			return "platex";
	}
	return format;
}


string Buffer::getDefaultOutputFormat() const
{
	if (!params().defaultOutputFormat.empty()
	    && params().defaultOutputFormat != "default")
		return params().defaultOutputFormat;
	typedef vector<Format const *> Formats;
	Formats formats = exportableFormats(true);
	if (isDocBook()
	    || isLiterate()
	    || params().useNonTeXFonts
	    || params().encoding().package() == Encoding::japanese) {
		if (formats.empty())
			return string();
		// return the first we find
		return formats.front()->name();
	}
	return lyxrc.default_view_format;
}


namespace {
	// helper class, to guarantee this gets reset properly
	class MarkAsExporting	{
	public:
		MarkAsExporting(Buffer const * buf) : buf_(buf) 
		{
			LASSERT(buf_, /* */);
			buf_->setExportStatus(true);
		}
		~MarkAsExporting() 
		{
			buf_->setExportStatus(false);
		}
	private:
		Buffer const * const buf_;
	};
}


void Buffer::setExportStatus(bool e) const
{
	d->doing_export = e;	
}


bool Buffer::isExporting() const
{
	return d->doing_export;
}


bool Buffer::doExport(string const & format, bool put_in_tempdir,
	bool includeall, string & result_file) const
{
	MarkAsExporting exporting(this);
	string backend_format;
	OutputParams runparams(&params().encoding());
	runparams.flavor = OutputParams::LATEX;
	runparams.linelen = lyxrc.plaintext_linelen;
	runparams.includeall = includeall;
	vector<string> backs = backends();
	if (find(backs.begin(), backs.end(), format) == backs.end()) {
		// Get shortest path to format
		Graph::EdgePath path;
		for (vector<string>::const_iterator it = backs.begin();
		     it != backs.end(); ++it) {
			Graph::EdgePath p = theConverters().getPath(*it, format);
			if (!p.empty() && (path.empty() || p.size() < path.size())) {
				backend_format = *it;
				path = p;
			}
		}
		if (path.empty()) {
			if (!put_in_tempdir) {
				// Only show this alert if this is an export to a non-temporary
				// file (not for previewing).
				Alert::error(_("Couldn't export file"), bformat(
					_("No information for exporting the format %1$s."),
					formats.prettyName(format)));
			}
			return false;
		}
		runparams.flavor = theConverters().getFlavor(path);

	} else {
		backend_format = format;
		// FIXME: Don't hardcode format names here, but use a flag
		if (backend_format == "pdflatex")
			runparams.flavor = OutputParams::PDFLATEX;
		else if (backend_format == "luatex")
			runparams.flavor = OutputParams::LUATEX;
		else if (backend_format == "xetex")
			runparams.flavor = OutputParams::XETEX;
	}

	string filename = latexName(false);
	filename = addName(temppath(), filename);
	filename = changeExtension(filename,
				   formats.extension(backend_format));

	// fix macros
	updateMacroInstances();

	// Plain text backend
	if (backend_format == "text") {
		runparams.flavor = OutputParams::TEXT;
		writePlaintextFile(*this, FileName(filename), runparams);
	}
	// HTML backend
	else if (backend_format == "xhtml") {
		runparams.flavor = OutputParams::HTML;
		switch (params().html_math_output) {
		case BufferParams::MathML: 
			runparams.math_flavor = OutputParams::MathAsMathML; 
			break;
		case BufferParams::HTML: 
			runparams.math_flavor = OutputParams::MathAsHTML; 
			break;
		case BufferParams::Images:
			runparams.math_flavor = OutputParams::MathAsImages; 
			break;
		case BufferParams::LaTeX:
			runparams.math_flavor = OutputParams::MathAsLaTeX; 
			break;											
		}
		
		makeLyXHTMLFile(FileName(filename), runparams);
	}	else if (backend_format == "lyx")
		writeFile(FileName(filename));
	// Docbook backend
	else if (isDocBook()) {
		runparams.nice = !put_in_tempdir;
		makeDocBookFile(FileName(filename), runparams);
	}
	// LaTeX backend
	else if (backend_format == format) {
		runparams.nice = true;
		if (!makeLaTeXFile(FileName(filename), string(), runparams))
			return false;
	} else if (!lyxrc.tex_allows_spaces
		   && contains(filePath(), ' ')) {
		Alert::error(_("File name error"),
			   _("The directory path to the document cannot contain spaces."));
		return false;
	} else {
		runparams.nice = false;
		if (!makeLaTeXFile(FileName(filename), filePath(), runparams))
			return false;
	}

	string const error_type = (format == "program")
		? "Build" : bufferFormat();
	ErrorList & error_list = d->errorLists[error_type];
	string const ext = formats.extension(format);
	FileName const tmp_result_file(changeExtension(filename, ext));
	bool const success = theConverters().convert(this, FileName(filename),
		tmp_result_file, FileName(absFileName()), backend_format, format,
		error_list);

	// Emit the signal to show the error list or copy it back to the
	// cloned Buffer so that it cab be emitted afterwards.
	if (format != backend_format) {
		if (d->cloned_buffer_) {
			d->cloned_buffer_->d->errorLists[error_type] = 
				d->errorLists[error_type];
		} else 
			errors(error_type);
		// also to the children, in case of master-buffer-view
		ListOfBuffers clist = getDescendents();
		ListOfBuffers::const_iterator cit = clist.begin();
		ListOfBuffers::const_iterator const cen = clist.end();
		for (; cit != cen; ++cit) {
			if (d->cloned_buffer_) {
				(*cit)->d->cloned_buffer_->d->errorLists[error_type] = 
					(*cit)->d->errorLists[error_type];
			} else
				(*cit)->errors(error_type, true);
		}
	}

	if (d->cloned_buffer_) {
		// Enable reverse dvi or pdf to work by copying back the texrow
		// object to the cloned buffer.
		// FIXME: There is a possibility of concurrent access to texrow
		// here from the main GUI thread that should be securized.
		d->cloned_buffer_->d->texrow = d->texrow;
		string const error_type = bufferFormat();
		d->cloned_buffer_->d->errorLists[error_type] = d->errorLists[error_type];
	}

	if (!success)
		return false;

	if (put_in_tempdir) {
		result_file = tmp_result_file.absFileName();
		return true;
	}

	result_file = changeExtension(d->exportFileName().absFileName(), ext);
	// We need to copy referenced files (e. g. included graphics
	// if format == "dvi") to the result dir.
	vector<ExportedFile> const files =
		runparams.exportdata->externalFiles(format);
	string const dest = onlyPath(result_file);
	bool use_force = use_gui ? lyxrc.export_overwrite == ALL_FILES
				 : force_overwrite == ALL_FILES;
	CopyStatus status = use_force ? FORCE : SUCCESS;
	
	vector<ExportedFile>::const_iterator it = files.begin();
	vector<ExportedFile>::const_iterator const en = files.end();
	for (; it != en && status != CANCEL; ++it) {
		string const fmt = formats.getFormatFromFile(it->sourceName);
		status = copyFile(fmt, it->sourceName,
			makeAbsPath(it->exportName, dest),
			it->exportName, status == FORCE);
	}

	if (status == CANCEL) {
		message(_("Document export cancelled."));
	} else if (tmp_result_file.exists()) {
		// Finally copy the main file
		use_force = use_gui ? lyxrc.export_overwrite != NO_FILES
				    : force_overwrite != NO_FILES;
		if (status == SUCCESS && use_force)
			status = FORCE;
		status = copyFile(format, tmp_result_file,
			FileName(result_file), result_file,
			status == FORCE);
		message(bformat(_("Document exported as %1$s "
			"to file `%2$s'"),
			formats.prettyName(format),
			makeDisplayPath(result_file)));
	} else {
		// This must be a dummy converter like fax (bug 1888)
		message(bformat(_("Document exported as %1$s"),
			formats.prettyName(format)));
	}

	return true;
}


bool Buffer::doExport(string const & format, bool put_in_tempdir,
		      bool includeall) const
{
	string result_file;
	// (1) export with all included children (omit \includeonly)
	if (includeall && !doExport(format, put_in_tempdir, true, result_file))
		return false;
	// (2) export with included children only
	return doExport(format, put_in_tempdir, false, result_file);
}


bool Buffer::preview(string const & format, bool includeall) const
{
	MarkAsExporting exporting(this);
	string result_file;
	// (1) export with all included children (omit \includeonly)
	if (includeall && !doExport(format, true, true))
		return false;
	// (2) export with included children only
	if (!doExport(format, true, false, result_file))
		return false;
	return formats.view(*this, FileName(result_file), format);
}


bool Buffer::isExportable(string const & format) const
{
	vector<string> backs = backends();
	for (vector<string>::const_iterator it = backs.begin();
	     it != backs.end(); ++it)
		if (theConverters().isReachable(*it, format))
			return true;
	return false;
}


vector<Format const *> Buffer::exportableFormats(bool only_viewable) const
{
	vector<string> const backs = backends();
	vector<Format const *> result =
		theConverters().getReachable(backs[0], only_viewable, true);
	for (vector<string>::const_iterator it = backs.begin() + 1;
	     it != backs.end(); ++it) {
		vector<Format const *>  r =
			theConverters().getReachable(*it, only_viewable, false);
		result.insert(result.end(), r.begin(), r.end());
	}
	return result;
}


vector<string> Buffer::backends() const
{
	vector<string> v;
	v.push_back(bufferFormat());
	// FIXME: Don't hardcode format names here, but use a flag
	if (v.back() == "latex")
		v.push_back("pdflatex");
	else if (v.back() == "xetex")
		v.push_back("luatex");
	v.push_back("xhtml");
	v.push_back("text");
	v.push_back("lyx");
	return v;
}


Buffer::ReadStatus Buffer::extractFromVC()
{
	bool const found = LyXVC::file_not_found_hook(d->filename);
	if (!found)
		return ReadFileNotFound;
	if (!d->filename.isReadableFile())
		return ReadVCError;
	return ReadSuccess;
}


Buffer::ReadStatus Buffer::loadEmergency()
{
	FileName const emergencyFile = getEmergencyFileName();
	if (!emergencyFile.exists() 
		  || emergencyFile.lastModified() <= d->filename.lastModified())
		return ReadFileNotFound;

	docstring const file = makeDisplayPath(d->filename.absFileName(), 20);
	docstring const text = bformat(_("An emergency save of the document "
		"%1$s exists.\n\nRecover emergency save?"), file);
	
	int const load_emerg = Alert::prompt(_("Load emergency save?"), text,
		0, 2, _("&Recover"), _("&Load Original"), _("&Cancel"));

	switch (load_emerg)
	{
	case 0: {
		docstring str;
		ReadStatus const ret_llf = loadThisLyXFile(emergencyFile);
		bool const success = (ret_llf == ReadSuccess);
		if (success) {
			if (isReadonly()) {
				Alert::warning(_("File is read-only"),
					bformat(_("An emergency file is succesfully loaded, "
					"but the original file %1$s is marked read-only. "
					"Please make sure to save the document as a different "
					"file."), from_utf8(d->filename.absFileName())));
			}
			markDirty();
			str = _("Document was successfully recovered.");
		} else
			str = _("Document was NOT successfully recovered.");
		str += "\n\n" + bformat(_("Remove emergency file now?\n(%1$s)"),
			makeDisplayPath(emergencyFile.absFileName()));

		int const del_emerg = 
			Alert::prompt(_("Delete emergency file?"), str, 1, 1,
				_("&Remove"), _("&Keep"));
		if (del_emerg == 0) {
			emergencyFile.removeFile();
			if (success)
				Alert::warning(_("Emergency file deleted"),
					_("Do not forget to save your file now!"), true);
			}
		return success ? ReadSuccess : ReadEmergencyFailure;
	}
	case 1: {
		int const del_emerg =
			Alert::prompt(_("Delete emergency file?"),
				_("Remove emergency file now?"), 1, 1,
				_("&Remove"), _("&Keep"));
		if (del_emerg == 0)
			emergencyFile.removeFile();
		return ReadOriginal;
	}

	default:
		break;
	}
	return ReadCancel;
}


Buffer::ReadStatus Buffer::loadAutosave()
{
	// Now check if autosave file is newer.
	FileName const autosaveFile = getAutosaveFileName();
	if (!autosaveFile.exists() 
		  || autosaveFile.lastModified() <= d->filename.lastModified()) 
		return ReadFileNotFound;

	docstring const file = makeDisplayPath(d->filename.absFileName(), 20);
	docstring const text = bformat(_("The backup of the document %1$s " 
		"is newer.\n\nLoad the backup instead?"), file);
	int const ret = Alert::prompt(_("Load backup?"), text, 0, 2,
		_("&Load backup"), _("Load &original"),	_("&Cancel"));
	
	switch (ret)
	{
	case 0: {
		ReadStatus const ret_llf = loadThisLyXFile(autosaveFile);
		// the file is not saved if we load the autosave file.
		if (ret_llf == ReadSuccess) {
			if (isReadonly()) {
				Alert::warning(_("File is read-only"),
					bformat(_("A backup file is succesfully loaded,  "
					"but the original file %1$s is marked read-only. "
					"Please make sure to save the document as a "
					"different file."), 
					from_utf8(d->filename.absFileName())));
			}
			markDirty();
			return ReadSuccess;
		}
		return ReadAutosaveFailure;
	}
	case 1:
		// Here we delete the autosave
		autosaveFile.removeFile();
		return ReadOriginal;
	default:
		break;
	}
	return ReadCancel;
}


Buffer::ReadStatus Buffer::loadLyXFile()
{
	if (!d->filename.isReadableFile()) {
		ReadStatus const ret_rvc = extractFromVC();
		if (ret_rvc != ReadSuccess)
			return ret_rvc;
	}

	ReadStatus const ret_re = loadEmergency();
	if (ret_re == ReadSuccess || ret_re == ReadCancel)
		return ret_re;
	
	ReadStatus const ret_ra = loadAutosave();
	if (ret_ra == ReadSuccess || ret_ra == ReadCancel)
		return ret_ra;

	return loadThisLyXFile(d->filename);
}


Buffer::ReadStatus Buffer::loadThisLyXFile(FileName const & fn)
{
	return readFile(fn);
}


void Buffer::bufferErrors(TeXErrors const & terr, ErrorList & errorList) const
{
	TeXErrors::Errors::const_iterator cit = terr.begin();
	TeXErrors::Errors::const_iterator end = terr.end();

	for (; cit != end; ++cit) {
		int id_start = -1;
		int pos_start = -1;
		int errorRow = cit->error_in_line;
		bool found = d->texrow.getIdFromRow(errorRow, id_start,
						       pos_start);
		int id_end = -1;
		int pos_end = -1;
		do {
			++errorRow;
			found = d->texrow.getIdFromRow(errorRow, id_end, pos_end);
		} while (found && id_start == id_end && pos_start == pos_end);

		errorList.push_back(ErrorItem(cit->error_desc,
			cit->error_text, id_start, pos_start, pos_end));
	}
}


void Buffer::setBuffersForInsets() const
{
	inset().setBuffer(const_cast<Buffer &>(*this)); 
}


void Buffer::updateBuffer(UpdateScope scope, UpdateType utype) const
{
	// Use the master text class also for child documents
	Buffer const * const master = masterBuffer();
	DocumentClass const & textclass = master->params().documentClass();
	
	// do this only if we are the top-level Buffer
	if (master == this)
		checkBibInfoCache();

	// keep the buffers to be children in this set. If the call from the
	// master comes back we can see which of them were actually seen (i.e.
	// via an InsetInclude). The remaining ones in the set need still be updated.
	static std::set<Buffer const *> bufToUpdate;
	if (scope == UpdateMaster) {
		// If this is a child document start with the master
		if (master != this) {
			bufToUpdate.insert(this);
			master->updateBuffer(UpdateMaster, utype);
			// Do this here in case the master has no gui associated with it. Then, 
			// the TocModel is not updated and TocModel::toc_ is invalid (bug 5699).
			if (!master->d->gui_)
				structureChanged();

			// was buf referenced from the master (i.e. not in bufToUpdate anymore)?
			if (bufToUpdate.find(this) == bufToUpdate.end())
				return;
		}

		// start over the counters in the master
		textclass.counters().reset();
	}

	// update will be done below for this buffer
	bufToUpdate.erase(this);

	// update all caches
	clearReferenceCache();
	updateMacros();

	Buffer & cbuf = const_cast<Buffer &>(*this);

	LASSERT(!text().paragraphs().empty(), /**/);

	// do the real work
	ParIterator parit = cbuf.par_iterator_begin();
	updateBuffer(parit, utype);

	if (master != this)
		// TocBackend update will be done later.
		return;

	cbuf.tocBackend().update();
	if (scope == UpdateMaster)
		cbuf.structureChanged();
}


static depth_type getDepth(DocIterator const & it)
{
	depth_type depth = 0;
	for (size_t i = 0 ; i < it.depth() ; ++i)
		if (!it[i].inset().inMathed())
			depth += it[i].paragraph().getDepth() + 1;
	// remove 1 since the outer inset does not count
	return depth - 1;
}

static depth_type getItemDepth(ParIterator const & it)
{
	Paragraph const & par = *it;
	LabelType const labeltype = par.layout().labeltype;

	if (labeltype != LABEL_ENUMERATE && labeltype != LABEL_ITEMIZE)
		return 0;

	// this will hold the lowest depth encountered up to now.
	depth_type min_depth = getDepth(it);
	ParIterator prev_it = it;
	while (true) {
		if (prev_it.pit())
			--prev_it.top().pit();
		else {
			// start of nested inset: go to outer par
			prev_it.pop_back();
			if (prev_it.empty()) {
				// start of document: nothing to do
				return 0;
			}
		}

		// We search for the first paragraph with same label
		// that is not more deeply nested.
		Paragraph & prev_par = *prev_it;
		depth_type const prev_depth = getDepth(prev_it);
		if (labeltype == prev_par.layout().labeltype) {
			if (prev_depth < min_depth)
				return prev_par.itemdepth + 1;
			if (prev_depth == min_depth)
				return prev_par.itemdepth;
		}
		min_depth = min(min_depth, prev_depth);
		// small optimization: if we are at depth 0, we won't
		// find anything else
		if (prev_depth == 0)
			return 0;
	}
}


static bool needEnumCounterReset(ParIterator const & it)
{
	Paragraph const & par = *it;
	LASSERT(par.layout().labeltype == LABEL_ENUMERATE, /**/);
	depth_type const cur_depth = par.getDepth();
	ParIterator prev_it = it;
	while (prev_it.pit()) {
		--prev_it.top().pit();
		Paragraph const & prev_par = *prev_it;
		if (prev_par.getDepth() <= cur_depth)
			return  prev_par.layout().labeltype != LABEL_ENUMERATE;
	}
	// start of nested inset: reset
	return true;
}


// set the label of a paragraph. This includes the counters.
void Buffer::Impl::setLabel(ParIterator & it, UpdateType utype) const
{
	BufferParams const & bp = owner_->masterBuffer()->params();
	DocumentClass const & textclass = bp.documentClass();
	Paragraph & par = it.paragraph();
	Layout const & layout = par.layout();
	Counters & counters = textclass.counters();

	if (par.params().startOfAppendix()) {
		// FIXME: only the counter corresponding to toplevel
		// sectioning should be reset
		counters.reset();
		counters.appendix(true);
	}
	par.params().appendix(counters.appendix());

	// Compute the item depth of the paragraph
	par.itemdepth = getItemDepth(it);

	if (layout.margintype == MARGIN_MANUAL
	    || layout.latextype == LATEX_BIB_ENVIRONMENT) {
		if (par.params().labelWidthString().empty())
			par.params().labelWidthString(par.expandLabel(layout, bp));
	} else {
		par.params().labelWidthString(docstring());
	}

	switch(layout.labeltype) {
	case LABEL_COUNTER:
		if (layout.toclevel <= bp.secnumdepth
		    && (layout.latextype != LATEX_ENVIRONMENT
			|| it.text()->isFirstInSequence(it.pit()))) {
			counters.step(layout.counter, utype);
			par.params().labelString(
				par.expandLabel(layout, bp));
		} else
			par.params().labelString(docstring());
		break;

	case LABEL_ITEMIZE: {
		// At some point of time we should do something more
		// clever here, like:
		//   par.params().labelString(
		//    bp.user_defined_bullet(par.itemdepth).getText());
		// for now, use a simple hardcoded label
		docstring itemlabel;
		switch (par.itemdepth) {
		case 0:
			itemlabel = char_type(0x2022);
			break;
		case 1:
			itemlabel = char_type(0x2013);
			break;
		case 2:
			itemlabel = char_type(0x2217);
			break;
		case 3:
			itemlabel = char_type(0x2219); // or 0x00b7
			break;
		}
		par.params().labelString(itemlabel);
		break;
	}

	case LABEL_ENUMERATE: {
		docstring enumcounter = layout.counter.empty() ? from_ascii("enum") : layout.counter;

		switch (par.itemdepth) {
		case 2:
			enumcounter += 'i';
		case 1:
			enumcounter += 'i';
		case 0:
			enumcounter += 'i';
			break;
		case 3:
			enumcounter += "iv";
			break;
		default:
			// not a valid enumdepth...
			break;
		}

		// Maybe we have to reset the enumeration counter.
		if (needEnumCounterReset(it))
			counters.reset(enumcounter);
		counters.step(enumcounter, utype);

		string const & lang = par.getParLanguage(bp)->code();
		par.params().labelString(counters.theCounter(enumcounter, lang));

		break;
	}

	case LABEL_SENSITIVE: {
		string const & type = counters.current_float();
		docstring full_label;
		if (type.empty())
			full_label = owner_->B_("Senseless!!! ");
		else {
			docstring name = owner_->B_(textclass.floats().getType(type).name());
			if (counters.hasCounter(from_utf8(type))) {
				string const & lang = par.getParLanguage(bp)->code();
				counters.step(from_utf8(type), utype);
				full_label = bformat(from_ascii("%1$s %2$s:"), 
						     name, 
						     counters.theCounter(from_utf8(type), lang));
			} else
				full_label = bformat(from_ascii("%1$s #:"), name);	
		}
		par.params().labelString(full_label);	
		break;
	}

	case LABEL_NO_LABEL:
		par.params().labelString(docstring());
		break;

	case LABEL_MANUAL:
	case LABEL_TOP_ENVIRONMENT:
	case LABEL_CENTERED_TOP_ENVIRONMENT:
	case LABEL_STATIC:	
	case LABEL_BIBLIO:
		par.params().labelString(par.expandLabel(layout, bp));
		break;
	}
}


void Buffer::updateBuffer(ParIterator & parit, UpdateType utype) const
{
	LASSERT(parit.pit() == 0, /**/);

	// Set the position of the text in the buffer to be able
	// to resolve macros in it.
	parit.text()->setMacrocontextPosition(parit);

	depth_type maxdepth = 0;
	pit_type const lastpit = parit.lastpit();
	for ( ; parit.pit() <= lastpit ; ++parit.pit()) {
		// reduce depth if necessary
		parit->params().depth(min(parit->params().depth(), maxdepth));
		maxdepth = parit->getMaxDepthAfter();

		if (utype == OutputUpdate) {
			// track the active counters
			// we have to do this for the master buffer, since the local
			// buffer isn't tracking anything.
			masterBuffer()->params().documentClass().counters().
					setActiveLayout(parit->layout());
		}
		
		// set the counter for this paragraph
		d->setLabel(parit, utype);

		// now the insets
		InsetList::const_iterator iit = parit->insetList().begin();
		InsetList::const_iterator end = parit->insetList().end();
		for (; iit != end; ++iit) {
			parit.pos() = iit->pos;
			iit->inset->updateBuffer(parit, utype);
		}
	}
}


int Buffer::spellCheck(DocIterator & from, DocIterator & to,
	WordLangTuple & word_lang, docstring_list & suggestions) const
{
	int progress = 0;
	WordLangTuple wl;
	suggestions.clear();
	word_lang = WordLangTuple();
	// OK, we start from here.
	DocIterator const end = doc_iterator_end(this);
	for (; from != end; from.forwardPos()) {
		// We are only interested in text so remove the math CursorSlice.
		while (from.inMathed()) {
			from.pop_back();
			from.pos()++;
		}
		// If from is at the end of the document (which is possible
		// when leaving the mathed) LyX will crash later.
		if (from == end)
			break;
		to = from;
		from.paragraph().spellCheck();
		SpellChecker::Result res = from.paragraph().spellCheck(from.pos(), to.pos(), wl, suggestions);
		if (SpellChecker::misspelled(res)) {
			word_lang = wl;
			break;
		}

		// Do not increase progress when from == to, otherwise the word
		// count will be wrong.
		if (from != to) {
			from = to;
			++progress;
		}
	}
	return progress;
}


Buffer::ReadStatus Buffer::reload()
{
	setBusy(true);
	// c.f. bug http://www.lyx.org/trac/ticket/6587
	removeAutosaveFile();
	// e.g., read-only status could have changed due to version control
	d->filename.refresh();
	docstring const disp_fn = makeDisplayPath(d->filename.absFileName());

	ReadStatus const status = loadLyXFile();
	if (status == ReadSuccess) {
		updateBuffer();
		changed(true);
		updateTitles();
		markClean();
		message(bformat(_("Document %1$s reloaded."), disp_fn));
	} else {
		message(bformat(_("Could not reload document %1$s."), disp_fn));
	}	
	setBusy(false);
	removePreviews();
	updatePreviews();
	errors("Parse");
	return status;
}


bool Buffer::saveAs(FileName const & fn)
{
	FileName const old_name = fileName();
	FileName const old_auto = getAutosaveFileName();
	bool const old_unnamed = isUnnamed();

	setFileName(fn);
	markDirty();
	setUnnamed(false);

	if (save()) {
		// bring the autosave file with us, just in case.
		moveAutosaveFile(old_auto);
		// validate version control data and
		// correct buffer title
		lyxvc().file_found_hook(fileName());
		updateTitles();
		// the file has now been saved to the new location.
		// we need to check that the locations of child buffers
		// are still valid.
		checkChildBuffers();
		return true;
	} else {
		// save failed
		// reset the old filename and unnamed state
		setFileName(old_name);
		setUnnamed(old_unnamed);
		return false;
	}
}


// FIXME We could do better here, but it is complicated. What would be
// nice is to offer either (a) to save the child buffer to an appropriate
// location, so that it would "move with the master", or else (b) to update
// the InsetInclude so that it pointed to the same file. But (a) is a bit
// complicated, because the code for this lives in GuiView.
void Buffer::checkChildBuffers()
{
	Impl::BufferPositionMap::iterator it = d->children_positions.begin();
	Impl::BufferPositionMap::iterator const en = d->children_positions.end();
	for (; it != en; ++it) {
		DocIterator dit = it->second;
		Buffer * cbuf = const_cast<Buffer *>(it->first);
		if (!cbuf || !theBufferList().isLoaded(cbuf))
			continue;
		Inset * inset = dit.nextInset();
		LASSERT(inset && inset->lyxCode() == INCLUDE_CODE, continue);
		InsetInclude * inset_inc = static_cast<InsetInclude *>(inset);
		docstring const & incfile = inset_inc->getParam("filename");
		string oldloc = cbuf->absFileName();
		string newloc = makeAbsPath(to_utf8(incfile),
				onlyPath(absFileName())).absFileName();
		if (oldloc == newloc)
			continue;
		// the location of the child file is incorrect.
		Alert::warning(_("Included File Invalid"),
				bformat(_("Saving this document to a new location has made the file:\n"
				"  %1$s\n"
				"inaccessible. You will need to update the included filename."),
				from_utf8(oldloc)));
		cbuf->setParent(0);
		inset_inc->setChildBuffer(0);
	}
	// invalidate cache of children
	d->children_positions.clear();
	d->position_to_children.clear();
}

} // namespace lyx
