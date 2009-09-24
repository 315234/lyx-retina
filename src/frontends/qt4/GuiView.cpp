/**
 * \file GuiView.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author John Levon
 * \author Abdelrazak Younes
 * \author Peter Kümmel
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "GuiView.h"

#include "Dialog.h"
#include "FileDialog.h"
#include "FontLoader.h"
#include "GuiApplication.h"
#include "GuiCommandBuffer.h"
#include "GuiCompleter.h"
#include "GuiWorkArea.h"
#include "GuiKeySymbol.h"
#include "GuiToc.h"
#include "GuiToolbar.h"
#include "LayoutBox.h"
#include "Menus.h"
#include "TocModel.h"

#include "qt_helpers.h"

#include "frontends/alert.h"

#include "buffer_funcs.h"
#include "Buffer.h"
#include "BufferList.h"
#include "BufferParams.h"
#include "BufferView.h"
#include "Converter.h"
#include "Cursor.h"
#include "CutAndPaste.h"
#include "Encoding.h"
#include "ErrorList.h"
#include "Format.h"
#include "FuncStatus.h"
#include "FuncRequest.h"
#include "Intl.h"
#include "Layout.h"
#include "Lexer.h"
#include "LyXAction.h"
#include "LyXFunc.h"
#include "LyX.h"
#include "LyXRC.h"
#include "LyXVC.h"
#include "Paragraph.h"
#include "SpellChecker.h"
#include "TextClass.h"
#include "Text.h"
#include "Toolbars.h"
#include "version.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/ExceptionMessage.h"
#include "support/FileName.h"
#include "support/filetools.h"
#include "support/gettext.h"
#include "support/filetools.h"
#include "support/ForkedCalls.h"
#include "support/lassert.h"
#include "support/lstrings.h"
#include "support/os.h"
#include "support/Package.h"
#include "support/Path.h"
#include "support/Systemcall.h"
#include "support/Timeout.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QPoint>
#include <QPushButton>
#include <QSettings>
#include <QShowEvent>
#include <QSplitter>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QUrl>
#include <QScrollBar>

#include <boost/bind.hpp>

#include <sstream>

#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

using namespace std;
using namespace lyx::support;

namespace lyx {
namespace frontend {

namespace {

class BackgroundWidget : public QWidget
{
public:
	BackgroundWidget()
	{
		LYXERR(Debug::GUI, "show banner: " << lyxrc.show_banner);
		/// The text to be written on top of the pixmap
		QString const text = lyx_version ?
			qt_("version ") + lyx_version : qt_("unknown version");
		splash_ = getPixmap("images/", "banner", "png");

		QPainter pain(&splash_);
		pain.setPen(QColor(0, 0, 0));
		QFont font;
		// The font used to display the version info
		font.setStyleHint(QFont::SansSerif);
		font.setWeight(QFont::Bold);
		font.setPointSize(int(toqstr(lyxrc.font_sizes[FONT_SIZE_LARGE]).toDouble()));
		pain.setFont(font);
		pain.drawText(260, 15, text);
	}

	void paintEvent(QPaintEvent *)
	{
		int x = (width() - splash_.width()) / 2;
		int y = (height() - splash_.height()) / 2;
		QPainter pain(this);
		pain.drawPixmap(x, y, splash_);
	}

private:
	QPixmap splash_;
};


/// Toolbar store providing access to individual toolbars by name.
typedef std::map<std::string, GuiToolbar *> ToolbarMap;

typedef boost::shared_ptr<Dialog> DialogPtr;

} // namespace anon


struct GuiView::GuiViewPrivate
{
	GuiViewPrivate()
		: current_work_area_(0), current_main_work_area_(0),
		layout_(0), autosave_timeout_(5000),
		in_show_(false)
	{
		// hardcode here the platform specific icon size
		smallIconSize = 14;  // scaling problems
		normalIconSize = 20; // ok, default
		bigIconSize = 26;    // better for some math icons

		splitter_ = new QSplitter;
		bg_widget_ = new BackgroundWidget;
		stack_widget_ = new QStackedWidget;
		stack_widget_->addWidget(bg_widget_);
		stack_widget_->addWidget(splitter_);
		setBackground();
	}

	~GuiViewPrivate()
	{
		delete splitter_;
		delete bg_widget_;
		delete stack_widget_;
	}

	QMenu * toolBarPopup(GuiView * parent)
	{
		// FIXME: translation
		QMenu * menu = new QMenu(parent);
		QActionGroup * iconSizeGroup = new QActionGroup(parent);

		QAction * smallIcons = new QAction(iconSizeGroup);
		smallIcons->setText(qt_("Small-sized icons"));
		smallIcons->setCheckable(true);
		QObject::connect(smallIcons, SIGNAL(triggered()),
			parent, SLOT(smallSizedIcons()));
		menu->addAction(smallIcons);

		QAction * normalIcons = new QAction(iconSizeGroup);
		normalIcons->setText(qt_("Normal-sized icons"));
		normalIcons->setCheckable(true);
		QObject::connect(normalIcons, SIGNAL(triggered()),
			parent, SLOT(normalSizedIcons()));
		menu->addAction(normalIcons);

		QAction * bigIcons = new QAction(iconSizeGroup);
		bigIcons->setText(qt_("Big-sized icons"));
		bigIcons->setCheckable(true);
		QObject::connect(bigIcons, SIGNAL(triggered()),
			parent, SLOT(bigSizedIcons()));
		menu->addAction(bigIcons);

		unsigned int cur = parent->iconSize().width();
		if ( cur == parent->d.smallIconSize)
			smallIcons->setChecked(true);
		else if (cur == parent->d.normalIconSize)
			normalIcons->setChecked(true);
		else if (cur == parent->d.bigIconSize)
			bigIcons->setChecked(true);

		return menu;
	}

	void setBackground()
	{
		stack_widget_->setCurrentWidget(bg_widget_);
		bg_widget_->setUpdatesEnabled(true);
	}

	TabWorkArea * tabWorkArea(int i)
	{
		return dynamic_cast<TabWorkArea *>(splitter_->widget(i));
	}

	TabWorkArea * currentTabWorkArea()
	{
		if (splitter_->count() == 1)
			// The first TabWorkArea is always the first one, if any.
			return tabWorkArea(0);

		for (int i = 0; i != splitter_->count(); ++i) {
			TabWorkArea * twa = tabWorkArea(i);
			if (current_main_work_area_ == twa->currentWorkArea())
				return twa;
		}

		// None has the focus so we just take the first one.
		return tabWorkArea(0);
	}

public:
	GuiWorkArea * current_work_area_;
	GuiWorkArea * current_main_work_area_;
	QSplitter * splitter_;
	QStackedWidget * stack_widget_;
	BackgroundWidget * bg_widget_;
	/// view's toolbars
	ToolbarMap toolbars_;
	/// The main layout box.
	/** 
	 * \warning Don't Delete! The layout box is actually owned by
	 * whichever toolbar contains it. All the GuiView class needs is a
	 * means of accessing it.
	 *
	 * FIXME: replace that with a proper model so that we are not limited
	 * to only one dialog.
	 */
	LayoutBox * layout_;

	///
	map<string, Inset *> open_insets_;

	///
	map<string, DialogPtr> dialogs_;

	unsigned int smallIconSize;
	unsigned int normalIconSize;
	unsigned int bigIconSize;
	///
	QTimer statusbar_timer_;
	/// auto-saving of buffers
	Timeout autosave_timeout_;
	/// flag against a race condition due to multiclicks, see bug #1119
	bool in_show_;

	///
	TocModels toc_models_;
};


GuiView::GuiView(int id)
	: d(*new GuiViewPrivate), id_(id), closing_(false)
{
	// GuiToolbars *must* be initialised before the menu bar.
	normalSizedIcons(); // at least on Mac the default is 32 otherwise, which is huge
	constructToolbars();

	// set ourself as the current view. This is needed for the menu bar
	// filling, at least for the static special menu item on Mac. Otherwise
	// they are greyed out.
	guiApp->setCurrentView(this);
	
	// Fill up the menu bar.
	guiApp->menus().fillMenuBar(menuBar(), this, true);

	setCentralWidget(d.stack_widget_);

	// Start autosave timer
	if (lyxrc.autosave) {
		d.autosave_timeout_.timeout.connect(boost::bind(&GuiView::autoSave, this));
		d.autosave_timeout_.setTimeout(lyxrc.autosave * 1000);
		d.autosave_timeout_.start();
	}
	connect(&d.statusbar_timer_, SIGNAL(timeout()),
		this, SLOT(clearMessage()));

	// We don't want to keep the window in memory if it is closed.
	setAttribute(Qt::WA_DeleteOnClose, true);

#if (!defined(Q_WS_WIN) && !defined(Q_WS_MACX))
	// assign an icon to main form. We do not do it under Qt/Win or Qt/Mac,
	// since the icon is provided in the application bundle.
	setWindowIcon(getPixmap("images/", "lyx", "png"));
#endif

	// For Drag&Drop.
	setAcceptDrops(true);

	statusBar()->setSizeGripEnabled(true);

	// Forbid too small unresizable window because it can happen
	// with some window manager under X11.
	setMinimumSize(300, 200);

	if (lyxrc.allow_geometry_session) {
		// Now take care of session management.
		if (restoreLayout())
			return;
	}

	// no session handling, default to a sane size.
	setGeometry(50, 50, 690, 510);
	initToolbars();

	// clear session data if any.
	QSettings settings;
	settings.remove("views");
}


GuiView::~GuiView()
{
	delete &d;
}


void GuiView::saveLayout() const
{
	QSettings settings;
	settings.beginGroup("views");
	settings.beginGroup(QString::number(id_));
#ifdef Q_WS_X11
	settings.setValue("pos", pos());
	settings.setValue("size", size());
#else
	settings.setValue("geometry", saveGeometry());
#endif
	settings.setValue("layout", saveState(0));
	settings.setValue("icon_size", iconSize());
}


bool GuiView::restoreLayout()
{
	QSettings settings;
	settings.beginGroup("views");
	settings.beginGroup(QString::number(id_));
	QString const icon_key = "icon_size";
	if (!settings.contains(icon_key))
		return false;

	//code below is skipped when when ~/.config/LyX is (re)created
	setIconSize(settings.value(icon_key).toSize());
#ifdef Q_WS_X11
	QPoint pos = settings.value("pos", QPoint(50, 50)).toPoint();
	QSize size = settings.value("size", QSize(690, 510)).toSize();
	resize(size);
	move(pos);
#else
	// Work-around for bug #6034: the window ends up in an undetermined
	// state when trying to restore a maximized window when it is
	// already maximized.
	if (!(windowState() & Qt::WindowMaximized))
		if (!restoreGeometry(settings.value("geometry").toByteArray()))
			setGeometry(50, 50, 690, 510);
#endif
	// Make sure layout is correctly oriented.
	setLayoutDirection(qApp->layoutDirection());

	// Allow the toc and view-source dock widget to be restored if needed.
	Dialog * dialog;
	if ((dialog = findOrBuild("toc", true)))
		// see bug 5082. At least setup title and enabled state.
		// Visibility will be adjusted by restoreState below.
		dialog->prepareView();
	if ((dialog = findOrBuild("view-source", true)))
		dialog->prepareView();

	if (!restoreState(settings.value("layout").toByteArray(), 0))
		initToolbars();
	updateDialogs();
	return true;
}


GuiToolbar * GuiView::toolbar(string const & name)
{
	ToolbarMap::iterator it = d.toolbars_.find(name);
	if (it != d.toolbars_.end())
		return it->second;

	LYXERR(Debug::GUI, "Toolbar::display: no toolbar named " << name);
	message(bformat(_("Unknown toolbar \"%1$s\""), from_utf8(name)));
	return 0;
}


