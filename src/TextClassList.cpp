/**
 * \file TextClassList.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "TextClassList.h"
#include "TextClass.h"
#include "debug.h"
#include "Lexer.h"

#include "support/filetools.h"

#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>


namespace lyx {
namespace fs = boost::filesystem;

using support::FileName;
using support::addName;
using support::libFileSearch;
using support::makeDisplayPath;

using boost::bind;
using boost::regex;
using boost::smatch;

using std::endl;
using std::equal_to;
using std::find_if;
using std::make_pair;
using std::sort;
using std::string;
using std::pair;
using std::ifstream;


// Gets textclass number from name
pair<bool, textclass_type> const
TextClassList::numberOfClass(string const & textclass) const
{
	ClassList::const_iterator cit =
		find_if(classlist_.begin(), classlist_.end(),
			bind(equal_to<string>(),
			     bind(&TextClass::name, _1),
			     textclass));

	return cit != classlist_.end() ?
		make_pair(true, textclass_type(cit - classlist_.begin())) :
		make_pair(false, textclass_type(0));
}


// Gets a textclass structure from number
TextClass const &
TextClassList::operator[](textclass_type textclass) const
{
	classlist_[textclass].load();
	if (textclass < classlist_.size())
		return classlist_[textclass];
	else
		return classlist_[0];
}


// used when sorting the textclass list.
class less_textclass_avail_desc
	: public std::binary_function<TextClass, TextClass, int>
{
public:
	int operator()(TextClass const & tc1,
		       TextClass const & tc2) const
	{
		// Ordering criteria:
		//   1. Availability of text class
		//   2. Description (lexicographic)

		return (tc1.isTeXClassAvailable() && !tc2.isTeXClassAvailable()) ||
			(tc1.isTeXClassAvailable() == tc2.isTeXClassAvailable() &&
			 tc1.description() < tc2.description());
	}
};


// Reads LyX textclass definitions according to textclass config file
bool TextClassList::read()
{
	Lexer lex(0, 0);
	support::FileName const real_file = libFileSearch("", "textclass.lst");
	LYXERR(Debug::TCLASS) << "Reading textclasses from `"
			      << real_file << '\'' << endl;

	if (real_file.empty()) {
		lyxerr << "TextClassList::Read: unable to find "
		          "textclass file  `"
		       << to_utf8(makeDisplayPath(real_file.absFilename(), 1000))
		       << "'. Exiting." << endl;
		return false;
		// This causes LyX to end... Not a desirable behaviour. Lgb
		// What do you propose? That the user gets a file dialog
		// and is allowed to hunt for the file? (Asger)
		// more that we have a layout for minimal.cls statically
		// compiled in... (Lgb)
	}

	if (!lex.setFile(real_file)) {
		lyxerr << "TextClassList::Read: "
			"lyxlex was not able to set file: "
		       << real_file << endl;
	}

	if (!lex.isOK()) {
		lyxerr << "TextClassList::Read: unable to open "
		          "textclass file  `"
		       << to_utf8(makeDisplayPath(real_file.absFilename(), 1000))
		       << "'\nCheck your installation. LyX can't continue."
		       << endl;
		return false;
	}

	bool finished = false;
	// Parse config-file
	LYXERR(Debug::TCLASS) << "Starting parsing of textclass.lst" << endl;
	while (lex.isOK() && !finished) {
		LYXERR(Debug::TCLASS) << "\tline by line" << endl;
		switch (lex.lex()) {
		case Lexer::LEX_FEOF:
			finished = true;
			break;
		default:
			string const fname = lex.getString();
			LYXERR(Debug::TCLASS) << "Fname: " << fname << endl;
			if (lex.next()) {
				string const clname = lex.getString();
				LYXERR(Debug::TCLASS) << "Clname: " << clname << endl;
				if (lex.next()) {
					string const desc = lex.getString();
					LYXERR(Debug::TCLASS) << "Desc: " << desc << endl;
					if (lex.next()) {
						bool avail = lex.getBool();
						LYXERR(Debug::TCLASS) << "Avail: " << avail << endl;
						// This code is run when we have
						// fname, clname, desc, and avail
						TextClass tmpl(fname, clname, desc, avail);
						if (lyxerr.debugging(Debug::TCLASS)) {
							tmpl.load();
						}
						classlist_.push_back(tmpl);
					}
				}
			}
		}
	}
	LYXERR(Debug::TCLASS) << "End of parsing of textclass.lst" << endl;

	if (classlist_.empty()) {
		lyxerr << "TextClassList::Read: no textclasses found!"
		       << endl;
		return false;
	}
	// Ok everything loaded ok, now sort the list.
	sort(classlist_.begin(), classlist_.end(), less_textclass_avail_desc());
	return true;
}


std::pair<bool, textclass_type> const
TextClassList::addTextClass(std::string const & textclass, std::string const & path)
{
	// only check for textclass.layout file, .cls can be anywhere in $TEXINPUTS
	// NOTE: latex class name is defined in textclass.layout, which can be different from textclass
	FileName const layout_file(addName(path, textclass + ".layout"));
	if (fs::exists(layout_file.toFilesystemEncoding())) {
		LYXERR(Debug::TCLASS) << "Adding class " << textclass << " from directory " << path << endl;
		// Read .layout file and get description, real latex classname etc
		//
		// This is a C++ version of function processLayoutFile in configure.py,
		// which uses the following regex
		//     \Declare(LaTeX|DocBook)Class\s*(\[([^,]*)(,.*)*\])*\s*{(.*)}
		ifstream ifs(layout_file.toFilesystemEncoding().c_str());
		static regex const reg("^#\\s*\\\\Declare(LaTeX|DocBook)Class\\s*"
			"(?:\\[([^,]*)(?:,.*)*\\])*\\s*\\{(.*)\\}\\s*");
		string line;
		while (getline(ifs, line)) {
			// look for the \DeclareXXXClass line
			smatch sub;
			if (regex_match(line, sub, reg)) {
				// returns: whole string, classtype (not used here), first option, description
				BOOST_ASSERT(sub.size()==4);
				// now, add the layout to textclass.
				TextClass tmpl(textclass, sub.str(2)==""?textclass:sub.str(2), 
					sub.str(3) + " <" + path + ">", true);
				if (lyxerr.debugging(Debug::TCLASS))
					tmpl.load(path);
				classlist_.push_back(tmpl);
				return make_pair(true, classlist_.size() - 1);
			}
		}
	}
	// If .layout is not in local directory, or an invalid layout is found, return false
	return make_pair(false, textclass_type(0));
}
	

// Global variable: textclass table.
TextClassList textclasslist;


// Reads the style files
bool LyXSetStyle()
{
	LYXERR(Debug::TCLASS) << "LyXSetStyle: parsing configuration..." << endl;

	if (!textclasslist.read()) {
		LYXERR(Debug::TCLASS) << "LyXSetStyle: an error occured "
			"during parsing.\n             Exiting." << endl;
		return false;
	}

	LYXERR(Debug::TCLASS) << "LyXSetStyle: configuration parsed." << endl;
	return true;
}


} // namespace lyx
