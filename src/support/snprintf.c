/*
 * snprintf.c - a portable implementation of snprintf
 *
 * AUTHOR
 *   Mark Martinec <mark.martinec@ijs.si>, April 1999.
 *
 *   Copyright 1999, Mark Martinec. All rights reserved.
 *
 * TERMS AND CONDITIONS
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the "Frontier Artistic License" which comes
 *   with this Kit.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *   See the Frontier Artistic License for more details.
 *
 *   You should have received a copy of the Frontier Artistic License
 *   with this Kit in the file named LICENSE.txt .
 *   If not, I'll be glad to provide one.
 *
 * FEATURES
 * - careful adherence to specs regarding flags, field width and precision;
 * - good performance for large string handling (large format, large
 *   argument or large paddings). Performance is similar to system's sprintf
 *   and in several cases significantly better (make sure you compile with
 *   optimizations turned on, tell the compiler the code is strict ANSI
 *   if necessary to give it more freedom for optimizations);
 * - return value semantics as per ISO C9X;
 * - written in standard ISO/ANSI C - requires an ANSI C compiler.
 *
 * SUPPORTED FORMATS AND DATA TYPES
 *
 * This snprintf only supports format specifiers:
 * s, c, d, o, u, x, X, p  (and synonyms: i, D, U, O - see below)
 * with flags: '-', '+', ' ', '0' and '#'.
 * An asterisk is supported for field width as well as precision.
 *
 * Data type modifiers 'h' (short int), 'l' (long int)
 * and 'll' (long long int) are supported.
 * NOTE:
 *   If macro SNPRINTF_LONGLONG_SUPPORT is not defined (default) the
 *   data type modifier 'll' is recognized but treated the same as 'l',
 *   which may cause argument value truncation! Defining
 *   SNPRINTF_LONGLONG_SUPPORT requires that your system's sprintf also
 *   handles data type modifier 'll'. long long int is a language
 *   extension which may not be portable.
 *
 * Conversion of numeric data (formats d, o, u, x, X, p) with data type
 * modifiers (none or h, l, ll) is left to the system routine sprintf,
 * but all handling of flags, field width and precision as well as c and
 * s formats is done very carefully by this portable routine. If a string
 * precision (truncation) is specified (e.g. %.8s) it is guaranteed the
 * string beyond the specified precision will not be referenced.
 *
 * Data type modifiers h, l and ll are ignored for c and s formats (data
 * types wint_t and wchar_t are not supported).
 *
 * The following common synonyms for conversion characters are supported:
 *   - i is a synonym for d
 *   - D is a synonym for ld, explicit data type modifiers are ignored
 *   - U is a synonym for lu, explicit data type modifiers are ignored
 *   - O is a synonym for lo, explicit data type modifiers are ignored
 *
 * The following is specifically not supported:
 *   - flag ' (thousands' grouping character) is recognized but ignored
 *   - numeric formats: f, e, E, g, G and synonym F
 *   - data type modifier 'L' (long double) and 'q' (quad - use 'll' instead)
 *   - wide character/string formats: C, lc, S, ls
 *   - writeback of converted string length: conversion character n
 *   - the n$ specification for direct reference to n-th argument
 *   - locales
 *
 * It is permitted for str_m to be zero, and it is permitted to specify NULL
 * pointer for resulting string argument if str_m is zero (as per ISO C9X).
 *
 * The return value is the number of characters which would be generated
 * for the given input, excluding the trailing null. If this value
 * is greater or equal to str_m, not all characters from the result
 * have been stored in str. If str_m is greater than zero it is
 * guaranteed the resulting string will be null-terminated.
 *
 * NOTE that this matches the ISO C9X and GNU C library 2.1,
 * but is different from some older implementations!
 *
 * Routines asprintf and vasprintf return a pointer (in the ptr argument)
 * to a buffer sufficiently large to hold the resulting string. This pointer
 * should be passed to free(3) to release the allocated storage when it is
 * no longer needed. If sufficient space cannot be allocated, these functions
 * will return -1 and set ptr to be a NULL pointer. These two routines are a
 * GNU C library extensions (glibc).
 *
 * Routines asnprintf and vasnprintf are similar to asprintf and vasprintf,
 * yet, like snprintf and vsnprintf counterparts, will write at most str_m-1
 * characters into the allocated output string, the last character in the
 * allocated buffer then gets the terminating null. If the formatted string
 * length (the return value) is greater than or equal to the str_m argument,
 * the resulting string was truncated and some of the formatted characters
 * were discarded. These routines present a handy way to limit the amount
 * of allocated memory to some sane value.
 *
 * AVAILABILITY
 *   http://www.ijs.si/software/snprintf/
 *
 * REVISION HISTORY
 * 1999-04	V0.9  Mark Martinec
 *		- initial version, some modifications after comparing printf
 *		  man pages for Digital Unix 4.0, Solaris 2.6 and HPUX 10,
 *		  and checking how Perl handles sprintf (differently!);
 * 1999-04-09	V1.0  Mark Martinec <mark.martinec@ijs.si>
 *		- added main test program, fixed remaining inconsistencies,
 *		  added optional (long long int) support;
 * 1999-04-12	V1.1  Mark Martinec <mark.martinec@ijs.si>
 *		- support the 'p' format (pointer to void);
 *		- if a string precision is specified
 *		  make sure the string beyond the specified precision
 *		  will not be referenced (e.g. by strlen);
 * 1999-04-13	V1.2  Mark Martinec <mark.martinec@ijs.si>
 *		- support synonyms %D=%ld, %U=%lu, %O=%lo;
 *		- speed up the case of long format string with few conversions;
 * 1999-06-30	V1.3  Mark Martinec <mark.martinec@ijs.si>
 *		- fixed runaway loop (eventually crashing when str_l wraps
 *		  beyond 2*31) while copying format string without
 *		  conversion specifiers to a buffer that is too short
 *		  (thanks to Edwin Young <edwiny@autonomy.com> for
 *		  spotting the problem);
 *		- added macros PORTABLE_SNPRINTF_VERSION_(MAJOR|MINOR)
 *		  to snprintf.h
 * 2000-02-14	V2.0 (never released) Mark Martinec <mark.martinec@ijs.si>
 *		- relaxed license terms: The Artistic License now applies.
 *		  You may still apply the GNU GENERAL PUBLIC LICENSE
 *		  as was distributed with previous versions, if you prefer;
 *		- changed REVISION HISTORY dates to use ISO 8601 date format;
 *		- added vsnprintf (patch also independently proposed by
 *		  Caolan McNamara 2000-05-04, and Keith M Willenson 2000-06-01)
 * 2000-06-27	V2.1  Mark Martinec <mark.martinec@ijs.si>
 *		- removed POSIX check for str_m<1; value 0 for str_m is
 *		  allowed by ISO C9X (and GNU C library 2.1) - (pointed out
 *		  on 2000-05-04 by Caolan McNamara, caolan@ csn dot ul dot ie).
 *		  Besides relaxed license this change in standards adherence
 *		  is the main reason to bump up the major version number;
 *		- added nonstandard routines asnprintf, vasnprintf, asprintf,
 *		  vasprintf that dynamically allocate storage for the
 *		  resulting string; these routines are not compiled by default,
 *		  see comments where NEED_V?ASN?PRINTF macros are defined;
 *		- autoconf contributed by Caolan McNamara
 */


