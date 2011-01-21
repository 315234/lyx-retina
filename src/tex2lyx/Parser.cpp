/**
 * \file Parser.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author André Pönitz 
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "Encoding.h"
#include "Parser.h"
#include "support/textutils.h"

#include <iostream>

using namespace std;

namespace lyx {

namespace {

CatCode theCatcode[256];

void catInit()
{
	static bool init_done = false;
	if (init_done) 
		return;
	init_done = true;

	fill(theCatcode, theCatcode + 256, catOther);
	fill(theCatcode + 'a', theCatcode + 'z' + 1, catLetter);
	fill(theCatcode + 'A', theCatcode + 'Z' + 1, catLetter);

	theCatcode[int('\\')] = catEscape;
	theCatcode[int('{')]  = catBegin;
	theCatcode[int('}')]  = catEnd;
	theCatcode[int('$')]  = catMath;
	theCatcode[int('&')]  = catAlign;
	theCatcode[int('\n')] = catNewline;
	theCatcode[int('#')]  = catParameter;
	theCatcode[int('^')]  = catSuper;
	theCatcode[int('_')]  = catSub;
	theCatcode[0x7f]      = catIgnore;
	theCatcode[int(' ')]  = catSpace;
	theCatcode[int('\t')] = catSpace;
	theCatcode[int('\r')] = catNewline;
	theCatcode[int('~')]  = catActive;
	theCatcode[int('%')]  = catComment;

	// This is wrong!
	theCatcode[int('@')]  = catLetter;
}

/*!
 * Translate a line ending to '\n'.
 * \p c must have catcode catNewline, and it must be the last character read
 * from \p is.
 */
char_type getNewline(idocstream & is, char_type c)
{
	// we have to handle 3 different line endings:
	// - UNIX (\n)
	// - MAC  (\r)
	// - DOS  (\r\n)
	if (c == '\r') {
		// MAC or DOS
		char_type wc;
		if (is.get(wc) && wc != '\n') {
			// MAC
			is.putback(wc);
		}
		return '\n';
	}
	// UNIX
	return c;
}

CatCode catcode(char_type c)
{
	if (c < 256)
		return theCatcode[(unsigned char)c];
	return catOther;
}

}


//
// Token
//

ostream & operator<<(ostream & os, Token const & t)
{
	if (t.cat() == catComment)
		os << '%' << t.cs() << '\n';
	else if (t.cat() == catSpace)
		os << t.cs();
	else if (t.cat() == catEscape)
		os << '\\' << t.cs() << ' ';
	else if (t.cat() == catLetter)
		os << t.cs();
	else if (t.cat() == catNewline)
		os << "[" << t.cs().size() << "\\n," << t.cat() << "]\n";
	else
		os << '[' << t.cs() << ',' << t.cat() << ']';
	return os;
}


string Token::asInput() const
{
	if (cat_ == catComment)
		return '%' + cs_ + '\n';
	if (cat_ == catEscape)
		return '\\' + cs_;
	return cs_;
}


bool Token::isAlnumASCII() const
{
	return cat_ == catLetter ||
	       (cat_ == catOther && cs_.length() == 1 && isDigitASCII(cs_[0]));
}


