// -*- C++ -*-
/**
 * \file InsetMathFontOld.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_FONTOLDINSET_H
#define MATH_FONTOLDINSET_H

#include "InsetMathNest.h"


namespace lyx {


class latexkeys;

/// Old-style font changes
class InsetMathFontOld : public InsetMathNest {
public:
	///
	explicit InsetMathFontOld(latexkeys const * key);
	/// we are in text mode.
	mode_type currentMode() const { return TEXT_MODE; }
	/// we write extra braces in any case...
	bool extraBraces() const { return true; }
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;
	///
	void infoize(std::ostream & os) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	/// the font to be used on screen
	latexkeys const * key_;
};


} // namespace lyx
#endif
