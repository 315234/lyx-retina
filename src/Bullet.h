// -*- C++ -*-
/**
 * \file Bullet.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Allan Rae
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef BULLET_H
#define BULLET_H

#include "LString.h"

///
class Bullet {
public:
	///
	Bullet(int f = -1, int c = -1, int s = -1);

	///
	explicit Bullet(string const &);

	///
	void setCharacter(int);
	///
	void setFont(int);
	///
	void setSize(int);
	///
	void setText(string const &);
	///
	int getCharacter() const;
	///
	int getFont() const;
	///
	int getSize() const;
	///
	string const & getText() const;
	///
	Bullet & operator=(Bullet const &);
	///
	friend bool operator==(Bullet const &, Bullet const &);
protected:
#ifdef ENABLE_ASSERTIONS
	///
	void testInvariant() const;
#endif
private:
	/**
	   This enum makes adding additional panels or changing panel sizes
	   easier. Since you only need change these values for all tests to
	   be correct for the new values.

	   Note: MAX means the size of the array so to test you need:
	   (x < MAX)  *not* (x <= MAX)
	*/
	enum {
		///
		MIN = -1,
		///
		FONTMAX = 6,
		///
		CHARMAX = 36,
		///
		SIZEMAX = 10
	};

	///
	void generateText() const;
	///
	static string const bulletSize(int);
	///
	static string const bulletEntry(int, int);

	///
	int font;
	///
	int character;
	///
	int size;

	// size, character and font are array indices to access
	// the predefined arrays of LaTeX equivalent strings.

	/** flag indicates if user has control of text (1)
	    or if I can use it to generate strings (0)
	    or have already (-1)
	*/
	mutable short user_text;

	//NOTE: Arranging these four shorts above to be together
	//      like this should ensure they are in a single cache line

	/** text may contain a user-defined LaTeX symbol command
	    or one generated internally from the font, character
	    and size settings.
	*/
	mutable string text;
};


inline
bool operator!=(Bullet const & b1, Bullet const & b2)
{
	return !(b1 == b2);
}

///
extern
Bullet const ITEMIZE_DEFAULTS[];

#endif /* BULLET_H_ */
