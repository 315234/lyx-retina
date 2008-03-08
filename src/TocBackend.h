// -*- C++ -*-
/**
 * \file TocBackend.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TOC_BACKEND_H
#define TOC_BACKEND_H

#include "ParIterator.h"

#include "support/strfwd.h"

#include <map>
#include <vector>
#include <string>


namespace lyx {

class Buffer;
class FuncRequest;

///
/**
*/
class TocItem
{
	friend class TocBackend;

public:
	/// Default constructor for STL containers.
	TocItem() {}
	///
	TocItem(ParConstIterator const & par_it,
		int depth,
		docstring const & s
		);
	///
	~TocItem() {}
	///
	int id() const;
	///
	int depth() const;
	///
	docstring const & str() const;
	///
	docstring const asString() const;

	/// the action corresponding to the goTo above
	FuncRequest action() const;

protected:
	/// Current position of item.
	ParConstIterator par_it_;

	/// nesting depth
	int depth_;

	/// Full item string
	docstring str_;
};


///
class Toc : public std::vector<TocItem> {};

typedef Toc::const_iterator TocIterator;

/// The ToC list.
/// A class and no typedef because we want to forward declare it.
class TocList : public std::map<std::string, Toc> {};


///
/**
*/
class TocBackend
{
public:
	///
	TocBackend(Buffer const * buffer) : buffer_(buffer) {}
	///
	void setBuffer(Buffer const * buffer) { buffer_ = buffer; }
	///
	void update();
	///
	void updateItem(ParConstIterator const & pit);

	///
	TocList const & tocs() const { return tocs_; }
	TocList & tocs() { return tocs_; }

	///
	Toc const & toc(std::string const & type) const;
	Toc & toc(std::string const & type);

	/// Return the first Toc Item before the cursor
	TocIterator item(
		std::string const & type, ///< Type of Toc.
		ParConstIterator const & ///< The cursor location in the document.
	) const;

	///
	void writePlaintextTocList(std::string const & type, odocstream & os) const;

private:
	///
	TocList tocs_;
	///
	Buffer const * buffer_;
}; // TocBackend

inline bool operator==(TocItem const & a, TocItem const & b)
{
	return a.id() == b.id() && a.str() == b.str() && a.depth() == b.depth();
}


inline bool operator!=(TocItem const & a, TocItem const & b)
{
	return !(a == b);
}


} // namespace lyx

#endif // TOC_BACKEND_H
