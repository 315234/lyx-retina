// -*- C++ -*-
/**
 * \file insetfloat.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef InsetFloat_H
#define InsetFloat_H


#include "insetcollapsable.h"
#include "toc.h"

#include <boost/signals/signal0.hpp>

class Painter;

struct InsetFloatParams {
	///
	InsetFloatParams() : placement("htbp"), wide(false) {}
	///
	string type;
	///
	string placement;
	///
	bool wide;
};


/** The float inset

*/
class InsetFloat : public InsetCollapsable {
public:
	///
	InsetFloat(BufferParams const &, string const &);
	///
	InsetFloat(InsetFloat const &, bool same_id = false);
	///
	~InsetFloat();
	///
	void write(Buffer const * buf, std::ostream & os) const;
	///
	void read(Buffer const * buf, LyXLex & lex);
	///
	void validate(LaTeXFeatures & features) const;
	///
	Inset * clone(Buffer const &, bool same_id = false) const;
	///
	Inset::Code lyxCode() const { return Inset::FLOAT_CODE; }
	///
	int latex(Buffer const *, std::ostream &, bool fragile, bool fp) const;
	///
	int docbook(Buffer const *, std::ostream &, bool mixcont) const;
	///
	string const editMessage() const;
	///
	bool insetAllowed(Inset::Code) const;
	/** returns true if, when outputing LaTeX, font changes should
	    be closed before generating this inset. This is needed for
	    insets that may contain several paragraphs */
	bool noFontChange() const { return true; }
	///
	string const & type() const;
	///
	void placement(string const & p);
	///
	string const & placement() const;
	///
	void wide(bool w, BufferParams const &);
	///
	bool wide() const;
	///
	void addToToc(toc::TocList &, Buffer const *) const;
	///
	bool  showInsetDialog(BufferView *) const;
	///
	boost::signal0<void> hideDialog;
	///
	InsetFloatParams const & params() const { return params_; }
	///
	void writeParams(std::ostream & os) const;
	///
	void readParams(Buffer const * buf, LyXLex & lex);
	
private:
	///
	InsetFloatParams params_;
};

#endif
