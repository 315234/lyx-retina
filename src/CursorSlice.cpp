/**
 * \file CursorSlice.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 * \author Andr� P�nitz
 * \author J�rgen Vigna
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "CursorSlice.h"

#include "Text.h"
#include "Paragraph.h"

#include "support/debug.h"

#include "insets/Inset.h"

#include "mathed/InsetMath.h"
#include "mathed/MathData.h"

#include "support/assert.h"

#include <ostream>

using namespace std;

namespace lyx {


CursorSlice::CursorSlice()
	: inset_(0), idx_(0), pit_(0), pos_(0)
{}


CursorSlice::CursorSlice(Inset & p)
	: inset_(&p), idx_(0), pit_(0), pos_(0)
{
	LASSERT(inset_, /**/);
}


MathData & CursorSlice::cell() const
{
	return inset_->asInsetMath()->cell(idx_);
}


Paragraph & CursorSlice::paragraph() const
{
	return text()->getPar(pit_);
}


pos_type CursorSlice::lastpos() const
{
	LASSERT(inset_, /**/);
	return inset_->asInsetMath() ? cell().size() 
		: (text()->empty() ? 0 : paragraph().size());
}


pit_type CursorSlice::lastpit() const
{
	if (inset_->inMathed())
		return 0;
	return text()->paragraphs().size() - 1;
}


CursorSlice::row_type CursorSlice::row() const
{
	LASSERT(asInsetMath(), /**/);
	return asInsetMath()->row(idx_);
}


CursorSlice::col_type CursorSlice::col() const
{
	LASSERT(asInsetMath(), /**/);
	return asInsetMath()->col(idx_);
}


void CursorSlice::forwardPos()
{
	//  move on one position if possible
	if (pos_ < lastpos()) {
		//lyxerr << "... next pos" << endl;
		++pos_;
		return;
	}

	// otherwise move on one paragraph if possible
	if (pit_ < lastpit()) {
		//lyxerr << "... next par" << endl;
		++pit_;
		pos_ = 0;
		return;
	}

	// otherwise move on one cell
	//lyxerr << "... next idx" << endl;

	LASSERT(idx_ < nargs(), /**/);

	++idx_;
	pit_ = 0;
	pos_ = 0;
}


void CursorSlice::forwardIdx()
{
	LASSERT(idx_ < nargs(), /**/);

	++idx_;
	pit_ = 0;
	pos_ = 0;
}


void CursorSlice::backwardPos()
{
	if (pos_ != 0) {
		--pos_;
		return;
	}

	if (pit_ != 0) {
		--pit_;
		pos_ = lastpos();
		return;
	}

	if (idx_ != 0) {
		--idx_;
		pit_ = lastpit();
		pos_ = lastpos();
		return;
	}

	LASSERT(false, /**/);
}


bool CursorSlice::at_end() const 
{
	return idx_ == lastidx() && pit_ == lastpit() && pos_ == lastpos();
}


bool CursorSlice::at_begin() const
{
	return idx_ == 0 && pit_ == 0 && pos_ == 0;
}


bool operator==(CursorSlice const & p, CursorSlice const & q)
{
	return p.inset_ == q.inset_
	       && p.idx_ == q.idx_
	       && p.pit_ == q.pit_
	       && p.pos_ == q.pos_;
}


bool operator!=(CursorSlice const & p, CursorSlice const & q)
{
	return p.inset_ != q.inset_
	       || p.idx_ != q.idx_
	       || p.pit_ != q.pit_
	       || p.pos_ != q.pos_;
}


bool operator<(CursorSlice const & p, CursorSlice const & q)
{
	if (p.inset_ != q.inset_) {
		LYXERR0("can't compare cursor and anchor in different insets\n"
		       << "p: " << p << '\n' << "q: " << q);
		LASSERT(false, /**/);
	}
	if (p.idx_ != q.idx_)
		return p.idx_ < q.idx_;
	if (p.pit_ != q.pit_)
		return p.pit_ < q.pit_;
	return p.pos_ < q.pos_;
}


bool operator>(CursorSlice const & p, CursorSlice const & q)
{
	return q < p;
}


bool operator<=(CursorSlice const & p, CursorSlice const & q)
{
	return !(q < p);
}


ostream & operator<<(ostream & os, CursorSlice const & item)
{
	return os
	   << "inset: " << (void *)item.inset_
//	   << " text: " << item.text()
	   << " idx: " << item.idx_
	   << " par: " << item.pit_
	   << " pos: " << item.pos_
//	   << " x: " << item.inset_->x()
//	   << " y: " << item.inset_->y()
;
}


} // namespace lyx
