// -*- C++ -*-
/**
 * \file outputparams.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 *  \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef OUTPUTPARAMS_H
#define OUTPUTPARAMS_H

#include <string>

#include "support/types.h"
#include <boost/shared_ptr.hpp>


class ExportData;
class LyXFont;


class OutputParams {
public:
	//
	enum FLAVOR {
		LATEX,
		PDFLATEX,
		XML
	};

	OutputParams();
	~OutputParams();

	/** The latex that we export depends occasionally on what is to
	    compile the file.
	*/
	FLAVOR flavor;

	/** Are we to write a 'nice' LaTeX file or not.
	    This esentially seems to mean whether InsetInclude, InsetGraphics
	    and InsetExternal should add the absolute path to any external
	    files or not.
	*/
	bool nice;

	/** moving_arg == true means that the environment in which the inset
	    is typeset is a moving argument. The inset should take care about
	    fragile commands by preceding the latex with \\protect.
	*/
	bool moving_arg;

	/** intitle == true means that the environment in which the
	    inset is typeset is part of a title (before a \\maketitle).
	    Footnotes in such environments have moving arguments.
	*/
	bool intitle;

	/** the font at the point where the inset is
	 */
	LyXFont const * local_font;

	/** Document language babel name
	 */
	mutable std::string document_language;

	/** free_spacing == true means that the inset is in a free-spacing
	    paragraph.
	*/
	bool free_spacing;

	/** This var is set by the return value from BufferParams::writeLaTeX
	*/
	bool use_babel;

	/** Line length to use with plaintext export.
	*/
	lyx::size_type linelen;

	/** The depth of the current paragraph, set for plaintext
	 *  export and used by InsetTabular
	 */
	int depth;

	/** Export data filled in by the latex(), docbook() etc methods.
	    This is a hack: Make it possible to add stuff to constant
	    OutputParams instances.
	*/
	boost::shared_ptr<ExportData> exportdata;

	/** Whether we are inside a comment inset. Insets that are including
	 *  external files like InsetGraphics, InsetInclude and InsetExternal
	 *  may only write the usual output and must not attempt to do
	 *  something with the included files (e.g. copying, converting)
	 *  if this flag is true, since they may not exist.
	 */
	bool inComment;
};

#endif // NOT OUTPUTPARAMS_H
