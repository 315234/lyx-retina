// -*- C++ -*-
/**
 * \file GuiView.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjornes
 * \author John Levon
 * \author Abdelrazak Younes
 * \author Peter Kümmel
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include "frontends/Delegates.h"

#include "support/strfwd.h"

#include <QMainWindow>

class QCloseEvent;
class QDragEnterEvent;
class QDropEvent;
class QMenu;
class QShowEvent;


namespace lyx {

namespace support { class FileName; }

class Buffer;
class BufferView;
class Cursor;
class DispatchResult;
class FuncStatus;
class FuncRequest;
class Inset;

namespace frontend {

class Dialog;
class LayoutBox;
class GuiToolbar;
class GuiWorkArea;
class TabWorkArea;
class TocModels;
class ToolbarInfo;

/**
 * GuiView - Qt4 main LyX window
 *
 * This class represents the main LyX window and provides
 * accessor functions to its content.
 *
 * Note: a QObject emits a destroyed(QObject *) Qt signal when it
 * is deleted. This might be useful for closing other dialogs
 * depending on a given GuiView.
 */
class GuiView : public QMainWindow, public GuiBufferViewDelegate,
	public GuiBufferDelegate
{
	Q_OBJECT

public:
	/// create a main window of the given dimensions
	GuiView(int id);

	~GuiView();

	/// closes the view such that the view knows that is closed
	/// programmatically and not by the user clicking the x.
	bool closeScheduled();

	int id() const { return id_; }

	///
	void setBusy(bool);
	/// are we busy ?
	bool busy() const;

	/// \name Generic accessor functions
	//@{
	/// The current BufferView refers to the BufferView that has the focus,
	/// including for example the one that is created when you use the
	/// advanced search and replace pane.
	/// \return the currently selected buffer view.
	BufferView * currentBufferView();
	BufferView const * currentBufferView() const;

	/// The document BufferView always refers to the view's main document
	/// BufferView. So, even if the BufferView in e.g., the advanced
	/// search and replace pane has the focus.
	/// \return the current document buffer view.
	BufferView * documentBufferView();
	BufferView const * documentBufferView() const;

	void newDocument(std::string const & filename,
		bool fromTemplate);

	/// display a message in the view
	/// could be called from any thread
	void message(docstring const &);
	
	bool getStatus(FuncRequest const & cmd, FuncStatus & flag);
	/// dispatch command.
	/// \return true if the \c FuncRequest has been dispatched.
	void dispatch(FuncRequest const & cmd, DispatchResult & dr);

	void restartCursor();
	/// Update the completion popup and the inline completion state.
	/// If \c start is true, then a new completion might be started.
	/// If \c keep is true, an active completion will be kept active
	/// even though the cursor moved. The update flags of \c cur might
	/// be changed.
	void updateCompletion(Cursor & cur, bool start, bool keep);

	///
	void setFocus();

	///
	void focusInEvent(QFocusEvent * e);
	/// set a buffer to the current workarea.
	void setBuffer(Buffer * b); ///< \c Buffer to set.

	/// load a document into the current workarea.
	Buffer * loadDocument(
		support::FileName const &  name, ///< File to load.
		bool tolastfiles = true  ///< append to the "Open recent" menu?
		);

	/// add toolbar, if newline==true, add a toolbar break before the toolbar
	GuiToolbar * makeToolbar(ToolbarInfo const & tbinfo, bool newline);
	void updateStatusBar();

	/// updates the possible layouts selectable
	void updateLayoutList();
	void updateToolbars();
	QMenu * createPopupMenu();

	///
	LayoutBox * getLayoutDialog() const;

	/// hides the workarea and makes sure it is clean
	bool hideWorkArea(GuiWorkArea * wa);
	/// closes the workarea
	bool closeWorkArea(GuiWorkArea * wa);
	/// closes the buffer
	bool closeBuffer(Buffer & buf);
	///
	void openDocument(std::string const & filename);
	///
	void importDocument(std::string const &);

	/// \name GuiBufferDelegate.
	//@{
	void resetAutosaveTimers();
	void errors(std::string const &, bool from_master = false);
	void structureChanged();
	void updateTocItem(std::string const &, DocIterator const &);
	//@}

	///
	TocModels & tocModels();
	
	/// called on timeout
	void autoSave();

	/// check for external change of any opened buffer, mainly for svn usage
	void checkExternallyModifiedBuffers();

	/** redraw \c inset in all the BufferViews in which it is currently
	 *  visible. If successful return a pointer to the owning Buffer.
	 */
	Buffer const * updateInset(Inset const *);

	/// \return the \c Workarea associated to \p  Buffer
	/// \retval 0 if no \c WorkArea is found.
	GuiWorkArea * workArea(Buffer & buffer);

	/// Add a \c WorkArea 
	/// \return the \c Workarea associated to \p  Buffer
	/// \retval 0 if no \c WorkArea is found.
	GuiWorkArea * addWorkArea(Buffer & buffer);
	/// \param work_area The current \c WorkArea, or \c NULL
	void setCurrentWorkArea(GuiWorkArea * work_area);
	///
	void removeWorkArea(GuiWorkArea * work_area);
	/// return the current WorkArea (the one that has the focus).
	GuiWorkArea const * currentWorkArea() const;
	/// return the current WorkArea (the one that has the focus).
	GuiWorkArea * currentWorkArea();

	/// return the current document WorkArea (it may not have the focus).
	GuiWorkArea const * currentMainWorkArea() const;
	/// return the current document WorkArea (it may not have the focus).
	GuiWorkArea * currentMainWorkArea();

Q_SIGNALS:
	void closing(int);
	void triggerShowDialog(QString const & qname, QString const & qdata, Inset * inset);

public Q_SLOTS:
	/// idle timeout.
	/// clear any temporary message and replace with current status.
	void clearMessage();

private Q_SLOTS:
	///
	void updateWindowTitle(GuiWorkArea * wa);
	///
	void resetWindowTitleAndIconText();

	///
	void on_currentWorkAreaChanged(GuiWorkArea *);
	///
	void on_lastWorkAreaRemoved();

	/// slots to change the icon size
	void smallSizedIcons();
	void normalSizedIcons();
	void bigSizedIcons();

	/// For completion of autosave or exporrt threads.
	void threadFinished();

	/// must be called in GUI thread
	void doShowDialog(QString const & qname, QString const & qdata,
	Inset * inset);

	/// must be called from GUI thread
	void updateStatusBarMessage(QString const & str);
	void clearMessageText();

private:
	/// Open given child document in current buffer directory.
	void openChildDocument(std::string const & filename);
	/// Close current document buffer.
	bool closeBuffer();
	/// Close all document buffers.
	bool closeBufferAll();
	///
	TabWorkArea * addTabWorkArea();

	/// connect to signals in the given BufferView
	void connectBufferView(BufferView & bv);
	/// disconnect from signals in the given BufferView
	void disconnectBufferView();
	/// connect to signals in the given buffer
	void connectBuffer(Buffer & buf);
	/// disconnect from signals in the given buffer
	void disconnectBuffer();
	///
	void dragEnterEvent(QDragEnterEvent * ev);
	///
	void dropEvent(QDropEvent * ev);
	/// make sure we quit cleanly
	void closeEvent(QCloseEvent * e);
	///
	void showEvent(QShowEvent *);

	/// in order to catch Tab key press.
	bool event(QEvent * e);
	bool focusNextPrevChild(bool);

	///
	bool goToFileRow(std::string const & argument);

	///
	struct GuiViewPrivate;
	GuiViewPrivate & d;

public:
	///
	/// dialogs for this view
	///

	///
	void resetDialogs();

	/// Hide all visible dialogs
	void hideAll() const;

	/// Update all visible dialogs.
	/** 
	 *  Check the status of all visible dialogs and disable or reenable
	 *  them as appropriate.
	 *
	 *  Disabling is needed for example when a dialog is open and the
	 *  cursor moves to a position where the corresponding inset is not
	 *  allowed.
	 */
	void updateDialogs();

	/** Show dialog could be called from arbitrary threads.
	    \param name == "bibtex", "citation" etc; an identifier used to
	    launch a particular dialog.
	    \param data is a string representation of the Inset contents.
	    It is often little more than the output from Inset::write.
	    It is passed to, and parsed by, the frontend dialog.
	    Several of these dialogs do not need any data,
	    so it defaults to string().
	    \param inset ownership is _not_ passed to the frontend dialog.
	    It is stored internally and used by the kernel to ascertain
	    what to do with the FuncRequest dispatched from the frontend
	    dialog on 'Apply'; should it be used to create a new inset at
	    the current cursor position or modify an existing, 'open' inset?
	*/
	void showDialog(std::string const & name,
		std::string const & data, Inset * inset = 0);

	/** \param name == "citation", "bibtex" etc; an identifier used
	    to reset the contents of a particular dialog with \param data.
	    See the comments to 'show', above.
	*/
	void updateDialog(std::string const & name, std::string const & data);

	/** All Dialogs of the given \param name will be closed if they are
	    connected to the given \param inset.
	*/
	void hideDialog(std::string const & name, Inset * inset);
	///
	void disconnectDialog(std::string const & name);

private:
	///
	void saveLayout() const;
	///
	bool restoreLayout();
	///
	GuiToolbar * toolbar(std::string const & name);
	///
	void constructToolbars();
	///
	void initToolbars();
	///
	bool lfunUiToggle(std::string const & ui_component);
	///
	void toggleFullScreen();
	///
	void insertLyXFile(docstring const & fname);
	///
	void insertPlaintextFile(docstring const & fname,
		bool asParagraph);

	/// Save a buffer as a new file. 
	/**
	Write a buffer to a new file name and rename the buffer
    according to the new file name.

    This function is e.g. used by menu callbacks and
    LFUN_BUFFER_WRITE_AS.

    If 'newname' is empty, the user is asked via a
    dialog for the buffer's new name and location.

    If 'newname' is non-empty and has an absolute path, that is used.
    Otherwise the base directory of the buffer is used as the base
    for any relative path in 'newname'.
	*/
	bool renameBuffer(Buffer & b, docstring const & newname);
	///
	bool saveBuffer(Buffer & b, std::ostream* stream = 0);
	/// Gets a new filename and saves the buffer encrypted
	bool saveBufferEncrypted(Buffer & b);	
	/// closes a workarea, if close_buffer is true the buffer will
	/// also be released, otherwise the buffer will be hidden.
	bool closeWorkArea(GuiWorkArea * wa, bool close_buffer);
	/// closes the tabworkarea and all tabs. If we are in a close event,
	/// all buffers will be closed, otherwise they will be hidden.
	bool closeTabWorkArea(TabWorkArea * twa);
	/// gives the user the possibility to save his work 
	/// or to discard the changes. If hiding is true, the
	/// document will be reloaded.
	bool saveBufferIfNeeded(Buffer & buf, bool hiding);
	/// closes all workareas
	bool closeWorkAreaAll();
	/// write all open workareas into the session file
	void writeSession() const;
	/// is the buffer in this workarea also shown in another tab ?
	/// This tab can either be in the same view or in another one.
	bool inMultiTabs(GuiWorkArea * wa);
	/// is the buffer in this workarea also shown in another view ?
	bool inMultiViews(GuiWorkArea * wa);
	///
	enum NextOrPrevious {
		NEXTBUFFER,
		PREVBUFFER
	};
	///
	void gotoNextOrPreviousBuffer(NextOrPrevious np);

	/// Is the dialog currently visible?
	bool isDialogVisible(std::string const & name) const;
	///
	Dialog * findOrBuild(std::string const & name, bool hide_it);
	///
	Dialog * build(std::string const & name);
	///
	void reloadBuffer();
	///
	void dispatchVC(FuncRequest const & cmd);
	///
	void showMessage();

	/// This view ID.
	int id_;

	/// flag to avoid two concurrent close events.
	bool closing_;
	/// if the view is busy the cursor shouldn't blink for instance.
	bool busy_;
};

} // namespace frontend
} // namespace lyx

#endif // GUIVIEW_H
