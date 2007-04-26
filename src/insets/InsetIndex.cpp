/**
 * \file InsetIndex.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */
#include <config.h>

#include "InsetIndex.h"

#include "DispatchResult.h"
#include "FuncRequest.h"
#include "gettext.h"
#include "LaTeXFeatures.h"
#include "MetricsInfo.h"
#include "sgml.h"

#include "support/std_ostream.h"


namespace lyx {

using std::string;
using std::ostream;


InsetIndex::InsetIndex(InsetCommandParams const & p)
	: InsetCommand(p, "index")
{}


// InsetIndex::InsetIndex(InsetCommandParams const & p, bool)
//	: InsetCommand(p, false)
// {}


docstring const InsetIndex::getScreenLabel(Buffer const &) const
{
	return _("Idx");
}


int InsetIndex::docbook(Buffer const &, odocstream & os,
			OutputParams const &) const
{
	os << "<indexterm><primary>"
           << sgml::escapeString(getParam("name"))
	   << "</primary></indexterm>";
	return 0;
}


InsetBase::Code InsetIndex::lyxCode() const
{
	return InsetBase::INDEX_CODE;
}



InsetPrintIndex::InsetPrintIndex(InsetCommandParams const & p)
	: InsetCommand(p, string())
{}


// InsetPrintIndex::InsetPrintIndex(InsetCommandParams const & p, bool)
//	: InsetCommand(p, false)
// {}


docstring const InsetPrintIndex::getScreenLabel(Buffer const &) const
{
	return _("Index");
}


void InsetPrintIndex::validate(LaTeXFeatures & features) const
{
	features.require("makeidx");
}


InsetBase::Code InsetPrintIndex::lyxCode() const
{
	return InsetBase::INDEX_PRINT_CODE;
}


} // namespace lyx
