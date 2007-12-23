/**
 * \file MathMacroTemplate.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "MathMacroTemplate.h"

#include "DocIterator.h"
#include "InsetMathBrace.h"
#include "InsetMathChar.h"
#include "InsetMathSqrt.h"
#include "MathMacro.h"
#include "MathMacroArgument.h"
#include "MathStream.h"
#include "MathParser.h"
#include "MathSupport.h"
#include "MathMacroArgument.h"

#include "Buffer.h"
#include "BufferView.h"
#include "Color.h"
#include "Cursor.h"
#include "support/debug.h"
#include "DispatchResult.h"
#include "FuncRequest.h"
#include "FuncStatus.h"
#include "support/gettext.h"
#include "Lexer.h"
#include "Undo.h"

#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"

#include "support/convert.h"
#include "support/docstream.h"
#include "support/lstrings.h"

#include "support/debug.h"

#include <sstream>

using namespace std;

namespace lyx {

using support::bformat;

//////////////////////////////////////////////////////////////////////

class InsetLabelBox : public InsetMathNest {
public:
	///
	InsetLabelBox(MathAtom const & atom, docstring label, 
								MathMacroTemplate const & parent, bool frame = false);
	InsetLabelBox(docstring label, MathMacroTemplate const & parent, 
								bool frame = false);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	
protected:
	///
	MathMacroTemplate const & parent_;
	///
	Inset * clone() const;
	///
	docstring const label_;
	///
	bool frame_;
};


InsetLabelBox::InsetLabelBox(MathAtom const & atom, docstring label, 
														 MathMacroTemplate const & parent, bool frame)
:	InsetMathNest(1), parent_(parent), label_(label), frame_(frame)
{
	cell(0).insert(0, atom);
}


InsetLabelBox::InsetLabelBox(docstring label,
														 MathMacroTemplate const & parent, bool frame)
:	InsetMathNest(1), parent_(parent), label_(label), frame_(frame)
{
}
	

Inset * InsetLabelBox::clone() const 
{
	return new InsetLabelBox(*this);
}


void InsetLabelBox::metrics(MetricsInfo & mi, Dimension & dim) const 
{
	// kernel
	cell(0).metrics(mi, dim);

	// frame
	if (frame_) {
		dim.wid += 6;
		dim.asc += 5;
		dim.des += 5;
	}
		
	// adjust to common height in main metrics phase
	if (!parent_.premetrics()) {
		dim.asc = max(dim.asc, parent_.commonLabelBoxAscent());
		dim.des = max(dim.des, parent_.commonLabelBoxDescent());
	}
	
	// label
	if (parent_.editing(mi.base.bv) && label_.length() > 0) {
		// grey
		FontInfo font = sane_font;
		font.setSize(FONT_SIZE_TINY);
		font.setColor(Color_mathmacrolabel);
		
		// make space for label and box
		int lwid = mathed_string_width(font, label_);
		int maxasc;
		int maxdes;
		math_font_max_dim(font, maxasc, maxdes);		
		
		dim.wid = max(dim.wid, lwid + 2);

		// space for the label
		if (!parent_.premetrics())
			dim.des += maxasc + maxdes + 1;
	}

	setDimCache(mi, dim);
}


void InsetLabelBox::draw(PainterInfo & pi, int x, int y) const 
{
	Dimension const dim = dimension(*pi.base.bv);
	Dimension const cdim = cell(0).dimension(*pi.base.bv);
	
	// kernel
	cell(0).draw(pi, x + (dim.wid - cdim.wid) / 2, y);
	
	// label
	if (parent_.editing(pi.base.bv) && label_.length() > 0) {
		// grey
		FontInfo font = sane_font;
		font.setSize(FONT_SIZE_TINY);
		font.setColor(Color_mathmacrolabel);
		
		// make space for label and box
		int lwid = mathed_string_width(font, label_);
		int maxasc;
		int maxdes;
		math_font_max_dim(font, maxasc, maxdes);
		
		if (lwid < dim.wid)
			pi.pain.text(x + (dim.wid - lwid) / 2, y + dim.des - maxdes, label_, font);
		else
			pi.pain.text(x, y + dim.des - maxdes, label_, font);
	}
	
	// draw frame
	int boxHeight = parent_.commonLabelBoxAscent() + parent_.commonLabelBoxDescent();
	if (frame_) {
		pi.pain.rectangle(x + 1, y - dim.ascent() + 1, 
											dim.wid - 2, boxHeight - 2, 
											Color_mathline);
	}
}


//////////////////////////////////////////////////////////////////////

class DisplayLabelBox : public InsetLabelBox {
public:
	///
	DisplayLabelBox(MathAtom const & atom, docstring label, 
									MathMacroTemplate const & parent);
		
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	
protected:
	///
	Inset * clone() const;
};


DisplayLabelBox::DisplayLabelBox(MathAtom const & atom, 
																 docstring label, 
																 MathMacroTemplate const & parent)
	: InsetLabelBox(atom, label, parent, true)
{
}
	
	
	
Inset * DisplayLabelBox::clone() const 
{
	return new DisplayLabelBox(*this);
}
	
	
void DisplayLabelBox::metrics(MetricsInfo & mi, Dimension & dim) const
{
	InsetLabelBox::metrics(mi, dim);
	if (!parent_.editing(mi.base.bv)
			&& parent_.cell(parent_.displayIdx()).empty()) {
		dim.wid = 0;
		dim.asc = 0;
		dim.des = 0;
		setDimCache(mi, dim);
	}
}

	
void DisplayLabelBox::draw(PainterInfo & pi, int x, int y) const
{
	if (parent_.editing(pi.base.bv)
			|| !parent_.cell(parent_.displayIdx()).empty()) {
		InsetLabelBox::draw(pi, x, y);
	} else {
		bool enabled = pi.pain.isDrawingEnabled();
		pi.pain.setDrawingEnabled(false);
		InsetLabelBox::draw(pi, x, y);
		pi.pain.setDrawingEnabled(enabled);
	}
}
	

//////////////////////////////////////////////////////////////////////

class InsetMathWrapper : public InsetMath {
public:
	///
	InsetMathWrapper(MathData const * value) : value_(value) {}
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	
private:
	///
	Inset * clone() const;
	///
	MathData const * value_;
};


Inset * InsetMathWrapper::clone() const 
{
	return new InsetMathWrapper(*this);
}


void InsetMathWrapper::metrics(MetricsInfo & mi, Dimension & dim) const 
{
	value_->metrics(mi, dim);
	//metricsMarkers2(dim);
}


void InsetMathWrapper::draw(PainterInfo & pi, int x, int y) const 
{
	value_->draw(pi, x, y);
	//drawMarkers(pi, x, y);
}


///////////////////////////////////////////////////////////////////////

class InsetNameWrapper : public InsetMathWrapper {
public:
	///
	InsetNameWrapper(MathData const * value, MathMacroTemplate const & parent);
	///
	void metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	
private:
	///
	MathMacroTemplate const & parent_;
	///
	Inset * clone() const;
};


InsetNameWrapper::InsetNameWrapper(MathData const * value, 
																	 MathMacroTemplate const & parent)
	:	InsetMathWrapper(value), parent_(parent)
{
}


Inset * InsetNameWrapper::clone() const 
{
	return new InsetNameWrapper(*this);
}


void InsetNameWrapper::metrics(MetricsInfo & mi, Dimension & dim) const 
{
	InsetMathWrapper::metrics(mi, dim);
	dim.wid += mathed_string_width(mi.base.font, from_ascii("\\"));
}


void InsetNameWrapper::draw(PainterInfo & pi, int x, int y) const 
{
	// create fonts
	PainterInfo namepi = pi;
	if (parent_.validMacro())
		namepi.base.font.setColor(Color_latex);
	else
		namepi.base.font.setColor(Color_error);		
	
	// draw backslash
	pi.pain.text(x, y, from_ascii("\\"), namepi.base.font);
	x += mathed_string_width(namepi.base.font, from_ascii("\\"));
	
	// draw name
	InsetMathWrapper::draw(namepi, x, y);
}

	
///////////////////////////////////////////////////////////////////////
	
	
MathMacroTemplate::MathMacroTemplate()
	: InsetMathNest(3), numargs_(0), optionals_(0), 
	  type_(MacroTypeNewcommand), lookOutdated_(true)
{
	initMath();
}


MathMacroTemplate::MathMacroTemplate(docstring const & name, int numargs,
	int optionals, MacroType type, 
	vector<MathData> const & optionalValues, 
	MathData const & def, MathData const & display)
	: InsetMathNest(optionals + 3), numargs_(numargs), 
	  optionals_(optionals), optionalValues_(optionalValues),
	  type_(type), lookOutdated_(true)
{
	initMath();

	if (numargs_ > 9)
		lyxerr << "MathMacroTemplate::MathMacroTemplate: wrong # of arguments: "
			<< numargs_ << endl;
	
	asArray(name, cell(0));
	optionalValues_.resize(9);
	for (int i = 0; i < optionals_; ++i) 
		cell(optIdx(i)) = optionalValues_[i];
	cell(defIdx()) = def;
	cell(displayIdx()) = display;

	updateLook();
}


MathMacroTemplate::MathMacroTemplate(docstring const & str)
	: InsetMathNest(3), numargs_(0), optionals_(0),
	type_(MacroTypeNewcommand), lookOutdated_(true)
{
	initMath();

	MathData ar;
	mathed_parse_cell(ar, str);
	if (ar.size() != 1 || !ar[0]->asMacroTemplate()) {
		lyxerr << "Cannot read macro from '" << ar << "'" << endl;
		asArray(from_ascii("invalidmacro"), cell(0));
		// FIXME: The macro template does not make sense after this.
		// The whole parsing should not be in a constructor which
		// has no chance to report failure.
		return;
	}
	operator=( *(ar[0]->asMacroTemplate()) );

	updateLook();
}


Inset * MathMacroTemplate::clone() const
{
	return new MathMacroTemplate(*this);
}


docstring MathMacroTemplate::name() const
{
	return asString(cell(0));
}


void MathMacroTemplate::updateToContext(MacroContext const & mc) const
{
	redefinition_ = mc.get(name()) != 0;
}

	
void MathMacroTemplate::updateLook() const
{
	lookOutdated_ = true;
}


void MathMacroTemplate::createLook() const
{
	look_.clear();

	// \foo
	look_.push_back(MathAtom(new InsetLabelBox(_("Name"), *this, false)));
	MathData & nameData = look_[look_.size() - 1].nucleus()->cell(0);
	nameData.push_back(MathAtom(new InsetNameWrapper(&cell(0), *this)));
	
	// [#1][#2]
	int i = 0;
	if (optionals_ > 0) {
		look_.push_back(MathAtom(new InsetLabelBox(_("optional"), *this, false)));
		MathData & optData = look_[look_.size() - 1].nucleus()->cell(0);
	
		for (; i < optionals_; ++i) {
			optData.push_back(MathAtom(new InsetMathChar('[')));
			optData.push_back(MathAtom(new InsetMathWrapper(&cell(1 + i))));
			optData.push_back(MathAtom(new InsetMathChar(']')));
		}
	}
	
	// {#3}{#4}
	for (; i < numargs_; ++i) {
		MathData arg;
		arg.push_back(MathAtom(new MathMacroArgument(i + 1)));
		look_.push_back(MathAtom(new InsetMathBrace(arg)));
	}
	
	// :=
	look_.push_back(MathAtom(new InsetMathChar(':')));
	look_.push_back(MathAtom(new InsetMathChar('=')));

	// definition
	look_.push_back(MathAtom(
		new InsetLabelBox(MathAtom(
			new InsetMathWrapper(&cell(defIdx()))), _("TeX"), *this,	true)));

	// display
	look_.push_back(MathAtom(
		new DisplayLabelBox(MathAtom(
			new InsetMathWrapper(&cell(displayIdx()))), _("LyX"), *this)));
}
	

void MathMacroTemplate::metrics(MetricsInfo & mi, Dimension & dim) const
{
	FontSetChanger dummy1(mi.base, from_ascii("mathnormal"));
	StyleChanger dummy2(mi.base, LM_ST_TEXT);
	
	// valid macro?
	MacroData const * macro = 0;
	if (validName()) {
		macro = mi.macrocontext.get(name());

		// updateToContext() - avoids another lookup
		redefinition_ = macro != 0;
	}
		
	// update look?
	if (lookOutdated_) {
		lookOutdated_ = false;
		createLook();
	}
	
	/// metrics for inset contents
	if (macro)
		macro->lock();		
	
	// first phase, premetric:
	premetrics_ = true;
	look_.metrics(mi, dim);
	labelBoxAscent_ = dim.asc;
	labelBoxDescent_ = dim.des;
	
	// second phase, main metric:
	premetrics_ = false;
	look_.metrics(mi, dim);
	
	if (macro)
		macro->unlock();
	
	dim.wid += 6;
	dim.des += 2;
	dim.asc += 2;
	
	setDimCache(mi, dim);
}


void MathMacroTemplate::draw(PainterInfo & pi, int x, int y) const
{
	FontSetChanger dummy1(pi.base, from_ascii("mathnormal"));
	StyleChanger dummy2(pi.base, LM_ST_TEXT);
	
	setPosCache(pi, x, y);
	Dimension const dim = dimension(*pi.base.bv);

	// draw outer frame
	int const a = y - dim.asc + 1;
	int const w = dim.wid - 2;
	int const h = dim.height() - 2;
	pi.pain.rectangle(x, a, w, h, Color_mathframe);	
	
	// just to be sure: set some dummy values for coord cache
	for (idx_type i = 0; i < nargs(); ++i) {
		cell(i).setXY(*pi.base.bv, x, y);
	}
	
	// draw contents
	look_.draw(pi, x + 3, y);
}

	
void MathMacroTemplate::edit(Cursor & cur, bool left)
{
	updateLook();
	cur.updateFlags(Update::Force);
	InsetMathNest::edit(cur, left);
}
	
	
Inset * MathMacroTemplate::editXY(Cursor & cur, int x, int y)
{
	Inset * ret = InsetMathNest::editXY(cur, x, y);
/*	if (!editing_ && editing(cur.bv())) {
		
		cur.updateFlags(Update::Force);
	}*/
	return ret;
}
	
	
bool MathMacroTemplate::notifyCursorLeaves(Cursor & cur)
{
	updateLook();
	cur.updateFlags(Update::Force);
	return InsetMathNest::notifyCursorLeaves(cur);
}
	

