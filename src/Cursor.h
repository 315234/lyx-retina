// -*- C++ -*-
/**
 * \file Cursor.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LCURSOR_H
#define LCURSOR_H

#include "DispatchResult.h"
#include "DocIterator.h"

#include <iosfwd>
#include <vector>


namespace lyx {

class Buffer;
class BufferView;
class FuncStatus;
class FuncRequest;
class Font;
class Row;

// these should go
class InsetMathUnknown;
class Encoding;


/// The cursor class describes the position of a cursor within a document.

// The public inheritance should go in favour of a suitable data member
// (or maybe private inheritance) at some point of time.
class Cursor : public DocIterator {
public:
	/// create the cursor of a BufferView
	explicit Cursor(BufferView & bv);

	/// dispatch from innermost inset upwards
	void dispatch(FuncRequest const & cmd);
	/// get the resut of the last dispatch
	DispatchResult result() const;
	/// add a new cursor slice
	void push(Inset & inset);
	/// add a new cursor slice, place cursor on left end
	void pushLeft(Inset & inset);
	/// pop one level off the cursor
	void pop();
	/// pop one slice off the cursor stack and go left
	bool popLeft();
	/// pop one slice off the cursor stack and go right
	bool popRight();
	/// make sure we are outside of given inset
	void leaveInset(Inset const & inset);
	/// sets cursor part
	void setCursor(DocIterator const & it);

	//
	// selection
	//
	/// selection active?
	bool selection() const { return selection_; }
	/// selection active?
	bool & selection() { return selection_; }
	/// did we place the anchor?
	bool mark() const { return mark_; }
	/// did we place the anchor?
	bool & mark() { return mark_; }
	///
	void setSelection();
	/// set selection at given position
	void setSelection(DocIterator const & where, int n);
	///
	void clearSelection();
	/// access start of selection
	CursorSlice selBegin() const;
	/// access end of selection
	CursorSlice selEnd() const;
	/// access start of selection
	DocIterator selectionBegin() const;
	/// access start of selection
	DocIterator selectionEnd() const;
	///
	bool selHandle(bool selecting);
	//
	docstring selectionAsString(bool label) const;
	///
	docstring currentState();

	/// auto-correct mode
	bool autocorrect() const { return autocorrect_; }
	/// auto-correct mode
	bool & autocorrect() { return autocorrect_; }
	/// are we entering a macro name?
	bool macromode() const { return macromode_; }
	/// are we entering a macro name?
	bool & macromode() { return macromode_; }
	/// returns x,y position
	void getPos(int & x, int & y) const;
	/// the row in the paragraph we're in
	Row const & textRow() const;

	//
	// common part
	//
	/// move one step to the left
	bool posLeft();
	/// move one step to the right
	bool posRight();

	/// insert an inset
	void insert(Inset *);
	/// insert a single char
	void insert(char_type c);
	/// insert a string
	void insert(docstring const & str);

	/// in pixels from left of screen
	int targetX() const;
	/// write acess to target x position of cursor
	int & x_target();
	/// return target x position of cursor
	int x_target() const;
	/// set targetX in current position
	void setTargetX();
	/// clear target x position of cursor
	void clearTargetX();

	/// access to normalized selection anchor
	CursorSlice anchor() const;
	/// sets anchor to cursor position
	void resetAnchor();
	/// access to owning BufferView
	BufferView & bv() const;
	/// access to owning Buffer
	Buffer & buffer() const;
	/// get some interesting description of top position
	void info(odocstream & os) const;
	/// are we in math mode (2), text mode (1) or unsure (0)?
	int currentMode();
	/// reset cursor bottom to the beginning of the given inset
	// (sort of 'chroot' environment...)
	void reset(Inset &);
	/// for spellchecking
	void replaceWord(std::string const & replacestring);
	/**
	 * the event was not (yet) dispatched.
	 *
	 * Should only be called by an inset's doDispatch() method. It means:
	 * I, the doDispatch() method of InsetFoo, hereby declare that I am
	 * not able to handle that request and trust my parent will do the
	 * Right Thing (even if my getStatus partner said that I can do it).
	 * It is sort of a kludge that should be used only rarely...
	 */
	void undispatched();
	/// the event was already dispatched
	void dispatched();
	/// Set which update should be done
	void updateFlags(Update::flags f);
	/**
	 * don't call update() when done
	 *
	 * Should only be called by an inset's doDispatch() method. It means:
	 * I handled that request and I can reassure you that the screen does
	 * not need to be re-drawn and all entries in the coord cache stay
	 * valid (and there are no other things to put in the coord cache).
	 * This is a fairly rare event as well and only some optimization.
	 * Not using noUpdate() should never be wrong.
	 */
	void noUpdate();
	/// fix cursor in circumstances that should never happen
	void fixIfBroken();

	/// output
	friend std::ostream & operator<<(std::ostream & os, Cursor const & cur);

