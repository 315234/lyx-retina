/**
 * \file FontList.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger Alstrup
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 * \author John Levon
 * \author Andr� P�nitz
 * \author Dekel Tsur
 * \author J�rgen Vigna
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "FontList.h"

#include <boost/next_prior.hpp>

#include <algorithm>

using namespace std;

namespace lyx {

namespace {

class matchFT
{
public:
	/// used by lower_bound and upper_bound
	int operator()(FontTable const & a, FontTable const & b) const {
		return a.pos() < b.pos();
	}
};

} // anon namespace

FontList::iterator FontList::fontIterator(pos_type pos)
{
	static Font dummy;
	FontTable search_elem(pos, dummy);
	return lower_bound(list_.begin(), list_.end(), search_elem,
		matchFT());
}


FontList::const_iterator FontList::fontIterator(pos_type pos) const
{
	static Font dummy;
	FontTable search_elem(pos, dummy);
	return lower_bound(list_.begin(), list_.end(), search_elem,
		matchFT());
}


Font & FontList::get(pos_type pos)
{
	iterator end = list_.end();
	iterator it = fontIterator(pos);
	if (it != end && it->pos() == pos)
		return it->font_;
	static Font dummy;
	return dummy;
}


void FontList::erase(pos_type pos)
{
	// Erase entries in the tables.
	iterator it = fontIterator(pos);
	iterator beg = list_.begin();
	if (it != list_.end() && it->pos() == pos
		&& (pos == 0 
			|| (it != list_.begin() && boost::prior(it)->pos() == pos - 1))) {

		// If it is a multi-character font
		// entry, we just make it smaller
		// (see update below), otherwise we
		// should delete it.
		unsigned int const i = it - list_.begin();
		list_.erase(it);
		if (i >= list_.size())
			return;
		it = list_.begin() + i;
		if (i > 0 && i < list_.size() &&
		    list_[i - 1].font() == list_[i].font()) {
			list_.erase(beg + i - 1);
			it = list_.begin() + i - 1;
		}
	}

	// Update all other entries
	iterator end = list_.end();
	for (; it != end; ++it)
		it->pos(it->pos() - 1);
}

void FontList::increasePosAfterPos(pos_type pos)
{
	List::iterator end = list_.end();
	List::iterator it = fontIterator(pos);
	for (; it != end; ++it)
		++it->pos_;
}


void FontList::decreasePosAfterPos(pos_type pos)
{
	List::iterator end = list_.end();
	List::iterator it = fontIterator(pos);
	for (; it != end; ++it)
		--it->pos_;
}


void FontList::setRange(pos_type startpos, pos_type endpos, Font const & font)
{
	// FIXME: Optimize!!!
	for (pos_type pos = startpos; pos != endpos; ++pos)
		set(pos, font);
}


void FontList::set(pos_type pos, Font const & font)
{
	// No need to simplify this because it will disappear
	// in a new kernel. (Asger)
	// Next search font table

	iterator beg = list_.begin();
	iterator it = beg;
	iterator endit = list_.end();
	bool found = false;
	for (; it != endit; ++it) {
		if (it->pos() >= pos) {
			found = true;
			break;
		}
	}
	if (found && it->font() == font)
		return;

	size_t const i = distance(beg, it);

	// Is position pos is a beginning of a font block?
	bool begin = pos == 0 || !found 
		|| (i > 0 && list_[i - 1].pos() == pos - 1);

	// Is position pos is the end of a font block?
	bool end = found && list_[i].pos() == pos;

	if (!begin && !end) {
		// The general case: The block is splitted into 3 blocks
		list_.insert(list_.begin() + i,
				FontTable(pos - 1, list_[i].font()));
		list_.insert(list_.begin() + i + 1,
				FontTable(pos, font));
		return;
	}

	if (begin && end) {
		// A single char block
		if (i + 1 < list_.size() &&
		    list_[i + 1].font() == font) {
			// Merge the singleton block with the next block
			list_.erase(list_.begin() + i);
			if (i > 0 && list_[i - 1].font() == font)
				list_.erase(list_.begin() + i - 1);
		} else if (i > 0 && list_[i - 1].font() == font) {
			// Merge the singleton block with the previous block
			list_[i - 1].pos(pos);
			list_.erase(list_.begin() + i);
		} else
			list_[i].font(font);
	} else if (begin) {
		if (i > 0 && list_[i - 1].font() == font)
			list_[i - 1].pos(pos);
		else
			list_.insert(list_.begin() + i,
					FontTable(pos, font));
	} else if (end) {
		list_[i].pos(pos - 1);
		if (!(i + 1 < list_.size() &&
		      list_[i + 1].font() == font))
			list_.insert(list_.begin() + i + 1,
					FontTable(pos, font));
	}
}


FontSize FontList::highestInRange
	(pos_type startpos, pos_type endpos, FontSize def_size) const
{
	if (list_.empty())
		return def_size;

	const_iterator end_it = list_.begin();
	const_iterator const end = list_.end();
	for (; end_it != end; ++end_it) {
		if (end_it->pos() >= endpos)
			break;
	}

	if (end_it != end)
		++end_it;

	FontList::const_iterator cit = list_.begin();
	for (; cit != end; ++cit) {
		if (cit->pos() >= startpos)
			break;
	}

	FontSize maxsize = FONT_SIZE_TINY;
	for (; cit != end_it; ++cit) {
		FontSize size = cit->font().fontInfo().size();
		if (size == FONT_SIZE_INHERIT)
			size = def_size;
		if (size > maxsize && size <= FONT_SIZE_HUGER)
			maxsize = size;
	}
	return maxsize;
}


bool FontList::hasChangeInRange(pos_type pos, int len) const
{
	// FIXME: can't we use fontIterator(pos) instead?
	const_iterator cit = list_.begin();
	const_iterator end = list_.end();
	for (; cit != end; ++cit) {
		if (cit->pos() >= pos)
			break;
	}
	if (cit != end && pos + len - 1 > cit->pos())
		return false;

	return true;
}


void FontList::validate(LaTeXFeatures & features) const
{
	const_iterator fcit = list_.begin();
	const_iterator fend = list_.end();
	for (; fcit != fend; ++fcit)
		fcit->font().validate(features);
}

} // namespace lyx
