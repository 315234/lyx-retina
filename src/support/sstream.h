/* This is part of libio/iostream, providing -*- C++ -*- input/output.
Copyright (C) 2000 Free Software Foundation

This file is part of the GNU IO Library.  This library is free
software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option)
any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this library; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

As a special exception, if you link this library with files
compiled with a GNU compiler to produce an executable, this does not cause
the resulting executable to be covered by the GNU General Public License.
This exception does not however invalidate any other reasons why
the executable file might be covered by the GNU General Public License. */

/* Written by Magnus Fromreide (magfr@lysator.liu.se). */

/* Sligtly modified for use in The LyX Project.
   Made to be usable with both std::string (as supplied with Gcc 2.95.2),
   and with lyxstring. Dynamic casts have been replaced by static_casts.
   One fix to avoid unsigned/signed warnings.

   Lars Gullik Bj�nnes (larsbj@lyx.org)
*/

#ifndef SSTREAM_H
#define SSTREAM_H

#include "support/std_string.h"
#include <iostream>
#include <streambuf.h>

namespace std
{
  class stringbuf : public streambuf
  {
  public:
    typedef char	char_type;
    typedef int		int_type;
    typedef streampos	pos_type;
    typedef streamoff	off_type;

    explicit stringbuf(int which=ios::in|ios::out) :
      streambuf(which), buf(), mode(static_cast<ios::open_mode>(which)),
      rpos(0), bufsize(1)
    { }

    explicit stringbuf(const string &s, int which=ios::in|ios::out) :
      streambuf(which), buf(s), mode(static_cast<ios::open_mode>(which)),
      bufsize(1)
    {
      if(mode & ios::in)
	{
	  setg(&defbuf, &defbuf + bufsize, &defbuf + bufsize);
	}
      if(mode & ios::out)
	{
	  setp(&defbuf, &defbuf + bufsize);
	}
      rpos = (mode & ios::ate ? s.size() : 0);
    }

    string str() const
    {
      const_cast<stringbuf*>(this)->sync();  // Sigh, really ugly hack
      return buf;
    };

    void str(const string& s)
    {
      buf = s;
      if(mode & ios::in)
	{
	  gbump(egptr() - gptr());
	}
      if(mode & ios::out)
	{
	  pbump(pbase() - pptr());
	}
      rpos = (mode & ios::ate ? s.size() : 0);
    }

  protected:
    inline virtual int sync();
    inline virtual int overflow(int = EOF);
    inline virtual int underflow();
  private:
    string			buf;
    ios::open_mode		mode;
    string::size_type	rpos;
    streamsize			bufsize;
    char			defbuf;
  };

  class stringstreambase : virtual public ios {
  protected:
    stringbuf __my_sb;
  public:
    string str() const
    {
      return static_cast<stringbuf*>(_strbuf)->str();
    }
    void str(const string& s)
    {
      clear();
      static_cast<stringbuf*>(_strbuf)->str(s);
    }

    stringbuf* rdbuf()
    {
      return &__my_sb;
    }
  protected:
    stringstreambase(int which) :
      __my_sb(which)
    {
      init (&__my_sb);
    }

    stringstreambase(const string& s, int which) :
      __my_sb(s, which)
    {
      init (&__my_sb);
    }
  };

  class istringstream : public stringstreambase, public istream {
  public:
    istringstream(int which=ios::in) :
      stringstreambase(which)
    { }

    istringstream(const string& s, int which=ios::in) :
      stringstreambase(s, which)
    { }
  };

  class ostringstream : public stringstreambase, public ostream {
  public:
    ostringstream(int which=ios::out) :
      stringstreambase(which)
    { }

    ostringstream(const string& s, int which=ios::out) :
      stringstreambase(s, which)
    { }
  };

  class stringstream : public stringstreambase, public iostream {
  public:
    stringstream(int which=ios::in|ios::out) :
      stringstreambase(which)
    { }

    stringstream(const string &s, int which=ios::in|ios::out) :
      stringstreambase(s, which)
    { }
  };

} // namespace std

inline int stringbuf::sync()
{
  if((mode & ios::out) == 0)
    return EOF;

  streamsize n = pptr() - pbase();
  if(n)
    {
      buf.replace(rpos, string::npos, pbase(), n);
      //if(buf.size() - rpos != n)
      if (buf.size() != n + rpos)
	return EOF;
      rpos += n;
      pbump(-n);
      gbump(egptr() - gptr());
    }
  return 0;
}

inline int stringbuf::overflow(int ch)
{
  if((mode & ios::out) == 0)
    return EOF;

  streamsize n = pptr() - pbase();

  if(n && sync())
    return EOF;

  if(ch != EOF)
    {
      string::size_type oldSize = buf.size();

      buf.replace(rpos, string::npos, 1, ch);
      if(buf.size() - oldSize != 1)
	return EOF;
      ++rpos;
    }
  return 0;
}

inline int stringbuf::underflow()
{
  sync();
  if((mode & ios::in) == 0)
    {
      return EOF;
    }
  if(rpos >= buf.size())
    {
      return EOF;
    }

  string::size_type n = egptr() - eback();
  string::size_type s;

  s = buf.copy(eback(), n, rpos);
  pbump(pbase() - pptr());
  gbump(eback() - gptr());
  int res = (0377 & buf[rpos]);
  rpos += s;
  return res;
}

#endif /* not __STRSTREAM__ */