void MathMacroTemplate::removeArguments(Cursor & cur, int from, int to) {
	for (DocIterator it = doc_iterator_begin(*this); it; it.forwardChar()) {
		if (!it.nextInset())
			continue;
		if (it.nextInset()->lyxCode() != MATHMACROARG_CODE)
			continue;
		MathMacroArgument * arg = static_cast<MathMacroArgument*>(it.nextInset());
		int n = arg->number() - 1;
		if (from <= n && n <= to) {
			int cellSlice = cur.find(it.cell());
			if (cellSlice != -1 && cur[cellSlice].pos() > it.pos())
				--cur[cellSlice].pos();

			it.cell().erase(it.pos());
		}
	}
	
	updateLook();
}


void MathMacroTemplate::shiftArguments(size_t from, int by) {
	for (DocIterator it = doc_iterator_begin(*this); it; it.forwardChar()) {
		if (!it.nextInset())
			continue;
		if (it.nextInset()->lyxCode() != MATHMACROARG_CODE)
			continue;
		MathMacroArgument * arg = static_cast<MathMacroArgument*>(it.nextInset());
		if (arg->number() >= from + 1)
			arg->setNumber(arg->number() + by);
	}

	updateLook();
}


// FIXME: factorize those functions here with a functional style, maybe using Boost's function
// objects?

