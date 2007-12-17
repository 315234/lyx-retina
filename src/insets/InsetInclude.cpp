/**
 * \file InsetInclude.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Richard Heck (conversion to InsetCommand)
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetInclude.h"

#include "Buffer.h"
#include "buffer_funcs.h"
#include "BufferList.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Cursor.h"
#include "support/debug.h"
#include "DispatchResult.h"
#include "Exporter.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "support/gettext.h"
#include "LaTeXFeatures.h"
#include "LyX.h"
#include "LyXRC.h"
#include "Lexer.h"
#include "MetricsInfo.h"
#include "OutputParams.h"
#include "TocBackend.h"
#include "TextClass.h"

#include "frontends/alert.h"
#include "frontends/Painter.h"

#include "graphics/PreviewImage.h"
#include "graphics/PreviewLoader.h"

#include "insets/RenderPreview.h"
#include "insets/InsetListingsParams.h"

#include "support/docstream.h"
#include "support/FileNameList.h"
#include "support/filetools.h"
#include "support/lstrings.h" // contains
#include "support/lyxalgo.h"
#include "support/convert.h"

#include <boost/bind.hpp>

using namespace std;
using namespace lyx::support;

namespace lyx {

namespace Alert = frontend::Alert;


namespace {

docstring const uniqueID()
{
	static unsigned int seed = 1000;
	return "file" + convert<docstring>(++seed);
}


/// the type of inclusion
enum Types {
	INCLUDE, VERB, INPUT, VERBAST, LISTINGS, NONE
};


Types type(string const & s)
{
	if (s == "input")
		return INPUT;
	if (s == "verbatiminput")
		return VERB;
	if (s == "verbatiminput*")
		return VERBAST;
	if (s == "lstinputlisting")
		return LISTINGS;
	if (s == "include")
		return INCLUDE;
	return NONE;
}


Types type(InsetCommandParams const & params)
{
	return type(params.getCmdName());
}


bool isListings(InsetCommandParams const & params)
{
	return type(params) == LISTINGS;
}


bool isVerbatim(InsetCommandParams const & params)
{
	Types const t = type(params);
	return t == VERB || t == VERBAST;
}


bool isInputOrInclude(InsetCommandParams const & params)
{
	Types const t = type(params);
 	return t == INPUT || t == INCLUDE;
}

} // namespace anon


InsetInclude::InsetInclude(InsetCommandParams const & p)
	: InsetCommand(p, "include"), include_label(uniqueID()),
	  preview_(new RenderMonitoredPreview(this)), set_label_(false)
{
	preview_->fileChanged(boost::bind(&InsetInclude::fileChanged, this));
}


InsetInclude::InsetInclude(InsetInclude const & other)
	: InsetCommand(other), include_label(other.include_label),
	  preview_(new RenderMonitoredPreview(this)), set_label_(false)
{
	preview_->fileChanged(boost::bind(&InsetInclude::fileChanged, this));
}


CommandInfo const * InsetInclude::findInfo(string const & /* cmdName */)
{
	// FIXME
	// This is only correct for the case of listings, but it'll do for now.
	// In the other cases, this second parameter should just be empty.
	static const char * const paramnames[] = {"filename", "lstparams", ""};
	static const bool isoptional[] = {false, true};
	static const CommandInfo info = {2, paramnames, isoptional};
	return &info;
}


bool InsetInclude::isCompatibleCommand(string const & s)
{
	return type(s) != NONE;
}


void InsetInclude::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action) {

	case LFUN_INSET_MODIFY: {
		InsetCommandParams p(INCLUDE_CODE);
		InsetCommandMailer::string2params("include", to_utf8(cmd.argument()), p);
		if (!p.getCmdName().empty()) {
			if (isListings(p)){
				InsetListingsParams par_old(to_utf8(params()["lstparams"]));
				InsetListingsParams par_new(to_utf8(p["lstparams"]));
				if (par_old.getParamValue("label") !=
				    par_new.getParamValue("label")
				    && !par_new.getParamValue("label").empty())
					cur.bv().buffer().changeRefsIfUnique(
						from_utf8(par_old.getParamValue("label")),
						from_utf8(par_new.getParamValue("label")),
						REF_CODE);
			}
			set(p, cur.buffer());
			cur.buffer().updateBibfilesCache();
		} else
			cur.noUpdate();
		break;
	}

	//pass everything else up the chain
	default:
		InsetCommand::doDispatch(cur, cmd);
		break;
	}
}


