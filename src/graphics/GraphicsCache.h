// -*- C++ -*-
/**
 * \file GraphicsCache.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Baruch Even
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 *
 * lyx::graphics::Cache is the manager of the image cache.
 * It is responsible for creating the lyx::graphics::CacheItem's
 * and maintaining them.
 *
 * lyx::graphics::Cache is a singleton class. It is possible to have only one
 * instance of it at any moment.
 */

#ifndef GRAPHICSCACHE_H
#define GRAPHICSCACHE_H

#include "support/std_string.h"
#include <vector>
#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>


namespace lyx {
namespace graphics {

class CacheItem;

class Cache : boost::noncopyable {
public:

	/// This is a singleton class. Get the instance.
	static Cache & get();

	/** Which graphics formats can be loaded directly by the image loader.
	 *  Other formats can be loaded if a converter to a loadable format
	 *  can be defined.
	 */
	std::vector<string> loadableFormats() const;

	/// Add a graphics file to the cache.
	void add(string const & file) const;

	/// Remove a file from the cache.
	void remove(string const & file) const;

	/// Returns \c true if the file is in the cache.
	bool inCache(string const & file) const;

	/** Get the cache item associated with file.
	 *  Returns an empty container if there is no such item.
	 *
	 *  IMPORTANT: whatever uses an image must make a local copy of this
	 *  ItemPtr. The boost::shared_ptr<>::use_count() function is
	 *  used to ascertain whether or not to remove the item from the cache
	 *  when remove(file) is called.
	 *
	 *  You have been warned!
	 */
	typedef boost::shared_ptr<CacheItem> ItemPtr;
	///
	ItemPtr const item(string const & file) const;

private:
	/** Make the c-tor, d-tor private so we can control how many objects
	 *  are instantiated.
	 */
	Cache();
	///
	~Cache();

	/// Use the Pimpl idiom to hide the internals.
	class Impl;
	/// The pointer never changes although *pimpl_'s contents may.
	boost::scoped_ptr<Impl> const pimpl_;
};

} // namespace graphics
} // namespace lyx

#endif // GRAPHICSCACHE_H