void GuiView::constructToolbars()
{
	ToolbarMap::iterator it = d.toolbars_.begin();
	for (; it != d.toolbars_.end(); ++it)
		delete it->second;
	d.toolbars_.clear();

	// I don't like doing this here, but the standard toolbar
	// destroys this object when it's destroyed itself (vfr)
	d.layout_ = new LayoutBox(*this);
	d.stack_widget_->addWidget(d.layout_);
	d.layout_->move(0,0);

	// extracts the toolbars from the backend
	Toolbars::Infos::iterator cit = guiApp->toolbars().begin();
	Toolbars::Infos::iterator end = guiApp->toolbars().end();
	for (; cit != end; ++cit)
		d.toolbars_[cit->name] =  new GuiToolbar(*cit, *this);
}


void GuiView::initToolbars()
{
	// extracts the toolbars from the backend
	Toolbars::Infos::iterator cit = guiApp->toolbars().begin();
	Toolbars::Infos::iterator end = guiApp->toolbars().end();
	for (; cit != end; ++cit) {
		GuiToolbar * tb = toolbar(cit->name);
		if (!tb)
			continue;
		int const visibility = guiApp->toolbars().defaultVisibility(cit->name);
		bool newline = true;
		tb->setVisible(false);
		tb->setVisibility(visibility);

		if (visibility & Toolbars::TOP) {
			if (newline)
				addToolBarBreak(Qt::TopToolBarArea);
			addToolBar(Qt::TopToolBarArea, tb);
		}

		if (visibility & Toolbars::BOTTOM) {
			// Qt < 4.2.2 cannot handle ToolBarBreak on non-TOP dock.
#if (QT_VERSION >= 0x040202)
			addToolBarBreak(Qt::BottomToolBarArea);
#endif
			addToolBar(Qt::BottomToolBarArea, tb);
		}

		if (visibility & Toolbars::LEFT) {
			// Qt < 4.2.2 cannot handle ToolBarBreak on non-TOP dock.
#if (QT_VERSION >= 0x040202)
			addToolBarBreak(Qt::LeftToolBarArea);
#endif
			addToolBar(Qt::LeftToolBarArea, tb);
		}

		if (visibility & Toolbars::RIGHT) {
			// Qt < 4.2.2 cannot handle ToolBarBreak on non-TOP dock.
#if (QT_VERSION >= 0x040202)
			addToolBarBreak(Qt::RightToolBarArea);
#endif
			addToolBar(Qt::RightToolBarArea, tb);
		}

		if (visibility & Toolbars::ON)
			tb->setVisible(true);
	}
}


TocModels & GuiView::tocModels()
{
	return d.toc_models_;
}


void GuiView::setFocus()
{
	LYXERR(Debug::DEBUG, "GuiView::setFocus()" << this);
	// Make sure LyXFunc points to the correct view.
	guiApp->setCurrentView(this);
	QMainWindow::setFocus();
	if (d.current_work_area_)
		d.current_work_area_->setFocus();
}


QMenu * GuiView::createPopupMenu()
{
	return d.toolBarPopup(this);
}


void GuiView::showEvent(QShowEvent * e)
{
	LYXERR(Debug::GUI, "Passed Geometry "
		<< size().height() << "x" << size().width()
		<< "+" << pos().x() << "+" << pos().y());

	if (d.splitter_->count() == 0)
		// No work area, switch to the background widget.
		d.setBackground();

	QMainWindow::showEvent(e);
}


/** Destroy only all tabbed WorkAreas. Destruction of other WorkAreas
 ** is responsibility of the container (e.g., dialog)
 **/
void GuiView::closeEvent(QCloseEvent * close_event)
{
	LYXERR(Debug::DEBUG, "GuiView::closeEvent()");
	closing_ = true;

	writeSession();

	// it can happen that this event arrives without selecting the view,
	// e.g. when clicking the close button on a background window.
	setFocus();
	if (!closeWorkAreaAll()) {
		closing_ = false;
		close_event->ignore();
		return;
	}

	// Make sure that nothing will use this to be closed View.
	guiApp->unregisterView(this);

	if (isFullScreen()) {
		// Switch off fullscreen before closing.
		toggleFullScreen();
		updateDialogs();
	}

	// Make sure the timer time out will not trigger a statusbar update.
	d.statusbar_timer_.stop();

	// Saving fullscreen requires additional tweaks in the toolbar code.
	// It wouldn't also work under linux natively.
	if (lyxrc.allow_geometry_session) {
		// Save this window geometry and layout.
		saveLayout();
		// Then the toolbar private states.
		ToolbarMap::iterator end = d.toolbars_.end();
		for (ToolbarMap::iterator it = d.toolbars_.begin(); it != end; ++it)
			it->second->saveSession();
		// Now take care of all other dialogs:
		map<string, DialogPtr>::const_iterator it = d.dialogs_.begin();
		for (; it!= d.dialogs_.end(); ++it)
			it->second->saveSession();
	}

	close_event->accept();
}


void GuiView::dragEnterEvent(QDragEnterEvent * event)
{
	if (event->mimeData()->hasUrls())
		event->accept();
	/// \todo Ask lyx-devel is this is enough:
	/// if (event->mimeData()->hasFormat("text/plain"))
	///	event->acceptProposedAction();
}


void GuiView::dropEvent(QDropEvent * event)
{
	QList<QUrl> files = event->mimeData()->urls();
	if (files.isEmpty())
		return;

	LYXERR(Debug::GUI, "GuiView::dropEvent: got URLs!");
	for (int i = 0; i != files.size(); ++i) {
		string const file = os::internal_path(fromqstr(
			files.at(i).toLocalFile()));
		if (!file.empty()) {
			// Asynchronously post the event. DropEvent usually come
			// from the BufferView. But reloading a file might close
			// the BufferView from within its own event handler.
			guiApp->dispatchDelayed(FuncRequest(LFUN_FILE_OPEN, file));
			event->accept();
		}
	}
}


void GuiView::message(docstring const & str)
{
	if (ForkedProcess::iAmAChild())
		return;

	statusBar()->showMessage(toqstr(str));
	d.statusbar_timer_.stop();
	d.statusbar_timer_.start(3000);
}


void GuiView::smallSizedIcons()
{
	setIconSize(QSize(d.smallIconSize, d.smallIconSize));
}


void GuiView::normalSizedIcons()
{
	setIconSize(QSize(d.normalIconSize, d.normalIconSize));
}


void GuiView::bigSizedIcons()
{
	setIconSize(QSize(d.bigIconSize, d.bigIconSize));
}


void GuiView::clearMessage()
{
	if (!hasFocus())
		return;
	statusBar()->showMessage(toqstr(theLyXFunc().viewStatusMessage()));
	d.statusbar_timer_.stop();
}


void GuiView::updateWindowTitle(GuiWorkArea * wa)
{
	if (wa != d.current_work_area_)
		return;
	setWindowTitle(qt_("LyX: ") + wa->windowTitle());
	setWindowIconText(wa->windowIconText());
}


void GuiView::on_currentWorkAreaChanged(GuiWorkArea * wa)
{
	disconnectBuffer();
	disconnectBufferView();
	connectBufferView(wa->bufferView());
	connectBuffer(wa->bufferView().buffer());
	d.current_work_area_ = wa;
	QObject::connect(wa, SIGNAL(titleChanged(GuiWorkArea *)),
		this, SLOT(updateWindowTitle(GuiWorkArea *)));
	updateWindowTitle(wa);

	structureChanged();

	// The document settings needs to be reinitialised.
	updateDialog("document", "");

	// Buffer-dependent dialogs must be updated. This is done here because
	// some dialogs require buffer()->text.
	updateDialogs();
}


void GuiView::on_lastWorkAreaRemoved()
{
	if (closing_)
		// We already are in a close event. Nothing more to do.
		return;

	if (d.splitter_->count() > 1)
		// We have a splitter so don't close anything.
		return;

	// Reset and updates the dialogs.
	d.toc_models_.reset(0);
	updateDialog("document", "");
	updateDialogs();

	resetWindowTitleAndIconText();

	if (lyxrc.open_buffers_in_tabs)
		// Nothing more to do, the window should stay open.
		return;

	if (guiApp->viewIds().size() > 1) {
		close();
		return;
	}

#ifdef Q_WS_MACX
	// On Mac we also close the last window because the application stay
	// resident in memory. On other platforms we don't close the last
	// window because this would quit the application.
	close();
#endif
}


void GuiView::updateStatusBar()
{
	// let the user see the explicit message
	if (d.statusbar_timer_.isActive())
		return;

	statusBar()->showMessage(toqstr(theLyXFunc().viewStatusMessage()));
}


bool GuiView::event(QEvent * e)
{
	switch (e->type())
	{
	// Useful debug code:
	//case QEvent::ActivationChange:
	//case QEvent::WindowDeactivate:
	//case QEvent::Paint:
	//case QEvent::Enter:
	//case QEvent::Leave:
	//case QEvent::HoverEnter:
	//case QEvent::HoverLeave:
	//case QEvent::HoverMove:
	//case QEvent::StatusTip:
	//case QEvent::DragEnter:
	//case QEvent::DragLeave:
	//case QEvent::Drop:
	//	break;

	case QEvent::WindowActivate: {
		GuiView * old_view = guiApp->currentView();
		if (this == old_view) {
			setFocus();
			return QMainWindow::event(e);
		}
		if (old_view && old_view->currentBufferView()) {
			// save current selection to the selection buffer to allow
			// middle-button paste in this window.
			cap::saveSelection(old_view->currentBufferView()->cursor());
		}
		guiApp->setCurrentView(this);
		if (d.current_work_area_) {
			BufferView & bv = d.current_work_area_->bufferView();
			connectBufferView(bv);
			connectBuffer(bv.buffer());
			// The document structure, name and dialogs might have
			// changed in another view.
			structureChanged();
			// The document settings needs to be reinitialised.
			updateDialog("document", "");
			updateDialogs();
		} else {
			resetWindowTitleAndIconText();
		}
		setFocus();
		return QMainWindow::event(e);
	}

	case QEvent::ShortcutOverride: {

// See bug 4888
#if (!defined Q_WS_X11) || (QT_VERSION >= 0x040500)
		if (isFullScreen() && menuBar()->isHidden()) {
			QKeyEvent * ke = static_cast<QKeyEvent*>(e);
			// FIXME: we should also try to detect special LyX shortcut such as
			// Alt-P and Alt-M. Right now there is a hack in
			// GuiWorkArea::processKeySym() that hides again the menubar for
			// those cases.
			if (ke->modifiers() & Qt::AltModifier && ke->key() != Qt::Key_Alt) {
				menuBar()->show();
				return QMainWindow::event(e);
			}
		}
#endif

		if (d.current_work_area_)
			// Nothing special to do.
			return QMainWindow::event(e);

		QKeyEvent * ke = static_cast<QKeyEvent*>(e);
		// Let Qt handle menu access and the Tab keys to navigate keys to navigate
		// between controls.
		if (ke->modifiers() & Qt::AltModifier || ke->key() == Qt::Key_Tab 
			|| ke->key() == Qt::Key_Backtab)
			return QMainWindow::event(e);

		// Allow processing of shortcuts that are allowed even when no Buffer
		// is viewed.
		KeySymbol sym;
		setKeySymbol(&sym, ke);
		theLyXFunc().processKeySym(sym, q_key_state(ke->modifiers()));
		e->accept();
		return true;
	}

	default:
		return QMainWindow::event(e);
	}
}

void GuiView::resetWindowTitleAndIconText()
{
    setWindowTitle(qt_("LyX"));
    setWindowIconText(qt_("LyX"));
}

bool GuiView::focusNextPrevChild(bool /*next*/)
{
	setFocus();
	return true;
}


void GuiView::setBusy(bool busy)
{
	if (d.current_work_area_) {
		d.current_work_area_->setUpdatesEnabled(!busy);
		if (busy)
			d.current_work_area_->stopBlinkingCursor();
		else
			d.current_work_area_->startBlinkingCursor();
	}

	if (busy)
		QApplication::setOverrideCursor(Qt::WaitCursor);
	else
		QApplication::restoreOverrideCursor();
}


GuiWorkArea * GuiView::workArea(Buffer & buffer)
{
	if (currentWorkArea()
	    && &currentWorkArea()->bufferView().buffer() == &buffer)
		return (GuiWorkArea *) currentWorkArea();
	if (TabWorkArea * twa = d.currentTabWorkArea())
		return twa->workArea(buffer);
	return 0;
}


