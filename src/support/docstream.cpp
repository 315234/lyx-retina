/**
 * \file docstream.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Georg Baum
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "docstream.h"
#include "unicode.h"

#include <cerrno>
#include <cstdio>
#include <iconv.h>
#include <locale>


using lyx::ucs4_codeset;

using std::string;


namespace {

// We use C IO throughout this file, because the facets might be used with
// lyxerr in the future.


/// codecvt facet for conversion of UCS4 (internal representation) to UTF8
/// (external representation) or vice versa
class iconv_codecvt_facet : public std::codecvt<lyx::char_type, char, std::mbstate_t>
{
	typedef std::codecvt<lyx::char_type, char, std::mbstate_t> base;
public:
	/// Constructor. You have to specify with \p inout whether you want
	/// to use this facet only for input, only for output or for both.
	explicit iconv_codecvt_facet(string const & encoding = "UTF-8",
			std::ios_base::openmode inout = std::ios_base::in | std::ios_base::out,
			size_t refs = 0)
		: base(refs), encoding_(encoding)
	{
		if (inout & std::ios_base::in) {
			in_cd_ = iconv_open(ucs4_codeset, encoding.c_str());
			if (in_cd_ == (iconv_t)(-1)) {
				fprintf(stderr, "Error %d returned from iconv_open(in_cd_): %s\n",
					errno, strerror(errno));
				fflush(stderr);
				throw lyx::iconv_codecvt_facet_exception();
			}
		} else
			in_cd_ = (iconv_t)(-1);
		if (inout & std::ios_base::out) {
			out_cd_ = iconv_open(encoding.c_str(), ucs4_codeset);
			if (out_cd_ == (iconv_t)(-1)) {
				fprintf(stderr, "Error %d returned from iconv_open(out_cd_): %s\n",
					errno, strerror(errno));
				fflush(stderr);
				throw lyx::iconv_codecvt_facet_exception();
			}
		} else
			out_cd_ = (iconv_t)(-1);
	}
protected:
	virtual ~iconv_codecvt_facet()
	{
		if (in_cd_ != (iconv_t)(-1))
			if (iconv_close(in_cd_) == -1) {
				fprintf(stderr, "Error %d returned from iconv_close(in_cd_): %s\n",
					errno, strerror(errno));
				fflush(stderr);
			}
		if (out_cd_ != (iconv_t)(-1))
			if (iconv_close(out_cd_) == -1) {
				fprintf(stderr, "Error %d returned from iconv_close(out_cd_): %s\n",
					errno, strerror(errno));
				fflush(stderr);
			}
	}
	virtual result do_out(state_type &, intern_type const * from,
			intern_type const * from_end, intern_type const *& from_next,
			extern_type * to, extern_type * to_end,
			extern_type *& to_next) const
	{
		size_t inbytesleft = (from_end - from) * sizeof(intern_type);
		size_t outbytesleft = (to_end - to) * sizeof(extern_type);
		from_next = from;
		to_next = to;
		result const retval = do_iconv(out_cd_,
				reinterpret_cast<char const **>(&from_next),
				&inbytesleft, &to_next, &outbytesleft);
		if (retval == base::error) {
			fprintf(stderr,
				"Error %d returned from iconv when converting from %s to %s: %s\n",
				errno, ucs4_codeset, encoding_.c_str(),
				strerror(errno));
			fputs("Converted input:", stderr);
			for (intern_type const * i = from; i < from_next; ++i) {
				unsigned int const c = *i;
				fprintf(stderr, " 0x%04x", c);
			}
			unsigned int const c = *from_next;
			fprintf(stderr, "\nStopped at: 0x%04x\n", c);
			fputs("Unconverted input:", stderr);
			for (intern_type const * i = from_next + 1; i < from_end; ++i) {
				unsigned int const c = *i;
				fprintf(stderr, " 0x%04x", c);
			}
			fputs("\nConverted output:", stderr);
			for (extern_type const * i = to; i < to_next; ++i) {
				// extern_type may be signed, avoid output of
				// something like 0xffffffc2
				unsigned int const c =
					*reinterpret_cast<unsigned char const *>(i);
				fprintf(stderr, " 0x%02x", c);
			}
			fputc('\n', stderr);
			fflush(stderr);
		}
		return retval;
	}
	virtual result do_unshift(state_type &, extern_type * to,
			extern_type *, extern_type *& to_next) const
	{
		// utf8 does not use shifting
		to_next = to;
		return base::noconv;
	}
	virtual result do_in(state_type &,
			extern_type const * from, extern_type const * from_end,
			extern_type const *& from_next,
			intern_type * to, intern_type * to_end,
			intern_type *& to_next) const
	{
		size_t inbytesleft = (from_end - from) * sizeof(extern_type);
		size_t outbytesleft = (to_end - to) * sizeof(intern_type);
		from_next = from;
		to_next = to;
		result const retval = do_iconv(in_cd_, &from_next, &inbytesleft,
				reinterpret_cast<char **>(&to_next),
				&outbytesleft);
		if (retval == base::error) {
			fprintf(stderr,
				"Error %d returned from iconv when converting from %s to %s: %s\n",
				errno, encoding_.c_str(), ucs4_codeset,
				strerror(errno));
			fputs("Converted input:", stderr);
			for (extern_type const * i = from; i < from_next; ++i) {
				// extern_type may be signed, avoid output of
				// something like 0xffffffc2
				unsigned int const c =
					*reinterpret_cast<unsigned char const *>(i);
				fprintf(stderr, " 0x%02x", c);
			}
			unsigned int const c =
				*reinterpret_cast<unsigned char const *>(from_next);
			fprintf(stderr, "\nStopped at: 0x%02x\n", c);
			fputs("Unconverted input:", stderr);
			for (extern_type const * i = from_next + 1; i < from_end; ++i) {
				unsigned int const c =
					*reinterpret_cast<unsigned char const *>(i);
				fprintf(stderr, " 0x%02x", c);
			}
			fputs("\nConverted output:", stderr);
			for (intern_type const * i = to; i < to_next; ++i) {
				unsigned int const c = *i;
				fprintf(stderr, " 0x%02x", c);
			}
			fputc('\n', stderr);
			fflush(stderr);
		}
		return retval;
	}
	virtual int do_encoding() const throw()
	{
		return 0;
	}
	virtual bool do_always_noconv() const throw()
	{
		return false;
	}
	virtual int do_length(state_type & /*state*/, extern_type const * from,
			extern_type const * end, size_t max) const
	{
		// The docs are a bit unclear about this method.
		// It seems that we should calculate the actual length of the
		// converted sequence, but that would not make sense, since
		// once could just do the conversion directly.
		// Therefore we just return the number of unconverted
		// characters, since that is the best guess we can do.
#if 0
		intern_type * to = new intern_type[max];
		intern_type * to_end = to + max;
		intern_type * to_next = to;
		extern_type const * from_next = from;
		do_in(state, from, end, from_next, to, to_end, to_next);
		delete[] to;
		return to_next - to;
#else
		size_t const length = end - from;
		return std::min(length, max);
#endif
	}
	virtual int do_max_length() const throw()
	{
		// FIXME: this information should be transferred to lib/encodings
		// UTF8 uses at most 4 bytes to represent one UCS4 code point
		// (see RFC 3629). RFC 2279 specifies 6 bytes, but that
		// information is outdated, and RFC 2279 has been superseded by
		// RFC 3629.
		// The CJK encodings use (different) multibyte representation as well.
		// All other encodings encode one UCS4 code point in one byte
		// (and can therefore only encode a subset of UCS4)
		// Note that BIG5 and SJIS do not work with LaTeX (see lib/encodings). 
		// Furthermore, all encodings that use shifting (like SJIS) do not work with 
		// iconv_codecvt_facet.
		if (encoding_ == "UTF-8" ||
		    encoding_ == "GB" ||
		    encoding_ == "EUC-TW")
			return 4;
		else if (encoding_ == "EUC-JP")
			return 3;
		else if (encoding_ == "BIG5" ||
			 encoding_ == "EUC-KR" ||
			 encoding_ == "EUC-CN" ||
			 encoding_ == "SJIS" ||
			 encoding_ == "GBK" ||
			 encoding_ == "JIS" )
			return 2;
		else
			return 1;
	}
