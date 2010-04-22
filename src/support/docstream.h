// -*- C++ -*-
/**
 * \file docstream.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Georg Baum
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_DOCSTREAM_H
#define LYX_DOCSTREAM_H

#include "support/docstring.h"

#include <fstream>
#include <sstream>

namespace lyx {

class iconv_codecvt_facet_exception : public std::exception {
public:
	virtual ~iconv_codecvt_facet_exception() throw() {}
	virtual const char * what() const throw();
};

/// Base class for UCS4 input streams
typedef std::basic_istream<char_type> idocstream;

/** Base class for UCS4 output streams.
    If you want to output a single UCS4 character, use \code
    os.put(c);
    \endcode, not \code
    os << c;
    \endcode . The latter will not output the character, but the code point
    as number. This is because we can't overload operator<< (our character
    type is not a real type but a typedef). Narrow characters of type char
    can be output as usual.
 */
typedef std::basic_ostream<char_type> odocstream;

/// File stream for reading UTF8-encoded files with automatic conversion to
/// UCS4.
class ifdocstream : public std::basic_ifstream<char_type> {
	typedef std::basic_ifstream<char_type> base;
public:
	ifdocstream();
	explicit ifdocstream(const char* s,
		std::ios_base::openmode mode = std::ios_base::in,
		std::string const & encoding = "UTF-8");
	~ifdocstream() {}
};

/// File stream for writing files in 8bit encoding \p encoding with automatic
/// conversion from UCS4.

class ofdocstream : public std::basic_ofstream<char_type> {
	typedef std::basic_ofstream<char_type> base;
public:
	ofdocstream();
	explicit ofdocstream(const char* s,
		std::ios_base::openmode mode = std::ios_base::out|std::ios_base::trunc,
		std::string const & encoding = "UTF-8");
	~ofdocstream() {}
	///
	void reset(std::string const & encoding);
};


extern template class std::numpunct<lyx::char_type>;
extern template class std::basic_ofstream<char_type>;

/// UCS4 input stringstream
typedef std::basic_istringstream<char_type> idocstringstream;

/// UCS4 output stringstream
typedef std::basic_ostringstream<char_type> odocstringstream;

/// Helper struct for changing stream encoding
struct SetEnc {
	SetEnc(std::string const & e) : encoding(e) {}
	std::string encoding;
};

/// Helper function for changing stream encoding
SetEnc setEncoding(std::string const & encoding);

/** Change the encoding of \p os to \p e.encoding.
    \p e.encoding must be a valid iconv name of an 8bit encoding.
    This does nothing if the stream is not a file stream, since only
    file streams do have an associated 8bit encoding.
    Usage: \code
    os << setEncoding("ISO-8859-1");
    \endcode
 */
odocstream & operator<<(odocstream & os, SetEnc e);
idocstream & operator<<(idocstream & os, SetEnc e);

}

#endif
