/**
 * \file InsetMathScript.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetMathScript.h"
#include "MathData.h"
#include "MathStream.h"
#include "MathSupport.h"
#include "InsetMathSymbol.h"
#include "InsetMathFont.h"
#include "DispatchResult.h"
#include "Cursor.h"
#include "debug.h"
#include "FuncRequest.h"
#include "Undo.h"

#include <boost/assert.hpp>


namespace lyx {

using std::string;
using std::max;
using std::auto_ptr;
using std::endl;



InsetMathScript::InsetMathScript()
	: InsetMathNest(1), cell_1_is_up_(false), limits_(0)
{}


InsetMathScript::InsetMathScript(bool up)
	: InsetMathNest(2), cell_1_is_up_(up), limits_(0)
{}


InsetMathScript::InsetMathScript(MathAtom const & at, bool up)
	: InsetMathNest(2), cell_1_is_up_(up), limits_(0)
{
	BOOST_ASSERT(nargs() >= 1);
	cell(0).push_back(at);
}


auto_ptr<InsetBase> InsetMathScript::doClone() const
{
	return auto_ptr<InsetBase>(new InsetMathScript(*this));
}


InsetMathScript const * InsetMathScript::asScriptInset() const
{
	return this;
}


InsetMathScript * InsetMathScript::asScriptInset()
{
	return this;
}


bool InsetMathScript::idxFirst(Cursor & cur) const
{
	cur.idx() = 0;
	cur.pos() = 0;
	return true;
}


bool InsetMathScript::idxLast(Cursor & cur) const
{
	cur.idx() = 0;
	cur.pos() = nuc().size();
	return true;
}


MathData const & InsetMathScript::down() const
{
	if (nargs() == 3)
		return cell(2);
	BOOST_ASSERT(nargs() > 1);
	return cell(1);
}


MathData & InsetMathScript::down()
{
	if (nargs() == 3)
		return cell(2);
	BOOST_ASSERT(nargs() > 1);
	return cell(1);
}


MathData const & InsetMathScript::up() const
{
	BOOST_ASSERT(nargs() > 1);
	return cell(1);
}


MathData & InsetMathScript::up()
{
	BOOST_ASSERT(nargs() > 1);
	return cell(1);
}


void InsetMathScript::ensure(bool up)
{
	if (nargs() == 1) {
		// just nucleus so far
		cells_.push_back(MathData());
		cell_1_is_up_ = up;
	} else if (nargs() == 2 && !has(up)) {
		if (up) {
			cells_.push_back(cell(1));
			cell(1).clear();
		} else {
			cells_.push_back(MathData());
		}
	}
}


MathData const & InsetMathScript::nuc() const
{
	return cell(0);
}


MathData & InsetMathScript::nuc()
{
	return cell(0);
}


namespace {

bool isAlphaSymbol(MathAtom const & at)
{
	if (at->asCharInset() ||
			(at->asSymbolInset() &&
			 at->asSymbolInset()->isOrdAlpha()))
		return true;

	if (at->asFontInset()) {
		MathData const & ar = at->asFontInset()->cell(0);
		for (size_t i = 0; i < ar.size(); ++i) {
			if (!(ar[i]->asCharInset() ||
					(ar[i]->asSymbolInset() &&
					 ar[i]->asSymbolInset()->isOrdAlpha())))
				return false;
		}
		return true;
	}
	return false;
}

} // namespace anon


int InsetMathScript::dy01(int asc, int des, int what) const
{
	int dasc = 0;
	int slevel = 0;
	bool isCharBox = nuc().size() ? isAlphaSymbol(nuc().back()) : false;
	if (hasDown()) {
		dasc = down().ascent();
		slevel = nuc().slevel();
		int ascdrop = dasc - slevel;
		int desdrop = isCharBox ? 0 : des + nuc().sshift();
		int mindes = nuc().mindes();
		des = max(desdrop, ascdrop);
		des = max(mindes, des);
	}
	if (hasUp()) {
		int minasc = nuc().minasc();
		int ascdrop = isCharBox ? 0 : asc - up().mindes();
		int udes = up().descent();
		asc = udes + nuc().sshift();
		asc = max(ascdrop, asc);
		asc = max(minasc, asc);
		if (hasDown()) {
			int del = asc - udes - dasc;
			if (del + des <= 2) {
				int newdes = 2 - del;
				del = slevel - asc + udes;
				if (del > 0) {
					asc += del;
					newdes -= del;
				}
				des = max(des, newdes);
			}
		}
	}
	return what ? asc : des;
}


int InsetMathScript::dy0() const
{
	int nd = ndes();
	if (!hasDown())
		return nd;
	int des = down().ascent();
	if (hasLimits())
		des += nd + 2;
	else {
		int na = nasc();
		des = dy01(na, nd, 0);
	}
	return des;
}


int InsetMathScript::dy1() const
{
	int na = nasc();
	if (!hasUp())
		return na;
	int asc = up().descent();
	if (hasLimits())
		asc += na + 2;
	else {
		int nd = ndes();
		asc = dy01(na, nd, 1);
	}
	asc = max(asc, 5);
	return asc;
}


int InsetMathScript::dx0() const
{
	BOOST_ASSERT(hasDown());
	return hasLimits() ? (dim_.wid - down().width()) / 2 : nwid();
}


int InsetMathScript::dx1() const
{
	BOOST_ASSERT(hasUp());
	return hasLimits() ? (dim_.wid - up().width()) / 2 : nwid() + nker();
}


int InsetMathScript::dxx() const
{
	return hasLimits() ? (dim_.wid - nwid()) / 2  :  0;
}


int InsetMathScript::nwid() const
{
	return nuc().size() ? nuc().width() : 2;
}


int InsetMathScript::nasc() const
{
	return nuc().size() ? nuc().ascent() : 5;
}


int InsetMathScript::ndes() const
{
	return nuc().size() ? nuc().descent() : 0;
}


int InsetMathScript::nker() const
{
	if (nuc().size()) {
		int kerning = nuc().kerning();
		return kerning > 0 ? kerning : 0;
	}
	return 0;
}


bool InsetMathScript::metrics(MetricsInfo & mi, Dimension & dim) const
{
	cell(0).metrics(mi);
	ScriptChanger dummy(mi.base);
	if (nargs() > 1)
		cell(1).metrics(mi);
	if (nargs() > 2)
		cell(2).metrics(mi);
	dim.wid = 0;
	if (hasLimits()) {
		dim.wid = nwid();
		if (hasUp())
			dim.wid = max(dim.wid, up().width());
		if (hasDown())
			dim.wid = max(dim.wid, down().width());
	} else {
		if (hasUp())
			dim.wid = max(dim.wid, nker() + up().width());
		if (hasDown())
			dim.wid = max(dim.wid, down().width());
		dim.wid += nwid();
	}
	int na = nasc();
	if (hasUp()) {
		int asc = dy1() + up().ascent();
		dim.asc = max(na, asc);
	} else
		dim.asc = na;
	int nd = ndes();
	if (hasDown()) {
		int des = dy0() + down().descent();
		dim.des = max(nd, des);
	} else
		dim.des = nd;
	metricsMarkers(dim);
	if (dim_ == dim)
		return false;
	dim_ = dim;
	return true;
}


void InsetMathScript::draw(PainterInfo & pi, int x, int y) const
{
	if (nuc().size())
		nuc().draw(pi, x + dxx(), y);
	else {
		nuc().setXY(*pi.base.bv, x + dxx(), y);
		if (editing(pi.base.bv))
                        pi.draw(x + dxx(), y, char_type('.'));
	}
	ScriptChanger dummy(pi.base);
	if (hasUp())
		up().draw(pi, x + dx1(), y - dy1());
	if (hasDown())
		down().draw(pi, x + dx0(), y + dy0());
	drawMarkers(pi, x, y);
}


void InsetMathScript::metricsT(TextMetricsInfo const & mi, Dimension & dim) const
{
	if (hasUp())
		up().metricsT(mi, dim);
	if (hasDown())
		down().metricsT(mi, dim);
	nuc().metricsT(mi, dim);
}


void InsetMathScript::drawT(TextPainter & pain, int x, int y) const
{
	if (nuc().size())
		nuc().drawT(pain, x + dxx(), y);
	if (hasUp())
		up().drawT(pain, x + dx1(), y - dy1());
	if (hasDown())
		down().drawT(pain, x + dx0(), y + dy0());
}



bool InsetMathScript::hasLimits() const
{
	// obvious cases
	if (limits_ == 1)
		return true;
	if (limits_ == -1)
		return false;

	// we can only display limits if the nucleus wants some
	if (!nuc().size())
		return false;
	if (!nuc().back()->isScriptable())
		return false;

	if (nuc().back()->asSymbolInset()) {
		// \intop is an alias for \int\limits, \ointop == \oint\limits
		if (nuc().back()->asSymbolInset()->name().find(from_ascii("intop")) != string::npos)
			return true;
		// per default \int has limits beside the \int even in displayed formulas
		if (nuc().back()->asSymbolInset()->name().find(from_ascii("int")) != string::npos)
			return false;
	}

	// assume "real" limits for everything else
	return true;
}


void InsetMathScript::removeScript(bool up)
{
	if (nargs() == 2) {
		if (up == cell_1_is_up_)
			cells_.pop_back();
	} else if (nargs() == 3) {
		if (up == true) {
			swap(cells_[1], cells_[2]);
			cell_1_is_up_ = false;
		} else {
			cell_1_is_up_ = true;
		}
		cells_.pop_back();
	}
}


bool InsetMathScript::has(bool up) const
{
	return idxOfScript(up);
}


bool InsetMathScript::hasUp() const
{
	//lyxerr << "1up: " << bool(cell_1_is_up_) << endl;
	//lyxerr << "hasUp: " << bool(idxOfScript(true)) << endl;
	return idxOfScript(true);
}


bool InsetMathScript::hasDown() const
{
	//lyxerr << "1up: " << bool(cell_1_is_up_) << endl;
	//lyxerr << "hasDown: " << bool(idxOfScript(false)) << endl;
	return idxOfScript(false);
}


InsetBase::idx_type InsetMathScript::idxOfScript(bool up) const
{
	if (nargs() == 1)
		return 0;
	if (nargs() == 2)
		return (cell_1_is_up_ == up) ? 1 : 0;
	if (nargs() == 3)
		return up ? 1 : 2;
	BOOST_ASSERT(false);
	// Silence compiler
	return 0;
}


bool InsetMathScript::idxRight(Cursor &) const
{
	return false;
}


bool InsetMathScript::idxLeft(Cursor &) const
{
	return false;
}


bool InsetMathScript::idxUpDown(Cursor & cur, bool up) const
{
	// in nucleus?
	if (cur.idx() == 0) {
		// don't go up/down if there is no cell in this direction
		if (!has(up))
			return false;
		// go up/down only if in the last position
		// or in the first position of something with displayed limits
		if (cur.pos() == cur.lastpos() || (cur.pos() == 0 && hasLimits())) {
			cur.idx() = idxOfScript(up);
			cur.pos() = 0;
			return true;
		}
		return false;
	}

	// Are we 'up'?
	if (has(up) && cur.idx() == idxOfScript(true)) {
		// can't go further up
		if (up)
			return false;
		// otherwise go to last position in the nucleus
		cur.idx() = 0;
		cur.pos() = cur.lastpos();
		return true;
	}

	// Are we 'down'?
	if (has(up) && cur.idx() == idxOfScript(false)) {
		// can't go further down
		if (!up)
			return false;
		// otherwise go to last position in the nucleus
		cur.idx() = 0;
		cur.pos() = cur.lastpos();
		return true;
	}

	return false;
}


void InsetMathScript::write(WriteStream & os) const
{
	if (nuc().size()) {
		os << nuc();
		//if (nuc().back()->takesLimits()) {
			if (limits_ == -1)
				os << "\\nolimits ";
			if (limits_ == 1)
				os << "\\limits ";
		//}
	} else {
		if (os.firstitem())
			LYXERR(Debug::MATHED) << "suppressing {} when writing"
					      << endl;
		else
			os << "{}";
	}

	if (hasDown() /*&& down().size()*/)
		os << "_{" << down() << '}';

	if (hasUp() /*&& up().size()*/)
		os << "^{" << up() << '}';

	if (lock_ && !os.latex())
		os << "\\lyxlock ";
}


