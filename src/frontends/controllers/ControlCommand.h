// -*- C++ -*-
/**
 * \file ControlCommand.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 *
 * ControlCommand is a controller class for dialogs that create or modify
 * an inset derived from InsetCommand.
 */

#ifndef CONTROLCOMMAND_H
#define CONTROLCOMMAND_H


#include "Dialog.h"
#include "insets/insetcommandparams.h"


class ControlCommand : public Dialog::Controller {
public:
	/** LFUN_INSET_APPLY requires a name, "citation", "ref" etc so that
	    it knows what to do with the rest of the contents.
	    An empty name indicates that no action will occur on 'Apply'.
	 */
	ControlCommand(Dialog &, std::string const & lfun_name = std::string());
	///
	InsetCommandParams & params() { return params_; }
	///
	InsetCommandParams const & params() const { return params_; }
	///
	virtual bool initialiseParams(std::string const & data);
	/// clean-up on hide.
	virtual void clearParams();
	/// clean-up on hide.
	virtual void dispatchParams();
	///
	virtual bool isBufferDependent() const { return true; }

private:
	///
	InsetCommandParams params_;
	/// Flags what action is taken by Kernel::dispatch()
	std::string const lfun_name_;
};


#endif // CONTROLCOMMAND_H
