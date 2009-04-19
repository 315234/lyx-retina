/**
 * \file Counters.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Martin Vermeer
 * \author André Pönitz
 * \author Richard Heck (roman numerals)
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Counters.h"
#include "Lexer.h"

#include "support/convert.h"
#include "support/debug.h"
#include "support/lassert.h"
#include "support/lstrings.h"

#include <sstream>

using namespace std;
using namespace lyx::support;

namespace lyx {


Counter::Counter()
{
	reset();
}


Counter::Counter(docstring const & mc, docstring const & ls, 
		 docstring const & lsa)
	: master_(mc), labelstring_(ls), labelstringappendix_(lsa)
{
	reset();
}


bool Counter::read(Lexer & lex)
{
	enum {
		CT_WITHIN = 1,
		CT_LABELSTRING,
		CT_LABELSTRING_APPENDIX,
		CT_END
	};

	LexerKeyword counterTags[] = {
		{ "end", CT_END },
		{ "labelstring", CT_LABELSTRING },
		{ "labelstringappendix", CT_LABELSTRING_APPENDIX },
		{ "within", CT_WITHIN }
	};

	lex.pushTable(counterTags);

	bool getout = false;
	while (!getout && lex.isOK()) {
		int le = lex.lex();
		switch (le) {
			case Lexer::LEX_UNDEF:
				lex.printError("Unknown counter tag `$$Token'");
				continue;
			default: 
				break;
		}
		switch (le) {
			case CT_WITHIN:
				lex.next();
				master_ = lex.getDocString();
				if (master_ == "none")
					master_.erase();
				break;
			case CT_LABELSTRING:
				lex.next();
				labelstring_ = lex.getDocString();
				labelstringappendix_ = labelstring_;
				break;
			case CT_LABELSTRING_APPENDIX:
				lex.next();
				labelstringappendix_ = lex.getDocString();
				break;
			case CT_END:
				getout = true;
				break;
		}
	}

	// Here if have a full counter if getout == true
	if (!getout)
		LYXERR0("No End tag found for counter!");
	lex.popTable();
	return getout;
}

void Counter::set(int v)
{
	value_ = v;
}


void Counter::addto(int v)
{
	value_ += v;
}


int Counter::value() const
{
	return value_;
}


void Counter::step()
{
	++value_;
}


void Counter::reset()
{
	value_ = 0;
}


docstring const & Counter::master() const
{
	return master_;
}


docstring const & Counter::labelString(bool in_appendix) const
{
	return in_appendix ? labelstringappendix_ : labelstring_;
}


docstring const & Counter::flatLabelString(bool in_appendix) const
{
	return in_appendix ? flatlabelstringappendix_ : flatlabelstring_;
}


docstring const & Counter::setFlatLabelStrings(docstring const & fls,
					       docstring const & flsa)
{
	flatlabelstring_ = fls;
	flatlabelstringappendix_ = flsa;
}


void Counters::newCounter(docstring const & newc,
			  docstring const & masterc, 
			  docstring const & ls,
			  docstring const & lsa)
{
	if (!masterc.empty() && !hasCounter(masterc)) {
		lyxerr << "Master counter does not exist: "
		       << to_utf8(masterc)
		       << endl;
		return;
	}
	counterList_[newc] = Counter(masterc, ls, lsa);
}


bool Counters::hasCounter(docstring const & c) const
{
	return counterList_.find(c) != counterList_.end();
}


bool Counters::read(Lexer & lex, docstring const & name)
{
	if (hasCounter(name)) {
		LYXERR(Debug::TCLASS, "Reading existing counter " << to_utf8(name));
		return counterList_[name].read(lex);
	}
	LYXERR(Debug::TCLASS, "Reading new counter " << to_utf8(name));
	Counter cnt;
	bool success = cnt.read(lex);
	if (success)
		counterList_[name] = cnt;
	else
		LYXERR0("Error reading counter `" << name << "'!");
	return success;
}


void Counters::set(docstring const & ctr, int const val)
{
	CounterList::iterator const it = counterList_.find(ctr);
	if (it == counterList_.end()) {
		lyxerr << "set: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}
	it->second.set(val);
}


void Counters::addto(docstring const & ctr, int const val)
{
	CounterList::iterator const it = counterList_.find(ctr);
	if (it == counterList_.end()) {
		lyxerr << "addto: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}
	it->second.addto(val);
}


int Counters::value(docstring const & ctr) const
{
	CounterList::const_iterator const cit = counterList_.find(ctr);
	if (cit == counterList_.end()) {
		lyxerr << "value: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return 0;
	}
	return cit->second.value();
}


void Counters::step(docstring const & ctr)
{
	CounterList::iterator it = counterList_.find(ctr);
	if (it == counterList_.end()) {
		lyxerr << "step: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}

	it->second.step();
	it = counterList_.begin();
	CounterList::iterator const end = counterList_.end();
	for (; it != end; ++it) {
		if (it->second.master() == ctr) {
			it->second.reset();
		}
	}
}


void Counters::reset()
{
	appendix_ = false;
	subfloat_ = false;
	current_float_.erase();
	CounterList::iterator it = counterList_.begin();
	CounterList::iterator const end = counterList_.end();
	std::vector<docstring> callers;
	for (; it != end; ++it) {
		it->second.reset();
		// Compute the explicit counter labels without any
		// \thexxx strings, in order to avoid recursion.  
		// It only needs to be done when the textclass is
		// updated, but in practice the extra work is probably
		// not noticeable (JMarc)
		docstring const & fls = flattenLabelString(it->first, false, callers);
		docstring const & flsa = flattenLabelString(it->first, true, callers);
		it->second.setFlatLabelStrings(fls, flsa);
	}
}


void Counters::reset(docstring const & match)
{
	LASSERT(!match.empty(), /**/);

	CounterList::iterator it = counterList_.begin();
	CounterList::iterator end = counterList_.end();
	for (; it != end; ++it) {
		if (it->first.find(match) != string::npos)
			it->second.reset();
	}
}