void InsetMathScript::normalize(NormalStream & os) const
{
	bool d = hasDown() && down().size();
	bool u = hasUp() && up().size();

	if (u && d)
		os << "[subsup ";
	else if (u)
		os << "[sup ";
	else if (d)
		os << "[sub ";

	if (nuc().size())
		os << nuc() << ' ';
	else
		os << "[par]";

	if (u && d)
		os << down() << ' ' << up() << ']';
	else if (d)
		os << down() << ']';
	else if (u)
		os << up() << ']';
}


void InsetMathScript::maple(MapleStream & os) const
{
	if (nuc().size())
		os << nuc();
	if (hasDown() && down().size())
		os << '[' << down() << ']';
	if (hasUp() && up().size())
		os << "^(" << up() << ')';
}


void InsetMathScript::mathematica(MathematicaStream & os) const
{
	bool d = hasDown() && down().size();
	bool u = hasUp() && up().size();

	if (nuc().size()) {
		if (d)
			os << "Subscript[" << nuc();
		else
			os << nuc();
	}

	if (u)
		os << "^(" << up() << ')';

	if (nuc().size()) {
		if (d)
			os << ',' << down() << ']';
	}
}


void InsetMathScript::mathmlize(MathStream & os) const
{
	bool d = hasDown() && down().size();
	bool u = hasUp() && up().size();

	if (u && d)
		os << MTag("msubsup");
	else if (u)
		os << MTag("msup");
	else if (d)
		os << MTag("msub");

	if (nuc().size())
		os << nuc();
	else
		os << "<mrow/>";

	if (u && d)
		os << down() << up() << ETag("msubsup");
	else if (u)
		os << up() << ETag("msup");
	else if (d)
		os << down() << ETag("msub");
}


