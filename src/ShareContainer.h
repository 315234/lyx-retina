// -*- C++ -*-
/**
 * \file ShareContainer.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef SHARECONTAINER_H
#define SHARECONTAINER_H

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>
#include <algorithm>
#include <functional>

/// Share objects between several users.
/**
   This class can be used to reduce memory consuption when you have a lot
   of equal objects used all over your code.

   \author Lars Gullik Bj�nnes
*/
template<class Share>
class ShareContainer : boost::noncopyable {
public:
	///
	typedef std::vector<boost::shared_ptr<Share> > Params;
	///
	typedef typename Params::value_type value_type;
	/// Return a shared_ptr that points to a element equal to ps.
	value_type
	get(Share const & ps) const {
		// First see if we already have this ps in the container
		typename Params::iterator it = std::find_if(params.begin(),
							    params.end(),
							    isEqual(ps));
		value_type tmp;
		if (it == params.end()) {
			// ok we don't have it so we should
			// insert it.
			tmp.reset(new Share(ps));
			params.push_back(tmp);
			// We clean here. This can cause us to have
			// some (one) unique elemements some times
			// but we should gain a lot in speed.
			clean();
		} else {
			// yes we have it already
			tmp = *it;
			// move it forward - optimization
			// makes the next find faster.
			if (it != params.begin())
				std::swap(*it, *(it - 1));
		}
		return tmp;
	}
private:
	/// A functor returning true if the elements are equal.
	struct isEqual : public std::unary_function<value_type, bool> {
		isEqual(Share const & s) : p_(s) {}
		bool operator()(value_type const & p1) const {
			return *p1.get() == p_;
		}
	private:
		Share const & p_;
	};
	/// A functor returning true if the element is unique.
	struct isUnique : public std::unary_function<value_type, bool> {
		bool operator()(value_type const & p) const {
			return p.unique();
		}
	};

	/** Remove all unique items.
	    This removes all elements from params that is only referenced
	    from the private container. This can be considered a memory
	    optimizaton.
	*/
	void clean() const {
		typename Params::iterator it = std::remove_if(params.begin(),
							      params.end(),
							      isUnique());
		params.erase(it, params.end());
	}

	/// The actual container.
	mutable Params params;
};
#endif
