// -*- C++ -*-
/**
 * \file InsetText.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETTEXT_H
#define INSETTEXT_H

#include "Inset.h"
#include "RowList_fwd.h"
#include "lyxfont.h"
#include "lyxtext.h"
#include "ParagraphList_fwd.h"

#include "support/types.h"

#include "frontends/mouse_state.h"


namespace lyx {

class Buffer;
class BufferParams;
class BufferView;
class CursorSlice;
class Dimension;
class LColor_color;


/**
 A text inset is like a TeX box to write full text
 (including styles and other insets) in a given space.
 */
class InsetText : public InsetOld {
public:
	///
	explicit InsetText(BufferParams const &);
	///
	InsetText();
	///
	virtual ~InsetText() {}

	/// empty inset to empty par
	void clear();
	///
	void read(Buffer const & buf, LyXLex & lex);
	///
	void write(Buffer const & buf, std::ostream & os) const;
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	/// draw inset selection
	void drawSelection(PainterInfo & pi, int x, int y) const;
	/// are we inside the area covered by the inset?
	virtual bool covers(BufferView const & bv, int x, int y) const;
	///
	virtual docstring const editMessage() const;
	///
	EDITABLE editable() const { return HIGHLY_EDITABLE; }
	///
	bool canTrackChanges() const { return true; }
	///
	InsetText const * asTextInset() const { return this; }
	///
	int latex(Buffer const &, odocstream &, OutputParams const &) const;
	///
	int plaintext(Buffer const &, odocstream &, OutputParams const &) const;
	///
	int docbook(Buffer const &, odocstream &, OutputParams const &) const;
	///
	void validate(LaTeXFeatures & features) const;

	/// return x,y of given position relative to the inset's baseline
	void cursorPos(BufferView const & bv, CursorSlice const & sl,
		bool boundary, int & x, int & y) const;
	///
	Code lyxCode() const { return TEXT_CODE; }
	///
	void setText(docstring const &, LyXFont const &, bool trackChanges);
	///
	void setAutoBreakRows(bool);
	///
	bool getAutoBreakRows() const { return text_.autoBreakRows_; }
	///
	void setDrawFrame(bool);
	///
	LColor_color frameColor() const;
	///
	void setFrameColor(LColor_color);
	///
	bool showInsetDialog(BufferView *) const;
	///
	LyXText * getText(int i) const {
		return (i == 0) ? const_cast<LyXText*>(&text_) : 0;
	}
	///
	virtual bool getStatus(LCursor & cur, FuncRequest const & cmd, FuncStatus &) const;

	/// set the change for the entire inset
	void setChange(Change const & change);
	/// accept the changes within the inset
	void acceptChanges(BufferParams const & bparams);
	/// reject the changes within the inset
	void rejectChanges(BufferParams const & bparams);

	/// append text onto the existing text
	void appendParagraphs(Buffer * bp, ParagraphList &);

	///
	void addPreview(graphics::PreviewLoader &) const;

	///
	void edit(LCursor & cur, bool left);
	///
	InsetBase * editXY(LCursor & cur, int x, int y);

	/// number of cells in this inset
	size_t nargs() const { return 1; }
	///
	ParagraphList & paragraphs();
	///
	ParagraphList const & paragraphs() const;
	///
	bool insetAllowed(Code) const { return true; }
	///
	bool allowSpellCheck() const { return true; }
	/// should paragraph indendation be ommitted in any case?
	bool neverIndent(Buffer const &) const;
	///
	InsetText(InsetText const &);
	///
	virtual bool wide() const { return wide_inset_; }
	///
	void setWide(bool wide_inset) { wide_inset_ = wide_inset; }

protected:
	///
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);

private:
	///
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	void init();

	///
	bool drawFrame_;
	/** We store the LColor::color value as an int to get LColor.h out
	 *  of the header file.
	 */
	int frame_color_;
	///
	mutable pit_type old_pit;
	///
	bool wide_inset_;
public:
	///
	mutable LyXText text_;
	///
	mutable LyXFont font_;
	///
	static int border_;
};

} // namespace lyx

#endif