void fixMacroInstancesAddRemove(Cursor const & from, docstring const & name, int n, bool insert) {
	Cursor dit = from;

	for (; dit; dit.forwardPos()) {
		// only until a macro is redefined
		if (dit.inset().lyxCode() == MATHMACRO_CODE) {
			MathMacroTemplate const & macroTemplate
			= static_cast<MathMacroTemplate const &>(dit.inset());
			if (macroTemplate.name() == name)
				break;
		}

		// in front of macro instance?
		Inset * inset = dit.nextInset();
		if (inset) {
			InsetMath * insetMath = inset->asInsetMath();
			if (insetMath) {
				MathMacro * macro = insetMath->asMacro();
				if (macro && macro->name() == name && macro->folded()) {
					// found macro instance
					if (insert)
						macro->insertArgument(n);
					else
						macro->removeArgument(n);
				}
			}
		}
	}
}


void fixMacroInstancesOptional(Cursor const & from, docstring const & name, int optionals) {
	Cursor dit = from;

	for (; dit; dit.forwardPos()) {
		// only until a macro is redefined
		if (dit.inset().lyxCode() == MATHMACRO_CODE) {
			MathMacroTemplate const & macroTemplate
			= static_cast<MathMacroTemplate const &>(dit.inset());
			if (macroTemplate.name() == name)
				break;
		}

		// in front of macro instance?
		Inset * inset = dit.nextInset();
		if (inset) {
			InsetMath * insetMath = inset->asInsetMath();
			if (insetMath) {
				MathMacro * macro = insetMath->asMacro();
				if (macro && macro->name() == name && macro->folded()) {
					// found macro instance
					macro->setOptionals(optionals);
				}
			}
		}
	}
}