void Counters::copy(Counters & from, Counters & to, docstring const & match)
{
	CounterList::iterator it = counterList_.begin();
	CounterList::iterator end = counterList_.end();
	for (; it != end; ++it) {
		if (it->first.find(match) != string::npos || match == "") {
			to.set(it->first, from.value(it->first));
		}
	}
}


namespace {

char loweralphaCounter(int const n)
{
	if (n < 1 || n > 26)
		return '?';
	return 'a' + n - 1;
}


char alphaCounter(int const n)
{
	if (n < 1 || n > 26)
		return '?';
	return 'A' + n - 1;
}


char hebrewCounter(int const n)
{
	static const char hebrew[22] = {
		'\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7', '\xe8',
		'\xe9', '\xeb', '\xec', '\xee', '\xf0', '\xf1', '\xf2', '\xf4', '\xf6',
		'\xf7', '\xf8', '\xf9', '\xfa'
	};

	if (n < 1 || n > 22)
		return '?';
	return hebrew[n - 1];
}



//On the special cases, see http://mathworld.wolfram.com/RomanNumerals.html
//and for a list of roman numerals up to and including 3999, see 
//http://www.research.att.com/~njas/sequences/a006968.txt. (Thanks to Joost
//for this info.)
docstring const romanCounter(int const n)
{
	static char const * const ones[9] = {
		"I",   "II",  "III", "IV", "V",
		"VI",  "VII", "VIII", "IX"
	};
	
	static char const * const tens[9] = {
		"X", "XX", "XXX", "XL", "L",
		"LX", "LXX", "LXXX", "XC"
	};
	
	static char const * const hunds[9] = {
		"C", "CC", "CCC", "CD", "D",
		"DC", "DCC", "DCCC", "CM"
	};
	
	if (n > 1000 || n < 1) 
		return from_ascii("??");
	
	int val = n;
	string roman;
	switch (n) {
	//special cases
	case 900: 
		roman = "CM";
		break;
	case 400:
		roman = "CD";
		break;
	default:
		if (val >= 100) {
			int hundreds = val / 100;
			roman = hunds[hundreds - 1];
			val = val % 100;
		}
		if (val >= 10) {
			switch (val) {
			//special case
			case 90:
				roman = roman + "XC";
				val = 0; //skip next
				break;
			default:
				int tensnum = val / 10;
				roman = roman + tens[tensnum - 1];
				val = val % 10;
			} // end switch
		} // end tens
		if (val > 0)
			roman = roman + ones[val -1];
	}
	return from_ascii(roman);
}


docstring const lowerromanCounter(int const n)
{
	return lowercase(romanCounter(n));
}

} // namespace anon


