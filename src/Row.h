// -*- C++ -*-
/**
 * \file Row.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Matthias Ettrich
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 *
 * Metrics for an on-screen text row.
 */

#ifndef ROW_H
#define ROW_H

#include "support/types.h"

#include "Dimension.h"


namespace lyx {

/**
 * An on-screen row of text. A paragraph is broken into a
 * RowList for display. Each Row contains position pointers
 * into the first and last character positions of that row.
 */
class Row {
public:
	///
	Row();
	///
	Row(pos_type pos);
	///
	bool changed() const { return changed_; }
	///
	void setChanged(bool c) { changed_ = c; }
	///
	void setCrc(size_type crc);
	///
	void pos(pos_type p);
	///
	pos_type pos() const { return pos_; }
	///
	void endpos(pos_type p);
	///
	pos_type endpos() const { return end_; }
	///
	void setDimension(Dimension const & dim);
	///
	Dimension const & dimension() const { return dim_; }
	///
	int height() const { return dim_.height(); }
	///
	int width() const { return dim_.wid; }
	///
	int ascent() const { return dim_.asc; }
	///
	int descent() const { return dim_.des; }

	/// current debugging only
	void dump(const char * = "") const;

	/// width of a separator (i.e. space)
	double separator;
	/// width of hfills in the body
	double hfill;
	/// width of hfills in the label
	double label_hfill;
	/// the x position of the row
	double x;

private:
	/// has the Row appearance changed since last drawing?
	bool changed_;
	/// CRC of row contents.
	size_type crc_;
	/// first pos covered by this row
	pos_type pos_;
	/// one behind last pos covered by this row
	pos_type end_;
	/// Row dimension.
	Dimension dim_;
};


} // namespace lyx

#endif
