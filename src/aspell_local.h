// -*- C++ -*-
/**
 * \file aspell_local.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Kevin Atkinson
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_ASPELL_H
#define LYX_ASPELL_H

#include <map>

#include "SpellBase.h"

class AspellSpeller;
class AspellStringEnumeration;
class AspellCanHaveError;
class AspellConfig;

class BufferParams;


class ASpell : public SpellBase {
public:
	/**
	 * Initialise the spellchecker with the given buffer params and language.
	 */
	ASpell(BufferParams const & params, std::string const & lang);

	virtual ~ASpell();

	/**
	 * return true if the spellchecker instance still exists
	 * Always true for aspell, since there is no separate process
	 */
	virtual bool alive() { return true; }

	/// check the given word and return the result
	virtual enum Result check(WordLangTuple const &);

	/// insert the given word into the personal dictionary
	virtual void insert(WordLangTuple const &);

	/// accept the given word temporarily
	virtual void accept(WordLangTuple const &);

	/// return the next near miss after a MISSED result
	virtual std::string const nextMiss();

	/// give an error message on messy exit
	virtual std::string const error();

private:
	/// add a speller of the given language
	void addSpeller(std::string const & lang);

	struct Speller {
		AspellSpeller * speller;
		AspellConfig * config;
	};

	typedef std::map<std::string, struct Speller> Spellers;

	/// the spellers
	Spellers spellers_;

	/// FIXME
	AspellStringEnumeration * els;
	/// FIXME
	AspellCanHaveError * spell_error_object;
};

#endif // ASPELL_H
