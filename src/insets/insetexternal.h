// -*- C++ -*-
/**
 * \file insetexternal.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup Nielsen
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSET_EXTERNAL_H
#define INSET_EXTERNAL_H

#include "insetbutton.h"
#include "ExternalTemplate.h"
#include "LString.h"

///
class InsetExternal : public InsetButton {
public:
	/// hold parameters settable from the GUI
	struct Params {
		Params(string const & f = string(),
					string const & p = string(),
					ExternalTemplate const & t = ExternalTemplate())
			: filename(f), parameters(p), templ(t) {}
		/// the filename
		string filename;
		/// the parameters of the current choice
		string parameters;
		/// the current template used
		ExternalTemplate templ;
	};

	InsetExternal();
	///
	virtual ~InsetExternal();
	///
	virtual dispatch_result localDispatch(FuncRequest const & cmd);	
	/// what appears in the minibuffer when opening
	virtual string const editMessage() const;
	///
	virtual EDITABLE editable() const { return IS_EDITABLE; }
	///
	virtual void write(Buffer const *, std::ostream &) const;
	///
	virtual void read(Buffer const *, LyXLex & lex);

	/** returns the number of rows (\n's) of generated tex code.
	 fragile == true means, that the inset should take care about
	 fragile commands by adding a \protect before.
	 If the free_spc (freespacing) variable is set, then this inset
	 is in a free-spacing paragraph.
	 */
	virtual int latex(Buffer const *, std::ostream &, LatexRunParams const &,
			  bool fragile, bool free_spc) const;
	/// write ASCII output to the ostream
	virtual int ascii(Buffer const *, std::ostream &, int linelen) const;
	/// write LinuxDoc output to the ostream
	virtual int linuxdoc(Buffer const *, std::ostream &) const;
	/// write DocBook output to the ostream
	virtual int docbook(Buffer const *, std::ostream &, bool mixcont) const;

	/// Updates needed features for this inset.
	virtual void validate(LaTeXFeatures & features) const;

	/// returns LyX code associated with the inset. Used for TOC, ...)
	virtual Inset::Code lyxCode() const { return EXTERNAL_CODE; }

	///
	virtual Inset * clone(Buffer const &, bool same_id = false) const;

	/// returns the text of the button
	virtual string const getScreenLabel(Buffer const *) const;

	// The following public members are used from the frontends code

	/// set the parameters from a Params structure
	virtual void setFromParams(Params const &);

	///
	void updateExternal() const;

	/// update the file represented by the template
	void updateExternal(string const &, Buffer const *) const;

	/// edit file of this template
	void editExternal() const;

	/// view file of this template
	void viewExternal() const;

	/// return a copy of our current params
	Params const & params() const;

	///
	void setView(BufferView * bv) { view_ = bv; }

private:
	/// Write the output for a specific file format
	int write(string const & format, Buffer const *,
		  std::ostream &) const;

	/// Execute this command in the directory of this document
	void executeCommand(string const & s, Buffer const * buf) const;

	/// Substitute meta-variables in this string
	string const doSubstitution(Buffer const *, string const & s) const;

	/// our owning view
	BufferView * view_;

	/// the current params
	Params params_;

	/// A temp filename
	string tempname_;
};


#include "mailinset.h"

class InsetExternalMailer : public MailInset {
public:
	///
	InsetExternalMailer(InsetExternal & inset);
	///
	virtual InsetBase & inset() const { return inset_; }
	///
	virtual string const & name() const { return name_; }
	///
	virtual string const inset2string() const;
	///
	static void string2params(string const &, InsetExternal::Params &);
	///
	static string const params2string(InsetExternal::Params const &);
private:
	///
	static string const name_;
	///
	InsetExternal & inset_;
};

#endif
