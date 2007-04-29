/**
 * \file BufferParams.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 * \author Andr� P�nitz
 * \author Martin Vermeer
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "BufferParams.h"

#include "Author.h"
#include "BranchList.h"
#include "Bullet.h"
#include "debug.h"
#include "Encoding.h"
#include "gettext.h"
#include "Language.h"
#include "LaTeXFeatures.h"
#include "Color.h"
#include "Font.h"
#include "Lexer.h"
#include "LyXRC.h"
#include "LyXTextClassList.h"
#include "OutputParams.h"
#include "tex-strings.h"
#include "Spacing.h"
#include "TexRow.h"
#include "VSpace.h"

#include "frontends/alert.h"

#include "support/lyxalgo.h" // for lyx::count
#include "support/convert.h"
#include "support/Translator.h"

#include <boost/array.hpp>

#include <sstream>


namespace lyx {

using support::bformat;
using support::rtrim;
using support::tokenPos;

using std::endl;
using std::string;
using std::istringstream;
using std::ostream;
using std::ostringstream;
using std::pair;

namespace Alert = lyx::frontend::Alert;


// Local translators
namespace {

// Paragraph separation
typedef Translator<string, BufferParams::PARSEP> ParSepTranslator;


ParSepTranslator const init_parseptranslator()
{
	ParSepTranslator translator(string_paragraph_separation[0], BufferParams::PARSEP_INDENT);
	translator.addPair(string_paragraph_separation[1], BufferParams::PARSEP_SKIP);
	return translator;
}


ParSepTranslator const & parseptranslator()
{
	static ParSepTranslator translator = init_parseptranslator();
	return translator;
}


// Quotes language
typedef Translator<string, InsetQuotes::quote_language> QuotesLangTranslator;


QuotesLangTranslator const init_quoteslangtranslator()
{
	QuotesLangTranslator translator(string_quotes_language[0], InsetQuotes::EnglishQ);
	translator.addPair(string_quotes_language[1], InsetQuotes::SwedishQ);
	translator.addPair(string_quotes_language[2], InsetQuotes::GermanQ);
	translator.addPair(string_quotes_language[3], InsetQuotes::PolishQ);
	translator.addPair(string_quotes_language[4], InsetQuotes::FrenchQ);
	translator.addPair(string_quotes_language[5], InsetQuotes::DanishQ);
	return translator;
}


QuotesLangTranslator const & quoteslangtranslator()
{
	static QuotesLangTranslator translator = init_quoteslangtranslator();
	return translator;
}


// Paper size
typedef Translator<std::string, PAPER_SIZE> PaperSizeTranslator;


PaperSizeTranslator const init_papersizetranslator()
{
	PaperSizeTranslator translator(string_papersize[0], PAPER_DEFAULT);
	translator.addPair(string_papersize[1], PAPER_CUSTOM);
	translator.addPair(string_papersize[2], PAPER_USLETTER);
	translator.addPair(string_papersize[3], PAPER_USLEGAL);
	translator.addPair(string_papersize[4], PAPER_USEXECUTIVE);
	translator.addPair(string_papersize[5], PAPER_A3);
	translator.addPair(string_papersize[6], PAPER_A4);
	translator.addPair(string_papersize[7], PAPER_A5);
	translator.addPair(string_papersize[8], PAPER_B3);
	translator.addPair(string_papersize[9], PAPER_B4);
	translator.addPair(string_papersize[10], PAPER_B5);
	return translator;
}


PaperSizeTranslator const & papersizetranslator()
{
	static PaperSizeTranslator translator = init_papersizetranslator();
	return translator;
}


// Paper orientation
typedef Translator<string, PAPER_ORIENTATION> PaperOrientationTranslator;


PaperOrientationTranslator const init_paperorientationtranslator()
{
	PaperOrientationTranslator translator(string_orientation[0], ORIENTATION_PORTRAIT);
	translator.addPair(string_orientation[1], ORIENTATION_LANDSCAPE);
	return translator;
}


PaperOrientationTranslator const & paperorientationtranslator()
{
	static PaperOrientationTranslator translator = init_paperorientationtranslator();
	return translator;
}


// Page sides
typedef Translator<int, LyXTextClass::PageSides> SidesTranslator;


SidesTranslator const init_sidestranslator()
{
	SidesTranslator translator(1, LyXTextClass::OneSide);
	translator.addPair(2, LyXTextClass::TwoSides);
	return translator;
}


SidesTranslator const & sidestranslator()
{
	static SidesTranslator translator = init_sidestranslator();
	return translator;
}


// LaTeX packages
typedef Translator<int, BufferParams::Package> PackageTranslator;


PackageTranslator const init_packagetranslator()
{
	PackageTranslator translator(0, BufferParams::package_off);
	translator.addPair(1, BufferParams::package_auto);
	translator.addPair(2, BufferParams::package_on);
	return translator;
}


PackageTranslator const & packagetranslator()
{
	static PackageTranslator translator = init_packagetranslator();
	return translator;
}


// Cite engine
typedef Translator<string, biblio::CiteEngine> CiteEngineTranslator;


CiteEngineTranslator const init_citeenginetranslator()
{
	CiteEngineTranslator translator("basic", biblio::ENGINE_BASIC);
	translator.addPair("natbib_numerical", biblio::ENGINE_NATBIB_NUMERICAL);
	translator.addPair("natbib_authoryear", biblio::ENGINE_NATBIB_AUTHORYEAR);
	translator.addPair("jurabib", biblio::ENGINE_JURABIB);
	return translator;
}


CiteEngineTranslator const & citeenginetranslator()
{
	static CiteEngineTranslator translator = init_citeenginetranslator();
	return translator;
}


// Spacing
typedef Translator<string, Spacing::Space> SpaceTranslator;


SpaceTranslator const init_spacetranslator()
{
	SpaceTranslator translator("default", Spacing::Default);
	translator.addPair("single", Spacing::Single);
	translator.addPair("onehalf", Spacing::Onehalf);
	translator.addPair("double", Spacing::Double);
	translator.addPair("other", Spacing::Other);
	return translator;
}


SpaceTranslator const & spacetranslator()
{
	static SpaceTranslator translator = init_spacetranslator();
	return translator;
}


textclass_type defaultTextclass()
{
	// Initialize textclass to point to article. if `first' is
	// true in the returned pair, then `second' is the textclass
	// number; if it is false, second is 0. In both cases, second
	// is what we want.
	return textclasslist.numberOfClass("article").second;
}

} // anon namespace


class BufferParams::Impl
{
public:
	Impl();

	AuthorList authorlist;
	BranchList branchlist;
	boost::array<Bullet, 4> temp_bullets;
	boost::array<Bullet, 4> user_defined_bullets;
	Spacing spacing;
	/** This is the amount of space used for paragraph_separation "skip",
	 * and for detached paragraphs in "indented" documents.
	 */
	VSpace defskip;
};


