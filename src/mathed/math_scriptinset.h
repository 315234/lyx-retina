// -*- C++ -*-
#ifndef MATH_SCRIPTINSET_H
#define MATH_SCRIPTINSET_H

#include "math_updowninset.h"

#ifdef __GNUG__
#pragma interface
#endif

/** Inset for super- and subscripts
    \author Andr� P�nitz
 */

class MathScriptInset : public MathUpDownInset {
public:
	///
	MathScriptInset(bool up, bool down);
	///
	MathInset * clone() const;
	///
	void WriteNormal(std::ostream &) const;
	/// Identifies ScriptInsets
	bool isUpDownInset() const { return true; }
	///
	void idxDelete(int & idx, bool & popit, bool & deleteit);
};

#endif
