// -*- C++ -*-
/**
 * \file trans.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TRANS_H
#define TRANS_H

#include <map>

#include "tex-accent.h"
#include "support/std_string.h"
#include "trans_decl.h"

class LyXLex;

class TransManager;

/**
  Trans: holds a .kmap file
  */
class Trans {
public:
	///
	Trans();
	///
	~Trans();

	///
	int Load(string const & language);
	///
	bool IsDefined() const;
	///
	string const & GetName() const;
	///
	string const process(char, TransManager &);
	///
	bool isAccentDefined(tex_accent, KmodInfo &) const;

private:
	///
	void AddDeadkey(tex_accent, string const &);
	///
	void FreeKeymap();
	///
	int Load(LyXLex &);
	///
	string const & Match(unsigned char c);
	///
	void InsertException(KmodException & exclist, char c,
			     string const & data, bool = false,
			     tex_accent = TEX_NOACCENT);
	///
	void FreeException(KmodException & exclist);

	///
	string name_;
	///
	std::map<int, string> keymap_;
	///
	std::map<int, KmodInfo> kmod_list_;
};


///
inline
string const & Trans::Match(unsigned char c)
{
	std::map<int, string>::iterator it = keymap_.find(c);
	if (it != keymap_.end()) {
		return it->second;
	}
	static string dummy;
	return dummy;
}

#endif // TRANS_H
