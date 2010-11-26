/**
 * \file LaTeXFeatures.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author José Matos
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author Jürgen Vigna
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "LaTeXFeatures.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "ColorSet.h"
#include "Converter.h"
#include "Encoding.h"
#include "Floating.h"
#include "FloatList.h"
#include "Language.h"
#include "Layout.h"
#include "Lexer.h"
#include "LyXRC.h"
#include "TextClass.h"

#include "insets/InsetLayout.h"

#include "support/debug.h"
#include "support/docstream.h"
#include "support/FileName.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/lstrings.h"

#include <algorithm>


using namespace std;
using namespace lyx::support;


namespace lyx {

/////////////////////////////////////////////////////////////////////
//
// Strings
//
/////////////////////////////////////////////////////////////////////

//\NeedsTeXFormat{LaTeX2e}
//\ProvidesPackage{lyx}[1996/01/11 LLE v0.2 (LyX LaTeX Extensions)]
//\message{LyX LaTeX Extensions (LLE v0.2) of 11-Jan-1996.}

static docstring const lyx_def = from_ascii(
	"\\providecommand{\\LyX}{L\\kern-.1667em\\lower.25em\\hbox{Y}\\kern-.125emX\\@}");

static docstring const noun_def = from_ascii(
	"\\newcommand{\\noun}[1]{\\textsc{#1}}");

static docstring const lyxarrow_def = from_ascii(
	"\\DeclareRobustCommand*{\\lyxarrow}{%\n"
	"\\@ifstar\n"
	"{\\leavevmode\\,$\\triangleleft$\\,\\allowbreak}\n"
	"{\\leavevmode\\,$\\triangleright$\\,\\allowbreak}}");

// for quotes without babel. This does not give perfect results, but
// anybody serious about non-english quotes should use babel (JMarc).

static docstring const quotedblbase_def = from_ascii(
	"\\ProvideTextCommandDefault{\\quotedblbase}{%\n"
	"  \\raisebox{-1.4ex}[1ex][.5ex]{\\textquotedblright}%\n"
	"  \\penalty10000\\hskip0em\\relax%\n"
	"}");

static docstring const quotesinglbase_def = from_ascii(
	"\\ProvideTextCommandDefault{\\quotesinglbase}{%\n"
	"  \\raisebox{-1.4ex}[1ex][.5ex]{\\textquoteright}%\n"
	"  \\penalty10000\\hskip0em\\relax%\n"
	"}");

static docstring const guillemotleft_def = from_ascii(
	"\\ProvideTextCommandDefault{\\guillemotleft}{%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'50\\kern-.15em\\char'50}%\n"
	"\\penalty10000\\hskip0pt\\relax%\n"
	"}");

static docstring const guillemotright_def = from_ascii(
	"\\ProvideTextCommandDefault{\\guillemotright}{%\n"
	"  \\penalty10000\\hskip0pt%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'51\\kern-.15em\\char'51}%\n"
	"}");

static docstring const guilsinglleft_def = from_ascii(
	"\\ProvideTextCommandDefault{\\guilsinglleft}{%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'50}%\n"
	"  \\penalty10000\\hskip0pt\\relax%\n"
	"}");

static docstring const guilsinglright_def = from_ascii(
	"\\ProvideTextCommandDefault{\\guilsinglright}{%\n"
	"  \\penalty10000\\hskip0pt%\n"
	"  {\\usefont{U}{lasy}{m}{n}\\char'51}%\n"
	"}");

static docstring const paragraphleftindent_def = from_ascii(
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
	"{\\end{list}}\n");

static docstring const floatingfootnote_def = from_ascii(
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
	"\\def\\SF@gobble@twobracket[#1]#2{}\n");

static docstring const binom_def = from_ascii(
	"%% Binom macro for standard LaTeX users\n"
	"\\newcommand{\\binom}[2]{{#1 \\choose #2}}\n");

static docstring const mathcircumflex_def = from_ascii(
	"%% For printing a cirumflex inside a formula\n"
	"\\newcommand{\\mathcircumflex}[0]{\\mbox{\\^{}}}\n");

static docstring const tabularnewline_def = from_ascii(
	"%% Because html converters don't know tabularnewline\n"
	"\\providecommand{\\tabularnewline}{\\\\}\n");
	
static docstring const lyxgreyedout_def = from_ascii(
	"%% The greyedout annotation environment\n"
	"\\newenvironment{lyxgreyedout}{\\textcolor{note_fontcolor}\\bgroup}{\\egroup}\n");

// We want to omit the file extension for includegraphics, but this does not
// work when the filename contains other dots.
// Idea from http://www.tex.ac.uk/cgi-bin/texfaq2html?label=unkgrfextn
static docstring const lyxdot_def = from_ascii(
	"%% A simple dot to overcome graphicx limitations\n"
	"\\newcommand{\\lyxdot}{.}\n");

static docstring const changetracking_dvipost_def = from_ascii(
	"%% Change tracking with dvipost\n"
	"\\dvipostlayout\n"
	"\\dvipost{osstart color push Red}\n"
	"\\dvipost{osend color pop}\n"
	"\\dvipost{cbstart color push Blue}\n"
	"\\dvipost{cbend color pop}\n"
	"\\newcommand{\\lyxadded}[3]{\\changestart#3\\changeend}\n"
	"\\newcommand{\\lyxdeleted}[3]{%\n"
	"\\changestart\\overstrikeon#3\\overstrikeoff\\changeend}\n");

static docstring const changetracking_xcolor_ulem_def = from_ascii(
	"%% Change tracking with ulem\n"
	"\\newcommand{\\lyxadded}[3]{{\\color{lyxadded}{}#3}}\n"
	"\\newcommand{\\lyxdeleted}[3]{{\\color{lyxdeleted}\\sout{#3}}}\n");

static docstring const changetracking_xcolor_ulem_hyperref_def = from_ascii(
	"%% Change tracking with ulem\n"
	"\\newcommand{\\lyxadded}[3]{{\\texorpdfstring{\\color{lyxadded}{}}{}#3}}\n"
	"\\newcommand{\\lyxdeleted}[3]{{\\texorpdfstring{\\color{lyxdeleted}\\sout{#3}}{}}}\n");

static docstring const changetracking_none_def = from_ascii(
	"\\newcommand{\\lyxadded}[3]{#3}\n"
	"\\newcommand{\\lyxdeleted}[3]{}\n");

static docstring const textgreek_def = from_ascii(
	"\\DeclareRobustCommand{\\greektext}{%\n"
	"  \\fontencoding{LGR}\\selectfont\\def\\encodingdefault{LGR}}\n"
	"\\DeclareRobustCommand{\\textgreek}[1]{\\leavevmode{\\greektext #1}}\n"
	"\\DeclareFontEncoding{LGR}{}{}\n"
	"\\DeclareTextSymbol{\\~}{LGR}{126}");

static docstring const textcyr_def = from_ascii(
	"\\DeclareRobustCommand{\\cyrtext}{%\n"
	"  \\fontencoding{T2A}\\selectfont\\def\\encodingdefault{T2A}}\n"
	"\\DeclareRobustCommand{\\textcyr}[1]{\\leavevmode{\\cyrtext #1}}\n"
	"\\AtBeginDocument{\\DeclareFontEncoding{T2A}{}{}}\n");

static docstring const lyxmathsym_def = from_ascii(
	"\\newcommand{\\lyxmathsym}[1]{\\ifmmode\\begingroup\\def\\b@ld{bold}\n"
	"  \\text{\\ifx\\math@version\\b@ld\\bfseries\\fi#1}\\endgroup\\else#1\\fi}\n");

static docstring const papersizedvi_def = from_ascii(
	"\\special{papersize=\\the\\paperwidth,\\the\\paperheight}\n");

static docstring const papersizepdf_def = from_ascii(
	"\\pdfpageheight\\paperheight\n"
	"\\pdfpagewidth\\paperwidth\n");

static docstring const cedilla_def = from_ascii(
	"\\newcommand{\\docedilla}[2]{\\underaccent{#1\\mathchar'30}{#2}}\n"
	"\\newcommand{\\cedilla}[1]{\\mathpalette\\docedilla{#1}}\n");

static docstring const subring_def = from_ascii(
	"\\newcommand{\\dosubring}[2]{\\underaccent{#1\\mathchar'27}{#2}}\n"
	"\\newcommand{\\subring}[1]{\\mathpalette\\dosubring{#1}}\n");

static docstring const subdot_def = from_ascii(
	"\\newcommand{\\dosubdot}[2]{\\underaccent{#1.}{#2}}\n"
	"\\newcommand{\\subdot}[1]{\\mathpalette\\dosubdot{#1}}\n");

static docstring const subhat_def = from_ascii(
	"\\newcommand{\\dosubhat}[2]{\\underaccent{#1\\mathchar'136}{#2}}\n"
	"\\newcommand{\\subhat}[1]{\\mathpalette\\dosubhat{#1}}\n");

static docstring const subtilde_def = from_ascii(
	"\\newcommand{\\dosubtilde}[2]{\\underaccent{#1\\mathchar'176}{#2}}\n"
	"\\newcommand{\\subtilde}[1]{\\mathpalette\\dosubtilde{#1}}\n");

static docstring const dacute_def = from_ascii(
	"\\DeclareMathAccent{\\dacute}{\\mathalpha}{operators}{'175}\n");

static docstring const tipasymb_def = from_ascii(
	"\\DeclareFontEncoding{T3}{}{}\n"
	"\\DeclareSymbolFont{tipasymb}{T3}{cmr}{m}{n}\n");

static docstring const dgrave_def = from_ascii(
	"\\DeclareMathAccent{\\dgrave}{\\mathord}{tipasymb}{'15}\n");

static docstring const rcap_def = from_ascii(
	"\\DeclareMathAccent{\\rcap}{\\mathord}{tipasymb}{'20}\n");

static docstring const ogonek_def = from_ascii(
	"\\newcommand{\\doogonek}[2]{\\setbox0=\\hbox{$#1#2$}\\underaccent{#1\\mkern-6mu\n"
	"  \\ifx#2O\\hskip0.5\\wd0\\else\\ifx#2U\\hskip0.5\\wd0\\else\\hskip\\wd0\\fi\\fi\n"
	"  \\ifx#2o\\mkern-2mu\\else\\ifx#2e\\mkern-1mu\\fi\\fi\n"
	"  \\mathchar\"0\\hexnumber@\\symtipasymb0C}{#2}}\n"
	"\\newcommand{\\ogonek}[1]{\\mathpalette\\doogonek{#1}}\n");

static docstring const lyxref_def = from_ascii(
		"\\RS@ifundefined{subref}\n"
		"  {\\def\\RSsubtxt{section~}\\newref{sub}{name = \\RSsubtxt}}\n" 
		"  {}\n"
		"\\RS@ifundefined{thmref}\n"
		"  {\\def\\RSthmtxt{theorem~}\\newref{thm}{name = \\RSthmtxt}}\n" 
		"  {}\n"
		"\\RS@ifundefined{lemref}\n"
		"  {\\def\\RSlemtxt{lemma~}\\newref{lem}{name = \\RSlemtxt}}\n" 
		"  {}\n");


/////////////////////////////////////////////////////////////////////
//
// LaTeXFeatures
//
/////////////////////////////////////////////////////////////////////

LaTeXFeatures::Packages LaTeXFeatures::packages_;


LaTeXFeatures::LaTeXFeatures(Buffer const & b, BufferParams const & p,
			     OutputParams const & r)
	: buffer_(&b), params_(p), runparams_(r), in_float_(false)
{}


bool LaTeXFeatures::useBabel() const
{
	if (usePolyglossia())
		return false;
	return (lyxrc.language_package_selection != LyXRC::LP_NONE)
	        || (bufferParams().language->lang() != lyxrc.default_language
	            && !bufferParams().language->babel().empty())
	        || this->hasLanguages();
}


bool LaTeXFeatures::usePolyglossia() const
{
	return (lyxrc.language_package_selection == LyXRC::LP_AUTO)
	        && isRequired("polyglossia")
	        && isAvailable("polyglossia")
	        && this->hasPolyglossiaLanguages();
}


void LaTeXFeatures::require(string const & name)
{
	features_.insert(name);
}


void LaTeXFeatures::require(set<string> const & names)
{
	features_.insert(names.begin(), names.end());
}


void LaTeXFeatures::getAvailable()
{
	Lexer lex;
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
			packages_.insert(lex.getString());
		}
	}
}


void LaTeXFeatures::useLayout(docstring const & layoutname)
{
	// Some code to avoid loops in dependency definition
	static int level = 0;
	const int maxlevel = 30;
	if (level > maxlevel) {
		lyxerr << "LaTeXFeatures::useLayout: maximum level of "
		       << "recursion attained by layout "
		       << to_utf8(layoutname) << endl;
		return;
	}

	DocumentClass const & tclass = params_.documentClass();
	if (tclass.hasLayout(layoutname)) {
		// Is this layout already in usedLayouts?
		if (find(usedLayouts_.begin(), usedLayouts_.end(), layoutname) 
		    != usedLayouts_.end())
			return;

		Layout const & layout = tclass[layoutname];
		require(layout.requires());

		if (!layout.depends_on().empty()) {
			++level;
			useLayout(layout.depends_on());
			--level;
		}
		usedLayouts_.push_back(layoutname);
	} else {
		lyxerr << "LaTeXFeatures::useLayout: layout `"
		       << to_utf8(layoutname) << "' does not exist in this class"
		       << endl;
	}

	--level;
}


void LaTeXFeatures::useInsetLayout(InsetLayout const & lay)
{
	docstring const & lname = lay.name();
	DocumentClass const & tclass = params_.documentClass();

	// this is a default inset layout, nothing useful here
	if (!tclass.hasInsetLayout(lname))
		return;
	// Is this layout already in usedInsetLayouts?
	if (find(usedInsetLayouts_.begin(), usedInsetLayouts_.end(), lname) 
			!= usedInsetLayouts_.end())
		return;

	require(lay.requires());
	usedInsetLayouts_.push_back(lname);
}


bool LaTeXFeatures::isRequired(string const & name) const
{
	return features_.find(name) != features_.end();
}


bool LaTeXFeatures::mustProvide(string const & name) const
{
	return isRequired(name) && !params_.documentClass().provides(name);
}


bool LaTeXFeatures::isAvailable(string const & name)
{
	string::size_type const i = name.find("->");
	if (i != string::npos) {
		string const from = name.substr(0,i);
		string const to = name.substr(i+2);
		//LYXERR0("from=[" << from << "] to=[" << to << "]");
		return theConverters().isReachable(from, to);
	}

	if (packages_.empty())
		getAvailable();
	string n = name;
	if (suffixIs(n, ".sty"))
		n.erase(name.length() - 4);
	return packages_.find(n) != packages_.end();
}


void LaTeXFeatures::addPreambleSnippet(string const & preamble)
{
	SnippetList::const_iterator begin = preamble_snippets_.begin();
	SnippetList::const_iterator end   = preamble_snippets_.end();
	if (find(begin, end, preamble) == end)
		preamble_snippets_.push_back(preamble);
}


void LaTeXFeatures::useFloat(string const & name, bool subfloat)
{
	if (!usedFloats_[name])
		usedFloats_[name] = subfloat;
	if (subfloat)
		require("subfig");
	// We only need float.sty if we use non builtin floats, or if we
	// use the "H" modifier. This includes modified table and
	// figure floats. (Lgb)
	Floating const & fl = params_.documentClass().floats().getType(name);
	if (!fl.floattype().empty() && fl.needsFloatPkg()) {
		require("float");
	}
}


void LaTeXFeatures::useLanguage(Language const * lang)
{
	if (!lang->babel().empty())
		UsedLanguages_.insert(lang);
	if (lang->lang() == "vietnamese")
		require("vietnamese");
	// CJK languages do not have a babel name.
	// They use the CJK package
	if (lang->encoding()->package() == Encoding::CJK)
		require("CJK");
	// japanese package is special
	if (lang->encoding()->package() == Encoding::japanese)
		require("japanese");
}


void LaTeXFeatures::includeFile(docstring const & key, string const & name)
{
	IncludedFiles_[key] = name;
}


bool LaTeXFeatures::hasLanguages() const
{
	return !UsedLanguages_.empty();
}


bool LaTeXFeatures::hasPolyglossiaLanguages() const
{
	LanguageList::const_iterator const begin = UsedLanguages_.begin();
	for (LanguageList::const_iterator cit = begin;
	     cit != UsedLanguages_.end();
	     ++cit) {
		if ((*cit)->polyglossia().empty())
			return false;
	}
	return true;
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


std::map<std::string, std::string> LaTeXFeatures::getPolyglossiaLanguages() const
{
	std::map<std::string, std::string> languages;

	LanguageList::const_iterator const begin = UsedLanguages_.begin();
	for (LanguageList::const_iterator cit = begin;
	     cit != UsedLanguages_.end();
	     ++cit) {
		languages[(*cit)->polyglossia()] = (*cit)->polyglossiaOpts();
	}
	return languages;
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
		    ((*it)->encoding()->package() == Encoding::inputenc
		     || (*it)->encoding()->package() == Encoding::japanese))
			encodings.insert((*it)->encoding()->latexName());
	return encodings;
}

namespace {

char const * simplefeatures[] = {
// note that the package order here will be the same in the LaTeX-output
	"array",
	"verbatim",
	"longtable",
	"rotating",
	"latexsym",
	"pifont",
	// subfig is handled in BufferParams.cpp
	"varioref",
	"prettyref",
	"refstyle",
	/*For a successful cooperation of the `wrapfig' package with the
	  `float' package you should load the `wrapfig' package *after*
	  the `float' package. See the caption package documentation
	  for explanation.*/
	"float",
	"rotfloat",
	"wrapfig",
	"booktabs",
	"dvipost",
	"fancybox",
	"calc",
	"units",
	"tipa",
	"tipx",
	"framed",
	"soul",
	"textcomp",
	"subscript",
	"pmboxdraw",
	"bbding",
	"ifsym",
	"marvosym",
	"txfonts",
	"mathrsfs",
	"ascii",
	"url",
	"covington",
	"csquotes",
	"enumitem",
	"endnotes",
	"ifthen",
	"amsthm",
	// listings is handled in BufferParams.cpp
	"bm",
	"pdfpages",
	"amscd",
	"slashed",
	"multirow",
	"tfrupee"
};