namespace {

FileName const masterFileName(Buffer const & buffer)
{
	return buffer.masterBuffer()->fileName();
}


string const parentFilename(Buffer const & buffer)
{
	return buffer.absFileName();
}


FileName const includedFilename(Buffer const & buffer,
			      InsetCommandParams const & params)
{
	return makeAbsPath(to_utf8(params["filename"]),
	       onlyPath(parentFilename(buffer)));
}


void add_preview(RenderMonitoredPreview &, InsetInclude const &, Buffer const &);

} // namespace anon


void InsetInclude::set(InsetCommandParams const & p, Buffer const & buffer)
{
	setParams(p);
	set_label_ = false;

	if (preview_->monitoring())
		preview_->stopMonitoring();

	if (type(params()) == INPUT)
		add_preview(*preview_, *this, buffer);
}


Inset * InsetInclude::clone() const
{
	return new InsetInclude(*this);
}


docstring const InsetInclude::getScreenLabel(Buffer const & buf) const
{
	docstring temp;

	switch (type(params())) {
		case INPUT:
			temp = buf.B_("Input");
			break;
		case VERB:
			temp = buf.B_("Verbatim Input");
			break;
		case VERBAST:
			temp = buf.B_("Verbatim Input*");
			break;
		case INCLUDE:
			temp = buf.B_("Include");
			break;
		case LISTINGS:
			temp = listings_label_;
			break;
		case NONE:
			BOOST_ASSERT(false);
	}

	temp += ": ";

	if (params()["filename"].empty())
		temp += "???";
	else
		temp += from_utf8(onlyFilename(to_utf8(params()["filename"])));

	return temp;
}


namespace {

/// return the child buffer if the file is a LyX doc and is loaded
Buffer * getChildBuffer(Buffer const & buffer, InsetCommandParams const & params)
{
	if (isVerbatim(params) || isListings(params))
		return 0;

	string const included_file = includedFilename(buffer, params).absFilename();
	if (!isLyXFilename(included_file))
		return 0;

	Buffer * childBuffer = theBufferList().getBuffer(included_file);

	//FIXME RECURSIVE INCLUDES
	if (childBuffer == & buffer)
		return 0;
	else
		return childBuffer;
}

} // namespace anon


/// return true if the file is or got loaded.
Buffer * loadIfNeeded(Buffer const & parent, InsetCommandParams const & params)
{
	if (isVerbatim(params) || isListings(params))
		return 0;

	string const parent_filename = parent.absFileName();
	FileName const included_file = makeAbsPath(to_utf8(params["filename"]),
			   onlyPath(parent_filename));

	if (!isLyXFilename(included_file.absFilename()))
		return 0;

	Buffer * child = theBufferList().getBuffer(included_file.absFilename());
	if (!child) {
		// the readonly flag can/will be wrong, not anymore I think.
		if (!included_file.exists())
			return 0;

		child = theBufferList().newBuffer(included_file.absFilename());
		if (!child->loadLyXFile(included_file)) {
			//close the buffer we just opened
			theBufferList().release(child);
			return 0;
		}
	}
	child->setParent(&parent);
	return child;
}


void resetParentBuffer(Buffer const * parent, InsetCommandParams const & params,
	bool close_it)
{
	if (isVerbatim(params) || isListings(params))
		return;

	string const parent_filename = parent->absFileName();
	FileName const included_file = makeAbsPath(to_utf8(params["filename"]),
			   onlyPath(parent_filename));

	if (!isLyXFilename(included_file.absFilename()))
		return;

	Buffer * child = theBufferList().getBuffer(included_file.absFilename());
	// File not opened, nothing to close.
	if (!child)
		return;

	// Child document has a different parent, don't close it.
	if (child->parent() != parent)
		return;

	//close the buffer.
	child->setParent(0);
	if (close_it)
		theBufferList().release(child);
	else
		updateLabels(*child);
}


