// -*- C++ -*-
/**
 * \file InsetMathBox.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_BOXINSET_H
#define MATH_BOXINSET_H

#include "InsetMathNest.h"

#include <string>


namespace lyx {


class Font;

/// Support for \\mbox

class InsetMathBox : public InsetMathNest {
public:
	///
	explicit InsetMathBox(docstring const & name);
	///
	mode_type currentMode() const { return TEXT_MODE; }
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream & ns) const;
	///
	void infoize(odocstream & os) const;

private:
	virtual std::auto_ptr<Inset> doClone() const;
	///
	docstring name_;
};



} // namespace lyx

#endif