template<class F>
void fixMacroInstancesFunctional(Cursor const & from, 
	docstring const & name, F & fix) {
	Cursor dit = from;

	for (; dit; dit.forwardPos()) {
		// only until a macro is redefined
		if (dit.inset().lyxCode() == MATHMACRO_CODE) {
			MathMacroTemplate const & macroTemplate
			= static_cast<MathMacroTemplate const &>(dit.inset());
			if (macroTemplate.name() == name)
				break;
		}

		// in front of macro instance?
		Inset * inset = dit.nextInset();
		if (inset) {
			InsetMath * insetMath = inset->asInsetMath();
			if (insetMath) {
				MathMacro * macro = insetMath->asMacro();
				if (macro && macro->name() == name && macro->folded())
					F(macro);
			}
		}
	}
}


void MathMacroTemplate::insertParameter(Cursor & cur, int pos, bool greedy) 
{
	if (pos <= numargs_ && pos >= optionals_ && numargs_ < 9) {
		++numargs_;
		shiftArguments(pos, 1);

		// append example #n
		cell(defIdx()).push_back(MathAtom(new MathMacroArgument(pos + 1)));
		if (!cell(displayIdx()).empty())
			cell(displayIdx()).push_back(MathAtom(new MathMacroArgument(pos + 1)));

		if (!greedy) {
			Cursor dit = cur;
			dit.leaveInset(*this);
			// TODO: this was dit.forwardPosNoDescend before. Check that this is the same
			dit.top().forwardPos();
			
			// fix macro instances
			fixMacroInstancesAddRemove(dit, name(), pos, true);
		}
	}

	updateLook();
}