void InsetMathScript::octave(OctaveStream & os) const
{
	if (nuc().size())
		os << nuc();
	if (hasDown() && down().size())
		os << '[' << down() << ']';
	if (hasUp() && up().size())
		os << "^(" << up() << ')';
}


void InsetMathScript::infoize(odocstream & os) const
{
	os << "Scripts";
}


void InsetMathScript::infoize2(odocstream & os) const
{
	if (limits_)
		os << (limits_ == 1 ? ", Displayed limits" : ", Inlined limits");
}


bool InsetMathScript::notifyCursorLeaves(Cursor & cur)
{
	InsetMathNest::notifyCursorLeaves(cur);

	//lyxerr << "InsetMathScript::notifyCursorLeaves: 1 " << cur << endl;

	// remove empty scripts if possible
	if (nargs() > 2) {
		// Case of two scripts. In this case, 1 = super, 2 = sub
		if (cur.idx() == 2 && cell(2).empty()) {
			// must be a subscript...
			recordUndoInset(cur);
			removeScript(false);
			return true;
		} else if (cur.idx() == 1 && cell(1).empty()) {
			// must be a superscript...
			recordUndoInset(cur);
			removeScript(true);
			return true;
		}
	} else if (nargs() > 1 && cur.idx() == 1 && cell(1).empty()) {
		// could be either subscript or super script
		recordUndoInset(cur);
		removeScript(cell_1_is_up_);
		// Let the script inset commit suicide. This is
		// modelled on Cursor.pullArg(), but tries not to
		// invoke notifyCursorLeaves again and does not touch
		// cur (since the top slice will be deleted
		// afterwards))
		MathData ar = cell(0);
		Cursor tmpcur = cur;
		tmpcur.pop();
		tmpcur.cell().erase(tmpcur.pos());
		tmpcur.cell().insert(tmpcur.pos(), ar);
		return true;
	}

	//lyxerr << "InsetMathScript::notifyCursorLeaves: 2 " << cur << endl;
	return false;
}


void InsetMathScript::doDispatch(Cursor & cur, FuncRequest & cmd)
{
	//lyxerr << "InsetMathScript: request: " << cmd << std::endl;

	if (cmd.action == LFUN_MATH_LIMITS) {
		if (!cmd.argument().empty()) {
			if (cmd.argument() == "limits")
				limits_ = 1;
			else if (cmd.argument() == "nolimits")
				limits_ = -1;
			else
				limits_ = 0;
		} else if (limits_ == 0)
			limits_ = hasLimits() ? -1 : 1;
		else
			limits_ = 0;
		return;
	}

	InsetMathNest::doDispatch(cur, cmd);
}


} // namespace lyx