public:
	///
	BufferView * bv_;
//private:
	/// the anchor position
	DocIterator anchor_;

	///
	DispatchResult disp_;


private:
	/**
	 * The target x position of the cursor. This is used for when
	 * we have text like :
	 *
	 * blah blah blah blah| blah blah blah
	 * blah blah blah
	 * blah blah blah blah blah blah
	 *
	 * When we move onto row 3, we would like to be vertically aligned
	 * with where we were in row 1, despite the fact that row 2 is
	 * shorter than x()
	 */
	int x_target_;
	/// do we have a selection?
	bool selection_;
	/// are we on the way to get one?
	bool mark_;
	/// If true, we are behind the previous char, otherwise we are in front
	// of the next char. This only make a difference when we are in front
	// of a big inset spanning a whole row and computing coordinates for
	// displaying the cursor.
	bool logicalpos_;

private:

	//
	// math specific stuff that could be promoted to "global" later
	//
	/// do we allow autocorrection
	bool autocorrect_;
	/// are we entering a macro name?
	bool macromode_;


///////////////////////////////////////////////////////////////////
//
// The part below is the non-integrated rest of the original math
// cursor. This should be either generalized for texted or moved
// back to the math insets.
//
///////////////////////////////////////////////////////////////////

public:
	///
	void insert(MathAtom const &);
	///
	void insert(MathData const &);
	/// return false for empty math insets
	bool erase();
	/// return false for empty math insets
	bool backspace();
	/// called for LFUN_UP etc
	bool up();
	/// called for LFUN_DOWN etc
	bool down();
	///
	void plainErase();
	///
	void plainInsert(MathAtom const & at);
	///
	void niceInsert(MathAtom const & at);
	///
	void niceInsert(docstring const & str);

	/// in pixels from top of screen
	void setScreenPos(int x, int y);
	/// current offset in the top cell

	/// interpret name a name of a macro. Returns true if
	/// something got inserted.
	bool macroModeClose();
	/// are we currently typing the name of a macro?
	bool inMacroMode() const;
	/// get access to the macro we are currently typing
	InsetMathUnknown * activeMacro();

	/// replace selected stuff with at, placing the former
	// selection in given cell of atom
	void handleNest(MathAtom const & at, int cell = 0);
	///
	bool isInside(Inset const *);

	/// make sure cursor position is valid
	void normalize();
	/// mark current cursor trace for redraw
	void touch();

	/// hack for reveal codes
	void markInsert();
	void markErase();
	/// injects content of a cell into parent
	void pullArg();
	/// split font inset etc
	void handleFont(std::string const & font);

	/// display a message
	void message(docstring const & msg) const;
	/// display an error message
	void errorMessage(docstring const & msg) const;
	///
	docstring getPossibleLabel();

	/// moves position somehow up or down
	bool goUpDown(bool up);

	/// the name of the macro we are currently inputting
	docstring macroName();
	/// where in the curent cell does the macro name start?
	int macroNamePos();
	/// can we enter the inset?
	bool openable(MathAtom const &) const;
	///
	Encoding const * getEncoding() const;
	/// font at cursor position
	Font getFont() const;
	///
	bool isRTL() const;
};



} // namespace lyx

#endif // LYXLCURSOR_H
