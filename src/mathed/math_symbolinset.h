// -*- C++ -*-
#ifndef MATH_SYMBOLINSET_H
#define MATH_SYMBOLINSET_H

#ifdef __GNUG__
#pragma interface
#endif

#include "math_diminset.h"

struct latexkeys;

// "normal" symbols that don't take limits and don't grow in displayed
// formulae

class MathSymbolInset : public MathDimInset {
public:
	///
	explicit MathSymbolInset(latexkeys const *);
	///
	explicit MathSymbolInset(char const *);
	///
	explicit MathSymbolInset(string const &);
	///
	MathInset * clone() const;
	///
	void metrics(MathMetricsInfo & st) const;
	///
	void draw(MathPainterInfo &, int x, int y) const;
	///
	bool isRelOp() const;
	/// do we take scripts?
	bool isScriptable() const;
	/// do we take \limits or \nolimits?
	bool takesLimits() const;
	/// identifies SymbolInset as such
	MathSymbolInset const * asSymbolInset() const { return this; }
	/// the LaTeX name of the symbol (without the backslash)
	string name() const;
	///
	bool match(MathAtom const &) const;
	/// request "external features"
	void validate(LaTeXFeatures & features) const;

	///
	void normalize(NormalStream &) const;
	///
	void maplize(MapleStream &) const;
	///
	void maximize(MaximaStream &) const;
	///
	void mathematicize(MathematicaStream &) const;
	///
	void mathmlize(MathMLStream &) const;
	///
	void octavize(OctaveStream &) const;
	///
	void write(WriteStream & os) const;
	///
	void infoize2(std::ostream & os) const;

private:
	///
	latexkeys const * sym_;
	///
	mutable int h_;
	///
	mutable bool scriptable_;
};
#endif