int InsetInclude::latex(Buffer const & buffer, odocstream & os,
			OutputParams const & runparams) const
{
	string incfile(to_utf8(params()["filename"]));

	// Do nothing if no file name has been specified
	if (incfile.empty())
		return 0;

	FileName const included_file = includedFilename(buffer, params());

	//Check we're not trying to include ourselves.
	//FIXME RECURSIVE INCLUDE
	//This isn't sufficient, as the inclusion could be downstream.
	//But it'll have to do for now.
	if (isInputOrInclude(params()) &&
		buffer.absFileName() == included_file.absFilename())
	{
		Alert::error(_("Recursive input"),
			       bformat(_("Attempted to include file %1$s in itself! "
			       "Ignoring inclusion."), from_utf8(incfile)));
		return 0;
	}

	Buffer const * const masterBuffer = buffer.masterBuffer();

	// if incfile is relative, make it relative to the master
	// buffer directory.
	if (!FileName(incfile).isAbsolute()) {
		// FIXME UNICODE
		incfile = to_utf8(makeRelPath(from_utf8(included_file.absFilename()),
					      from_utf8(masterBuffer->filePath())));
	}

	// write it to a file (so far the complete file)
	string const exportfile = changeExtension(incfile, ".tex");
	string const mangled =
		DocFileName(changeExtension(included_file.absFilename(),".tex")).
			mangledFilename();
	FileName const writefile(makeAbsPath(mangled, masterBuffer->temppath()));

	if (!runparams.nice)
		incfile = mangled;
	else if (!isValidLaTeXFilename(incfile)) {
		frontend::Alert::warning(_("Invalid filename"),
					 _("The following filename is likely to cause trouble "
					   "when running the exported file through LaTeX: ") +
					    from_utf8(incfile));
	}
	LYXERR(Debug::LATEX, "incfile:" << incfile);
	LYXERR(Debug::LATEX, "exportfile:" << exportfile);
	LYXERR(Debug::LATEX, "writefile:" << writefile);

	if (runparams.inComment || runparams.dryrun) {
		//Don't try to load or copy the file if we're
		//in a comment or doing a dryrun
	} else if (isInputOrInclude(params()) &&
		 isLyXFilename(included_file.absFilename())) {
		//if it's a LyX file and we're inputting or including,
		//try to load it so we can write the associated latex
		if (!loadIfNeeded(buffer, params()))
			return false;

		Buffer * tmp = theBufferList().getBuffer(included_file.absFilename());

		if (tmp->params().getBaseClass() != masterBuffer->params().getBaseClass()) {
			// FIXME UNICODE
			docstring text = bformat(_("Included file `%1$s'\n"
						"has textclass `%2$s'\n"
							     "while parent file has textclass `%3$s'."),
					      included_file.displayName(),
					      from_utf8(tmp->params().getTextClass().name()),
					      from_utf8(masterBuffer->params().getTextClass().name()));
			Alert::warning(_("Different textclasses"), text);
			//return 0;
		}

		// Make sure modules used in child are all included in master
		//FIXME It might be worth loading the children's modules into the master
		//over in BufferParams rather than doing this check.
		vector<string> const masterModules = masterBuffer->params().getModules();
		vector<string> const childModules = tmp->params().getModules();
		vector<string>::const_iterator it = childModules.begin();
		vector<string>::const_iterator end = childModules.end();
		for (; it != end; ++it) {
			string const module = *it;
			vector<string>::const_iterator found =
				find(masterModules.begin(), masterModules.end(), module);
			if (found != masterModules.end()) {
				docstring text = bformat(_("Included file `%1$s'\n"
							"uses module `%2$s'\n"
							"which is not used in parent file."),
				       included_file.displayName(), from_utf8(module));
				Alert::warning(_("Module not found"), text);
			}
		}

		tmp->markDepClean(masterBuffer->temppath());

// FIXME: handle non existing files
// FIXME: Second argument is irrelevant!
// since only_body is true, makeLaTeXFile will not look at second
// argument. Should we set it to string(), or should makeLaTeXFile
// make use of it somehow? (JMarc 20031002)
		// The included file might be written in a different encoding
		Encoding const * const oldEnc = runparams.encoding;
		runparams.encoding = &tmp->params().encoding();
		tmp->makeLaTeXFile(writefile,
				   masterFileName(buffer).onlyPath().absFilename(),
				   runparams, false);
		runparams.encoding = oldEnc;
	} else {
		// In this case, it's not a LyX file, so we copy the file
		// to the temp dir, so that .aux files etc. are not created
		// in the original dir. Files included by this file will be
		// found via input@path, see ../Buffer.cpp.
		unsigned long const checksum_in  = included_file.checksum();
		unsigned long const checksum_out = writefile.checksum();

		if (checksum_in != checksum_out) {
			if (!included_file.copyTo(writefile)) {
				// FIXME UNICODE
				LYXERR(Debug::LATEX,
					to_utf8(bformat(_("Could not copy the file\n%1$s\n"
								  "into the temporary directory."),
						   from_utf8(included_file.absFilename()))));
				return 0;
			}
		}
	}

	string const tex_format = (runparams.flavor == OutputParams::LATEX) ?
			"latex" : "pdflatex";
	if (isVerbatim(params())) {
		incfile = latex_path(incfile);
		// FIXME UNICODE
		os << '\\' << from_ascii(params().getCmdName()) << '{'
		   << from_utf8(incfile) << '}';
	} else if (type(params()) == INPUT) {
		runparams.exportdata->addExternalFile(tex_format, writefile,
						      exportfile);

		// \input wants file with extension (default is .tex)
		if (!isLyXFilename(included_file.absFilename())) {
			incfile = latex_path(incfile);
			// FIXME UNICODE
			os << '\\' << from_ascii(params().getCmdName())
			   << '{' << from_utf8(incfile) << '}';
		} else {
		incfile = changeExtension(incfile, ".tex");
		incfile = latex_path(incfile);
			// FIXME UNICODE
			os << '\\' << from_ascii(params().getCmdName())
			   << '{' << from_utf8(incfile) <<  '}';
		}
	} else if (type(params()) == LISTINGS) {
		os << '\\' << from_ascii(params().getCmdName());
		string const opt = to_utf8(params()["lstparams"]);
		// opt is set in QInclude dialog and should have passed validation.
		InsetListingsParams params(opt);
		if (!params.params().empty())
			os << "[" << from_utf8(params.params()) << "]";
		os << '{'  << from_utf8(incfile) << '}';
	} else {
		runparams.exportdata->addExternalFile(tex_format, writefile,
						      exportfile);

		// \include don't want extension and demands that the
		// file really have .tex
		incfile = changeExtension(incfile, string());
		incfile = latex_path(incfile);
		// FIXME UNICODE
		os << '\\' << from_ascii(params().getCmdName()) << '{'
		   << from_utf8(incfile) << '}';
	}

	return 0;
}


