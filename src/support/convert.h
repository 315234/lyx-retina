// -*- C++ -*-
/**
 * \file convert.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Andr� P�nitz
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 *
 * A collection of string helper functions that works with string.
 * Some of these would certainly benefit from a rewrite/optimization.
 */

#ifndef CONVERT_H
#define CONVERT_H

template <class Target, class Source>
Target convert(Source arg);

#endif
