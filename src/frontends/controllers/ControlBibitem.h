// -*- C++ -*-
/**
 * \file ControlBibitem.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef CONTROLBIBITEM_H
#define CONTROLBIBITEM_H


#include "ControlCommand.h"

/** A controller for Bibitem dialogs.
 */
class ControlBibitem : public ControlCommand {
public:
	///
	ControlBibitem(LyXView &, Dialogs &);
private:
	/// Dispatch the changed parameters to the kernel.
	virtual void applyParamsToInset();
	/// not needed.
	virtual void applyParamsNoInset() {}
};

#endif // CONTROLBIBITEM_H
