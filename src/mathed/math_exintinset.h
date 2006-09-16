// -*- C++ -*-
/**
 * \file math_exintinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_EXINTINSET_H
#define MATH_EXINTINSET_H


// \int_l^u f(x) dx in one block (as opposed to 'f','(','x',')' or 'f','x')
// or \sum, \prod...  for interfacing external programs

#include "math_nestinset.h"

#include <string>


// cell(0) is stuff before the 'd', cell(1) the stuff after
class InsetMathExInt : public InsetMathNest {
public:
	///
	explicit InsetMathExInt(std::string const & name_);
	///
	void symbol(std::string const &);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;

	///
	void normalize(NormalStream &) const;
	///
	void maxima(MaximaStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void write(WriteStream & os) const;
private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	bool hasScripts() const;

	///
	std::string symbol_;
};

#endif