BufferParams::Impl::Impl()
	: defskip(VSpace::MEDSKIP)
{
	// set initial author
	// FIXME UNICODE
	authorlist.record(Author(from_utf8(lyxrc.user_name), from_utf8(lyxrc.user_email)));
}


BufferParams::Impl *
BufferParams::MemoryTraits::clone(BufferParams::Impl const * ptr)
{
	BOOST_ASSERT(ptr);

	return new BufferParams::Impl(*ptr);
}


void BufferParams::MemoryTraits::destroy(BufferParams::Impl * ptr)
{
	delete ptr;
}


BufferParams::BufferParams()
	: textclass(defaultTextclass()), pimpl_(new Impl)
{
	paragraph_separation = PARSEP_INDENT;
	quotes_language = InsetQuotes::EnglishQ;
	fontsize = "default";

	/*  PaperLayout */
	papersize = PAPER_DEFAULT;
	orientation = ORIENTATION_PORTRAIT;
	use_geometry = false;
	use_amsmath = package_auto;
	use_esint = package_auto;
	cite_engine_ = biblio::ENGINE_BASIC;
	use_bibtopic = false;
	trackChanges = false;
	outputChanges = false;
	secnumdepth = 3;
	tocdepth = 3;
	language = default_language;
	fontsRoman = "default";
	fontsSans = "default";
	fontsTypewriter = "default";
	fontsDefaultFamily = "default";
	fontsSC = false;
	fontsOSF = false;
	fontsSansScale = 100;
	fontsTypewriterScale = 100;
	inputenc = "auto";
	graphicsDriver = "default";
	sides = LyXTextClass::OneSide;
	columns = 1;
	pagestyle = "default";
	compressed = false;
	for (int iter = 0; iter < 4; ++iter) {
		user_defined_bullet(iter) = ITEMIZE_DEFAULTS[iter];
		temp_bullet(iter) = ITEMIZE_DEFAULTS[iter];
	}
}


BufferParams::~BufferParams()
{}


AuthorList & BufferParams::authors()
{
	return pimpl_->authorlist;
}


AuthorList const & BufferParams::authors() const
{
	return pimpl_->authorlist;
}


BranchList & BufferParams::branchlist()
{
	return pimpl_->branchlist;
}


BranchList const & BufferParams::branchlist() const
{
	return pimpl_->branchlist;
}


Bullet & BufferParams::temp_bullet(lyx::size_type const index)
{
	BOOST_ASSERT(index < 4);
	return pimpl_->temp_bullets[index];
}


Bullet const & BufferParams::temp_bullet(lyx::size_type const index) const
{
	BOOST_ASSERT(index < 4);
	return pimpl_->temp_bullets[index];
}


Bullet & BufferParams::user_defined_bullet(lyx::size_type const index)
{
	BOOST_ASSERT(index < 4);
	return pimpl_->user_defined_bullets[index];
}


Bullet const & BufferParams::user_defined_bullet(lyx::size_type const index) const
{
	BOOST_ASSERT(index < 4);
	return pimpl_->user_defined_bullets[index];
}


Spacing & BufferParams::spacing()
{
	return pimpl_->spacing;
}


Spacing const & BufferParams::spacing() const
{
	return pimpl_->spacing;
}


VSpace const & BufferParams::getDefSkip() const
{
	return pimpl_->defskip;
}


void BufferParams::setDefSkip(VSpace const & vs)
{
	pimpl_->defskip = vs;
}


