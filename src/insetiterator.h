// -*- C++ -*-
/**
 * \file InsetIterator.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETITERATOR_H
#define INSETITERATOR_H

#include "DocIterator.h"


namespace lyx {


class InsetIterator : public DocIterator
{
public:
	///
	explicit InsetIterator(InsetBase & inset);
	///
	void operator++() { forwardInset(); }
	///
	void operator--();
	///
	InsetBase * operator->() { return nextInset(); }
	///
	InsetBase & operator*() { return *nextInset(); }
};

InsetIterator inset_iterator_begin(InsetBase & inset);

InsetIterator inset_iterator_end(InsetBase & inset);



} // namespace lyx

#endif
