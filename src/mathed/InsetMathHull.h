// -*- C++ -*-
/**
 * \file InsetMathHull.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_HULLINSET_H
#define MATH_HULLINSET_H

#include "InsetMathGrid.h"

#include "OutputEnums.h"

#include <boost/scoped_ptr.hpp>


namespace lyx {

class InsetLabel;
class ParConstIterator;
class RenderPreview;


/// This provides an interface between "LyX insets" and "LyX math insets"
class InsetMathHull : public InsetMathGrid {
public:
	///
	InsetMathHull(Buffer * buf);
	///
	InsetMathHull(Buffer * buf, HullType type);
	///
	~InsetMathHull();
	///
	void setBuffer(Buffer &);
	///
	void updateLabels(ParIterator const &, UpdateType);
	///
	void addToToc(DocIterator const &);
	///
	InsetMathHull & operator=(InsetMathHull const &);
	///
	mode_type currentMode() const;
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	/// 
	void drawBackground(PainterInfo & pi, int x, int y) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	void metricsT(TextMetricsInfo const & mi, Dimension & dim) const;
	///
	void drawT(TextPainter &, int x, int y) const;
	///
	docstring label(row_type row) const;
	///
	void label(row_type row, docstring const & label);
	///
	std::vector<InsetLabel *> getLabels() { return label_; }
	///
	ColorCode backgroundColor(PainterInfo const &) const;
	///
	void numbered(row_type row, bool num);
	///
	bool numbered(row_type row) const;
	///
	bool numberedType() const;
	///
	bool ams() const;
	///
	void validate(LaTeXFeatures & features) const;
	/// identifies HullInset
	InsetMathHull const * asHullInset() const { return this; }
	/// identifies HullInset
	InsetMathHull * asHullInset() { return this; }

	/// add a row
	void addRow(row_type row);
	/// delete a row
	void delRow(row_type row);
	/// swap two rows
	void swapRow(row_type row);
	/// add a column
	void addCol(col_type col);
	/// delete a column
	void delCol(col_type col);

	/// get type
	HullType getType() const;
	/// change type
	void mutate(HullType newtype);

	///
	int defaultColSpace(col_type col);
	///
	char defaultColAlign(col_type col);
	///
	bool idxFirst(Cursor &) const;
	///
	bool idxLast(Cursor &) const;

	///
	void write(WriteStream & os) const;
	///
	void mathmlize(MathStream &) const;
	///
	void normalize(NormalStream &) const;
	///
	void infoize(odocstream & os) const;

	///
	void write(std::ostream & os) const;
	///
	void header_write(WriteStream &) const;
	///
	void footer_write(WriteStream &) const;
	///
	void read(Lexer & lex);
	///
	bool readQuiet(Lexer & lex);
	///
	int plaintext(odocstream &, OutputParams const &) const;
	///
	int docbook(odocstream &, OutputParams const &) const;
	///
	docstring xhtml(XHTMLStream &, OutputParams const &) const;
	/// the string that is passed to the TOC
	void tocString(odocstream &) const;

	/// get notification when the cursor leaves this inset
	bool notifyCursorLeaves(Cursor const & old, Cursor & cur);
	///
	//bool insetAllowed(InsetCode code) const;
	///
	void addPreview(DocIterator const & inset_pos,
		graphics::PreviewLoader &) const;
	/// Prepare the preview if preview is enabled.
	void preparePreview(DocIterator const & pos) const;
	/// Recreates the preview if preview is enabled.
	void reloadPreview(DocIterator const & pos) const;
	///
	void initUnicodeMath() const;

	///
	static int displayMargin() { return 12; }
	
	/// Force inset into LTR environment if surroundings are RTL?
	virtual bool forceLTR() const { return true; }

	///
	virtual docstring contextMenu(BufferView const &, int, int) const;
	///
	InsetCode lyxCode() const { return MATH_HULL_CODE; }

protected:
	InsetMathHull(InsetMathHull const &);

	virtual void doDispatch(Cursor & cur, FuncRequest & cmd);

	/// do we want to handle this event?
	bool getStatus(Cursor & cur, FuncRequest const & cmd,
		FuncStatus & status) const;
	///
	docstring eolString(row_type row, bool fragile) const;

private:
	virtual Inset * clone() const;
	///
	void setType(HullType type);
	///
	void validate1(LaTeXFeatures & features);
	///
	docstring nicelabel(row_type row) const;
	///
	void doExtern(Cursor & cur, FuncRequest & func);
	///
	void glueall();
	/*!
	 * split every row at the first relation operator.
	 * The number of columns must be 1. One column is added.
	 * The first relation operator and everything after it goes to the
	 * second column.
	 */
	void splitTo2Cols();
	/*!
	 * split every row at the first relation operator.
	 * The number of columns must be < 3. One or two columns are added.
	 * The first relation operator goes to the second column.
	 * Everything after it goes to the third column.
	 */
	void splitTo3Cols();
	/// change number of columns, split or combine columns if necessary.
	void changeCols(col_type);
	///
	docstring standardFont() const;
	/// consistency check
	void check() const;
	/// can this change its number of rows?
	bool rowChangeOK() const;
	/// can this change its number of cols?
	bool colChangeOK() const;

	/// "none", "simple", "display", "eqnarray",...
	HullType type_;
	///
	std::vector<bool> nonum_;
	///
	std::vector<InsetLabel *> label_;
	///
	boost::scoped_ptr<RenderPreview> preview_;
	///
	mutable bool use_preview_;
//
// Incorporate me
//
public:
	///
	virtual void mutateToText();
	///
	virtual void revealCodes(Cursor & cur) const;
	///
	bool editable() const { return true; }
	///
	void edit(Cursor & cur, bool front, 
		EntryDirection entry_from = ENTRY_DIRECTION_IGNORE);
	///
	Inset * editXY(Cursor & cur, int x, int y);
	///
	DisplayType display() const;

protected:
	///
	void handleFont(Cursor & cur, docstring const & arg,
		docstring const & font);
	///
	void handleFont2(Cursor & cur, docstring const & arg);
	///
	bool previewState(BufferView * bv) const;
};



} // namespace lyx
#endif
