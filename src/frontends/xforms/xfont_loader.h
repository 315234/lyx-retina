// -*- C++ -*-
/**
 * \file xfont_loader.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef FONTLOADER_H_
#define FONTLOADER_H_


#include <X11/Xlib.h>
#include "lyxfont.h"
#include "support/std_string.h"

class FontInfo;

/** This class takes care of loading fonts. It uses FontInfo to make
intelligent guesses about matching font size, and it tries different tags
itself in order to match the font loading demands.  Later, I plan to extend
this with support for T1Lib, probably via a new class building on this.
(Asger) */
class xfont_loader {
public:
	///
	xfont_loader();

	///
	~xfont_loader();

	/// Update fonts after zoom, dpi, font names, or norm change
	void update();

	/// Load font
	XFontStruct * load(LyXFont::FONT_FAMILY family,
			   LyXFont::FONT_SERIES series,
			   LyXFont::FONT_SHAPE shape,
			   LyXFont::FONT_SIZE size) {
		if (fontstruct[family][series][shape][size] != 0)
			return fontstruct[family][series][shape][size];
		else
			return doLoad(family, series, shape, size);
	};
	/// Do we have anything matching?
	bool available(LyXFont const & f);

private:
	/// Array of font structs
	XFontStruct * fontstruct[LyXFont::NUM_FAMILIES][2][4][10];

	/// Array of font infos
	FontInfo * fontinfo[LyXFont::NUM_FAMILIES][2][4];

	/// Reset font handler
	void reset();

	/// Unload all fonts
	void unload();

	/// Get font info
	void getFontinfo(LyXFont::FONT_FAMILY family,
			 LyXFont::FONT_SERIES series,
			 LyXFont::FONT_SHAPE shape);

	/** Does the actual loading of a font. Updates fontstruct. */
	XFontStruct * doLoad(LyXFont::FONT_FAMILY family,
			     LyXFont::FONT_SERIES series,
			     LyXFont::FONT_SHAPE shape,
			     LyXFont::FONT_SIZE size);
};

///
extern xfont_loader fontloader;

#endif
