// -*- C++ -*-
/**
 * \file lstrings.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 *
 * Full author contact details are available in file CREDITS.
 *
 * A collection of string helper functions that works with string.
 * Some of these would certainly benefit from a rewrite/optimization.
 */

#ifndef LSTRINGS_H
#define LSTRINGS_H

#include "support/docstring.h"

#include <vector>


namespace lyx {
namespace support {

/// Compare \p s and \p s2, ignoring the case.
/// Does not depend on the locale.
int compare_no_case(docstring const & s, docstring const & s2);

/// Compare \p s and \p s2, ignoring the case of ASCII characters only.
int compare_ascii_no_case(std::string const & s, std::string const & s2);

/// Compare \p s and \p s2, ignoring the case of ASCII characters only.
int compare_ascii_no_case(docstring const & s, docstring const & s2);

///
inline
int compare(char const * a, char const * b)
{
#ifndef CXX_GLOBAL_CSTD
	return std::strcmp(a, b);
#else
	return strcmp(a, b);
#endif
}

///
inline
int compare(char const * a, char const * b, unsigned int len)
{
#ifndef CXX_GLOBAL_CSTD
	return std::strncmp(a, b, len);
#else
	return strncmp(a, b, len);
#endif
}

///
bool isStrInt(std::string const & str);

/// does the std::string represent an unsigned integer value ?
bool isStrUnsignedInt(std::string const & str);

///
bool isStrDbl(std::string const & str);

bool isHex(lyx::docstring const & str);

int hexToInt(lyx::docstring const & str);

/// is \p str pure ascii?
bool isAscii(docstring const & str);

/// Changes the case of \p c to lowercase.
/// Caution: Depends on the locale
char lowercase(char c);

/// Changes the case of \p c to uppercase.
/// Caution: Depends on the locale
char uppercase(char c);

/// Changes the case of \p c to lowercase.
/// Does not depend on the locale.
char_type lowercase(char_type c);

/// Changes the case of \p c to uppercase.
/// Does not depend on the locale.
char_type uppercase(char_type c);

/// same as lowercase(), but ignores locale
std::string const ascii_lowercase(std::string const &);
docstring const ascii_lowercase(docstring const &);

/// Changes the case of \p s to lowercase.
/// Does not depend on the locale.
docstring const lowercase(docstring const & s);

/// Changes the case of \p s to uppercase.
/// Does not depend on the locale.
docstring const uppercase(docstring const & s);

/// Does the string start with this prefix?
bool prefixIs(docstring const &, char_type);

/// Does the std::string start with this prefix?
bool prefixIs(std::string const &, std::string const &);
bool prefixIs(docstring const &, docstring const &);

/// Does the string end with this char?
bool suffixIs(std::string const &, char);
bool suffixIs(docstring const &, char_type);

/// Does the std::string end with this suffix?
bool suffixIs(std::string const &, std::string const &);

///
inline bool contains(std::string const & a, std::string const & b)
{
	return a.find(b) != std::string::npos;
}

inline bool contains(docstring const & a, docstring const & b)
{
	return a.find(b) != docstring::npos;
}

inline bool contains(std::string const & a, char b)
{
	return a.find(b) != std::string::npos;
}

inline bool contains(docstring const & a, char_type b)
{
	return a.find(b) != docstring::npos;
}

///
bool containsOnly(std::string const &, std::string const &);

/** Extracts a token from this string at the nth delim.
    Doesn't modify the original string. Similar to strtok.
    Example:
    \code
    token("a;bc;d", ';', 1) == "bc";
    token("a;bc;d", ';', 2) == "d";
    \endcode
*/
std::string const token(std::string const & a, char delim, int n);

docstring const token(docstring const & a, char_type delim, int n);

/** Search a token in this string using the delim.
    Doesn't modify the original string. Returns -1 in case of
    failure.
    Example:
    \code
    tokenPos("a;bc;d", ';', "bc") == 1;
    tokenPos("a;bc;d", ';', "d") == 2;
    \endcode
*/
int tokenPos(std::string const & a, char delim, std::string const & tok);


/// Substitute all \a oldchar with \a newchar
std::string const subst(std::string const & a, char oldchar, char newchar);

/// Substitute all \a oldchar with \a newchar
docstring const subst(docstring const & a, char_type oldchar, char_type newchar);

/// substitutes all instances of \a oldstr with \a newstr
std::string const subst(std::string const & a,
		   std::string const & oldstr, std::string const & newstr);

/// substitutes all instances of \a oldstr with \a newstr
docstring const subst(docstring const & a,
		docstring const & oldstr, docstring const & newstr);

/** Trims characters off the end and beginning of a string.
    \code
    trim("ccabccc", "c") == "ab".
    \endcode
*/
docstring const trim(docstring const & a, char const * p = " ");

/** Trims characters off the end and beginning of a string.
    \code
    trim("ccabccc", "c") == "ab".
    \endcode
*/
std::string const trim(std::string const & a, char const * p = " ");

/** Trims characters off the end of a string.
    \code
    rtrim("abccc", "c") == "ab".
    \endcode
*/
std::string const rtrim(std::string const & a, char const * p = " ");
docstring const rtrim(docstring const & a, char const * p = " ");

/** Trims characters off the beginning of a string.
    \code
   ("ababcdef", "ab") = "cdef"
    \endcode
*/
std::string const ltrim(std::string const & a, char const * p = " ");
docstring const ltrim(docstring const & a, char const * p = " ");

/** Splits the string by the first delim.
    Splits the string by the first appearance of delim.
    The leading string up to delim is returned in piece (not including
    delim), while the original string is cut from after the delimiter.
    Example:
    \code
    s1= ""; s2= "a;bc".split(s1, ';') -> s1 == "a"; s2 == "bc";
    \endcode
*/
std::string const split(std::string const & a, std::string & piece, char delim);
docstring const split(docstring const & a, docstring & piece, char_type delim);

/// Same as split but does not return a piece
std::string const split(std::string const & a, char delim);

/// Same as split but uses the last delim.
std::string const rsplit(std::string const & a, std::string & piece, char delim);

/// Escapes non ASCII chars and other problematic characters that cause
/// problems in latex labels.
docstring const escape(docstring const & lab);

/// gives a vector of stringparts which have the delimiter delim
std::vector<std::string> const getVectorFromString(std::string const & str,
					      std::string const & delim = std::string(","));
std::vector<docstring> const getVectorFromString(docstring const & str,
		docstring const & delim = from_ascii(","));

// the same vice versa
std::string const getStringFromVector(std::vector<std::string> const & vec,
				 std::string const & delim = std::string(","));

/// Search \p search_token in \p str and return the position if it is
/// found, else -1. The last item in \p str must be "".
int findToken(char const * const str[], std::string const & search_token);

/// Convert internal line endings to line endings as expected by the OS
docstring const externalLineEnding(docstring const & str);

/// Convert line endings in any formnat to internal line endings
docstring const internalLineEnding(docstring const & str);


#ifdef I_AM_NOT_AFRAID_OF_HEADER_LIBRARIES

#include <boost/format.hpp>

template<class Arg1>
docstring bformat(docstring const & fmt, Arg1 arg1)
{
	return (boost::basic_format<char_type>(fmt) % arg1).str();
}


template<class Arg1, class Arg2>
docstring bformat(docstring const & fmt, Arg1 arg1, Arg2 arg2)
{
	return (boost::basic_format<char_type>(fmt) % arg1 % arg2).str();
}


template<class Arg1, class Arg2, class Arg3>
docstring bformat(docstring const & fmt, Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
	return (boost::basic_format<char_type>(fmt) % arg1 % arg2 % arg3).str();
}


template<class Arg1, class Arg2, class Arg3, class Arg4>
docstring bformat(docstring const & fmt, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
	return (boost::basic_format<char_type>(fmt) % arg1 % arg2 % arg3 % arg4).str();
}

#else

template <class Arg1>
docstring bformat(docstring const & fmt, Arg1);

template <class Arg1, class Arg2>
docstring bformat(docstring const & fmt, Arg1, Arg2);

template <class Arg1, class Arg2, class Arg3>
docstring bformat(docstring const & fmt, Arg1, Arg2, Arg3);

template <class Arg1, class Arg2, class Arg3, class Arg4>
docstring bformat(docstring const & fmt, Arg1, Arg2, Arg3, Arg4);

#endif

} // namespace support
} // namespace lyx

#endif