GuiWorkArea * GuiView::addWorkArea(Buffer & buffer)
{
	// Automatically create a TabWorkArea if there are none yet.
	TabWorkArea * tab_widget = d.splitter_->count() 
		? d.currentTabWorkArea() : addTabWorkArea();
	return tab_widget->addWorkArea(buffer, *this);
}


TabWorkArea * GuiView::addTabWorkArea()
{
	TabWorkArea * twa = new TabWorkArea;
	QObject::connect(twa, SIGNAL(currentWorkAreaChanged(GuiWorkArea *)),
		this, SLOT(on_currentWorkAreaChanged(GuiWorkArea *)));
	QObject::connect(twa, SIGNAL(lastWorkAreaRemoved()),
			 this, SLOT(on_lastWorkAreaRemoved()));

	d.splitter_->addWidget(twa);
	d.stack_widget_->setCurrentWidget(d.splitter_);
	return twa;
}


GuiWorkArea const * GuiView::currentWorkArea() const
{
	return d.current_work_area_;
}


GuiWorkArea * GuiView::currentWorkArea()
{
	return d.current_work_area_;
}


GuiWorkArea const * GuiView::currentMainWorkArea() const
{
	if (d.currentTabWorkArea() == NULL)
		return NULL;
	return d.currentTabWorkArea()->currentWorkArea();
}


GuiWorkArea * GuiView::currentMainWorkArea()
{
	if (d.currentTabWorkArea() == NULL)
		return NULL;
	return d.currentTabWorkArea()->currentWorkArea();
}


void GuiView::setCurrentWorkArea(GuiWorkArea * wa)
{
	LYXERR(Debug::DEBUG, "Setting current wa: " << wa << endl);
	if (wa == NULL) {
		d.current_work_area_ = NULL;
		d.setBackground();
		return;
	}
	GuiWorkArea * old_gwa = theGuiApp()->currentView()->currentWorkArea();
	if (old_gwa == wa)
		return;

	if (currentBufferView())
		cap::saveSelection(currentBufferView()->cursor());

	theGuiApp()->setCurrentView(this);
	d.current_work_area_ = wa;
	for (int i = 0; i != d.splitter_->count(); ++i) {
		if (d.tabWorkArea(i)->setCurrentWorkArea(wa)) {
			//if (d.current_main_work_area_)
			//	d.current_main_work_area_->setFrameStyle(QFrame::NoFrame);
			d.current_main_work_area_ = wa;
			//d.current_main_work_area_->setFrameStyle(QFrame::Box | QFrame::Plain);
			//d.current_main_work_area_->setLineWidth(2);
			LYXERR(Debug::DEBUG, "Current wa: " << currentWorkArea() << ", Current main wa: " << currentMainWorkArea());
			return;
		}
	}
	LYXERR(Debug::DEBUG, "This is not a tabbed wa");
	on_currentWorkAreaChanged(wa);
	BufferView & bv = wa->bufferView();
	bv.cursor().fixIfBroken();
	bv.updateMetrics();
	wa->setUpdatesEnabled(true);
	LYXERR(Debug::DEBUG, "Current wa: " << currentWorkArea() << ", Current main wa: " << currentMainWorkArea());
}


void GuiView::removeWorkArea(GuiWorkArea * wa)
{
	LASSERT(wa, return);
	if (wa == d.current_work_area_) {
		disconnectBuffer();
		disconnectBufferView();
		d.current_work_area_ = 0;
		d.current_main_work_area_ = 0;
	}

	bool found_twa = false;
	for (int i = 0; i != d.splitter_->count(); ++i) {
		TabWorkArea * twa = d.tabWorkArea(i);
		if (twa->removeWorkArea(wa)) {
			// Found in this tab group, and deleted the GuiWorkArea.
			found_twa = true;
			if (twa->count() != 0) {
				if (d.current_work_area_ == 0)
					// This means that we are closing the current GuiWorkArea, so
					// switch to the next GuiWorkArea in the found TabWorkArea.
					setCurrentWorkArea(twa->currentWorkArea());
			} else {
				// No more WorkAreas in this tab group, so delete it.
				delete twa;
			}
			break;
		}
	}

	// It is not a tabbed work area (i.e., the search work area), so it
	// should be deleted by other means.
	LASSERT(found_twa, /* */);

	if (d.current_work_area_ == 0) {
		if (d.splitter_->count() != 0) {
			TabWorkArea * twa = d.currentTabWorkArea();
			setCurrentWorkArea(twa->currentWorkArea());
		} else {
			// No more work areas, switch to the background widget.
			setCurrentWorkArea(0);
		}
	}
}


LayoutBox * GuiView::getLayoutDialog() const
{
	return d.layout_;
}


void GuiView::updateLayoutList()
{
	if (d.layout_)
		d.layout_->updateContents(false);
}


void GuiView::updateToolbars()
{
	ToolbarMap::iterator end = d.toolbars_.end();
	if (d.current_work_area_) {
		bool const math =
			d.current_work_area_->bufferView().cursor().inMathed();
		bool const table =
			lyx::getStatus(FuncRequest(LFUN_LAYOUT_TABULAR)).enabled();
		bool const review =
			lyx::getStatus(FuncRequest(LFUN_CHANGES_TRACK)).enabled() &&
			lyx::getStatus(FuncRequest(LFUN_CHANGES_TRACK)).onoff(true);
		bool const mathmacrotemplate =
			lyx::getStatus(FuncRequest(LFUN_IN_MATHMACROTEMPLATE)).enabled();

		for (ToolbarMap::iterator it = d.toolbars_.begin(); it != end; ++it)
			it->second->update(math, table, review, mathmacrotemplate);
	} else
		for (ToolbarMap::iterator it = d.toolbars_.begin(); it != end; ++it)
			it->second->update(false, false, false, false);
}


void GuiView::setBuffer(Buffer * newBuffer)
{
	LYXERR(Debug::DEBUG, "Setting buffer: " << newBuffer << std::endl);
	LASSERT(newBuffer, return);
	setBusy(true);

	GuiWorkArea * wa = workArea(*newBuffer);
	if (wa == 0) {
		newBuffer->masterBuffer()->updateLabels();
		wa = addWorkArea(*newBuffer);
	} else {
		//Disconnect the old buffer...there's no new one.
		disconnectBuffer();
	}
	connectBuffer(*newBuffer);
	connectBufferView(wa->bufferView());
	setCurrentWorkArea(wa);

	setBusy(false);
}


void GuiView::connectBuffer(Buffer & buf)
{
	buf.setGuiDelegate(this);
}


void GuiView::disconnectBuffer()
{
	if (d.current_work_area_)
		d.current_work_area_->bufferView().buffer().setGuiDelegate(0);
}


void GuiView::connectBufferView(BufferView & bv)
{
	bv.setGuiDelegate(this);
}


void GuiView::disconnectBufferView()
{
	if (d.current_work_area_)
		d.current_work_area_->bufferView().setGuiDelegate(0);
}


void GuiView::errors(string const & error_type, bool from_master)
{
	ErrorList & el = from_master ? 
		documentBufferView()->buffer().masterBuffer()->errorList(error_type)
		: documentBufferView()->buffer().errorList(error_type);
	string data = error_type;
	if (from_master)
		data = "from_master|" + error_type;
	if (!el.empty())
		showDialog("errorlist", data);
}


void GuiView::updateTocItem(std::string const & type, DocIterator const & dit)
{
	d.toc_models_.updateItem(toqstr(type), dit);
}


void GuiView::structureChanged()
{
	d.toc_models_.reset(documentBufferView());
	// Navigator needs more than a simple update in this case. It needs to be
	// rebuilt.
	updateDialog("toc", "");
}


void GuiView::updateDialog(string const & name, string const & data)
{
	if (!isDialogVisible(name))
		return;

	map<string, DialogPtr>::const_iterator it = d.dialogs_.find(name);
	if (it == d.dialogs_.end())
		return;

	Dialog * const dialog = it->second.get();
	if (dialog->isVisibleView())
		dialog->initialiseParams(data);
}


BufferView * GuiView::documentBufferView()
{
	return currentMainWorkArea()
		? &currentMainWorkArea()->bufferView()
		: 0;
}


BufferView const * GuiView::documentBufferView() const 
{
	return currentMainWorkArea()
		? &currentMainWorkArea()->bufferView()
		: 0;
}


BufferView * GuiView::currentBufferView()
{
	return d.current_work_area_ ? &d.current_work_area_->bufferView() : 0;
}


BufferView const * GuiView::currentBufferView() const
{
	return d.current_work_area_ ? &d.current_work_area_->bufferView() : 0;
}


void GuiView::autoSave()
{
	LYXERR(Debug::INFO, "Running autoSave()");

	if (documentBufferView())
		documentBufferView()->buffer().autoSave();
}


void GuiView::resetAutosaveTimers()
{
	if (lyxrc.autosave)
		d.autosave_timeout_.restart();
}


