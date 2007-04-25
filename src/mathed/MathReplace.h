// -*- C++ -*-
/**
 * \file ReplaceData.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_REPLACE_H
#define MATH_REPLACE_H


#include "MathArray.h"


namespace lyx {

class ReplaceData {
public:
	///
	MathArray from;
	///
	MathArray to;
};


} // namespace lyx

#endif