int const nb_simplefeatures = sizeof(simplefeatures) / sizeof(char const *);

}


string const LaTeXFeatures::getColorOptions() const
{
	ostringstream colors;

	// Handling the color packages separately is needed to be able to load them
	// before babel when hyperref is loaded with the colorlinks option
	// for more info see Bufferparams.cpp

	// [x]color.sty
	if (mustProvide("color") || mustProvide("xcolor")) {
		string const package =
			(mustProvide("xcolor") ? "xcolor" : "color");
		if (params_.graphicsDriver == "default"
			|| params_.graphicsDriver == "none")
			colors << "\\usepackage{" << package << "}\n";
		else
			colors << "\\usepackage["
				 << params_.graphicsDriver
				 << "]{" << package << "}\n";
	}

	// pdfcolmk must be loaded after color
	if (mustProvide("pdfcolmk"))
		colors << "\\usepackage{pdfcolmk}\n";

	// the following 3 color commands must be set after color
	// is loaded and before pdfpages, therefore add the command
	// here define the set color
	if (mustProvide("pagecolor")) {
		colors << "\\definecolor{page_backgroundcolor}{rgb}{";
		colors << outputLaTeXColor(params_.backgroundcolor) << "}\n";
		// set the page color
		colors << "\\pagecolor{page_backgroundcolor}\n";
	}

	if (mustProvide("fontcolor")) {
		colors << "\\definecolor{document_fontcolor}{rgb}{";
		colors << outputLaTeXColor(params_.fontcolor) << "}\n";
		// set the color
		colors << "\\color{document_fontcolor}\n";
	}

	if (mustProvide("lyxgreyedout")) {
		colors << "\\definecolor{note_fontcolor}{rgb}{";
		colors << outputLaTeXColor(params_.notefontcolor) << "}\n";
		// the color will be set together with the definition of
		// the lyxgreyedout environment (see lyxgreyedout_def)
	}

	// color for shaded boxes
	if (isRequired("framed") && mustProvide("color")) {
		colors << "\\definecolor{shadecolor}{rgb}{";
		colors << outputLaTeXColor(params_.boxbgcolor) << "}\n";
		// this color is automatically used by the LaTeX-package "framed"
	}

	return colors.str();
}