int InsetInclude::plaintext(Buffer const & buffer, odocstream & os,
			    OutputParams const &) const
{
	if (isVerbatim(params()) || isListings(params())) {
		os << '[' << getScreenLabel(buffer) << '\n';
		// FIXME: We don't know the encoding of the file, default to UTF-8.
		os << includedFilename(buffer, params()).fileContents("UTF-8");
		os << "\n]";
		return PLAINTEXT_NEWLINE + 1; // one char on a separate line
	} else {
		docstring const str = '[' + getScreenLabel(buffer) + ']';
		os << str;
		return str.size();
	}
}


int InsetInclude::docbook(Buffer const & buffer, odocstream & os,
			  OutputParams const & runparams) const
{
	string incfile = to_utf8(params()["filename"]);

	// Do nothing if no file name has been specified
	if (incfile.empty())
		return 0;

	string const included_file = includedFilename(buffer, params()).absFilename();

	//Check we're not trying to include ourselves.
	//FIXME RECURSIVE INCLUDE
	//This isn't sufficient, as the inclusion could be downstream.
	//But it'll have to do for now.
	if (buffer.absFileName() == included_file) {
		Alert::error(_("Recursive input"),
			       bformat(_("Attempted to include file %1$s in itself! "
			       "Ignoring inclusion."), from_utf8(incfile)));
		return 0;
	}

	// write it to a file (so far the complete file)
	string const exportfile = changeExtension(incfile, ".sgml");
	DocFileName writefile(changeExtension(included_file, ".sgml"));

	if (loadIfNeeded(buffer, params())) {
		Buffer * tmp = theBufferList().getBuffer(included_file);

		string const mangled = writefile.mangledFilename();
		writefile = makeAbsPath(mangled,
					buffer.masterBuffer()->temppath());
		if (!runparams.nice)
			incfile = mangled;

		LYXERR(Debug::LATEX, "incfile:" << incfile);
		LYXERR(Debug::LATEX, "exportfile:" << exportfile);
		LYXERR(Debug::LATEX, "writefile:" << writefile);

		tmp->makeDocBookFile(writefile, runparams, true);
	}

	runparams.exportdata->addExternalFile("docbook", writefile,
					      exportfile);
	runparams.exportdata->addExternalFile("docbook-xml", writefile,
					      exportfile);

	if (isVerbatim(params()) || isListings(params())) {
		os << "<inlinegraphic fileref=\""
		   << '&' << include_label << ';'
		   << "\" format=\"linespecific\">";
	} else
		os << '&' << include_label << ';';

	return 0;
}


