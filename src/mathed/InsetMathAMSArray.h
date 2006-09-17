// -*- C++ -*-
/**
 * \file InsetMathAMSArray.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_AMSARRAYINSET_H
#define MATH_AMSARRAYINSET_H

#include "InsetMathGrid.h"


/// Inset for things like [pbvV]matrix, psmatrix etc
class InsetMathAMSArray : public InsetMathGrid {
public:
	///
	InsetMathAMSArray(std::string const & name, int m, int n);
	///
	InsetMathAMSArray(std::string const & name);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pain, int x, int y) const;
	///
	InsetMathAMSArray * asAMSArrayInset() { return this; }
	///
	InsetMathAMSArray const * asAMSArrayInset() const { return this; }

	///
	bool getStatus(LCursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const;
	///
	void write(WriteStream & os) const;
	///
	void infoize(std::ostream & os) const;
	///
	void normalize(NormalStream &) const;
	///
	void validate(LaTeXFeatures & features) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	char const * name_left() const;
	///
	char const * name_right() const;

	///
	std::string name_;
};

#endif