/* Define HAVE_SNPRINTF if your system already has snprintf and vsnprintf.
 *
 * If HAVE_SNPRINTF is defined this module will not produce code for
 * snprintf and vsnprintf, unless PREFER_PORTABLE_SNPRINTF is defined as well,
 * causing this portable version of snprintf to be called portable_snprintf
 * (and portable_vsnprintf).
 */
/* #define HAVE_SNPRINTF */

/* Define PREFER_PORTABLE_SNPRINTF if your system does have snprintf and
 * vsnprintf but you would prefer to use the portable routine(s) instead.
 * In this case the portable routine is declared as portable_snprintf
 * (and portable_vsnprintf) and a macro 'snprintf' (and 'vsnprintf')
 * is defined to expand to 'portable_v?snprintf' - see file snprintf.h .
 * Defining this macro is only useful if HAVE_SNPRINTF is also defined,
 * but does does no harm if defined nevertheless.
 */
/* #define PREFER_PORTABLE_SNPRINTF */

/* Define SNPRINTF_LONGLONG_SUPPORT if you want to support
 * data type (long long int) and data type modifier 'll' (e.g. %lld).
 * If undefined, 'll' is recognized but treated as a single 'l'.
 *
 * If the system's sprintf does not handle 'll'
 * the SNPRINTF_LONGLONG_SUPPORT must not be defined!
 *
 * This is off by default since (long long int) is a language extension.
 */
/* #define SNPRINTF_LONGLONG_SUPPORT */

