// -*- C++ -*-
/**
 * \file insetparent.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSET_PARENT_H
#define INSET_PARENT_H


#include "insetcommand.h"

class Buffer;

/** Reference to the parent document.

  Useful to load a parent document from a child document and to
  share parent's properties between preambleless children.
 */
class InsetParent : public InsetCommand {
public:
	///
	InsetParent(InsetCommandParams const &, Buffer const &, bool same_id = false);
	///
	virtual Inset * clone(Buffer const & buffer, bool same_id = false) const {
		return new InsetParent(params(), buffer, same_id);
	}
	///
	dispatch_result localDispatch(FuncRequest const & cmd);
	///
	string const getScreenLabel(Buffer const *) const;
	///
	EDITABLE editable() const { return IS_EDITABLE; }
	///
	Inset::Code lyxCode() const { return Inset::PARENT_CODE; }
	///
	int latex(Buffer const *, std::ostream &, LatexRunParams const &,
		  bool fragile, bool free_spc) const;
	///
	void setParent(string const & fn) { setContents(fn); }
};
#endif
