// -*- C++ -*-
/**
 * \file QBibtex.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef QBIBTEX_H
#define QBIBTEX_H


#include "QDialogView.h"

class ControlBibtex;
class QBibtexDialog;

class QBibtex
	: public QController<ControlBibtex, QView<QBibtexDialog> >
{
public:
	friend class QBibtexDialog;

	QBibtex(Dialog &);
protected:
	virtual bool isValid();
private:
	/// Apply changes
	virtual void apply();
	/// update
	virtual void update_contents();
	/// build the dialog
	virtual void build_dialog();
};

#endif // QBIBTEX_H
