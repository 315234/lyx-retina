// -*- C++ -*-

/**
 *  \file button_inset.h
 *
 *  This file is part of LyX, the document processor.
 *  Licence details can be found in the file COPYING.
 *
 *  \author Andr� P�nitz
 *
 *  Full author contact details are available in file CREDITS.
 */

#ifndef BUTTON_INSET_H
#define BUTTON_INSET_H


#include "math_nestinset.h"

/// try to implement the button-like insets "natively" for mathed
class ButtonInset: public MathNestInset {
public:
	///
	ButtonInset();
	///
	Dimension metrics(MetricsInfo & mi) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;

protected:
	/// This should provide the text for the button
	virtual string screenLabel() const = 0;
};

#endif
