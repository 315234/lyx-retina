// -*- C++ -*-
/**
 * \file InsetSpecialChar.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup Nielsen
 * \author Jean-Marc Lasgouttes
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_SPECIALCHAR_H
#define INSET_SPECIALCHAR_H


#include "Inset.h"


namespace lyx {

class LaTeXFeatures;

///  Used to insert special chars
class InsetSpecialChar : public Inset {
public:

	/// The different kinds of special chars we support
	enum Kind {
		/// Optional hyphenation point (\-)
		HYPHENATION,
		/// Ligature break point (\textcompwordmark)
		LIGATURE_BREAK,
		/// ... (\ldots)
		LDOTS,
		/// End of sentence punctuation (\@)
		END_OF_SENTENCE,
		/// Menu separator
		MENU_SEPARATOR,
	};

	///
	InsetSpecialChar() {}
	///
	explicit
	InsetSpecialChar(Kind k);
	///
	Kind kind() const;
	///
	void metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(Buffer const &, std::ostream &) const;
	/// Will not be used when lyxf3
	void read(Buffer const &, Lexer & lex);
	///
	int latex(Buffer const &, odocstream &,
		  OutputParams const &) const;
	///
	int plaintext(Buffer const &, odocstream &,
		      OutputParams const &) const;
	///
	int docbook(Buffer const &, odocstream &,
		    OutputParams const &) const;
	/// the string that is passed to the TOC
	virtual int textString(Buffer const &, odocstream &,
		OutputParams const &) const;
	///
	Inset::Code lyxCode() const { return Inset::SPECIALCHAR_CODE; }
	/// We don't need \begin_inset and \end_inset
	bool directWrite() const { return true; }
	///
	void validate(LaTeXFeatures &) const;

	// should this inset be handled like a normal charater
	bool isChar() const;
	/// is this equivalent to a letter?
	bool isLetter() const;
	// should we break lines after this inset?
	bool isLineSeparator() const;
private:
	virtual Inset * clone() const;

	/// And which kind is this?
	Kind kind_;
};


} // namespace lyx

#endif
