// -*- C++ -*-
/**
 * \file ControlInclude.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author John Levon
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef CONTROLINCLUDE_H
#define CONTROLINCLUDE_H


#include "ControlInset.h"
#include "insets/insetinclude.h" // InsetIncludeParams

/** A controller for the Include file dialog.
 */
class ControlInclude
	: public ControlInset<InsetInclude, InsetInclude::Params>
{
public:
	///
	enum Type {
		///
		INPUT,
		///
		VERBATIM,
		///
		INCLUDE
	};
	///
	ControlInclude(LyXView &, Dialogs &);

	/// Browse for a file
	string const Browse(string const &, Type);

	/// load a file
	void load(string const & file);

	/// test if file exist
	bool fileExists(string const & file);
private:
	/// Dispatch the changed parameters to the kernel.
	virtual void applyParamsToInset();
	/// Should be used but currently isn't
	virtual void applyParamsNoInset() {}
	/// get the parameters from the string passed to createInset.
	virtual InsetInclude::Params const getParams(string const &)
		{ return InsetInclude::Params(); }
	/// get the parameters from the inset passed to showInset.
	virtual InsetInclude::Params const
	getParams(InsetInclude const & inset) {
		return inset.params();
	}
};
#endif // CONTROLINCLUDE_H
