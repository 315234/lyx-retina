/**
 * \file InsetMathFracBase.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathFracBase.h"
#include "MathData.h"
#include "cursor.h"


namespace lyx {


InsetMathFracBase::InsetMathFracBase()
	: InsetMathNest(2)
{}


bool InsetMathFracBase::idxRight(LCursor &) const
{
	return false;
}


bool InsetMathFracBase::idxLeft(LCursor &) const
{
	return false;
}


bool InsetMathFracBase::idxUpDown(LCursor & cur, bool up) const
{
	InsetMath::idx_type target = !up; // up ? 0 : 1, since upper cell has idx 0
	if (cur.idx() == target)
		return false;
	cur.idx() = target;
	cur.pos() = cell(target).x2pos(cur.x_target());
	return true;
}


} // namespace lyx
