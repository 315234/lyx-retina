// -*- C++ -*-
/**
 * \file InsetMathChar.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_CHARINSET_H
#define MATH_CHARINSET_H

#include "InsetMath.h"

#include "lyxfont.h"

namespace lyx {

/// The base character inset.
class InsetMathChar : public InsetMath {
public:
	///
	explicit InsetMathChar(char_type c);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter &, int x, int y) const;
	///
	int kerning() const { return kerning_; }

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream & ns) const;
	///
	void octave(OctaveStream & os) const;
	///
	void mathmlize(MathStream & ms) const;
	/// identifies Charinsets
	InsetMathChar const * asCharInset() const { return this; }
	///
	char_type getChar() const { return char_; }
	///
	bool isRelOp() const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	/// the character
	char_type char_;
	/// cached kerning for superscript
	mutable int kerning_;
	///
	mutable LyXFont font_cache_;
};

} // namespace lyx

#endif
