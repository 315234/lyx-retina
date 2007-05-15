/**
 * \file TocBackend.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Jean-Marc Lasgouttes
 * \author Angus Leeming
 * \author Abdelrazak Younes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "TocBackend.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "FloatList.h"
#include "FuncRequest.h"
#include "LyXAction.h"
#include "Paragraph.h"
#include "debug.h"

#include "insets/InsetOptArg.h"

#include "support/convert.h"


namespace lyx {

using std::vector;
using std::string;


///////////////////////////////////////////////////////////////////////////
// TocItem implementation

TocItem::TocItem(ParConstIterator const & par_it, int d,
		docstring const & s)
		: par_it_(par_it), depth_(d), str_(s)
{
/*
	if (!uid_.empty())
		return;

	size_t pos = s.find(" ");
	if (pos == string::npos) {
		// Non labelled item
		uid_ = s;
		return;
	}

	string s2 = s.substr(0, pos);

	if (s2 == "Chapter" || s2 == "Part") {
		size_t pos2 = s.find(" ", pos + 1);
		if (pos2 == string::npos) {
			// Unnumbered Chapter?? This should not happen.
			uid_ = s.substr(pos + 1);
			return;
		}
		// Chapter or Part
		uid_ = s.substr(pos2 + 1);
		return;
	}
	// Numbered Item.
	uid_ = s.substr(pos + 1);
	*/
}

bool const TocItem::isValid() const
{
	return depth_ != -1;
}


int const TocItem::id() const
{
	return par_it_->id();
}


int const TocItem::depth() const
{
	return depth_;
}


docstring const & TocItem::str() const
{
	return str_;
}


docstring const TocItem::asString() const
{
	return docstring(4 * depth_, ' ') + str_;
}


FuncRequest TocItem::action() const
{
	return FuncRequest(LFUN_PARAGRAPH_GOTO, convert<string>(id()));
}





///////////////////////////////////////////////////////////////////////////
// TocBackend implementation

Toc const & TocBackend::toc(std::string const & type) const
{
	// Is the type already supported?
	TocList::const_iterator it = tocs_.find(type);
	BOOST_ASSERT(it != tocs_.end());

	return it->second;
}


void TocBackend::updateItem(ParConstIterator const & par_it)
{
	if (toc("tableofcontents").empty()) {
		// TODO should not happen, 
		// a call to TocBackend::update() is missing somewhere
		LYXERR(Debug::INFO)
				<< "TocBackend.cpp: TocBackend::updateItem"
				<< "called but the TOC is empty" << std::endl;
		return;
	}

	BufferParams const & bufparams = buffer_->params();
	const int min_toclevel = bufparams.getTextClass().min_toclevel();

	TocIterator toc_item = item("tableofcontents", par_it);

	docstring tocstring;

	// For each paragraph, traverse its insets and let them add
	// their toc items
	InsetList::const_iterator it = toc_item->par_it_->insetlist.begin();
	InsetList::const_iterator end = toc_item->par_it_->insetlist.end();
	for (; it != end; ++it) {
		Inset & inset = *it->inset;
		if (inset.lyxCode() == Inset::OPTARG_CODE) {
			if (!tocstring.empty())
				break;
			Paragraph const & par = 
				*static_cast<InsetOptArg&>(inset).paragraphs().begin();
			if (!toc_item->par_it_->getLabelstring().empty())
				tocstring = toc_item->par_it_->getLabelstring() + ' ';
			tocstring += par.asString(*buffer_, false);
			break;
		}
	}

	int const toclevel = toc_item->par_it_->layout()->toclevel;
	if (toclevel != Layout::NOT_IN_TOC
	    && toclevel >= min_toclevel
		&& tocstring.empty())
			tocstring = toc_item->par_it_->asString(*buffer_, true);

	const_cast<TocItem &>(*toc_item).str_ = tocstring;
}


void TocBackend::update()
{
	tocs_.clear();

	BufferParams const & bufparams = buffer_->params();
	const int min_toclevel = bufparams.getTextClass().min_toclevel();

	Toc & toc = tocs_["tableofcontents"];
	ParConstIterator pit = buffer_->par_iterator_begin();
	ParConstIterator end = buffer_->par_iterator_end();
	for (; pit != end; ++pit) {

		// the string that goes to the toc (could be the optarg)
		docstring tocstring;

		// For each paragraph, traverse its insets and let them add
		// their toc items
		InsetList::const_iterator it = pit->insetlist.begin();
		InsetList::const_iterator end = pit->insetlist.end();
		for (; it != end; ++it) {
			Inset & inset = *it->inset;
			inset.addToToc(tocs_, *buffer_);
			switch (inset.lyxCode()) {
			case Inset::OPTARG_CODE: {
				if (!tocstring.empty())
					break;
				Paragraph const & par = 
					*static_cast<InsetOptArg&>(inset).paragraphs().begin();
				if (!pit->getLabelstring().empty())
					tocstring = pit->getLabelstring() + ' ';
				tocstring += par.asString(*buffer_, false);
				break;
			}
			default:
				break;
			}
		}

		/// now the toc entry for the paragraph
		int const toclevel = pit->layout()->toclevel;
		if (toclevel != Layout::NOT_IN_TOC
		    && toclevel >= min_toclevel) {
			// insert this into the table of contents
			if (tocstring.empty())
				tocstring = pit->asString(*buffer_, true);
			toc.push_back(
				TocItem(pit, toclevel - min_toclevel, tocstring));
		}
	}
}


TocIterator const TocBackend::item(
	std::string const & type, ParConstIterator const & par_it) const
{
	TocList::const_iterator toclist_it = tocs_.find(type);
	// Is the type supported?
	BOOST_ASSERT(toclist_it != tocs_.end());

	Toc const & toc_vector = toclist_it->second;
	TocIterator last = toc_vector.begin();
	TocIterator it = toc_vector.end();
	if (it == last)
		return it;

	--it;

	ParConstIterator par_it_text = par_it;
	if (par_it_text.inMathed())
		// It would be better to do
		//   par_it_text.backwardInset();
		// but this method does not exist.
		while (par_it_text.inMathed())
			par_it_text.backwardPos();

	for (; it != last; --it) {
		
		// A good solution for Items inside insets would be to do:
		//
		//if (std::distance(it->par_it_, current) <= 0)
		//	return it;
		//
		// But for an unknown reason, std::distance(current, it->par_it_) always
		// returns  a positive value and std::distance(it->par_it_, current) takes forever...
		// So for now, we do:
		if (it->par_it_.pit() <= par_it_text.pit())
			return it;
	}

	// We are before the first Toc Item:
	return last;
}


void TocBackend::writePlaintextTocList(string const & type, odocstream & os) const
{
	TocList::const_iterator cit = tocs_.find(type);
	if (cit != tocs_.end()) {
		TocIterator ccit = cit->second.begin();
		TocIterator end = cit->second.end();
		for (; ccit != end; ++ccit)
			os << ccit->asString() << '\n';
	}
}


} // namespace lyx
