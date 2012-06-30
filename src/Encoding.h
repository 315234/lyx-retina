// -*- C++ -*-
/**
 * \file Encoding.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bjønnes
 * \author Jean-Marc Lasgouttes
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef ENCODING_H
#define ENCODING_H

#include "support/docstring.h"
#include "support/types.h"

#include <map>
#include <set>
#include <vector>

namespace lyx {

namespace support { class FileName; }

class Buffer;
class LaTeXFeatures;

class EncodingException : public std::exception {
public:
	EncodingException(char_type c);
	virtual ~EncodingException() throw() {}
	virtual const char * what() const throw();
 
	char_type failed_char;
	int par_id;
	pos_type pos;
};


///
class Encoding {
public:
	/// Which LaTeX package handles this encoding?
	enum Package {
		none,
		inputenc,
		CJK,
		japanese
	};
	///
	Encoding() {}
	///
	Encoding(std::string const & n, std::string const & l,
		 std::string const & g, std::string const & i,
		 bool f, Package p);
	///
	void init() const;
	///
	std::string const & name() const { return name_; }
	///
	std::string const & latexName() const { return latexName_; }
	///
	std::string const & guiName() const { return guiName_; }
	///
	std::string const & iconvName() const { return iconvName_; }
	///
	bool const & hasFixedWidth() const { return fixedwidth_; }
	/// \p c is representable in this encoding without a LaTeX macro
	bool encodable(char_type c) const;
	/**
	 * Convert \p c to something that LaTeX can understand.
	 * This is either the character itself (if it is representable
	 * in this encoding), or a LaTeX macro.
	 * If the character is not representable in this encoding, but no
	 * LaTeX macro is known, a warning is given of lyxerr, and the
	 * character is returned.
	 * \return the converted character and a flag indicating whether
	 * the command needs to be terminated by {} or a space.
	 */
	std::pair<docstring, bool> latexChar(char_type c) const;
	/**
	 * Convert \p input to something that LaTeX can understand.
	 * This is either the string itself (if it is representable
	 * in this encoding), or a LaTeX macro.
	 * If a character is not representable in this encoding, but no
	 * LaTeX macro is known, a warning is given of lyxerr, and the
	 * character is returned in the second string of the pair and
	 * omitted in the first.
	 * \p dryrun specifies whether the string is used within source
	 * preview (which yields a special warning).
	 */
	std::pair<docstring, docstring> latexString(docstring const input,
						    bool dryrun = false) const;
	/// Which LaTeX package handles this encoding?
	Package package() const { return package_; }
	/// A list of all characters usable in this encoding
	std::vector<char_type> symbolsList() const;
private:
	///
	std::string name_;
	///
	std::string latexName_;
	///
	std::string guiName_;
	///
	std::string iconvName_;
	/// Is this a fixed width encoding?
	bool fixedwidth_;
	///
	typedef std::set<char_type> CharSet;
	/// Set of UCS4 characters that we can encode (for singlebyte
	/// encodings only)
	mutable CharSet encodable_;
	/// All code points below this are encodable. This helps us to avoid
	/// lokup of ASCII characters in encodable_ and gives about 1 sec
	/// speedup on export of the Userguide.
	mutable char_type start_encodable_;
	/// Which LaTeX package handles this encoding?
	Package package_;
	/**
	 * If this is true the stored information about the encoding covers
	 * all encodable characters. We set this to false initially so that
	 * we only need to query iconv for the actually used encodings.
	 * This is needed especially for the multibyte encodings, if we
	 * complete all encoding info on startup it takes 2-3 minutes.
	 */
	mutable bool complete_;
};

class Encodings {
public:
	///
	typedef std::set<char_type> MathCommandSet;
	///
	typedef std::set<char_type> TextCommandSet;
	///
	typedef std::set<char_type> MathSymbolSet;
	///
	typedef std::map<std::string, Encoding> EncodingList;
	/// iterator to iterate over all encodings.
	/// We hide the fact that our encoding list is implemented as a map.
	class const_iterator : public EncodingList::const_iterator {
		typedef EncodingList::const_iterator base;
	public:
		const_iterator() : base() {}
		const_iterator(base const & b) : base(b) {}
		Encoding const & operator*() const { return base::operator*().second; }
		Encoding const * operator->() const { return &(base::operator*().second); }
	};
	///
	Encodings();
	/// Read the encodings.
	/// \param encfile encodings definition file
	/// \param symbolsfile unicode->LaTeX mapping file
	void read(support::FileName const & encfile,
		  support::FileName const & symbolsfile);
	/// Get encoding from LyX name \p name
	Encoding const * fromLyXName(std::string const & name) const;
	/// Get encoding from LaTeX name \p name
	Encoding const * fromLaTeXName(std::string const & name) const;

