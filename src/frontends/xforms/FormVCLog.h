// -*- C++ -*-
/**
 * \file FormVCLog.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef FORMVCLOG_H
#define FORMVCLOG_H


#include "FormBrowser.h"

class ControlVCLog;

/**
 * This class provides an XForms implementation of the Version Control
 * log viewer
 */
class FormVCLog : public FormController<ControlVCLog, FormBrowser> {
public:
	///
	FormVCLog(Dialog &);

	// Functions accessible to the Controller.

	/// Set the Params variable for the Controller.
	virtual void apply() {}
	/// Update dialog before/whilst showing it.
	virtual void update();
};

#endif // FORMVCLOG_H
