// -*- C++ -*-
/**
 * \file paragraph_funcs.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS
 */

#ifndef PARAGRAPH_FUNCS_H
#define PARAGRAPH_FUNCS_H

#include "support/types.h"

class Buffer;
class BufferParams;
class Paragraph;
class TexRow;

///
void breakParagraph(BufferParams const & bparams,
		    Paragraph *,
		    lyx::pos_type pos,
		    int flag);

///
void breakParagraphConservative(BufferParams const & bparams,
				Paragraph *,
				lyx::pos_type pos);

/**
 * Append the next paragraph onto the tail of this one.
 * Be careful, this doesent make any check at all.
 */
void mergeParagraph(BufferParams const & bparams, Paragraph *);


#if 0
/// for the environments
Paragraph * depthHook(Paragraph * par, Paragraph::depth_type depth);

Paragraph * outerHook(Paragraph * par);

/// Is it the first par with same depth and layout?
bool isFirstInSequence(Paragraph * par);

/** Check if the current paragraph is the last paragraph in a
    proof environment */
int getEndLabel(Paragraph * para, BufferParams const & bparams);
#endif

Paragraph * TeXDeeper(Buffer const * buf,
		      BufferParams const & bparams,
		      Paragraph * pit,
		      std::ostream & os, TexRow & texrow);

Paragraph * TeXEnvironment(Buffer const * buf,
			   BufferParams const & bparams,
			   Paragraph * pit,
			   std::ostream & os, TexRow & texrow);

Paragraph * TeXOnePar(Buffer const * buf,
		      BufferParams const & bparams,
		      Paragraph * pit,
		      std::ostream & os, TexRow & texrow,
		      bool moving_arg);

#endif // PARAGRAPH_FUNCS_H
