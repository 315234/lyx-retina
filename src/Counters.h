// -*- C++ -*-
/**
 * \file Counters.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 * \author Martin Vermeer
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef COUNTERS_H
#define COUNTERS_H

#include "support/docstring.h"

#include <map>


namespace lyx {

/// This represents a single counter.
class Counter {
public:
	///
	Counter();
	///
	Counter(docstring const & mc, docstring const & ls, 
		docstring const & lsa);
	///
	void set(int v);
	///
	void addto(int v);
	///
	int value() const;
	///
	void step();
	///
	void reset();
	/// Returns the master counter of this counter.
	docstring const & master() const;
	/// Returns a LaTeX-like string to format the counter. 
	/* This is similar to what one gets in LaTeX when using
	 * "\the<counter>".
	 */
	docstring const & labelString() const;
	/// Returns a LaTeX-like string to format the counter in appendix.
	/* This is similar to what one gets in LaTeX when using
	 * "\the<counter>" in an appendix.
	 */
	docstring const & labelStringAppendix() const;
private:
	///
	int value_;
	/// contains master counter name.
	/* The master counter is the counter that, if stepped
	 * (incremented) zeroes this counter. E.g. "subsection"'s
	 * master is "section".
	 */
	docstring master_;
	// Contains a LaTeX-like string to format the counter.
	docstring labelstring_;
	// The same as labelstring_, but in appendices.
	docstring labelstringappendix_;
};


/// This is a class of (La)TeX type counters.
/// Every instantiation is an array of counters of type Counter.
class Counters {
public:
	/// Add a new counter to array.
	void newCounter(docstring const & newc);
	/// Add new counter having oldc as its master and ls as its label.
	void newCounter(docstring const & newc,
			docstring const & masterc,
			docstring const & ls,
			docstring const & lsa);
	/// Checks whether the given counter exists.
	bool hasCounter(docstring const & c) const;
	///
	void set(docstring const & ctr, int val);
	///
	void addto(docstring const & ctr, int val);
	///
	int value(docstring const & ctr) const;
	/// Increment by one counter named by arg, and zeroes slave
	/// counter(s) for which it is the master.
	/* Sub-slaves not zeroed! That happens at slave's first step
	 * 0->1. Seems to be sufficient.
	 */
	void step(docstring const & ctr);
	/// Reset all counters.
	void reset();
	/// Reset counters matched by match string.
	void reset(docstring const & match);
	/// Copy counters whose name matches match from the &from to
	/// the &to array of counters. Empty string matches all.
	void copy(Counters & from, Counters & to,
		  docstring const & match = docstring());
	/// returns the expanded string representation of the counter.
	docstring theCounter(docstring const & c);
	/// Replace om format all the LaTeX-like macros that depend on
	/// counters.
	docstring counterLabel(docstring const & format);
	/// Are we in apendix?
	bool appendix() const { return appendix_; };
	/// Set the state variable indicating whether we are in appendix.
	void appendix(bool a) { appendix_ = a; };
	/// Returns the current enclosing float.
	std::string const & current_float() const { return current_float_; }
	/// Sets the current enclosing float.
	void current_float(std::string const & f) { current_float_ = f; }
private:
	/// Returns the value of the counter according to the
	/// numbering scheme numbertype.
	/* Available numbering schemes are arabic (1, 2,...), roman
	 * (i, ii,...), Roman (I, II,...), alph (a, b,...), Alpha (A,
	 * B,...) and hebrew.
	 */
	docstring labelItem(docstring const & ctr,
			    docstring const & numbertype);
	/// Maps counter (layout) names to actual counters.
	typedef std::map<docstring, Counter> CounterList;
	/// Instantiate.
	CounterList counterList;
	/// Are we in an appendix?
	bool appendix_;
	/// The current enclosing float.
	std::string current_float_;
};


} // namespace lyx

#endif
