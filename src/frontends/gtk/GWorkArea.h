// -*- C++ -*-
/**
 * \file GWorkArea.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Huang Ying
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GWORKAREA_H
#define GWORKAREA_H

#include "GPainter.h"

#include <boost/shared_ptr.hpp>

#include <gtkmm.h>
#include <gtk/gtk.h>

#include <X11/Xft/Xft.h>

// FIXME: defined in X.h, spuriously pulled in by some gui headers
#undef CursorShape

class LColor_color;
class LyXView;

namespace lyx {
namespace frontend {

class ColorCache {
public:
	typedef std::map<LColor_color, boost::shared_ptr<Gdk::Color> > Map;
	typedef Map::iterator MapIt;
	typedef std::map<LColor_color, boost::shared_ptr<XftColor> > Map2;
	typedef Map2::iterator MapIt2;
	Gdk::Color * getColor(LColor_color);
	XftColor * getXftColor(LColor_color);
	void cacheColor(LColor_color, Gdk::Color *);
	void cacheXftColor(LColor_color, XftColor *);
	void clear();
private:
	Map cache_;
	Map2 cache2_;
};

extern ColorCache colorCache;

class ColorHandler {
public:
	ColorHandler(GWorkArea & owner) : owner_(owner) {}
	XftColor * getXftColor(LColor_color clr);
	Gdk::Color * getGdkColor(LColor_color clr);
private:
	GWorkArea & owner_;
};


class GWorkArea : public sigc::trackable {
public:
	GWorkArea(LyXView & owner, int width, int height);
	virtual ~GWorkArea();

	virtual Painter & getPainter();
	///
	virtual int workWidth() const;
	///
	virtual int workHeight() const;
	/// return x position of window
	int xpos() const;
	/// return y position of window
	int ypos() const;
	///
	Glib::RefPtr<Gdk::Window> getWindow();
	Display * getDisplay() const;
	Glib::RefPtr<Gdk::Pixmap> getPixmap();
	Glib::RefPtr<Gdk::GC> getGC();
	Glib::RefPtr<Gdk::Colormap> getColormap();
	XftDraw * getXftDraw();
	ColorHandler & getColorHandler();

	virtual void setScrollbarParams(int height, int pos, int line_height);
	void inputCommit(gchar * str);

	LyXView & view()
	{
		return view_;
	}

private:
	bool onExpose(GdkEventExpose * event);
	bool onConfigure(GdkEventConfigure * event);
	void onScroll();
	bool onScrollWheel(GdkEventScroll * event);
	bool onButtonPress(GdkEventButton * event);
	bool onButtonRelease(GdkEventButton * event);
	bool onMotionNotify(GdkEventMotion * event);
	bool onKeyPress(GdkEventKey * event);
	LyXView & view_;
	Gtk::HBox hbox_;
	Gtk::DrawingArea workArea_;
	Gtk::VScrollbar vscrollbar_;
	/// The pixmap overlay on the workarea
	Glib::RefPtr<Gdk::Pixmap> workAreaPixmap_;
	Glib::RefPtr<Gdk::GC> workAreaGC_;
	GPainter painter_;
	XftDraw * draw_;
	ColorHandler colorHandler_;
	GtkIMContext * imContext_;
	std::string inputCache_;
	bool adjusting_;
};

} // namespace frontend
} // namespace lyx

#endif