private:
	/// Do the actual conversion. The interface is equivalent to that of
	/// iconv() (but const correct).
	inline base::result do_iconv(iconv_t cd, char const ** from,
			size_t * inbytesleft, char ** to, size_t * outbytesleft) const
	{
		char const * const to_start = *to;
		size_t converted = iconv(cd, const_cast<char ICONV_CONST **>(from),
				inbytesleft, to, outbytesleft);
		if (converted == (size_t)(-1)) {
			switch(errno) {
			case EINVAL:
			case E2BIG:
				return base::partial;
			case EILSEQ:
			default:
				return base::error;
			}
		}
		if (*to == to_start)
			return base::noconv;
		return base::ok;
	}
	iconv_t in_cd_;
	iconv_t out_cd_;
	/// The narrow encoding
	std::string encoding_;
};

} // namespace anon


namespace lyx {


const char * iconv_codecvt_facet_exception::what() const throw()
{
	return "iconv problem in iconv_codecvt_facet initialization";
}


idocfstream::idocfstream(string const & encoding) : base()
{
	std::locale global;
	std::locale locale(global, new iconv_codecvt_facet(encoding, in));
	imbue(locale);
}


idocfstream::idocfstream(const char* s, std::ios_base::openmode mode,
			 string const & encoding)
	: base()
{
	// We must imbue the stream before openening the file
	std::locale global;
	std::locale locale(global, new iconv_codecvt_facet(encoding, in));
	imbue(locale);
	open(s, mode);
}


odocfstream::odocfstream(string const & encoding) : base()
{
	std::locale global;
	std::locale locale(global, new iconv_codecvt_facet(encoding, out));
	imbue(locale);
}


odocfstream::odocfstream(const char* s, std::ios_base::openmode mode,
			 string const & encoding)
	: base()
{
	// We must imbue the stream before openening the file
	std::locale global;
	std::locale locale(global, new iconv_codecvt_facet(encoding, out));
	imbue(locale);
	open(s, mode);
}


SetEnc setEncoding(string const & encoding)
{
	return SetEnc(encoding);
}


odocstream & operator<<(odocstream & os, SetEnc e)
{
	if (std::has_facet<iconv_codecvt_facet>(os.rdbuf()->getloc())) {
		// This stream must be a file stream, since we never imbue
		// any other stream with a locale having a iconv_codecvt_facet.
		// Flush the stream so that all pending output is written
		// with the old encoding.
		os.flush();
		std::locale locale(os.rdbuf()->getloc(),
			new iconv_codecvt_facet(e.encoding, std::ios_base::out));
		// FIXME Does changing the codecvt facet of an open file
		// stream always work? It does with gcc 4.1, but I have read
		// somewhere that it does not with MSVC.
		// What does the standard say?
		os.imbue(locale);
	}
	return os;
}


#if ! defined(USE_WCHAR_T)
odocstream & operator<<(odocstream & os, char c)
{
	os.put(c);
	return os;
}
#endif

}

