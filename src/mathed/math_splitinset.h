// -*- C++ -*-
/**
 * \file math_splitinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_SPLITINSET_H
#define MATH_SPLITINSET_H

#include "math_gridinset.h"


class InsetMathSplit : public InsetMathGrid {
public:
	///
	explicit InsetMathSplit(std::string const & name);

	///
	void draw(PainterInfo & pi, int x, int y) const;

	///
	bool getStatus(LCursor & cur, FuncRequest const & cmd,
		FuncStatus & flag) const;

	void write(WriteStream & os) const;
	///
	void infoize(std::ostream & os) const;
	///
	void validate(LaTeXFeatures & features) const;
	///
	int defaultColSpace(col_type) { return 0; }
	///
	char defaultColAlign(col_type);
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	std::string name_;
};

#endif
