// -*- C++ -*-

/*
 * Common parts of the math LyX insets
 * \author  Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS
 *
 * You are free to use and modify this code under the terms of
 * the GNU General Public Licence version 2 or later.
 */

#ifndef INSET_FORMULABASE_H
#define INSET_FORMULABASE_H


#include "insets/updatableinset.h"
#include "frontends/mouse_state.h"
#include "lyxfont.h"

#include <boost/weak_ptr.hpp>
#include <iosfwd>

class Buffer;
class BufferView;
class MathAtom;

/// An abstract base class for all math related LyX insets
class InsetFormulaBase : public UpdatableInset {
public:
	///
	InsetFormulaBase();
	///
	virtual int ascent(BufferView *, LyXFont const &) const = 0;
	///
	virtual int descent(BufferView *, LyXFont const &) const = 0;
	///
	virtual int width(BufferView *, LyXFont const &) const = 0;
	///
	virtual void draw(BufferView *,LyXFont const &, int, float &) const = 0;
	/// lowest x coordinate
	virtual int xlow() const;
	/// highest x coordinate
	virtual int xhigh() const;
	/// lowest y coordinate
	virtual int ylow() const;
	/// highest y coordinate
	virtual int yhigh() const;

public:
	///
	// Don't use this for AMS validation as long as there is no
	// user-accessible way to override "false positives"
	virtual void validate(LaTeXFeatures &) const;
	///
	virtual Inset * clone(Buffer const &, bool same_id = false) const = 0;
	///
	virtual Inset::Code lyxCode() const;
	/// what appears in the minibuffer when opening
	virtual string const editMessage() const;
	///
	virtual void edit(BufferView *, int x, int y, mouse_button::state button);
	///
	virtual void edit(BufferView *, bool front = true);
	///
	virtual void toggleInsetCursor(BufferView *);
	///
	virtual void showInsetCursor(BufferView *, bool show = true);
	///
	virtual void hideInsetCursor(BufferView *);
	///
	virtual void fitInsetCursor(BufferView *) const;
	///
	virtual void getCursorPos(BufferView *, int &, int &) const;
	///
	virtual void toggleInsetSelection(BufferView * bv);
	///
	virtual void insetUnlock(BufferView *);

	/// To allow transparent use of math editing functions
	virtual dispatch_result localDispatch(FuncRequest const &);
	/// To allow transparent use of math editing functions
	//virtual void status(FuncRequest const &);

	///
	virtual std::vector<string> const getLabelList() const;
	///
	virtual MathAtom const & par() const = 0;
	///
	virtual MathAtom & par() = 0;
	///
	virtual void updateLocal(BufferView * bv, bool mark_dirty);
	///
	// And shouldn't this really return a shared_ptr<BufferView> instead?
	BufferView * view() const;

	///
	virtual bool searchForward(BufferView *, string const &,
				   bool = true, bool = false);
	///
	virtual bool searchBackward(BufferView *, string const &,
				    bool = true, bool = false);
	///
	virtual bool isTextInset() const { return true; }
	///
	virtual void mutateToText();
	///
	virtual void revealCodes(BufferView *) const;
	///
	virtual EDITABLE editable() const { return HIGHLY_EDITABLE; }
	///
	bool display() const;
	// return the selection as string
	string selectionAsString() const;

private:
	/// unimplemented
	void operator=(const InsetFormulaBase &);
	/// common base for handling accents
	void handleAccent(BufferView * bv, string const & arg, string const & name);
	/// lfun handler
	dispatch_result lfunMousePress(FuncRequest const &);
	///
	dispatch_result lfunMouseRelease(FuncRequest const &);
	///
	dispatch_result lfunMouseMotion(FuncRequest const &);

protected:
	///
	mutable boost::weak_ptr<BufferView> view_;
	///
	mutable LyXFont font_;

protected:
	///
	void metrics(BufferView * bv, LyXFont const & font) const;
	///
	void metrics(BufferView * bv = 0) const;
	///
	void handleFont(BufferView * bv, string const & arg, string const & font);

	///
	mutable int xo_;
	///
	mutable int yo_;
};

// We don't really mess want around with mathed stuff outside mathed.
// So do it here.
void mathDispatch(FuncRequest const &);

///
void releaseMathCursor(BufferView * bv);

#endif