docstring Counters::labelItem(docstring const & ctr,
			      docstring const & numbertype) const
{
	CounterList::const_iterator const cit = counterList_.find(ctr);
	if (cit == counterList_.end()) {
		lyxerr << "Counter "
		       << to_utf8(ctr)
		       << " does not exist." << endl;
		return docstring();
	}

	int val = cit->second.value();

	if (numbertype == "hebrew")
		return docstring(1, hebrewCounter(val));

	if (numbertype == "alph")
		return docstring(1, loweralphaCounter(val));

	if (numbertype == "Alph")
		return docstring(1, alphaCounter(val));

	if (numbertype == "roman")
		return lowerromanCounter(val);

	if (numbertype == "Roman")
		return romanCounter(val);

	return convert<docstring>(val);
}


docstring Counters::theCounter(docstring const & counter) const
{
	CounterList::const_iterator it = counterList_.find(counter); 
	if (it == counterList_.end())
		return from_ascii("??");
	return counterLabel(it->second.flatLabelString(appendix()));
}


docstring Counters::flattenLabelString(docstring const & counter, bool in_appendix, 
				       vector<docstring> & callers) const
{
	docstring label;

	if (find(callers.begin(), callers.end(), counter) != callers.end()) {
		// recursion detected
		lyxerr << "Warning: Recursion in label for counter `"
		       << counter << "' detected"
		       << endl;
		return from_ascii("??");
	}
		
	CounterList::const_iterator it = counterList_.find(counter); 
	if (it == counterList_.end())
		return from_ascii("??");
	Counter const & c = it->second;

	docstring ls = c.labelString(in_appendix);

	callers.push_back(counter);
	if (ls.empty()) {
		if (!c.master().empty())
			ls = flattenLabelString(c.master(), in_appendix, callers) 
				+ from_ascii(".");
		callers.pop_back();
		return ls + from_ascii("\\arabic{") + counter + "}";
	}

	while (true) {
		//lyxerr << "ls=" << to_utf8(ls) << endl;
		size_t const i = ls.find(from_ascii("\\the"), 0);
		if (i == docstring::npos)
			break;
		size_t const j = i + 4;
		size_t k = j;
		while (k < ls.size() && lowercase(ls[k]) >= 'a' 
		       && lowercase(ls[k]) <= 'z')
			++k;
		docstring const & newc = ls.substr(j, k - j);
		docstring const & repl = flattenLabelString(newc, in_appendix, callers);
		ls.replace(i, k - j + 4, repl);
	}
	callers.pop_back();

	return ls;
}


docstring Counters::counterLabel(docstring const & format) const
{
	docstring label = format;

	// FIXME: Using regexps would be better, but we compile boost without
	// wide regexps currently.
	while (true) {
		//lyxerr << "label=" << to_utf8(label) << endl;
		size_t const i = label.find(from_ascii("\\the"), 0);
		if (i == docstring::npos)
			break;
		size_t const j = i + 4;
		size_t k = j;
		while (k < label.size() && lowercase(label[k]) >= 'a' 
		       && lowercase(label[k]) <= 'z')
			++k;
		docstring const & newc = label.substr(j, k - j);
		docstring const & repl = theCounter(newc);
		label.replace(i, k - j + 4, repl);
	}
	while (true) {
		//lyxerr << "label=" << to_utf8(label) << endl;

		size_t const i = label.find('\\', 0);
		if (i == docstring::npos)
			break;
		size_t const j = label.find('{', i + 1);
		if (j == docstring::npos)
			break;
		size_t const k = label.find('}', j + 1);
		if (k == docstring::npos)
			break;
		docstring const numbertype(label, i + 1, j - i - 1);
		docstring const counter(label, j + 1, k - j - 1);
		docstring const & rep = labelItem(counter, numbertype);
		label = docstring(label, 0, i) + rep
			+ docstring(label, k + 1, docstring::npos);
	}
	//lyxerr << "DONE! label=" << to_utf8(label) << endl;
	return label;
}


} // namespace lyx
