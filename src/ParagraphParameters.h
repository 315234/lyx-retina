// -*- C++ -*-
/**
 * \file ParagraphParameters.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Angus Leeming
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef PARAGRAPHPARAMETERS_H
#define PARAGRAPHPARAMETERS_H

#include "Layout.h"
#include "Length.h"
#include "Spacing.h"

#include "support/types.h"
#include "support/docstring.h"

#include <iosfwd>
#include <string>


namespace lyx {

class BufferView;
class Length;
class Lexer;
class Paragraph;
class Spacing;


///
class ParagraphParameters {
public:
	///
	ParagraphParameters();
	///
	void clear();
	///
	bool sameLayout(ParagraphParameters const &) const;
	///
	Spacing const & spacing() const;
	///
	void spacing(Spacing const &);
	///
	bool noindent() const;
	///
	void noindent(bool);
	///
	LyXAlignment align() const;
	///
	void align(LyXAlignment);
	///
	depth_type depth() const;
	///
	void depth(depth_type);
	///
	bool startOfAppendix() const;
	///
	void startOfAppendix(bool);
	///
	bool appendix() const;
	///
	void appendix(bool);
	///
	docstring const & labelString() const;
	///
	void labelString(docstring const &);
	///
	docstring const & labelWidthString() const;
	///
	void labelWidthString(docstring const &);
	///
	Length const & leftIndent() const;
	///
	void leftIndent(Length const &);

	/// read the parameters from a lex
	void read(Lexer & lex);

	/// write out the parameters to a stream
	void write(std::ostream & os) const;

	//friend bool operator==(ParameterStruct const & ps1,
	//ParameterStruct const & ps2);

private:
	///
	Spacing spacing_;
	///
	bool noindent_;
	///
	bool start_of_appendix_;
	///
	bool appendix_;
	///
	LyXAlignment align_;
	///
	depth_type depth_;
	///
	docstring labelstring_;
	///
	docstring labelwidthstring_;
	///
	Length leftindent_;
};



/** Generate a string \param data from \param par's ParagraphParameters.
    The function also generates some additional info needed by the
    Paragraph dialog.
 */
void params2string(Paragraph const & par, std::string & data);


} // namespace lyx

#endif
