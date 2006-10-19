// -*- C++ -*-
/**
 * \file insetvspace.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_VSPACE_H
#define INSET_VSPACE_H


#include "inset.h"
#include "vspace.h"
#include "mailinset.h"

class InsetVSpace : public InsetOld {
public:
	///
	InsetVSpace() {}
	///
	InsetVSpace(VSpace const &);
	///
	~InsetVSpace();
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	int latex(Buffer const &, lyx::odocstream &,
		  OutputParams const &) const;
	///
	int plaintext(Buffer const &, lyx::odocstream &,
		      OutputParams const &) const;
	///
	int docbook(Buffer const &, lyx::odocstream &,
		    OutputParams const &) const;
	///
	void read(Buffer const &, LyXLex & lex);
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	bool display() const { return true; }
	/// How much?
	VSpace const & space() const { return space_; }

protected:
	///
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	std::string const label() const;

	///
	VSpace space_;
};


class InsetVSpaceMailer : public MailInset {
public:
	///
	InsetVSpaceMailer(InsetVSpace & inset);
	///
	virtual InsetBase & inset() const { return inset_; }
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

#endif