bool GuiView::getStatus(FuncRequest const & cmd, FuncStatus & flag)
{
	bool enable = true;
	Buffer * buf = currentBufferView()
		? &currentBufferView()->buffer() : 0;
	Buffer * doc_buffer = documentBufferView()
		? &(documentBufferView()->buffer()) : 0;

	// Check whether we need a buffer
	if (!lyxaction.funcHasFlag(cmd.action, LyXAction::NoBuffer) && !buf) {
		// no, exit directly
		flag.message(from_utf8(N_("Command not allowed with"
				    "out any document open")));
		flag.setEnabled(false);
		return true;
	}

	if (cmd.origin == FuncRequest::TOC) {
		GuiToc * toc = static_cast<GuiToc*>(findOrBuild("toc", false));
		FuncStatus fs;
		if (toc->getStatus(documentBufferView()->cursor(), cmd, fs))
			flag |= fs;
		else
			flag.setEnabled(false);
		return true;
	}

	switch(cmd.action) {
	case LFUN_BUFFER_IMPORT:
		break;

	case LFUN_BUFFER_RELOAD:
		enable = doc_buffer && !doc_buffer->isUnnamed()
			&& doc_buffer->fileName().exists()
			&& (!doc_buffer->isClean()
			   || doc_buffer->isExternallyModified(Buffer::timestamp_method));
		break;

	case LFUN_BUFFER_CHILD_OPEN:
		enable = doc_buffer;
		break;

	case LFUN_BUFFER_WRITE:
		enable = doc_buffer && (doc_buffer->isUnnamed() || !doc_buffer->isClean());
		break;

	//FIXME: This LFUN should be moved to GuiApplication.
	case LFUN_BUFFER_WRITE_ALL: {
		// We enable the command only if there are some modified buffers
		Buffer * first = theBufferList().first();
		enable = false;
		if (!first)
			break;
		Buffer * b = first;
		// We cannot use a for loop as the buffer list is a cycle.
		do {
			if (!b->isClean()) {
				enable = true;
				break;
			}
			b = theBufferList().next(b);
		} while (b != first); 
		break;
	}

	case LFUN_BUFFER_WRITE_AS:
		enable = doc_buffer;
		break;

	case LFUN_BUFFER_CLOSE:
		enable = doc_buffer;
		break;

	case LFUN_BUFFER_CLOSE_ALL:
		enable = theBufferList().last() != theBufferList().first();
		break;

	case LFUN_SPLIT_VIEW:
		if (cmd.getArg(0) == "vertical")
			enable = doc_buffer && (d.splitter_->count() == 1 ||
					 d.splitter_->orientation() == Qt::Vertical);
		else
			enable = doc_buffer && (d.splitter_->count() == 1 ||
					 d.splitter_->orientation() == Qt::Horizontal);
		break;

	case LFUN_CLOSE_TAB_GROUP:
		enable = d.currentTabWorkArea();
		break;

	case LFUN_TOOLBAR_TOGGLE:
		if (GuiToolbar * t = toolbar(cmd.getArg(0)))
			flag.setOnOff(t->isVisible());
		break;

	case LFUN_UI_TOGGLE:
		flag.setOnOff(isFullScreen());
		break;

	case LFUN_DIALOG_DISCONNECT_INSET:
		break;

	case LFUN_DIALOG_HIDE:
		// FIXME: should we check if the dialog is shown?
		break;

	case LFUN_DIALOG_TOGGLE:
		flag.setOnOff(isDialogVisible(cmd.getArg(0)));
		// fall through to set "enable"
	case LFUN_DIALOG_SHOW: {
		string const name = cmd.getArg(0);
		if (!doc_buffer)
			enable = name == "aboutlyx"
				|| name == "file" //FIXME: should be removed.
				|| name == "prefs"
				|| name == "texinfo";
		else if (name == "print")
			enable = doc_buffer->isExportable("dvi")
				&& lyxrc.print_command != "none";
		else if (name == "character" || name == "symbols") {
			if (!buf || buf->isReadonly()
				|| !currentBufferView()->cursor().inTexted())
				enable = false;
			else {
				// FIXME we should consider passthru
				// paragraphs too.
				Inset const & in = currentBufferView()->cursor().inset();
				enable = !in.getLayout().isPassThru();
			}
		}
		else if (name == "latexlog")
			enable = FileName(doc_buffer->logName()).isReadableFile();
		else if (name == "spellchecker")
			enable = theSpellChecker() && !doc_buffer->isReadonly();
		else if (name == "vclog")
			enable = doc_buffer->lyxvc().inUse();
		break;
	}

	case LFUN_DIALOG_UPDATE: {
		string const name = cmd.getArg(0);
		if (!buf)
			enable = name == "prefs";
		break;
	}

	case LFUN_COMMAND_EXECUTE:
	case LFUN_MESSAGE:
	case LFUN_MENU_OPEN:
		// Nothing to check.
		break;

	case LFUN_INSET_APPLY: {
		string const name = cmd.getArg(0);
		Inset * inset = getOpenInset(name);
		if (inset) {
			FuncRequest fr(LFUN_INSET_MODIFY, cmd.argument());
			FuncStatus fs;
			if (!inset->getStatus(currentBufferView()->cursor(), fr, fs)) {
				// Every inset is supposed to handle this
				LASSERT(false, break);
			}
			flag |= fs;
		} else {
			FuncRequest fr(LFUN_INSET_INSERT, cmd.argument());
			flag |= lyx::getStatus(fr);
		}
		enable = flag.enabled();
		break;
	}

	case LFUN_COMPLETION_INLINE:
		if (!d.current_work_area_
		    || !d.current_work_area_->completer().inlinePossible(
			currentBufferView()->cursor()))
		    enable = false;
		break;

	case LFUN_COMPLETION_POPUP:
		if (!d.current_work_area_
		    || !d.current_work_area_->completer().popupPossible(
			currentBufferView()->cursor()))
		    enable = false;
		break;

	case LFUN_COMPLETION_COMPLETE:
		if (!d.current_work_area_
			|| !d.current_work_area_->completer().inlinePossible(
			currentBufferView()->cursor()))
		    enable = false;
		break;

	case LFUN_COMPLETION_ACCEPT:
		if (!d.current_work_area_
		    || (!d.current_work_area_->completer().popupVisible()
			&& !d.current_work_area_->completer().inlineVisible()
			&& !d.current_work_area_->completer().completionAvailable()))
			enable = false;
		break;

	case LFUN_COMPLETION_CANCEL:
		if (!d.current_work_area_
		    || (!d.current_work_area_->completer().popupVisible()
			&& !d.current_work_area_->completer().inlineVisible()))
			enable = false;
		break;

	case LFUN_BUFFER_ZOOM_OUT:
		enable = doc_buffer && lyxrc.zoom > 10;
		break;

	case LFUN_BUFFER_ZOOM_IN:
		enable = doc_buffer;
		break;
	
	case LFUN_BUFFER_NEXT:
	case LFUN_BUFFER_PREVIOUS:
		// FIXME: should we check is there is an previous or next buffer?
		break;
	case LFUN_BUFFER_SWITCH:
		// toggle on the current buffer, but do not toggle off
		// the other ones (is that a good idea?)
		if (doc_buffer
			&& to_utf8(cmd.argument()) == doc_buffer->absFileName())
			flag.setOnOff(true);
		break;

	case LFUN_VC_REGISTER:
		enable = doc_buffer && !doc_buffer->lyxvc().inUse();
		break;
	case LFUN_VC_CHECK_IN:
		enable = doc_buffer && doc_buffer->lyxvc().checkInEnabled();
		break;
	case LFUN_VC_CHECK_OUT:
		enable = doc_buffer && doc_buffer->lyxvc().checkOutEnabled();
		break;
	case LFUN_VC_LOCKING_TOGGLE:
		enable = doc_buffer && !doc_buffer->isReadonly()
			&& doc_buffer->lyxvc().lockingToggleEnabled();
		flag.setOnOff(enable && !doc_buffer->lyxvc().locker().empty());
		break;
	case LFUN_VC_REVERT:
		enable = doc_buffer && doc_buffer->lyxvc().inUse();
		break;
	case LFUN_VC_UNDO_LAST:
		enable = doc_buffer && doc_buffer->lyxvc().undoLastEnabled();
		break;
	case LFUN_VC_COMMAND: {
		if (cmd.argument().empty())
			enable = false;
		if (!doc_buffer && contains(cmd.getArg(0), 'D'))
			enable = false;
		break;
	}

	case LFUN_SERVER_GOTO_FILE_ROW:
		break;

	default:
		return false;
	}

	if (!enable)
		flag.setEnabled(false);

	return true;
}


static FileName selectTemplateFile()
{
	FileDialog dlg(qt_("Select template file"));
	dlg.setButton1(qt_("Documents|#o#O"), toqstr(lyxrc.document_path));
	dlg.setButton1(qt_("Templates|#T#t"), toqstr(lyxrc.template_path));

	FileDialog::Result result = dlg.open(toqstr(lyxrc.template_path),
			     QStringList(qt_("LyX Documents (*.lyx)")));

	if (result.first == FileDialog::Later)
		return FileName();
	if (result.second.isEmpty())
		return FileName();
	return FileName(fromqstr(result.second));
}


Buffer * GuiView::loadDocument(FileName const & filename, bool tolastfiles)
{
	setBusy(true);

	Buffer * newBuffer = checkAndLoadLyXFile(filename);

	if (!newBuffer) {
		message(_("Document not loaded."));
		setBusy(false);
		return 0;
	}
	
	setBuffer(newBuffer);

	// scroll to the position when the file was last closed
	if (lyxrc.use_lastfilepos) {
		LastFilePosSection::FilePos filepos =
			theSession().lastFilePos().load(filename);
		documentBufferView()->moveToPosition(filepos.pit, filepos.pos, 0, 0);
	}

	if (tolastfiles)
		theSession().lastFiles().add(filename);

	setBusy(false);
	return newBuffer;
}


void GuiView::openDocument(string const & fname)
{
	string initpath = lyxrc.document_path;

	if (documentBufferView()) {
		string const trypath = documentBufferView()->buffer().filePath();
		// If directory is writeable, use this as default.
		if (FileName(trypath).isDirWritable())
			initpath = trypath;
	}

	string filename;

	if (fname.empty()) {
		FileDialog dlg(qt_("Select document to open"), LFUN_FILE_OPEN);
		dlg.setButton1(qt_("Documents|#o#O"), toqstr(lyxrc.document_path));
		dlg.setButton2(qt_("Examples|#E#e"),
				toqstr(addPath(package().system_support().absFilename(), "examples")));

		QStringList filter(qt_("LyX Documents (*.lyx)"));
		filter << qt_("LyX-1.3.x Documents (*.lyx13)")
			<< qt_("LyX-1.4.x Documents (*.lyx14)")
			<< qt_("LyX-1.5.x Documents (*.lyx15)")
			<< qt_("LyX-1.6.x Documents (*.lyx16)");
		FileDialog::Result result =
			dlg.open(toqstr(initpath), filter);

		if (result.first == FileDialog::Later)
			return;

		filename = fromqstr(result.second);

		// check selected filename
		if (filename.empty()) {
			message(_("Canceled."));
			return;
		}
	} else
		filename = fname;

	// get absolute path of file and add ".lyx" to the filename if
	// necessary. 
	FileName const fullname = 
			fileSearch(string(), filename, "lyx", support::may_not_exist);
	if (!fullname.empty())
		filename = fullname.absFilename();

	if (!fullname.onlyPath().isDirectory()) {
		Alert::warning(_("Invalid filename"),
				bformat(_("The directory in the given path\n%1$s\ndoes not exist."),
				from_utf8(fullname.absFilename())));
		return;
	}
	// if the file doesn't exist, let the user create one
	if (!fullname.exists()) {
		// the user specifically chose this name. Believe him.
		Buffer * const b = newFile(filename, string(), true);
		if (b)
			setBuffer(b);
		return;
	}

	docstring const disp_fn = makeDisplayPath(filename);
	message(bformat(_("Opening document %1$s..."), disp_fn));

	docstring str2;
	Buffer * buf = loadDocument(fullname);
	if (buf) {
		buf->updateLabels();
		setBuffer(buf);
		buf->errors("Parse");
		str2 = bformat(_("Document %1$s opened."), disp_fn);
		if (buf->lyxvc().inUse())
			str2 += " " + from_utf8(buf->lyxvc().versionString()) +
				" " + _("Version control detected.");
	} else {
		str2 = bformat(_("Could not open document %1$s"), disp_fn);
	}
	message(str2);
}

// FIXME: clean that
static bool import(GuiView * lv, FileName const & filename,
	string const & format, ErrorList & errorList)
{
	FileName const lyxfile(support::changeExtension(filename.absFilename(), ".lyx"));

	string loader_format;
	vector<string> loaders = theConverters().loaders();
	if (find(loaders.begin(), loaders.end(), format) == loaders.end()) {
		for (vector<string>::const_iterator it = loaders.begin();
		     it != loaders.end(); ++it) {
			if (!theConverters().isReachable(format, *it))
				continue;

			string const tofile =
				support::changeExtension(filename.absFilename(),
				formats.extension(*it));
			if (!theConverters().convert(0, filename, FileName(tofile),
				filename, format, *it, errorList))
				return false;
			loader_format = *it;
			break;
		}
		if (loader_format.empty()) {
			frontend::Alert::error(_("Couldn't import file"),
				     bformat(_("No information for importing the format %1$s."),
					 formats.prettyName(format)));
			return false;
		}
	} else
		loader_format = format;

	if (loader_format == "lyx") {
		Buffer * buf = lv->loadDocument(lyxfile);
		if (!buf)
			return false;
		buf->updateLabels();
		lv->setBuffer(buf);
		buf->errors("Parse");
	} else {
		Buffer * const b = newFile(lyxfile.absFilename(), string(), true);
		if (!b)
			return false;
		lv->setBuffer(b);
		bool as_paragraphs = loader_format == "textparagraph";
		string filename2 = (loader_format == format) ? filename.absFilename()
			: support::changeExtension(filename.absFilename(),
					  formats.extension(loader_format));
		lv->currentBufferView()->insertPlaintextFile(FileName(filename2),
			as_paragraphs);
		guiApp->setCurrentView(lv);
		lyx::dispatch(FuncRequest(LFUN_MARK_OFF));
	}

	return true;
}


