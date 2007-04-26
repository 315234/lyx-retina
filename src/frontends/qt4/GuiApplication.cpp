/**
 * \file qt4/GuiApplication.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiApplication.h"

#include "qt_helpers.h"
#include "QLImage.h"
#include "socket_callback.h"

#include "frontends/LyXView.h"

#include "graphics/LoaderQueue.h"

#include "support/lstrings.h"
#include "support/os.h"
#include "support/Package.h"

#include "BufferView.h"
#include "Color.h"
#include "debug.h"
#include "FuncRequest.h"
#include "LyX.h"
#include "LyXFunc.h"
#include "LyXRC.h"

#include <QApplication>
#include <QClipboard>
#include <QEventLoop>
#include <QFileOpenEvent>
#include <QLocale>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QTimer>
#include <QTranslator>
#include <QWidget>

#ifdef Q_WS_X11
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#endif

#include <boost/bind.hpp>

#include <exception>

using std::string;
using std::endl;

///////////////////////////////////////////////////////////////
// You can find other X11 specific stuff
// at the end of this file...
///////////////////////////////////////////////////////////////

namespace {

int getDPI()
{
	QWidget w;
	return int(0.5 * (w.logicalDpiX() + w.logicalDpiY()));
}

} // namespace anon


namespace lyx {

frontend::Application * createApplication(int & argc, char * argv[])
{
	return new frontend::GuiApplication(argc, argv);
}


namespace frontend {

GuiApplication * guiApp;


GuiApplication::~GuiApplication()
{
	socket_callbacks_.clear();
}


GuiApplication::GuiApplication(int & argc, char ** argv)
	: QApplication(argc, argv), Application(argc, argv)
{
	// Qt bug? setQuitOnLastWindowClosed(true); does not work
	setQuitOnLastWindowClosed(false);

#ifdef Q_WS_X11
	// doubleClickInterval() is 400 ms on X11 which is just too long.
	// On Windows and Mac OS X, the operating system's value is used.
	// On Microsoft Windows, calling this function sets the double
	// click interval for all applications. So we don't!
	QApplication::setDoubleClickInterval(300);
#endif

	// install translation file for Qt built-in dialogs
	QString language_name = QString("qt_") + QLocale::system().name();
	language_name.truncate(5);
	if (qt_trans_.load(language_name,
		QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
	{
		qApp->installTranslator(&qt_trans_);
		// even if the language calls for RtL, don't do that
		qApp->setLayoutDirection(Qt::LeftToRight);
		LYXERR(Debug::GUI)
			<< "Successfully installed Qt translations for locale "
			<< fromqstr(language_name) << std::endl;
	} else
		LYXERR(Debug::GUI)
			<< "Could not find  Qt translations for locale "
			<< fromqstr(language_name) << std::endl;

	using namespace lyx::graphics;

	Image::newImage = boost::bind(&QLImage::newImage);
	Image::loadableFormats = boost::bind(&QLImage::loadableFormats);

	// needs to be done before reading lyxrc
	lyxrc.dpi = getDPI();

	LoaderQueue::setPriority(10,100);

	guiApp = this;
}


Clipboard& GuiApplication::clipboard()
{
	return clipboard_;
}


Selection& GuiApplication::selection()
{
	return selection_;
}


int const GuiApplication::exec()
{
	QTimer::singleShot(1, this, SLOT(execBatchCommands()));
	return QApplication::exec();
}


void GuiApplication::exit(int status)
{
	QApplication::exit(status);
}


void GuiApplication::execBatchCommands()
{
	LyX::ref().execBatchCommands();
}


string const GuiApplication::romanFontName()
{
	QFont font;
	font.setKerning(false);
	font.setStyleHint(QFont::Serif);
	font.setFamily("serif");

	return fromqstr(QFontInfo(font).family());
}


string const GuiApplication::sansFontName()
{
	QFont font;
	font.setKerning(false);
	font.setStyleHint(QFont::SansSerif);
	font.setFamily("sans");

	return fromqstr(QFontInfo(font).family());
}


string const GuiApplication::typewriterFontName()
{
	QFont font;
	font.setKerning(false);
	font.setStyleHint(QFont::TypeWriter);
	font.setFamily("monospace");

	return fromqstr(QFontInfo(font).family());
}


bool GuiApplication::event(QEvent * e)
{
	switch(e->type()) {
	case QEvent::FileOpen: {
		// Open a file; this happens only on Mac OS X for now
		QFileOpenEvent * foe = static_cast<QFileOpenEvent *>(e);
		lyx::dispatch(FuncRequest(LFUN_FILE_OPEN,
					  fromqstr(foe->file())));
		return true;
	}
	default:
		return QApplication::event(e);
	}
}


bool GuiApplication::notify(QObject * receiver, QEvent * event)
{
	bool return_value;
	try {
		return_value = QApplication::notify(receiver, event);
	}
	catch (std::exception  const & e) {
		lyxerr << "Caught \"normal\" exception: " << e.what() << endl;
		LyX::cref().emergencyCleanup();
		abort();
	}
	catch (...) {
		lyxerr << "Caught some really weird exception..." << endl;
		LyX::cref().emergencyCleanup();
		abort();
	}

	return return_value;
}


void GuiApplication::syncEvents()
{
	// This is the ONLY place where processEvents may be called.
	// During screen update/ redraw, this method is disabled to
	// prevent keyboard events being handed to the LyX core, where
	// they could cause re-entrant calls to screen update.
	processEvents(QEventLoop::ExcludeUserInputEvents);
}


bool GuiApplication::getRgbColor(Color_color col,
	RGBColor & rgbcol)
{
	QColor const & qcol = color_cache_.get(col);
	if (!qcol.isValid()) {
		rgbcol.r = 0;
		rgbcol.g = 0;
		rgbcol.b = 0;
		return false;
	}
	rgbcol.r = qcol.red();
	rgbcol.g = qcol.green();
	rgbcol.b = qcol.blue();
	return true;
}


string const GuiApplication::hexName(Color_color col)
{
	return lyx::support::ltrim(fromqstr(color_cache_.get(col).name()), "#");
}


void GuiApplication::updateColor(Color_color)
{
	// FIXME: Bleh, can't we just clear them all at once ?
	color_cache_.clear();
}


void GuiApplication::registerSocketCallback(int fd, boost::function<void()> func)
{
	socket_callbacks_[fd] =
		boost::shared_ptr<socket_callback>(new socket_callback(fd, func));
}


void GuiApplication::unregisterSocketCallback(int fd)
{
	socket_callbacks_.erase(fd);
}

////////////////////////////////////////////////////////////////////////
// X11 specific stuff goes here...
#ifdef Q_WS_X11
bool GuiApplication::x11EventFilter(XEvent * xev)
{
	if (!currentView())
		return false;

	switch (xev->type) {
	case SelectionRequest: {
		if (xev->xselectionrequest.selection != XA_PRIMARY)
			break;
		LYXERR(Debug::GUI) << "X requested selection." << endl;
		BufferView * bv = currentView()->view();
		if (bv) {
			docstring const sel = bv->requestSelection();
			if (!sel.empty())
				selection_.put(sel);
		}
		break;
	}
	case SelectionClear: {
		if (xev->xselectionclear.selection != XA_PRIMARY)
			break;
		LYXERR(Debug::GUI) << "Lost selection." << endl;
		BufferView * bv = currentView()->view();
		if (bv)
			bv->clearSelection();
		break;
	}
	}
	return false;
}
#endif


} // namespace frontend
} // namespace lyx

#include "GuiApplication_moc.cpp"
