// -*- C++ -*-
/**
 * \file Language.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 * \author Dekel Tsur
 * \author Jürgen Vigna
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <map>
#include <string>


namespace lyx {

namespace support { class FileName; }

class Encoding;
class Lexer;

///
class Language {
public:
	///
	Language() : rightToLeft_(false), translated_(false) {}
	/// LyX language name
	std::string const & lang() const { return lang_; }
	/// Babel language name
	std::string const & babel() const { return babel_; }
	/// polyglossia language name
	std::string const & polyglossia() const { return polyglossia_name_; }
	/// polyglossia language options
	std::string const & polyglossiaOpts() const { return polyglossia_opts_; }
	/// translatable GUI name
	std::string const & display() const { return display_; }
	/// is this a RTL language?
	bool rightToLeft() const { return rightToLeft_; }
	/// Is an (at least partial) translation of this language available?
	bool translated() const { return translated_; }
	/// default encoding
	Encoding const * encoding() const { return encoding_; }
	///
	std::string const & encodingStr() const { return encodingStr_; }
	/// language code
	std::string const & code() const { return code_; }
	/// set code (needed for rc.spellchecker_alt_lang)
	void setCode(std::string const c) { code_ = c; }
	/// language variety (needed by aspell checker)
	std::string const & variety() const { return variety_; }
	/// set variety (needed for rc.spellchecker_alt_lang)
	void setVariety(std::string const v) { variety_ = v; }
	/// create a unique ID from lang code and variety
	std::string const id() const {
		return variety_.empty() ? code_ : code_ + "-" + variety_; }
	/// preamble settings after babel was called
	std::string const & babel_postsettings() const { return babel_postsettings_; }
	/// preamble settings before babel is called
	std::string const & babel_presettings() const { return babel_presettings_; }
	/// This language internally sets a font encoding
	bool internalFontEncoding() const { return internal_enc_; }
	/// This language needs to be passed to babel itself (not the class)
	bool asBabelOptions() const { return as_babel_options_; }
	///
	bool read(Lexer & lex);
	///
	bool readLanguage(Lexer & lex);
	// for the use in std::map
	friend bool operator<(Language const & p, Language const & q);
private:
	///
	std::string lang_;
	///
	std::string babel_;
	///
	std::string polyglossia_name_;
	///
	std::string polyglossia_opts_;
	///
	std::string display_;
	///
	bool rightToLeft_;
	///
	std::string encodingStr_;
	///
	Encoding const * encoding_;
	///
	std::string code_;
	///
	std::string variety_;
	///
	std::string babel_postsettings_;
	///
	std::string babel_presettings_;
	///
	bool internal_enc_;
	///
	bool as_babel_options_;
	///
	bool translated_;
};


inline bool operator<(Language const & p, Language const & q)
{
	return q.lang() > p.lang();
}


class Languages
{
public:
	///
	typedef std::map<std::string, Language> LanguageList;
	///
	typedef LanguageList::const_iterator const_iterator;
	///
	typedef LanguageList::size_type size_type;
	///
	void read(support::FileName const & filename);
	///
	Language const * getLanguage(std::string const & language) const;
	///
	size_type size() const { return languagelist.size(); }
	///
	const_iterator begin() const { return languagelist.begin(); }
	///
	const_iterator end() const { return languagelist.end(); }
	///

private:
	///
	LanguageList languagelist;
};

/// Global singleton instance.
extern Languages languages;
/// Default language defined in LyXRC
extern Language const * default_language;
/// Used to indicate that the language should be left unchanged when
/// applying a font change.
extern Language const * ignore_language;
/// Default language defined in LyXRC
extern Language const * latex_language;
/// Used to indicate that the language should be reset to the Buffer
// language when applying a font change.
extern Language const * reset_language;


} // namespace lyx

#endif