void GuiView::importDocument(string const & argument)
{
	string format;
	string filename = split(argument, format, ' ');

	LYXERR(Debug::INFO, format << " file: " << filename);

	// need user interaction
	if (filename.empty()) {
		string initpath = lyxrc.document_path;
		if (documentBufferView()) {
			string const trypath = documentBufferView()->buffer().filePath();
			// If directory is writeable, use this as default.
			if (FileName(trypath).isDirWritable())
				initpath = trypath;
		}

		docstring const text = bformat(_("Select %1$s file to import"),
			formats.prettyName(format));

		FileDialog dlg(toqstr(text), LFUN_BUFFER_IMPORT);
		dlg.setButton1(qt_("Documents|#o#O"), toqstr(lyxrc.document_path));
		dlg.setButton2(qt_("Examples|#E#e"),
			toqstr(addPath(package().system_support().absFilename(), "examples")));

		docstring filter = formats.prettyName(format);
		filter += " (*.";
		// FIXME UNICODE
		filter += from_utf8(formats.extension(format));
		filter += ')';

		FileDialog::Result result =
			dlg.open(toqstr(initpath), fileFilters(toqstr(filter)));

		if (result.first == FileDialog::Later)
			return;

		filename = fromqstr(result.second);

		// check selected filename
		if (filename.empty())
			message(_("Canceled."));
	}

	if (filename.empty())
		return;

	// get absolute path of file
	FileName const fullname(support::makeAbsPath(filename));

	FileName const lyxfile(support::changeExtension(fullname.absFilename(), ".lyx"));

	// Check if the document already is open
	Buffer * buf = theBufferList().getBuffer(lyxfile);
	if (buf) {
		setBuffer(buf);
		if (!closeBuffer()) {
			message(_("Canceled."));
			return;
		}
	}

	docstring const displaypath = makeDisplayPath(lyxfile.absFilename(), 30);

	// if the file exists already, and we didn't do
	// -i lyx thefile.lyx, warn
	if (lyxfile.exists() && fullname != lyxfile) {

		docstring text = bformat(_("The document %1$s already exists.\n\n"
			"Do you want to overwrite that document?"), displaypath);
		int const ret = Alert::prompt(_("Overwrite document?"),
			text, 0, 1, _("&Overwrite"), _("&Cancel"));

		if (ret == 1) {
			message(_("Canceled."));
			return;
		}
	}

	message(bformat(_("Importing %1$s..."), displaypath));
	ErrorList errorList;
	if (import(this, fullname, format, errorList))
		message(_("imported."));
	else
		message(_("file not imported!"));

	// FIXME (Abdel 12/08/06): Is there a need to display the error list here?
}


void GuiView::newDocument(string const & filename, bool from_template)
{
	FileName initpath(lyxrc.document_path);
	if (documentBufferView()) {
		FileName const trypath(documentBufferView()->buffer().filePath());
		// If directory is writeable, use this as default.
		if (trypath.isDirWritable())
			initpath = trypath;
	}

	string templatefile;
	if (from_template) {
		templatefile = selectTemplateFile().absFilename();
		if (templatefile.empty())
			return;
	}
	
	Buffer * b;
	if (filename.empty())
		b = newUnnamedFile(templatefile, initpath);
	else
		b = newFile(filename, templatefile, true);

	if (b)
		setBuffer(b);

	// If no new document could be created, it is unsure 
	// whether there is a valid BufferView.
	if (currentBufferView())
		// Ensure the cursor is correctly positioned on screen.
		currentBufferView()->showCursor();
}


void GuiView::insertLyXFile(docstring const & fname)
{
	BufferView * bv = documentBufferView();
	if (!bv)
		return;

	// FIXME UNICODE
	FileName filename(to_utf8(fname));
	
	if (!filename.empty()) {
		bv->insertLyXFile(filename);
		return;
	}

	// Launch a file browser
	// FIXME UNICODE
	string initpath = lyxrc.document_path;
	string const trypath = bv->buffer().filePath();
	// If directory is writeable, use this as default.
	if (FileName(trypath).isDirWritable())
		initpath = trypath;

	// FIXME UNICODE
	FileDialog dlg(qt_("Select LyX document to insert"), LFUN_FILE_INSERT);
	dlg.setButton1(qt_("Documents|#o#O"), toqstr(lyxrc.document_path));
	dlg.setButton2(qt_("Examples|#E#e"),
		toqstr(addPath(package().system_support().absFilename(),
		"examples")));

	FileDialog::Result result = dlg.open(toqstr(initpath),
			     QStringList(qt_("LyX Documents (*.lyx)")));

	if (result.first == FileDialog::Later)
		return;

	// FIXME UNICODE
	filename.set(fromqstr(result.second));

	// check selected filename
	if (filename.empty()) {
		// emit message signal.
		message(_("Canceled."));
		return;
	}

	bv->insertLyXFile(filename);
}


void GuiView::insertPlaintextFile(docstring const & fname,
	bool asParagraph)
{
	BufferView * bv = documentBufferView();
	if (!bv)
		return;

	if (!fname.empty() && !FileName::isAbsolute(to_utf8(fname))) {
		message(_("Absolute filename expected."));
		return;
	}

	// FIXME UNICODE
	FileName filename(to_utf8(fname));
	
	if (!filename.empty()) {
		bv->insertPlaintextFile(filename, asParagraph);
		return;
	}

	FileDialog dlg(qt_("Select file to insert"), (asParagraph ?
		LFUN_FILE_INSERT_PLAINTEXT_PARA : LFUN_FILE_INSERT_PLAINTEXT));

	FileDialog::Result result = dlg.open(toqstr(bv->buffer().filePath()),
		QStringList(qt_("All Files (*)")));

	if (result.first == FileDialog::Later)
		return;

	// FIXME UNICODE
	filename.set(fromqstr(result.second));

	// check selected filename
	if (filename.empty()) {
		// emit message signal.
		message(_("Canceled."));
		return;
	}

	bv->insertPlaintextFile(filename, asParagraph);
}


bool GuiView::renameBuffer(Buffer & b, docstring const & newname)
{
	FileName fname = b.fileName();
	FileName const oldname = fname;

	if (!newname.empty()) {
		// FIXME UNICODE
		fname = support::makeAbsPath(to_utf8(newname), oldname.onlyPath().absFilename());
	} else {
		// Switch to this Buffer.
		setBuffer(&b);

		// No argument? Ask user through dialog.
		// FIXME UNICODE
		FileDialog dlg(qt_("Choose a filename to save document as"),
				   LFUN_BUFFER_WRITE_AS);
		dlg.setButton1(qt_("Documents|#o#O"), toqstr(lyxrc.document_path));
		dlg.setButton2(qt_("Templates|#T#t"), toqstr(lyxrc.template_path));

		if (!isLyXFilename(fname.absFilename()))
			fname.changeExtension(".lyx");

		FileDialog::Result result =
			dlg.save(toqstr(fname.onlyPath().absFilename()),
			       QStringList(qt_("LyX Documents (*.lyx)")),
				     toqstr(fname.onlyFileName()));

		if (result.first == FileDialog::Later)
			return false;

		fname.set(fromqstr(result.second));

		if (fname.empty())
			return false;

		if (!isLyXFilename(fname.absFilename()))
			fname.changeExtension(".lyx");
	}

	if (FileName(fname).exists()) {
		docstring const file = makeDisplayPath(fname.absFilename(), 30);
		docstring text = bformat(_("The document %1$s already "
					   "exists.\n\nDo you want to "
					   "overwrite that document?"), 
					 file);
		int const ret = Alert::prompt(_("Overwrite document?"),
			text, 0, 2, _("&Overwrite"), _("&Rename"), _("&Cancel"));
		switch (ret) {
		case 0: break;
		case 1: return renameBuffer(b, docstring());
		case 2: return false;
		}
	}

	FileName oldauto = b.getAutosaveFilename();

	// Ok, change the name of the buffer
	b.setFileName(fname.absFilename());
	b.markDirty();
	bool unnamed = b.isUnnamed();
	b.setUnnamed(false);
	b.saveCheckSum(fname);

	// bring the autosave file with us, just in case.
	b.moveAutosaveFile(oldauto);
	
	if (!saveBuffer(b)) {
		oldauto = b.getAutosaveFilename();
		b.setFileName(oldname.absFilename());
		b.setUnnamed(unnamed);
		b.saveCheckSum(oldname);
		b.moveAutosaveFile(oldauto);
		return false;
	}

	return true;
}


bool GuiView::saveBuffer(Buffer & b)
{
	if (workArea(b) && workArea(b)->inDialogMode())
		return true;

	if (b.isUnnamed())
		return renameBuffer(b, docstring());

	if (b.save()) {
		theSession().lastFiles().add(b.fileName());
		return true;
	}

	// Switch to this Buffer.
	setBuffer(&b);

	// FIXME: we don't tell the user *WHY* the save failed !!
	docstring const file = makeDisplayPath(b.absFileName(), 30);
	docstring text = bformat(_("The document %1$s could not be saved.\n\n"
				   "Do you want to rename the document and "
				   "try again?"), file);
	int const ret = Alert::prompt(_("Rename and save?"),
		text, 0, 2, _("&Rename"), _("&Retry"), _("&Cancel"));
	switch (ret) {
	case 0:
		if (!renameBuffer(b, docstring()))
			return false;
		break;
	case 1:
		break;
	case 2:
		return false;
	}

	return saveBuffer(b);
}


bool GuiView::hideWorkArea(GuiWorkArea * wa)
{
	return closeWorkArea(wa, false);
}


bool GuiView::closeWorkArea(GuiWorkArea * wa)
{
	Buffer & buf = wa->bufferView().buffer();
	return closeWorkArea(wa, !buf.parent());
}


bool GuiView::closeBuffer()
{
	GuiWorkArea * wa = currentMainWorkArea();
	Buffer & buf = wa->bufferView().buffer();
	return wa && closeWorkArea(wa, !buf.parent());
}


void GuiView::writeSession() const {
	GuiWorkArea const * active_wa = currentMainWorkArea();
	for (int i = 0; i < d.splitter_->count(); ++i) {
		TabWorkArea * twa = d.tabWorkArea(i);
		for (int j = 0; j < twa->count(); ++j) {
			GuiWorkArea * wa = static_cast<GuiWorkArea *>(twa->widget(j));
			Buffer & buf = wa->bufferView().buffer();
			theSession().lastOpened().add(buf.fileName(), wa == active_wa);
		}
	}
}


bool GuiView::closeBufferAll()
{
	// Close the workareas in all other views
	QList<int> const ids = guiApp->viewIds();
	for (int i = 0; i != ids.size(); ++i) {
		if (id_ != ids[i] && !guiApp->view(ids[i]).closeWorkAreaAll())
			return false;
	}

	// Close our own workareas
	if (!closeWorkAreaAll())
		return false;

	// Now close the hidden buffers. We prevent hidden buffers from being
	// dirty, so we can just close them.
	theBufferList().closeAll();
	return true;
}


bool GuiView::closeWorkAreaAll()
{
	setCurrentWorkArea(currentMainWorkArea());

	// We might be in a situation that there is still a tabWorkArea, but
	// there are no tabs anymore. This can happen when we get here after a 
	// TabWorkArea::lastWorkAreaRemoved() signal. Therefore we count how
	// many TabWorkArea's have no documents anymore.
	int empty_twa = 0;

	// We have to call count() each time, because it can happen that
	// more than one splitter will disappear in one iteration (bug 5998).
	for (; d.splitter_->count() > empty_twa; ) {
		TabWorkArea * twa = d.tabWorkArea(empty_twa);

		if (twa->count() == 0)
			++empty_twa;
		else {
			setCurrentWorkArea(twa->currentWorkArea());
			if (!closeTabWorkArea(twa))
				return false;
		}
	}
	return true;
}


bool GuiView::closeWorkArea(GuiWorkArea * wa, bool close_buffer)
{
	Buffer & buf = wa->bufferView().buffer();

	// If we are in a close_event all children will be closed in some time,
	// so no need to do it here. This will ensure that the children end up
	// in the session file in the correct order. If we close the master
	// buffer, we can close or release the child buffers here too.
	if (close_buffer && !closing_) {
		vector<Buffer *> clist = buf.getChildren();
		for (vector<Buffer *>::const_iterator it = clist.begin();
			 it != clist.end(); ++it) {
			// If a child is dirty, do not close
			// without user intervention
			//FIXME: should we look in other tabworkareas?
			Buffer * child_buf = *it;
			GuiWorkArea * child_wa = workArea(*child_buf);
			if (child_wa) {
				if (!closeWorkArea(child_wa, true))
					return false;
			} else
				theBufferList().releaseChild(&buf, child_buf);
		}
	}
	// goto bookmark to update bookmark pit.
	//FIXME: we should update only the bookmarks related to this buffer!
	LYXERR(Debug::DEBUG, "GuiView::closeBuffer()");
	for (size_t i = 0; i < theSession().bookmarks().size(); ++i)
		theLyXFunc().gotoBookmark(i+1, false, false);

	// if we are only hiding the buffer and there are multiple views
	// of the buffer, then we do not need to ensure a clean buffer.
	bool const allow_dirty = inMultiTabs(wa) && !close_buffer;

	if (allow_dirty || saveBufferIfNeeded(buf, !close_buffer)) {
		// save in sessions if requested
		// do not save childs if their master
		// is opened as well
		if (!close_buffer)
			removeWorkArea(wa);
		else
			theBufferList().release(&buf);
		return true;
	}
	return false;
}


