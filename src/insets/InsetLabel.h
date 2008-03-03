// -*- C++ -*-
/**
 * \file InsetLabel.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSET_LABEL_H
#define INSET_LABEL_H

#include "InsetCommand.h"


namespace lyx {

class InsetLabel : public InsetCommand {
public:
	///
	InsetLabel(InsetCommandParams const &);

	/// verify label and update references.
	/**
	  * Overloaded from Inset::initView.
	  **/
	void initView();

	///
	docstring screenLabel() const;
	///
	EDITABLE editable() const { return IS_EDITABLE; }
	///
	InsetCode lyxCode() const { return LABEL_CODE; }
	/// Appends \c list with this label
	void getLabelList(std::vector<docstring> & list) const;
	///
	int latex(odocstream &, OutputParams const &) const;
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	static ParamInfo const & findInfo(std::string const &);
	///
	static std::string defaultCommand() { return "label"; };
	///
	static bool isCompatibleCommand(std::string const & s) 
		{ return s == "label"; }
	///
	void updateLabels(ParIterator const & it);
	///
	void addToToc(ParConstIterator const &) const;
	///
	void update(docstring const & new_label, bool updaterefs = true);
protected:
	///
	void doDispatch(Cursor & cur, FuncRequest & cmd);
private:
	///
	Inset * clone() const { return new InsetLabel(*this); }
	///
	docstring screen_label_;
};


} // namespace lyx

#endif