/* Define NEED_SNPRINTF_ONLY if you only need snprintf, and not vsnprintf.
 * If NEED_SNPRINTF_ONLY is defined, the snprintf will be defined directly,
 * otherwise both snprintf and vsnprintf routines will be defined
 * and snprintf will be a simple wrapper around vsnprintf, at the expense
 * of an extra procedure call.
 */
/* #define NEED_SNPRINTF_ONLY */

/* Define NEED_V?ASN?PRINTF macros if you need library extension
 * routines asprintf, vasprintf, asnprintf, vasnprintf respectively,
 * and your system library does not provide them. They are all small
 * wrapper routines around portable_vsnprintf. Defining any of the four
 * NEED_V?ASN?PRINTF macros automatically turns off NEED_SNPRINTF_ONLY
 * and turns on PREFER_PORTABLE_SNPRINTF.
 *
 * Watch for name conflicts with the system library if these routines
 * are already present there.
 *
 * NOTE: vasprintf and vasnprintf routines need va_copy() from stdarg.h, as
 * specified by C9X, to be able to traverse the same list of arguments twice.
 * I don't know of any other standard and portable way of achieving the same.
 * With some versions of gcc you may use __va_copy(). You might even get away
 * with "ap2 = ap", in this case you must not call va_end(ap2) !
 */
/* #define NEED_ASPRINTF   */
/* #define NEED_ASNPRINTF  */
/* #define NEED_VASPRINTF  */
/* #define NEED_VASNPRINTF */


/* Define the following macros if desired:
 *   SOLARIS_COMPATIBLE, SOLARIS_BUG_COMPATIBLE,
 *   HPUX_COMPATIBLE, HPUX_BUG_COMPATIBLE,
 *   DIGITAL_UNIX_COMPATIBLE, DIGITAL_UNIX_BUG_COMPATIBLE,
 *   PERL_COMPATIBLE, PERL_BUG_COMPATIBLE,
 *
 * - For portable applications it is best not to rely on peculiarities
 *   of a given implementation so it may be best not to define any
 *   of the macros that select compatibility and to avoid features
 *   that vary among the systems.
 *
 * - Selecting compatibility with more than one operating system
 *   is not strictly forbidden but is not recommended.
 *
 * - 'x'_BUG_COMPATIBLE implies 'x'_COMPATIBLE .
 *
 * - 'x'_COMPATIBLE refers to (and enables) a behaviour that is
 *   documented in a sprintf man page on a given operating system
 *   and actually adhered to by the system's sprintf (but not on
 *   most other operating systems). It may also refer to and enable
 *   a behaviour that is declared 'undefined' or 'implementation specific'
 *   in the man page but a given implementation behaves predictably
 *   in a certain way.
 *
 * - 'x'_BUG_COMPATIBLE refers to (and enables) a behaviour of system's sprintf
 *   that contradicts the sprintf man page on the same operating system.
 *
 * - I do not claim that the 'x'_COMPATIBLE and 'x'_BUG_COMPATIBLE
 *   conditionals take into account all idiosyncrasies of a particular
 *   implementation, there may be other incompatibilities.
 */

/* added by Lgb, the LyX Project */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


/* ============================================= */
/* NO USER SERVICABLE PARTS FOLLOWING THIS POINT */
/* ============================================= */

#define PORTABLE_SNPRINTF_VERSION_MAJOR 2
#define PORTABLE_SNPRINTF_VERSION_MINOR 1

#if defined(NEED_ASPRINTF) || defined(NEED_ASNPRINTF) || defined(NEED_VASPRINTF) || defined(NEED_VASNPRINTF)
# if defined(NEED_SNPRINTF_ONLY)
# undef NEED_SNPRINTF_ONLY
# endif
# if !defined(PREFER_PORTABLE_SNPRINTF)
# define PREFER_PORTABLE_SNPRINTF
# endif
#endif

#if defined(SOLARIS_BUG_COMPATIBLE) && !defined(SOLARIS_COMPATIBLE)
#define SOLARIS_COMPATIBLE
#endif

#if defined(HPUX_BUG_COMPATIBLE) && !defined(HPUX_COMPATIBLE)
#define HPUX_COMPATIBLE
#endif

#if defined(DIGITAL_UNIX_BUG_COMPATIBLE) && !defined(DIGITAL_UNIX_COMPATIBLE)
#define DIGITAL_UNIX_COMPATIBLE
#endif

