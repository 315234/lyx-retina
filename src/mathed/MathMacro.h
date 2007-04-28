// -*- C++ -*-
/**
 * \file MathMacro.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_MACRO_H
#define MATH_MACRO_H

#include "InsetMathNest.h"
#include "MathData.h"
#include "InsetMathNest.h"
#include "MacroTable.h"


namespace lyx {


/// This class contains the data for a macro.
class MathMacro : public InsetMathNest {
public:
	/// A macro can be built from an existing template
	MathMacro(docstring const & name, int numargs);
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void drawExpanded(PainterInfo & pi, int x, int y) const;
	/// draw selection background
	void drawSelection(PainterInfo & pi, int x, int y) const;
	/// draw decorations.
	void drawDecoration(PainterInfo & pi, int x, int y) const
	{ drawMarkers2(pi, x, y); }
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	/// get cursor position
	void cursorPos(BufferView const & bv, CursorSlice const & sl,
		bool boundary, int & x, int & y) const;
	///
	InsetBase * editXY(Cursor & cur, int x, int y);
	/// target pos when we enter the inset from the left by pressing "Right"
	bool idxFirst(Cursor &) const;
	/// target pos when we enter the inset from the right by pressing "Left"
	bool idxLast(Cursor &) const;
	///
	virtual bool notifyCursorLeaves(Cursor &);
	///
	docstring name() const;
	///
	void setExpansion(MathData const & exp, MathData const & args) const;

	///
	void validate(LaTeXFeatures &) const;

	///
	void maple(MapleStream &) const;
	///
	void mathmlize(MathStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void infoize(odocstream &) const;
	///
	void infoize2(odocstream &) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	void updateExpansion() const;
	///
	void expand() const;

	/// name of macro
	docstring name_;
	/// the unexpanded macro defintition
	mutable MathData tmpl_;
	/// the macro substituted with our args
	mutable MathData expanded_;
};




} // namespace lyx
#endif