	///
	const_iterator begin() const { return encodinglist.begin(); }
	///
	const_iterator end() const { return encodinglist.end(); }

	///
	enum LetterForm {
		///
		FORM_ISOLATED,
		///
		FORM_FINAL,
		///
		FORM_INITIAL,
		///
		FORM_MEDIAL
	};
	///
	static bool isHebrewComposeChar(char_type c);
	///
	static bool isArabicComposeChar(char_type c);
	///
	static bool isArabicSpecialChar(char_type c);
	///
	static bool isArabicChar(char_type c);
	///
	static char_type transformChar(char_type c, LetterForm form);
	/// Is this a combining char?
	static bool isCombiningChar(char_type c);
	/// Return the TIPA shortcut
	static std::string const TIPAShortcut(char_type c);
	/**
	 * Is this a known char from some language?
	 * If \p preamble is empty and code point \p c is known to belong
	 * to a supported script, true is returned and \p preamble is set
	 * to the corresponding entry in the unicodesymbols file.
	 * If \p preamble is not empty, a check is made whether code point
	 * \p c is a known character matching the preamble entry.
	 */
	static bool isKnownScriptChar(char_type const c, std::string & preamble);
	/**
	 * Do we have to output this character as LaTeX command in any case?
	 * This is true if the "force" flag is set.
	 * We need this if the inputencoding does not support a certain glyph.
	 */
	static bool isForced(char_type c);
	/**
	 * Do we have to display in italics this character when in mathmode?
	 * This is true if the "mathalpha" flag is set. We use this for
	 * letters and accented characters that are output as math commands.
	 */
	static bool isMathAlpha(char_type c);
	/**
	 * Register \p c as a mathmode command.
	 */
	static void addMathCmd(char_type c) { mathcmd.insert(c); }
	/**
	 * Register \p c as a textmode command.
	 */
	static void addTextCmd(char_type c) { textcmd.insert(c); }
	/**
	 * Register \p c as a mathmode symbol.
	 */
	static void addMathSym(char_type c) { mathsym.insert(c); }
	/**
	 * Tell whether \p c is registered as a mathmode command.
	 */
	static bool isMathCmd(char_type c) { return mathcmd.count(c); }
	/**
	 * Tell whether \p c is registered as a textmode command.
	 */
	static bool isTextCmd(char_type c) { return textcmd.count(c); }
	/**
	 * Tell whether \p c is registered as a mathmode symbol.
	 */
	static bool isMathSym(char_type c) { return mathsym.count(c); }
	/**
	 * Initialize mathcmd, textcmd, and mathsym sets.
	 */
	static void initUnicodeMath(Buffer const & buffer, bool for_master = true);
	/**
	 * If \p c cannot be encoded in the given \p encoding, convert
	 * it to something that LaTeX can understand in mathmode.
	 * \p needsTermination indicates whether the command needs to be
	 * terminated by {} or a space.
	 * \return whether \p command is a mathmode command
	 */
	static bool latexMathChar(char_type c, bool mathmode,
			Encoding const * encoding, docstring & command,
			bool & needsTermination);
	/**
	 * Convert the LaTeX command in \p cmd to the corresponding unicode
	 * point and set \p combining to true if it is a combining symbol.
	 * \p needsTermination indicates whether the command needs to be
	 * terminated by {} or a space.
	 */
	static char_type fromLaTeXCommand(docstring const & cmd, int cmdtype,
			bool & combining, bool & needsTermination,
			std::set<std::string> * req = 0);
	///
	enum LatexCmd {
		///
		MATH_CMD = 1,
		///
		TEXT_CMD = 2
	};
	/**
	 * Convert the LaTeX commands in \p cmd and \return a docstring
	 * of corresponding unicode points. The conversion stops at the
	 * first command which could not be converted, and the remaining
	 * unconverted commands are returned in \p rem.
	 * The \p cmdtype parameter can be used to limit recognized
	 * commands to math or text mode commands only.
	 * \p needsTermination indicates whether the command needs to be
	 * terminated by {} or a space.
	 */
	static docstring fromLaTeXCommand(docstring const & cmd, int cmdtype,
			bool & needsTermination, docstring & rem,
			std::set<std::string> * req = 0);
	/**
	 * Add the preamble snippet needed for the output of \p c to
	 * \p features.
	 * This does not depend on the used encoding, since the inputenc
	 * package only maps the code point \p c to a command, it does not
	 * make this command available.
	 */
	static void validate(char_type c, LaTeXFeatures & features, bool for_mathed = false);

private:
	///
	EncodingList encodinglist;
	///
	static MathCommandSet mathcmd;
	///
	static TextCommandSet textcmd;
	///
	static MathSymbolSet mathsym;
};

extern Encodings encodings;


} // namespace lyx

#endif
