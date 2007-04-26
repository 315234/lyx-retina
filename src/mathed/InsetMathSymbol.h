// -*- C++ -*-
/**
 * \file InsetMathSymbol.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef MATH_SYMBOLINSET_H
#define MATH_SYMBOLINSET_H

#include "InsetMath.h"

#include "LyXFont.h"

namespace lyx {

class latexkeys;


/** "normal" symbols that don't take limits and don't grow in displayed
 *  formulae.
 */
class InsetMathSymbol : public InsetMath {
public:
	///
	explicit InsetMathSymbol(latexkeys const * l);
	///
	explicit InsetMathSymbol(char const * name);
	///
	explicit InsetMathSymbol(docstring const & name);
	///
	bool metrics(MetricsInfo & mi, Dimension & dim) const;
	///
	void draw(PainterInfo &, int x, int y) const;
	///
	int kerning() const { return kerning_; }

	///
	bool isRelOp() const;
	///
	bool isOrdAlpha() const;
	/// do we take scripts?
	bool isScriptable() const;
	/// do we take \limits or \nolimits?
	bool takesLimits() const;
	/// identifies SymbolInset as such
	InsetMathSymbol const * asSymbolInset() const { return this; }
	/// the LaTeX name of the symbol (without the backslash)
	docstring name() const;
	/// request "external features"
	void validate(LaTeXFeatures & features) const;

	///
	void normalize(NormalStream &) const;
	///
	void maple(MapleStream &) const;
	///
	void maxima(MaximaStream &) const;
	///
	void mathematica(MathematicaStream &) const;
	///
	void mathmlize(MathStream &) const;
	///
	void octave(OctaveStream &) const;
	///
	void write(WriteStream & os) const;
	///
	void infoize2(odocstream & os) const;

private:
	virtual std::auto_ptr<InsetBase> doClone() const;
	///
	latexkeys const * sym_;
	///
	mutable int h_;
	/// cached superscript kerning
	mutable int kerning_;
	///
	mutable bool scriptable_;
	///
	mutable LyXFont font_cache_;
};

} // namespace lyx

#endif
