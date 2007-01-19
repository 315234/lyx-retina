// -*- C++ -*-
/**
 * \file insetcharstyle.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author Martin Vermeer
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETCHARSTYLE_H
#define INSETCHARSTYLE_H

#include "insetcollapsable.h"
#include "lyxtextclass.h"


namespace lyx {


class InsetCharStyleParams {
public:
	///
	void write(std::ostream & os) const;
	///
	void read(LyXLex & lex);
	///
	std::string type;
	///
	std::string latextype;
	///
	std::string latexname;
	///
	std::string latexparam;
	///
	LyXFont font;
	///
	LyXFont labelfont;
	///
	bool show_label;
};


/** The CharStyle inset, also XML short element

*/
class InsetCharStyle : public InsetCollapsable {
public:
	/// Construct an undefined character style
	InsetCharStyle(BufferParams const &, std::string const);
	///
	InsetCharStyle(BufferParams const &, CharStyles::iterator);
	/// Is this character style defined in the document's textclass?
	/// May be wrong after textclass change or paste from another document
	bool undefined() const;
	/// Set the character style to "undefined"
	void setUndefined();
	/// (Re-)set the character style parameters from \p cs
	void setDefined(CharStyles::iterator cs);
	///
	virtual docstring const editMessage() const;
	///
	InsetBase::Code lyxCode() const { return InsetBase::CHARSTYLE_CODE; }
	///
	void write(Buffer const &, std::ostream &) const;
	///
	void read(Buffer const & buf, LyXLex & lex);
	///
	bool metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo &, int, int) const;
	///
	void getDrawFont(LyXFont &) const;
	///
	bool forceDefaultParagraphs(idx_type) const { return true; }
	///
	int latex(Buffer const &, odocstream &,
		  OutputParams const &) const;
	///
	int docbook(Buffer const &, odocstream &,
		    OutputParams const &) const;
	///
	int plaintext(Buffer const &, odocstream &,
		  OutputParams const &) const;
	/// the string that is passed to the TOC
	virtual void textString(Buffer const &, odocstream &) const;
	///
	void validate(LaTeXFeatures &) const;

	///
	InsetCharStyleParams const & params() const { return params_; }

	/// should paragraph indendation be ommitted in any case?
	bool neverIndent(Buffer const &) const { return true; }

protected:
	InsetCharStyle(InsetCharStyle const &);
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);
	///
	bool getStatus(LCursor & cur, FuncRequest const & cmd, FuncStatus &) const;

private:
	friend class InsetCharStyleParams;

	virtual std::auto_ptr<InsetBase> doClone() const;

	/// used by the constructors
	void init();
	///
	InsetCharStyleParams params_;
};


} // namespace lyx

#endif