#ifdef FILEDEBUG
void debugToken(std::ostream & os, Token const & t, unsigned int flags)
{
	char sep = ' ';
	os << "t: " << t << " flags: " << flags;
	if (flags & FLAG_BRACE_LAST) { os << sep << "BRACE_LAST"; sep = '|'; }
	if (flags & FLAG_RIGHT     ) { os << sep << "RIGHT"     ; sep = '|'; }
	if (flags & FLAG_END       ) { os << sep << "END"       ; sep = '|'; }
	if (flags & FLAG_BRACK_LAST) { os << sep << "BRACK_LAST"; sep = '|'; }
	if (flags & FLAG_TEXTMODE  ) { os << sep << "TEXTMODE"  ; sep = '|'; }
	if (flags & FLAG_ITEM      ) { os << sep << "ITEM"      ; sep = '|'; }
	if (flags & FLAG_LEAVE     ) { os << sep << "LEAVE"     ; sep = '|'; }
	if (flags & FLAG_SIMPLE    ) { os << sep << "SIMPLE"    ; sep = '|'; }
	if (flags & FLAG_EQUATION  ) { os << sep << "EQUATION"  ; sep = '|'; }
	if (flags & FLAG_SIMPLE2   ) { os << sep << "SIMPLE2"   ; sep = '|'; }
	if (flags & FLAG_OPTION    ) { os << sep << "OPTION"    ; sep = '|'; }
	if (flags & FLAG_BRACED    ) { os << sep << "BRACED"    ; sep = '|'; }
	if (flags & FLAG_CELL      ) { os << sep << "CELL"      ; sep = '|'; }
	if (flags & FLAG_TABBING   ) { os << sep << "TABBING"   ; sep = '|'; }
	os << "\n";
}
#endif


//
// Parser
//


Parser::Parser(idocstream & is)
	: lineno_(0), pos_(0), iss_(0), is_(is), encoding_latex_("utf8")
{
}


Parser::Parser(string const & s)
	: lineno_(0), pos_(0), 
	  iss_(new idocstringstream(from_utf8(s))), is_(*iss_), 
	  encoding_latex_("utf8")
{
}


Parser::~Parser()
{
	delete iss_;
}


void Parser::setEncoding(std::string const & e)
{
	Encoding const * enc = encodings.fromLaTeXName(e);
	if (!enc) {
		cerr << "Unknown encoding " << e << ". Ignoring." << std::endl;
		return;
	}
	//cerr << "setting encoding to " << enc->iconvName() << std::endl;
	is_ << lyx::setEncoding(enc->iconvName());
	encoding_latex_ = e;
}


void Parser::push_back(Token const & t)
{
	tokens_.push_back(t);
}


// We return a copy here because the tokens_ vector may get reallocated
Token const Parser::prev_token() const
{
	static const Token dummy;
	return pos_ > 1 ? tokens_[pos_ - 2] : dummy;
}


// We return a copy here because the tokens_ vector may get reallocated
Token const Parser::curr_token() const
{
	static const Token dummy;
	return pos_ > 0 ? tokens_[pos_ - 1] : dummy;
}


// We return a copy here because the tokens_ vector may get reallocated
Token const Parser::next_token()
{
	static const Token dummy;
	return good() ? tokens_[pos_] : dummy;
}


// We return a copy here because the tokens_ vector may get reallocated
Token const Parser::get_token()
{
	static const Token dummy;
	//cerr << "looking at token " << tokens_[pos_] << " pos: " << pos_ << '\n';
	return good() ? tokens_[pos_++] : dummy;
}


bool Parser::isParagraph()
{
	// A new paragraph in TeX ist started
	// - either by a newline, following any amount of whitespace
	//   characters (including zero), and another newline
	// - or the token \par
	if (curr_token().cat() == catNewline &&
	    (curr_token().cs().size() > 1 ||
	     (next_token().cat() == catSpace &&
	      pos_ < tokens_.size() - 1 &&
	      tokens_[pos_ + 1].cat() == catNewline)))
		return true;
	if (curr_token().cat() == catEscape && curr_token().cs() == "par")
		return true;
	return false;
}


bool Parser::skip_spaces(bool skip_comments)
{
	// We just silently return if we have no more tokens.
	// skip_spaces() should be callable at any time,
	// the caller must check p::good() anyway.
	bool skipped = false;
	while (good()) {
		get_token();
		if (isParagraph()) {
			putback();
			break;
		}
		if (curr_token().cat() == catSpace ||
		    curr_token().cat() == catNewline) {
			skipped = true;
			continue;
		}
		if ((curr_token().cat() == catComment && curr_token().cs().empty()))
			continue;
		if (skip_comments && curr_token().cat() == catComment) {
			// If positions_ is not empty we are doing some kind
			// of look ahead
			if (!positions_.empty())
				cerr << "  Ignoring comment: "
				     << curr_token().asInput();
		} else {
			putback();
			break;
		}
	}
	return skipped;
}


