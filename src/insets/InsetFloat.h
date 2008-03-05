// -*- C++ -*-
/**
 * \file InsetFloat.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETFLOAT_H
#define INSETFLOAT_H

#include "InsetCollapsable.h"
#include "MailInset.h"


namespace lyx {


class InsetFloatParams {
public:
	///
	InsetFloatParams() : wide(false), sideways(false), subfloat(false) {}
	///
	void write(std::ostream & os) const;
	///
	void read(Lexer & lex);
	///
	std::string type;
	///
	std::string placement;
	///
	bool wide;
	///
	bool sideways;
	///
	bool subfloat;
};


/** The float inset

*/
class InsetFloat : public InsetCollapsable {
public:
	///
	InsetFloat(Buffer const &, std::string const &);
	///
	~InsetFloat();
	///
	docstring name() const { return name_; }
	///
	void write(std::ostream & os) const;
	///
	void read(Lexer & lex);
	///
	void validate(LaTeXFeatures & features) const;
	///
	InsetCode lyxCode() const { return FLOAT_CODE; }
	///
	bool isMacroScope() const { return true; }
	///
	int latex(odocstream &, OutputParams const &) const;
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring editMessage() const;
	///
	bool insetAllowed(InsetCode) const;
	/** returns true if, when outputing LaTeX, font changes should
	    be closed before generating this inset. This is needed for
	    insets that may contain several paragraphs */
	bool noFontChange() const { return true; }
	///
	void wide(bool w, BufferParams const &);
	///
	void sideways(bool s, BufferParams const &);
	///
	void subfloat(bool s, BufferParams const &);
	///
	bool showInsetDialog(BufferView *) const;
	///
	InsetFloatParams const & params() const { return params_; }
private:
	///
	bool getStatus(Cursor &, FuncRequest const &, FuncStatus &) const;
	// Update the counters of this inset and of its contents
	void updateLabels(ParIterator const &);
	///
	void doDispatch(Cursor & cur, FuncRequest & cmd);
	///
	Inset * clone() const { return new InsetFloat(*this); }
	///
	docstring getCaption(OutputParams const &) const;
	///
	InsetFloatParams params_;
	///
	docstring name_;
};


class InsetFloatMailer : public MailInset {
public:
	///
	InsetFloatMailer(InsetFloat & inset);
	///
	virtual Inset & inset() const { return inset_; }
	///
	virtual std::string const & name() const { return name_; }
	///
	virtual std::string const inset2string(Buffer const &) const;
	///
	static void string2params(std::string const &, InsetFloatParams &);
	///
	static std::string const params2string(InsetFloatParams const &);
private:
	///
	static std::string const name_;
	///
	InsetFloat & inset_;
};


} // namespace lyx

#endif
