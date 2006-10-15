// -*- C++ -*-
/* \file coordcache.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef COORDCACHE_H
#define COORDCACHE_H

// It seems that MacOSX define the check macro.
#undef check

class InsetBase;
class LyXText;
class MathArray;
class Paragraph;

#include "support/types.h"

#include <boost/assert.hpp>

#include <map>

namespace lyx {

void lyxbreaker(void const * data, const char * hint, int size);

class Point {
public:
	Point()
		: x_(0), y_(0)
	{}

	Point(int x, int y)
		: x_(x), y_(y)
	{
		BOOST_ASSERT(x > -1000000);
		BOOST_ASSERT(x <  1000000);
		BOOST_ASSERT(y > -1000000);
		BOOST_ASSERT(y <  1000000);
	}

	int x_, y_;
};

template <class T> class CoordCacheBase {
public:
	void clear()
	{
		data_.clear();
	}

	void add(T const * thing, int x, int y)
	{
		data_[thing] = Point(x, y);
	}

	int x(T const * thing) const
	{
		check(thing, "x");
		return data_.find(thing)->second.x_;
	}

	int y(T const * thing) const
	{
		check(thing, "y");
		return data_.find(thing)->second.y_;
	}

	Point xy(T const * thing) const
	{
		check(thing, "xy");
		return data_.find(thing)->second;
	}

	bool has(T const * thing) const
	{
		return data_.find(thing) != data_.end();
	}

//	T * find(int x, int y) const
//	{
//		T *
//		cache_type iter
//	}

private:
	friend class CoordCache;

	void check(T const * thing, char const * hint) const
	{
		if (!has(thing))
			lyxbreaker(thing, hint, data_.size());
	}

	typedef std::map<T const *, Point> cache_type;
	cache_type data_;
};

/**
 * A global cache that allows us to come from a paragraph in a document
 * to a position point on the screen.
 * All points cached in this cache are only valid between subsequent
 * updated. (x,y) == (0,0) is the upper left screen corner, x increases
 * to the right, y increases downwords.
 * The cache is built in BufferView::Pimpl::metrics which is called
 * from BufferView::Pimpl::update. The individual points are added
 * while we paint them. See for instance paintPar in RowPainter.C.
 */
class CoordCache {
public:
	void clear();
	Point get(LyXText const *, lyx::pit_type);

	/// A map from paragraph index number to screen point
	typedef std::map<lyx::pit_type, Point> InnerParPosCache;
	/// A map from a LyXText to the map of paragraphs to screen points
	typedef std::map<LyXText const *, InnerParPosCache> ParPosCache;
	/// A map from a CursorSlice to screen points
	typedef std::map<LyXText const *, InnerParPosCache> SliceCache;

	/// A map from MathArray to position on the screen
	CoordCacheBase<MathArray> & arrays() { return arrays_; }
	CoordCacheBase<MathArray> const & getArrays() const { return arrays_; }
	/// A map from insets to positions on the screen
	CoordCacheBase<InsetBase> & insets() { return insets_; }
	CoordCacheBase<InsetBase> const & getInsets() const { return insets_; }
	/// A map from (LyXText, paragraph) pair to screen positions
	ParPosCache & parPos() { return pars_; }
	ParPosCache const & getParPos() const { return pars_; }
	///
	SliceCache & slice(bool boundary)
	{
		return boundary ? slices1_ : slices0_;
	}
	SliceCache const & getSlice(bool boundary) const
	{
		return boundary ? slices1_ : slices0_;
	}

private:
	/// MathArrays
	CoordCacheBase<MathArray> arrays_;
	// All insets
	CoordCacheBase<InsetBase> insets_;
	/// Paragraph grouped by owning text
	ParPosCache pars_;
	/// Used with boundary == 0
	SliceCache slices0_;
	/// Used with boundary == 1
	SliceCache slices1_;
};

} // namespace lyx

#endif
