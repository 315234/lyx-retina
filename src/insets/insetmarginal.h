// -*- C++ -*-
/**
 * \file insetmarginal.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSETMARGINAL_H
#define INSETMARGINAL_H


#include "insetfootlike.h"

/** The marginal note inset

*/
class InsetMarginal : public InsetFootlike {
public:
	///
	InsetMarginal(BufferParams const &);
	///
	InsetMarginal(InsetMarginal const &, bool same_id = false);
	///
	Inset * clone(Buffer const &, bool same_id = false) const;
	///
	Inset::Code lyxCode() const { return Inset::MARGIN_CODE; }
	///
	int latex(Buffer const *, std::ostream &, bool fragile, bool fp) const;
	///
	string const editMessage() const;
};

#endif
