// -*- C++ -*-
/**
 * \file math_biginset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_BIGINSET_H
#define MATH_BIGINSET_H

#include "math_diminset.h"

#include <string>

/// Inset for \\bigl & Co.
class InsetMathBig : public InsetMathDim {
public:
	///
	InsetMathBig(std::string const & name, std::string const & delim);
	///
	std::string name() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void write(WriteStream & os) const;
	///
	void normalize(NormalStream & os) const;
	///
	void infoize2(std::ostream & os) const;
	///
	static bool isBigInsetDelim(std::string const &);

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	size_type size() const;
	///
	double increase() const;

	/// \\bigl or what?
	std::string const name_;
	/// ( or [ or \\Vert...
	std::string const delim_;
};

#endif
