// -*- C++ -*-
/**
 * \file lyxalgo.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 *
 * A variety of useful templates.
 */

#ifndef LYX_ALGO_H
#define LYX_ALGO_H

#include <utility>
#include <iterator>
#include <algorithm>
#include <set>


namespace lyx {


/// Returns true if the sequence first,last is sorted, false if not.
template <class For>
bool sorted(For first, For last)
{
	if (first == last) return true;
	For tmp = first;
	while (++tmp != last) {
		if (*tmp < *first++) return false;
	}
	return true;
}


/// Cmp is the same Cmp as you would pass to std::sort.
template <class For, class Cmp>
bool sorted(For first, For last, Cmp cmp)
{
	if (first == last) return true;
	For tmp = first;
	while (++tmp != last) {
		if (cmp(*tmp, *first++)) return false;
	}
	return true;
}


struct firster {
	template <class P1, class P2>
	P1 operator()(std::pair<P1, P2> const & p) {
		return p.first;
	}
};


/**
 * copy elements in the given range to the output iterator
 * if the predicate evaluates as true
 */
template <class InputIter, class OutputIter, class Func>
OutputIter copy_if(InputIter first, InputIter last,
	       OutputIter result, Func func)
{
	for (; first != last; ++first) {
		if (func(*first)) {
			*result++ = *first;
		}
	}
	return result;
}


/// A slot in replacement for std::count for systems where it is broken.
template <class Iterator, class T>
typename std::iterator_traits<Iterator>::difference_type
count (Iterator first, Iterator last, T const & value)
{
#ifdef HAVE_STD_COUNT
	return std::count(first, last, value);
#else
	std::iterator_traits<Iterator>::difference_type n = 0;
	while (first != last)
		if (*first++ == value) ++n;
	return n;
#endif
}

/// Remove all duplicate entries in c.
template<class C>
void eliminate_duplicates(C & c)
{
	C unique_c;
	std::set<typename C::value_type> s;

	for (typename C::iterator p = c.begin(); p != c.end(); ++p) {
		if (s.find(*p) == s.end()) {
			unique_c.push_back(*p);
			s.insert(*p);
		}
	}
	swap(c, unique_c);
}

} // namespace lyx

#endif // LYX_ALGO_H