void MathMacroTemplate::removeParameter(Cursor & cur, int pos, bool greedy)
{
	if (pos < numargs_ && pos >= 0) {
		--numargs_;
		removeArguments(cur, pos, pos);
		shiftArguments(pos + 1, -1);

		// removed optional parameter?
		if (pos < optionals_) {
			--optionals_;
			optionalValues_[pos] = cell(optIdx(pos));
			cells_.erase(cells_.begin() + optIdx(pos));

			// fix cursor
			int macroSlice = cur.find(this);
			if (macroSlice != -1) {
				if (cur[macroSlice].idx() == optIdx(pos)) {
					cur.cutOff(macroSlice);
					cur[macroSlice].idx() = 1;
					cur[macroSlice].pos() = 0;
				} else if (cur[macroSlice].idx() > optIdx(pos))
					--cur[macroSlice].idx();
			}
		}

		if (!greedy) {
			// fix macro instances
			//boost::function<void(MathMacro *)> fix = _1->insertArgument(n);
			//fixMacroInstancesFunctional(dit, name(), fix);
			Cursor dit = cur;
			dit.leaveInset(*this);
			// TODO: this was dit.forwardPosNoDescend before. Check that this is the same
			dit.top().forwardPos();
			fixMacroInstancesAddRemove(dit, name(), pos, false);
		}
	}

	updateLook();
}


