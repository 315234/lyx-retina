/**
 * \file EnchantChecker.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Caolán McNamara
 * \author Jürgen Spitzmüller
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include <enchant++.h>

#include "EnchantChecker.h"
#include "LyXRC.h"
#include "WordLangTuple.h"

#include "support/lassert.h"
#include "support/debug.h"
#include "support/docstring_list.h"

#include <map>
#include <string>

using namespace std;

namespace lyx {

namespace {

struct Speller {
	enchant::Dict * speller;
};

typedef map<string, Speller> Spellers;
  
} // anon namespace

struct EnchantChecker::Private
{
	Private() {}

	~Private();

	/// add a speller of the given language
	enchant::Dict * addSpeller(string const & lang);

	///
	enchant::Dict * speller(string const & lang);

	/// the spellers
	Spellers spellers_;
};


EnchantChecker::Private::~Private()
{
	Spellers::iterator it = spellers_.begin();
	Spellers::iterator end = spellers_.end();

	for (; it != end; ++it) {
		delete it->second.speller;
	}
}


enchant::Dict * EnchantChecker::Private::addSpeller(string const & lang)
{
	enchant::Broker * instance = enchant::Broker::instance();

	if (!instance->dict_exists(lang))
		// FIXME error handling?
		return 0;

	enchant::Dict * dict = instance->request_dict(lang);

	if (dict) {
		Speller m;
		m.speller = dict;
		spellers_[lang] = m;
		return m.speller;
	}
	// FIXME error handling?
	return 0;
}


enchant::Dict * EnchantChecker::Private::speller(string const & lang)
{
	Spellers::iterator it = spellers_.find(lang);
	if (it != spellers_.end())
		return it->second.speller;
	
	return addSpeller(lang);
}


EnchantChecker::EnchantChecker(): d(new Private)
{
}


EnchantChecker::~EnchantChecker()
{
	delete d;
}


SpellChecker::Result EnchantChecker::check(WordLangTuple const & word)
{
	enchant::Dict * m = d->speller(word.lang()->code());

	if (!m || word.word().empty())
		return WORD_OK;

	string utf8word = to_utf8(word.word());

	if (m->check(utf8word))
		return WORD_OK;

	return UNKNOWN_WORD;
}


void EnchantChecker::insert(WordLangTuple const & word)
{
	Spellers::iterator it = d->spellers_.find(word.lang()->code());
	if (it != d->spellers_.end())
		it->second.speller->add(to_utf8(word.word()));
}


void EnchantChecker::accept(WordLangTuple const & word)
{
	Spellers::iterator it = d->spellers_.find(word.lang()->code());
	if (it != d->spellers_.end())
		it->second.speller->add_to_session(to_utf8(word.word()));
}


void EnchantChecker::suggest(WordLangTuple const & wl,
	docstring_list & suggestions)
{
	suggestions.clear();
	enchant::Dict * m = d->speller(wl.lang()->code());

	if (!m)
		return;

	string utf8word = to_utf8(wl.word());

	vector<string> suggs = m->suggest(utf8word);
	vector<string>::const_iterator it = suggs.begin();
	
	for (; it != suggs.end(); ++it)
		suggestions.push_back(from_utf8(*it));
}


bool EnchantChecker::hasDictionary(Language const * lang) const
{
	if (!lang)
		return false;
	enchant::Broker * instance = enchant::Broker::instance();
	return (instance->dict_exists(lang->code()));
}


docstring const EnchantChecker::error()
{
	return docstring();
}


} // namespace lyx
