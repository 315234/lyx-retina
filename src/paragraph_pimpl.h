// -*- C++ -*-
/**
 * \file paragraph_pimpl.h
 * Copyright 1995-2002 the LyX Team
 * Read the file COPYING
 */

#ifndef PARAGRAPH_PIMPL_H
#define PARAGRAPH_PIMPL_H

#include "paragraph.h"
#include "ParagraphParameters.h"
#include "changes.h"
#include "counters.h"

#include <boost/scoped_ptr.hpp>

class LyXLayout;

struct Paragraph::Pimpl {
	///
	typedef std::vector<value_type> TextContainer;

	///
	Pimpl(Paragraph * owner);
	/// Copy constructor
	Pimpl(Pimpl const &, Paragraph * owner, bool same_ids = false);
	///
	lyx::pos_type size() const {
		return text.size();
	}
	///
	bool empty() const {
		return text.empty();
	}
	///
	void clear();
	///
	void setContentsFromPar(Paragraph const & par);
	/// set tracking mode
	void trackChanges(Change::Type type = Change::UNCHANGED);
	/// stop tracking
	void untrackChanges();
	/// set all text as new for change mode
	void cleanChanges();
	/// look up change type at given pos
	Change::Type lookupChange(lyx::pos_type pos) const;
	/// look up change at given pos
	Change const lookupChangeFull(lyx::pos_type pos) const;
	/// is there a change in the given range ?
	bool isChanged(lyx::pos_type start, lyx::pos_type end) const;
	/// is there a non-addition in this range ?
	bool isChangeEdited(lyx::pos_type start, lyx::pos_type end) const;

	/// set change at pos
	void setChange(lyx::pos_type pos, Change::Type type);

	/// mark as erased
	void markErased();

	/// accept change
	void acceptChange(lyx::pos_type start, lyx::pos_type end);

	/// reject change
	void rejectChange(lyx::pos_type start, lyx::pos_type end);

	/// are we tracking changes ?
	bool tracking() const {
		return changes_.get();
	}

	///
	value_type getChar(lyx::pos_type pos) const;
	///
	void setChar(lyx::pos_type pos, value_type c);
	///
	void insertChar(lyx::pos_type pos, value_type c, LyXFont const & font, Change change = Change(Change::INSERTED));
	///
	void insertInset(lyx::pos_type pos, Inset * inset, LyXFont const & font, Change change = Change(Change::INSERTED));
	/// definite erase
	void eraseIntern(lyx::pos_type pos);
	/// erase the given position
	void erase(lyx::pos_type pos);
	/// erase the given range
	int erase(lyx::pos_type start, lyx::pos_type end);
	///
	Inset * inset_owner;

	/** A font entry covers a range of positions. Notice that the
	    entries in the list are inserted in random order.
	    I don't think it's worth the effort to implement a more effective
	    datastructure, because the number of different fonts in a paragraph
	    is limited. (Asger)
	    Nevertheless, I decided to store fontlist using a sorted vector:
	    fontlist = { {pos_1,font_1} , {pos_2,font_2} , ... } where
	    pos_1 < pos_2 < ..., font_{i-1} != font_i for all i,
	    and font_i covers the chars in positions pos_{i-1}+1,...,pos_i
	    (font_1 covers the chars 0,...,pos_1) (Dekel)
	*/
	struct FontTable  {
		///
		FontTable(lyx::pos_type p, LyXFont const & f)
			: pos_(p)
			{
				font_ = container.get(f);
			}
		///
		lyx::pos_type pos() const { return pos_; }
		///
		void pos(lyx::pos_type p) { pos_ = p; }
		///
		LyXFont const & font() const { return *font_; }
		///
		void font(LyXFont const & f) { font_ = container.get(f);}
	private:
		/// End position of paragraph this font attribute covers
		lyx::pos_type pos_;
		/** Font. Interpretation of the font values:
		    If a value is LyXFont::INHERIT_*, it means that the font
		    attribute is inherited from either the layout of this
		    paragraph or, in the case of nested paragraphs, from the
		    layout in the environment one level up until completely
		    resolved.
		    The values LyXFont::IGNORE_* and LyXFont::TOGGLE are NOT
		    allowed in these font tables.
		*/
		boost::shared_ptr<LyXFont> font_;
		///
		static ShareContainer<LyXFont> container;
	};
	///
	friend struct matchFT;
	///
	struct matchFT {
		/// used by lower_bound and upper_bound
		inline
		int operator()(FontTable const & a, FontTable const & b) const {
			return a.pos() < b.pos();
		}
	};

	///
	typedef std::vector<FontTable> FontList;
	///
	FontList fontlist;

	///
	void simpleTeXBlanks(std::ostream &, TexRow & texrow,
			     lyx::pos_type const i,
			     unsigned int & column,
			     LyXFont const & font,
			     LyXLayout const & style);
	///
	void simpleTeXSpecialChars(Buffer const *, BufferParams const &,
				   std::ostream &, TexRow & texrow,
				   bool moving_arg,
				   LyXFont & font, LyXFont & running_font,
				   LyXFont & basefont,
				   LyXFont const & outerfont,
				   bool & open_font,
				   Change::Type & running_change,
				   LyXLayout const & style,
				   lyx::pos_type & i,
				   unsigned int & column, value_type const c);

	///
	void validate(LaTeXFeatures & features,
		      LyXLayout const & layout) const;

	///
	unsigned int id_;
	///
	static unsigned int paragraph_id;
	///
	ParagraphParameters params;

private:
	/// erase at the given position. Returns true if it was actually erased
	bool erasePos(lyx::pos_type pos);

	/// match a string against a particular point in the paragraph
	bool isTextAt(string const & str, lyx::pos_type pos) const;

	/// for recording and looking up changes in revision tracking mode
	boost::scoped_ptr<Changes> changes_;

	/// Who owns us?
	Paragraph * owner_;
	///
	TextContainer text;
};

#endif
