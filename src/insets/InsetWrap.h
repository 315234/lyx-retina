// -*- C++ -*-
/**
 * \file InsetWrap.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Dekel Tsur
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETWRAP_H
#define INSETWRAP_H

#include "InsetCollapsable.h"
#include "Length.h"
#include "MailInset.h"


namespace lyx {


class InsetWrapParams {
public:
	///
	void write(std::ostream &) const;
	///
	void read(Lexer &);

	///
	std::string type;
	///
	std::string placement;
	///
	Length width;
};


/** The wrap inset
 */
class InsetWrap : public InsetCollapsable {
public:
	///
	InsetWrap(BufferParams const &, std::string const &);
	///
	~InsetWrap();
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	void read(Buffer const & buf, Lexer & lex);
	///
	void validate(LaTeXFeatures & features) const;
	///
	InsetBase::Code lyxCode() const { return InsetBase::WRAP_CODE; }
	///
	int latex(Buffer const &, odocstream &,
	          OutputParams const &) const;
	///
	int plaintext(Buffer const &, odocstream &,
	              OutputParams const &) const;
	///
	int docbook(Buffer const &, odocstream &,
	            OutputParams const &) const;
	///
	virtual docstring const editMessage() const;
	///
	bool insetAllowed(InsetBase::Code) const;
	///
	void addToToc(TocList &, Buffer const &) const;
	///
	bool showInsetDialog(BufferView *) const;
	///
	InsetWrapParams const & params() const { return params_; }
	///
	bool getStatus(Cursor &, FuncRequest const &, FuncStatus &) const;
protected:
	///
	virtual void doDispatch(Cursor & cur, FuncRequest & cmd);
private:
	virtual std::auto_ptr<InsetBase> doClone() const;

	///
	InsetWrapParams params_;
};


class InsetWrapMailer : public MailInset {
public:
	///
	InsetWrapMailer(InsetWrap & inset);
	///
	virtual InsetBase & inset() const { return inset_; }
	///
	virtual std::string const & name() const { return name_; }
	///
	virtual std::string const inset2string(Buffer const &) const;
	///
	static void string2params(std::string const &, InsetWrapParams &);
	///
	static std::string const params2string(InsetWrapParams const &);
private:
	///
	static std::string const name_;
	///
	InsetWrap & inset_;
};


} // namespace lyx

#endif
