/**
 * \file InsetNomencl.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author O. U. Baran
 *
 * Full author contact details are available in file CREDITS.
 */
#include <config.h>

#include "InsetNomencl.h"
#include "InsetNote.h"

#include "Buffer.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "gettext.h"
#include "InsetIterator.h"
#include "LaTeXFeatures.h"
#include "MetricsInfo.h"
#include "sgml.h"


namespace lyx {

using std::string;


InsetNomencl::InsetNomencl(InsetCommandParams const & p)
	: InsetCommand(p, "nomenclature"),
	  glossary_entry_id(sgml::uniqueID(from_ascii("gloss")))
{}


docstring const InsetNomencl::getScreenLabel(Buffer const &) const
{
	return _("Glo");
}


int InsetNomencl::docbook(Buffer const &, odocstream & os,
		OutputParams const &) const
{
	os << "<glossterm linkend=\"" << glossary_entry_id << "\">"
	   << sgml::escapeString(getParam("symbol"))
	   << "</glossterm>";
	return 0;
}


int InsetNomencl::docbookGlossary(odocstream & os) const
{
	os << "<glossentry id=\"" << glossary_entry_id << "\">\n"
	   << "<glossterm>"
	   << sgml::escapeString(getParam("symbol"))
	   << "</glossterm>\n"
	   << "<glossdef><para>"
	   << sgml::escapeString(getParam("description"))
	   << "</para></glossdef>\n"
	   <<"</glossentry>\n";
	return 4;
}


void InsetNomencl::validate(LaTeXFeatures & features) const
{
	features.require("nomencl");
}


InsetBase::Code InsetNomencl::lyxCode() const
{
	return InsetBase::NOMENCL_CODE;
}


InsetPrintNomencl::InsetPrintNomencl(InsetCommandParams const & p)
	: InsetCommand(p, string())
{}


docstring const InsetPrintNomencl::getScreenLabel(Buffer const &) const
{
	return _("Glossary");
}


int InsetPrintNomencl::docbook(Buffer const & buf, odocstream & os,
		OutputParams const &) const
{
	os << "<glossary>\n";
	int newlines = 2;
	for (InsetIterator it = inset_iterator_begin(buf.inset()); it;) {
		if (it->lyxCode() == InsetBase::NOMENCL_CODE) {
			newlines += static_cast<InsetNomencl const &>(*it).docbookGlossary(os);
			++it;
		} else if(it->lyxCode() == InsetBase::NOTE_CODE &&
		          static_cast<InsetNote const &>(*it).params().type == InsetNoteParams::Note) {
			// Don't output anything nested in note insets
			size_t const depth = it.depth();
			++it;
			while (it.depth() > depth)
				++it;
		} else {
			++it;
		}
	}
	os << "</glossary>\n";
	return newlines;
}


void InsetPrintNomencl::validate(LaTeXFeatures & features) const
{
	features.require("nomencl");
}


InsetBase::Code InsetPrintNomencl::lyxCode() const
{
	return InsetBase::NOMENCL_PRINT_CODE;
}


} // namespace lyx
