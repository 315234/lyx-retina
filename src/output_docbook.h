// -*- C++ -*-
/**
 * \file output_docbook.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jos� Matos
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef OUTPUT_DOCBOOK_H
#define OUTPUT_DOCBOOK_H

#include <iosfwd>

class Buffer;
class OutputParams;
class ParagraphList;

///
void docbookParagraphs(ParagraphList const & subset,
		       Buffer const & buf,
		       std::ostream & os,
		       OutputParams const & runparams);
#endif