#if ! defined(USE_WCHAR_T) && defined(__GNUC__)
// We get undefined references to these virtual methods. This looks like
// a bug in gcc. The implementation here does not do anything useful, since
// it is overriden in iconv_codecvt_facet.
namespace std {

template<> codecvt<lyx::char_type, char, mbstate_t>::result
codecvt<lyx::char_type, char, mbstate_t>::do_out(
	mbstate_t &, const lyx::char_type *, const lyx::char_type *,
	const lyx::char_type *&, char *, char *, char *&) const
{
	return error;
}


template<> codecvt<lyx::char_type, char, mbstate_t>::result
codecvt<lyx::char_type, char, mbstate_t>::do_unshift(
	mbstate_t &, char *, char *, char *&) const
{
	return error;
}


template<> codecvt<lyx::char_type, char, mbstate_t>::result
codecvt<lyx::char_type, char, mbstate_t>::do_in(
	mbstate_t &, const char *, const char *, const char *&,
	lyx::char_type *, lyx::char_type *, lyx::char_type *&) const
{
	return error;
}


template<>
int codecvt<lyx::char_type, char, mbstate_t>::do_encoding() const throw()
{
	return 0;
}


template<>
bool codecvt<lyx::char_type, char, mbstate_t>::do_always_noconv() const throw()
{
	return true;
}

#if __GNUC__ == 3 && __GNUC_MINOR__ < 4

template<>
int codecvt<lyx::char_type, char, mbstate_t>::do_length(
	mbstate_t const &, const char *, const char *, size_t) const
{
	return 1;
}

#else

template<>
int codecvt<lyx::char_type, char, mbstate_t>::do_length(
	mbstate_t &, const char *, const char *, size_t) const
{
	return 1;
}

#endif

template<>
int codecvt<lyx::char_type, char, mbstate_t>::do_max_length() const throw()
{
	return 4;
}

} // namespace std
#endif
