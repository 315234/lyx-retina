// -*- C++ -*-
/**
 * \file insetline.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_LINE_H
#define INSET_LINE_H


#include "inset.h"

class InsetLine : public InsetOld {
public:

	InsetLine() {}

	std::auto_ptr<InsetBase> clone() const {
		return std::auto_ptr<InsetBase>(new InsetLine);
	}

	InsetOld::Code lyxCode() const { return InsetOld::LINE_CODE; }

	void metrics(MetricsInfo &, Dimension &) const;

	void draw(PainterInfo & pi, int x, int y) const;

	int latex(Buffer const &, std::ostream &,
			  LatexRunParams const &) const;

	int ascii(Buffer const &, std::ostream &,
		  LatexRunParams const &) const;

	int linuxdoc(Buffer const &, std::ostream &,
		     LatexRunParams const &) const;

	int docbook(Buffer const &, std::ostream &,
		    LatexRunParams const &) const;

	void read(Buffer const &, LyXLex & lex);

	void write(Buffer const & buf, std::ostream & os) const;
	/// We don't need \begin_inset and \end_inset
	bool directWrite() const { return true; }

	bool display() const { return true; }
	///
	void validate(LaTeXFeatures & features) const;
};

#endif // INSET_NEWLINE_H
