/**
 * \file InsetMathMatrix.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathMatrix.h"
#include "MathData.h"
#include "MathStream.h"

#include "support/convert.h"

using namespace std;

namespace lyx {

InsetMathMatrix::InsetMathMatrix(InsetMathGrid const & p, 
			docstring const & left, docstring const & right)
	: InsetMathGrid(p), left_(left), right_(right)
{}


Inset * InsetMathMatrix::clone() const
{
	return new InsetMathMatrix(*this);
}


void InsetMathMatrix::write(WriteStream & os) const
{
	InsetMathGrid::write(os);
}


void InsetMathMatrix::normalize(NormalStream & os) const
{
	InsetMathGrid::normalize(os);
}


void InsetMathMatrix::maple(MapleStream & os) const
{
	os << "matrix(" << int(nrows()) << ',' << int(ncols()) << ",[";
	for (idx_type idx = 0; idx < nargs(); ++idx) {
		if (idx)
			os << ',';
		os << cell(idx);
	}
	os << "])";
}


void InsetMathMatrix::maxima(MaximaStream & os) const
{
	os << "matrix(";
	for (row_type row = 0; row < nrows(); ++row) {
		if (row)
			os << ',';
		os << '[';
		for (col_type col = 0; col < ncols(); ++col) {
			if (col)
				os << ',';
			os << cell(index(row, col));
		}
		os << ']';
	}
	os << ')';
}


void InsetMathMatrix::mathematica(MathematicaStream & os) const
{
	os << '{';
	for (row_type row = 0; row < nrows(); ++row) {
		if (row)
			os << ',';
		os << '{';
		for (col_type col = 0; col < ncols(); ++col) {
			if (col)
				os << ',';
			os << cell(index(row, col));
		}
		os << '}';
	}
	os << '}';
}


void InsetMathMatrix::mathmlize(MathStream & os) const
{
	os << "<mo form='prefix' fence='true' stretchy='true' symmetric='true' lspace='thinmathspace'>"
	   << left_ << "</mo>";
	os << MTag("mtable");
	for (row_type row = 0; row < nrows(); ++row) {
		os << MTag("mtr");
		for (col_type col = 0; col < ncols(); ++col)
			os << MTag("mtd") << cell(index(row, col)) << ETag("mtd");
		os << ETag("mtr");
	}
	os << ETag("mtable");
	os << "<mo form='postfix' fence='true' stretchy='true' symmetric='true' lspace='thinmathspace'>"
	   << right_ << "</mo>";
}


void InsetMathMatrix::htmlize(HtmlStream & os) const
{
	os << MTag("table", "class='matrix'") << '\n';

	// we do not print the delimiters but instead try to hack them
	string const rows = convert<string>(nrows());
	string const lattrib = 
			"class='ldelim' rowspan='" + rows + "'";
	string const rattrib = 
			"class='rdelim' rowspan='" + rows + "'";
	
	for (row_type row = 0; row < nrows(); ++row) {
		os << MTag("tr") << '\n';
		if (row == 0)
			os << MTag("td", lattrib) << ETag("td") << '\n';
		for (col_type col = 0; col < ncols(); ++col)
			os << MTag("td") << cell(index(row, col)) << ETag("td") << '\n';
		if (row == 0)
			os << MTag("td", rattrib) << ETag("td") << '\n';
		os << ETag("tr") << '\n';
	}
	os << ETag("table") << '\n';
}


void InsetMathMatrix::octave(OctaveStream & os) const
{
	os << '[';
	for (row_type row = 0; row < nrows(); ++row) {
		if (row)
			os << ';';
		os << '[';
		for (col_type col = 0; col < ncols(); ++col)
			os << cell(index(row, col)) << ' ';
		os << ']';
	}
	os << ']';
}


} // namespace lyx