void Parser::unskip_spaces(bool skip_comments)
{
	while (pos_ > 0) {
		if ( curr_token().cat() == catSpace ||
		    (curr_token().cat() == catNewline && curr_token().cs().size() == 1))
			putback();
		else if (skip_comments && curr_token().cat() == catComment) {
			// TODO: Get rid of this
			// If positions_ is not empty we are doing some kind
			// of look ahead
			if (!positions_.empty())
				cerr << "Unignoring comment: "
				     << curr_token().asInput();
			putback();
		}
		else
			break;
	}
}


void Parser::putback()
{
	--pos_;
}


void Parser::pushPosition()
{
	positions_.push_back(pos_);
}


void Parser::popPosition()
{
	pos_ = positions_.back();
	positions_.pop_back();
}


bool Parser::good()
{
	if (pos_ < tokens_.size())
		return true;
	tokenize_one();
	return pos_ < tokens_.size();
}


char Parser::getChar()
{
	if (!good())
		error("The input stream is not well...");
	return get_token().character();
}


bool Parser::hasOpt()
{
	// An optional argument can occur in any of the following forms:
	// - \foo[bar]
	// - \foo [bar]
	// - \foo
	//   [bar]
	// - \foo %comment
	//   [bar]

	// remember current position
	unsigned int oldpos = pos_;
	// skip spaces and comments
	while (good()) {
		get_token();
		if (isParagraph()) {
			putback();
			break;
		}
		if (curr_token().cat() == catSpace ||
		    curr_token().cat() == catNewline ||
		    curr_token().cat() == catComment)
			continue;
		putback();
		break;
	}
	bool const retval = (next_token().asInput() == "[");
	pos_ = oldpos;
	return retval;
}


Parser::Arg Parser::getFullArg(char left, char right)
{
	skip_spaces(true);

	// This is needed if a partial file ends with a command without arguments,
	// e. g. \medskip
	if (! good())
		return make_pair(false, string());

	string result;
	char c = getChar();

	if (c != left) {
		putback();
		return make_pair(false, string());
	} else
		while ((c = getChar()) != right && good()) {
			// Ignore comments
			if (curr_token().cat() == catComment) {
				if (!curr_token().cs().empty())
					cerr << "Ignoring comment: " << curr_token().asInput();
			}
			else
				result += curr_token().asInput();
		}

	return make_pair(true, result);
}


string Parser::getArg(char left, char right)
{
	return getFullArg(left, right).second;
}


string Parser::getFullOpt()
{
	Arg arg = getFullArg('[', ']');
	if (arg.first)
		return '[' + arg.second + ']';
	return string();
}


string Parser::getOpt(bool keepws)
{
	string const res = getArg('[', ']');
	if (res.empty()) {
		if (keepws)
			unskip_spaces(true);
		return string();
	}
	return '[' + res + ']';
}


string Parser::getOptContent()
// the same as getOpt but without the brackets
{
	string const res = getArg('[', ']');
	return res.empty() ? string() : res;
}


string Parser::getFullParentheseArg()
{
	Arg arg = getFullArg('(', ')');
	if (arg.first)
		return '(' + arg.second + ')';
	return string();
}


