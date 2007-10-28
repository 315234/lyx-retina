// -*- C++ -*-
/**
 * \file Painter.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef PAINTER_H
#define PAINTER_H

#include "ColorCode.h"

#include "support/docstring.h"

namespace lyx {

class FontInfo;

namespace graphics { class Image; }

namespace frontend {

/**
 * Painter - A painter class to encapsulate all graphics parameters and operations
 *
 * Every graphics operation in LyX should be made by this class. The
 * painter is used for drawing on the WorkArea, and is passed around
 * during draw operations.
 *
 * It hides low level windows system parameters so insets and other
 * clients don't have to worry about them and we can control graphics and
 * GUI toolkit dependent drawing functions inside this single class.
 *
 * The intention for a toolkit is that it uses these methods to paint
 * onto a backing pixmap. Only when expose events arrive via the event
 * queue (perhaps generated via Screen::expose), does the copy onto
 * the actual WorkArea widget take place. Paints are wrapped in (possibly
 * recursive) calls to start() and end() to facilitate the backing pixmap
 * management.
 *
 * Note that the methods return *this for convenience.
 *
 * Caution: All char_type and docstring arguments of the text drawing
 * methods of this class are no UCS4 chars or strings if the font is a
 * symbol font. They simply denote the code points of the font instead.
 * You have to keep this in mind when you implement the methods in a
 * frontend. You must not pass these parameters to a unicode conversion
 * function in particular.
 */
class Painter {
public:
	Painter(): drawing_enabled_(true) {}
	/// possible line widths
	enum line_width {
		line_thin, //< thin line
		line_thick //< thick line
	};

	/// possible line styles
	enum line_style {
		line_solid, //< solid line
		line_onoffdash //< dashes with spaces
	};

	/// possible character styles of preedit string.
	/// This is used for CJK input method support.
	enum preedit_style {
		preedit_default, //< when unselecting, no cursor and dashed underline.
		preedit_selecting, //< when selecting.
		preedit_cursor //< with cursor.
	};

	virtual ~Painter() {}

	/// draw a line from point to point
	virtual void line(
		int x1, int y1,
		int x2, int y2,
		ColorCode,
		line_style = line_solid,
		line_width = line_thin) = 0;

	/**
	 * lines -  draw a set of lines
	 * @param xp array of points' x co-ords
	 * @param yp array of points' y co-ords
	 * @param np size of the points array
	 */
	virtual void lines(
		int const * xp,
		int const * yp,
		int np,
		ColorCode,
		line_style = line_solid,
		line_width = line_thin) = 0;

	/// draw a rectangle
	virtual void rectangle(
		int x, int y,
		int w, int h,
		ColorCode,
		line_style = line_solid,
		line_width = line_thin) = 0;

	/// draw a filled rectangle
	virtual void fillRectangle(
		int x, int y,
		int w, int h,
		ColorCode) = 0;

	/// draw an arc
	virtual void arc(
		int x, int y,
		unsigned int w, unsigned int h,
		int a1, int a2,
		ColorCode) = 0;

	/// draw a pixel
	virtual void point(
		int x, int y,
		ColorCode) = 0;

	/// draw a filled rectangle with the shape of a 3D button
	virtual void button(int x, int y,
		int w, int h, bool mouseHover);

	/// draw an image from the image cache
	virtual void image(int x, int y,
		int w, int h,
		graphics::Image const & image) = 0;

	/// draw a string at position x, y (y is the baseline)
	/**
	* \return the width of the drawn text.
	*/
	virtual int text(int x, int y,
		docstring const & str, FontInfo const & f) = 0;

	void setDrawingEnabled(bool drawing_enabled = true)
	{ drawing_enabled_ = drawing_enabled; }

	/// draw a char at position x, y (y is the baseline)
	/**
	* \return the width of the drawn text.
	*/
	virtual int text(int x, int y, char_type c, FontInfo const & f) = 0;

	/**
	 * Draw a string and enclose it inside a rectangle. If
	 * back color is specified, the background is cleared with
	 * the given color. If frame is specified, a thin frame is drawn
	 * around the text with the given color.
	 */
	void rectText(int x, int baseline,
		docstring const & str,
		FontInfo const & font,
		ColorCode back,
		ColorCode frame);

	/// draw a string and enclose it inside a button frame
	void buttonText(int x, int baseline, docstring const & s,
		FontInfo const & font, bool mouseHover);

	/// draw a character of a preedit string for cjk support.
	int preeditText(int x, int y,
		char_type c, FontInfo const & f, preedit_style style);

protected:
	/// check the font, and if set, draw an underline
	void underline(FontInfo const & f,
		int x, int y, int width);

	/// check the font, and if set, draw an dashed underline
	void dashedUnderline(FontInfo const & f,
		int x, int y, int width);

	/// draw a bevelled button border
	void buttonFrame(int x, int y, int w, int h);

	/// Indicate wether real screen drawing shall be done or not.
	bool isDrawingEnabled() const { return drawing_enabled_; }

private:
	///
	bool drawing_enabled_;
};

} // namespace frontend
} // namespace lyx

#endif // PAINTER_H