string const BufferParams::readToken(Lexer & lex, string const & token)
{
	if (token == "\\textclass") {
		lex.next();
		string const classname = lex.getString();
		pair<bool, lyx::textclass_type> pp =
			textclasslist.numberOfClass(classname);
		if (pp.first) {
			textclass = pp.second;
		} else {
			// if text class does not exist, try to load it from filepath
			pp = textclasslist.addTextClass(classname, filepath);
			if (pp.first) {
				textclass = pp.second;
			} else {
				textclass = defaultTextclass();
				return classname;
			}
		}
		// FIXME: isTeXClassAvailable will try to load the layout file, but will
		// fail because of the lack of path info. Warnings will be given although
		// the layout file will be correctly loaded later.
		if (!getLyXTextClass().isTeXClassAvailable()) {
			docstring const msg =
				bformat(_("The layout file requested by this document,\n"
					         "%1$s.layout,\n"
					         "is not usable. This is probably because a LaTeX\n"
					         "class or style file required by it is not\n"
					         "available. See the Customization documentation\n"
					         "for more information.\n"), from_utf8(classname));
			Alert::warning(_("Document class not available"),
				       msg + _("LyX will not be able to produce output."));
		}
	} else if (token == "\\begin_preamble") {
		readPreamble(lex);
	} else if (token == "\\options") {
		lex.eatLine();
		options = lex.getString();
	} else if (token == "\\language") {
		readLanguage(lex);
	} else if (token == "\\inputencoding") {
		lex >> inputenc;
	} else if (token == "\\graphics") {
		readGraphicsDriver(lex);
	} else if (token == "\\font_roman") {
		lex >> fontsRoman;
	} else if (token == "\\font_sans") {
		lex >> fontsSans;
	} else if (token == "\\font_typewriter") {
		lex >> fontsTypewriter;
	} else if (token == "\\font_default_family") {
		lex >> fontsDefaultFamily;
	} else if (token == "\\font_sc") {
		lex >> fontsSC;
	} else if (token == "\\font_osf") {
		lex >> fontsOSF;
	} else if (token == "\\font_sf_scale") {
		lex >> fontsSansScale;
	} else if (token == "\\font_tt_scale") {
		lex >> fontsTypewriterScale;
	} else if (token == "\\paragraph_separation") {
		string parsep;
		lex >> parsep;
		paragraph_separation = parseptranslator().find(parsep);
	} else if (token == "\\defskip") {
		lex.next();
		pimpl_->defskip = VSpace(lex.getString());
	} else if (token == "\\quotes_language") {
		string quotes_lang;
		lex >> quotes_lang;
		quotes_language = quoteslangtranslator().find(quotes_lang);
	} else if (token == "\\papersize") {
		string ppsize;
		lex >> ppsize;
		papersize = papersizetranslator().find(ppsize);
	} else if (token == "\\use_geometry") {
		lex >> use_geometry;
	} else if (token == "\\use_amsmath") {
		int use_ams;
		lex >> use_ams;
		use_amsmath = packagetranslator().find(use_ams);
	} else if (token == "\\use_esint") {
		int useesint;
		lex >> useesint;
		use_esint = packagetranslator().find(useesint);
	} else if (token == "\\cite_engine") {
		string engine;
		lex >> engine;
		cite_engine_ = citeenginetranslator().find(engine);
	} else if (token == "\\use_bibtopic") {
		lex >> use_bibtopic;
	} else if (token == "\\tracking_changes") {
		lex >> trackChanges;
	} else if (token == "\\output_changes") {
		lex >> outputChanges;
	} else if (token == "\\branch") {
		lex.next();
		docstring branch = lex.getDocString();
		branchlist().add(branch);
		while (true) {
			lex.next();
			string const tok = lex.getString();
			if (tok == "\\end_branch")
				break;
			Branch * branch_ptr = branchlist().find(branch);
			if (tok == "\\selected") {
				lex.next();
				if (branch_ptr)
					branch_ptr->setSelected(lex.getInteger());
			}
			// not yet operational
			if (tok == "\\color") {
				lex.eatLine();
				string color = lex.getString();
				if (branch_ptr)
					branch_ptr->setColor(color);
				// Update also the Color table:
				if (color == "none")
					color = lcolor.getX11Name(Color::background);
				// FIXME UNICODE
				lcolor.setColor(to_utf8(branch), color);

			}
		}
	} else if (token == "\\author") {
		lex.eatLine();
		istringstream ss(lex.getString());
		Author a;
		ss >> a;
		author_map.push_back(pimpl_->authorlist.record(a));
	} else if (token == "\\paperorientation") {
		string orient;
		lex >> orient;
		orientation = paperorientationtranslator().find(orient);
	} else if (token == "\\paperwidth") {
		lex >> paperwidth;
	} else if (token == "\\paperheight") {
		lex >> paperheight;
	} else if (token == "\\leftmargin") {
		lex >> leftmargin;
	} else if (token == "\\topmargin") {
		lex >> topmargin;
	} else if (token == "\\rightmargin") {
		lex >> rightmargin;
	} else if (token == "\\bottommargin") {
		lex >> bottommargin;
	} else if (token == "\\headheight") {
		lex >> headheight;
	} else if (token == "\\headsep") {
		lex >> headsep;
	} else if (token == "\\footskip") {
		lex >> footskip;
	} else if (token == "\\paperfontsize") {
		lex >> fontsize;
	} else if (token == "\\papercolumns") {
		lex >> columns;
	} else if (token == "\\papersides") {
		int psides;
		lex >> psides;
		sides = sidestranslator().find(psides);
	} else if (token == "\\paperpagestyle") {
		lex >> pagestyle;
	} else if (token == "\\bullet") {
		readBullets(lex);
	} else if (token == "\\bulletLaTeX") {
		readBulletsLaTeX(lex);
	} else if (token == "\\secnumdepth") {
		lex >> secnumdepth;
	} else if (token == "\\tocdepth") {
		lex >> tocdepth;
	} else if (token == "\\spacing") {
		string nspacing;
		lex >> nspacing;
		string tmp_val;
		if (nspacing == "other") {
			lex >> tmp_val;
		}
		spacing().set(spacetranslator().find(nspacing), tmp_val);
	} else if (token == "\\float_placement") {
		lex >> float_placement;
	} else {
		return token;
	}

	return string();
}


