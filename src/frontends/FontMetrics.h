// -*- C++ -*-
/**
 * \file FontMetrics.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author John Levon
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef FONT_METRICS_H
#define FONT_METRICS_H

#include "support/docstring.h"

/**
 * A class holding helper functions for determining
 * the screen dimensions of fonts.
 *
 * The geometry is the standard typographical geometry,
 * as follows :
 *
 * --------------+------------------<maxAscent
 *               |          |
 *               <-------> (right bearing)
 *               <-> (left bearing)
 * char ascent>___          |
 *               ^   oooo   |  oooo
 *   origin>____ |  oo  oo  | oo  oo
 *              \|  oo  oo  | oo  oo
 * --------------+---ooooo--|--oooo-<baseline
 *               |      oo  |
 * char          |  oo  oo  |
 * descent>______|   oooo   |
 *               <-  width ->
 * --------------+----------+-------<maxDescent
 *
 */

namespace lyx {
namespace frontend {

class FontMetrics
{
public:
	virtual ~FontMetrics() {}

	/// return the maximum ascent of the font
	virtual int maxAscent() const = 0;
	/// return the maximum descent of the font
	virtual int maxDescent() const = 0;
	/// return the ascent of the char in the font
	virtual int ascent(lyx::char_type c) const = 0;
	/// return the descent of the char in the font
	virtual int descent(lyx::char_type c) const = 0;
	/// return the left bearing of the char in the font
	virtual int lbearing(lyx::char_type c) const = 0;
	/// return the right bearing of the char in the font
	virtual int rbearing(lyx::char_type c) const = 0;
	/// return the width of the string in the font
	virtual int width(lyx::char_type const * s, size_t n) const = 0;
	/// FIXME ??
	virtual int signedWidth(lyx::docstring const & s) const = 0;
	/**
	 * fill in width,ascent,descent with the values for the
	 * given string in the font.
	 */
	virtual void rectText(lyx::docstring const & str,
		int & width,
		int & ascent,
		int & descent) const = 0;
	/**
	 * fill in width,ascent,descent with the values for the
	 * given string in the font for a button.
	 */
	virtual void buttonText(lyx::docstring const & str,
		int & width,
		int & ascent,
		int & descent) const = 0;

	/// return the maximum descent of the font
	inline int maxHeight() const {
		return maxAscent() + maxDescent();
	}

	/// return the descent of the char in the font
	inline int height(lyx::char_type c) const
	{
		return ascent(c) + descent(c);
	}

	/// return the inner width of the char in the font
	inline int center(lyx::char_type c) const {
		return (rbearing(c) - lbearing(c)) / 2;
	}

	/// return the width of the char in the font
	inline int width(lyx::char_type c) const
	{
		lyx::char_type tmp[2] = { c, L'\0'};
		return width(tmp, 1);
	}

	/// return the width of the string in the font
	inline int width(lyx::docstring const & s) const
	{
	    return s.empty() ? 0 : width(s.data(), s.length());
	}
};

} // namespace frontend
} // namespace lyx

#endif // FONT_METRICS_H
