// -*- C++ -*-
/**
 * \file output_latex.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef OUTPUT_LATEX_H
#define OUTPUT_LATEX_H

#include "support/docstream.h"


namespace lyx {

class Buffer;
class BufferParams;
class Encoding;
class Paragraph;
class ParagraphList;
class OutputParams;
class TexRow;

/// Export up to \p number optarg insets
int latexOptArgInsets(Buffer const & buf, Paragraph const & par,
		      odocstream & os, OutputParams const & runparams,
		      int number);

/** Export \p paragraphs of buffer \p buf to LaTeX.
    Don't use a temporary stringstream for \p os if the final output is
    supposed to go to a file.
    \sa Buffer::writeLaTeXSource for the reason.
 */
void latexParagraphs(Buffer const & buf,
		     ParagraphList const & paragraphs,
		     odocstream & ofs,
		     TexRow & texrow,
		     OutputParams const &,
		     std::string const & everypar = std::string());

/// Switch the encoding of \p os from \p oldEnc to \p newEnc if needed.
/// \return the number of characters written to \p os.
int switchEncoding(odocstream & os, BufferParams const & bparams,
		   bool moving_arg, Encoding const & oldEnc,
		   Encoding const & newEnc);

} // namespace lyx

#endif