bool GuiView::closeTabWorkArea(TabWorkArea * twa)
{
	while (twa == d.currentTabWorkArea()) {
		twa->setCurrentIndex(twa->count()-1);

		GuiWorkArea * wa = twa->currentWorkArea();
		Buffer & b = wa->bufferView().buffer();

		// We only want to close the buffer if the same buffer is not visible
		// in another view, and if this is not a child and if we are closing
		// a view (not a tabgroup).
		bool const close_buffer = 
			!inMultiViews(wa) && !b.parent() && closing_;

		if (!closeWorkArea(wa, close_buffer))
			return false;
	}
	return true;
}


bool GuiView::saveBufferIfNeeded(Buffer & buf, bool hiding)
{
	if (buf.isClean() || buf.paragraphs().empty())
		return true;

	// Switch to this Buffer.
	setBuffer(&buf);

	docstring file;
	// FIXME: Unicode?
	if (buf.isUnnamed())
		file = from_utf8(buf.fileName().onlyFileName());
	else
		file = buf.fileName().displayName(30);

	// Bring this window to top before asking questions.
	raise();
	activateWindow();

	int ret;
	if (hiding && buf.isUnnamed()) {
		docstring const text = bformat(_("The document %1$s has not been "
					     "saved yet.\n\nDo you want to save "
					     "the document?"), file);
		ret = Alert::prompt(_("Save new document?"), 
			text, 0, 1, _("&Save"), _("&Cancel"));
		if (ret == 1)
			++ret;
	} else {
		docstring const text = bformat(_("The document %1$s has unsaved changes."
			"\n\nDo you want to save the document or discard the changes?"), file);
		ret = Alert::prompt(_("Save changed document?"),
			text, 0, 2, _("&Save"), _("&Discard"), _("&Cancel"));
	}

	switch (ret) {
	case 0:
		if (!saveBuffer(buf))
			return false;
		break;
	case 1:
		// if we crash after this we could
		// have no autosave file but I guess
		// this is really improbable (Jug)
		buf.removeAutosaveFile();
		if (hiding)
			// revert all changes
			buf.loadLyXFile(buf.fileName());
		buf.markClean();
		break;
	case 2:
		return false;
	}
	return true;
}


bool GuiView::inMultiTabs(GuiWorkArea * wa)
{
	Buffer & buf = wa->bufferView().buffer();

	for (int i = 0; i != d.splitter_->count(); ++i) {
		GuiWorkArea * wa_ = d.tabWorkArea(i)->workArea(buf);
		if (wa_ && wa_ != wa)
			return true;
	}
	return inMultiViews(wa);
}


bool GuiView::inMultiViews(GuiWorkArea * wa)
{
	QList<int> const ids = guiApp->viewIds();
	Buffer & buf = wa->bufferView().buffer();

	int found_twa = 0;
	for (int i = 0; i != ids.size() && found_twa <= 1; ++i) {
		if (id_ == ids[i])
			continue;
		
		if (guiApp->view(ids[i]).workArea(buf))
			return true;
	}
	return false;
}


void GuiView::gotoNextOrPreviousBuffer(NextOrPrevious np)
{
	Buffer * const curbuf = documentBufferView()
		? &documentBufferView()->buffer() : 0;
	Buffer * nextbuf = curbuf;
	while (true) {
		if (np == NEXTBUFFER)
			nextbuf = theBufferList().next(nextbuf);
		else
			nextbuf = theBufferList().previous(nextbuf);
		if (nextbuf == curbuf)
			break;
		if (nextbuf == 0) {
			nextbuf = curbuf;
			break;
		}
		if (workArea(*nextbuf))
			break;
	}
	setBuffer(nextbuf);
}


/// make sure the document is saved
static bool ensureBufferClean(Buffer * buffer)
{
	LASSERT(buffer, return false);
	if (buffer->isClean() && !buffer->isUnnamed())
		return true;

	docstring const file = buffer->fileName().displayName(30);
	docstring title;
	docstring text;
	if (!buffer->isUnnamed()) {
		text = bformat(_("The document %1$s has unsaved "
					     "changes.\n\nDo you want to save "
					     "the document?"), file);
		title = _("Save changed document?");
		
	} else {
		text = bformat(_("The document %1$s has not been "
					     "saved yet.\n\nDo you want to save "
					     "the document?"), file);
		title = _("Save new document?");
	}
	int const ret = Alert::prompt(title, text, 0, 1, _("&Save"), _("&Cancel"));

	if (ret == 0)
		dispatch(FuncRequest(LFUN_BUFFER_WRITE));

	return buffer->isClean() && !buffer->isUnnamed();
}


void GuiView::reloadBuffer()
{
	Buffer * buf = &documentBufferView()->buffer();
	FileName filename = buf->fileName();
	// The user has already confirmed that the changes, if any, should
	// be discarded. So we just release the Buffer and don't call closeBuffer();
	theBufferList().release(buf);
	buf = loadDocument(filename);
	docstring const disp_fn = makeDisplayPath(filename.absFilename());
	docstring str;
	if (buf) {
		buf->updateLabels();
		setBuffer(buf);
		buf->errors("Parse");
		str = bformat(_("Document %1$s reloaded."), disp_fn);
	} else {
		str = bformat(_("Could not reload document %1$s"), disp_fn);
	}
	message(str);
}


void GuiView::dispatchVC(FuncRequest const & cmd)
{
	Buffer * buffer = documentBufferView()
		? &(documentBufferView()->buffer()) : 0;

	switch (cmd.action) {
	case LFUN_VC_REGISTER:
		if (!buffer || !ensureBufferClean(buffer))
			break;
		if (!buffer->lyxvc().inUse()) {
			if (buffer->lyxvc().registrer())
				reloadBuffer();
		}
		break;

	case LFUN_VC_CHECK_IN:
		if (!buffer || !ensureBufferClean(buffer))
			break;
		if (buffer->lyxvc().inUse() && !buffer->isReadonly()) {
			message(from_utf8(buffer->lyxvc().checkIn()));
			reloadBuffer();
		}
		break;

	case LFUN_VC_CHECK_OUT:
		if (!buffer || !ensureBufferClean(buffer))
			break;
		if (buffer->lyxvc().inUse()) {
			message(from_utf8(buffer->lyxvc().checkOut()));
			reloadBuffer();
		}
		break;

	case LFUN_VC_LOCKING_TOGGLE:
		LASSERT(buffer, return);
		if (!ensureBufferClean(buffer) || buffer->isReadonly())
			break;
		if (buffer->lyxvc().inUse()) {
			string res = buffer->lyxvc().lockingToggle();
			if (res.empty()) {
				frontend::Alert::error(_("Revision control error."),
				_("Error when setting the locking property."));
			} else {
				message(from_utf8(res));
				reloadBuffer();
			}
		}
		break;

	case LFUN_VC_REVERT:
		LASSERT(buffer, return);
		buffer->lyxvc().revert();
		reloadBuffer();
		break;

	case LFUN_VC_UNDO_LAST:
		LASSERT(buffer, return);
		buffer->lyxvc().undoLast();
		reloadBuffer();
		break;

	case LFUN_VC_COMMAND: {
		string flag = cmd.getArg(0);
		if (buffer && contains(flag, 'R') && !ensureBufferClean(buffer))
			break;
		docstring message;
		if (contains(flag, 'M')) {
			if (!Alert::askForText(message, _("LyX VC: Log Message")))
				break;
		}
		string path = cmd.getArg(1);
		if (contains(path, "$$p") && buffer)
			path = subst(path, "$$p", buffer->filePath());
		LYXERR(Debug::LYXVC, "Directory: " << path);
		FileName pp(path);
		if (!pp.isReadableDirectory()) {
			lyxerr << _("Directory is not accessible.") << endl;
			break;
		}
		support::PathChanger p(pp);

		string command = cmd.getArg(2);
		if (command.empty())
			break;
		if (buffer) {
			command = subst(command, "$$i", buffer->absFileName());
			command = subst(command, "$$p", buffer->filePath());
		}
		command = subst(command, "$$m", to_utf8(message));
		LYXERR(Debug::LYXVC, "Command: " << command);
		Systemcall one;
		one.startscript(Systemcall::Wait, command);

		if (!buffer)
			break;
		if (contains(flag, 'I'))
			buffer->markDirty();
		if (contains(flag, 'R'))
			reloadBuffer();

		break;
		}
	default:
		break;
	}
}


void GuiView::openChildDocument(string const & fname)
{
	LASSERT(documentBufferView(), return);
	Buffer & buffer = documentBufferView()->buffer();
	FileName const filename = support::makeAbsPath(fname, buffer.filePath());
	documentBufferView()->saveBookmark(false);
	Buffer * child = 0;
	bool parsed = false;
	if (theBufferList().exists(filename)) {
		child = theBufferList().getBuffer(filename);
	} else {
		message(bformat(_("Opening child document %1$s..."),
		makeDisplayPath(filename.absFilename())));
		child = loadDocument(filename, false);
		parsed = true;
	}
	if (!child)
		return;

	// Set the parent name of the child document.
	// This makes insertion of citations and references in the child work,
	// when the target is in the parent or another child document.
	child->setParent(&buffer);
	child->masterBuffer()->updateLabels();
	setBuffer(child);
	if (parsed)
		child->errors("Parse");
}


bool GuiView::goToFileRow(string const & argument)
{
	string file_name;
	int row;
	istringstream is(argument);
	is >> file_name >> row;
	file_name = os::internal_path(file_name);
	Buffer * buf = 0;
	string const abstmp = package().temp_dir().absFilename();
	string const realtmp = package().temp_dir().realPath();
	// We have to use os::path_prefix_is() here, instead of
	// simply prefixIs(), because the file name comes from
	// an external application and may need case adjustment.
	if (os::path_prefix_is(file_name, abstmp, os::CASE_ADJUSTED)
		|| os::path_prefix_is(file_name, realtmp, os::CASE_ADJUSTED)) {
		// Needed by inverse dvi search. If it is a file
		// in tmpdir, call the apropriated function.
		// If tmpdir is a symlink, we may have the real
		// path passed back, so we correct for that.
		if (!prefixIs(file_name, abstmp))
			file_name = subst(file_name, realtmp, abstmp);
		buf = theBufferList().getBufferFromTmp(file_name);
	} else {
		// Must replace extension of the file to be .lyx
		// and get full path
		FileName const s = fileSearch(string(),
					      support::changeExtension(file_name, ".lyx"), "lyx");
		// Either change buffer or load the file
		if (theBufferList().exists(s))
			buf = theBufferList().getBuffer(s);
		else if (s.exists()) {
			buf = loadDocument(s);
			buf->updateLabels();
			buf->errors("Parse");
		} else {
			message(bformat(
					_("File does not exist: %1$s"),
					makeDisplayPath(file_name)));
			return false;
		}
	}
	setBuffer(buf);
	documentBufferView()->setCursorFromRow(row);
	return true;
}


