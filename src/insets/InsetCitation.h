// -*- C++ -*-
/**
 * \file InsetCitation.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 * \author Herbert Voß
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_CITATION_H
#define INSET_CITATION_H

#include "InsetCommand.h"

#include "Citation.h"

namespace lyx {

/////////////////////////////////////////////////////////////////////////
//
// InsetCitation
//
/////////////////////////////////////////////////////////////////////////

/// Used to insert citations
class InsetCitation : public InsetCommand
{
public:
	///
	InsetCitation(Buffer * buf, InsetCommandParams const &);

	/// \name Public functions inherited from Inset class
	//@{
	///
	bool isLabeled() const { return true; }
	///
	bool hasSettings() const { return true; }
	///
	docstring toolTip(BufferView const & bv, int x, int y) const;
	///
	InsetCode lyxCode() const { return CITE_CODE; }
	///
	void latex(otexstream &, OutputParams const &) const;
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring xhtml(XHTMLStream &, OutputParams const &) const;
	///
	void toString(odocstream &) const;
	///
	void forToc(docstring &, size_t) const;
	///
	void validate(LaTeXFeatures &) const;
	///
	void updateBuffer(ParIterator const & it, UpdateType);
	///
	void addToToc(DocIterator const &) const;
	///
	docstring contextMenuName() const;
	//@}

	/// \name Static public methods obligated for InsetCommand derived classes
	//@{
	///
	static ParamInfo const & findInfo(std::string const &);
	// FIXME This is the locus of the design problem we have.
	// It really ought to do what default_cite_command() does,
	// but to do that it needs to know what CiteEngine we are
	// using.
	static std::string defaultCommand() { return "cite"; }
	///
	static bool isCompatibleCommand(std::string const & cmd);
	//@}

private:
	/// tries to make a pretty label and makes a basic one if not
	docstring generateLabel(bool for_xhtml = false) const;
	/// makes a pretty label
	docstring complexLabel(bool for_xhtml = false) const;
	/// makes a very basic label, in case we can't make a pretty one
	docstring basicLabel(bool for_xhtml = false) const;

	/// \name Private functions inherited from Inset class
	//@{
	///
	Inset * clone() const { return new InsetCitation(*this); }
	//@}

	/// \name Private functions inherited from InsetCommand class
	//@{
	///
	docstring screenLabel() const;
	//@}

	/// we'll eventually want to be able to get info on this from the 
	/// various CiteEngines
	static ParamInfo param_info_;

	///
	class Cache {
	public:
		///
		Cache() : engine(ENGINE_BASIC), params(CITE_CODE) {}
		///
		CiteEngine engine;
		///
		InsetCommandParams params;
		///
		docstring generated_label;
		///
		docstring screen_label;
	};
	///
	mutable Cache cache;
};


} // namespace lyx

#endif // INSET_CITATION_H
