// -*- C++ -*-
/**
 * \file xforms/FormError.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FORMERROR_H
#define FORMERROR_H


#include "FormBase.h"

class ControlError;
struct FD_error;

/** This class provides an XForms implementation of the Error Dialog.
 */
class FormError : public FormCB<ControlError, FormDB<FD_error> > {
public:
	/// Constructor
	FormError();
private:
	/// not needed.
	virtual void apply() {}
	/// Build the dialog
	virtual void build();
	/// Update dialog before showing it
	virtual void update();
};

#endif // FORMERROR_H
