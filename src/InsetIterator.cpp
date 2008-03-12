/**
 * \file InsetIterator.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetIterator.h"

#include <boost/assert.hpp>


namespace lyx {


InsetIterator::InsetIterator(Inset & inset)
	: DocIterator(inset)
{
}


InsetIterator inset_iterator_begin(Inset & inset)
{
	InsetIterator it = InsetIterator(inset);
	it.forwardInset();
	return it;
}


InsetIterator inset_iterator_end(Inset & inset)
{
	return InsetIterator(inset);
}


} // namespace lyx
