// -*- C++ -*-
/**
 * \file lyx_cb.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_CB_H
#define LYX_CB_H

#include "LString.h"

class Buffer;
class BufferView;

///
extern bool quitting;

///
bool MenuWrite(Buffer * buffer);
/// write the given file, or ask if no name given
bool WriteAs(Buffer * buffer, string const & filename = string());
///
void QuitLyX();
///
void AutoSave(BufferView * bv);
///
void NewFile(BufferView * bv, string const & filename);
///
void InsertAsciiFile(BufferView * bv, string const & f, bool asParagraph);
///
string getContentsOfAsciiFile(BufferView * bv, string const & f, bool asParagraph);
///
string const getPossibleLabel(BufferView const & bv);
///
void Reconfigure(BufferView * bv);
#endif
