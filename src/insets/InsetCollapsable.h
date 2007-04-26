// -*- C++ -*-
/**
 * \file InsetCollapsable.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INSETCOLLAPSABLE_H
#define INSETCOLLAPSABLE_H

#include "Inset.h"
#include "InsetText.h"

#include "Box.h"
#include "LyXFont.h"

#include <string>

namespace lyx {

class LyXText;
class Paragraph;
class CursorSlice;

namespace frontend { class Painter; }

/** A collapsable text inset

*/
class InsetCollapsable : public InsetText {
public:
	///
	static int const TEXT_TO_TOP_OFFSET = 2;
	///
	static int const TEXT_TO_BOTTOM_OFFSET = 2;
	///
	InsetCollapsable(BufferParams const &, CollapseStatus status = Open);
	///
	InsetCollapsable(InsetCollapsable const & rhs);
	///
	void read(Buffer const &, Lexer &);
	///
	void write(Buffer const &, std::ostream &) const;
	///
	bool metrics(MetricsInfo &, Dimension &) const;
	///
	void draw(PainterInfo & pi, int x, int y) const;
	///
	void drawSelection(PainterInfo & pi, int x, int y) const;
	/// return x,y of given position relative to the inset's baseline
	void cursorPos(BufferView const & bv, CursorSlice const & sl,
		bool boundary, int & x, int & y) const;
	///
	bool hitButton(FuncRequest const &) const;
	///
	docstring const getNewLabel(docstring const & l) const;
	///
	EDITABLE editable() const;
	/// can we go further down on mouse click?
	bool descendable() const;
	///
	void setLabel(docstring const & l);
	///
	virtual void setButtonLabel() {}
	///
	void setLabelFont(LyXFont & f);
	///
	bool isOpen() const { return status_ == Open || status_ == Inlined; }
	///
	bool inlined() const { return status_ == Inlined; }
	///
	CollapseStatus status() const;
	///
	bool allowSpellCheck() const { return true; }
	///
	bool getStatus(LCursor &, FuncRequest const &, FuncStatus &) const;
	///
	void setStatus(LCursor & cur, CollapseStatus st);
	///
	bool setMouseHover(bool mouse_hover);

protected:
	///
	virtual void doDispatch(LCursor & cur, FuncRequest & cmd);
	///
	Dimension dimensionCollapsed() const;
	///
	Box const & buttonDim() const;
	///
	void edit(LCursor & cur, bool left);
	///
	InsetBase * editXY(LCursor & cur, int x, int y);
	///
	void setInlined() { status_ = Inlined; }
	///
	docstring floatName(std::string const & type, BufferParams const &) const;

protected:
	///
	LyXFont labelfont_;
	///
	mutable Box button_dim;
	///
	mutable int topx;
	///
	mutable int topbaseline;
	///
	mutable docstring label;
private:
	///
	mutable CollapseStatus status_;
	/// a substatus of the Open status, determined automatically in metrics
	mutable bool openinlined_;
	/// the inset will automatically open when the cursor is inside
	mutable bool autoOpen_;
	///
	mutable Dimension textdim_;
	/// changes color when mouse enters/leaves this inset
	bool mouse_hover_;
};

// A helper function that pushes the cursor out of the inset.
void leaveInset(LCursor & cur, InsetBase const & in);

} // namespace lyx

#endif
