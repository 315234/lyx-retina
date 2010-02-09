/**
 * \file GuiApplication.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUIAPPLICATION_H
#define GUIAPPLICATION_H

#include "frontends/Application.h"

#include <QApplication>
#include <QList>

class QAbstractItemModel;
class QIcon;
class QObject;
class QSessionManager;
class QSortFilterProxyModel;

namespace lyx {

class BufferView;
class ColorCache;
class KeySymbol;

namespace frontend {

class GuiView;
class LyXView;
class GlobalMenuBar;
class GuiWorkArea;
class Menus;
class SocketNotifier;
class Toolbars;

/// The Qt main application class
/**
There should be only one instance of this class. No Qt object
initialisation should be done before the instanciation of this class.
*/
class GuiApplication : public QApplication, public Application
{
	Q_OBJECT

public:
	GuiApplication(int & argc, char ** argv);
	~GuiApplication();

	/// Clear all session information.
	void clearSession();

	/// \name Methods inherited from Application class
	//@{
	void dispatch(FuncRequest const &);
	void dispatch(FuncRequest const &, DispatchResult & dr);
	FuncStatus getStatus(FuncRequest const & cmd) const;
	LyXView * currentWindow();
	void dispatchDelayed(FuncRequest const &);
	void resetGui();
	void restoreGuiSession();
	Clipboard & clipboard();
	Selection & selection();
	FontLoader & fontLoader();
	int exec();
	void exit(int status);
	bool event(QEvent * e);
	bool getRgbColor(ColorCode col, RGBColor & rgbcol);
	std::string const hexName(ColorCode col);
	void registerSocketCallback(int fd, SocketCallback func);
	void unregisterSocketCallback(int fd);
	bool searchMenu(FuncRequest const & func, docstring_list & names) const;
	docstring iconName(FuncRequest const & f, bool unknown);
	void hideDialogs(std::string const & name, Inset * inset) const;
	Buffer const * updateInset(Inset const * inset) const;
	void handleKeyFunc(FuncCode action);
	//@}

	Toolbars const & toolbars() const;
	Toolbars & toolbars();
	Menus const & menus() const;
	Menus & menus();

	/// \name Methods inherited from QApplication class
	//@{
	bool notify(QObject * receiver, QEvent * event);
	void commitData(QSessionManager & sm);
#ifdef Q_WS_X11
	bool x11EventFilter(XEvent * ev);
#endif
	//@}

	/// Create the main window with given geometry settings.
	/// \param geometry_arg: only for Windows platform.
	/// \param optional id identifier.
	void createView(QString const & geometry_arg = QString(),
		bool autoShow = true, int id = 0);
	/// FIXME: this method and the one above are quite ugly.
	void createView(int id);
	///
	GuiView const * currentView() const { return current_view_; }
	///
	GuiView * currentView() { return current_view_; }
	///
	void setCurrentView(GuiView * view) { current_view_ = view; }
	///
	QList<int> viewIds() const;

	///
	ColorCache & colorCache();

	QAbstractItemModel * languageModel();

	/// return a suitable serif font name.
	QString const romanFontName();

	/// return a suitable sans serif font name.
	QString const sansFontName();

	/// return a suitable monospaced font name.
	QString const typewriterFontName();
	///
	void unregisterView(GuiView * gv);
	///
	GuiView & view(int id) const;
	///
	void processKeySym(KeySymbol const & key, KeyModifier state);
	/// return the status bar state string
	docstring viewStatusMessage();

	/// goto a bookmark
	/// openFile: whether or not open a file if the file is not opened
	/// switchToBuffer: whether or not switch to buffer if the buffer is
	///		not the current buffer
	void gotoBookmark(unsigned int idx, bool openFile, bool switchToBuffer);

private Q_SLOTS:
	///
	void execBatchCommands();
	///
	void socketDataReceived(int fd);
	/// events to be triggered by general_timer_ should go here
	void handleRegularEvents();
	///
	void onLastWindowClosed();

	///
	void processFuncRequestQueue();

private:
	///
	bool closeAllViews();
	/// read the given ui (menu/toolbar) file
	bool readUIFile(QString const & name, bool include = false);
	///
	void setGuiLanguage();

	/// This LyXView is the one receiving Clipboard and Selection
	/// events
	GuiView * current_view_;

	///
	struct Private;
	Private * const d;
}; // GuiApplication

extern GuiApplication * guiApp;

/// \return the icon file name for the given action.
QString iconName(FuncRequest const & f, bool unknown);


/// \return the pixmap for the given path, name and extension.
QPixmap getPixmap(QString const & path, QString const & name, QString const & ext);

/// \return an icon for the given action.
QIcon getIcon(FuncRequest const & f, bool unknown);

GuiApplication * theGuiApp();

} // namespace frontend
} // namespace lyx

#endif // GUIAPPLICATION_H
