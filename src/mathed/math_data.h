// -*- C++ -*-
/**
 * \file math_data.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_DATA_H
#define MATH_DATA_H

#include <iosfwd>
#include <vector>

#include "math_atom.h"
#include "dimension.h"

class LaTeXFeatures;
class ReplaceData;
class MetricsInfo;
class PainterInfo;
class TextMetricsInfo;
class TextPainter;


class MathArray : private std::vector<MathAtom> {
public:
	/// re-use inhertited stuff
	typedef std::vector<MathAtom> base_type;
	using base_type::const_iterator;
	using base_type::iterator;
	using base_type::size_type;
	using base_type::difference_type;
	using base_type::size;
	using base_type::empty;
	using base_type::clear;
	using base_type::begin;
	using base_type::end;
	using base_type::push_back;
	using base_type::pop_back;
	using base_type::back;
	using base_type::front;
	using base_type::swap;
	///
	typedef size_type idx_type;
	typedef size_type pos_type;

public:
	///
	MathArray();
	///
	MathArray(const_iterator from, const_iterator to);
	///
	void append(MathArray const & ar);

	/// inserts single atom at position pos
	void insert(size_type pos, MathAtom const & at);
	/// inserts multiple atoms at position pos
	void insert(size_type pos, MathArray const & ar);

	/// erase range from pos1 to pos2
	void erase(iterator pos1, iterator pos2);
	/// erase single atom
	void erase(iterator pos);
	/// erase range from pos1 to pos2
	void erase(size_type pos1, size_type pos2);
	/// erase single atom
	void erase(size_type pos);

	///
	void dump() const;
	///
	void dump2() const;
	///
	void replace(ReplaceData &);
	///
	void substitute(MathArray const & m);

	/// looks for exact match
	bool match(MathArray const & ar) const;
	/// looks for inclusion match starting at pos
	bool matchpart(MathArray const & ar, pos_type pos) const;
	/// looks for containment, return == size mean not found
	size_type find(MathArray const & ar) const;
	/// looks for containment, return == size mean not found
	size_type find_last(MathArray const & ar) const;
	///
	bool contains(MathArray const & ar) const;
	///
	void validate(LaTeXFeatures &) const;

	/// checked write access
	MathAtom & operator[](pos_type);
	/// checked read access
	MathAtom const & operator[](pos_type) const;
	/// rebuild cached metrics information
	void metrics(MetricsInfo & mi) const;
	/// rebuild cached metrics information
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	/// redraw cell using cache metrics information
	void draw(PainterInfo & pi, int x, int y) const;
	/// rebuild cached metrics information
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	/// redraw cell using cache metrics information
	void drawT(TextPainter & pi, int x, int y) const;
	/// mark cell for re-drawing
	void touch() const;

	/// access to cached x coordinate of last drawing
	int xo() const { return xo_; }
	/// access to cached y coordinate of last drawing
	int yo() const { return yo_; }
	/// access to cached x coordinate of mid point of last drawing
	int xm() const { return xo_ + dim_.wid / 2; }
	/// access to cached y coordinate of mid point of last drawing
	int ym() const { return yo_ + (dim_.des - dim_.asc) / 2; }
	///
	bool contains(int x, int y) const;
	/// write access to coordinate;
	void setXY(int x, int y) const;
	/// returns x coordinate of given position in the array
	int pos2x(size_type pos) const;
	/// returns position of given x coordinate
	int pos2x(size_type pos, int glue) const;
	/// returns position of given x coordinate
	size_type x2pos(int pos) const;
	/// returns position of given x coordinate fstarting from a certain pos
	size_type x2pos(int targetx, int glue) const;
	/// returns distance of this cell to the point given by x and y
	// assumes valid position and size cache
	int dist(int x, int y) const;

	/// ascent of this cell above the baseline
	int ascent() const { return dim_.asc; }
	/// descent of this cell below the baseline
	int descent() const { return dim_.des; }
	/// height of the cell
	int height() const { return dim_.asc + dim_.des; }
	/// width of this cell
	int width() const { return dim_.wid; }
	/// dimensions of cell
	Dimension const & dim() const { return dim_; }
	/// dimensions of cell
	void setDim(Dimension const & d) const { dim_ = d; }
	/// bounding box of this cell
	void boundingBox(int & xlow, int & xhigh, int & ylow, int & yhigh);
	/// gives center coordinates
	void center(int & x, int & y) const;
	/// adjust (x,y) to point on boundary on a straight line from the center
	void towards(int & x, int & y) const;

private:
	/// is this an exact match at this position?
	bool find1(MathArray const & ar, size_type pos) const;

	/// cached dimensions of cell
	mutable Dimension dim_;
	/// cached x coordinate of last drawing
	mutable int xo_;
	/// cached y coordinate of last drawing
	mutable int yo_;
	/// cached cleaness of cell
	mutable bool clean_;
	/// cached draw status of cell
	mutable bool drawn_;
};

///
std::ostream & operator<<(std::ostream & os, MathArray const & ar);


#endif