string const Parser::verbatimEnvironment(string const & name)
{
	if (!good())
		return string();

	ostringstream os;
	for (Token t = get_token(); good(); t = get_token()) {
		if (t.cat() == catBegin) {
			putback();
			os << '{' << verbatim_item() << '}';
		} else if (t.asInput() == "\\begin") {
			string const env = getArg('{', '}');
			os << "\\begin{" << env << '}'
			   << verbatimEnvironment(env)
			   << "\\end{" << env << '}';
		} else if (t.asInput() == "\\end") {
			string const end = getArg('{', '}');
			if (end != name)
				cerr << "\\end{" << end
				     << "} does not match \\begin{" << name
				     << "}." << endl;
			return os.str();
		} else
			os << t.asInput();
	}
	cerr << "unexpected end of input" << endl;
	return os.str();
}


void Parser::tokenize_one()
{
	catInit();
	char_type c;
	if (!is_.get(c)) 
		return;

	switch (catcode(c)) {
	case catSpace: {
		docstring s(1, c);
		while (is_.get(c) && catcode(c) == catSpace)
			s += c;
		if (catcode(c) != catSpace)
			is_.putback(c);
		push_back(Token(s, catSpace));
		break;
	}
		
	case catNewline: {
		++lineno_;
		docstring s(1, getNewline(is_, c));
		while (is_.get(c) && catcode(c) == catNewline) {
			++lineno_;
			s += getNewline(is_, c);
		}
		if (catcode(c) != catNewline)
			is_.putback(c);
		push_back(Token(s, catNewline));
		break;
	}
		
	case catComment: {
		// We don't treat "%\n" combinations here specially because
		// we want to preserve them in the preamble
		docstring s;
		while (is_.get(c) && catcode(c) != catNewline)
			s += c;
		// handle possible DOS line ending
		if (catcode(c) == catNewline)
			c = getNewline(is_, c);
		// Note: The '%' at the beginning and the '\n' at the end
		// of the comment are not stored.
		++lineno_;
		push_back(Token(s, catComment));
		break;
	}
		
	case catEscape: {
		is_.get(c);
		if (!is_) {
			error("unexpected end of input");
		} else {
			docstring s(1, c);
			if (catcode(c) == catLetter) {
				// collect letters
				while (is_.get(c) && catcode(c) == catLetter)
					s += c;
				if (catcode(c) != catLetter)
					is_.putback(c);
			}
			push_back(Token(s, catEscape));
		}
		break;
	}
		
	case catIgnore: {
		cerr << "ignoring a char: " << c << "\n";
		break;
	}
		
	default:
		push_back(Token(docstring(1, c), catcode(c)));
	}
	//cerr << tokens_.back();
}


void Parser::dump() const
{
	cerr << "\nTokens: ";
	for (unsigned i = 0; i < tokens_.size(); ++i) {
		if (i == pos_)
			cerr << " <#> ";
		cerr << tokens_[i];
	}
	cerr << " pos: " << pos_ << "\n";
}


void Parser::error(string const & msg)
{
	cerr << "Line ~" << lineno_ << ":  parse error: " << msg << endl;
	dump();
	//exit(1);
}


string Parser::verbatimOption()
{
	string res;
	if (next_token().character() == '[') {
		Token t = get_token();
		for (t = get_token(); t.character() != ']' && good(); t = get_token()) {
			if (t.cat() == catBegin) {
				putback();
				res += '{' + verbatim_item() + '}';
			} else
				res += t.cs();
		}
	}
	return res;
}


string Parser::verbatim_item()
{
	if (!good())
		error("stream bad");
	skip_spaces();
	if (next_token().cat() == catBegin) {
		Token t = get_token(); // skip brace
		string res;
		for (Token t = get_token(); t.cat() != catEnd && good(); t = get_token()) {
			if (t.cat() == catBegin) {
				putback();
				res += '{' + verbatim_item() + '}';
			}
			else
				res += t.asInput();
		}
		return res;
	}
	return get_token().asInput();
}


void Parser::reset()
{
	pos_ = 0;
}


void Parser::setCatCode(char c, CatCode cat)
{
	theCatcode[(unsigned char)c] = cat;
}


CatCode Parser::getCatCode(char c) const
{
	return theCatcode[(unsigned char)c];
}


} // namespace lyx