void BufferParams::writeFile(ostream & os) const
{
	// The top of the file is written by the buffer.
	// Prints out the buffer info into the .lyx file given by file

	// the textclass
	os << "\\textclass " << textclasslist[textclass].name() << '\n';

	// then the preamble
	if (!preamble.empty()) {
		// remove '\n' from the end of preamble
		string const tmppreamble = rtrim(preamble, "\n");
		os << "\\begin_preamble\n"
		   << tmppreamble
		   << "\n\\end_preamble\n";
	}

	// the options
	if (!options.empty()) {
		os << "\\options " << options << '\n';
	}

	// then the text parameters
	if (language != ignore_language)
		os << "\\language " << language->lang() << '\n';
	os << "\\inputencoding " << inputenc
	   << "\n\\font_roman " << fontsRoman
	   << "\n\\font_sans " << fontsSans
	   << "\n\\font_typewriter " << fontsTypewriter
	   << "\n\\font_default_family " << fontsDefaultFamily
	   << "\n\\font_sc " << convert<string>(fontsSC)
	   << "\n\\font_osf " << convert<string>(fontsOSF)
	   << "\n\\font_sf_scale " << fontsSansScale
	   << "\n\\font_tt_scale " << fontsTypewriterScale
	   << "\n\\graphics " << graphicsDriver << '\n';

	if (!float_placement.empty()) {
		os << "\\float_placement " << float_placement << '\n';
	}
	os << "\\paperfontsize " << fontsize << '\n';

	spacing().writeFile(os);

	os << "\\papersize " << string_papersize[papersize]
	   << "\n\\use_geometry " << convert<string>(use_geometry)
	   << "\n\\use_amsmath " << use_amsmath
	   << "\n\\use_esint " << use_esint
	   << "\n\\cite_engine " << citeenginetranslator().find(cite_engine_)
	   << "\n\\use_bibtopic " << convert<string>(use_bibtopic)
	   << "\n\\paperorientation " << string_orientation[orientation]
	   << '\n';

	BranchList::const_iterator it = branchlist().begin();
	BranchList::const_iterator end = branchlist().end();
	for (; it != end; ++it) {
		os << "\\branch " << to_utf8(it->getBranch())
		   << "\n\\selected " << it->getSelected()
		   << "\n\\color " << lyx::X11hexname(it->getColor())
		   << "\n\\end_branch"
		   << "\n";
	}

	if (!paperwidth.empty())
		os << "\\paperwidth "
		   << VSpace(paperwidth).asLyXCommand() << '\n';
	if (!paperheight.empty())
		os << "\\paperheight "
		   << VSpace(paperheight).asLyXCommand() << '\n';
	if (!leftmargin.empty())
		os << "\\leftmargin "
		   << VSpace(leftmargin).asLyXCommand() << '\n';
	if (!topmargin.empty())
		os << "\\topmargin "
		   << VSpace(topmargin).asLyXCommand() << '\n';
	if (!rightmargin.empty())
		os << "\\rightmargin "
		   << VSpace(rightmargin).asLyXCommand() << '\n';
	if (!bottommargin.empty())
		os << "\\bottommargin "
		   << VSpace(bottommargin).asLyXCommand() << '\n';
	if (!headheight.empty())
		os << "\\headheight "
		   << VSpace(headheight).asLyXCommand() << '\n';
	if (!headsep.empty())
		os << "\\headsep "
		   << VSpace(headsep).asLyXCommand() << '\n';
	if (!footskip.empty())
		os << "\\footskip "
		   << VSpace(footskip).asLyXCommand() << '\n';
	os << "\\secnumdepth " << secnumdepth
	   << "\n\\tocdepth " << tocdepth
	   << "\n\\paragraph_separation "
	   << string_paragraph_separation[paragraph_separation]
	   << "\n\\defskip " << getDefSkip().asLyXCommand()
	   << "\n\\quotes_language "
	   << string_quotes_language[quotes_language]
	   << "\n\\papercolumns " << columns
	   << "\n\\papersides " << sides
	   << "\n\\paperpagestyle " << pagestyle << '\n';
	for (int i = 0; i < 4; ++i) {
		if (user_defined_bullet(i) != ITEMIZE_DEFAULTS[i]) {
			if (user_defined_bullet(i).getFont() != -1) {
				os << "\\bullet " << i << " "
				   << user_defined_bullet(i).getFont() << " "
				   << user_defined_bullet(i).getCharacter() << " "
				   << user_defined_bullet(i).getSize() << "\n";
			}
			else {
				// FIXME UNICODE
				os << "\\bulletLaTeX " << i << " \""
				   << lyx::to_ascii(user_defined_bullet(i).getText())
				   << "\"\n";
			}
		}
	}

	os << "\\tracking_changes " << convert<string>(trackChanges) << "\n";
	os << "\\output_changes " << convert<string>(outputChanges) << "\n";

	AuthorList::Authors::const_iterator a_it = pimpl_->authorlist.begin();
	AuthorList::Authors::const_iterator a_end = pimpl_->authorlist.end();
	for (; a_it != a_end; ++a_it) {
		os << "\\author " << a_it->second << "\n";
	}
}