void MathMacroTemplate::makeOptional(Cursor & cur) {
	if (numargs_ > 0 && optionals_ < numargs_) {
		++optionals_;
		cells_.insert(cells_.begin() + optIdx(optionals_ - 1), optionalValues_[optionals_ - 1]);
		// fix cursor
		int macroSlice = cur.find(this);
		if (macroSlice != -1 && cur[macroSlice].idx() >= optIdx(optionals_ - 1))
			++cur[macroSlice].idx();

		// fix macro instances
		Cursor dit = cur;
		dit.leaveInset(*this);
		// TODO: this was dit.forwardPosNoDescend before. Check that this is the same
		dit.top().forwardPos();
		fixMacroInstancesOptional(dit, name(), optionals_);
	}

	updateLook();
}


void MathMacroTemplate::makeNonOptional(Cursor & cur) {
	if (numargs_ > 0 && optionals_ > 0) {
		--optionals_;
		
		// store default value for later if the use changes his mind
		optionalValues_[optionals_] = cell(optIdx(optionals_));
		cells_.erase(cells_.begin() + optIdx(optionals_));

		// fix cursor
		int macroSlice = cur.find(this);
		if (macroSlice != -1) {
			if (cur[macroSlice].idx() > optIdx(optionals_))
				--cur[macroSlice].idx();
			else if (cur[macroSlice].idx() == optIdx(optionals_)) {
				cur.cutOff(macroSlice);
				cur[macroSlice].idx() = optIdx(optionals_);
				cur[macroSlice].pos() = 0;
			}
		}

		// fix macro instances
		Cursor dit = cur;
		dit.leaveInset(*this);
		// TODO: this was dit.forwardPosNoDescend before. Check that this is the same
		dit.top().forwardPos();
		fixMacroInstancesOptional(dit, name(), optionals_);
	}

	updateLook();
}


