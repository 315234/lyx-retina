// -*- C++ -*-
/**
 * \file inseterror.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSET_ERROR_H
#define INSET_ERROR_H


#include "inset.h"
#include "LString.h"

/** Used for error messages from LaTeX runs.

  The edit-operation opens a
  dialog with the text of the error-message. The inset is displayed as
  "Error" in a box, and automatically deleted. */
class InsetError : public Inset {
public:
	///
	explicit
	InsetError(string const &, bool same_id = false);
	///
	~InsetError();
	///
	virtual dispatch_result localDispatch(FuncRequest const & cmd);
	///
	int ascent(BufferView *, LyXFont const &) const;
	///
	int descent(BufferView *, LyXFont const &) const;
	///
	int width(BufferView *, LyXFont const &) const;
	///
	void draw(BufferView *, LyXFont const &, int, float &) const;
	///
	void write(Buffer const *, std::ostream &) const {}
	///
	void read(Buffer const *, LyXLex &) {}
	///
	int latex(Buffer const *, std::ostream &, bool, bool) const { return 0; }
	///
	int ascii(Buffer const *, std::ostream &, int) const { return 0; }
	///
	int linuxdoc(Buffer const *, std::ostream &) const { return 0; }
	///
	int docbook(Buffer const *, std::ostream &, bool) const { return 0; }
	///
	bool autoDelete() const { return true; }
	/// what appears in the minibuffer when opening
	string const editMessage() const;
	///
	void edit(BufferView *, int, int, mouse_button::state);
	///
	void edit(BufferView * bv, bool front = true);
	///
	EDITABLE editable() const { return IS_EDITABLE; }
	///
	Inset * clone(Buffer const &, bool same_id = false) const {
		return new InsetError(contents, same_id);
	}
	///
	Inset::Code lyxCode() const { return Inset::ERROR_CODE; }
	/// We don't want "begin" and "end inset" in lyx-file
	bool directWrite() const { return true; };
	///
	string const & getContents() const { return contents; }
private:
	///
	string contents;
};
#endif
