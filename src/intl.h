// -*- C++ -*-
/**
 * \file intl.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author unknown
 * \author Lars Gullik Bj�nnes
 * \author Angus Leeming
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef INTL_H
#define INTL_H

#include "LString.h"
#include "trans_mgr.h"


/**
 * This class is used for managing keymaps
 * for composing characters in LyX.
 */
class Intl {
public:
	/// which keymap is currently used ?
	enum Keymap {
		PRIMARY,
		SECONDARY
	};

	Intl();

	/// {en/dis}able the keymap
	void KeyMapOn(bool on);

	/// set the primary language keymap
	void KeyMapPrim();

	/// set the secondary language keymap
	void KeyMapSec();

	/// turn on/off key mappings, status in keymapon
	void ToggleKeyMap();

	/// initialize key mapper
	void InitKeyMapper(bool on);

	// Get the Translation Manager
	TransManager & getTransManager();

	/// using primary or secondary keymap ?
	Keymap keymap;

private:
	/// is key mapping enabled ?
	bool keymapon;
	/// the primary language keymap
	string & prim_lang;
	/// the secondary language keymap
	string & sec_lang;
	/// the translation manager
	TransManager trans;
};


inline
TransManager & Intl::getTransManager()
{
	return trans;
}

#endif /* INTL_H */
