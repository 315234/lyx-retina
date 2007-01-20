// -*- C++ -*-
/**
 * \file metricsinfo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef METRICSINFO_H
#define METRICSINFO_H

#include "lyxfont.h"
#include "support/docstring.h"
#include <string>

class BufferView;

namespace lyx {

namespace frontend { class Painter; }


/// Standard Sizes (mode styles)
enum Styles {
	///
	LM_ST_DISPLAY = 0,
	///
	LM_ST_TEXT,
	///
	LM_ST_SCRIPT,
	///
	LM_ST_SCRIPTSCRIPT
};


//
// This is the part common to MetricsInfo and PainterInfo
//
class MetricsBase {
public:
	///
	MetricsBase();
	///
	MetricsBase(BufferView * bv, LyXFont const & font, int textwidth);

	/// the current view
	BufferView * bv;
	/// current font
	LyXFont font;
	/// current math style (display/text/script/..)
	Styles style;
	/// name of current font - mathed specific
	std::string fontname;
	/// This is the width available in pixels
	int textwidth;
};


//
// This contains a MetricsBase and information that's only relevant during
// the first phase of the two-phase draw
//
class MetricsInfo {
public:
	///
	MetricsInfo();
	///
	MetricsInfo(BufferView * bv, LyXFont const & font, int textwidth);

	///
	MetricsBase base;
};


//
// This contains a MetricsBase and information that's only relevant during
// the second phase of the two-phase draw
//
class PainterInfo {
public:
	///
	PainterInfo(BufferView * bv, frontend::Painter & pain);
	///
	void draw(int x, int y, char_type c);
	///
	void draw(int x, int y, docstring const & str);

	///
	MetricsBase base;
	///
	frontend::Painter & pain;
	/// Whether the text at this point is right-to-left (for InsetNewline)
	bool ltr_pos;
	/// Whether the parent is deleted (change tracking)
	bool erased_;
};

class TextMetricsInfo {};

enum ScreenUpdateStrategy {
	NoScreenUpdate,
	SingleParUpdate,
	FullScreenUpdate,
	DecorationUpdate
};

class ViewMetricsInfo
{
public:
	ViewMetricsInfo()
			: p1(0), p2(0), y1(0), y2(0),
			update_strategy(FullScreenUpdate), size(0)
		{}
	ViewMetricsInfo(pit_type p1, pit_type p2, int y1, int y2,
			ScreenUpdateStrategy updatestrategy, pit_type size)
			: p1(p1), p2(p2), y1(y1), y2(y2),
			update_strategy(updatestrategy), size(size)
		{}

	pit_type p1;
	pit_type p2;
	int y1;
	int y2;
	ScreenUpdateStrategy update_strategy;
	pit_type size;
};


// Generic base for temporarily changing things.
// The original state gets restored when the Changer is destructed.

template <class Struct, class Temp = Struct>
class Changer {
public:
	///
	Changer(Struct & orig) : orig_(orig) {}
protected:
	///
	Struct & orig_;
	///
	Temp save_;
};



// temporarily change some aspect of a font
class FontChanger : public Changer<LyXFont> {
public:
	///
	FontChanger(LyXFont & orig, docstring const & font);
	FontChanger(MetricsBase & mb, char const * const font);
	///
	~FontChanger();
};


// temporarily change a full font
class FontSetChanger : public Changer<MetricsBase> {
public:
	///
	FontSetChanger(MetricsBase & mb, docstring const & font);
	FontSetChanger(MetricsBase & mb, char const * const font);
	///
	~FontSetChanger();
};


// temporarily change the style
class StyleChanger : public Changer<MetricsBase> {
public:
	///
	StyleChanger(MetricsBase & mb, Styles style);
	///
	~StyleChanger();
};


// temporarily change the style to script style
class ScriptChanger : public StyleChanger {
public:
	///
	ScriptChanger(MetricsBase & mb);
};


// temporarily change the style suitable for use in fractions
class FracChanger : public StyleChanger {
public:
	///
	FracChanger(MetricsBase & mb);
};


// temporarily change the style suitable for use in tabulars and arrays
class ArrayChanger : public StyleChanger {
public:
	///
	ArrayChanger(MetricsBase & mb);
};



// temporarily change the shape of a font
class ShapeChanger : public Changer<LyXFont, LyXFont::FONT_SHAPE> {
public:
	///
	ShapeChanger(LyXFont & font, LyXFont::FONT_SHAPE shape);
	///
	~ShapeChanger();
};


// temporarily change the available text width
class WidthChanger : public Changer<MetricsBase>
{
public:
	///
	WidthChanger(MetricsBase & mb, int width);
	///
	~WidthChanger();
};


// temporarily change the used color
class ColorChanger : public Changer<LyXFont, std::string> {
public:
	///
	ColorChanger(LyXFont & font, std::string const & color);
	///
	~ColorChanger();
};

} // namespace lyx

#endif
