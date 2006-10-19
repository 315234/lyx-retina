// -*- C++ -*-
/**
 * \file TextPainter.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TEXTPAINTER_H
#define TEXTPAINTER_H


#include "support/docstream.h"

#include <vector>

// FIXME: Abdel 16/10/2006
// This TextPainter class is never used, this is dead code.
/* Georg explanation of current situation:
AFAIK the text painter was used to export math formulas as ASCII art.
The text painter is	like a real painter, but operating on a very coarse
grid of character cells	where each cell can be filled with an ASCII character.
I don't know why it is currently disabled. I do know that we have a bugzilla
request for reenabling it.
*/

class TextPainter {
	public:
		///
		TextPainter(int xmax, int ymax);
		///
		void draw(int x, int y, lyx::char_type const * str);
		///
		void draw(int x, int y, lyx::char_type c);
		///
		void show(lyx::odocstream & os, int offset = 0) const;
		///
		int textheight() const { return ymax_; }
		///
		void horizontalLine(int x, int y, int len, lyx::char_type c = '-');
		///
		void verticalLine(int x, int y, int len, lyx::char_type c = '|');

	private:
		///
		typedef std::vector<lyx::char_type> data_type;
		///
		lyx::char_type at(int x, int y) const;
		///
		lyx::char_type & at(int x, int y);

		/// xsize of the painter area
		int xmax_;
		/// ysize of the painter area
		int ymax_;
		/// the image
		data_type data_;
};

#endif
