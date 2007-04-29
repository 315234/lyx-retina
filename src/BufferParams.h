// -*- C++ -*-
/**
 * \file BufferParams.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BUFFERPARAMS_H
#define BUFFERPARAMS_H

#include "TextClass.h"
#include "paper.h"

#include "insets/InsetQuotes.h"

#include "support/copied_ptr.h"
#include "support/types.h"

#include "frontends/controllers/frontend_helpers.h"

#include <vector>


namespace lyx {

class AuthorList;
class BranchList;
class Bullet;
class Encoding;
class Lexer;
class LatexFeatures;
class Spacing;
class TexRow;
class VSpace;
class Language;


/** Buffer parameters.
 *  This class contains all the parameters for this a buffer uses. Some
 *  work needs to be done on this class to make it nice. Now everything
 *  is in public.
 */
class BufferParams {
public:
	///
	enum PARSEP {
		///
		PARSEP_INDENT,
		///
		PARSEP_SKIP
	};
	///
	BufferParams();
	~BufferParams();

	/// read a header token, if unrecognised, return it or an unknown class name
	std::string const readToken(Lexer & lex, std::string const & token);

	///
	void writeFile(std::ostream &) const;

	/** \returns true if the babel package is used (interogates
	 *  the BufferParams and a LyXRC variable).
	 *  This returned value can then be passed to the insets...
	 */
	bool writeLaTeX(odocstream &, LaTeXFeatures &, TexRow &) const;

	///
	void useClassDefaults();

	///
	bool hasClassDefaults() const;

	///
	VSpace const & getDefSkip() const;

	///
	void setDefSkip(VSpace const & vs);

	/** Wether paragraphs are separated by using a indent like in
	 *  articles or by using a little skip like in letters.
	 */
	PARSEP paragraph_separation;
	///
	InsetQuotes::quote_language quotes_language;
	///
	InsetQuotes::quote_times quotes_times;
	///
	std::string fontsize;
	///
	textclass_type textclass;
	///
	TextClass const & getTextClass() const;

	/// returns the main font for the buffer (document)
	Font const getFont() const;

	/* this are for the PaperLayout */
	/// the papersize
	PAPER_SIZE papersize;
	///
	PAPER_ORIENTATION orientation;
	/// use custom margins
	bool use_geometry;
	///
	std::string paperwidth;
	///
	std::string paperheight;
	///
	std::string leftmargin;
	///
	std::string topmargin;
	///
	std::string rightmargin;
	///
	std::string bottommargin;
	///
	std::string headheight;
	///
	std::string headsep;
	///
	std::string footskip;

	/* some LaTeX options */
	/// The graphics driver
	std::string graphicsDriver;
	/// the rm font
	std::string fontsRoman;
	/// the sf font
	std::string fontsSans;
	/// the tt font
	std::string fontsTypewriter;
	/// the default family (rm, sf, tt)
	std::string fontsDefaultFamily;
	/// use expert Small Caps
	bool fontsSC;
	/// use Old Style Figures
	bool fontsOSF;
	/// the scale factor of the sf font
	int fontsSansScale;
	/// the scale factor of the tt font
	int fontsTypewriterScale;
	///
	Spacing & spacing();
	Spacing const & spacing() const;
	///
	int secnumdepth;
	///
	int tocdepth;
	///
	Language const * language;
	/// BranchList:
	BranchList & branchlist();
	BranchList const & branchlist() const;
	/**
	 * The input encoding for LaTeX. This can be one of
	 * - \c auto: find out the input encoding from the used languages
	 * - \c default: ditto
	 * - any encoding supported by the inputenc package
	 * The encoding of the LyX file is always utf8 and has nothing to
	 * do with this setting.
	 * The difference between \c auto and \c default is that \c auto also
	 * causes loading of the inputenc package, while \c default does not.
	 * \c default will not work unless the user takes additional measures
	 * (such as using special environments like the CJK environment from
	 * CJK.sty).
	 * \c default can be seen as an unspecified 8bit encoding, since LyX
	 * does not interpret it in any way apart from display on screen.
	 */
	std::string inputenc;
	/// The main encoding used by this buffer for LaTeX output.
	/// Individual pieces of text can use different encodings.
	Encoding const & encoding() const;
	///
	std::string preamble;
	///
	std::string options;
	///
	std::string float_placement;
	///
	unsigned int columns;
	///
	TextClass::PageSides sides;
	///
	std::string pagestyle;
	/// \param index should lie in the range 0 <= \c index <= 3.
	Bullet & temp_bullet(size_type index);
	Bullet const & temp_bullet(size_type index) const;
	/// \param index should lie in the range 0 <= \c index <= 3.
	Bullet & user_defined_bullet(size_type index);
	Bullet const & user_defined_bullet(size_type index) const;
	///
	void readPreamble(Lexer &);
	///
	void readLanguage(Lexer &);
	///
	void readGraphicsDriver(Lexer &);
	///
	void readBullets(Lexer &);
	///
	void readBulletsLaTeX(Lexer &);

	/// Whether to load a package such as amsmath or esint.
	/// The enum values must not be changed (file format!)
	enum Package {
		/// Don't load the package. For experts only.
		package_off = 0,
		/// Load the package if needed (recommended)
		package_auto = 1,
		/// Always load the package (e.g. if the document contains
		/// some ERT that needs the package)
		package_on = 2
	};
	/// Whether and how to load amsmath
	Package use_amsmath;
	/// Whether and how to load esint
	Package use_esint;
	///
	bool use_bibtopic;
	/// revision tracking for this buffer ?
	bool trackChanges;
	/** This param decides whether change tracking marks should be output
	 *  (using the dvipost package) or if the current "state" of the
	 *  document should be output instead. Since dvipost needs dvi
	 *  specials, it only works with dvi/ps output (the param will be
	 *  ignored with other output flavors and disabled when dvipost is
	 *  not installed).
	 */
	bool outputChanges;
	/// Time ago we agreed that this was a buffer property [ale990407]
	std::string parentname;
	///
	bool compressed;

	/// the author list for the document
	AuthorList & authors();
	AuthorList const & authors() const;

	/// map of the file's author IDs to buffer author IDs
	std::vector<unsigned int> author_map;
	///
	std::string const dvips_options() const;
	///
	std::string const paperSizeName() const;
	///
	std::string const babelCall(std::string const & lang_opts) const;
	/// set up the document fonts
	std::string const loadFonts(std::string const & rm,
				     std::string const & sf, std::string const & tt,
				     bool const & sc, bool const & osf,
				     int const & sfscale, int const & ttscale) const;
	/// path of the current buffer
	std::string filepath;
	/// get the appropriate cite engine (natbib handling)
	biblio::CiteEngine getEngine() const;

	///
	void setCiteEngine(biblio::CiteEngine const);

private:
	/** Use the Pimpl idiom to hide those member variables that would otherwise
	 *  drag in other header files.
	 */
	class Impl;
	class MemoryTraits {
	public:
		static Impl * clone(Impl const *);
		static void destroy(Impl *);
	};
	support::copied_ptr<Impl, MemoryTraits> pimpl_;

	///
	biblio::CiteEngine cite_engine_;
};

} // namespace lyx

#endif
