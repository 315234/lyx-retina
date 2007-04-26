// -*- C++ -*-
/**
 * \file ControlTabular.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 *
 * This is pretty icky, we should really be able to use
 * ControlInset. We can't because there are no params for
 * tabular inset.
 */

#ifndef CONTROLTABULAR_H
#define CONTROLTABULAR_H

#include "Dialog.h"
#include "LyXTabular.h"

namespace lyx {
namespace frontend {

class ControlTabular : public Dialog::Controller {
public:

	ControlTabular(Dialog &);
	///
	virtual bool initialiseParams(std::string const & data);
	/// clean-up on hide.
	virtual void clearParams();
	/// We use set() instead.
	virtual void dispatchParams() {};
	///
	virtual bool isBufferDependent() const { return true; }
	///
	virtual kb_action getLfun() const { return LFUN_TABULAR_FEATURE; }

	///
	LyXTabular::idx_type getActiveCell() const;
	/// get the contained tabular
	LyXTabular const & tabular() const;
	/// return true if units should default to metric
	bool useMetricUnits() const;
	/// set a parameter
	void set(LyXTabular::Feature, std::string const & arg = std::string());

	/// borders
	void toggleTopLine();
	void toggleBottomLine();
	void toggleLeftLine();
	void toggleRightLine();

	void setSpecial(std::string const & special);

	void setWidth(std::string const & width);

	void toggleMultiColumn();

	void rotateTabular(bool yes);
	void rotateCell(bool yes);

	enum HALIGN { LEFT, RIGHT, CENTER, BLOCK };

	void halign(HALIGN h);

	enum VALIGN { TOP, MIDDLE, BOTTOM };

	void valign(VALIGN h);

	void booktabs(bool yes);

	void longTabular(bool yes);

private:
	///
	LyXTabular::idx_type active_cell_;
	///
	boost::scoped_ptr<LyXTabular> params_;
};

} // namespace frontend
} // namespace lyx

#endif // CONTROLTABULAR_H
