// -*- C++ -*-
/**
 * \file xforms/FormUrl.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FORMURL_H
#define FORMURL_H


#include "FormBase.h"

class ControlUrl;
struct FD_url;

/** This class provides an XForms implementation of the Url Dialog.
 */
class FormUrl : public FormCB<ControlUrl, FormDB<FD_url> > {
public:
	///
	FormUrl();
private:
	/// Apply from dialog (modify or create inset)
	virtual void apply();
	/// Build the dialog
	virtual void build();
	/// Update dialog before showing it
	virtual void update();
};

#endif // FORMURL_H