void MathMacroTemplate::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	string const arg = to_utf8(cmd.argument());
	switch (cmd.action) {

	case LFUN_MATH_MACRO_ADD_PARAM: 
		if (numargs_ < 9) {
			cur.recordUndoFullDocument();
			size_t pos = numargs_;
			if (arg.size() != 0)
				pos = (size_t)convert<int>(arg) - 1; // it is checked for >=0 in getStatus
			insertParameter(cur, pos);
		}
		break;


	case LFUN_MATH_MACRO_REMOVE_PARAM: 
		if (numargs_ > 0) {
			cur.recordUndoFullDocument();
			size_t pos = numargs_ - 1;
			if (arg.size() != 0)
				pos = (size_t)convert<int>(arg) - 1; // it is checked for >=0 in getStatus
			removeParameter(cur, pos);
		}
		break;

	case LFUN_MATH_MACRO_APPEND_GREEDY_PARAM:
		if (numargs_ < 9) {
			cur.recordUndoFullDocument();
			insertParameter(cur, numargs_, true);
		}
		break;

	case LFUN_MATH_MACRO_REMOVE_GREEDY_PARAM:
		if (numargs_ > 0) {
			cur.recordUndoFullDocument();
			removeParameter(cur, numargs_ - 1, true);
		}
		break;

	case LFUN_MATH_MACRO_MAKE_OPTIONAL:
		cur.recordUndoFullDocument();
		makeOptional(cur);
		break;

	case LFUN_MATH_MACRO_MAKE_NONOPTIONAL:
		cur.recordUndoFullDocument();
		makeNonOptional(cur);
		break;

	case LFUN_MATH_MACRO_ADD_OPTIONAL_PARAM:
		if (numargs_ < 9) {
			cur.recordUndoFullDocument();
			insertParameter(cur, optionals_);
			makeOptional(cur);
		}
		break;

	case LFUN_MATH_MACRO_REMOVE_OPTIONAL_PARAM:
		if (optionals_ > 0) {
			cur.recordUndoFullDocument();
			removeParameter(cur, optionals_ - 1);
		} break;

	case LFUN_MATH_MACRO_ADD_GREEDY_OPTIONAL_PARAM:
		if (numargs_ == optionals_) {
			cur.recordUndoFullDocument();
			insertParameter(cur, 0, true);
			makeOptional(cur);
		}
		break;

	default:
		InsetMathNest::doDispatch(cur, cmd);
		break;
	}
}


bool MathMacroTemplate::getStatus(Cursor & /*cur*/, FuncRequest const & cmd,
	FuncStatus & flag) const
{
	bool ret = true;
	string const arg = to_utf8(cmd.argument());
	switch (cmd.action) {
		case LFUN_MATH_MACRO_ADD_PARAM: {
			int num = numargs_ + 1;
			if (arg.size() != 0)
				num = convert<int>(arg);
			bool on = (num >= optionals_ 
				   && numargs_ < 9 && num <= numargs_ + 1);
			flag.enabled(on);
			break;
		}

		case LFUN_MATH_MACRO_APPEND_GREEDY_PARAM:
			flag.enabled(numargs_ < 9);
			break;

		case LFUN_MATH_MACRO_REMOVE_PARAM: {
			int num = numargs_;
			if (arg.size() != 0)
				num = convert<int>(arg);
			flag.enabled(num >= 1 && num <= numargs_);
			break;
		}

		case LFUN_MATH_MACRO_MAKE_OPTIONAL:
			flag.enabled(numargs_ > 0 
				     && optionals_ < numargs_ 
				     && type_ != MacroTypeDef);
			break;

		case LFUN_MATH_MACRO_MAKE_NONOPTIONAL:
			flag.enabled(optionals_ > 0 
				     && type_ != MacroTypeDef);
			break;

		case LFUN_MATH_MACRO_ADD_OPTIONAL_PARAM:
			flag.enabled(numargs_ < 9);
			break;

		case LFUN_MATH_MACRO_REMOVE_OPTIONAL_PARAM:
			flag.enabled(optionals_ > 0);
			break;

		case LFUN_MATH_MACRO_ADD_GREEDY_OPTIONAL_PARAM:
			flag.enabled(numargs_ == 0 
				     && type_ != MacroTypeDef);
			break;

		case LFUN_IN_MATHMACROTEMPLATE:
			flag.enabled();
			break;

		default:
			ret = false;
			break;
	}
	return ret;
}


void MathMacroTemplate::read(Buffer const &, Lexer & lex)
{
	MathData ar;
	mathed_parse_cell(ar, lex.getStream());
	if (ar.size() != 1 || !ar[0]->asMacroTemplate()) {
		lyxerr << "Cannot read macro from '" << ar << "'" << endl;
		lyxerr << "Read: " << to_utf8(asString(ar)) << endl;
		return;
	}
	operator=( *(ar[0]->asMacroTemplate()) );
	
	updateLook();
}


void MathMacroTemplate::write(Buffer const &, ostream & os) const
{
	odocstringstream oss;
	WriteStream wi(oss, false, false);
	oss << "FormulaMacro\n";
	write(wi);
	os << to_utf8(oss.str());
}


