// -*- C++ -*-
/**
 * \file FormSendto.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FORMSENDTO_H
#define FORMSENDTO_H

#ifdef __GNUG__
#pragma interface
#endif

#include "FormBase.h"
#include <vector>

class ControlSendto;
class Format;
struct FD_sendto;

/** This class provides an XForms implementation of the Custom Export Dialog.
 */
class FormSendto : public FormCB<ControlSendto, FormDB<FD_sendto> > {
public:
	///
	FormSendto();
private:
	/// Apply from dialog (modify or create inset)
	virtual void apply();
	/// Build the dialog
	virtual void build();
	/// Update the dialog
	virtual void update();
	/// Filter the inputs on callback from xforms
	virtual ButtonPolicy::SMInput input(FL_OBJECT *, long);

	std::vector<Format const *> all_formats_;
};

#endif // FORMSENDTO_H
