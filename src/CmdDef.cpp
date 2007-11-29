/**
 * \file CmdDef.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Bernhard Roider
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "CmdDef.h"

#include "LyXAction.h"
#include "Lexer.h"

#include "support/debug.h"
#include "support/filetools.h"
#include "support/lstrings.h"

#include <ostream>

using std::endl;
using std::string;

namespace lyx {

using support::FileName;
using support::i18nLibFileSearch;
using support::trim;


namespace {

enum CmdDefTags {
	BN_DEFFILE,
	BN_DEFINE
};

keyword_item cmdDefTags[] = {
	{ "\\def_file", BN_DEFFILE },
	{ "\\define", BN_DEFINE }
};

}


bool CmdDef::read(string const & def_file)
{
	const int cmdDefCount = sizeof(cmdDefTags) / sizeof(keyword_item);

	Lexer lexrc(cmdDefTags, cmdDefCount);
	if (lyxerr.debugging(Debug::PARSER))
		lexrc.printTable(lyxerr);

	FileName const tmp(i18nLibFileSearch("commands", def_file, "def"));
	lexrc.setFile(tmp);
	if (!lexrc.isOK()) {
		lyxerr << "CmdDef::read: cannot open def file:"
		       << tmp << endl;
		return false;
	}

	//LYXERR(Debug::KBMAP, "Reading def file:" << tmp);

	bool error = false;
	while (lexrc.isOK()) {
		switch (lexrc.lex()) {
		case Lexer::LEX_UNDEF:
			lexrc.printError("Unknown tag `$$Token'");
			error = true;
			continue;
		case Lexer::LEX_FEOF:
			continue;
		case BN_DEFINE:
		{
			string name, def;

			if (lexrc.next()) {
				name = lexrc.getString();
			} else {
				lexrc.printError("BN_DEFINE: Missing command name");
				error = true;
				break;
			}

			if (lexrc.next(true)) {
				def = lexrc.getString();
			} else {
				lexrc.printError("BN_DEFINE: missing command definition");
				error = true;
				break;
			}

			newCmdDefResult e = newCmdDef(name, def);
			switch (e) {
				case CmdDefNameEmpty:
					lexrc.printError("BN_DEFINE: Command name is empty");
					error = true;
					break;
				case CmdDefExists:
					lexrc.printError("BN_DEFINE: Command `" + name + "' already defined");
					error = true;
					break;
				case CmdDefInvalid:
					lexrc.printError("BN_DEFINE: Command definition for `" + name + "' is not valid");
					error = true;
			}

			break;
		}
		case BN_DEFFILE:
			if (lexrc.next()) {
				string const tmp(lexrc.getString());
				error |= !read(tmp);
			} else {
				lexrc.printError("BN_DEFFILE: Missing file name");
				error = true;
				break;

			}
			break;
		}
	}

	if (error)
		lyxerr << "CmdDef::read: error while reading def file:"
		       << tmp << endl;
	return !error;
}


bool CmdDef::lock(string const & name, FuncRequest & func)
{
	if (cmdDefMap.empty())
	{
		func = FuncRequest::unknown;
		return false;
	}

	string const name2 = trim(name);

	CmdDefMap::const_iterator pos = cmdDefMap.find(name2);

	if (pos == cmdDefMap.end()) 
	{
		func = FuncRequest::unknown;
		return false;
	}

	if (pos->second->locked)
	{
		func = FuncRequest::noaction;
		return false;
	}

	pos->second->locked = true;
	func = pos->second->func;
	return true;
}


void CmdDef::release(string const & name)
{
	if (cmdDefMap.empty()) 
		return;

	string const name2 = trim(name);

	CmdDefMap::const_iterator pos = cmdDefMap.find(name2);

	if (pos == cmdDefMap.end()) 
		return;

	pos->second->locked = false;
}

CmdDef::newCmdDefResult CmdDef::newCmdDef(string const & name, 
										  string const & def)
{
	string const name2 = trim(name);

	if (name2.empty()) 
		return CmdDefNameEmpty;

	if (cmdDefMap.find(name) != cmdDefMap.end())
		return CmdDefExists;

	FuncRequest	func = lyxaction.lookupFunc(def);
	if (func.action == LFUN_NOACTION ||
		func.action == LFUN_UNKNOWN_ACTION) {
			return CmdDefInvalid;
	}

	boost::shared_ptr<CmdDefInfo> info;
	info.reset(new CmdDefInfo(func));
	cmdDefMap[name2] = info;

	return CmdDefOk;
}


} // namespace lyx
