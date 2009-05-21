/**
 * \file InsetNomencl.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author O. U. Baran
 * \author Uwe Stöhr
 * \author Jürgen Spitzmüller
 *
 * Full author contact details are available in file CREDITS.
 */
#include <config.h>

#include "InsetNomencl.h"
#include "InsetNote.h"

#include "Buffer.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "InsetIterator.h"
#include "InsetList.h"
#include "LaTeXFeatures.h"
#include "MetricsInfo.h"
#include "sgml.h"

#include "support/docstream.h"
#include "support/gettext.h"
#include "support/lstrings.h"

using namespace std;
using namespace lyx::support;

namespace lyx {


/////////////////////////////////////////////////////////////////////
//
// InsetNomencl
//
/////////////////////////////////////////////////////////////////////

InsetNomencl::InsetNomencl(InsetCommandParams const & p)
	: InsetCommand(p, "nomenclature"),
	  nomenclature_entry_id(sgml::uniqueID(from_ascii("nomen")))
{}


ParamInfo const & InsetNomencl::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("prefix", ParamInfo::LATEX_OPTIONAL);
		param_info_.add("symbol", ParamInfo::LATEX_REQUIRED);
		param_info_.add("description", ParamInfo::LATEX_REQUIRED);
	}
	return param_info_;
}


docstring InsetNomencl::screenLabel() const
{
	size_t const maxLabelChars = 25;

	docstring label = _("Nom: ") + getParam("symbol");
	if (label.size() > maxLabelChars) {
		label.erase(maxLabelChars - 3);
		label += "...";
	}
	return label;
}


docstring InsetNomencl::toolTip(BufferView const & /*bv*/, int /*x*/, int /*y*/) const
{
	docstring tip = _("Nomenclature Symbol: ") + getParam("symbol") + "\n";
	tip += _("Description: ") + "\t"
		+ subst(getParam("description"), from_ascii("\\\\"), from_ascii("\n\t"));
	if (!getParam("prefix").empty())
		tip += "\n" + _("Sorting: ") + getParam("prefix");
	return tip;
}



int InsetNomencl::docbook(odocstream & os, OutputParams const &) const
{
	os << "<glossterm linkend=\"" << nomenclature_entry_id << "\">"
	   << sgml::escapeString(getParam("symbol"))
	   << "</glossterm>";
	return 0;
}


int InsetNomencl::docbookGlossary(odocstream & os) const
{
	os << "<glossentry id=\"" << nomenclature_entry_id << "\">\n"
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


/////////////////////////////////////////////////////////////////////
//
// InsetPrintNomencl
//
/////////////////////////////////////////////////////////////////////

InsetPrintNomencl::InsetPrintNomencl(InsetCommandParams const & p)
	: InsetCommand(p, "printnomenclature")
{}


ParamInfo const & InsetPrintNomencl::findInfo(string const & /* cmdName */)
{
	// there are no parameters to give because the symbol width is set via
	// nomencl's \nomlabelwidth in InsetPrintNomencl::latex and not as
	// optional parameter of \printnomenclature
	static ParamInfo param_info_;
	return param_info_;
}


docstring InsetPrintNomencl::screenLabel() const
{
	return _("Nomenclature");
}


int InsetPrintNomencl::docbook(odocstream & os, OutputParams const &) const
{
	os << "<glossary>\n";
	int newlines = 2;
	InsetIterator it = inset_iterator_begin(buffer().inset());
	while (it) {
		if (it->lyxCode() == NOMENCL_CODE) {
			newlines += static_cast<InsetNomencl const &>(*it).docbookGlossary(os);
			++it;
		} else if (!it->producesOutput()) {
			// Ignore contents of insets that are not in output
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


docstring nomenclWidest(Buffer const & buffer)
{
	// nomenclWidest() determines and returns the widest used nomenclature
	// symbol in the document

	int w = 0;
	docstring symb;
	InsetNomencl const * nomencl = 0;

	ParagraphList::const_iterator it = buffer.paragraphs().begin();
	ParagraphList::const_iterator end = buffer.paragraphs().end();

	for (; it != end; ++it) {
		if (it->insetList().empty())
			continue;
		InsetList::const_iterator iit = it->insetList().begin();
		InsetList::const_iterator eend = it->insetList().end();
		for (; iit != eend; ++iit) {
			Inset * inset = iit->inset;
			if (inset->lyxCode() != NOMENCL_CODE)
				continue;
			nomencl = static_cast<InsetNomencl const *>(inset);
			docstring const symbol = nomencl->getParam("symbol");
			int const wx = symbol.size();
			if (wx > w) {
				w = wx;
				symb = symbol;
			}
		}
	}

	// return the widest symbol
	return symb;
}


int InsetPrintNomencl::latex(odocstream & os, OutputParams const &) const
{
	int lines = 0;
	// this must be output before the command \printnomenclature
	docstring widest = nomenclWidest(buffer());
	if (!widest.empty()) {
		// set the label width via nomencl's command \nomlabelwidth
		os << "\\settowidth{\\nomlabelwidth}{";
		os << widest <<"}\n";
		++lines;
	}
	// output the command \printnomenclature
	os << getCommand();
	return lines;
}


void InsetPrintNomencl::validate(LaTeXFeatures & features) const
{
	features.require("nomencl");
}


InsetCode InsetPrintNomencl::lyxCode() const
{
	return NOMENCL_PRINT_CODE;
}


} // namespace lyx