void MathMacroTemplate::write(WriteStream & os) const
{
	write(os, false);
}


void MathMacroTemplate::write(WriteStream & os, bool overwriteRedefinition) const
{
	if (type_ == MacroTypeDef) {
		os << "\\def\\" << name().c_str();
		for (int i = 1; i <= numargs_; ++i)
			os << '#' << i;
	} else {
		// newcommand or renewcommand
		if (redefinition_ && !overwriteRedefinition)
			os << "\\renewcommand";
		else
			os << "\\newcommand";
		os << "{\\" << name().c_str() << '}';
		if (numargs_ > 0)
			os << '[' << numargs_ << ']';
		
		// optional values
		if (os.latex()) {
			// in latex only one optional possible, simulate the others
			if (optionals_ >= 1) {
				docstring optValue = asString(cell(optIdx(0)));
				if (optValue.find(']') != docstring::npos)
					os << "[{" << cell(optIdx(0)) << "}]";
				else
					os << "[" << cell(optIdx(0)) << "]";
			}
		} else {
			// in lyx we handle all optionals as real optionals
			for (int i = 0; i < optionals_; ++i) {
				docstring optValue = asString(cell(optIdx(i)));
				if (optValue.find(']') != docstring::npos)
					os << "[{" << cell(optIdx(i)) << "}]";
				else
					os << "[" << cell(optIdx(i)) << "]";
			}
		}
	}

	os << "{" << cell(defIdx()) << "}";

	if (os.latex()) {
		// writing .tex. done.
		os << "\n";
	} else {
		// writing .lyx, write special .tex export only if necessary
		if (!cell(displayIdx()).empty())
			os << "\n{" << cell(displayIdx()) << '}';
	}
}


int MathMacroTemplate::plaintext(Buffer const & buf, odocstream & os,
				 OutputParams const &) const
{
	static docstring const str = '[' + buf.B_("math macro") + ']';

	os << str;
	return str.size();
}


bool MathMacroTemplate::validName() const
{
	docstring n = name();

	// empty name?
	if (n.size() == 0)
		return false;

	// converting back and force doesn't swallow anything?
	/*MathData ma;
	asArray(n, ma);
	if (asString(ma) != n)
		return false;*/

	// valid characters?
	for (size_t i = 0; i < n.size(); ++i) {
		if (!(n[i] >= 'a' && n[i] <= 'z') &&
				!(n[i] >= 'A' && n[i] <= 'Z')) 
			return false;
	}

	return true;
}


bool MathMacroTemplate::validMacro() const
{
	return validName();
}


bool MathMacroTemplate::fixNameAndCheckIfValid()
{
	// check all the characters/insets in the name cell
	size_t i = 0;
	MathData & data = cell(0);
	while (i < data.size()) {
		InsetMathChar const * cinset = data[i]->asCharInset();
		if (cinset) {
			// valid character in [a-zA-Z]?
			char_type c = cinset->getChar();
			if ((c >= 'a' && c <= 'z')
			    || (c >= 'A' && c <= 'Z')) {
				++i;
				continue;
			}
		}
		
		// throw cell away
		data.erase(i);
	}

	// now it should be valid if anything in the name survived
	return data.size() > 0;
}


void MathMacroTemplate::getDefaults(vector<docstring> & defaults) const
{
	defaults.resize(numargs_);
	for (int i = 0; i < optionals_; ++i)
		defaults[i] = asString(cell(optIdx(i)));	
}


docstring MathMacroTemplate::definition() const
{
	return asString(cell(defIdx()));
}


docstring MathMacroTemplate::displayDefinition() const
{
	return asString(cell(displayIdx()));
}


size_t MathMacroTemplate::numArgs() const
{
	return numargs_;
}


size_t MathMacroTemplate::numOptionals() const
{
	return optionals_;
}

	
void MathMacroTemplate::infoize(odocstream & os) const
{
	os << "Math Macro: \\" << name();
}

} // namespace lyx
