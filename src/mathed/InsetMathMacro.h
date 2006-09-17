// -*- C++ -*-
/**
 * \file InsetMathMacro.h
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
#include "MathMacroTable.h"


/// This class contains the data for a macro.
class MathMacro : public InsetMathNest {
public:
	/// A macro can be built from an existing template
	MathMacro(std::string const & name, int numargs);
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void drawExpanded(PainterInfo & pi, int x, int y) const;
	/// draw selection background
	void drawSelection(PainterInfo & pi, int x, int y) const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	/// get cursor position
	void cursorPos(CursorSlice const & sl, bool boundary, int & x, int & y) const;
	///
	InsetBase * editXY(LCursor & cur, int x, int y);
	///
	std::string name() const;
	///
	void setExpansion(MathArray const & exp, MathArray const & args) const;

	///
	void validate(LaTeXFeatures &) const;

	///
	void maple(MapleStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void infoize(std::ostream &) const;
	///
	void infoize2(std::ostream &) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	void updateExpansion() const;
	///
	void expand() const;

	/// name of macro
	std::string name_;
	/// the unexpanded macro defintition
	mutable MathArray tmpl_;
	/// the macro substituted with our args
	mutable MathArray expanded_;
};


#endif