bool GuiView::dispatch(FuncRequest const & cmd)
{
	BufferView * bv = currentBufferView();
	// By default we won't need any update.
	if (bv)
		bv->cursor().updateFlags(Update::None);

	Buffer * doc_buffer = documentBufferView()
		? &(documentBufferView()->buffer()) : 0;

	bool dispatched = true;

	if (cmd.origin == FuncRequest::TOC) {
		GuiToc * toc = static_cast<GuiToc*>(findOrBuild("toc", false));
		toc->doDispatch(bv->cursor(), cmd);
		return true;
	}

	switch(cmd.action) {
		case LFUN_BUFFER_CHILD_OPEN:
			openChildDocument(to_utf8(cmd.argument()));
			break;

		case LFUN_BUFFER_IMPORT:
			importDocument(to_utf8(cmd.argument()));
			break;

		case LFUN_BUFFER_SWITCH:
			if (FileName::isAbsolute(to_utf8(cmd.argument()))) {
				Buffer * buffer = 
					theBufferList().getBuffer(FileName(to_utf8(cmd.argument())));
				if (buffer)
					setBuffer(buffer);
				else
					bv->cursor().message(_("Document not loaded"));
			}
			break;

		case LFUN_BUFFER_NEXT:
			gotoNextOrPreviousBuffer(NEXTBUFFER);
			break;

		case LFUN_BUFFER_PREVIOUS:
			gotoNextOrPreviousBuffer(PREVBUFFER);
			break;

		case LFUN_COMMAND_EXECUTE: {
			bool const show_it = cmd.argument() != "off";
			// FIXME: this is a hack, "minibuffer" should not be
			// hardcoded.
			if (GuiToolbar * t = toolbar("minibuffer")) {
				t->setVisible(show_it);
				if (show_it && t->commandBuffer())
					t->commandBuffer()->setFocus();
			}
			break;
		}
		case LFUN_DROP_LAYOUTS_CHOICE:
			d.layout_->showPopup();
			break;

		case LFUN_MENU_OPEN:
			if (QMenu * menu = guiApp->menus().menu(toqstr(cmd.argument()), *this))
				menu->exec(QCursor::pos());
			break;

		case LFUN_FILE_INSERT:
			insertLyXFile(cmd.argument());
			break;
		case LFUN_FILE_INSERT_PLAINTEXT_PARA:
			insertPlaintextFile(cmd.argument(), true);
			break;

		case LFUN_FILE_INSERT_PLAINTEXT:
			insertPlaintextFile(cmd.argument(), false);
			break;

		case LFUN_BUFFER_RELOAD: {
			LASSERT(doc_buffer, break);
			docstring const file = makeDisplayPath(doc_buffer->absFileName(), 20);
			docstring text = bformat(_("Any changes will be lost. Are you sure "
							     "you want to revert to the saved version of the document %1$s?"), file);
			int const ret = Alert::prompt(_("Revert to saved document?"),
				text, 1, 1, _("&Revert"), _("&Cancel"));

			if (ret == 0)
				reloadBuffer();
			break;
		}

		case LFUN_BUFFER_WRITE:
			LASSERT(doc_buffer, break);
			saveBuffer(*doc_buffer);
			break;

		case LFUN_BUFFER_WRITE_AS:
			LASSERT(doc_buffer, break);
			renameBuffer(*doc_buffer, cmd.argument());
			break;

		case LFUN_BUFFER_WRITE_ALL: {
			Buffer * first = theBufferList().first();
			if (!first)
				break;
			message(_("Saving all documents..."));
			// We cannot use a for loop as the buffer list cycles.
			Buffer * b = first;
			do {
				if (!b->isClean()) {
					saveBuffer(*b);
					LYXERR(Debug::ACTION, "Saved " << b->absFileName());
				}
				b = theBufferList().next(b);
			} while (b != first); 
			message(_("All documents saved."));
			break;
		}

		case LFUN_BUFFER_CLOSE:
			closeBuffer();
			break;

		case LFUN_BUFFER_CLOSE_ALL:
			closeBufferAll();
			break;

		case LFUN_TOOLBAR_TOGGLE: {
			string const name = cmd.getArg(0);
			if (GuiToolbar * t = toolbar(name))
				t->toggle();
			break;
		}

		case LFUN_DIALOG_UPDATE: {
			string const name = to_utf8(cmd.argument());
			// Can only update a dialog connected to an existing inset
			Inset * inset = getOpenInset(name);
			if (inset) {
				FuncRequest fr(LFUN_INSET_DIALOG_UPDATE, cmd.argument());
				inset->dispatch(currentBufferView()->cursor(), fr);
			} else if (name == "paragraph") {
				lyx::dispatch(FuncRequest(LFUN_PARAGRAPH_UPDATE));
			} else if (name == "prefs" || name == "document") {
				updateDialog(name, string());
			}
			break;
		}

		case LFUN_DIALOG_TOGGLE: {
			if (isDialogVisible(cmd.getArg(0)))
				dispatch(FuncRequest(LFUN_DIALOG_HIDE, cmd.argument()));
			else
				dispatch(FuncRequest(LFUN_DIALOG_SHOW, cmd.argument()));
			break;
		}

		case LFUN_DIALOG_DISCONNECT_INSET:
			disconnectDialog(to_utf8(cmd.argument()));
			break;

		case LFUN_DIALOG_HIDE: {
			guiApp->hideDialogs(to_utf8(cmd.argument()), 0);
			break;
		}

		case LFUN_DIALOG_SHOW: {
			string const name = cmd.getArg(0);
			string data = trim(to_utf8(cmd.argument()).substr(name.size()));

			if (name == "character") {
				data = freefont2string();
				if (!data.empty())
					showDialog("character", data);
			} else if (name == "latexlog") {
				Buffer::LogType type; 
				string const logfile = doc_buffer->logName(&type);
				switch (type) {
				case Buffer::latexlog:
					data = "latex ";
					break;
				case Buffer::buildlog:
					data = "literate ";
					break;
				}
				data += Lexer::quoteString(logfile);
				showDialog("log", data);
			} else if (name == "vclog") {
				string const data = "vc " +
					Lexer::quoteString(doc_buffer->lyxvc().getLogFile());
				showDialog("log", data);
			} else if (name == "symbols") {
				data = bv->cursor().getEncoding()->name();
				if (!data.empty())
					showDialog("symbols", data);
			// bug 5274
			} else if (name == "prefs" && isFullScreen()) {
				FuncRequest fr(LFUN_INSET_INSERT, "fullscreen");
				lfunUiToggle(fr);
				showDialog("prefs", data);
			} else
				showDialog(name, data);
			break;
		}

		case LFUN_MESSAGE:
			message(cmd.argument());
			break;

		case LFUN_INSET_APPLY: {
			string const name = cmd.getArg(0);
			Inset * inset = getOpenInset(name);
			if (inset) {
				// put cursor in front of inset.
				if (!currentBufferView()->setCursorFromInset(inset)) {
					LASSERT(false, break);
				}
				BufferView * bv = currentBufferView();
				// useful if we are called from a dialog.
				bv->cursor().beginUndoGroup();
				bv->cursor().recordUndo();
				FuncRequest fr(LFUN_INSET_MODIFY, cmd.argument());
				inset->dispatch(bv->cursor(), fr);
				bv->cursor().endUndoGroup();
			} else {
				FuncRequest fr(LFUN_INSET_INSERT, cmd.argument());
				lyx::dispatch(fr);
			}
			break;
		}

		case LFUN_UI_TOGGLE:
			lfunUiToggle(cmd);
			// Make sure the keyboard focus stays in the work area.
			setFocus();
			break;

		case LFUN_SPLIT_VIEW: {
			LASSERT(doc_buffer, break);
			string const orientation = cmd.getArg(0);
			d.splitter_->setOrientation(orientation == "vertical"
				? Qt::Vertical : Qt::Horizontal);
			TabWorkArea * twa = addTabWorkArea();
			GuiWorkArea * wa = twa->addWorkArea(*doc_buffer, *this);
			setCurrentWorkArea(wa);
			break;
		}
		case LFUN_CLOSE_TAB_GROUP:
			if (TabWorkArea * twa = d.currentTabWorkArea()) {
				closeTabWorkArea(twa);
				d.current_work_area_ = 0;
				twa = d.currentTabWorkArea();
				// Switch to the next GuiWorkArea in the found TabWorkArea.
				if (twa) {
					// Make sure the work area is up to date.
					setCurrentWorkArea(twa->currentWorkArea());
				} else {
					setCurrentWorkArea(0);
				}
			}
			break;
			
		case LFUN_COMPLETION_INLINE:
			if (d.current_work_area_)
				d.current_work_area_->completer().showInline();
			break;

		case LFUN_COMPLETION_POPUP:
			if (d.current_work_area_)
				d.current_work_area_->completer().showPopup();
			break;


		case LFUN_COMPLETION_COMPLETE:
			if (d.current_work_area_)
				d.current_work_area_->completer().tab();
			break;

		case LFUN_COMPLETION_CANCEL:
			if (d.current_work_area_) {
				if (d.current_work_area_->completer().popupVisible())
					d.current_work_area_->completer().hidePopup();
				else
					d.current_work_area_->completer().hideInline();
			}
			break;

		case LFUN_COMPLETION_ACCEPT:
			if (d.current_work_area_)
				d.current_work_area_->completer().activate();
			break;

		case LFUN_BUFFER_ZOOM_IN:
		case LFUN_BUFFER_ZOOM_OUT:
			if (cmd.argument().empty()) {
				if (cmd.action == LFUN_BUFFER_ZOOM_IN)
					lyxrc.zoom += 20;
				else
					lyxrc.zoom -= 20;
			} else
				lyxrc.zoom += convert<int>(cmd.argument());

			if (lyxrc.zoom < 10)
				lyxrc.zoom = 10;
				
			// The global QPixmapCache is used in GuiPainter to cache text
			// painting so we must reset it.
			QPixmapCache::clear();
			guiApp->fontLoader().update();
			lyx::dispatch(FuncRequest(LFUN_SCREEN_FONT_UPDATE));
			break;

		case LFUN_VC_REGISTER:
		case LFUN_VC_CHECK_IN:
		case LFUN_VC_CHECK_OUT:
		case LFUN_VC_LOCKING_TOGGLE:
		case LFUN_VC_REVERT:
		case LFUN_VC_UNDO_LAST:
		case LFUN_VC_COMMAND:
			dispatchVC(cmd);
			break;

		case LFUN_SERVER_GOTO_FILE_ROW:
			goToFileRow(to_utf8(cmd.argument()));
			break;

		default:
			dispatched = false;
			break;
	}

	// Part of automatic menu appearance feature.
	if (isFullScreen()) {
		if (menuBar()->isVisible() && lyxrc.full_screen_menubar)
			menuBar()->hide();
		if (statusBar()->isVisible())
			statusBar()->hide();
	}

	return dispatched;
}


void GuiView::lfunUiToggle(FuncRequest const & cmd)
{
	string const arg = cmd.getArg(0);
	if (arg == "scrollbar") {
		// hide() is of no help
		if (d.current_work_area_->verticalScrollBarPolicy() ==
			Qt::ScrollBarAlwaysOff)

			d.current_work_area_->setVerticalScrollBarPolicy(
				Qt::ScrollBarAsNeeded);
		else
			d.current_work_area_->setVerticalScrollBarPolicy(
				Qt::ScrollBarAlwaysOff);
		return;
	}
	if (arg == "statusbar") {
		statusBar()->setVisible(!statusBar()->isVisible());
		return;
	}
	if (arg == "menubar") {
		menuBar()->setVisible(!menuBar()->isVisible());
		return;
	}
#if QT_VERSION >= 0x040300
	if (arg == "frame") {
		int l, t, r, b;
		getContentsMargins(&l, &t, &r, &b);
		//are the frames in default state?
		d.current_work_area_->setFrameStyle(QFrame::NoFrame);
		if (l == 0) {
			setContentsMargins(-2, -2, -2, -2);
		} else {
			setContentsMargins(0, 0, 0, 0);
		}
		return;
	}
#endif
	if (arg == "fullscreen") {
		toggleFullScreen();
		return;
	}

	message(bformat("LFUN_UI_TOGGLE " + _("%1$s unknown command!"), from_utf8(arg)));
}


