// -*- C++ -*-
/**
 * \file ControlCitation.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef CONTROLCITATION_H
#define CONTROLCITATION_H


#include "ControlCommand.h"
#include "biblio.h" // biblio::InfoMap

/** A controller for Citation dialogs.
 */
class ControlCitation : public ControlCommand {
public:
	///
	ControlCitation(Dialog &);

	///
	virtual bool initialiseParams(string const & data);
	/// clean-up on hide.
	virtual void clearParams();

	/** Disconnect from the inset when the Apply button is pressed.
	 *  Allows easy insertion of multiple citations.
	 */
	virtual bool disconnectOnApply() const { return true; }

	/// Returns a reference to the map of stored keys
	biblio::InfoMap const & bibkeysInfo() const;

	///
	bool usingNatbib() const;
	/// Possible citations based on this key
	std::vector<string> const getCiteStrings(string const & key) const;

	/// available CiteStyle-s (depends on availability of Natbib
	static std::vector<biblio::CiteStyle> const & getCiteStyles() {
		return citeStyles_;
	}
private:
	/// The info associated with each key
	biblio::InfoMap bibkeysInfo_;

	///
	static std::vector<biblio::CiteStyle> citeStyles_;
};


#endif // CONTROLCITATION_H
