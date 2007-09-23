/**
 * \file InsetMathArray.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "LaTeXFeatures.h"
#include "InsetMathArray.h"
#include "MathData.h"
#include "MathParser.h"
#include "MathStream.h"

#include "support/lstrings.h"

#include <iterator>
#include <sstream>


namespace lyx {

using std::getline;
using std::istringstream;
using std::istream_iterator;
using std::vector;
using std::string;


InsetMathArray::InsetMathArray(docstring const & name, int m, int n)
	: InsetMathGrid(m, n), name_(name)
{}


InsetMathArray::InsetMathArray(docstring const & name, int m, int n,
		char valign, docstring const & halign)
	: InsetMathGrid(m, n, valign, halign), name_(name)
{}


InsetMathArray::InsetMathArray(docstring const & name, char valign,
		docstring const & halign)
	: InsetMathGrid(valign, halign), name_(name)
{}


InsetMathArray::InsetMathArray(docstring const & name, docstring const & str)
	: InsetMathGrid(1, 1), name_(name)
{
	vector< vector<string> > dat;
	istringstream is(to_utf8(str));
	string line;
	while (getline(is, line)) {
		istringstream ls(line);
		typedef istream_iterator<string> iter;
		vector<string> v = vector<string>(iter(ls), iter());
		if (v.size())
			dat.push_back(v);
	}

	for (row_type row = 1; row < dat.size(); ++row)
		addRow(0);
	for (col_type col = 1; col < dat[0].size(); ++col)
		addCol(0);
	for (row_type row = 0; row < dat.size(); ++row)
		for (col_type col = 0; col < dat[0].size(); ++col)
			mathed_parse_cell(cell(index(row, col)), from_utf8(dat[row][col]));
}


Inset * InsetMathArray::clone() const
{
	return new InsetMathArray(*this);
}


void InsetMathArray::metrics(MetricsInfo & mi, Dimension & dim) const
{
	ArrayChanger dummy(mi.base);
	InsetMathGrid::metrics(mi, dim);
	dim.wid += 6;
}


Dimension const InsetMathArray::dimension(BufferView const & bv) const
{
	Dimension dim = InsetMathGrid::dimension(bv);
	dim.wid += 6;
	return dim;
}


void InsetMathArray::draw(PainterInfo & pi, int x, int y) const
{
	setPosCache(pi, x, y);
	ArrayChanger dummy(pi.base);
	InsetMathGrid::drawWithMargin(pi, x, y, 4, 2);
}


void InsetMathArray::write(WriteStream & os) const
{
	if (os.fragile())
		os << "\\protect";
	os << "\\begin{" << name_ << '}';

	if (v_align_ == 't' || v_align_ == 'b')
		os << '[' << char(v_align_) << ']';
	os << '{' << halign() << "}\n";

	InsetMathGrid::write(os);

	if (os.fragile())
		os << "\\protect";
	os << "\\end{" << name_ << '}';
	// adding a \n here is bad if the array is the last item
	// in an \eqnarray...
}


void InsetMathArray::infoize(odocstream & os) const
{
	docstring name = name_;
	name[0] = support::uppercase(name[0]);
	os << name << ' ';
}


void InsetMathArray::normalize(NormalStream & os) const
{
	os << '[' << name_ << ' ';
	InsetMathGrid::normalize(os);
	os << ']';
}


void InsetMathArray::maple(MapleStream & os) const
{
	os << "array(";
	InsetMathGrid::maple(os);
	os << ')';
}


void InsetMathArray::validate(LaTeXFeatures & features) const
{
	if (name_ == "subarray")
		features.require("amsmath");
	InsetMathGrid::validate(features);
}


} // namespace lyx
