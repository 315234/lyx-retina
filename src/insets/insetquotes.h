// -*- C++ -*-
/**
 * \file insetquotes.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jean-Marc Lasgouttes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSET_QUOTES_H
#define INSET_QUOTES_H


#include "inset.h"

class BufferParams;
class Language;

struct LaTeXFeatures;


/** Quotes.
  Used for the various quotes. German, English, French, all either
  double or single **/
class InsetQuotes : public Inset {
public:
	///
	enum quote_language {
		///
		EnglishQ,
		///
		SwedishQ,
		///
		GermanQ,
		///
		PolishQ,
		///
		FrenchQ,
		///
		DanishQ
	};
	///
	enum quote_side {
		///
		LeftQ,
		///
		RightQ
	};
	///
	enum quote_times {
		///
		SingleQ,
		///
		DoubleQ
	};

	/** The constructor works like this:
	  \begin{itemize}
	    \item fls <- french single quote left
	    \item grd <- german double quote right
	    \item etc.
	  \end{itemize}
	  */
	explicit
	InsetQuotes(string const & str = "eld");
	/// Create the right quote inset after character c
	InsetQuotes(char c, BufferParams const & params);
	///
	Inset * clone() const;
	///
	void metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
#if 0
	///
	LyXFont const convertFont(LyXFont const & font) const;
#endif
	///
	void write(Buffer const *, std::ostream &) const;
	///
	void read(Buffer const *, LyXLex & lex);
	///
	int latex(Buffer const *, std::ostream &,
		  LatexRunParams const &) const;
	///
	int ascii(Buffer const *, std::ostream &, int linelen) const;
	///
	int linuxdoc(Buffer const *, std::ostream &) const;
	///
	int docbook(Buffer const *, std::ostream &, bool mixcont) const;
	///
	void validate(LaTeXFeatures &) const;
	///
	Inset::Code lyxCode() const;
	// should this inset be handled like a normal charater
	bool isChar() const { return true; }

private:
	///
	quote_language language_;
	///
	quote_side side_;
	///
	quote_times times_;

	/** The parameters of the constructor are the language, the
	    side and the multiplicity of the quote.
	 */
	InsetQuotes(quote_language l, quote_side s, quote_times t);
	///
	void parseString(string const &);
	///
	string const dispString(Language const *) const;
};
#endif
