// -*- C++ -*-
/**
 * \file SpellBase.h
 * Copyright 2002 the LyX Team
 * Read the file COPYING
 *
 * \author unknown
 */

#ifndef SPELL_BASE_H
#define SPELL_BASE_H

#include "LString.h" // can't forward declare...

class BufferParams;
class WordLangTuple;

/**
 * Base class of all spell checker implementations.
 */
class SpellBase {
public:

	/// the result from checking a single word
	enum Result  {
		OK = 1, //< word is correct
		ROOT, //< root of given word was found
		COMPOUNDWORD, //< word found through compound formation
		UNKNOWN, //< word not found
		MISSED, //< not found, with suggestions
		IGNORE //< number of other ignored "word"
	};

	virtual ~SpellBase() {}

	/// return true if the spellchecker instance still exists
	virtual bool alive() = 0;

	/// check the given word of the given lang code and return the result
	virtual enum Result check(WordLangTuple const &) = 0;

	/// insert the given word into the personal dictionary
	virtual void insert(WordLangTuple const &) = 0;

	/// accept the given word temporarily
	virtual void accept(WordLangTuple const &) = 0;

	/// return the next near miss after a MISSED result
	virtual string const nextMiss() = 0;

	/// give an error message on messy exit
	virtual string const error() = 0;

};

#endif // SPELL_BASE_H
