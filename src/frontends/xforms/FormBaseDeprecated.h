// -*- C++ -*-
/**
 * \file FormBaseDeprecated.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming 
 *
 * Full author contact details are available in file CREDITS
 */

/* A base class for those remaining xforms dialogs that haven't yet undergone
 * the controller-view split.
 * It is meant to be used solely as the parent class to FormBaseBI
 * and FormBaseBD.
 */

#ifndef FORMBASEDEPRECATED_H
#define FORMBASEDEPRECATED_H

#ifdef __GNUG__
#pragma interface
#endif

#include "xformsBC.h"
#include "FeedbackController.h"
#include "forms_fwd.h"

#include "LString.h"
#include <boost/utility.hpp>
#include <boost/signals/connection.hpp>
#include <X11/Xlib.h> // for Pixmap

class Buffer;
class Dialogs;
class LyXView;
class Tooltips;

class FormBaseDeprecated : boost::noncopyable, public FeedbackController
{
public:
	///
	FormBaseDeprecated(LyXView &, Dialogs &, string const &, bool);
	///
	virtual ~FormBaseDeprecated();

	/// Callback functions
	void WMHideCB();
	///
	void ApplyCB();
	///
	void OKCB();
	///
	void CancelCB();
	///
	void InputCB(FL_OBJECT *, long);
	///
	void RestoreCB();

	Tooltips & tooltips();

	/// Create the dialog if necessary, update it and display it.
	virtual void show();
	/// Hide the dialog.
	virtual void hide();

protected: // methods

	/// Pointer to the actual instantiation of the ButtonController.
	virtual xformsBC & bc() = 0;

	/** Redraw the form (on receipt of a Signal indicating, for example,
	 *  that the xform colors have been re-mapped).
	 *  Must be virtual because dialogs with tabbed folders will need to
	 *  redraw the form for each tab.
	 */
	virtual void redraw();

	/// Update the dialog.
	virtual void update() {}
	/// Connect signals. Also perform any necessary initialisation.
	virtual void connect();
	/// Disconnect signals. Also perform any necessary housekeeping.
	virtual void disconnect();
	/// Build the dialog
	virtual void build() = 0;
	/** Filter the inputs on callback from xforms
	 *  Return true if inputs are valid.
	 */
	virtual bool input(FL_OBJECT *, long) {
		return true;
	}
	/// Apply from dialog (modify or create inset)
	virtual void apply() {}
	/// OK from dialog
	virtual void ok() {
		apply();
		hide();
	}
	/// Cancel from dialog
	virtual void cancel() {
		hide();
	}
	/// Restore from dialog
	virtual void restore() {
		update();
	}
	/// Pointer to the actual instantiation of xform's form
	virtual FL_FORM * form() const = 0;

	/** Prepare the way to:
	 *  1. display feedback as the mouse moves over ob. This feedback will
	 *  typically be rather more verbose than just a tooltip.
	 *  2. activate the button controller after a paste with the middle
	 *  mouse button.
	 */
	static void setPrehandler(FL_OBJECT * ob);

	/** Which LyXFunc do we use?
	 *  We could modify Dialogs to have a visible LyXFunc* instead and
	 *  save a couple of bytes per dialog.
	 */
	LyXView & lv_;
	/// Used so we can get at the signals we have to connect to.
	Dialogs & d_;
	/// Hide connection.
	boost::signals::connection h_;
	/// Redraw connection.
	boost::signals::connection r_;
	/// dialog title, displayed by WM.
	string title_;

private:
	/** Called on the first show() request, initialising various bits and
	 *  pieces.
	 */
	void prepare_to_show();

	/// Passed to the window manager to give a pretty little symbol ;-)
	Pixmap icon_pixmap_;
	///
	Pixmap icon_mask_;
	/// The dialog's minimum allowable dimensions.
	int minw_;
	///
	int minh_;
	/// Can the dialog be resized after it has been created?
	bool allow_resize_;
	///
	Tooltips * tooltips_;
};


/** This class is an XForms GUI base class for Buffer Independent dialogs.
 *  Such dialogs do not require an update Connection although they may use
 *  an update() function which is also supported by restore().
 */
class FormBaseBI : public FormBaseDeprecated {
protected:
	/// Constructor
	FormBaseBI(LyXView &, Dialogs &, string const &,
		   bool allowResize = true);

	/// Connect signals
	virtual void connect();
};


/** This class is an XForms GUI base class for Buffer Dependent dialogs
 */
class FormBaseBD : public FormBaseDeprecated {
protected:
	/// Constructor
	FormBaseBD(LyXView &, Dialogs &, string const &,
		   bool allowResize = true);

	/// Connect signals
	virtual void connect();
	/// Disconnect signals
	virtual void disconnect();
	/// bool indicates if a buffer switch took place
	virtual void updateSlot(bool) { update(); }

	/// Update connection.
	boost::signals::connection u_;
};


#endif // FORMBASEDEPRECATED_H