#if defined(PERL_BUG_COMPATIBLE) && !defined(PERL_COMPATIBLE)
#define PERL_COMPATIBLE
#endif

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <errno.h>

#ifdef isdigit
#undef isdigit
#endif
#define isdigit(c) ((c) >= '0' && (c) <= '9')

/* prototypes */

#if defined(NEED_ASPRINTF)
int asprintf   (char **ptr, const char *fmt, /*args*/ ...);
#endif
#if defined(NEED_VASPRINTF)
int vasprintf  (char **ptr, const char *fmt, va_list ap);
#endif
#if defined(NEED_ASNPRINTF)
int asnprintf  (char **ptr, size_t str_m, const char *fmt, /*args*/ ...);
#endif
#if defined(NEED_VASNPRINTF)
int vasnprintf (char **ptr, size_t str_m, const char *fmt, va_list ap);
#endif

#ifndef va_copy
#define va_copy(ap2,ap) ap2 = ap
#endif

#if defined(HAVE_SNPRINTF)
/* declare our portable snprintf  routine under name portable_snprintf  */
/* declare our portable vsnprintf routine under name portable_vsnprintf */
#else
/* declare our portable routines under names snprintf and vsnprintf */
#define portable_snprintf snprintf
#if !defined(NEED_SNPRINTF_ONLY)
#define portable_vsnprintf vsnprintf
#endif
#endif

#if !defined(HAVE_SNPRINTF) || defined(PREFER_PORTABLE_SNPRINTF)
int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
#if !defined(NEED_SNPRINTF_ONLY)
int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
#endif
#endif

/* declarations */

#if !defined(lint)
static char credits[] = "\n\
@(#)snprintf.c, v2.1: Mark Martinec, <mark.martinec@ijs.si>\n\
@(#)snprintf.c, v2.1: Copyright 1999, Mark Martinec. Artistic license applies.\n\
@(#)snprintf.c, v2.1: http://www.ijs.si/software/snprintf/\n";
#endif