bool BufferParams::writeLaTeX(odocstream & os, LaTeXFeatures & features,
			      TexRow & texrow) const
{
	os << "\\documentclass";

	LyXTextClass const & tclass = getLyXTextClass();

	ostringstream clsoptions; // the document class options.

	if (tokenPos(tclass.opt_fontsize(),
		     '|', fontsize) >= 0) {
		// only write if existing in list (and not default)
		clsoptions << fontsize << "pt,";
	}

	// custom, A3, B3 and B4 paper sizes need geometry
	bool nonstandard_papersize = papersize == PAPER_B3 
		|| papersize == PAPER_B4
		|| papersize == PAPER_A3
		|| papersize == PAPER_CUSTOM;

	if (!use_geometry) {
		switch (papersize) {
		case PAPER_A4:
			clsoptions << "a4paper,";
			break;
		case PAPER_USLETTER:
			clsoptions << "letterpaper,";
			break;
		case PAPER_A5:
			clsoptions << "a5paper,";
			break;
		case PAPER_B5:
			clsoptions << "b5paper,";
			break;
		case PAPER_USEXECUTIVE:
			clsoptions << "executivepaper,";
			break;
		case PAPER_USLEGAL:
			clsoptions << "legalpaper,";
			break;
		case PAPER_DEFAULT:
		case PAPER_A3:
		case PAPER_B3:
		case PAPER_B4:
		case PAPER_CUSTOM:
			break;
		}
	}

	// if needed
	if (sides != tclass.sides()) {
		switch (sides) {
		case LyXTextClass::OneSide:
			clsoptions << "oneside,";
			break;
		case LyXTextClass::TwoSides:
			clsoptions << "twoside,";
			break;
		}
	}

	// if needed
	if (columns != tclass.columns()) {
		if (columns == 2)
			clsoptions << "twocolumn,";
		else
			clsoptions << "onecolumn,";
	}

	if (!use_geometry
	    && orientation == ORIENTATION_LANDSCAPE)
		clsoptions << "landscape,";

	// language should be a parameter to \documentclass
	if (language->babel() == "hebrew"
	    && default_language->babel() != "hebrew")
		// This seems necessary
		features.useLanguage(default_language);

	ostringstream language_options;
	bool const use_babel = features.useBabel();
	if (use_babel) {
		language_options << features.getLanguages();
		language_options << language->babel();
		if (lyxrc.language_global_options)
			clsoptions << language_options.str() << ',';
	}

	// the user-defined options
	if (!options.empty()) {
		clsoptions << options << ',';
	}

	string strOptions(clsoptions.str());
	if (!strOptions.empty()) {
		strOptions = rtrim(strOptions, ",");
		// FIXME UNICODE
		os << '[' << from_utf8(strOptions) << ']';
	}

	os << '{' << from_ascii(tclass.latexname()) << "}\n";
	texrow.newline();
	// end of \documentclass defs

	// font selection must be done before loading fontenc.sty
	string const fonts =
		loadFonts(fontsRoman, fontsSans,
			  fontsTypewriter, fontsSC, fontsOSF,
			  fontsSansScale, fontsTypewriterScale);
	if (!fonts.empty()) {
		os << from_ascii(fonts);
		texrow.newline();
	}
	if (fontsDefaultFamily != "default")
		os << "\\renewcommand{\\familydefault}{\\"
		   << from_ascii(fontsDefaultFamily) << "}\n";
	// this one is not per buffer
	if (lyxrc.fontenc != "default") {
		os << "\\usepackage[" << from_ascii(lyxrc.fontenc)
		   << "]{fontenc}\n";
		texrow.newline();
	}

	if (inputenc == "auto") {
		string const doc_encoding =
			language->encoding()->latexName();

		// Create a list with all the input encodings used
		// in the document
		std::set<string> encodings =
			features.getEncodingSet(doc_encoding);

		// thailatex does not use the inputenc package, but sets up
		// babel directly for tis620-0 encoding, therefore we must
		// not request inputenc for tis620-0 encoding
		if (!encodings.empty() || doc_encoding != "tis620-0") {
			os << "\\usepackage[";
			std::set<string>::const_iterator it = encodings.begin();
			std::set<string>::const_iterator const end = encodings.end();
			if (it != end) {
				os << from_ascii(*it);
				++it;
			}
			for (; it != end; ++it)
				os << ',' << from_ascii(*it);
			if (doc_encoding != "tis620-0") {
				if (!encodings.empty())
					os << ',';
				os << from_ascii(doc_encoding);
			}
			os << "]{inputenc}\n";
			texrow.newline();
		}
	} else if (inputenc != "default" && inputenc != "tis620-0" &&
	           inputenc != "ascii") {
		os << "\\usepackage[" << from_ascii(inputenc)
		   << "]{inputenc}\n";
		texrow.newline();
	}

	if (use_geometry || nonstandard_papersize) {
		os << "\\usepackage{geometry}\n";
		texrow.newline();
		os << "\\geometry{verbose";
		if (orientation == ORIENTATION_LANDSCAPE)
			os << ",landscape";
		switch (papersize) {
		case PAPER_CUSTOM:
			if (!paperwidth.empty())
				os << ",paperwidth="
				   << from_ascii(paperwidth);
			if (!paperheight.empty())
				os << ",paperheight="
				   << from_ascii(paperheight);
			break;
		case PAPER_USLETTER:
			os << ",letterpaper";
			break;
		case PAPER_USLEGAL:
			os << ",legalpaper";
			break;
		case PAPER_USEXECUTIVE:
			os << ",executivepaper";
			break;
		case PAPER_A3:
			os << ",a3paper";
			break;
		case PAPER_A4:
			os << ",a4paper";
			break;
		case PAPER_A5:
			os << ",a5paper";
			break;
		case PAPER_B3:
			os << ",b3paper";
			break;
		case PAPER_B4:
			os << ",b4paper";
			break;
		case PAPER_B5:
			os << ",b5paper";
			break;
		default:
			// default papersize ie PAPER_DEFAULT
			switch (lyxrc.default_papersize) {
			case PAPER_DEFAULT: // keep compiler happy
			case PAPER_USLETTER:
				os << ",letterpaper";
				break;
			case PAPER_USLEGAL:
				os << ",legalpaper";
				break;
			case PAPER_USEXECUTIVE:
				os << ",executivepaper";
				break;
			case PAPER_A3:
				os << ",a3paper";
				break;
			case PAPER_A4:
				os << ",a4paper";
				break;
			case PAPER_A5:
				os << ",a5paper";
				break;
			case PAPER_B5:
				os << ",b5paper";
				break;
			case PAPER_B3:
			case PAPER_B4:
			case PAPER_CUSTOM:
				break;
			}
		}
		if (!topmargin.empty())
			os << ",tmargin=" << from_ascii(Length(topmargin).asLatexString());
		if (!bottommargin.empty())
			os << ",bmargin=" << from_ascii(Length(bottommargin).asLatexString());
		if (!leftmargin.empty())
			os << ",lmargin=" << from_ascii(Length(leftmargin).asLatexString());
		if (!rightmargin.empty())
			os << ",rmargin=" << from_ascii(Length(rightmargin).asLatexString());
		if (!headheight.empty())
			os << ",headheight=" << from_ascii(Length(headheight).asLatexString());
		if (!headsep.empty())
			os << ",headsep=" << from_ascii(Length(headsep).asLatexString());
		if (!footskip.empty())
			os << ",footskip=" << from_ascii(Length(footskip).asLatexString());
		os << "}\n";
		texrow.newline();
	}

	if (tokenPos(tclass.opt_pagestyle(),
		     '|', pagestyle) >= 0) {
		if (pagestyle == "fancy") {
			os << "\\usepackage{fancyhdr}\n";
			texrow.newline();
		}
		os << "\\pagestyle{" << from_ascii(pagestyle) << "}\n";
		texrow.newline();
	}

	// Only if class has a ToC hierarchy
	if (tclass.hasTocLevels()) {
		if (secnumdepth != tclass.secnumdepth()) {
			os << "\\setcounter{secnumdepth}{"
			   << secnumdepth
			   << "}\n";
			texrow.newline();
		}
		if (tocdepth != tclass.tocdepth()) {
			os << "\\setcounter{tocdepth}{"
			   << tocdepth
			   << "}\n";
			texrow.newline();
		}
	}

	if (paragraph_separation) {
		switch (getDefSkip().kind()) {
		case VSpace::SMALLSKIP:
			os << "\\setlength{\\parskip}{\\smallskipamount}\n";
			break;
		case VSpace::MEDSKIP:
			os << "\\setlength{\\parskip}{\\medskipamount}\n";
			break;
		case VSpace::BIGSKIP:
			os << "\\setlength{\\parskip}{\\bigskipamount}\n";
			break;
		case VSpace::LENGTH:
			os << "\\setlength{\\parskip}{"
			   << from_utf8(getDefSkip().length().asLatexString())
			   << "}\n";
			break;
		default: // should never happen // Then delete it.
			os << "\\setlength{\\parskip}{\\medskipamount}\n";
			break;
		}
		texrow.newline();

		os << "\\setlength{\\parindent}{0pt}\n";
		texrow.newline();
	}

	// If we use jurabib, we have to call babel here.
	if (use_babel && features.isRequired("jurabib")) {
		os << from_ascii(babelCall(language_options.str()))
		   << '\n'
		   << from_ascii(features.getBabelOptions());
		texrow.newline();
	}

	// Now insert the LyX specific LaTeX commands...

	// The optional packages;
	docstring lyxpreamble(from_ascii(features.getPackages()));

	// this might be useful...
	lyxpreamble += "\n\\makeatletter\n";

	// Some macros LyX will need
	docstring tmppreamble(from_ascii(features.getMacros()));

	if (!tmppreamble.empty()) {
		lyxpreamble += "\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
			"LyX specific LaTeX commands.\n"
			+ tmppreamble + '\n';
	}

	// the text class specific preamble
	tmppreamble = features.getTClassPreamble();
	if (!tmppreamble.empty()) {
		lyxpreamble += "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
			"Textclass specific LaTeX commands.\n"
			+ tmppreamble + '\n';
	}

	/* the user-defined preamble */
	if (!preamble.empty()) {
		// FIXME UNICODE
		lyxpreamble += "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
			"User specified LaTeX commands.\n"
			+ from_utf8(preamble) + '\n';
	}

	// Itemize bullet settings need to be last in case the user
	// defines their own bullets that use a package included
	// in the user-defined preamble -- ARRae
	// Actually it has to be done much later than that
	// since some packages like frenchb make modifications
	// at \begin{document} time -- JMarc
	docstring bullets_def;
	for (int i = 0; i < 4; ++i) {
		if (user_defined_bullet(i) != ITEMIZE_DEFAULTS[i]) {
			if (bullets_def.empty())
				bullets_def += "\\AtBeginDocument{\n";
			bullets_def += "  \\def\\labelitemi";
			switch (i) {
				// `i' is one less than the item to modify
			case 0:
				break;
			case 1:
				bullets_def += 'i';
				break;
			case 2:
				bullets_def += "ii";
				break;
			case 3:
				bullets_def += 'v';
				break;
			}
			bullets_def += '{' +
				user_defined_bullet(i).getText()
				+ "}\n";
		}
	}

	if (!bullets_def.empty())
		lyxpreamble += bullets_def + "}\n\n";

	// We try to load babel late, in case it interferes
	// with other packages.
	// Jurabib has to be called after babel, though.
	if (use_babel && !features.isRequired("jurabib")) {
		// FIXME UNICODE
		lyxpreamble += from_utf8(babelCall(language_options.str())) + '\n';
		lyxpreamble += from_utf8(features.getBabelOptions());
	}

	lyxpreamble += "\\makeatother\n";

	// dvipost settings come after everything else
	if (features.isAvailable("dvipost") && outputChanges) {
		lyxpreamble +=
			"\\dvipostlayout\n"
			"\\dvipost{osstart color push Red}\n"
			"\\dvipost{osend color pop}\n"
			"\\dvipost{cbstart color push Blue}\n"
			"\\dvipost{cbend color pop}\n";
	}

	int const nlines =
		int(lyx::count(lyxpreamble.begin(), lyxpreamble.end(), '\n'));
	for (int j = 0; j != nlines; ++j) {
		texrow.newline();
	}

	os << lyxpreamble;
	return use_babel;
}


