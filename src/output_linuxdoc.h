// -*- C++ -*-
/**
 * \file output_linuxdoc.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jos� Matos
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef OUTPUT_LINUXDOC_H
#define OUTPUT_LINUXDOC_H

#include <iosfwd>

class Buffer;
class ParagraphList;
class OutputParams;

///
void linuxdocParagraphs(Buffer const & buf,
			ParagraphList const & paragraphs,
			std::ostream & os,
			OutputParams const & runparams);
	

#endif
