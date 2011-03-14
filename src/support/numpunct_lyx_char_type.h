// -*- C++ -*-
/**
 * \file numpunct_lyx_char_type.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Peter K�mmel
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_NUMPUNCT_LYX_CHAR_TYPE_H
#define LYX_NUMPUNCT_LYX_CHAR_TYPE_H


#include <locale>


namespace std
{

	template<>
	class numpunct<lyx::char_type> : public numpunct<char>
	{
	public:

		typedef lyx::char_type char_type;
		typedef basic_string<lyx::char_type> string_type;

		static locale::id id;

		explicit numpunct(size_t __refs = 0) : numpunct<char>(__refs)
		{}

		char_type decimal_point() const
		{ return numpunct<char>::decimal_point(); }

		char_type thousands_sep() const
		{ return numpunct<char>::thousands_sep(); }

		string grouping() const
		{ return numpunct<char>::grouping(); }

		string_type truename() const
		{ return lyx::from_ascii(numpunct<char>::truename()); }

		string_type falsename() const
		{ return lyx::from_ascii(numpunct<char>::falsename()); }


	protected:
		virtual ~numpunct();

	};

	// Fixed in VC11:
	// http://connect.microsoft.com/VisualStudio/feedback/details/572376/msvc10-c-std-numpunct-has-a-hardcoded-dllimport-in-definition

}
#endif