void BufferParams::useClassDefaults()
{
	LyXTextClass const & tclass = textclasslist[textclass];

	sides = tclass.sides();
	columns = tclass.columns();
	pagestyle = tclass.pagestyle();
	options = tclass.options();
	// Only if class has a ToC hierarchy
	if (tclass.hasTocLevels()) {
		secnumdepth = tclass.secnumdepth();
		tocdepth = tclass.tocdepth();
	}
}


bool BufferParams::hasClassDefaults() const
{
	LyXTextClass const & tclass = textclasslist[textclass];

	return (sides == tclass.sides()
		&& columns == tclass.columns()
		&& pagestyle == tclass.pagestyle()
		&& options == tclass.options()
		&& secnumdepth == tclass.secnumdepth()
		&& tocdepth == tclass.tocdepth());
}


LyXTextClass const & BufferParams::getLyXTextClass() const
{
	return textclasslist[textclass];
}


Font const BufferParams::getFont() const
{
	Font f = getLyXTextClass().defaultfont();
	f.setLanguage(language);
	if (fontsDefaultFamily == "rmdefault")
		f.setFamily(Font::ROMAN_FAMILY);
	else if (fontsDefaultFamily == "sfdefault")
		f.setFamily(Font::SANS_FAMILY);
	else if (fontsDefaultFamily == "ttdefault")
		f.setFamily(Font::TYPEWRITER_FAMILY);
	return f;
}


