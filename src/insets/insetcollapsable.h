// -*- C++ -*-
/**
 * \file insetcollapsable.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef INSETCOLLAPSABLE_H
#define INSETCOLLAPSABLE_H


#include "inset.h"
#include "insettext.h"
#include "lyxfont.h"
#include "LColor.h"

#include <boost/weak_ptr.hpp>

class Painter;
class LyXText;
class Paragraph;
class LyXCursor;

/** A collapsable text inset

*/
class InsetCollapsable : public UpdatableInset {
public:
	///
	enum UpdateCodes {
		NONE = 0,
		FULL
	};
	///
	static int const TEXT_TO_TOP_OFFSET = 2;
	///
	static int const TEXT_TO_BOTTOM_OFFSET = 2;
	/// inset is initially collapsed if bool = true
	InsetCollapsable(BufferParams const &, bool = false);
	///
	InsetCollapsable(InsetCollapsable const & in, bool same_id = false);
	///
	void read(Buffer const *, LyXLex &);
	///
	void write(Buffer const *, std::ostream &) const;
	///
	int ascent(BufferView *, LyXFont const &) const;
	///
	int descent(BufferView *, LyXFont const &) const;
	///
	int width(BufferView *, LyXFont const &) const;
	///
	void draw(BufferView *, const LyXFont &, int , float &, bool) const;
	///
	void update(BufferView *, LyXFont const &, bool =false);
	///
	void edit(BufferView *, int, int, mouse_button::state);
	///
	void edit(BufferView *, bool front = true);
	///
	EDITABLE editable() const;
	///
	bool insertInset(BufferView *, Inset * inset);
	///
	virtual bool insetAllowed(Inset::Code code) const {
		return inset.insetAllowed(code);
	}
	///
	bool isTextInset() const { return true; }
	///
	bool doClearArea() const;
	///
	void insetUnlock(BufferView *);
	///
	bool needFullRow() const { return isOpen(); }
	///
	bool lockInsetInInset(BufferView *, UpdatableInset *);
	///
	bool unlockInsetInInset(BufferView *, UpdatableInset *,
				bool lr = false);
	///
	bool updateInsetInInset(BufferView *, Inset *);
	///
	int insetInInsetY() const;
	///
	RESULT localDispatch(FuncRequest const &);
	///
	int latex(Buffer const *, std::ostream &,
		  bool fragile, bool free_spc) const;
	///
	int ascii(Buffer const *, std::ostream &, int) const;
	///
	int linuxdoc(Buffer const *, std::ostream &) const;
	///
	int docbook(Buffer const *, std::ostream &, bool mixcont) const;
	///
	void validate(LaTeXFeatures & features) const;
	///
	void getCursorPos(BufferView *, int & x, int & y) const;
	///
	void toggleInsetCursor(BufferView *);
	///
	void showInsetCursor(BufferView *, bool show = true);
	///
	void hideInsetCursor(BufferView *);
	///
	void fitInsetCursor(BufferView * bv) const {
		inset.fitInsetCursor(bv);
	}
	UpdatableInset * getLockingInset() const;
	///
	UpdatableInset * getFirstLockingInsetOfType(Inset::Code);
	///
	void setFont(BufferView *, LyXFont const &, bool toggleall = false,
		 bool selectall = false);
	///
	void setLabel(string const & l) const;
	///
	void setLabelFont(LyXFont & f) { labelfont = f; }
#if 0
	///
	void setAutoCollapse(bool f) { autocollapse = f; }
#endif
#if 0
	///
	int getMaxWidth(BufferView *, UpdatableInset const *) const;
#endif
	///
	LyXText * getLyXText(BufferView const *, bool const recursive) const;
	///
	void deleteLyXText(BufferView *, bool recursive=true) const;
	///
	void resizeLyXText(BufferView *, bool force = false) const;
	///
	std::vector<string> const getLabelList() const;
	///
	bool nodraw() const;
	///
	int scroll(bool recursive=true) const;
	///
	void scroll(BufferView *bv, float sx) const {
		UpdatableInset::scroll(bv, sx);
	}
	///
	void scroll(BufferView *bv, int offset) const {
		UpdatableInset::scroll(bv, offset);
	}
	///
	Inset * getInsetFromID(int id) const;
	///
	Paragraph * firstParagraph() const;
	///
	Paragraph * getFirstParagraph(int) const;
	///
	LyXCursor const & cursor(BufferView *) const;
	///
	bool isOpen() const { return !collapsed_; }
	///
	void open(BufferView *);
	///
	void close(BufferView *) const;
	///
	bool allowSpellcheck() const { return inset.allowSpellcheck(); }
	///
	WordLangTuple const
	selectNextWordToSpellcheck(BufferView *, float &) const;
	///
	void selectSelectedWord(BufferView * bv) {
		inset.selectSelectedWord(bv);
	}
	///
	void toggleSelection(BufferView * bv, bool kill_selection) {
		inset.toggleSelection(bv, kill_selection);
	}
 
	void markErased();
 
	bool nextChange(BufferView * bv, lyx::pos_type & length);

	///
	bool searchForward(BufferView * bv, string const & str,
			   bool = true, bool = false);
	bool searchBackward(BufferView * bv, string const & str,
			    bool = true, bool = false);

	///
	void addPreview(grfx::PreviewLoader &) const;

protected:
	///
	virtual void cache(BufferView *) const;
	///
	virtual BufferView * view() const;

	///
	int ascent_collapsed() const;
	///
	int descent_collapsed() const;
	///
	int width_collapsed() const;
	///
	void draw_collapsed(Painter & pain, int , float &) const;
	///
	int getMaxTextWidth(Painter & pain, UpdatableInset const *) const;

	///
	mutable bool collapsed_;
	///
	LColor::color framecolor;
	///
	LyXFont labelfont;
public:
	///
	mutable InsetText inset;
protected:
	///
	mutable int button_length;
	///
	mutable int button_top_y;
	///
	mutable int button_bottom_y;
	///
	mutable int topx;
	mutable int topbaseline;
	mutable UpdateCodes need_update;

private:
	///
	void lfunMousePress(FuncRequest const &);
	///
	bool lfunMouseRelease(FuncRequest const &);
	///
	void lfunMouseMotion(FuncRequest const &);

	///
	mutable string label;
#if 0
	///
	bool autocollapse;
#endif
	///
	mutable int oldWidth;
	///
	bool in_update;
	///
	mutable bool first_after_edit;
	///
	mutable boost::weak_ptr<BufferView> view_;
};

#endif
