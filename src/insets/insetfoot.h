// -*- C++ -*-
/**
 * \file insetfoot.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSETFOOT_H
#define INSETFOOT_H


#include "LaTeXFeatures.h"
#include "insetfootlike.h"

/** The footnote inset

*/
class InsetFoot : public InsetFootlike {
public:
	///
	InsetFoot(BufferParams const &);
	///
	InsetFoot(InsetFoot const &);
	///
	//InsetFoot(InsetFoot const &, bool same_id);
	///
	Inset * clone(Buffer const &) const;
	///
	//Inset * clone(Buffer const &, bool same_id) const;
	///
	Inset::Code lyxCode() const { return Inset::FOOT_CODE; }
	///
	int latex(Buffer const *, std::ostream &,
		  LatexRunParams const &) const;
	///
	int docbook(Buffer const *, std::ostream &, bool mixcont) const;
	///
	string const editMessage() const;
};

#endif