void GuiView::toggleFullScreen()
{
	if (isFullScreen()) {
		for (int i = 0; i != d.splitter_->count(); ++i)
			d.tabWorkArea(i)->setFullScreen(false);
#if QT_VERSION >= 0x040300
		setContentsMargins(0, 0, 0, 0);
#endif
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		restoreLayout();
		menuBar()->show();
		statusBar()->show();
	} else {
		// bug 5274
		hideDialogs("prefs", 0);
		for (int i = 0; i != d.splitter_->count(); ++i)
			d.tabWorkArea(i)->setFullScreen(true);
#if QT_VERSION >= 0x040300
		setContentsMargins(-2, -2, -2, -2);
#endif
		saveLayout();
		setWindowState(windowState() ^ Qt::WindowFullScreen);
		statusBar()->hide();
		if (lyxrc.full_screen_menubar)
			menuBar()->hide();
		if (lyxrc.full_screen_toolbars) {
			ToolbarMap::iterator end = d.toolbars_.end();
			for (ToolbarMap::iterator it = d.toolbars_.begin(); it != end; ++it)
				it->second->hide();
		}
	}

	// give dialogs like the TOC a chance to adapt
	updateDialogs();
}


Buffer const * GuiView::updateInset(Inset const * inset)
{
	if (!d.current_work_area_)
		return 0;

	if (inset)
		d.current_work_area_->scheduleRedraw();

	return &d.current_work_area_->bufferView().buffer();
}


void GuiView::restartCursor()
{
	/* When we move around, or type, it's nice to be able to see
	 * the cursor immediately after the keypress.
	 */
	if (d.current_work_area_)
		d.current_work_area_->startBlinkingCursor();

	// Take this occasion to update the other GUI elements.
	updateDialogs();
	updateStatusBar();
}


void GuiView::updateCompletion(Cursor & cur, bool start, bool keep)
{
	if (d.current_work_area_)
		d.current_work_area_->completer().updateVisibility(cur, start, keep);
}

namespace {

// This list should be kept in sync with the list of insets in
// src/insets/Inset.cpp.  I.e., if a dialog goes with an inset, the
// dialog should have the same name as the inset.
// Changes should be also recorded in LFUN_DIALOG_SHOW doxygen
// docs in LyXAction.cpp.

char const * const dialognames[] = {
"aboutlyx", "bibitem", "bibtex", "box", "branch", "changes", "character",
"citation", "document", "errorlist", "ert", "external", "file", "findreplace",
"findreplaceadv", "float", "graphics", "href", "include", "index",
"index_print", "info", "listings", "label", "log", "mathdelimiter",
"mathmatrix", "mathspace", "nomenclature", "nomencl_print", "note",
"paragraph", "phantom", "prefs", "print", "ref", "sendto", "space",
"spellchecker", "symbols", "tabular", "tabularcreate", "thesaurus", "texinfo",
"toc", "view-source", "vspace", "wrap" };

char const * const * const end_dialognames =
	dialognames + (sizeof(dialognames) / sizeof(char *));

class cmpCStr {
public:
	cmpCStr(char const * name) : name_(name) {}
	bool operator()(char const * other) {
		return strcmp(other, name_) == 0;
	}
private:
	char const * name_;
};


bool isValidName(string const & name)
{
	return find_if(dialognames, end_dialognames,
			    cmpCStr(name.c_str())) != end_dialognames;
}

} // namespace anon


void GuiView::resetDialogs()
{
	// Make sure that no LFUN uses any LyXView.
	guiApp->setCurrentView(0);
	saveLayout();
	menuBar()->clear();
	constructToolbars();
	guiApp->menus().fillMenuBar(menuBar(), this, false);
	d.layout_->updateContents(true);
	// Now update controls with current buffer.
	guiApp->setCurrentView(this);
	restoreLayout();
	restartCursor();
}


Dialog * GuiView::findOrBuild(string const & name, bool hide_it)
{
	if (!isValidName(name))
		return 0;

	map<string, DialogPtr>::iterator it = d.dialogs_.find(name);

	if (it != d.dialogs_.end()) {
		if (hide_it)
			it->second->hideView();
		return it->second.get();
	}

	Dialog * dialog = build(name);
	d.dialogs_[name].reset(dialog);
	if (lyxrc.allow_geometry_session)
		dialog->restoreSession();
	if (hide_it)
		dialog->hideView();
	return dialog;
}


void GuiView::showDialog(string const & name, string const & data,
	Inset * inset)
{
	if (d.in_show_)
		return;

	d.in_show_ = true;
	try {
		Dialog * dialog = findOrBuild(name, false);
		if (dialog) {
			dialog->showData(data);
			if (inset)
				d.open_insets_[name] = inset;
		}
	}
	catch (ExceptionMessage const & ex) {
		d.in_show_ = false;
		throw ex;
	}
	d.in_show_ = false;
}


bool GuiView::isDialogVisible(string const & name) const
{
	map<string, DialogPtr>::const_iterator it = d.dialogs_.find(name);
	if (it == d.dialogs_.end())
		return false;
	return it->second.get()->isVisibleView() && !it->second.get()->isClosing();
}


void GuiView::hideDialog(string const & name, Inset * inset)
{
	map<string, DialogPtr>::const_iterator it = d.dialogs_.find(name);
	if (it == d.dialogs_.end())
		return;

	if (inset && inset != getOpenInset(name))
		return;

	Dialog * const dialog = it->second.get();
	if (dialog->isVisibleView())
		dialog->hideView();
	d.open_insets_[name] = 0;
}


void GuiView::disconnectDialog(string const & name)
{
	if (!isValidName(name))
		return;

	if (d.open_insets_.find(name) != d.open_insets_.end())
		d.open_insets_[name] = 0;
}


Inset * GuiView::getOpenInset(string const & name) const
{
	if (!isValidName(name))
		return 0;

	map<string, Inset *>::const_iterator it = d.open_insets_.find(name);
	return it == d.open_insets_.end() ? 0 : it->second;
}


void GuiView::hideAll() const
{
	map<string, DialogPtr>::const_iterator it  = d.dialogs_.begin();
	map<string, DialogPtr>::const_iterator end = d.dialogs_.end();

	for(; it != end; ++it)
		it->second->hideView();
}


void GuiView::updateDialogs()
{
	map<string, DialogPtr>::const_iterator it  = d.dialogs_.begin();
	map<string, DialogPtr>::const_iterator end = d.dialogs_.end();

	for(; it != end; ++it) {
		Dialog * dialog = it->second.get();
		if (dialog && dialog->isVisibleView())
			dialog->checkStatus();
	}
	updateToolbars();
	updateLayoutList();
}


// will be replaced by a proper factory...
Dialog * createGuiAbout(GuiView & lv);
Dialog * createGuiBibitem(GuiView & lv);
Dialog * createGuiBibtex(GuiView & lv);
Dialog * createGuiBox(GuiView & lv);
Dialog * createGuiBranch(GuiView & lv);
Dialog * createGuiChanges(GuiView & lv);
Dialog * createGuiCharacter(GuiView & lv);
Dialog * createGuiCitation(GuiView & lv);
Dialog * createGuiDelimiter(GuiView & lv);
Dialog * createGuiDocument(GuiView & lv);
Dialog * createGuiErrorList(GuiView & lv);
Dialog * createGuiERT(GuiView & lv);
Dialog * createGuiExternal(GuiView & lv);
Dialog * createGuiFloat(GuiView & lv);
Dialog * createGuiGraphics(GuiView & lv);
Dialog * createGuiInclude(GuiView & lv);
Dialog * createGuiIndex(GuiView & lv);
Dialog * createGuiInfo(GuiView & lv);
Dialog * createGuiLabel(GuiView & lv);
Dialog * createGuiListings(GuiView & lv);
Dialog * createGuiLog(GuiView & lv);
Dialog * createGuiMathHSpace(GuiView & lv);
Dialog * createGuiMathMatrix(GuiView & lv);
Dialog * createGuiNomenclature(GuiView & lv);
Dialog * createGuiNote(GuiView & lv);
Dialog * createGuiParagraph(GuiView & lv);
Dialog * createGuiPhantom(GuiView & lv);
Dialog * createGuiPreferences(GuiView & lv);
Dialog * createGuiPrint(GuiView & lv);
Dialog * createGuiPrintindex(GuiView & lv);
Dialog * createGuiPrintNomencl(GuiView & lv);
Dialog * createGuiRef(GuiView & lv);
Dialog * createGuiSearch(GuiView & lv);
Dialog * createGuiSearchAdv(GuiView & lv);
Dialog * createGuiSendTo(GuiView & lv);
Dialog * createGuiShowFile(GuiView & lv);
Dialog * createGuiSpellchecker(GuiView & lv);
Dialog * createGuiSymbols(GuiView & lv);
Dialog * createGuiTabularCreate(GuiView & lv);
Dialog * createGuiTabular(GuiView & lv);
Dialog * createGuiTexInfo(GuiView & lv);
Dialog * createGuiTextHSpace(GuiView & lv);
Dialog * createGuiToc(GuiView & lv);
Dialog * createGuiThesaurus(GuiView & lv);
Dialog * createGuiHyperlink(GuiView & lv);
Dialog * createGuiVSpace(GuiView & lv);
Dialog * createGuiViewSource(GuiView & lv);
Dialog * createGuiWrap(GuiView & lv);


Dialog * GuiView::build(string const & name)
{
	LASSERT(isValidName(name), return 0);

	if (name == "aboutlyx")
		return createGuiAbout(*this);
	if (name == "bibitem")
		return createGuiBibitem(*this);
	if (name == "bibtex")
		return createGuiBibtex(*this);
	if (name == "box")
		return createGuiBox(*this);
	if (name == "branch")
		return createGuiBranch(*this);
	if (name == "changes")
		return createGuiChanges(*this);
	if (name == "character")
		return createGuiCharacter(*this);
	if (name == "citation")
		return createGuiCitation(*this);
	if (name == "document")
		return createGuiDocument(*this);
	if (name == "errorlist")
		return createGuiErrorList(*this);
	if (name == "ert")
		return createGuiERT(*this);
	if (name == "external")
		return createGuiExternal(*this);
	if (name == "file")
		return createGuiShowFile(*this);
	if (name == "findreplace")
		return createGuiSearch(*this);
	if (name == "findreplaceadv")
		return createGuiSearchAdv(*this);
	if (name == "float")
		return createGuiFloat(*this);
	if (name == "graphics")
		return createGuiGraphics(*this);
	if (name == "href")
		return createGuiHyperlink(*this);
	if (name == "include")
		return createGuiInclude(*this);
	if (name == "index")
		return createGuiIndex(*this);
	if (name == "index_print")
		return createGuiPrintindex(*this);
	if (name == "info")
		return createGuiInfo(*this);
	if (name == "label")
		return createGuiLabel(*this);
	if (name == "listings")
		return createGuiListings(*this);
	if (name == "log")
		return createGuiLog(*this);
	if (name == "mathdelimiter")
		return createGuiDelimiter(*this);
	if (name == "mathspace")
		return createGuiMathHSpace(*this);
	if (name == "mathmatrix")
		return createGuiMathMatrix(*this);
	if (name == "nomenclature")
		return createGuiNomenclature(*this);
	if (name == "nomencl_print")
		return createGuiPrintNomencl(*this);
	if (name == "note")
		return createGuiNote(*this);
	if (name == "paragraph")
		return createGuiParagraph(*this);
	if (name == "phantom")
		return createGuiPhantom(*this);
	if (name == "prefs")
		return createGuiPreferences(*this);
	if (name == "print")
		return createGuiPrint(*this);
	if (name == "ref")
		return createGuiRef(*this);
	if (name == "sendto")
		return createGuiSendTo(*this);
	if (name == "space")
		return createGuiTextHSpace(*this);
	if (name == "spellchecker")
		return createGuiSpellchecker(*this);
	if (name == "symbols")
		return createGuiSymbols(*this);
	if (name == "tabular")
		return createGuiTabular(*this);
	if (name == "tabularcreate")
		return createGuiTabularCreate(*this);
	if (name == "texinfo")
		return createGuiTexInfo(*this);
	if (name == "thesaurus")
		return createGuiThesaurus(*this);
	if (name == "toc")
		return createGuiToc(*this);
	if (name == "view-source")
		return createGuiViewSource(*this);
	if (name == "vspace")
		return createGuiVSpace(*this);
	if (name == "wrap")
		return createGuiWrap(*this);

	return 0;
}


} // namespace frontend
} // namespace lyx

#include "moc_GuiView.cpp"
