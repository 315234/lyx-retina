// -*- C++ -*-
/**
 * \file XLyXKeySym.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Asger and Juergen
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef XLYXKEYSYM_H
#define XLYXKEYSYM_H


#include "LString.h"
#include <X11/Xlib.h>
#include "frontends/LyXKeySym.h"

/**
 * This is the XForms (X11) version of LyXKeySym.
 */
class XLyXKeySym : public LyXKeySym {
public:
	XLyXKeySym();

	/// X11 specific initialization with an X11 KeySym
	void initFromKeySym(KeySym);

	virtual void init(string const & symbolname);

	virtual ~XLyXKeySym() {}

	/// Is this a valid key?
	virtual bool isOK() const;

	/// Is this a modifier key only?
	virtual bool isModifier() const;

	virtual string getSymbolName() const;

	/**
	 * Return the value of the keysym into the local ISO encoding.
	 * This converts the LyXKeySym to a 8-bit encoded character.
	 * This relies on user to use the right encoding.
	 */
	virtual char getISOEncoded(string const & encoding) const;

	///
	unsigned int keysym() const {
		return keysym_;
	}
private:
	unsigned int keysym_;
};

#endif // XLYXKEYSYM_H
