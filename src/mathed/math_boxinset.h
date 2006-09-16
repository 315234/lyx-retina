// -*- C++ -*-
/**
 * \file math_boxinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_BOXINSET_H
#define MATH_BOXINSET_H

#include "math_nestinset.h"

#include <string>


class LyXFont;

/// Support for \\mbox

class InsetMathBox : public InsetMathNest {
public:
	///
	explicit InsetMathBox(std::string const & name);
	///
	mode_type currentMode() const { return TEXT_MODE; }
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream & ns) const;
	///
	void infoize(std::ostream & os) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	std::string name_;
};


#endif
