/**
 * \file MathMacroArgument.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "MathMacroArgument.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "debug.h"


namespace lyx {

using std::endl;
using std::auto_ptr;
using std::size_t;
using std::vector;


MathMacroArgument::MathMacroArgument(size_t n)
	: number_(n)
{
	if (n < 1 || n > 9) {
		lyxerr << "MathMacroArgument::MathMacroArgument: wrong Argument id: "
			<< n << endl;
	}
	// The profiler tells us not to use
	// str_ = '#' + convert<docstring>(n);
	// so we do the conversion of n to ASCII manually.
	// This works because 1 <= n <= 9.
	str_.resize(2);
	str_[0] = '#';
	str_[1] = '0' + n;
}


auto_ptr<Inset> MathMacroArgument::doClone() const
{
	return auto_ptr<Inset>(new MathMacroArgument(*this));
}


void MathMacroArgument::write(WriteStream & os) const
{
	os << str_;
}


bool MathMacroArgument::metrics(MetricsInfo & mi, Dimension & dim) const
{
	mathed_string_dim(mi.base.font, str_, dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void MathMacroArgument::draw(PainterInfo & pi, int x, int y) const
{
	drawStrRed(pi, x, y, str_);
	setPosCache(pi, x, y);
}


void MathMacroArgument::normalize(NormalStream & os) const
{
	os << "[macroarg " << str_ << "] ";
}


} // namespace lyx
