// -*- C++ -*-
/**
 * \file lyxlength.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Matthias Ettrich
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_LENGTH_H
#define LYX_LENGTH_H

#include "support/docstring.h"


namespace lyx {

// Solaris/x86 version 9 and earlier define these
#undef PC
#undef SP


/**
 * LyXLength - Represents latex length measurement
 */
class LyXLength {
public:
	/// length units
	enum UNIT {
		SP, ///< Scaled point (65536sp = 1pt) TeX's smallest unit.
		PT, ///< Point = 1/72.27in = 0.351mm
		BP, ///< Big point (72bp = 1in), also PostScript point
		DD, ///< Didot point = 1/72 of a French inch, = 0.376mm
		MM, ///< Millimeter = 2.845pt
		PC, ///< Pica = 12pt = 4.218mm
		CC, ///< Cicero = 12dd = 4.531mm
		CM, ///< Centimeter = 10mm = 2.371pc
		IN, ///< Inch = 25.4mm = 72.27pt = 6.022pc
		EX, ///< Height of a small "x" for the current font.
		EM, ///< Width of capital "M" in current font.
		MU, ///< Math unit (18mu = 1em) for positioning in math mode
		PTW, //< Percent of TextWidth
		PCW, //< Percent of ColumnWidth
		PPW, //< Percent of PageWidth
		PLW, //< Percent of LineWidth
		PTH, //< Percent of TextHeight		// Herbert 2002-05-16
		PPH, //< Percent of PaperHeight		// Herbert 2002-05-16
		UNIT_NONE ///< no unit
	};

	///
	LyXLength();
	///
	LyXLength(double v, LyXLength::UNIT u);

	/// "data" must be a decimal number, followed by a unit
	explicit LyXLength(std::string const & data);

	void swap(LyXLength & rhs)
	{
		std::swap(val_, rhs.val_);
		std::swap(unit_, rhs.unit_);
	}

	///
	double value() const;
	///
	LyXLength::UNIT unit() const;
	///
	void value(double);
	///
	void unit(LyXLength::UNIT unit);
	///
	bool zero() const;
	///
	bool empty() const;
	/// return string representation
	std::string const asString() const;
	/// return string representation
	docstring const asDocstring() const;
	/// return string representation for LaTeX
	std::string const asLatexString() const;
	/// return the on-screen size of this length
	int inPixels(int text_width, int em_width = 0) const;
	/// return the on-screen size of this length of an image
	int inBP() const;

	friend bool isValidLength(std::string const & data, LyXLength * result);

private:
	///
	double          val_;
	///
	LyXLength::UNIT unit_;
};

///
bool operator==(LyXLength const & l1, LyXLength const & l2);
///
bool operator!=(LyXLength const & l1, LyXLength const & l2);
/** Test whether \p data represents a valid length.
 * 
 * \returns whether \p data is a valid length
 * \param data Length in LyX format. Since the only difference between LyX
 * and LaTeX format is the representation of length variables as units (e.g.
 * \c text% vs. \c \\textwidth) you can actually use this function as well
 * for testing LaTeX lengths as long as they only contain real units like pt.
 * \param result Pointer to a LyXLength variable. If \p result is not 0 and
 * \p data is valid, the length represented by it is stored into \p result.
 */
bool isValidLength(std::string const & data, LyXLength * result = 0);
/// return the LyX name of the given unit number
char const * stringFromUnit(int unit);


} // namespace lyx

#endif // LYXLENGTH_H
