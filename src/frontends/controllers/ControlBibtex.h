// -*- C++ -*-
/**
 * \file ControlBibtex.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author John Levon
 * \author Angus Leeming
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef CONTROLBIBTEX_H
#define CONTROLBIBTEX_H

#include "ControlCommand.h"

#include "support/docstring.h"

#include <vector>

namespace lyx {

namespace support { class FileFilterList; }


namespace frontend {

/** A controller for Bibtex dialogs.
 */
class ControlBibtex : public ControlCommand {
public:
	///
	ControlBibtex(Dialog &);

	/// Browse for a .bib file
	docstring const browseBib(docstring const & in_name) const;

	/// Browse for a .bst file
	docstring const browseBst(docstring const & in_name) const;

	/// get the list of bst files
	void getBibStyles(std::vector<std::string> & data) const;
	/// get the list of bib files
	void getBibFiles(std::vector<std::string> & data) const;
	/// build filelists of all availabe bib/bst/cls/sty-files. done through
	/// kpsewhich and an external script, saved in *Files.lst
	void rescanBibStyles() const;
	/// do we use bibtopic (for sectioned bibliography)?
	bool usingBibtopic() const;
	/// should we put the bibliography to the TOC?
	bool bibtotoc() const;
	/// which stylefile do we use?
	std::string const getStylefile() const;
};

} // namespace frontend
} // namespace lyx

#endif // CONTROLBIBTEX_H
