// Baruch Even  baruch@ev-en.org  2001-02-20
//  This version is a modified version for use in LyX
//  The modifications are done in order to use it where exceptions are disabled
//  Currently it has no "no memory" checks in place, asserts are probably the
//  only real way.
//  all changed are #ifded'ed by LYX_NO_EXCEPTIONS
#define LYX_NO_EXCEPTIONS

//  Boost smart_ptr.hpp header file  -----------------------------------------//

//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999. Permission to copy,
//  use, modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided "as is"
//  without express or implied warranty, and with no claim as to its
//  suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.

//  Revision History
//  21 May 01  Require complete type where incomplete type is unsafe.
//             (suggested by Vladimir Prus)
//  21 May 01  operator= fails if operand transitively owned by *this, as in a
//             linked list (report by Ken Johnson, fix by Beman Dawes)
//  21 Jan 01  Suppress some useless warnings with MSVC (David Abrahams)
//  19 Oct 00  Make shared_ptr ctor from auto_ptr explicit. (Robert Vugts) 
//  24 Jul 00  Change throw() to // never throws.  See lib guidelines
//             Exception-specification rationale. (Beman Dawes)
//  22 Jun 00  Remove #if continuations to fix GCC 2.95.2 problem (Beman Dawes)
//   1 Feb 00  Additional shared_ptr BOOST_NO_MEMBER_TEMPLATES workarounds
//             (Dave Abrahams)
//  31 Dec 99  Condition tightened for no member template friend workaround
//             (Dave Abrahams)
//  30 Dec 99  Moved BOOST_NMEMBER_TEMPLATES compatibility code to config.hpp
//             (Dave Abrahams)
//  30 Nov 99  added operator ==, operator !=, and std::swap and std::less
//             specializations for shared types (Darin Adler)
//  11 Oct 99  replaced op[](int) with op[](std::size_t) (Ed Brey, Valentin
//             Bonnard), added shared_ptr workaround for no member template
//             friends (Matthew Langston)
//  25 Sep 99  added shared_ptr::swap and shared_array::swap (Luis Coelho).
//  20 Jul 99  changed name to smart_ptr.hpp, #include <boost/config.hpp>,
//             #include <boost/utility.hpp> and use boost::noncopyable
//  17 May 99  remove scoped_array and shared_array operator*() as
//             unnecessary (Beman Dawes)
//  14 May 99  reorder code so no effects when bad_alloc thrown (Abrahams/Dawes)
//  13 May 99  remove certain throw() specifiers to avoid generated try/catch
//             code cost (Beman Dawes)
//  11 May 99  get() added, conversion to T* placed in macro guard (Valentin
//             Bonnard, Dave Abrahams, and others argued for elimination
//             of the automatic conversion)
//  28 Apr 99  #include <memory> fix (Valentin Bonnard)
//  28 Apr 99  rename transfer() to share() for clarity (Dave Abrahams)
//  28 Apr 99  remove unsafe shared_array template conversions(Valentin Bonnard)
//  28 Apr 99  p(r) changed to p(r.px) for clarity (Dave Abrahams)
//  21 Apr 99  reset() self assignment fix (Valentin Bonnard)
//  21 Apr 99  dispose() provided to improve clarity (Valentin Bonnard)
//  27 Apr 99  leak when new throws fixes (Dave Abrahams)
//  21 Oct 98  initial Version (Greg Colvin/Beman Dawes)

#ifndef LYX_SMART_PTR_H
#define LYX_SMART_PTR_H

#include <boost/config.hpp>   // for broken compiler workarounds
#include <cstddef>            // for std::size_t
#include <memory>             // for std::auto_ptr
#include <algorithm>          // for std::swap
#include "support/utility.h"  // for boost::noncopyable, checked_delete, checked_array_delete
#include <functional>         // for std::less
#include <boost/static_assert.hpp> // for BOOST_STATIC_ASSERT

#ifdef LYX_NO_EXCEPTIONS
#include <assert.h>
#endif

