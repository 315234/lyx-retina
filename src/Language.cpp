/**
 * \file Language.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author Jürgen Spitzmüller
 * \author Dekel Tsur
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Language.h"

#include "Encoding.h"
#include "Lexer.h"
#include "LyXRC.h"

#include "support/debug.h"
#include "support/FileName.h"
#include "support/filetools.h"
#include "support/lstrings.h"
#include "support/Messages.h"

using namespace std;
using namespace lyx::support;


namespace lyx {

Languages languages;
Language ignore_lang;
Language latex_lang;
Language const * default_language;
Language const * ignore_language = &ignore_lang;
Language const * latex_language = &latex_lang;
Language const * reset_language = 0;


docstring const Language::translateLayout(string const & m) const
{
	if (m.empty())
		return docstring();

	if (!isAscii(m)) {
		lyxerr << "Warning: not translating `" << m
		       << "' because it is not pure ASCII.\n";
		return from_utf8(m);
	}

	TranslationMap::const_iterator it = layoutTranslations_.find(m);
	if (it != layoutTranslations_.end())
		return it->second;

	return from_ascii(m);
}


bool Language::readLanguage(Lexer & lex)
{
	enum LanguageTags {
		LA_AS_BABELOPTS = 1,
		LA_BABELNAME,
		LA_ENCODING,
		LA_END,
		LA_GUINAME,
		LA_INTERNAL_ENC,
		LA_LANG_CODE,
		LA_LANG_VARIETY,
		LA_POLYGLOSSIANAME,
		LA_POLYGLOSSIAOPTS,
		LA_POSTBABELPREAMBLE,
		LA_PREBABELPREAMBLE,
		LA_RTL
	};

	// Keep these sorted alphabetically!
	LexerKeyword languageTags[] = {
		{ "asbabeloptions",       LA_AS_BABELOPTS },
		{ "babelname",            LA_BABELNAME },
		{ "encoding",             LA_ENCODING },
		{ "end",                  LA_END },
		{ "guiname",              LA_GUINAME },
		{ "internalencoding",     LA_INTERNAL_ENC },
		{ "langcode",             LA_LANG_CODE },
		{ "langvariety",          LA_LANG_VARIETY },
		{ "polyglossianame",      LA_POLYGLOSSIANAME },
		{ "polyglossiaopts",      LA_POLYGLOSSIAOPTS },
		{ "postbabelpreamble",    LA_POSTBABELPREAMBLE },
		{ "prebabelpreamble",     LA_PREBABELPREAMBLE },
		{ "rtl",                  LA_RTL }
	};

	bool error = false;
	bool finished = false;
	lex.pushTable(languageTags);
	// parse style section
	while (!finished && lex.isOK() && !error) {
		int le = lex.lex();
		// See comment in LyXRC.cpp.
		switch (le) {
		case Lexer::LEX_FEOF:
			continue;

		case Lexer::LEX_UNDEF: // parse error
			lex.printError("Unknown language tag `$$Token'");
			error = true;
			continue;

		default: 
			break;
		}
		switch (static_cast<LanguageTags>(le)) {
		case LA_END: // end of structure
			finished = true;
			break;
		case LA_AS_BABELOPTS:
			lex >> as_babel_options_;
			break;
		case LA_BABELNAME:
			lex >> babel_;
			break;
		case LA_POLYGLOSSIANAME:
			lex >> polyglossia_name_;
			break;
		case LA_POLYGLOSSIAOPTS:
			lex >> polyglossia_opts_;
			break;
		case LA_ENCODING:
			lex >> encodingStr_;
			break;
		case LA_GUINAME:
			lex >> display_;
			break;
		case LA_INTERNAL_ENC:
			lex >> internal_enc_;
			break;
		case LA_LANG_CODE:
			lex >> code_;
			break;
		case LA_LANG_VARIETY:
			lex >> variety_;
			break;
		case LA_POSTBABELPREAMBLE:
			babel_postsettings_ =
				lex.getLongString("EndPostBabelPreamble");
			break;
		case LA_PREBABELPREAMBLE:
			babel_presettings_ =
				lex.getLongString("EndPreBabelPreamble");
			break;
		case LA_RTL:
			lex >> rightToLeft_;
			break;
		}
	}
	lex.popTable();
	return finished && !error;
}


bool Language::read(Lexer & lex)
{
	as_babel_options_ = 0;
	encoding_ = 0;
	internal_enc_ = 0;
	rightToLeft_ = 0;

	if (!lex.next()) {
		lex.printError("No name given for language: `$$Token'.");
		return false;
	}

	lang_ = lex.getString();
	LYXERR(Debug::INFO, "Reading language " << lang_);
	if (!readLanguage(lex)) {
		LYXERR0("Error parsing language `" << lang_ << '\'');
		return false;
	}

	encoding_ = encodings.fromLyXName(encodingStr_);
	if (!encoding_ && !encodingStr_.empty()) {
		encoding_ = encodings.fromLyXName("iso8859-1");
		LYXERR0("Unknown encoding " << encodingStr_);
	}
	// cache translation status. Calling getMessages() directly in
	// PrefLanguage::PrefLanguage() did only work if the gui language
	// was set to auto (otherwise all languages would be marked as available).
	translated_ = getMessages(code()).available();
	return true;
}


bool Language::readLayoutTranslations(Lexer & lex)
{
	layoutTranslations_.clear();
	while (lex.isOK()) {
		if (lex.checkFor("End"))
			break;
		if (!lex.next(true))
			return false;
		string const key = lex.getString();
		if (!lex.next(true))
			return false;
		docstring const val = lex.getDocString();
		layoutTranslations_[key] = val;
	}
	return true;
}


void Language::readLayoutTranslations(Language const & lang)
{
	layoutTranslations_ = lang.layoutTranslations_;
}


void Languages::read(FileName const & filename)
{
	Lexer lex;
	lex.setFile(filename);
	lex.setContext("Languages::read");
	while (lex.isOK()) {
		int le = lex.lex();
		switch (le) {
		case Lexer::LEX_FEOF:
			continue;

		default:
			break;
		}
		if (lex.getString() != "Language") {
			lex.printError("Unknown Language tag `$$Token'");
			continue;
		}
		Language l;
		l.read(lex);
		if (!lex)
			break;
		if (l.lang() == "latex")
			latex_lang = l;
		else if (l.lang() == "ignore")
			ignore_lang = l;
		else
			languagelist[l.lang()] = l;
	}

	default_language = getLanguage(lyxrc.default_language);
	if (!default_language) {
		LYXERR0("Default language \"" << lyxrc.default_language
		       << "\" not found!");
		default_language = getLanguage("english");
		if (!default_language)
			default_language = &(*languagelist.begin()).second;
		LYXERR0("Using \"" << default_language->lang() << "\" instead!");
	}

	// Read layout translations
	FileName const path = libFileSearch(string(), "layouttranslations");
	readLayoutTranslations(path);
}


void Languages::readLayoutTranslations(support::FileName const & filename)
{
	Lexer lex;
	lex.setFile(filename);
	lex.setContext("Languages::read");
	while (lex.isOK()) {
		if (!lex.checkFor("Translation")) {
			if (lex.isOK())
				lex.printError("Unknown layout translation tag `$$Token'");
			break;
		}
		if (!lex.next(true))
			break;
		string const code = lex.getString();
		// we need to mimic gettext: code can be a two-letter code,
		// which should match all variants, e.g. "de" should match
		// "de_DE", "de_AT" etc.
		Language * firstlang = 0;
		LanguageList::iterator const end = languagelist.end();
		for (LanguageList::iterator it = languagelist.begin(); it != end; ++it) {
			// special case for chinese:
			// simplified  => code == "zh_CN", langcode == "zh_CN"
			// traditional => code == "zh_TW", langcode == "zh_CN"
			string const langcode = it->second.code();
			string const name = it->second.lang();
			if ((code == langcode && name != "chinese-traditional") ||
			    (code == "zh_TW" && name == "chinese-traditional") ||
			    (code.size() == 2 && langcode.size() > 2 &&
			     code + '_' == langcode.substr(0, 3))) {
				if (firstlang)
					it->second.readLayoutTranslations(*firstlang);
				else {
					if (!it->second.readLayoutTranslations(lex)) {
						lex.printError("Could not read "
						               "layout translations "
						               "for language `" +
						               code + "'");
						break;
					}
					firstlang = &(it->second);
				}
			}
		}
		if (!firstlang) {
			lex.printError("Unknown language `" + code + "'");
			break;
		}
	}
}


Language const * Languages::getLanguage(string const & language) const
{
	if (language == "reset")
		return reset_language;
	if (language == "ignore")
		return ignore_language;
	const_iterator it = languagelist.find(language);
	return it == languagelist.end() ? reset_language : &it->second;
}


} // namespace lyx
