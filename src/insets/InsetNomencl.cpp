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
#include "Font.h"
#include "Encoding.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "InsetIterator.h"
#include "InsetList.h"
#include "LaTeXFeatures.h"
#include "Length.h"
#include "LyX.h"
#include "OutputParams.h"
#include "output_xhtml.h"
#include "sgml.h"

#include "frontends/FontMetrics.h"

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

InsetNomencl::InsetNomencl(Buffer * buf, InsetCommandParams const & p)
	: InsetCommand(buf, p, "nomenclature"),
	  nomenclature_entry_id(sgml::uniqueID(from_ascii("nomen")))
{}


ParamInfo const & InsetNomencl::findInfo(string const & /* cmdName */)
{
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		param_info_.add("prefix", ParamInfo::LATEX_OPTIONAL);
		param_info_.add("symbol", ParamInfo::LATEX_REQUIRED,
				ParamInfo::HANDLING_LATEXIFY);
		param_info_.add("description", ParamInfo::LATEX_REQUIRED,
				ParamInfo::HANDLING_LATEXIFY);
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


docstring InsetNomencl::xhtml(XHTMLStream &, OutputParams const &) const
{
	return docstring();
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

InsetPrintNomencl::InsetPrintNomencl(Buffer * buf, InsetCommandParams const & p)
	: InsetCommand(buf, p, "nomencl_print")
{}


ParamInfo const & InsetPrintNomencl::findInfo(string const & /* cmdName */)
{
	// The symbol width is set via nomencl's \nomlabelwidth in 
	// InsetPrintNomencl::latex and not as optional parameter of
	// \printnomenclature
	static ParamInfo param_info_;
	if (param_info_.empty()) {
		// how is the width set?
		// values: none|auto|custom
		param_info_.add("set_width", ParamInfo::LYX_INTERNAL);
		// custom width
		param_info_.add("width", ParamInfo::LYX_INTERNAL);
	}
	return param_info_;
}


docstring InsetPrintNomencl::screenLabel() const
{
	return _("Nomenclature");
}


void InsetPrintNomencl::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	switch (cmd.action()) {

	case LFUN_INSET_MODIFY: {
		InsetCommandParams p(NOMENCL_PRINT_CODE);
		// FIXME UNICODE
		InsetCommand::string2params("nomencl_print",
			to_utf8(cmd.argument()), p);
		if (p.getCmdName().empty()) {
			cur.noUpdate();
			break;
		}
		setParams(p);
		break;
	}

	default:
		InsetCommand::doDispatch(cur, cmd);
		break;
	}
}


bool InsetPrintNomencl::getStatus(Cursor & cur, FuncRequest const & cmd,
	FuncStatus & status) const
{
	switch (cmd.action()) {

	case LFUN_INSET_DIALOG_UPDATE:
	case LFUN_INSET_MODIFY:
		status.setEnabled(true);
		return true;

	default:
		return InsetCommand::getStatus(cur, cmd, status);
	}
}


docstring InsetPrintNomencl::xhtml(XHTMLStream &, OutputParams const &) const
{
	return docstring();
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


namespace {
docstring nomenclWidest(Buffer const & buffer, OutputParams const & runparams)
{
	// nomenclWidest() determines and returns the widest used
	// nomenclature symbol in the document

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
			docstring const symbol =
				nomencl->getParam("symbol");
			// This is only an approximation,
			// but the best we can get.
			int const wx = use_gui ?
				theFontMetrics(Font()).width(symbol) :
				symbol.size();
			if (wx > w) {
				w = wx;
				symb = symbol;
			}
		}
	}
	// return the widest (or an empty) string
	if (symb.empty())
		return symb;

	// we have to encode the string properly
	docstring latex_symb;
	for (size_t n = 0; n < symb.size(); ++n) {
		try {
			latex_symb += runparams.encoding->latexChar(symb[n]);
		} catch (EncodingException & /* e */) {
			if (runparams.dryrun) {
				latex_symb += "<" + _("LyX Warning: ")
					   + _("uncodable character") + " '";
				latex_symb += docstring(1, symb[n]);
				latex_symb += "'>";
			}
		}
	}
	return latex_symb;
}
} // namespace anon


int InsetPrintNomencl::latex(odocstream & os, OutputParams const & runparams_in) const
{
	OutputParams runparams = runparams_in;
	int lines = 0;
	if (getParam("set_width") == "auto") {
		docstring widest = nomenclWidest(buffer(), runparams);
		// Set the label width via nomencl's command \nomlabelwidth.
		// This must be output before the command \printnomenclature
		if (!widest.empty()) {
			os << "\\settowidth{\\nomlabelwidth}{"
			   << widest
			   << "}\n";
			++lines;
		}
	} else if (getParam("set_width") == "custom") {
		// custom length as optional arg of \printnomenclature
		string const width =
			Length(to_ascii(getParam("width"))).asLatexString();
		os << '\\'
		   << from_ascii(getCmdName())
		   << '['
		   << from_ascii(width)
		   << "]{}";
		return lines;
	}
	// output the command \printnomenclature
	os << getCommand(runparams);
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


docstring InsetPrintNomencl::contextMenu(BufferView const &, int, int) const
{
	return from_ascii("context-nomenclprint");
}


} // namespace lyx
