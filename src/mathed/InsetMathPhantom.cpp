/**
 * \file InsetMathPhantom.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Georg Baum
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathPhantom.h"
#include "MathStream.h"
#include "MathStream.h"

#include "Color.h"

#include "frontends/Painter.h"

#include "support/std_ostream.h"


namespace lyx {


InsetMathPhantom::InsetMathPhantom(Kind k)
	: InsetMathNest(1), kind_(k)
{}


Inset * InsetMathPhantom::clone() const
{
	return new InsetMathPhantom(*this);
}


void InsetMathPhantom::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(0).metrics(mi, dim);
	metricsMarkers(dim);
	dim_ = dim;
}


void InsetMathPhantom::draw(PainterInfo & pi, int x, int y) const
{
	static int const arrow_size = 4;

	// We first draw the text and then an arrow
	Color_color const origcol = pi.base.font.color();
	pi.base.font.setColor(Color::special);
	cell(0).draw(pi, x + 1, y);
	pi.base.font.setColor(origcol);

	if (kind_ == phantom || kind_ == vphantom) {
		// y1---------
		//           / \.
		// y2-----  / | \.
		//            |
		//            |
		// y3-----  \ | /
		//           \ /
		// y4---------
		//          | | |
		//         /  |  \.
		//        x1  x2 x3

		int const x2 = x + dim_.wid / 2;
		int const x1 = x2 - arrow_size;
		int const x3 = x2 + arrow_size;

		int const y1 = y - dim_.asc;
		int const y2 = y1 + arrow_size;
		int const y4 = y + dim_.des;
		int const y3 = y4 - arrow_size;

		// top arrow
		pi.pain.line(x2, y1, x1, y2, Color::added_space);
		pi.pain.line(x2, y1, x3, y2, Color::added_space);

		// bottom arrow
		pi.pain.line(x2, y4, x1, y3, Color::added_space);
		pi.pain.line(x2, y4, x3, y3, Color::added_space);

		// joining line
		pi.pain.line(x2, y1, x2, y4, Color::added_space);
	}

	if (kind_ == phantom || kind_ == hphantom) {
		// y1----   /          \.
		//        /              \.
		// y2--- <---------------->
		//        \              /
		// y3----   \          /
		//       |   |        |   |
		//      x1  x2       x3  x4

		int const x1 = x;
		int const x2 = x + arrow_size;
		int const x4 = x + dim_.wid;
		int const x3 = x4 - arrow_size;

		int const y2 = y + (dim_.des - dim_.asc) / 2;
		int const y1 = y2 - arrow_size;
		int const y3 = y2 + arrow_size;

		// left arrow
		pi.pain.line(x1, y2, x2, y3, Color::added_space);
		pi.pain.line(x1, y2, x2, y1, Color::added_space);

		// right arrow
		pi.pain.line(x4, y2, x3, y3, Color::added_space);
		pi.pain.line(x4, y2, x3, y1, Color::added_space);

		// joining line
		pi.pain.line(x1, y2, x4, y2, Color::added_space);
	}

	drawMarkers(pi, x, y);
}


void InsetMathPhantom::write(WriteStream & os) const
{
	switch (kind_) {
	case phantom:
		os << "\\phantom{";
		break;
	case vphantom:
		os << "\\vphantom{";
		break;
	case hphantom:
		os << "\\hphantom{";
		break;
	}
	os << cell(0) << '}';
}


void InsetMathPhantom::normalize(NormalStream & os) const
{
	switch (kind_) {
	case phantom:
		os << "[phantom ";
		break;
	case vphantom:
		os << "[vphantom ";
		break;
	case hphantom:
		os << "[hphantom ";
		break;
	}
	os << cell(0) << ']';
}


void InsetMathPhantom::infoize(odocstream & os) const
{
	switch (kind_) {
	case phantom:
		os << "Phantom";
		break;
	case vphantom:
		os << "Vphantom";
		break;
	case hphantom:
		os << "Hphantom";
		break;
	}
}


} // namespace lyx
