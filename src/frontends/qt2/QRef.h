// -*- C++ -*-
/**
 * \file QRef.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef QREF_H
#define QREF_H


#include "QDialogView.h"
#include <vector>
#include "support/std_string.h"

class ControlRef;
class QRefDialog;


class QRef
	: public QController<ControlRef, QView<QRefDialog> >
{
public:
	friend class QRefDialog;

	QRef(Dialog &);
private:
	/// apply changes
	virtual void apply();
	/// build dialog
	virtual void build_dialog();
	/// update dialog
	virtual void update_contents();

	/// is name allowed for this ?
	bool nameAllowed();

	/// is type allowed for this ?
	bool typeAllowed();

	/// go to current reference
	void gotoRef();

	/// set go back button
	void setGoBack();

	/// set goto ref button
	void setGotoRef();

	/// re-enter references
	void redoRefs();

	/// update references
	void updateRefs();

	/// sort or not persistent state
	bool sort_;

	/// at a reference ?
	bool at_ref_;

	/// the references
	std::vector<string> refs_;
};

#endif // QREF_H
