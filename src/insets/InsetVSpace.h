// -*- C++ -*-
/**
 * \file InsetVSpace.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_VSPACE_H
#define INSET_VSPACE_H


#include "Inset.h"
#include "VSpace.h"
#include "MailInset.h"


namespace lyx {

class InsetVSpace : public Inset {
public:
	///
	InsetVSpace() {}
	///
	InsetVSpace(VSpace const &);
	///
	~InsetVSpace();
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
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
	void read(Buffer const &, Lexer & lex);
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	DisplayType display() const { return AlignCenter; }
	/// How much?
	VSpace const & space() const { return space_; }

protected:
	///
	virtual void doDispatch(Cursor & cur, FuncRequest & cmd);

private:
	virtual std::auto_ptr<Inset> doClone() const;
	///
	docstring const label() const;

	///
	VSpace space_;
};


class InsetVSpaceMailer : public MailInset {
public:
	///
	InsetVSpaceMailer(InsetVSpace & inset);
	///
	virtual Inset & inset() const { return inset_; }
	///
	virtual std::string const & name() const { return name_; }
	///
	virtual std::string const inset2string(Buffer const &) const;
	///
	static void string2params(std::string const &, VSpace &);
	///
	static std::string const params2string(VSpace const &);
private:
	///
	static std::string const name_;
	///
	InsetVSpace & inset_;
};


} // namespace lyx

#endif
