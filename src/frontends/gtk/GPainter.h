// -*- C++ -*-
/**
 * \file GPainter.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Huang Ying
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GPAINTER_H
#define GPAINTER_H

#include "frontends/Painter.h"

#include <gtkmm.h>

#include <X11/Xft/Xft.h>

#include <map>

class LyXFont;

namespace lyx {
namespace frontend {

class GWorkArea;
class ColorHandler;

/**
 * GPainter - a painter implementation for Gtkmm
 */
class GPainter : public Painter {
public:
	GPainter(GWorkArea &);

	/// return the width of the work area in pixels
	virtual int paperWidth() const;
	/// return the height of the work area in pixels
	virtual int paperHeight() const;

	inline void setForeground(LColor_color clr);
	inline void setLineParam(line_style ls, line_width lw);
	XftColor * getXftColor(LColor_color clr);
	/// draw a line from point to point
	virtual void line(
		int x1, int y1,
		int x2, int y2,
		LColor_color,
		line_style = line_solid,
		line_width = line_thin);

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
		LColor_color,
		line_style = line_solid,
		line_width = line_thin);

	/// draw a rectangle
	virtual void rectangle(
		int x, int y,
		int w, int h,
		LColor_color,
		line_style = line_solid,
		line_width = line_thin);

	/// draw a filled rectangle
	virtual void fillRectangle(
		int x, int y,
		int w, int h,
		LColor_color);

	/// draw an arc
	virtual void arc(
		int x, int y,
		unsigned int w, unsigned int h,
		int a1, int a2,
		LColor_color);

	/// draw a pixel
	virtual void point(
		int x, int y,
		LColor_color);

	/// draw an image from the image cache
	virtual void image(int x, int y,
		int w, int h,
		graphics::Image const & image);

	/// draw a string at position x, y (y is the baseline)
	virtual void text(int x, int y,
		std::string const & str, LyXFont const & f);

	/// draw a string at position x, y (y is the baseline)
	virtual void text(int x, int y,
		char const * str, size_t l,
		LyXFont const & f);

	/// draw a char at position x, y (y is the baseline)
	virtual void text(int x, int y,
			       char c, LyXFont const & f);

	void start();
	
	

private:
	/// our owner who we paint upon
	GWorkArea & owner_;
	
	Glib::RefPtr<Gdk::GC> gc_;
	Glib::RefPtr<Gdk::Pixmap> pixmap_;
	ColorHandler *colorhandler_;
	LColor_color currentcolor_;
};

} // namespace frontend
} // namespace lyx

#endif // XPAINTER_H
