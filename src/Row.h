// -*- C++ -*-
/**
 * \file Row.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Matthias Ettrich
 * \author Lars Gullik Bjønnes
 *
 * Full author contact details are available in file CREDITS.
 *
 * Metrics for an on-screen text row.
 */

#ifndef ROW_H
#define ROW_H

#include "Changes.h"
#include "Dimension.h"
#include "Font.h"

#include "support/docstring.h"
#include "support/types.h"

#include <vector>

namespace lyx {

class DocIterator;
class Inset;

/**
 * An on-screen row of text. A paragraph is broken into a
 * RowList for display. Each Row contains position pointers
 * into the first and last character positions of that row.
 */
class Row {
public:
/**
 * One element of a Row. It has a set of attributes that can be used
 * by other methods that need to parse the Row contents.
 */
	struct Element {
		enum Type {
			STRING,
			COMPLETION,
			SEPARATOR,
			INSET,
			SPACE
		};

		Element(Type const t, pos_type p, Font const & f, Change const & ch)
			: type(t), pos(p), endpos(p + 1), inset(0),
			  extra(0), font(f), change(ch), final(false) {}

		//
		bool isSeparator() const { return type == SEPARATOR; }
		// returns total width of element, including separator overhead
		double width() const { return dim.wid + extra; };
		// returns position in pixels (from the left) of position
		// \param i in the row element
		double pos2x(pos_type const i) const;

		// The kind of row element
		Type type;
		// position of the element in the paragraph
		pos_type pos;
		// first position after the element in the paragraph
		pos_type endpos;
		// The dimension of the chunk (does not contains the
		// separator correction)
		Dimension dim;

		// Non-zero only if element is an inset
		Inset const * inset;

		// Only non-null for separator elements
		double extra;

		// Non-empty if element is a string or separator
		docstring str;
		//
		Font font;
		//
		Change change;
		// is it possible to add contents to this element?
		bool final;

		friend std::ostream & operator<<(std::ostream & os, Element const & row);
	};


	///
	Row();
	///
	bool changed() const { return changed_; }
	///
	void setChanged(bool c) { changed_ = c; }
	///
	void setCrc(size_type crc) const;
	/// Set the selection begin and end.
	/**
	  * This is const because we update the selection status only at draw()
	  * time.
	  */
	void setSelection(pos_type sel_beg, pos_type sel_end) const;
	///
	bool selection() const;
	/// Set the selection begin and end and whether the left and/or right
	/// margins are selected.
	void setSelectionAndMargins(DocIterator const & beg,
		DocIterator const & end) const;

	///
	void pos(pos_type p);
	///
	pos_type pos() const { return pos_; }
	///
	void endpos(pos_type p);
	///
	pos_type endpos() const { return end_; }
	///
	Dimension const & dimension() const { return dim_; }
	///
	Dimension & dimension() { return dim_; }
	///
	int height() const { return dim_.height(); }
	///
	int width() const { return dim_.wid; }
	///
	int ascent() const { return dim_.asc; }
	///
	int descent() const { return dim_.des; }

	///
	void add(pos_type pos, Inset const * ins, Dimension const & dim,
		 Font const & f, Change const & ch);
	///
	void add(pos_type pos, char_type const c,
		 Font const & f, Change const & ch);
	///
	void addCompletion(pos_type pos, docstring const & s,
			   Font const & f, Change const & ch);
	///
	void addSeparator(pos_type pos, char_type const c,
			  Font const & f, Change const & ch);
	///
	void addSpace(pos_type pos, int width, Font const & f, Change const & ch);

	///
	typedef std::vector<Element> Elements;
	///
	typedef Elements::iterator iterator;
	///
	typedef Elements::const_iterator const_iterator;
	///
	iterator begin() { return elements_.begin(); }
	///
	iterator end() { return elements_.end(); }
	///
	const_iterator begin() const { return elements_.begin(); }
	///
	const_iterator end() const { return elements_.end(); }

	///
	bool empty() const { return elements_.empty(); }
	///
	Element & back() { return elements_.back(); }
	///
	Element const & back() const { return elements_.back(); }
	/// remove last element
	void pop_back();
	/// remove all row elements
	void clear() { elements_.clear(); }
	/**
	 * remove all elements after last separator and update endpos
	 * if necessary.
	 * \param keep is the minimum amount of text to keep.
	 */
	void separate_back(pos_type keep);

	/**
	 * If last element of the row is a string, compute its width
	 * and mark it final.
	 */
	void finalizeLast();

	/**
	 * Find sequences of RtL elements and reverse them.
	 * This should be called once the row is completely built.
	 */
	void reverseRtL();

	friend std::ostream & operator<<(std::ostream & os, Row const & row);

	/// width of a separator (i.e. space)
	double separator;
	/// width of hfills in the label
	double label_hfill;
	/// the x position of the row
	double x;
	///
	mutable pos_type sel_beg;
	///
	mutable pos_type sel_end;
	///
	mutable bool begin_margin_sel;
	///
	mutable bool end_margin_sel;

private:
	/// Decides whether the margin is selected.
	/**
	  * \param margin_begin
	  * \param beg
	  * \param end
	  */
	bool isMarginSelected(bool left_margin, DocIterator const & beg,
		DocIterator const & end) const;

	/**
	 * Returns true if a char or string with font \c f and change
	 * type \c ch can be added to the current last element of the
	 * row.
	 */
	bool sameString(Font const & f, Change const & ch) const;

	///
	Elements elements_;

	/// has the Row appearance changed since last drawing?
	mutable bool changed_;
	/// CRC of row contents.
	mutable size_type crc_;
	/// first pos covered by this row
	pos_type pos_;
	/// one behind last pos covered by this row
	pos_type end_;
	/// Row dimension.
	Dimension dim_;
};


} // namespace lyx

#endif
