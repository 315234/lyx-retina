/**
 * \file ControlVSpace.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Edwin Leuven
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "ControlVSpace.h"
#include "ButtonController.h"

#include "funcrequest.h"
#include "insets/InsetVSpace.h"


using std::istringstream;
using std::string;

namespace lyx {
namespace frontend {

ControlVSpace::ControlVSpace(Dialog & parent)
	: Dialog::Controller(parent)
{}


bool ControlVSpace::initialiseParams(string const & data)
{
	InsetVSpaceMailer::string2params(data, params_);

	// so that the user can press Ok
	dialog().bc().valid();

	return true;
}


void ControlVSpace::clearParams()
{
	params_ = VSpace();
}


void ControlVSpace::dispatchParams()
{
	string const str = InsetVSpaceMailer::params2string(params_);
	kernel().dispatch(FuncRequest(getLfun(), str));
}


VSpace & ControlVSpace::params()
{
	return params_;
}


VSpace const & ControlVSpace::params() const
{
	return params_;
}

} // namespace frontend
} // namespace lyx