string const LaTeXFeatures::getPackages() const
{
	ostringstream packages;
	DocumentClass const & tclass = params_.documentClass();

	// FIXME: currently, we can only load packages and macros known
	// to LyX.
	// However, with the Require tag of layouts/custom insets,
	// also unknown packages can be requested. They are silently
	// swallowed now. We should change this eventually.

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

	// esint is preferred for esintoramsmath
	if ((mustProvide("amsmath")
	     && params_.use_amsmath != BufferParams::package_off)
	    || (mustProvide("esintoramsmath")
	        && params_.use_esint == BufferParams::package_off
	        && params_.use_amsmath != BufferParams::package_off)) {
		packages << "\\usepackage{amsmath}\n";
	} else {
		// amsbsy and amstext are already provided by amsmath
		if (mustProvide("amsbsy"))
			packages << "\\usepackage{amsbsy}\n";
		if (mustProvide("amstext"))
			packages << "\\usepackage{amstext}\n";
	}
	
	// wasysym is a simple feature, but it must be after amsmath if both
	// are used
	// wasysym redefines some integrals (e.g. iint) from amsmath. That
	// leads to inconsistent integrals. We only load this package if
	// the document does not contain integrals (then isRequired("esint")
	// is false) or if esint is used, since esint redefines all relevant
	// integral symbols from wasysym and amsmath.
	// See http://www.lyx.org/trac/ticket/1942
	if (mustProvide("wasysym") &&
	    (params_.use_esint != BufferParams::package_off || !isRequired("esint")))
		packages << "\\usepackage{wasysym}\n";

	// accents must be loaded after amsmath
	if (mustProvide("accents"))
		packages << "\\usepackage{accents}\n";

	// mathdots must be loaded after amsmath
	if (mustProvide("mathdots") &&
		params_.use_mathdots != BufferParams::package_off)
		packages << "\\usepackage{mathdots}\n";

	// yhmath must be loaded after amsmath
	if (mustProvide("yhmath"))
		packages << "\\usepackage{yhmath}\n";

	// [x]color and pdfcolmk are handled in getColorOptions() above
	
	// makeidx.sty
	if (isRequired("makeidx") || isRequired("splitidx")) {
		if (!tclass.provides("makeidx") && !isRequired("splitidx"))
			packages << "\\usepackage{makeidx}\n";
		if (!tclass.provides("splitidx") && isRequired("splitidx"))
			packages << "\\usepackage{splitidx}\n";
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
	
	// lyxskak.sty --- newer chess support based on skak.sty
	if (mustProvide("chess"))
		packages << "\\usepackage[ps,mover]{lyxskak}\n";

	// setspace.sty
	if (mustProvide("setspace") && !tclass.provides("SetSpace"))
		packages << "\\usepackage{setspace}\n";

	// amssymb.sty
	if (mustProvide("amssymb")
	    || params_.use_amsmath == BufferParams::package_on)
		packages << "\\usepackage{amssymb}\n";

	// esint must be after amsmath and wasysym, since it will redeclare
	// inconsistent integral symbols
	if ((mustProvide("esint") || mustProvide("esintoramsmath")) &&
	    params_.use_esint != BufferParams::package_off)
		packages << "\\usepackage{esint}\n";

	// natbib.sty
	// Some classes load natbib themselves, but still allow (or even require)
	// plain numeric citations (ReVTeX is such a case, see bug 5182).
	// This special case is indicated by the "natbib-internal" key.
	if (mustProvide("natbib") && !tclass.provides("natbib-internal")) {
		packages << "\\usepackage[";
		if (params_.citeEngine() == ENGINE_NATBIB_NUMERICAL)
			packages << "numbers";
		else
			packages << "authoryear";
		packages << "]{natbib}\n";
	}

	// jurabib -- we need version 0.6 at least.
	if (mustProvide("jurabib"))
		packages << "\\usepackage{jurabib}[2004/01/25]\n";
	
	// xargs -- we need version 1.09 at least
	if (mustProvide("xargs"))
		packages << "\\usepackage{xargs}[2008/03/08]\n";

	// bibtopic -- the dot provides the aux file naming which
	// LyX can detect.
	if (mustProvide("bibtopic"))
		packages << "\\usepackage[dot]{bibtopic}\n";

	if (mustProvide("xy"))
		packages << "\\usepackage[all]{xy}\n";

	if (mustProvide("feyn"))
		packages << "\\usepackage{feyn}\n"; //Diagram

	if (mustProvide("ulem"))
		packages << "\\PassOptionsToPackage{normalem}{ulem}\n"
			    "\\usepackage{ulem}\n";

	if (params_.use_mhchem == BufferParams::package_on ||
	    (mustProvide("mhchem") &&
	     params_.use_mhchem != BufferParams::package_off))
		packages << "\\PassOptionsToPackage{version=3}{mhchem}\n"
			    "\\usepackage{mhchem}\n";

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

	return packages.str();
}


string LaTeXFeatures::getPreambleSnippets() const 
{
	ostringstream snip;
	SnippetList::const_iterator pit  = preamble_snippets_.begin();
	SnippetList::const_iterator pend = preamble_snippets_.end();
	for (; pit != pend; ++pit)
		snip << *pit << '\n';
	return snip.str();
}


docstring const LaTeXFeatures::getMacros() const
{
	odocstringstream macros;

	if (!preamble_snippets_.empty()) {
		macros << '\n';
		macros << from_utf8(getPreambleSnippets());
	}

	if (mustProvide("papersize")) {
		if (runparams_.flavor == OutputParams::LATEX)
			macros << papersizedvi_def << '\n';
		else
			macros << papersizepdf_def << '\n';
	}

	if (mustProvide("LyX"))
		macros << lyx_def << '\n';

	if (mustProvide("noun"))
		macros << noun_def << '\n';

	if (mustProvide("lyxarrow"))
		macros << lyxarrow_def << '\n';

	if (!usePolyglossia() && mustProvide("textgreek")) {
		// Avoid a LaTeX error if times fonts are used and the grtimes
		// package is installed but actual fonts are not (bug 6469).
		if (params_.fonts_roman == "times")
			macros << subst(textgreek_def,
					from_ascii("\\greektext #1"),
					from_ascii("%\n  \\IfFileExists"
						   "{grtm10.tfm}{}{\\fontfamily"
						   "{cmr}}\\greektext #1"))
			       << '\n';
		else
			macros << textgreek_def << '\n';
	}

	if (!usePolyglossia() && mustProvide("textcyr"))
		macros << textcyr_def << '\n';

	if (mustProvide("lyxmathsym"))
		macros << lyxmathsym_def << '\n';

	if (mustProvide("cedilla"))
		macros << cedilla_def << '\n';

	if (mustProvide("subring"))
		macros << subring_def << '\n';

	if (mustProvide("subdot"))
		macros << subdot_def << '\n';

	if (mustProvide("subhat"))
		macros << subhat_def << '\n';

	if (mustProvide("subtilde"))
		macros << subtilde_def << '\n';

	if (mustProvide("dacute"))
		macros << dacute_def << '\n';

	if (mustProvide("tipasymb"))
		macros << tipasymb_def << '\n';

	if (mustProvide("dgrave"))
		macros << dgrave_def << '\n';

	if (mustProvide("rcap"))
		macros << rcap_def << '\n';

	if (mustProvide("ogonek"))
		macros << ogonek_def << '\n';

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

	// greyed-out environment (note inset)
	// the color is specified in the routine
	// getColorOptions() to avoid LaTeX-package clashes
	if (mustProvide("lyxgreyedout"))
		macros << lyxgreyedout_def;

	if (mustProvide("lyxdot"))
		macros << lyxdot_def << '\n';

	// floats
	getFloatDefinitions(macros);
	
	if (mustProvide("refstyle")) 
		macros << lyxref_def << '\n';	
	
	// change tracking
	if (mustProvide("ct-dvipost"))
		macros << changetracking_dvipost_def;
	
	if (mustProvide("ct-xcolor-ulem")) {
		streamsize const prec = macros.precision(2);
	
		RGBColor cadd = rgbFromHexName(lcolor.getX11Name(Color_addedtext));
		macros << "\\providecolor{lyxadded}{rgb}{"
		       << cadd.r / 255.0 << ',' << cadd.g / 255.0 << ',' << cadd.b / 255.0 << "}\n";

		RGBColor cdel = rgbFromHexName(lcolor.getX11Name(Color_deletedtext));
		macros << "\\providecolor{lyxdeleted}{rgb}{"
		       << cdel.r / 255.0 << ',' << cdel.g / 255.0 << ',' << cdel.b / 255.0 << "}\n";

		macros.precision(prec);
		
		if (isRequired("hyperref"))
			macros << changetracking_xcolor_ulem_hyperref_def;
		else
			macros << changetracking_xcolor_ulem_def;
	}

	if (mustProvide("ct-none"))
		macros << changetracking_none_def;

	return macros.str();
}


string const LaTeXFeatures::getBabelPresettings() const
{
	ostringstream tmp;

	LanguageList::const_iterator it  = UsedLanguages_.begin();
	LanguageList::const_iterator end = UsedLanguages_.end();
	for (; it != end; ++it)
		if (!(*it)->babel_presettings().empty())
			tmp << (*it)->babel_presettings() << '\n';
	if (!params_.language->babel_presettings().empty())
		tmp << params_.language->babel_presettings() << '\n';

	return tmp.str();
}


string const LaTeXFeatures::getBabelPostsettings() const
{
	ostringstream tmp;

	LanguageList::const_iterator it  = UsedLanguages_.begin();
	LanguageList::const_iterator end = UsedLanguages_.end();
	for (; it != end; ++it)
		if (!(*it)->babel_postsettings().empty())
			tmp << (*it)->babel_postsettings() << '\n';
	if (!params_.language->babel_postsettings().empty())
		tmp << params_.language->babel_postsettings() << '\n';

	return tmp.str();
}


bool LaTeXFeatures::needBabelLangOptions() const
{
	if (!lyxrc.language_global_options || params_.language->asBabelOptions())
		return true;

	LanguageList::const_iterator it  = UsedLanguages_.begin();
	LanguageList::const_iterator end = UsedLanguages_.end();
	for (; it != end; ++it)
		if ((*it)->asBabelOptions())
			return true;

	return false;
}


docstring const LaTeXFeatures::getTClassPreamble() const
{
	// the text class specific preamble
	DocumentClass const & tclass = params_.documentClass();
	odocstringstream tcpreamble;

	tcpreamble << tclass.preamble();

	list<docstring>::const_iterator cit = usedLayouts_.begin();
	list<docstring>::const_iterator end = usedLayouts_.end();
	for (; cit != end; ++cit)
		tcpreamble << tclass[*cit].preamble();

	cit = usedInsetLayouts_.begin();
	end = usedInsetLayouts_.end();
	TextClass::InsetLayouts const & ils = tclass.insetLayouts();
	for (; cit != end; ++cit) {
		TextClass::InsetLayouts::const_iterator it = ils.find(*cit);
		if (it == ils.end())
			continue;
		tcpreamble << it->second.preamble();
	}

	return tcpreamble.str();
}


docstring const LaTeXFeatures::getTClassHTMLPreamble() const 
{
	DocumentClass const & tclass = params_.documentClass();
	odocstringstream tcpreamble;

	tcpreamble << tclass.htmlpreamble();

	list<docstring>::const_iterator cit = usedLayouts_.begin();
	list<docstring>::const_iterator end = usedLayouts_.end();
	for (; cit != end; ++cit)
		tcpreamble << tclass[*cit].htmlpreamble();

	cit = usedInsetLayouts_.begin();
	end = usedInsetLayouts_.end();
	TextClass::InsetLayouts const & ils = tclass.insetLayouts();
	for (; cit != end; ++cit) {
		TextClass::InsetLayouts::const_iterator it = ils.find(*cit);
		if (it == ils.end())
			continue;
		tcpreamble << it->second.htmlpreamble();
	}

	return tcpreamble.str();
}


docstring const LaTeXFeatures::getTClassHTMLStyles() const {
	DocumentClass const & tclass = params_.documentClass();
	odocstringstream tcpreamble;

	list<docstring>::const_iterator cit = usedLayouts_.begin();
	list<docstring>::const_iterator end = usedLayouts_.end();
	for (; cit != end; ++cit)
		tcpreamble << tclass[*cit].htmlstyle();

	cit = usedInsetLayouts_.begin();
	end = usedInsetLayouts_.end();
	TextClass::InsetLayouts const & ils = tclass.insetLayouts();
	for (; cit != end; ++cit) {
		TextClass::InsetLayouts::const_iterator it = ils.find(*cit);
		if (it == ils.end())
			continue;
		tcpreamble << it->second.htmlstyle();
	}

	return tcpreamble.str();
}


namespace {
docstring const getFloatI18nPreamble(docstring const & type, docstring const & name, docstring const & lang)
{
	odocstringstream os;
	os << "\\addto\\captions" << lang
	   << "{\\renewcommand{\\" << type << "name}{" << name << "}}\n";
	return os.str();
}
}


docstring const LaTeXFeatures::getTClassI18nPreamble(bool use_babel) const
{
	DocumentClass const & tclass = params_.documentClass();
	// collect preamble snippets in a set to prevent multiple identical
	// commands (would happen if e.g. both theorem and theorem* are used)
	set<docstring> snippets;
	typedef LanguageList::const_iterator lang_it;
	lang_it const lbeg = UsedLanguages_.begin();
	lang_it const lend =  UsedLanguages_.end();
	list<docstring>::const_iterator cit = usedLayouts_.begin();
	list<docstring>::const_iterator end = usedLayouts_.end();
	for (; cit != end; ++cit) {
		// language dependent commands (once per document)
		snippets.insert(tclass[*cit].langpreamble(buffer().language()));
		// commands for language changing (for multilanguage documents)
		if (use_babel && !UsedLanguages_.empty()) {
			snippets.insert(tclass[*cit].babelpreamble(buffer().language()));
			for (lang_it lit = lbeg; lit != lend; ++lit)
				snippets.insert(tclass[*cit].babelpreamble(*lit));
		}
	}
	if (use_babel && !UsedLanguages_.empty()) {
		FloatList const & floats = params_.documentClass().floats();
		UsedFloats::const_iterator fit = usedFloats_.begin();
		UsedFloats::const_iterator fend = usedFloats_.end();
		for (; fit != fend; ++fit) {
			Floating const & fl = floats.getType(fit->first);
			docstring const type = from_ascii(fl.floattype());
			docstring const flname = from_utf8(fl.name());
			docstring name = translateIfPossible(flname,
				buffer().language()->code());
			snippets.insert(getFloatI18nPreamble(
				type, name,
				from_ascii(buffer().language()->babel())));
			for (lang_it lit = lbeg; lit != lend; ++lit) {
				name = translateIfPossible(flname,
					(*lit)->code());
				snippets.insert(getFloatI18nPreamble(
					type, name,
					from_ascii((*lit)->babel())));
			}
		}
	}

	odocstringstream tcpreamble;
	set<docstring>::const_iterator const send = snippets.end();
	set<docstring>::const_iterator it = snippets.begin();
	for (; it != send; ++it)
		tcpreamble << *it;
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
			     << (isSGMLFileName(fi->second) ? " SYSTEM \"" : " \"")
			     << makeRelPath(from_utf8(fi->second), basename) << "\">";

	return sgmlpreamble.str();
}


void LaTeXFeatures::showStruct() const
{
	lyxerr << "LyX needs the following commands when LaTeXing:"
	       << "\n***** Packages:" << getPackages()
	       << "\n***** Macros:" << to_utf8(getMacros())
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


void LaTeXFeatures::getFloatDefinitions(odocstream & os) const
{
	FloatList const & floats = params_.documentClass().floats();

	// Here we will output the code to create the needed float styles.
	// We will try to do this as minimal as possible.
	// \floatstyle{ruled}
	// \newfloat{algorithm}{htbp}{loa}
	// \providecommand{\algorithmname}{Algorithm}
	// \floatname{algorithm}{\protect\algorithmname}
	UsedFloats::const_iterator cit = usedFloats_.begin();
	UsedFloats::const_iterator end = usedFloats_.end();
	for (; cit != end; ++cit) {
		Floating const & fl = floats.getType(cit->first);

		// For builtin floats we do nothing.
		if (!fl.needsFloatPkg()) 
			continue;

		// We have to special case "table" and "figure"
		if (fl.floattype() == "tabular" || fl.floattype() == "figure") {
			// Output code to modify "table" or "figure"
			// but only if builtin == false
			// and that have to be true at this point in the
			// function.
			docstring const type = from_ascii(fl.floattype());
			docstring const placement = from_ascii(fl.placement());
			docstring const style = from_ascii(fl.style());
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

			docstring const type = from_ascii(fl.floattype());
			docstring const placement = from_ascii(fl.placement());
			docstring const ext = from_ascii(fl.ext());
			docstring const within = from_ascii(fl.within());
			docstring const style = from_ascii(fl.style());
			docstring const name = translateIfPossible(
					from_utf8(fl.name()),
					buffer().language()->code());
			os << "\\floatstyle{" << style << "}\n"
			   << "\\newfloat{" << type << "}{" << placement
			   << "}{" << ext << '}';
			if (!within.empty())
				os << '[' << within << ']';
			os << '\n'
			   << "\\providecommand{\\" << type << "name}{"
			   << name << "}\n"
			   << "\\floatname{" << type << "}{\\protect\\"
			   << type << "name}\n";

			// What missing here is to code to minimalize the code
			// output so that the same floatstyle will not be
			// used several times, when the same style is still in
			// effect. (Lgb)
		}
		if (cit->second)
			os << "\n\\newsubfloat{" << from_ascii(fl.floattype()) << "}\n";
	}
}


} // namespace lyx
