// -*- C++ -*-
#ifndef MATH_ENVINSET_H
#define MATH_ENVINSET_H

#include "math_nestinset.h"
#include "math_metricsinfo.h"


/** Environtments � la \begin{something}...\end{something}
 *  \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
*/

class MathEnvInset : public MathNestInset {
public:
	///
	MathEnvInset(string const & name_);
	///
	MathInset * clone() const;
	///
	void draw(MathPainterInfo &, int x, int y) const;
	///
	void write(WriteStream & os) const;
	/// write normalized content
	void normalize(NormalStream & ns) const;
	///
	void metrics(MathMetricsInfo & mi) const;
	///
	void infoize(std::ostream & os) const;

private:
	/// name of that environment
	string name_;
};

#endif
