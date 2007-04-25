/**
 * \file ControlNote.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "ControlNote.h"
#include "funcrequest.h"
#include "insets/InsetNote.h"
#include "gettext.h"


using std::string;

namespace lyx {
namespace frontend {

ControlNote::ControlNote(Dialog & parent)
	: Dialog::Controller(parent)
{}


bool ControlNote::initialiseParams(string const & data)
{
	InsetNoteParams params;
	InsetNoteMailer::string2params(data, params);
	params_.reset(new InsetNoteParams(params));
	return true;
}


void ControlNote::clearParams()
{
	params_.reset();
}


void ControlNote::dispatchParams()
{
	string const lfun = InsetNoteMailer::params2string(params());
	kernel().dispatch(FuncRequest(getLfun(), lfun));
}

} // namespace frontend
} // namespace lyx