#if defined(NEED_ASPRINTF)
int asprintf(char **ptr, const char *fmt, /*args*/ ...) {
  va_list ap;
  size_t str_m;
  int str_l;

  *ptr = NULL;
  va_start(ap, fmt);                            /* measure the required size */
  str_l = portable_vsnprintf(NULL, (size_t) 0, fmt, ap);
  va_end(ap);
  assert(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
  *ptr = (char *) malloc(str_m = (size_t)str_l + 1);
  if (*ptr == NULL) { errno = ENOMEM; str_l = -1; }
  else {
    int str_l2;
    va_start(ap, fmt);
    str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
    va_end(ap);
    assert(str_l2 == str_l);
  }
  return str_l;
}
#endif

#if defined(NEED_VASPRINTF)
int vasprintf(char **ptr, const char *fmt, va_list ap) {
  size_t str_m;
  int str_l;

  *ptr = NULL;
  { va_list ap2;
    va_copy(ap2, ap);  /* don't consume the original ap, we'll need it again */
                       /* measure the required size: */
    str_l = portable_vsnprintf(NULL, (size_t) 0, fmt, ap2);
    va_end(ap2);
  }
  assert(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
  *ptr = (char *) malloc(str_m = (size_t)str_l + 1);
  if (*ptr == NULL) { errno = ENOMEM; str_l = -1; }
  else {
    int str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
    assert(str_l2 == str_l);
  }
  return str_l;
}
#endif

#if defined(NEED_ASNPRINTF)
int asnprintf (char **ptr, size_t str_m, const char *fmt, /*args*/ ...) {
  va_list ap;
  int str_l;

  *ptr = NULL;
  va_start(ap, fmt);                            /* measure the required size */
  str_l = portable_vsnprintf(NULL, (size_t) 0, fmt, ap);
  va_end(ap);
  assert(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
  if ((size_t)str_l + 1 < str_m) str_m = (size_t)str_l + 1;      /* truncate */
  /* if str_m is 0, no buffer is allocated, just set *ptr to NULL */
  if (str_m == 0) {  /* not interested in resulting string, just return size */
  } else {
    *ptr = (char *) malloc(str_m);
    if (*ptr == NULL) { errno = ENOMEM; str_l = -1; }
    else {
      int str_l2;
      va_start(ap, fmt);
      str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
      va_end(ap);
      assert(str_l2 == str_l);
    }
  }
  return str_l;
}
#endif

#if defined(NEED_VASNPRINTF)
int vasnprintf (char **ptr, size_t str_m, const char *fmt, va_list ap) {
  int str_l;

  *ptr = NULL;
  { va_list ap2;
    va_copy(ap2, ap);  /* don't consume the original ap, we'll need it again */
                       /* measure the required size: */
    str_l = portable_vsnprintf(NULL, (size_t) 0, fmt, ap2);
    va_end(ap2);
  }
  assert(str_l >= 0);        /* possible integer overflow if str_m > INT_MAX */
  if ((size_t)str_l + 1 < str_m) str_m = (size_t)str_l + 1;      /* truncate */
  /* if str_m is 0, no buffer is allocated, just set *ptr to NULL */
  if (str_m == 0) {  /* not interested in resulting string, just return size */
  } else {
    *ptr = (char *) malloc(str_m);
    if (*ptr == NULL) { errno = ENOMEM; str_l = -1; }
    else {
      int str_l2 = portable_vsnprintf(*ptr, str_m, fmt, ap);
      assert(str_l2 == str_l);
    }
  }
  return str_l;
}
#endif

/*
 * If the system does have snprintf and the portable routine is not
 * specifically required, this module produces no code for snprintf/vsnprintf.
 */
#if !defined(HAVE_SNPRINTF) || defined(PREFER_PORTABLE_SNPRINTF)

#if !defined(NEED_SNPRINTF_ONLY)
int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...) {
  va_list ap;
  int str_l;

  va_start(ap, fmt);
  str_l = portable_vsnprintf(str, str_m, fmt, ap);
  va_end(ap);
  return str_l;
}
#endif

#if defined(NEED_SNPRINTF_ONLY)
int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...) {
#else
int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap) {
#endif

#if defined(NEED_SNPRINTF_ONLY)
  va_list ap;
#endif
  size_t str_l = 0;
  const char *p = fmt;

/* In contrast with POSIX, the ISO C9X now says
 * that str can be NULL and str_m can be 0. This is more useful. */
/*if (str_m < 1) return -1;*/

#if defined(NEED_SNPRINTF_ONLY)
  va_start(ap, fmt);
#endif
  if (!p) p = "";
  while (*p) {
    if (*p != '%') {
   /* if (str_l < str_m) str[str_l++] = *p++;    -- this would be sufficient */
   /* but the following code achieves better performance for cases
    * where format string is long and contains few conversions */
      const char *q = strchr(p+1,'%');
      int n = !q ? strlen(p) : (q-p);
      int avail = (int)(str_m-str_l);
      if (avail > 0) {
        register int k; register char *r; register const char* p1;
        for (p1=p, r=str+str_l, k=(n>avail?avail:n); k>0; k--) *r++ = *p1++;
      }
      p += n; str_l += n;
    } else {
      const char *starting_p;
      int min_field_width = 0, precision = 0;
      int zero_padding = 0, precision_specified = 0, justify_left = 0;
      int alternative_form = 0, force_sign = 0;
      int space_for_positive = 1; /* If both the ' ' and '+' flags appear,
                                     the ' ' flag should be ignored. */
      char data_type_modifier = '\0';      /* allowed valued: \0, h, l, L, p */
      char tmp[32];/* temporary buffer for simple numeric->string conversion */

      const char *str_arg = 0;/* string address in case of string arguments  */
      int str_arg_l;  /* natural field width of arg without padding and sign */

      long int long_arg;  /* long int argument value - always defined
        in case of numeric arguments, regardless of data type modifiers.
        In case of data type modifier 'll' the value is stored in long_long_arg
        and only the sign of long_arg is guaranteed to be correct */
      void *ptr_arg; /* pointer argument value - only defined for p format   */
      int int_arg;   /* int argument value - only defined if no h or l modif.*/
#ifdef SNPRINTF_LONGLONG_SUPPORT
      long long int long_long_arg = 0;  /* long long argument value - only
                                           defined if ll modifier is present */
#endif
      int number_of_zeros_to_pad = 0;
      int zero_padding_insertion_ind = 0;
      char fmt_spec = '\0';            /* current format specifier character */

      starting_p = p; p++;  /* skip '%' */
   /* parse flags */
      while (*p == '0' || *p == '-' || *p == '+' ||
             *p == ' ' || *p == '#' || *p == '\'') {
        switch (*p) {
        case '0': zero_padding = 1; break;
        case '-': justify_left = 1; break;
        case '+': force_sign = 1; space_for_positive = 0; break;
        case ' ': force_sign = 1;
     /* If both the ' ' and '+' flags appear, the ' ' flag should be ignored */
#ifdef PERL_COMPATIBLE
     /* ... but in Perl the last of ' ' and '+' applies */
                  space_for_positive = 1;
#endif
                  break;
        case '#': alternative_form = 1; break;
        case '\'': break;
        }
        p++;
      }
   /* If the '0' and '-' flags both appear, the '0' flag should be ignored. */

   /* parse field width */
      if (*p == '*') {
        p++; min_field_width = va_arg(ap, int);
        if (min_field_width < 0)
          { min_field_width = -min_field_width; justify_left = 1; }
      } else if (isdigit((int)(*p))) {
        min_field_width = *p++ - '0';
        while (isdigit((int)(*p)))
          min_field_width = 10*min_field_width + (*p++ - '0');
      }
   /* parse precision */
      if (*p == '.') {
        p++; precision_specified = 1;
        if (*p == '*') {
          p++; precision = va_arg(ap, int);
          if (precision < 0) {
            precision_specified = 0; precision = 0;
         /* NOTE:
          *   Solaris 2.6 man page claims that in this case the precision
          *   should be set to 0.  Digital Unix 4.0 and HPUX 10 man page
          *   claim that this case should be treated as unspecified precision,
          *   which is what we do here.
          */
          }
        } else if (isdigit((int)(*p))) {
          precision = *p++ - '0';
          while (isdigit((int)(*p))) precision = 10*precision + (*p++ - '0');
        }
      }
   /* parse 'h', 'l' and 'll' data type modifiers */
      if (*p == 'h' || *p == 'l') {
        data_type_modifier = *p; p++;
        if (data_type_modifier == 'l' && *p == 'l') {/* double l = long long */
#ifdef SNPRINTF_LONGLONG_SUPPORT
          data_type_modifier = '2';               /* double l encoded as '2' */
#else
          data_type_modifier = 'l';                /* treat it as single 'l' */
#endif
          p++;
        }
      }
      fmt_spec = *p;
   /* common synonyms: */
      switch (fmt_spec) {
      case 'i': fmt_spec = 'd'; break;
      case 'D': fmt_spec = 'd'; data_type_modifier = 'l'; break;
      case 'U': fmt_spec = 'u'; data_type_modifier = 'l'; break;
      case 'O': fmt_spec = 'o'; data_type_modifier = 'l'; break;
      default: break;
      }
   /* get parameter value, do initial processing */
      switch (fmt_spec) {
      case '%': /* % behaves similar to 's' regarding flags and field widths */
      case 'c': /* c behaves similar to 's' regarding flags and field widths */
      case 's':
        data_type_modifier = '\0';       /* wint_t and wchar_t not supported */
     /* the result of zero padding flag with non-numeric format is undefined */
     /* Solaris and HPUX 10 does zero padding in this case, Digital Unix not */
#ifdef DIGITAL_UNIX_COMPATIBLE
        zero_padding = 0;        /* turn zero padding off for string formats */
#endif
        str_arg_l = 1;
        switch (fmt_spec) {
        case '%':
          str_arg = p; break;
        case 'c':
          { int j = va_arg(ap, int); str_arg = (const char*) &j; }
          break;
        case 's':
          str_arg = va_arg(ap, const char *);
          if (!str_arg) str_arg_l = 0;
       /* make sure not to address string beyond the specified precision !!! */
          else if (!precision_specified) str_arg_l = strlen(str_arg);
       /* truncate string if necessary as requested by precision */
          else if (precision <= 0) str_arg_l = 0;
          else {
            const char *q = memchr(str_arg,'\0',(size_t)precision);
            str_arg_l = !q ? precision : (q-str_arg);
          }
          break;
        default: break;
        }
        break;
      case 'd': case 'o': case 'u': case 'x': case 'X': case 'p':
        long_arg = 0; int_arg = 0; ptr_arg = NULL;
        if (fmt_spec == 'p') {
        /* HPUX 10: An l, h, ll or L before any other conversion character
         *   (other than d, i, o, u, x, or X) is ignored.
         * Digital Unix:
         *   not specified, but seems to behave as HPUX does.
         * Solaris: If an h, l, or L appears before any other conversion
         *   specifier (other than d, i, o, u, x, or X), the behavior
         *   is undefined. (Actually %hp converts only 16-bits of address
         *   and %llp treats address as 64-bit data which is incompatible
         *   with (void *) argument on a 32-bit system).
         */
#ifdef SOLARIS_COMPATIBLE
#  ifdef SOLARIS_BUG_COMPATIBLE
          /* keep data type modifiers even if it represents 'll' */
#  else
          if (data_type_modifier == '2') data_type_modifier = '\0';
#  endif
#else
          data_type_modifier = '\0';
#endif
          ptr_arg = va_arg(ap, void *); long_arg = !ptr_arg ? 0 : 1;
        } else {
          switch (data_type_modifier) {
          case '\0':
          case 'h':
         /* It is non-portable to specify a second argument of char or short
          * to va_arg, because arguments seen by the called function
          * are not char or short.  C converts char and short arguments
          * to int before passing them to a function.
          */
            int_arg = va_arg(ap, int); long_arg = int_arg; break;
          case 'l':
            long_arg = va_arg(ap, long int); break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
          case '2':
            long_long_arg = va_arg(ap, long long int);
            /* only the sign of long_arg is guaranteed */
            if      (long_long_arg > 0) long_arg = +1;
            else if (long_long_arg < 0) long_arg = -1;
            else long_arg = 0;
            break;
#endif
          }
        }
        str_arg = tmp; str_arg_l = 0;
     /* NOTE:
      *   For d, i, o, u, x, and X conversions, if precision is specified,
      *   the '0' flag should be ignored. This is so with Solaris 2.6,
      *   Digital UNIX 4.0 and HPUX 10;  but not with Perl.
      */
#ifndef PERL_COMPATIBLE
        if (precision_specified) zero_padding = 0;
#endif
        if (fmt_spec == 'd') {
          if (force_sign && long_arg >= 0)
            tmp[str_arg_l++] = space_for_positive ? ' ' : '+';
         /* leave negative numbers for sprintf to handle,
            to avoid handling tricky cases like (short int)(-32768) */
        } else if (alternative_form) {
          if (long_arg != 0 && (fmt_spec == 'x' || fmt_spec == 'X') )
            { tmp[str_arg_l++] = '0'; tmp[str_arg_l++] = fmt_spec; }
#ifdef HPUX_COMPATIBLE
          else if (fmt_spec == 'p'
         /* HPUX 10: for an alternative form of p conversion,
          *          a nonzero result is prefixed by 0x. */
#ifndef HPUX_BUG_COMPATIBLE
         /* Actually it uses 0x prefix even for a zero value. */
                   && long_arg != 0
#endif
                 ) { tmp[str_arg_l++] = '0'; tmp[str_arg_l++] = 'x'; }
#endif
        }
        zero_padding_insertion_ind = str_arg_l;
        if (!precision_specified) precision = 1;   /* default precision is 1 */
        if (precision == 0 && long_arg == 0
#ifdef HPUX_BUG_COMPATIBLE
            && fmt_spec != 'p'
         /* HPUX 10 man page claims: With conversion character p the result of
          * converting a zero value with a precision of zero is a null string.
          * Actually it returns all zeroes. */
#endif
       ) {  /* converted to null string */  }
        else {
          char f[5]; int f_l = 0;
          f[f_l++] = '%';
          if (!data_type_modifier) { }
          else if (data_type_modifier=='2') { f[f_l++] = 'l'; f[f_l++] = 'l'; }
          else f[f_l++] = data_type_modifier;
          f[f_l++] = fmt_spec; f[f_l++] = '\0';
          if (fmt_spec == 'p') str_arg_l+=sprintf(tmp+str_arg_l, f, ptr_arg);
          else {
            switch (data_type_modifier) {
            case '\0':
            case 'h': str_arg_l+=sprintf(tmp+str_arg_l, f, int_arg);  break;
            case 'l': str_arg_l+=sprintf(tmp+str_arg_l, f, long_arg); break;
#ifdef SNPRINTF_LONGLONG_SUPPORT
            case '2': str_arg_l+=sprintf(tmp+str_arg_l,f,long_long_arg); break;
#endif
            }
          }
          if (zero_padding_insertion_ind < str_arg_l &&
              tmp[zero_padding_insertion_ind] == '-')
            zero_padding_insertion_ind++;
        }
        { int num_of_digits = str_arg_l - zero_padding_insertion_ind;
          if (alternative_form && fmt_spec == 'o'
#ifdef HPUX_COMPATIBLE                                  /* ("%#.o",0) -> ""  */
              && (str_arg_l > 0)
#endif
#ifdef DIGITAL_UNIX_BUG_COMPATIBLE                      /* ("%#o",0) -> "00" */
#else
              && !(zero_padding_insertion_ind < str_arg_l
                   && tmp[zero_padding_insertion_ind] == '0')
#endif
         ) {      /* assure leading zero for alternative-form octal numbers */
            if (!precision_specified || precision < num_of_digits+1)
              { precision = num_of_digits+1; precision_specified = 1; }
          }
       /* zero padding to specified precision? */
          if (num_of_digits < precision)
            number_of_zeros_to_pad = precision - num_of_digits;
        }
     /* zero padding to specified minimal field width? */
        if (!justify_left && zero_padding) {
          int n = min_field_width - (str_arg_l+number_of_zeros_to_pad);
          if (n > 0) number_of_zeros_to_pad += n;
        }
        break;
      default:  /* unrecognized format, keep format string unchanged */
        zero_padding = 0;   /* turn zero padding off for non-numeric formats */
#ifndef DIGITAL_UNIX_COMPATIBLE
        justify_left = 1; min_field_width = 0;                /* reset flags */
#endif
#ifdef PERL_COMPATIBLE
     /* keep the entire format string unchanged */
        str_arg = starting_p; str_arg_l = p - starting_p;
#else
     /* discard the unrecognized format, just keep the unrecognized fmt char */
        str_arg = p; str_arg_l = 0;
#endif
        if (*p) str_arg_l++;  /* include invalid fmt specifier if not at EOS */
        break;
      }
      if (*p) p++;          /* step over the just processed format specifier */
   /* insert padding to the left as requested by min_field_width */
      if (!justify_left) {                /* left padding with blank or zero */
        int n = min_field_width - (str_arg_l+number_of_zeros_to_pad);
        if (n > 0) {
          int avail = (int)(str_m-str_l);
          if (avail > 0) {      /* memset(str+str_l, zp, (n>avail?avail:n)); */
            const char zp = (zero_padding ? '0' : ' ');
            register int k; register char *r;
            for (r=str+str_l, k=(n>avail?avail:n); k>0; k--) *r++ = zp;
          }
          str_l += n;
        }
      }
   /* zero padding as requested by the precision for numeric formats requred?*/
      if (number_of_zeros_to_pad <= 0) {
     /* will not copy first part of numeric here,   *
      * force it to be copied later in its entirety */
        zero_padding_insertion_ind = 0;
      } else {
     /* insert first part of numerics (sign or '0x') before zero padding */
        int n = zero_padding_insertion_ind;
        if (n > 0) {
          int avail = (int)(str_m-str_l);
          if (avail > 0) memcpy(str+str_l, str_arg, (size_t)(n>avail?avail:n));
          str_l += n;
        }
     /* insert zero padding as requested by the precision */
        n = number_of_zeros_to_pad;
        if (n > 0) {
          int avail = (int)(str_m-str_l);
          if (avail > 0) {     /* memset(str+str_l, '0', (n>avail?avail:n)); */
            register int k; register char *r;
            for (r=str+str_l, k=(n>avail?avail:n); k>0; k--) *r++ = '0';
          }
          str_l += n;
        }
      }
   /* insert formatted string (or unmodified format for unknown formats) */
      { int n = str_arg_l - zero_padding_insertion_ind;
        if (n > 0) {
          int avail = (int)(str_m-str_l);
          if (avail > 0) memcpy(str+str_l, str_arg+zero_padding_insertion_ind,
                                (size_t)(n>avail ? avail : n) );
          str_l += n;
        }
      }
   /* insert right padding */
      if (justify_left) {          /* right blank padding to the field width */
        int n = min_field_width - (str_arg_l+number_of_zeros_to_pad);
        if (n > 0) {
          int avail = (int)(str_m-str_l);
          if (avail > 0) {     /* memset(str+str_l, ' ', (n>avail?avail:n)); */
            register int k; register char *r;
            for (r=str+str_l, k=(n>avail?avail:n); k>0; k--) *r++ = ' ';
          }
          str_l += n;
        }
      }
    }
  }
#if defined(NEED_SNPRINTF_ONLY)
  va_end(ap);
#endif
  if (str_m > 0) { /* make sure the string is null-terminated
                      even at the expense of overwriting the last character */
    str[str_l <= str_m-1 ? str_l : str_m-1] = '\0';
  }

  return str_l;    /* return the number of characters formatted
                      (excluding trailing null character),
                      that is, the number of characters that would have been
                      written to the buffer if it were large enough */
}
#endif
