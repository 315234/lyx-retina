/**
 * \file src/FontIterator.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alfredo Braunstein
 *
 * Full author contact details are available in file CREDITS.
 *
 */


#include <config.h>

#include "FontIterator.h"

#include "Buffer.h"
#include "LyXText.h"
#include "Paragraph.h"


namespace lyx {


FontIterator::FontIterator(Buffer const & buffer, LyXText const & text,
		Paragraph const & par, pos_type pos)
	: buffer_(buffer), text_(text), par_(par), pos_(pos),
	  font_(text.getFont(buffer, par, pos)),
	  endspan_(par.fontSpan(pos).last),
	  bodypos_(par.beginOfBody())
{}


LyXFont const & FontIterator::operator*() const
{
	return font_;
}


LyXFont * FontIterator::operator->()
{
	return &font_;
}


FontIterator & FontIterator::operator++()
{
	++pos_;
	if (pos_ > endspan_ || pos_ == bodypos_) {
		font_ = text_.getFont(buffer_, par_, pos_);
		endspan_ = par_.fontSpan(pos_).last;
	}
	return *this;
}


} // namespace lyx
