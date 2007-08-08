/**
 * \file Counters.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Martin Vermeer
 * \author Andr� P�nitz
 * \author Richard Heck (roman numerals)
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Counters.h"
#include "debug.h"

#include "support/lstrings.h"
#include "support/convert.h"

#include <boost/assert.hpp>

#include <sstream>

using std::endl;
using std::ostringstream;
using std::string;


namespace lyx {


Counter::Counter()
{
	reset();
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


void Counter::setMaster(docstring const & m)
{
	master_ = m;
}


void Counters::newCounter(docstring const & newc)
{
	// First check if newc already exist
	CounterList::iterator const cit = counterList.find(newc);
	// if already exist give warning and return
	if (cit != counterList.end()) {
		lyxerr << "New counter already exists: "
		       << to_utf8(newc)
		       << endl;
		return;
	}
	counterList[newc];
}


void Counters::newCounter(docstring const & newc,
			  docstring const & masterc)
{
	// First check if newc already exists
	CounterList::iterator const cit = counterList.find(newc);
	// if already existant give warning and return
	if (cit != counterList.end()) {
		lyxerr << "New counter already exists: "
		       << to_utf8(newc)
		       << endl;
		return;
	}
	// then check if masterc exists
	CounterList::iterator const it = counterList.find(masterc);
	// if not give warning and return
	if (it == counterList.end()) {
		lyxerr << "Master counter does not exist: "
		       << to_utf8(masterc)
		       << endl;
		return;
	}

	counterList[newc].setMaster(masterc);
}


void Counters::set(docstring const & ctr, int const val)
{
	CounterList::iterator const it = counterList.find(ctr);
	if (it == counterList.end()) {
		lyxerr << "set: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}
	it->second.set(val);
}


void Counters::addto(docstring const & ctr, int const val)
{
	CounterList::iterator const it = counterList.find(ctr);
	if (it == counterList.end()) {
		lyxerr << "addto: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}
	it->second.addto(val);
}


int Counters::value(docstring const & ctr) const
{
	CounterList::const_iterator const cit = counterList.find(ctr);
	if (cit == counterList.end()) {
		lyxerr << "value: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return 0;
	}
	return cit->second.value();
}


void Counters::step(docstring const & ctr)
{
	CounterList::iterator it = counterList.find(ctr);
	if (it == counterList.end()) {
		lyxerr << "step: Counter does not exist: "
		       << to_utf8(ctr) << endl;
		return;
	}

	it->second.step();
	it = counterList.begin();
	CounterList::iterator const end = counterList.end();
	for (; it != end; ++it) {
		if (it->second.master() == ctr) {
			it->second.reset();
		}
	}
}


void Counters::reset()
{
	appendix_ = false;
	CounterList::iterator it = counterList.begin();
	CounterList::iterator const end = counterList.end();
	for (; it != end; ++it) {
		it->second.reset();
	}
}


void Counters::reset(docstring const & match)
{
	BOOST_ASSERT(!match.empty());

	CounterList::iterator it = counterList.begin();
	CounterList::iterator end = counterList.end();
	for (; it != end; ++it) {
		if (it->first.find(match) != string::npos)
			it->second.reset();
	}
}


void Counters::copy(Counters & from, Counters & to, docstring const & match)
{
	CounterList::iterator it = counterList.begin();
	CounterList::iterator end = counterList.end();
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
	return support::lowercase(romanCounter(n));
}

} // namespace anon


docstring Counters::labelItem(docstring const & ctr,
			      docstring const & numbertype)
{
	CounterList::const_iterator const cit = counterList.find(ctr);
	if (cit == counterList.end()) {
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


docstring Counters::counterLabel(docstring const & format)
{
	docstring label = format;
	while (true) {
#ifdef WITH_WARNINGS
#warning Using boost::regex or boost::spirit would make this code a lot simpler... (Lgb)
#endif

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
		docstring const rep = labelItem(counter, numbertype);
		label = docstring(label, 0, i) + rep
			+ docstring(label, k + 1, docstring::npos);
		//lyxerr << "  : " << " (" << counter  << ","
		//	<< numbertype << ") -> " << label << endl;
	}
	//lyxerr << "counterLabel: " << format  << " -> "	<< label << endl;
	return label;
}


} // namespace lyx
