// -*- C++ -*-
/**
 * \file rowpainter.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author various
 * \author John Levon
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef ROWPAINTER_H
#define ROWPAINTER_H

#include "support/types.h"

namespace lyx {

class Bidi;
class BufferView;
class Font;
class PainterInfo;
class Paragraph;
class ParagraphList;
class ParagraphMetrics;
class Row;
class Text;
class TextMetrics;
class ViewMetricsInfo;

namespace frontend { class Painter; }

/**
 * A class used for painting an individual row of text.
 * FIXME: get rid of that class.
 */
class RowPainter {
public:
	/// initialise and run painter
	RowPainter(PainterInfo & pi, Text const & text,
		pit_type pit, Row const & row, Bidi & bidi, int x, int y);

	/// paint various parts
	/// FIXME: transfer to TextMetrics
	void paintAppendix();
	void paintDepthBar();
	void paintChangeBar();
	void paintFirst();
	void paintLast();
	void paintText();

private:
	void paintForeignMark(double orig_x, Font const & font, int desc = 0);
	void paintHebrewComposeChar(pos_type & vpos, Font const & font);
	void paintArabicComposeChar(pos_type & vpos, Font const & font);
	void paintChars(pos_type & vpos, Font const & font,
			bool hebrew, bool arabic);
	int paintAppendixStart(int y);
	void paintFromPos(pos_type & vpos);
	void paintInset(pos_type const pos, Font const & font);

	/// return left margin
	int leftMargin() const;

	/// return the label font for this row
	Font const getLabelFont() const;

	/// bufferview to paint on
	BufferView & bv_;

	/// Painter to use
	frontend::Painter & pain_;

	/// Text for the row
	Text const & text_;
	TextMetrics & text_metrics_;
	ParagraphList const & pars_;

	/// The row to paint
	Row const & row_;

	/// Row's paragraph
	pit_type const pit_;
	Paragraph const & par_;
	ParagraphMetrics const & pm_;

	/// bidi cache, comes from outside the rowpainter because
	/// rowpainters are normally created in a for loop and there only
	/// one of them is active at a time.
	Bidi & bidi_;

	/// is row erased? (change tracking)
	bool erased_;

	// Looks ugly - is
	double const xo_;
	int const yo_;    // current baseline
	double x_;
	int width_;
	double separator_;
	double hfill_;
	double label_hfill_;
};

/// some space for drawing the 'nested' markers (in pixel)
inline int nestMargin() { return 15; }

/// margin for changebar
inline int changebarMargin() { return 12; }

/// right margin
inline int rightMargin() { return 10; }

} // namespace lyx

#endif // ROWPAINTER_H
