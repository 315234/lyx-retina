// -*- C++ -*-
/**
 * \file insetiterator.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETITERATOR_H
#define INSETITERATOR_H

#include "dociterator.h"


class InsetIterator : public DocumentIterator
{
public:
	///
	InsetIterator() {}
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

#endif
