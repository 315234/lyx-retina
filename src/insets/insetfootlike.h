// -*- C++ -*-
/**
 * \file insetfootlike.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETFOOTLIKE_H
#define INSETFOOTLIKE_H

#include "insetcollapsable.h"

// To have this class is probably a bit overkill... (Lgb)

// The footnote inset
class InsetFootlike : public InsetCollapsable {
public:
	///
	InsetFootlike(BufferParams const &);
	///
	InsetFootlike(InsetFootlike const &);
	///
	void write(Buffer const * buf, std::ostream & os) const;
	///
	bool insetAllowed(InsetOld::Code) const;
	/** returns true if, when outputing LaTeX, font changes should
	    be closed before generating this inset. This is needed for
	    insets that may contain several paragraphs */
	bool noFontChange() const { return true; }
};

#endif
