// -*- C++ -*-
/**
 * \file math_envinset.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_ENVINSET_H
#define MATH_ENVINSET_H

#include "math_nestinset.h"

#include <string>


/// Environtments � la \begin{something}...\end{something}
class InsetMathEnv : public InsetMathNest {
public:
	///
	InsetMathEnv(std::string const & name_);
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	void write(WriteStream & os) const;
	/// write normalized content
	void normalize(NormalStream & ns) const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void infoize(std::ostream & os) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	/// name of that environment
	std::string name_;
};

#endif
