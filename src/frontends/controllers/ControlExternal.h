// -*- C++ -*-
/**
 * \file ControlExternal.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup
 * \author John Levon
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef CONTROLEXTERNAL_H
#define CONTROLEXTERNAL_H


#include "ControlInset.h"
#include "insets/insetexternal.h"

/** A controller for External dialogs.
 */
class ControlExternal
	: public ControlInset<InsetExternal, InsetExternal::Params>
{
public:
	///
	ControlExternal(LyXView &, Dialogs &);

	///
	void editExternal();
	///
	void viewExternal();
	///
	void updateExternal();
	///
	std::vector<string> const getTemplates() const;
	///
	int getTemplateNumber(string const &) const;
	///
	ExternalTemplate getTemplate(int) const;
	///
	string const Browse(string const &) const;
private:
	///
	virtual void applyParamsToInset();
	/// not needed.
	virtual void applyParamsNoInset() {}
	/// get the parameters from the string passed to createInset.
	virtual InsetExternal::Params const getParams(string const &);
	/// get the parameters from the inset passed to showInset.
	virtual InsetExternal::Params const getParams(InsetExternal const &);
};

#endif // CONTROLEXTERNAL_H
