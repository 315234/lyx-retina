/**
 * \file LaTeXFeatures.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jos� Matos
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author J�rgen Vigna
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "LaTeXFeatures.h"

#include "BufferParams.h"
#include "Color.h"
#include "debug.h"
#include "Encoding.h"
#include "Floating.h"
#include "FloatList.h"
#include "Language.h"
#include "Lexer.h"
#include "LyXRC.h"

#include "support/docstream.h"
#include "support/filetools.h"

#include "frontends/controllers/frontend_helpers.h"

namespace lyx {

using support::isSGMLFilename;
using support::libFileSearch;
using support::makeRelPath;
using support::onlyPath;

using std::endl;
using std::find;
using std::string;
using std::list;
using std::ostream;
using std::ostringstream;
using std::set;

/////////////////////////////////////////////////////////////////////
//
// Strings
//
/////////////////////////////////////////////////////////////////////

//\NeedsTeXFormat{LaTeX2e}
//\ProvidesPackage{lyx}[1996/01/11 LLE v0.2 (LyX LaTeX Extensions)]
//\message{LyX LaTeX Extensions (LLE v0.2) of 11-Jan-1996.}

static string const lyx_def =
	"\\providecommand{\\LyX}{L\\kern-.1667em\\lower.25em\\hbox{Y}\\kern-.125emX\\@}";

static string const lyxline_def =
	"\\newcommand{\\lyxline}[1][1pt]{%\n"
	"  \\par\\noindent%\n"
	"  \\rule[.5ex]{\\linewidth}{#1}\\par}";

static string const noun_def = "\\newcommand{\\noun}[1]{\\textsc{#1}}";

static string const lyxarrow_def =
	"\\newcommand{\\lyxarrow}{\\leavevmode\\,$\\triangleright$\\,\\allowbreak}";

// for quotes without babel. This does not give perfect results, but
// anybody serious about non-english quotes should use babel (JMarc).

static string const quotedblbase_def =
	"\\ProvideTextCommandDefault{\\quotedblbase}{%\n"
	"  \\raisebox{-1.4ex}[1ex][.5ex]{\\textquotedblright}%\n"
	"  \\penalty10000\\hskip0em\\relax%\n"
	"}";

static string const quotesinglbase_def =
	"\\ProvideTextCommandDefault{\\quotesinglbase}{%\n"
	"  \\raisebox{-1.4ex}[1ex][.5ex]{\\textquoteright}%\n"
	"  \\penalty10000\\hskip0em\\relax%\n"
	"}";

static string const guillemotleft_def =
	"\\ProvideTextCommandDefault{\\guillemotleft}{%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'50\\kern-.15em\\char'50}%\n"
	"\\penalty10000\\hskip0pt\\relax%\n"
	"}";

static string const guillemotright_def =
	"\\ProvideTextCommandDefault{\\guillemotright}{%\n"
	"  \\penalty10000\\hskip0pt%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'51\\kern-.15em\\char'51}%\n"
	"}";

static string const guilsinglleft_def =
	"\\ProvideTextCommandDefault{\\guilsinglleft}{%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'50}%\n"
	"  \\penalty10000\\hskip0pt\\relax%\n"
	"}";

static string const guilsinglright_def =
	"\\ProvideTextCommandDefault{\\guilsinglright}{%\n"
	"  \\penalty10000\\hskip0pt%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'51}%\n"
	"}";

static string const paragraphleftindent_def =
	"\\newenvironment{LyXParagraphLeftIndent}[1]%\n"
	"{\n"
	"  \\begin{list}{}{%\n"
	"    \\setlength{\\topsep}{0pt}%\n"
	"    \\addtolength{\\leftmargin}{#1}\n"
// ho hum, yet more things commented out with no hint as to why they
// weren't just removed
//	"%%    \\addtolength{\\leftmargin}{#1\\textwidth}\n"
//	"%%    \\setlength{\\textwidth}{#2\\textwidth}\n"
//	"%%    \\setlength\\listparindent\\parindent%\n"
//	"%%    \\setlength\\itemindent\\parindent%\n"
	"    \\setlength{\\parsep}{0pt plus 1pt}%\n"
	"  }\n"
	"  \\item[]\n"
	"}\n"
	"{\\end{list}}\n";

static string const floatingfootnote_def =
	"%% Special footnote code from the package 'stblftnt.sty'\n"
	"%% Author: Robin Fairbairns -- Last revised Dec 13 1996\n"
	"\\let\\SF@@footnote\\footnote\n"
	"\\def\\footnote{\\ifx\\protect\\@typeset@protect\n"
	"    \\expandafter\\SF@@footnote\n"
	"  \\else\n"
	"    \\expandafter\\SF@gobble@opt\n"
	"  \\fi\n"
	"}\n"
	"\\expandafter\\def\\csname SF@gobble@opt \\endcsname{\\@ifnextchar[%]\n"
	"  \\SF@gobble@twobracket\n"
	"  \\@gobble\n"
	"}\n"
	"\\edef\\SF@gobble@opt{\\noexpand\\protect\n"
	"  \\expandafter\\noexpand\\csname SF@gobble@opt \\endcsname}\n"
	"\\def\\SF@gobble@twobracket[#1]#2{}\n";

static string const boldsymbol_def =
	"%% Bold symbol macro for standard LaTeX users\n"
	"\\providecommand{\\boldsymbol}[1]{\\mbox{\\boldmath $#1$}}\n";

static string const binom_def =
	"%% Binom macro for standard LaTeX users\n"
	"\\newcommand{\\binom}[2]{{#1 \\choose #2}}\n";

static string const mathcircumflex_def =
	"%% For printing a cirumflex inside a formula\n"
	"\\newcommand{\\mathcircumflex}[0]{\\mbox{\\^{}}}\n";

static string const tabularnewline_def =
	"%% Because html converters don't know tabularnewline\n"
	"\\providecommand{\\tabularnewline}{\\\\}\n";

static string const lyxgreyedout_def =
	"%% The greyedout annotation environment\n"
	"\\newenvironment{lyxgreyedout}{\\textcolor[gray]{0.8}\\bgroup}{\\egroup}\n";

// We want to omit the file extension for includegraphics, but this does not
// work when the filename contains other dots.
// Idea from http://www.tex.ac.uk/cgi-bin/texfaq2html?label=unkgrfextn
static string const lyxdot_def =
	"%% A simple dot to overcome graphicx limitations\n"
	"\\newcommand{\\lyxdot}{.}\n";

static string const changetracking_dvipost_def =
	"%% Change tracking with dvipost\n"
	"\\dvipostlayout\n"
	"\\dvipost{osstart color push Red}\n"
	"\\dvipost{osend color pop}\n"
	"\\dvipost{cbstart color push Blue}\n"
	"\\dvipost{cbend color pop}\n"
	"\\newcommand{\\lyxinserted}[3]{\\changestart#3\\changeend}\n"
	"\\newcommand{\\lyxdeleted}[3]{%\n"
	"\\changestart\\overstrikeon#3\\overstrikeoff\\changeend}\n";

// TODO
//static string const changetracking_soul_def =
//	"\\newcommand{\\lyxinserted}[3]{\\uwave{\\textcolor{blue}{#3}}}\n"
//	"\\newcommand{\\lyxdeleted}[3]{\\sout{\\textcolor{red}{#3}}}";

static string const changetracking_none_def =
	"\\newcommand{\\lyxinserted}[3]{#3}\n"
	"\\newcommand{\\lyxdeleted}[3]{}";


/////////////////////////////////////////////////////////////////////
//
// LaTeXFeatures
//
/////////////////////////////////////////////////////////////////////

LaTeXFeatures::PackagesList LaTeXFeatures::packages_;


LaTeXFeatures::LaTeXFeatures(Buffer const & b, BufferParams const & p,
			     OutputParams const & r)
	: buffer_(&b), params_(p), runparams_(r)
{}


bool LaTeXFeatures::useBabel() const
{
	return lyxrc.language_use_babel ||
		(bufferParams().language->lang() != lyxrc.default_language &&
		 !bufferParams().language->babel().empty()) ||
		this->hasLanguages();
}


void LaTeXFeatures::require(string const & name)
{
	if (isRequired(name))
		return;

	features_.push_back(name);
}


void LaTeXFeatures::getAvailable()
{
	Lexer lex(0, 0);
	support::FileName const real_file = libFileSearch("", "packages.lst");

	if (real_file.empty())
		return;

	lex.setFile(real_file);

	if (!lex.isOK())
		return;

	// Make sure that we are clean
	packages_.clear();

	bool finished = false;
	// Parse config-file
	while (lex.isOK() && !finished) {
		switch (lex.lex()) {
		case Lexer::LEX_FEOF:
			finished = true;
			break;
		default:
			string const name = lex.getString();
			PackagesList::const_iterator begin = packages_.begin();
			PackagesList::const_iterator end   = packages_.end();
			if (find(begin, end, name) == end)
				packages_.push_back(name);
		}
	}
}


void LaTeXFeatures::useLayout(string const & layoutname)
{
	// Some code to avoid loops in dependency definition
	static int level = 0;
	const int maxlevel = 30;
	if (level > maxlevel) {
		lyxerr << "LaTeXFeatures::useLayout: maximum level of "
		       << "recursion attained by layout "
		       << layoutname << endl;
		return;
	}

	TextClass const & tclass = params_.getTextClass();
	if (tclass.hasLayout(layoutname)) {
		// Is this layout already in usedLayouts?
		list<string>::const_iterator cit = usedLayouts_.begin();
		list<string>::const_iterator end = usedLayouts_.end();
		for (; cit != end; ++cit) {
			if (layoutname == *cit)
				return;
		}

		Layout_ptr const & lyt = tclass[layoutname];
		if (!lyt->depends_on().empty()) {
			++level;
			useLayout(lyt->depends_on());
			--level;
		}
		usedLayouts_.push_back(layoutname);
	} else {
		lyxerr << "LaTeXFeatures::useLayout: layout `"
		       << layoutname << "' does not exist in this class"
		       << endl;
	}

	--level;
}


bool LaTeXFeatures::isRequired(string const & name) const
{
	return find(features_.begin(), features_.end(), name) != features_.end();
}


bool LaTeXFeatures::mustProvide(string const & name) const
{
	return isRequired(name) && !params_.getTextClass().provides(name);
}


bool LaTeXFeatures::isAvailable(string const & name)
{
	if (packages_.empty())
		getAvailable();
	return find(packages_.begin(), packages_.end(), name) != packages_.end();
}


void LaTeXFeatures::addPreambleSnippet(string const & preamble)
{
	FeaturesList::const_iterator begin = preamble_snippets_.begin();
	FeaturesList::const_iterator end   = preamble_snippets_.end();
	if (find(begin, end, preamble) == end)
		preamble_snippets_.push_back(preamble);
}


void LaTeXFeatures::useFloat(string const & name)
{
	usedFloats_.insert(name);
	// We only need float.sty if we use non builtin floats, or if we
	// use the "H" modifier. This includes modified table and
	// figure floats. (Lgb)
	Floating const & fl = params_.getTextClass().floats().getType(name);
	if (!fl.type().empty() && !fl.builtin()) {
		require("float");
	}
}


void LaTeXFeatures::useLanguage(Language const * lang)
{
	if (!lang->babel().empty())
		UsedLanguages_.insert(lang);
}


void LaTeXFeatures::includeFile(docstring const & key, string const & name)
{
	IncludedFiles_[key] = name;
}


bool LaTeXFeatures::hasLanguages() const
{
	return !UsedLanguages_.empty();
}


string LaTeXFeatures::getLanguages() const
{
	ostringstream languages;

	LanguageList::const_iterator const begin = UsedLanguages_.begin();
	for (LanguageList::const_iterator cit = begin;
	     cit != UsedLanguages_.end();
	     ++cit) {
		if (cit != begin)
			languages << ',';
		languages << (*cit)->babel();
	}
	return languages.str();
}


set<string> LaTeXFeatures::getEncodingSet(string const & doc_encoding) const
{
	// This does only find encodings of languages supported by babel, but
	// that does not matter since we don't have a language with an
	// encoding supported by inputenc but without babel support.
	set<string> encodings;
	LanguageList::const_iterator it  = UsedLanguages_.begin();
	LanguageList::const_iterator end = UsedLanguages_.end();
	for (; it != end; ++it)
		if ((*it)->encoding()->latexName() != doc_encoding &&
		    (*it)->encoding()->package() == Encoding::inputenc)
			encodings.insert((*it)->encoding()->latexName());
	return encodings;
}

namespace {

char const * simplefeatures[] = {
	"array",
	"verbatim",
	"longtable",
	"rotating",
	"latexsym",
	"pifont",
	"subfigure",
	"floatflt",
	"varioref",
	"prettyref",
	"float",
	"booktabs",
	"dvipost",
	"fancybox",
	"calc",
	"nicefrac",
	"tipa",
	"framed",
	"textcomp",
};

int const nb_simplefeatures = sizeof(simplefeatures) / sizeof(char const *);

}


string const LaTeXFeatures::getPackages() const
{
	ostringstream packages;
	TextClass const & tclass = params_.getTextClass();

	//
	//  These are all the 'simple' includes.  i.e
	//  packages which we just \usepackage{package}
	//
	for (int i = 0; i < nb_simplefeatures; ++i) {
		if (mustProvide(simplefeatures[i]))
			packages << "\\usepackage{"
				 << simplefeatures[i] << "}\n";
	}

	//
	// The rest of these packages are somewhat more complicated
	// than those above.
	//

	if (mustProvide("amsmath")
	    && params_.use_amsmath != BufferParams::package_off) {
		packages << "\\usepackage{amsmath}\n";
	}

	// wasysym is a simple feature, but it must be after amsmath if both
	// are used
	// wasysym redefines some integrals (e.g. iint) from amsmath. That
	// leads to inconsistent integrals. We only load this package if
	// esint is used, since esint redefines all relevant integral
	// symbols from wasysym and amsmath.
	// See http://bugzilla.lyx.org/show_bug.cgi?id=1942
	if (mustProvide("wasysym") && isRequired("esint") &&
	    params_.use_esint != BufferParams::package_off)
		packages << "\\usepackage{wasysym}\n";

	// color.sty
	if (mustProvide("color")) {
		if (params_.graphicsDriver == "default")
			packages << "\\usepackage{color}\n";
		else
			packages << "\\usepackage["
				 << params_.graphicsDriver
				 << "]{color}\n";
	}

	// makeidx.sty
	if (isRequired("makeidx")) {
		if (!tclass.provides("makeidx"))
			packages << "\\usepackage{makeidx}\n";
		packages << "\\makeindex\n";
	}

	// graphicx.sty
	if (mustProvide("graphicx") && params_.graphicsDriver != "none") {
		if (params_.graphicsDriver == "default")
			packages << "\\usepackage{graphicx}\n";
		else
			packages << "\\usepackage["
				 << params_.graphicsDriver
				 << "]{graphicx}\n";
	}
	// shadecolor for shaded
	if (mustProvide("framed")) {
		RGBColor c = RGBColor(lcolor.getX11Name(Color::shadedbg));
		packages << "\\definecolor{shadecolor}{rgb}{" 
			<< c.r/255 << ',' << c.g/255 << ',' << c.b/255 << "}\n";
	}

	// lyxskak.sty --- newer chess support based on skak.sty
	if (mustProvide("chess")) {
		packages << "\\usepackage[ps,mover]{lyxskak}\n";
	}

	// setspace.sty
	if ((params_.spacing().getSpace() != Spacing::Single
	     && !params_.spacing().isDefault())
	    || isRequired("setspace")) {
		packages << "\\usepackage{setspace}\n";
	}
	switch (params_.spacing().getSpace()) {
	case Spacing::Default:
	case Spacing::Single:
		// we dont use setspace.sty so dont print anything
		//packages += "\\singlespacing\n";
		break;
	case Spacing::Onehalf:
		packages << "\\onehalfspacing\n";
		break;
	case Spacing::Double:
		packages << "\\doublespacing\n";
		break;
	case Spacing::Other:
		packages << "\\setstretch{"
			 << params_.spacing().getValue() << "}\n";
		break;
	}

	// amssymb.sty
	if (mustProvide("amssymb") 
	    || params_.use_amsmath == BufferParams::package_on)
		packages << "\\usepackage{amssymb}\n";

	// esint must be after amsmath and wasysym, since it will redeclare
	// inconsistent integral symbols
	if (mustProvide("esint") 
	    && params_.use_esint != BufferParams::package_off)
		packages << "\\usepackage{esint}\n";

	// url.sty
	if (mustProvide("url"))
		packages << "\\IfFileExists{url.sty}{\\usepackage{url}}\n"
			    "                      {\\newcommand{\\url}{\\texttt}}\n";

	// natbib.sty
	if (mustProvide("natbib")) {
		packages << "\\usepackage[";
		if (params_.getEngine() == biblio::ENGINE_NATBIB_NUMERICAL) {
			packages << "numbers";
		} else {
			packages << "authoryear";
		}
		packages << "]{natbib}\n";
	}

	// jurabib -- we need version 0.6 at least.
	if (mustProvide("jurabib")) {
		packages << "\\usepackage{jurabib}[2004/01/25]\n";
	}

	// bibtopic -- the dot provides the aux file naming which
	// LyX can detect.
	if (mustProvide("bibtopic")) {
		packages << "\\usepackage[dot]{bibtopic}\n";
	}

	if (mustProvide("xy"))
		packages << "\\usepackage[all]{xy}\n";

	if (mustProvide("nomencl")) {
		// Make it work with the new and old version of the package,
		// but don't use the compatibility option since it is
		// incompatible to other packages.
		packages << "\\usepackage{nomencl}\n"
	                    "% the following is useful when we have the old nomencl.sty package\n"
	                    "\\providecommand{\\printnomenclature}{\\printglossary}\n"
	                    "\\providecommand{\\makenomenclature}{\\makeglossary}\n"
		            "\\makenomenclature\n";
	}

	if (mustProvide("listings"))
		packages << "\\usepackage{listings}\n";
 
	return packages.str();
}


string const LaTeXFeatures::getMacros() const
{
	ostringstream macros;

	if (!preamble_snippets_.empty())
		macros << '\n';
	FeaturesList::const_iterator pit  = preamble_snippets_.begin();
	FeaturesList::const_iterator pend = preamble_snippets_.end();
	for (; pit != pend; ++pit) {
		macros << *pit << '\n';
	}

	if (mustProvide("LyX"))
		macros << lyx_def << '\n';

	if (mustProvide("lyxline"))
		macros << lyxline_def << '\n';

	if (mustProvide("noun"))
		macros << noun_def << '\n';

	if (mustProvide("lyxarrow"))
		macros << lyxarrow_def << '\n';

	// quotes.
	if (mustProvide("quotesinglbase"))
		macros << quotesinglbase_def << '\n';
	if (mustProvide("quotedblbase"))
		macros << quotedblbase_def << '\n';
	if (mustProvide("guilsinglleft"))
		macros << guilsinglleft_def << '\n';
	if (mustProvide("guilsinglright"))
		macros << guilsinglright_def << '\n';
	if (mustProvide("guillemotleft"))
		macros << guillemotleft_def << '\n';
	if (mustProvide("guillemotright"))
		macros << guillemotright_def << '\n';

	// Math mode
	if (mustProvide("boldsymbol") && !isRequired("amsmath"))
		macros << boldsymbol_def << '\n';
	if (mustProvide("binom") && !isRequired("amsmath"))
		macros << binom_def << '\n';
	if (mustProvide("mathcircumflex"))
		macros << mathcircumflex_def << '\n';

	// other
	if (mustProvide("ParagraphLeftIndent"))
		macros << paragraphleftindent_def;
	if (mustProvide("NeedLyXFootnoteCode"))
		macros << floatingfootnote_def;

	// some problems with tex->html converters
	if (mustProvide("NeedTabularnewline"))
		macros << tabularnewline_def;

	// greyedout environment (note inset)
	if (mustProvide("lyxgreyedout"))
		macros << lyxgreyedout_def;

	if (mustProvide("lyxdot"))
		macros << lyxdot_def << '\n';

	// floats
	getFloatDefinitions(macros);

	// change tracking
	if (mustProvide("dvipost"))
		macros << changetracking_dvipost_def;
	if (mustProvide("ct-none"))
		macros << changetracking_none_def;

	return macros.str();
}


string const LaTeXFeatures::getBabelOptions() const
{
	ostringstream tmp;

	LanguageList::const_iterator it  = UsedLanguages_.begin();
	LanguageList::const_iterator end =  UsedLanguages_.end();
	for (; it != end; ++it)
		if (!(*it)->latex_options().empty())
			tmp << (*it)->latex_options() << '\n';
	if (!params_.language->latex_options().empty())
		tmp << params_.language->latex_options() << '\n';

	return tmp.str();
}


docstring const LaTeXFeatures::getTClassPreamble() const
{
	// the text class specific preamble
	TextClass const & tclass = params_.getTextClass();
	odocstringstream tcpreamble;

	tcpreamble << tclass.preamble();

	list<string>::const_iterator cit = usedLayouts_.begin();
	list<string>::const_iterator end = usedLayouts_.end();
	for (; cit != end; ++cit) {
		tcpreamble << tclass[*cit]->preamble();
	}

	CharStyles::iterator cs = tclass.charstyles().begin();
	CharStyles::iterator csend = tclass.charstyles().end();
	for (; cs != csend; ++cs) {
		if (isRequired(cs->name))
			tcpreamble << cs->preamble;
	}

	return tcpreamble.str();
}


docstring const LaTeXFeatures::getLyXSGMLEntities() const
{
	// Definition of entities used in the document that are LyX related.
	odocstringstream entities;

	if (mustProvide("lyxarrow")) {
		entities << "<!ENTITY lyxarrow \"-&gt;\">" << '\n';
	}

	return entities.str();
}


docstring const LaTeXFeatures::getIncludedFiles(string const & fname) const
{
	odocstringstream sgmlpreamble;
	// FIXME UNICODE
	docstring const basename(from_utf8(onlyPath(fname)));

	FileMap::const_iterator end = IncludedFiles_.end();
	for (FileMap::const_iterator fi = IncludedFiles_.begin();
	     fi != end; ++fi)
		// FIXME UNICODE
		sgmlpreamble << "\n<!ENTITY " << fi->first
			     << (isSGMLFilename(fi->second) ? " SYSTEM \"" : " \"")
			     << makeRelPath(from_utf8(fi->second), basename) << "\">";

	return sgmlpreamble.str();
}


void LaTeXFeatures::showStruct() const {
	lyxerr << "LyX needs the following commands when LaTeXing:"
	       << "\n***** Packages:" << getPackages()
	       << "\n***** Macros:" << getMacros()
	       << "\n***** Textclass stuff:" << to_utf8(getTClassPreamble())
	       << "\n***** done." << endl;
}


Buffer const & LaTeXFeatures::buffer() const
{
	return *buffer_;
}


void LaTeXFeatures::setBuffer(Buffer const & buffer)
{
	buffer_ = &buffer;
}


BufferParams const & LaTeXFeatures::bufferParams() const
{
	return params_;
}


void LaTeXFeatures::getFloatDefinitions(ostream & os) const
{
	FloatList const & floats = params_.getTextClass().floats();

	// Here we will output the code to create the needed float styles.
	// We will try to do this as minimal as possible.
	// \floatstyle{ruled}
	// \newfloat{algorithm}{htbp}{loa}
	// \floatname{algorithm}{Algorithm}
	UsedFloats::const_iterator cit = usedFloats_.begin();
	UsedFloats::const_iterator end = usedFloats_.end();
	// ostringstream floats;
	for (; cit != end; ++cit) {
		Floating const & fl = floats.getType((*cit));

		// For builtin floats we do nothing.
		if (fl.builtin()) continue;

		// We have to special case "table" and "figure"
		if (fl.type() == "tabular" || fl.type() == "figure") {
			// Output code to modify "table" or "figure"
			// but only if builtin == false
			// and that have to be true at this point in the
			// function.
			string const type = fl.type();
			string const placement = fl.placement();
			string const style = fl.style();
			if (!style.empty()) {
				os << "\\floatstyle{" << style << "}\n"
				   << "\\restylefloat{" << type << "}\n";
			}
			if (!placement.empty()) {
				os << "\\floatplacement{" << type << "}{"
				   << placement << "}\n";
			}
		} else {
			// The other non builtin floats.

			string const type = fl.type();
			string const placement = fl.placement();
			string const ext = fl.ext();
			string const within = fl.within();
			string const style = fl.style();
			string const name = fl.name();
			os << "\\floatstyle{" << style << "}\n"
			   << "\\newfloat{" << type << "}{" << placement
			   << "}{" << ext << '}';
			if (!within.empty())
				os << '[' << within << ']';
			os << '\n'
			   << "\\floatname{" << type << "}{"
			   << name << "}\n";

			// What missing here is to code to minimalize the code
			// output so that the same floatstyle will not be
			// used several times, when the same style is still in
			// effect. (Lgb)
		}
	}
}


} // namespace lyx
