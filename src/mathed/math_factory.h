// -*- C++ -*-
/**
 * \file math_factory.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_FACTORY_H
#define MATH_FACTORY_H

#include <string>

class MathAtom;
class MathArray;


MathAtom createInsetMath(std::string const &);

/** Fills ar with the contents of str.
 *  str is created by the frontend dialog's and returned to the LyX core.
 *  The function returns true if successful.
 */
bool createInsetMath_fromDialogStr(std::string const &, MathArray &);

#endif
