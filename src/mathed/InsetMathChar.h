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


namespace lyx {

/// The base character inset.
class InsetMathChar : public InsetMath {
public:
	///
	explicit InsetMathChar(char c);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter &, int x, int y) const;
	///
	int width() const { return width_; }

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream & ns) const;
	///
	void octave(OctaveStream & os) const;
	///
	void mathmlize(MathMLStream & ms) const;
	/// identifies Charinsets
	InsetMathChar const * asCharInset() const { return this; }
	///
	char getChar() const { return char_; }
	///
	bool isRelOp() const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	/// the character
	char char_;
	/// cached width
	mutable int width_;
};

} // namespace lyx

#endif
