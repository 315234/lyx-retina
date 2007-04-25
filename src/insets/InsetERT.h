// -*- C++ -*-
/**
 * \file InsetERT.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETERT_H
#define INSETERT_H

#include "InsetCollapsable.h"
#include "MailInset.h"


namespace lyx {

/** A collapsable text inset for LaTeX insertions.

  To write full ert (including styles and other insets) in a given
  space.

  Note that collapsed_ encompasses both the inline and collapsed button
  versions of this inset.
*/

class Language;

class InsetERT : public InsetCollapsable {
public:
	///
	InsetERT(BufferParams const &, CollapseStatus status = Open);
#if 0
	///
	InsetERT(BufferParams const &,
		 Language const *, std::string const & contents, CollapseStatus status);
#endif
	///
	~InsetERT();
	///
	InsetBase::Code lyxCode() const { return InsetBase::ERT_CODE; }
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	void read(Buffer const & buf, LyXLex & lex);
	///
	virtual docstring const editMessage() const;
	///
	bool insetAllowed(InsetBase::Code code) const;
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
	void validate(LaTeXFeatures &) const {}
	///
	bool metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	bool showInsetDialog(BufferView *) const;
	///
	void getDrawFont(LyXFont &) const;
	///
	bool forceDefaultParagraphs(idx_type) const { return true; }
	/// should paragraph indendation be ommitted in any case?
	bool neverIndent(Buffer const &) const { return true; }
protected:
	InsetERT(InsetERT const &);
	///
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);
	///
	bool getStatus(LCursor & cur, FuncRequest const & cmd, FuncStatus &) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	void init();
	///
	void setButtonLabel();
	///
	bool allowSpellCheck() const { return false; }
};


class InsetERTMailer : public MailInset {
public:
	///
	InsetERTMailer(InsetERT & inset);
	///
	virtual InsetBase & inset() const { return inset_; }
	///
	virtual std::string const & name() const { return name_; }
	///
	virtual std::string const inset2string(Buffer const &) const;
	///
	static void string2params(std::string const &,
		InsetCollapsable::CollapseStatus &);
	///
	static std::string const params2string(InsetCollapsable::CollapseStatus);
private:
	///
	static std::string const name_;
	///
	InsetERT & inset_;
};


} // namespace lyx

#endif
