/**
 * \file output_plaintext.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "output_plaintext.h"

#include "Buffer.h"
#include "BufferParams.h"
#include "debug.h"
#include "gettext.h"
#include "output.h"
#include "OutputParams.h"
#include "Paragraph.h"
#include "ParagraphList.h"
#include "ParagraphParameters.h"

#include "support/lstrings.h"


namespace lyx {

using support::ascii_lowercase;
using support::compare_ascii_no_case;
using support::contains;
using support::FileName;

using std::endl;
using std::ostream;
using std::pair;
using std::string;


void writePlaintextFile(Buffer const & buf, FileName const & fname,
	OutputParams const & runparams)
{
	odocfstream ofs;
	if (!openFileWrite(ofs, fname))
		return;
	writePlaintextFile(buf, ofs, runparams);
}


void writePlaintextFile(Buffer const & buf, odocstream & os,
	OutputParams const & runparams)
{
	bool ref_printed = false;
	ParagraphList const par = buf.paragraphs();
	ParagraphList::const_iterator beg = par.begin();
	ParagraphList::const_iterator end = par.end();
	ParagraphList::const_iterator it = beg;
	for (; it != end; ++it) {
		writePlaintextParagraph(buf, *it, os, runparams, ref_printed);
		os << "\n";
		if (runparams.linelen > 0)
			os << "\n";
	}
}


namespace {

pair<int, docstring> const addDepth(int depth, int ldepth)
{
	int d = depth * 2;
	if (ldepth > depth)
		d += (ldepth - depth) * 2;
	return make_pair(d, docstring(d, ' '));
}

}


void writePlaintextParagraph(Buffer const & buf,
		    Paragraph const & par,
		    odocstream & os,
		    OutputParams const & runparams,
		    bool & ref_printed)
{
	int ltype = 0;
	depth_type ltype_depth = 0;
	depth_type depth = par.params().depth();

	// First write the layout
	string const & tmp = par.layout()->name();
	if (compare_ascii_no_case(tmp, "itemize") == 0) {
		ltype = 1;
		ltype_depth = depth + 1;
	} else if (compare_ascii_no_case(tmp, "enumerate") == 0) {
		ltype = 2;
		ltype_depth = depth + 1;
	} else if (contains(ascii_lowercase(tmp), "ection")) {
		ltype = 3;
		ltype_depth = depth + 1;
	} else if (contains(ascii_lowercase(tmp), "aragraph")) {
		ltype = 4;
		ltype_depth = depth + 1;
	} else if (compare_ascii_no_case(tmp, "description") == 0) {
		ltype = 5;
		ltype_depth = depth + 1;
	} else if (compare_ascii_no_case(tmp, "abstract") == 0) {
		ltype = 6;
		ltype_depth = 0;
	} else if (compare_ascii_no_case(tmp, "bibliography") == 0) {
		ltype = 7;
		ltype_depth = 0;
	} else {
		ltype = 0;
		ltype_depth = 0;
	}

	/* the labelwidthstring used in lists */

	/* noindent ? */

	/* what about the alignment */

	// runparams.linelen == 0 is special and means we don't have paragraph breaks

	string::size_type currlinelen = 0;

	os << docstring(depth * 2, ' ');
	currlinelen += depth * 2;

	//--
	// we should probably change to the paragraph language in the
	// gettext here (if possible) so that strings are output in
	// the correct language! (20012712 Jug)
	//--
	switch (ltype) {
	case 0: // Standard
	case 4: // (Sub)Paragraph
	case 5: // Description
		break;

	case 6: // Abstract
		if (runparams.linelen > 0) {
			os << _("Abstract") << "\n\n";
			currlinelen = 0;
		} else {
			docstring const abst = _("Abstract: ");
			os << abst;
			currlinelen += abst.length();
		}
		break;

	case 7: // Bibliography
		if (!ref_printed) {
			if (runparams.linelen > 0) {
				os << _("References") << "\n\n";
				currlinelen = 0;
			} else {
				docstring const refs = _("References: ");
				os << refs;
				currlinelen += refs.length();
			}
			ref_printed = true;
		}
		break;

	default: {
		docstring const label = par.params().labelString();
		if (!label.empty()) {
			os << label << ' ';
			currlinelen += label.length() + 1;
		}
		break;
	}

	}

	if (currlinelen == 0) {
		pair<int, docstring> p = addDepth(depth, ltype_depth);
		os << p.second;
		currlinelen += p.first;
	}

	docstring word;

	for (pos_type i = 0; i < par.size(); ++i) {
		// deleted characters don't make much sense in plain text output
		if (par.isDeleted(i))
			continue;

		char_type c = par.getUChar(buf.params(), i);

		if (c == Paragraph::META_INSET || c == ' ') {
			if (runparams.linelen > 0 &&
			    currlinelen + word.length() > runparams.linelen) {
				os << '\n';
				pair<int, docstring> p = addDepth(depth, ltype_depth);
				os << p.second;
				currlinelen = p.first;
			}
			os << word;
			currlinelen += word.length();
			word.erase();
		}

		switch (c) {
		case Paragraph::META_INSET: {
			OutputParams rp = runparams;
			rp.depth = par.params().depth();
			int len = par.getInset(i)->plaintext(buf, os, rp);
			if (len >= Inset::PLAINTEXT_NEWLINE)
				currlinelen = len - Inset::PLAINTEXT_NEWLINE;
			else
				currlinelen += len;
			break;
		}

		case ' ':
			os << ' ';
			currlinelen++;
			break;

		case '\0':
			LYXERR(Debug::INFO) <<
				"writePlaintextFile: NULL char in structure." << endl;
			break;

		default:
			word += c;
			break;
		}
	}

	// currlinelen may be greater than runparams.linelen!
	// => check whether word is empty and do nothing in this case
	if (!word.empty()) {
		if (runparams.linelen > 0 &&
		    currlinelen + word.length() > runparams.linelen) {
			os << '\n';
			pair<int, docstring> p = addDepth(depth, ltype_depth);
			os << p.second;
			currlinelen = p.first;
		}
		os << word;
	}
}


} // namespace lyx