void InsetInclude::validate(LaTeXFeatures & features) const
{
	string incfile = to_utf8(params()["filename"]);
	string writefile;

	Buffer const & buffer = features.buffer();

	string const included_file = includedFilename(buffer, params()).absFilename();

	if (isLyXFilename(included_file))
		writefile = changeExtension(included_file, ".sgml");
	else
		writefile = included_file;

	if (!features.runparams().nice && !isVerbatim(params()) && !isListings(params())) {
		incfile = DocFileName(writefile).mangledFilename();
		writefile = makeAbsPath(incfile,
					buffer.masterBuffer()->temppath()).absFilename();
	}

	features.includeFile(include_label, writefile);

	if (isVerbatim(params()))
		features.require("verbatim");
	else if (isListings(params()))
		features.require("listings");

	// Here we must do the fun stuff...
	// Load the file in the include if it needs
	// to be loaded:
	if (loadIfNeeded(buffer, params())) {
		// a file got loaded
		Buffer * const tmp = theBufferList().getBuffer(included_file);
		// make sure the buffer isn't us
		// FIXME RECURSIVE INCLUDES
		// This is not sufficient, as recursive includes could be
		// more than a file away. But it will do for now.
		if (tmp && tmp != & buffer) {
			// We must temporarily change features.buffer,
			// otherwise it would always be the master buffer,
			// and nested includes would not work.
			features.setBuffer(*tmp);
			tmp->validate(features);
			features.setBuffer(buffer);
		}
	}
}


void InsetInclude::getLabelList(Buffer const & buffer,
				vector<docstring> & list) const
{
	if (isListings(params())) {
		InsetListingsParams p(to_utf8(params()["lstparams"]));
		string label = p.getParamValue("label");
		if (!label.empty())
			list.push_back(from_utf8(label));
	}
	else if (loadIfNeeded(buffer, params())) {
		string const included_file = includedFilename(buffer, params()).absFilename();
		Buffer * tmp = theBufferList().getBuffer(included_file);
		tmp->setParent(0);
		tmp->getLabelList(list);
		tmp->setParent(const_cast<Buffer *>(&buffer));
	}
}


void InsetInclude::fillWithBibKeys(Buffer const & buffer,
		BiblioInfo & keys, InsetIterator const & /*di*/) const
{
	if (loadIfNeeded(buffer, params())) {
		string const included_file = includedFilename(buffer, params()).absFilename();
		Buffer * tmp = theBufferList().getBuffer(included_file);
		//FIXME This is kind of a dirty hack and should be made reasonable.
		tmp->setParent(0);
		keys.fillWithBibKeys(tmp);
		tmp->setParent(&buffer);
	}
}


void InsetInclude::updateBibfilesCache(Buffer const & buffer)
{
	Buffer * const tmp = getChildBuffer(buffer, params());
	if (tmp) {
		tmp->setParent(0);
		tmp->updateBibfilesCache();
		tmp->setParent(&buffer);
	}
}


FileNameList const &
InsetInclude::getBibfilesCache(Buffer const & buffer) const
{
	Buffer * const tmp = getChildBuffer(buffer, params());
	if (tmp) {
		tmp->setParent(0);
		FileNameList const & cache = tmp->getBibfilesCache();
		tmp->setParent(&buffer);
		return cache;
	}
	static FileNameList const empty;
	return empty;
}


void InsetInclude::metrics(MetricsInfo & mi, Dimension & dim) const
{
	BOOST_ASSERT(mi.base.bv);

	bool use_preview = false;
	if (RenderPreview::status() != LyXRC::PREVIEW_OFF) {
		graphics::PreviewImage const * pimage =
			preview_->getPreviewImage(mi.base.bv->buffer());
		use_preview = pimage && pimage->image();
	}

	if (use_preview) {
		preview_->metrics(mi, dim);
	} else {
		if (!set_label_) {
			set_label_ = true;
			button_.update(getScreenLabel(mi.base.bv->buffer()),
				       true);
		}
		button_.metrics(mi, dim);
	}

	Box b(0, dim.wid, -dim.asc, dim.des);
	button_.setBox(b);
}


