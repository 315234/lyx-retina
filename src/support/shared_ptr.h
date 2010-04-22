// -*- C++ -*-
/**
 * \file shared_ptr.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Peter Kümmel
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LYX_SHARED_PTR_H
#define LYX_SHARED_PTR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1600)

#include <memory>

namespace lyx
{
	using std::tr1::shared_ptr;
}

#else

#include <boost/shared_ptr.hpp>

namespace lyx
{
	using boost::shared_ptr;
}

#endif


#endif
