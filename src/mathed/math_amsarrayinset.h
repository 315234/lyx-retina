// -*- C++ -*-
/**
 * \file math_amsarrayinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_AMSARRAYINSET_H
#define MATH_AMSARRAYINSET_H

#include "math_gridinset.h"


/// Inset for things like [pbvV]matrix, psmatrix etc
class MathAMSArrayInset : public MathGridInset {
public:
	///
	MathAMSArrayInset(string const & name, int m, int n);
	///
	MathAMSArrayInset(string const & name);
	///
	virtual std::auto_ptr<InsetBase> clone() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pain, int x, int y) const;
	///
	MathAMSArrayInset * asAMSArrayInset() { return this; }
	///
	MathAMSArrayInset const * asAMSArrayInset() const { return this; }

	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream &) const;

private:
	///
	char const * name_left() const;
	///
	char const * name_right() const;

	///
	string name_;
};

#endif