void InsetInclude::draw(PainterInfo & pi, int x, int y) const
{
	BOOST_ASSERT(pi.base.bv);

	bool use_preview = false;
	if (RenderPreview::status() != LyXRC::PREVIEW_OFF) {
		graphics::PreviewImage const * pimage =
			preview_->getPreviewImage(pi.base.bv->buffer());
		use_preview = pimage && pimage->image();
	}

	if (use_preview)
		preview_->draw(pi, x, y);
	else
		button_.draw(pi, x, y);
}


Inset::DisplayType InsetInclude::display() const
{
	return type(params()) == INPUT ? Inline : AlignCenter;
}



//
// preview stuff
//

void InsetInclude::fileChanged() const
{
	Buffer const * const buffer = updateFrontend();
	if (!buffer)
		return;

	preview_->removePreview(*buffer);
	add_preview(*preview_.get(), *this, *buffer);
	preview_->startLoading(*buffer);
}


namespace {

bool preview_wanted(InsetCommandParams const & params, Buffer const & buffer)
{
	FileName const included_file = includedFilename(buffer, params);

	return type(params) == INPUT && params.preview() &&
		included_file.isReadableFile();
}


docstring const latex_string(InsetInclude const & inset, Buffer const & buffer)
{
	odocstringstream os;
	// We don't need to set runparams.encoding since this will be done
	// by latex() anyway.
	OutputParams runparams(0);
	runparams.flavor = OutputParams::LATEX;
	inset.latex(buffer, os, runparams);

	return os.str();
}


void add_preview(RenderMonitoredPreview & renderer, InsetInclude const & inset,
		 Buffer const & buffer)
{
	InsetCommandParams const & params = inset.params();
	if (RenderPreview::status() != LyXRC::PREVIEW_OFF &&
	    preview_wanted(params, buffer)) {
		renderer.setAbsFile(includedFilename(buffer, params));
		docstring const snippet = latex_string(inset, buffer);
		renderer.addPreview(snippet, buffer);
	}
}

} // namespace anon


void InsetInclude::addPreview(graphics::PreviewLoader & ploader) const
{
	Buffer const & buffer = ploader.buffer();
	if (preview_wanted(params(), buffer)) {
		preview_->setAbsFile(includedFilename(buffer, params()));
		docstring const snippet = latex_string(*this, buffer);
		preview_->addPreview(snippet, ploader);
	}
}


void InsetInclude::addToToc(TocList & toclist, Buffer const & buffer,
	ParConstIterator const & pit) const
{
	if (isListings(params())) {
		InsetListingsParams p(to_utf8(params()["lstparams"]));
		string caption = p.getParamValue("caption");
		if (caption.empty())
			return;
		Toc & toc = toclist["listing"];
		docstring const str = convert<docstring>(toc.size() + 1)
			+ ". " +  from_utf8(caption);
		// This inset does not have a valid ParConstIterator
		// so it has to use the iterator of its parent paragraph
		toc.push_back(TocItem(pit, 0, str));
		return;
	}
	Buffer const * const childbuffer = getChildBuffer(buffer, params());
	if (!childbuffer)
		return;

	TocList const & childtoclist = childbuffer->tocBackend().tocs();
	TocList::const_iterator it = childtoclist.begin();
	TocList::const_iterator const end = childtoclist.end();
	for(; it != end; ++it)
		toclist[it->first].insert(toclist[it->first].end(),
				it->second.begin(), it->second.end());
}


void InsetInclude::updateLabels(Buffer const & buffer, ParIterator const &)
{
	Buffer const * const childbuffer = getChildBuffer(buffer, params());
	if (childbuffer)
		lyx::updateLabels(*childbuffer, true);
	else if (isListings(params())) {
		InsetListingsParams const par(to_utf8(params()["lstparams"]));
		if (par.getParamValue("caption").empty())
			listings_label_.clear();
		else {
			Counters & counters = buffer.params().getTextClass().counters();
			docstring const cnt = from_ascii("listing");
			if (counters.hasCounter(cnt)) {
				counters.step(cnt);
				listings_label_ = buffer.B_("Program Listing ")
					+ convert<docstring>(counters.value(cnt));
			} else
				listings_label_ = buffer.B_("Program Listing");
		}
	}
}


void InsetInclude::registerEmbeddedFiles(Buffer const & buffer,
	EmbeddedFiles & files) const
{
	// include and input are temprarily not considered.
	if (isVerbatim(params()) || isListings(params()))
		files.registerFile(includedFilename(buffer, params()).absFilename(),
			false, this);
}

} // namespace lyx
