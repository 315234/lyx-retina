// -*- C++ -*-
/* This file is part of
 * ======================================================
 *
 *           LyX, The Document Processor
 *
 *           Copyright 1995-2001 the LyX Team.
 *
 * ====================================================== */

#ifndef CUTANDPASTE_H
#define CUTANDPASTE_H

#include "support/types.h"

class Paragraph;
class BufferParams;
class LyXTextClass;

///
class CutAndPaste {
public:
	/// realcut == false is we actually want a delete
	static
	bool cutSelection(Paragraph * startpar, Paragraph ** endpar,
			  int start, int & end, char tc, bool doclear = false,
			  bool realcut = true);
	///
	static
	bool copySelection(Paragraph * startpar, Paragraph * endpar,
			   int start, int end, char tc);
	///
	static
	bool pasteSelection(Paragraph ** par, Paragraph ** endpar,
			    int & pos, char tc);
	///
	static
	int nrOfParagraphs();
	/** needed to switch between different classes this works
	    for a list of paragraphs beginning with the specified par
	    return value is the number of wrong conversions
	*/
	static
	int SwitchLayoutsBetweenClasses(lyx::textclass_type c1,
					lyx::textclass_type c2,
					Paragraph * par,
					BufferParams const & bparams);
	///
	static
	bool checkPastePossible(Paragraph *);
};

#endif
