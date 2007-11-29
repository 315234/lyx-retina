/**
 * \file InsetFormulaMacro.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Alejandro Aguilar Sierra
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "InsetFormulaMacro.h"
#include "MacroTable.h"
#include "MathMacroTemplate.h"

#include "BufferView.h"
#include "Cursor.h"
#include "support/debug.h"
#include "support/gettext.h"
#include "Lexer.h"
#include "OutputParams.h"

#include "frontends/FontMetrics.h"
#include "frontends/Painter.h"

#include "support/lstrings.h"

#include <sstream>

using std::string;
using std::ostream;
using std::endl;

namespace lyx {

using support::bformat;


InsetFormulaMacro::InsetFormulaMacro()
	: InsetMathNest(2), name_("unknownA")
{}


InsetFormulaMacro::InsetFormulaMacro
		(docstring const & name, int nargs, docstring const & type)
	: InsetMathNest(2), name_(name)
{
	MathMacroTable::create(MathAtom(new MathMacroTemplate(name, nargs, type)));
}


InsetFormulaMacro::InsetFormulaMacro(string const & s)
	: InsetMathNest(2), name_("unknownB")
{
	std::istringstream is(s);
	read(is);
}


Inset * InsetFormulaMacro::clone() const
{
	return new InsetFormulaMacro(*this);
}


void InsetFormulaMacro::write(Buffer const &, ostream & os) const
{
	os << "FormulaMacro\n";
	WriteStream wi(os, false, false);
	tmpl()->write(wi);
}


int InsetFormulaMacro::latex(Buffer const &, odocstream & os,
			     OutputParams const & runparams) const
{
	//lyxerr << "InsetFormulaMacro::latex" << endl;
	WriteStream wi(os, runparams.moving_arg, true);
	tmpl()->write(wi);
	return 2;
}


int InsetFormulaMacro::plaintext(Buffer const &, odocstream & os,
				 OutputParams const &) const
{
	odocstringstream oss;
	WriteStream wi(oss, false, true);
	tmpl()->write(wi);

	docstring const str = oss.str();
	os << str;
	return str.size();
}


int InsetFormulaMacro::docbook(Buffer const & buf, ostream & os,
			       OutputParams const & runparams) const
{
	return plaintext(buf, os, runparams);
}


void InsetFormulaMacro::read(Buffer const &, Lexer & lex)
{
	read(lex.getStream());
}


void InsetFormulaMacro::read(std::istream & is)
{
	auto_ptr<MathMacroTemplate> p(new MathMacroTemplate(is));
	name_ = p->name();
	MathMacroTable::create(MathAtom(p.release()));
}


string InsetFormulaMacro::prefix() const
{
	return to_utf8(bformat(_(" Macro: %1$s: "), lyx::from_utf8(name_)));
}


void InsetFormulaMacro::metrics(MetricsInfo & mi, Dimension & dim) const
{
	//lyxerr << "InsetFormulaMacro: " << this << " -- " << &tmpl() << endl;
	tmpl()->metrics(mi, dim);
	dim.asc += 5;
	dim.des += 5;
	dim.wid += 10 + theFontMetrics(mi.base.font).width(prefix());
	dim_ = dim;
}


void InsetFormulaMacro::draw(PainterInfo & p, int x, int y) const
{
	// label
	Font font = p.base.font;
	font.setColor(Color_math);

	PainterInfo pi(p.base.bv, p.pain);
	pi.base.style = LM_ST_TEXT;
	pi.base.font  = font;

	int const a = y - dim_.asc + 1;
	int const w = dim_.wid - 2;
	int const h = dim_.height() - 2;

	// Color_mathbg used to be "AntiqueWhite" but is "linen" now, too
	pi.pain.fillRectangle(x, a, w, h, Color_mathmacrobg);
	pi.pain.rectangle(x, a, w, h, Color_mathframe);

	// FIXME
#if 0
	Cursor & cur = p.base.bv->cursor();
	if (cur.isInside(this))
		cur.drawSelection(pi);
#endif

	pi.pain.text(x + 2, y, prefix(), font);

	// body
	tmpl()->draw(pi,
		x + theFontMetrics(p.base.font).width(prefix()) + 5,
		y);

	setPosCache(pi, x, y);
}


MathAtom & InsetFormulaMacro::tmpl() const
{
	return MathMacroTable::provide(name_);
}


} // namespace lyx