void BufferParams::readPreamble(Lexer & lex)
{
	if (lex.getString() != "\\begin_preamble")
		lyxerr << "Error (BufferParams::readPreamble):"
			"consistency check failed." << endl;

	preamble = lex.getLongString("\\end_preamble");
}


void BufferParams::readLanguage(Lexer & lex)
{
	if (!lex.next()) return;

	string const tmptok = lex.getString();

	// check if tmptok is part of tex_babel in tex-defs.h
	language = languages.getLanguage(tmptok);
	if (!language) {
		// Language tmptok was not found
		language = default_language;
		lyxerr << "Warning: Setting language `"
		       << tmptok << "' to `" << language->lang()
		       << "'." << endl;
	}
}


void BufferParams::readGraphicsDriver(Lexer & lex)
{
	if (!lex.next()) return;

	string const tmptok = lex.getString();
	// check if tmptok is part of tex_graphics in tex_defs.h
	int n = 0;
	while (true) {
		string const test = tex_graphics[n++];

		if (test == tmptok) {
			graphicsDriver = tmptok;
			break;
		} else if (test == "") {
			lex.printError(
				"Warning: graphics driver `$$Token' not recognized!\n"
				"         Setting graphics driver to `default'.\n");
			graphicsDriver = "default";
			break;
		}
	}
}


void BufferParams::readBullets(Lexer & lex)
{
	if (!lex.next()) return;

	int const index = lex.getInteger();
	lex.next();
	int temp_int = lex.getInteger();
	user_defined_bullet(index).setFont(temp_int);
	temp_bullet(index).setFont(temp_int);
	lex >> temp_int;
	user_defined_bullet(index).setCharacter(temp_int);
	temp_bullet(index).setCharacter(temp_int);
	lex >> temp_int;
	user_defined_bullet(index).setSize(temp_int);
	temp_bullet(index).setSize(temp_int);
}


void BufferParams::readBulletsLaTeX(Lexer & lex)
{
	// The bullet class should be able to read this.
	if (!lex.next()) return;
	int const index = lex.getInteger();
	lex.next(true);
	docstring const temp_str = lex.getDocString();

	user_defined_bullet(index).setText(temp_str);
	temp_bullet(index).setText(temp_str);
}


string const BufferParams::paperSizeName() const
{
	char real_papersize = papersize;
	if (real_papersize == PAPER_DEFAULT)
		real_papersize = lyxrc.default_papersize;

	switch (real_papersize) {
	case PAPER_A3:
		return "a3";
	case PAPER_A4:
		return "a4";
	case PAPER_A5:
		return "a5";
	case PAPER_B5:
		return "b5";
	case PAPER_USEXECUTIVE:
		return "foolscap";
	case PAPER_USLEGAL:
		return "legal";
	case PAPER_USLETTER:
	default:
		return "letter";
	}
}


string const BufferParams::dvips_options() const
{
	string result;

	if (use_geometry
	    && papersize == PAPER_CUSTOM
	    && !lyxrc.print_paper_dimension_flag.empty()
	    && !paperwidth.empty()
	    && !paperheight.empty()) {
		// using a custom papersize
		result = lyxrc.print_paper_dimension_flag;
		result += ' ' + paperwidth;
		result += ',' + paperheight;
	} else {
		string const paper_option = paperSizeName();
		if (paper_option != "letter" ||
		    orientation != ORIENTATION_LANDSCAPE) {
			// dvips won't accept -t letter -t landscape.
			// In all other cases, include the paper size
			// explicitly.
			result = lyxrc.print_paper_flag;
			result += ' ' + paper_option;
		}
	}
	if (orientation == ORIENTATION_LANDSCAPE &&
	    papersize != PAPER_CUSTOM)
		result += ' ' + lyxrc.print_landscape_flag;
	return result;
}


