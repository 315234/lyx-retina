// -*- C++ -*-
/**
 * \file InsetFoot.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETFOOT_H
#define INSETFOOT_H

#include "InsetFootlike.h"


namespace lyx {

/** The footnote inset

*/
class InsetFoot : public InsetFootlike {
public:
	///
	InsetFoot(BufferParams const &);
	///
	InsetCode lyxCode() const { return FOOT_CODE; }
	///
	docstring name() const { return from_ascii("Foot"); }
	///
	int latex(odocstream &, OutputParams const &) const;
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring editMessage() const;
	/// Update the counters of this inset and of its contents
	void updateLabels(ParIterator const &);
	///
	void addToToc(ParConstIterator const &) const;

protected:
	InsetFoot(InsetFoot const &);
private:
	Inset * clone() const;
};


} // namespace lyx

#endif
