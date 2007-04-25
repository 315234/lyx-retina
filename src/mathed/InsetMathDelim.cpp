/**
 * \file InsetMathDelim.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathDelim.h"
#include "MathData.h"
#include "MathStream.h"
#include "MathStream.h"
#include "MathSupport.h"

#include "frontends/FontMetrics.h"

namespace lyx {


using std::string;
using std::max;
using std::auto_ptr;


static docstring convertDelimToLatexName(docstring const & name)
{
	if (name.size() == 1) {
		char_type const c = name[0];
		if (c == '<' || c == '(' || c == '[' || c == '.' 
		    || c == '>' || c == ')' || c == ']' || c == '/' || c == '|')
			return name;
	}
	return '\\' + name + ' ';
}


InsetMathDelim::InsetMathDelim(docstring const & l, docstring const & r)
	: InsetMathNest(1), left_(l), right_(r)
{}


InsetMathDelim::InsetMathDelim
		(docstring const & l, docstring const & r, MathArray const & ar)
	: InsetMathNest(1), left_(l), right_(r)
{
	cell(0) = ar;
}


auto_ptr<InsetBase> InsetMathDelim::doClone() const
{
	return auto_ptr<InsetBase>(new InsetMathDelim(*this));
}


void InsetMathDelim::write(WriteStream & os) const
{
	os << "\\left" << convertDelimToLatexName(left_) << cell(0)
	   << "\\right" << convertDelimToLatexName(right_);
}


void InsetMathDelim::normalize(NormalStream & os) const
{
	os << "[delim " << convertDelimToLatexName(left_) << ' '
	   << convertDelimToLatexName(right_) << ' ' << cell(0) << ']';
}


bool InsetMathDelim::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(0).metrics(mi);
	Dimension t = theFontMetrics(mi.base.font).dimension('I');
	int h0 = (t.asc + t.des) / 2;
	int a0 = max(cell(0).ascent(), t.asc)   - h0;
	int d0 = max(cell(0).descent(), t.des)  + h0;
	dw_ = cell(0).height() / 5;
	if (dw_ > 8)
		dw_ = 8;
	if (dw_ < 4)
		dw_ = 4;
	dim.wid = cell(0).width() + 2 * dw_ + 8;
	dim.asc = max(a0, d0) + h0;
	dim.des = max(a0, d0) - h0;
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathDelim::draw(PainterInfo & pi, int x, int y) const
{
	int const b = y - dim_.asc;
	cell(0).draw(pi, x + dw_ + 4, y);
	mathed_draw_deco(pi, x + 4, b, dw_, dim_.height(), left_);
	mathed_draw_deco(pi, x + dim_.width() - dw_ - 4,
		b, dw_, dim_.height(), right_);
	setPosCache(pi, x, y);
}


bool InsetMathDelim::isParenthesis() const
{
	return left_ == "(" && right_ == ")";
}


bool InsetMathDelim::isBrackets() const
{
	return left_ == "[" && right_ == "]";
}


bool InsetMathDelim::isAbs() const
{
	return left_ == "|" && right_ == "|";
}


void InsetMathDelim::maple(MapleStream & os) const
{
	if (isAbs()) {
		if (cell(0).size() == 1 && cell(0).front()->asMatrixInset())
			os << "linalg[det](" << cell(0) << ')';
		else
			os << "abs(" << cell(0) << ')';
	}
	else
		os << left_ << cell(0) << right_;
}


void InsetMathDelim::maxima(MaximaStream & os) const
{
	if (isAbs()) {
		if (cell(0).size() == 1 && cell(0).front()->asMatrixInset())
			os << "determinant(" << cell(0) << ')';
		else
			os << "abs(" << cell(0) << ')';
	}
	else
		os << left_ << cell(0) << right_;
}


void InsetMathDelim::mathematica(MathematicaStream & os) const
{
	if (isAbs()) {
		if (cell(0).size() == 1 && cell(0).front()->asMatrixInset())
			os << "Det" << cell(0) << ']';
		else
			os << "Abs[" << cell(0) << ']';
	}
	else
		os << left_ << cell(0) << right_;
}


void InsetMathDelim::mathmlize(MathStream & os) const
{
	os << "<fenced open=\"" << left_ << "\" close=\""
		<< right_ << "\">" << cell(0) << "</fenced>";
}


void InsetMathDelim::octave(OctaveStream & os) const
{
	if (isAbs())
		os << "det(" << cell(0) << ')';
	else
		os << left_ << cell(0) << right_;
}


} // namespace lyx