namespace lyx {

//  shared_ptr  --------------------------------------------------------------//

//  An enhanced relative of scoped_ptr with reference counted copy semantics.
//  The object pointed to is deleted when the last shared_ptr pointing to it
//  is destroyed or reset.

template<typename T> class shared_c_ptr {
  public:
   typedef T element_type;

   explicit shared_c_ptr(T* p =0) : px(p) {
#ifndef LYX_NO_EXCEPTIONS
      try { pn = new long(1); }  // fix: prevent leak if new throws
      catch (...) { checked_delete(p); throw; } 
#else
	  pn = new long(1);
	  assert(pn != 0);
#endif
   }

   shared_c_ptr(const shared_c_ptr& r) : px(r.px) { ++*(pn = r.pn); }  // never throws

   ~shared_c_ptr() { dispose(); }

   shared_c_ptr& operator=(const shared_c_ptr& r) {
      share(r.px,r.pn);
      return *this;
   }

#if !defined( BOOST_NO_MEMBER_TEMPLATES )
   template<typename Y>
      shared_c_ptr(const shared_c_ptr<Y>& r) : px(r.px) {  // never throws 
         ++*(pn = r.pn); 
      }
#ifndef BOOST_NO_AUTO_PTR
   template<typename Y>
      explicit shared_c_ptr(std::auto_ptr<Y>& r) { 
         pn = new long(1); // may throw
         px = r.release(); // fix: moved here to stop leak if new throws
      }
#endif 

   template<typename Y>
      shared_c_ptr& operator=(const shared_c_ptr<Y>& r) { 
         share(r.px,r.pn);
         return *this;
      }

#ifndef BOOST_NO_AUTO_PTR
   template<typename Y>
      shared_c_ptr& operator=(std::auto_ptr<Y>& r) {
         // code choice driven by guarantee of "no effect if new throws"
         if (*pn == 1) { checked_delete(px); }
         else { // allocate new reference counter
           long * tmp = new long(1); // may throw
           --*pn; // only decrement once danger of new throwing is past
           pn = tmp;
         } // allocate new reference counter
         px = r.release(); // fix: moved here so doesn't leak if new throws 
         return *this;
      }
#endif
#else
#ifndef BOOST_NO_AUTO_PTR
      explicit shared_c_ptr(std::auto_ptr<T>& r) { 
         pn = new long(1); // may throw
         px = r.release(); // fix: moved here to stop leak if new throws
      } 

      shared_c_ptr& operator=(std::auto_ptr<T>& r) {
         // code choice driven by guarantee of "no effect if new throws"
         if (*pn == 1) { checked_delete(px); }
         else { // allocate new reference counter
           long * tmp = new long(1); // may throw
           --*pn; // only decrement once danger of new throwing is past
           pn = tmp;
         } // allocate new reference counter
         px = r.release(); // fix: moved here so doesn't leak if new throws 
         return *this;
      }
#endif
#endif

   void reset(T* p=0) {
      if ( px == p ) return;  // fix: self-assignment safe
      if (--*pn == 0) { checked_delete(px); }
      else { // allocate new reference counter
#ifndef LYX_NO_EXCEPTIONS		  
        try { pn = new long; }  // fix: prevent leak if new throws
        catch (...) {
          ++*pn;  // undo effect of --*pn above to meet effects guarantee 
          checked_delete(p);
          throw;
        } // catch
#else
		pn = new long;
		assert(pn != 0);
#endif
      } // allocate new reference counter
      *pn = 1;
      px = p;
   } // reset

   T& operator*() const          { return *px; }  // never throws
   T* operator->() const         { return px; }  // never throws
   T* get() const                { return px; }  // never throws

   long use_count() const        { return *pn; }  // never throws
   bool unique() const           { return *pn == 1; }  // never throws

   void swap(shared_c_ptr<T>& other)  // never throws
     { std::swap(px,other.px); std::swap(pn,other.pn); }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)
// Don't split this line into two; that causes problems for some GCC 2.95.2 builds
#if defined(BOOST_NO_MEMBER_TEMPLATES) || !defined( BOOST_NO_MEMBER_TEMPLATE_FRIENDS )
   private:
#endif

   T*     px;     // contained pointer
   long*  pn;     // ptr to reference counter

// Don't split this line into two; that causes problems for some GCC 2.95.2 builds
#if !defined( BOOST_NO_MEMBER_TEMPLATES ) && !defined( BOOST_NO_MEMBER_TEMPLATE_FRIENDS )
   template<typename Y> friend class shared_ptr;
#endif

   void dispose() { if (--*pn == 0) { checked_delete(px); delete pn; } }

   void share(T* rpx, long* rpn) {
      if (pn != rpn) { // Q: why not px != rpx? A: fails when both == 0
         ++*rpn; // done before dispose() in case rpn transitively
                 // dependent on *this (bug reported by Ken Johnson)
         dispose();
         px = rpx;
         pn = rpn;
      }
   } // share
};  // shared_ptr

template<typename T, typename U>
  inline bool operator==(const shared_c_ptr<T>& a, const shared_c_ptr<U>& b)
    { return a.get() == b.get(); }

template<typename T, typename U>
  inline bool operator!=(const shared_c_ptr<T>& a, const shared_c_ptr<U>& b)
    { return a.get() != b.get(); }

} // namespace boost

//  specializations for things in namespace std  -----------------------------//

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

namespace std {

// Specialize std::swap to use the fast, non-throwing swap that's provided
// as a member function instead of using the default algorithm which creates
// a temporary and uses assignment.

template<typename T>
  inline void swap(lyx::shared_c_ptr<T>& a, lyx::shared_c_ptr<T>& b)
    { a.swap(b); }

// Specialize std::less so we can use shared pointers and arrays as keys in
// associative collections.

// It's still a controversial question whether this is better than supplying
// a full range of comparison operators (<, >, <=, >=).

template<typename T>
  struct less< lyx::shared_c_ptr<T> >
    : binary_function<lyx::shared_c_ptr<T>, lyx::shared_c_ptr<T>, bool>
  {
    bool operator()(const lyx::shared_c_ptr<T>& a,
        const lyx::shared_c_ptr<T>& b) const
      { return less<T*>()(a.get(),b.get()); }
  };

} // namespace std

#endif  // ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // LYX_SMART_PTR_H


