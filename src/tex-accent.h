// -*- C++ -*-
/**
 * \file tex-accent.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TEX_ACCENT
#define TEX_ACCENT

#include "lfuns.h"

/* the names used by TeX and XWindows for deadkeys/accents are not the same
   so here follows a table to clearify the differences. Please correct this
   if I got it wrong

   |------------------|------------------|------------------|--------------|
   |      TeX         |     XWindows     |   \bind/LFUN     | used by intl |
   |------------------|------------------|------------------|--------------|
   |    grave         |    grave         |LFUN_ACCENT_GRAVE        | grave
   |    acute         |    acute         |LFUN_ACCENT_ACUTE        | acute
   |    circumflex    |    circumflex    |LFUN_ACCENT_CIRCUMFLEX   | circumflex
   | umlaut/dieresis  |    diaeresis     |LFUN_ACCENT_UMLAUT       | umlaut
   |    tilde         |    tilde         |LFUN_ACCENT_TILDE        | tilde
   |    macron        |    maron         |LFUN_ACCENT_MACRON       | macron
   |    dot           |    abovedot      |LFUN_ACCENT_DOT          | dot
   |    cedilla       |    cedilla       |LFUN_ACCENT_CEDILLA      | cedilla
   |    underdot      |                  |LFUN_ACCENT_UNDERDOT     | underdot
   |    underbar      |                  |LFUN_ACCENT_UNDERBAR     | underbar
   |    h�cek         |    caron         |LFUN_ACCENT_CARON        | caron
   |    breve         |    breve         |LFUN_ACCENT_BREVE        | breve
   |    tie           |                  |LFUN_ACCENT_TIE          | tie
   | Hungarian umlaut |    doubleacute   |LFUN_ACCENT_HUNGARIAN_UMLAUT  | hungarian umlaut
   |    circle        |    abovering     |LFUN_ACCENT_CIRCLE       | circle
   |                  |    ogonek        |                  |
   |                  |    iota          |                  |
   |                  |    voiced_sound  |                  |
   |                  | semivoiced_sound |                  |
   |                  |                  |LFUN_ACCENT_SPECIAL_CARON| special caron
   */
// I am not sure how some of the XWindows names coresponds to the TeX ones.

///
enum tex_accent {
	///
	TEX_NOACCENT = 0,
	///
	TEX_ACUTE,
	///
	TEX_GRAVE,
	///
	TEX_MACRON,
	///
	TEX_TILDE,
	///
	TEX_UNDERBAR,
	///
	TEX_CEDILLA,
	///
	TEX_UNDERDOT,
	///
	TEX_CIRCUMFLEX,
	///
	TEX_CIRCLE,
	///
	TEX_TIE,
	///
	TEX_BREVE,
	///
	TEX_CARON,
//  TEX_SPECIAL_CARON,
	///
	TEX_HUNGUML,
	///
	TEX_UMLAUT,
	///
	TEX_DOT,
	///
	TEX_OGONEK,
	///
	TEX_MAX_ACCENT= TEX_OGONEK
};


/** the native is so we know what characters TeX can write in a decent way
 it should be up to the user if _all_ characters should be allowed to
 be accented. Lgb. */
struct tex_accent_struct {
	///
	tex_accent accent;
	///
	char const * cmd;
#if 0
	///
	char const * native;
#endif
	///
	char const * name;
	///
	kb_action action;
};

///
extern tex_accent_struct lyx_accent_table[];

#endif
