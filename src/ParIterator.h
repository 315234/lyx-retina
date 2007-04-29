// -*- C++ -*-
/* \file ParIterator.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef PARITERATOR_H
#define PARITERATOR_H

#include "DocIterator.h"

#include "support/types.h"

#include <vector>


namespace lyx {

class Inset;
class LyXText;
class ParagraphList;


class ParIterator : public std::iterator<std::forward_iterator_tag, Paragraph>,
		    public DocIterator
{
public:
	typedef std::iterator<std::forward_iterator_tag, Paragraph> StdIt;

	typedef StdIt::value_type value_type;
	typedef StdIt::difference_type difference_type;
	typedef StdIt::pointer pointer;
	typedef StdIt::reference reference;

	///
	ParIterator() : DocIterator() {}

	///
	ParIterator(Inset &, pit_type pit);
	///
	ParIterator(ParIterator const &);
	///
	ParIterator(DocIterator const &);

	/// This really should be implemented...
	//ParIterator & operator=(ParIterator const &);
	///
	ParIterator & operator++();
	///
	ParIterator operator++(int);
	/// See comment in ParIterator.cpp
	//ParIterator & operator--();
	///
	Paragraph & operator*() const;
	///
	Paragraph * operator->() const;
	/// This gives us the top-most parent paragraph
	pit_type outerPar() const;
	///
	pit_type pit() const;
	///
	/// return the paragraph this cursor is in
	pit_type & pit() { return DocIterator::pit(); }

	ParagraphList & plist() const;
};


DocIterator makeDocIterator(ParIterator const &, pos_type);

ParIterator par_iterator_begin(Inset & inset);

ParIterator par_iterator_end(Inset & inset);


///
bool operator==(ParIterator const & iter1, ParIterator const & iter2);

///
bool operator!=(ParIterator const & iter1, ParIterator const & iter2);


class ParConstIterator : public std::iterator<std::forward_iterator_tag,
			 Paragraph>,
			 public DocIterator
{
public:
	///
	ParConstIterator(): DocIterator() {}
	///
	ParConstIterator(ParConstIterator const &);
	///
	ParConstIterator(DocIterator const &);
	///

	ParConstIterator & operator++();
	///
	ParConstIterator & operator--();
	///
	Paragraph const & operator*() const;
	///
	Paragraph const * operator->() const;
	///
	ParagraphList const & plist() const;
};

bool operator==(ParConstIterator const & iter1,
		ParConstIterator const & iter2);

bool operator!=(ParConstIterator const & iter1,
		ParConstIterator const & iter2);


ParConstIterator par_const_iterator_begin(Inset const & inset);

ParConstIterator par_const_iterator_end(Inset const & inset);



} // namespace lyx

#endif