string const BufferParams::babelCall(string const & lang_opts) const
{
	string tmp = lyxrc.language_package;
	if (!lyxrc.language_global_options && tmp == "\\usepackage{babel}")
		tmp = string("\\usepackage[") + lang_opts + "]{babel}";
	return tmp;
}


string const BufferParams::loadFonts(string const & rm,
				     string const & sf, string const & tt,
				     bool const & sc, bool const & osf,
				     int const & sfscale, int const & ttscale) const
{
	/* The LaTeX font world is in a flux. In the PSNFSS font interface,
	   several packages have been replaced by others, that might not
	   be installed on every system. We have to take care for that
	   (see psnfss.pdf). We try to support all psnfss fonts as well
	   as the fonts that have become de facto standard in the LaTeX
	   world (e.g. Latin Modern). We do not support obsolete fonts
	   (like PSLatex). In general, it should be possible to mix any
	   rm font with any sf or tt font, respectively. (JSpitzm)
	   TODO:
		-- separate math fonts.
	*/

	if (rm == "default" && sf == "default" && tt == "default")
		//nothing to do
		return string();

	ostringstream os;

	// ROMAN FONTS
	// Computer Modern (must be explicitely selectable -- there might be classes
	// that define a different default font!
	if (rm == "cmr") {
		os << "\\renewcommand{\\rmdefault}{cmr}\n";
		// osf for Computer Modern needs eco.sty
		if (osf)
			os << "\\usepackage{eco}\n";
	}
	// Latin Modern Roman
	else if (rm == "lmodern")
		os << "\\usepackage{lmodern}\n";
	// AE
	else if (rm == "ae") {
		// not needed when using OT1 font encoding.
		if (lyxrc.fontenc != "default")
			os << "\\usepackage{ae,aecompl}\n";
	}
	// Times
	else if (rm == "times") {
		// try to load the best available package
		if (LaTeXFeatures::isAvailable("mathptmx"))
			os << "\\usepackage{mathptmx}\n";
		else if (LaTeXFeatures::isAvailable("mathptm"))
			os << "\\usepackage{mathptm}\n";
		else
			os << "\\usepackage{times}\n";
	}
	// Palatino
	else if (rm == "palatino") {
		// try to load the best available package
		if (LaTeXFeatures::isAvailable("mathpazo")) {
			os << "\\usepackage";
			if (osf || sc) {
				os << '[';
				if (!osf)
					os << "sc";
				else
					// "osf" includes "sc"!
					os << "osf";
				os << ']';
			}
			os << "{mathpazo}\n";
		}
		else if (LaTeXFeatures::isAvailable("mathpple"))
			os << "\\usepackage{mathpple}\n";
		else
			os << "\\usepackage{palatino}\n";
	}
	// Utopia
	else if (rm == "utopia") {
		// fourier supersedes utopia.sty, but does
		// not work with OT1 encoding.
		if (LaTeXFeatures::isAvailable("fourier")
		    && lyxrc.fontenc != "default") {
			os << "\\usepackage";
			if (osf || sc) {
				os << '[';
				if (sc)
					os << "expert";
				if (osf && sc)
					os << ',';
				if (osf)
					os << "oldstyle";
				os << ']';
			}
			os << "{fourier}\n";
		}
		else
			os << "\\usepackage{utopia}\n";
	}
	// Bera (complete fontset)
	else if (rm == "bera" && sf == "default" && tt == "default")
		os << "\\usepackage{bera}\n";
	// everything else
	else if (rm != "default")
		os << "\\usepackage" << "{" << rm << "}\n";

	// SANS SERIF
	// Helvetica, Bera Sans
	if (sf == "helvet" || sf == "berasans") {
		if (sfscale != 100)
			os << "\\usepackage[scaled=" << float(sfscale) / 100
			   << "]{" << sf << "}\n";
		else
			os << "\\usepackage{" << sf << "}\n";
	}
	// Avant Garde
	else if (sf == "avant")
		os << "\\usepackage{" << sf << "}\n";
	// Computer Modern, Latin Modern, CM Bright
	else if (sf != "default")
		os << "\\renewcommand{\\sfdefault}{" << sf << "}\n";

	// monospaced/typewriter
	// Courier, LuxiMono
	if (tt == "luximono" || tt == "beramono") {
		if (ttscale != 100)
			os << "\\usepackage[scaled=" << float(ttscale) / 100
			   << "]{" << tt << "}\n";
		else
			os << "\\usepackage{" << tt << "}\n";
	}
	// Courier
	else if (tt == "courier" )
		os << "\\usepackage{" << tt << "}\n";
	// Computer Modern, Latin Modern, CM Bright
	else if  (tt != "default")
		os << "\\renewcommand{\\ttdefault}{" << tt << "}\n";

	return os.str();
}


Encoding const & BufferParams::encoding() const
{
	if (inputenc == "auto" || inputenc == "default")
		return *(language->encoding());
	Encoding const * const enc =
		encodings.getFromLaTeXName(inputenc);
	if (enc)
		return *enc;
	lyxerr << "Unknown inputenc value `" << inputenc
	       << "'. Using `auto' instead." << endl;
	return *(language->encoding());
}


biblio::CiteEngine BufferParams::getEngine() const
{
	// FIXME the class should provide the numerical/
	// authoryear choice
	if (getLyXTextClass().provides("natbib")
	    && cite_engine_ != biblio::ENGINE_NATBIB_NUMERICAL)
		return biblio::ENGINE_NATBIB_AUTHORYEAR;
	return cite_engine_;
}


void BufferParams::setCiteEngine(biblio::CiteEngine const cite_engine)
{
	cite_engine_ = cite_engine;
}

} // namespace lyx
