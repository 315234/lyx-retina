// -*- C++ -*-
/**
 * \file math_fracbase.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_FRACBASE_H
#define MATH_FRACBASE_H

#include "math_nestinset.h"


class MathFracbaseInset : public MathNestInset {
public:
	///
	MathFracbaseInset();
	///
	bool idxUpDown(LCursor &, bool up) const;
	///
	bool idxLeft(LCursor &) const;
	///
	bool idxRight(LCursor &) const;
};

#endif
