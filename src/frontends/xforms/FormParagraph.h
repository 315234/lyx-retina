// -*- C++ -*-
/**
 * \file FormParagraph.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef FORM_PARAGRAPH_H
#define FORM_PARAGRAPH_H


#include "FormBase.h"
#include "RadioButtonGroup.h"

struct FD_paragraph;
class ControlParagraph;

/** This class provides an XForms implementation of the FormParagraph dialog.
 */
class FormParagraph
	: public FormCB<ControlParagraph, FormDB<FD_paragraph> > {
public:
	///
	FormParagraph();
private:
	/// Build the dialog
	virtual void build();
	/// Apply from dialog
	virtual void apply();
	/// Update the dialog
	virtual void update();

	/// Filter the inputs on callback from xforms
	virtual ButtonPolicy::SMInput input(FL_OBJECT *, long);

	/// alignment radio buttons
	RadioButtonGroup alignment_;
};

#endif
