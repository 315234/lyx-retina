/**
 * \file LyXAction.cpp
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Jean-Marc Lasgouttes
 * \author John Levon
 * \author Andr� P�nitz
 *
 * Full author contact details are available in file CREDITS.
 */

#include <config.h>

#include "LyXAction.h"

#include "support/debug.h"
#include "FuncRequest.h"

#include "support/lstrings.h"

#include <boost/assert.hpp>

using namespace std;
using namespace lyx::support;

namespace lyx {

/*
     NAMING RULES FOR USER-COMMANDS
     Here's the set of rules to apply when a new command name is introduced:

     1) Use the object.event order. That is, use `word-forward'
	instead of `forward-word'.
     2) Don't introduce an alias for an already named object. Same for events.
     3) Forward movement or focus is called `forward' (not `right').
     4) Backward movement or focus is called `backward' (not `left').
     5) Upward movement of focus is called `up'.
     6) Downward movement is called `down'.
     7) The begin of an object is called `begin' (not `start').
     8) The end of an object is called `end'.

     (May 19 1996, 12:04, RvdK)
*/

/* LFUN documentation (a start at least, Chr 2007-08-12)
 *
 * The documentation below is primarily notes about restrictions and
 * oddities relating to the different LFUNs.
 *
 * Try to find an appropriate (thematical) place when adding the new LFUN.
 *
 * Doxygen template below. Some notes: Parameters should be set in uppercase
 * and put in <BRACKETS>, as so.
 */

/**
 * \var lyx::kb_action lyx::LFUN_
 * \li Action: 
 * \li Notion: 
 * \li Syntax: 
 * \li Params: 
 * \li Sample:
 * \li Origin: 
 */

LyXAction lyxaction;


void LyXAction::newFunc(kb_action action, string const & name,
			unsigned int attrib, LyXAction::func_type type)
{
	lyx_func_map[name] = action;
	func_info tmpinfo;
	tmpinfo.name = name;
	tmpinfo.attrib = attrib;
	tmpinfo.type = type;
	lyx_info_map[action] = tmpinfo;
}


// Needed for LFUNs documentation to be accepted, since doxygen won't take
// \var inside functions.
#ifndef DOXYGEN_SHOULD_SKIP_THIS
void LyXAction::init()
{
	// This function was changed to use the array below in initalization
	// instead of calling newFunc numerous times because of compilation
	// times. Since the array is not static we get back the memory it
	// occupies after the init is completed. It compiles several
	// magnitudes faster.

	static bool init;
	if (init) return;

	struct ev_item {
		kb_action action;
		char const * name;
		unsigned int attrib;
		func_type type;
	};

	ev_item const items[] = {
#endif
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_ACUTE
 * \li Action: Adds an acute accent \htmlonly (&aacute;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-acute
 */
		{ LFUN_ACCENT_ACUTE, "accent-acute", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_BREVE
 * \li Action: Adds a breve accent \htmlonly (&#259;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-breve
 */
		{ LFUN_ACCENT_BREVE, "accent-breve", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_CARON
 * \li Action: Adds a caron \htmlonly (&#462;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-caron
 */
		{ LFUN_ACCENT_CARON, "accent-caron", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_CEDILLA
 * \li Action: Adds a cedilla \htmlonly (&ccedil;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-cedilla
 */
		{ LFUN_ACCENT_CEDILLA, "accent-cedilla", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_CIRCLE
 * \li Action: Adds a circle accent \htmlonly (&aring;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-circle
 */
		{ LFUN_ACCENT_CIRCLE, "accent-circle", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_CIRCUMFLEX
 * \li Action: Adds a circumflex \htmlonly (&ecirc;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-circumflex
 */
		{ LFUN_ACCENT_CIRCUMFLEX, "accent-circumflex", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_DOT
 * \li Action: Adds a dot accent \htmlonly (&#380;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-dot
 */
		{ LFUN_ACCENT_DOT, "accent-dot", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_GRAVE
 * \li Action: Adds a grave accent \htmlonly (&egrave;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-grave
 */
		{ LFUN_ACCENT_GRAVE, "accent-grave", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_HUNGARIAN_UMLAUT
 * \li Action: Adds a Hungarian umlaut \htmlonly (&#337;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-grave
 */
		{ LFUN_ACCENT_HUNGARIAN_UMLAUT, "accent-hungarian-umlaut", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_MACRON
 * \li Action: Adds a macron \htmlonly (&#257;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-macron
 */
		{ LFUN_ACCENT_MACRON, "accent-macron", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_OGONEK
 * \li Action: Adds an ogonek accent \htmlonly (&#261;)\endhtmlonly
               to the next character typed.
 * \li Syntax: accent-ogonek
 */
		{ LFUN_ACCENT_OGONEK, "accent-ogonek", Noop, Edit },
		{ LFUN_ACCENT_SPECIAL_CARON, "accent-special-caron", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_TIE
 * \li Action: Adds a tie \htmlonly (a&#865;)\endhtmlonly
               over the next two character typed.
 * \li Notion: the following char will finish the tie.
 * \li Syntax: accent-tie
 */
		{ LFUN_ACCENT_TIE, "accent-tie", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_TILDE
 * \li Action: Adds a tilde \htmlonly (&atilde;)\endhtmlonly
               over the next character typed.
 * \li Syntax: accent-tilde
 */
		{ LFUN_ACCENT_TILDE, "accent-tilde", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_UMLAUT
 * \li Action: Adds an umlaut \htmlonly (&auml;)\endhtmlonly
               over the next character typed.
 * \li Syntax: accent-umlaut
 */
		{ LFUN_ACCENT_UMLAUT, "accent-umlaut", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_UNDERBAR
 * \li Action: Adds a bar \htmlonly (a&#800;)\endhtmlonly
               under the next character typed.
 * \li Syntax: accent-underbar
 */
		{ LFUN_ACCENT_UNDERBAR, "accent-underbar", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ACCENT_UNDERDOT
 * \li Action: Adds a dot \htmlonly (&#7841;)\endhtmlonly
               under the next character typed.
 * \li Syntax: accent-underdot
 */
		{ LFUN_ACCENT_UNDERDOT, "accent-underdot", Noop, Edit },

/**
 * \var lyx::kb_action lyx::LFUN_BREAK_PARAGRAPH
 * \li Action: Breaks the current paragraph at the current location.
 * \li Syntax: break-paragraph
 */
		{ LFUN_BREAK_PARAGRAPH, "break-paragraph", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_BREAK_PARAGRAPH_SKIP
 * \li Action: Breaks the current paragraph at the current location,
               unless used at the beginning of a line, where it sets
               the label width string to empty.
 * \li Syntax: break-paragraph-skip
 */
		{ LFUN_BREAK_PARAGRAPH_SKIP, "break-paragraph-skip", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CAPTION_INSERT
 * \li Action: Inserts a caption inset.
 * \li Syntax: caption-insert
 * \li Origin: Lgb, 18 Jul 2000
 */
		{ LFUN_CAPTION_INSERT, "caption-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_DATE_INSERT
 * \li Action: Inserts the current date.
 * \li Syntax: date-insert [<ARG>]
 * \li Param: <ARG>: Format of date. The default value (%x) can be set
                     in Preferences->Date format. For possible formats
                     see manual page of strftime function.
 * \li Origin: jdblair, 31 Jan 2000
 */
		{ LFUN_DATE_INSERT, "date-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_FOOTNOTE_INSERT
 * \li Action: Inserts a footnote inset.
 * \li Syntax: footnote-insert
 * \li Origin: Jug, 7 Mar 2000
 */
		{ LFUN_FOOTNOTE_INSERT, "footnote-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_ERT_INSERT
 * \li Action: Inserts an ERT inset.
 * \li Syntax: ert-insert
 * \li Origin: Jug, 18 Feb 2000
 */
		{ LFUN_ERT_INSERT, "ert-insert", Noop, Edit },
		{ LFUN_FLOAT_INSERT, "float-insert", Noop, Edit },
		{ LFUN_FLOAT_WIDE_INSERT, "float-wide-insert", Noop, Edit },
		{ LFUN_WRAP_INSERT, "wrap-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_HFILL_INSERT
 * \li Action: Inserts an hfill inset.
 * \li Syntax: hfill-insert
 */
		{ LFUN_HFILL_INSERT, "hfill-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_OPTIONAL_INSERT
 * \li Action: Inserts an optional-argument (short title) inset.
 * \li Syntax: optional-insert
 * \li Origin: Martin, 12 Aug 2002
 */
		{ LFUN_OPTIONAL_INSERT, "optional-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_LINE_INSERT
 * \li Action: Inserts a horizontal line.
 * \li Syntax: line-insert
 * \li Origin: poenitz,  Oct 27 2003
 */
		{ LFUN_LINE_INSERT, "line-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_NEWPAGE_INSERT
 * \li Action: Inserts a new page.
 * \li Syntax: newpage-insert
 * \li Origin: uwestoehr, 24 Nov 2007
 */
		{ LFUN_NEWPAGE_INSERT, "newpage-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_PAGEBREAK_INSERT
 * \li Action: Inserts a pagebreak.
 * \li Syntax: pagebreak-insert
 * \li Origin: poenitz,  Oct 27 2003
 */
		{ LFUN_PAGEBREAK_INSERT, "pagebreak-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_MARGINALNOTE_INSERT
 * \li Action: Inserts a marginal note.
 * \li Syntax: marginalnote-insert
 * \li Origin: Lgb, 26 Jun 2000
 */
		{ LFUN_MARGINALNOTE_INSERT, "marginalnote-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_UNICODE_INSERT
 * \li Action: Inserts a single unicode character.
 * \li Syntax: unicode-insert <CHAR>
 * \li Params: <CHAR>: The character to insert, given as its code
                       point, in hexadecimal, e.g.: unicode-insert 0x0100.
 * \li Origin: Lgb, 22 Oct 2006
 */
		{ LFUN_UNICODE_INSERT, "unicode-insert", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_LISTING_INSERT
 * \li Action: Inserts a new listings inset.
 * \li Syntax: listing-insert
 * \li Origin: Herbert, 10 Nov 2001; bpeng, 2 May 2007
 */
		{ LFUN_LISTING_INSERT, "listing-insert", Noop, Edit },
		{ LFUN_QUOTE_INSERT, "quote-insert", Noop, Edit },
		{ LFUN_INFO_INSERT, "info-insert", Noop, Edit },
		{ LFUN_BRANCH_INSERT, "branch-insert", Noop, Edit },
		{ LFUN_BOX_INSERT, "box-insert", Noop, Edit },
		{ LFUN_FLEX_INSERT, "flex-insert", Noop, Edit },
		{ LFUN_SELF_INSERT, "self-insert", SingleParUpdate, Hidden },
		{ LFUN_SPACE_INSERT, "space-insert", Noop, Edit },
		{ LFUN_HYPERLINK_INSERT, "href-insert", Noop, Edit },
		{ LFUN_SPECIALCHAR_INSERT, "specialchar-insert", Noop, Edit },
		{ LFUN_CLEARPAGE_INSERT, "clearpage-insert", Noop, Edit },
		{ LFUN_CLEARDOUBLEPAGE_INSERT, "cleardoublepage-insert", Noop, Edit },
		{ LFUN_TOC_INSERT, "toc-insert", Noop, Edit },
		{ LFUN_APPENDIX, "appendix", Noop, Edit },

#if 0
		{ LFUN_LIST_INSERT, "list-insert", Noop, Edit },
		{ LFUN_THEOREM_INSERT, "theorem-insert", Noop, Edit },
#endif

		{ LFUN_DELETE_BACKWARD_SKIP, "delete-backward-skip", Noop, Edit },
		{ LFUN_DELETE_FORWARD_SKIP, "delete-forward-skip", Noop, Edit },
		{ LFUN_DOWN, "down", ReadOnly | NoUpdate, Edit },
		{ LFUN_DOWN_SELECT, "down-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_UP, "up", ReadOnly | NoUpdate, Edit },
		{ LFUN_UP_SELECT, "up-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_SCREEN_DOWN, "screen-down", ReadOnly, Edit },
		{ LFUN_SCREEN_DOWN_SELECT, "screen-down-select", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_SCROLL
 * \li Action: Scroll the buffer view.
 * \li Notion: Only scrolls the screen up or down; does not move the cursor.
 * \li Syntax: scroll <TYPE> <QUANTITY>
 * \li Params: <TYPE>:  line|page\n
               <QUANTITY>: up|down|<number>\n
 * \li Origin: Abdelrazak Younes, Dec 27 2007
 */
		{ LFUN_SCROLL, "scroll", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_SCREEN_RECENTER
 * \li Action: Recenters the screen on the current cursor position.
 * \li Syntax: screen-recenter
 */
		{ LFUN_SCREEN_RECENTER, "screen-recenter", ReadOnly, Edit },
		{ LFUN_SCREEN_UP, "screen-up", ReadOnly, Edit },
		{ LFUN_SCREEN_UP_SELECT, "screen-up-select", ReadOnly, Edit },

		{ LFUN_ERROR_NEXT, "error-next", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_BACKWARD
 * \li Action: Moves the cursor one position logically backwards.
 * \li Notion: This is not the action which should be bound to the arrow keys,
               because backwards may be left or right, depending on the
               language. The arrow keys should be bound to #LFUN_CHAR_LEFT or
               #LFUN_CHAR_RIGHT actions, which in turn may employ this one.
 * \li Syntax: char-backward
 */
		{ LFUN_CHAR_BACKWARD, "char-backward", ReadOnly | NoUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_BACKWARD_SELECT
 * \li Action: Moves the cursor one position logically backwards, adding
               traversed position to the selection.
 * \li Notion: See also #LFUN_CHAR_BACKWARD.
 * \li Syntax: char-backward-select
 */
		{ LFUN_CHAR_BACKWARD_SELECT, "char-backward-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_CHAR_DELETE_BACKWARD, "delete-backward", SingleParUpdate, Edit },
		{ LFUN_CHAR_DELETE_FORWARD, "delete-forward", SingleParUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_FORWARD
 * \li Action: Moves the cursor one position logically forward.
 * \li Notion: This is not the action which should be bound to the arrow keys,
               because forward may be left or right, depending on the language.
               The arrow keys should be bound to #LFUN_CHAR_LEFT or
               #LFUN_CHAR_RIGHT actions, which in turn may employ this one.
 * \li Syntax: char-forward
 */
		{ LFUN_CHAR_FORWARD, "char-forward", ReadOnly | NoUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_FORWARD_SELECT
 * \li Action: Moves the cursor one position logically forward, adding
               traversed position to the selection.
 * \li Notion: See also #LFUN_CHAR_FORWARD.
 * \li Syntax: char-forward-select
 */
		{ LFUN_CHAR_FORWARD_SELECT, "char-forward-select", ReadOnly | SingleParUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_LEFT
 * \li Action: Moves the cursor one position "to the left".
 * \li Notion: This is the action which should be taken when the "left" key
               is pressed. Generally, it moves the cursor one position to the
               left. However, in Bidi text this become slightly more
               complicated, and there are different modes of cursor movement.
               In "visual mode", this moves left, plain and simple. In "logical
               mode", movement is logically forward in RTL paragraphs, and
               logically backwards in LTR paragraphs.
 * \li Syntax: char-left
 */
		{ LFUN_CHAR_LEFT, "char-left", ReadOnly | NoUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_LEFT_SELECT
 * \li Action: Moves the cursor one position "to the left", adding 
               traversed position to the selection.
 * \li Notion: See also #LFUN_CHAR_LEFT for exact details of the movement.
 * \li Syntax: char-left-select
 */
		{ LFUN_CHAR_LEFT_SELECT, "char-left-select", ReadOnly | SingleParUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_RIGHT
 * \li Action: Moves the cursor one position "to the right".
 * \li Notion: This is the action which should be taken when the "right" key
               is pressed. Generally, it moves the cursor one position to the
               right. However, in Bidi text this become slightly more
               complicated, and there are different modes of cursor movement.
               In "visual mode", this moves right, plain and simple. In "logical
               mode", movement is logically forward in LTR paragraphs, and
               logically backwards in RTL paragraphs.
 * \li Syntax: char-right
 */
		{ LFUN_CHAR_RIGHT, "char-right", ReadOnly | NoUpdate, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CHAR_RIGHT_SELECT
 * \li Action: Moves the cursor one position "to the right", adding
               traversed position to the selection.
 * \li Notion: See also #LFUN_CHAR_RIGHT for exact details of the movement.
 * \li Syntax: char-right-select
 */
		{ LFUN_CHAR_RIGHT_SELECT, "char-right-select", ReadOnly | SingleParUpdate, Edit },

		{ LFUN_WORD_BACKWARD, "word-backward", ReadOnly | NoUpdate, Edit },
		{ LFUN_WORD_BACKWARD_SELECT, "word-backward-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_WORD_CAPITALIZE, "word-capitalize", Noop, Edit },
		{ LFUN_WORD_DELETE_BACKWARD, "word-delete-backward", Noop, Edit },
		{ LFUN_WORD_DELETE_FORWARD, "word-delete-forward", Noop, Edit },
		{ LFUN_WORD_FIND_BACKWARD, "word-find-backward", ReadOnly, Edit },
		{ LFUN_WORD_FIND_FORWARD, "word-find-forward", ReadOnly, Edit },
		{ LFUN_WORD_FORWARD, "word-forward", ReadOnly | NoUpdate, Edit },
		{ LFUN_WORD_FORWARD_SELECT, "word-forward-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_WORD_LEFT, "word-left", ReadOnly | NoUpdate, Edit },
		{ LFUN_WORD_LEFT_SELECT, "word-left-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_WORD_LOWCASE, "word-lowcase", Noop, Edit },
		{ LFUN_WORD_RIGHT, "word-right", ReadOnly | NoUpdate, Edit },
		{ LFUN_WORD_RIGHT_SELECT, "word-right-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_WORD_SELECT, "word-select", ReadOnly, Edit },
		{ LFUN_WORD_UPCASE, "word-upcase", Noop, Edit },
		{ LFUN_WORD_FIND, "word-find", ReadOnly, Edit },
		{ LFUN_WORD_REPLACE, "word-replace", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_THESAURUS_ENTRY
 * \li Action: Look up thesaurus entries with respect to the word under the cursor.
 * \li Syntax: thesaurus-entry
 * \li Origin: Levon, 20 Jul 2001
 */
		{ LFUN_THESAURUS_ENTRY, "thesaurus-entry", ReadOnly, Edit },
		{ LFUN_BUFFER_BEGIN, "buffer-begin", ReadOnly, Edit },
		{ LFUN_BUFFER_BEGIN_SELECT, "buffer-begin-select", ReadOnly, Edit },
		{ LFUN_BUFFER_END, "buffer-end", ReadOnly, Edit },
		{ LFUN_BUFFER_END_SELECT, "buffer-end-select", ReadOnly, Edit },

		{ LFUN_LINE_BEGIN, "line-begin", ReadOnly | NoUpdate, Edit },
		{ LFUN_LINE_BEGIN_SELECT, "line-begin-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_LINE_BREAK, "line-break", Noop, Edit },
		{ LFUN_LINE_DELETE, "line-delete-forward", Noop, Edit }, // there is no line-delete-backward
		{ LFUN_LINE_END, "line-end", ReadOnly | NoUpdate, Edit },
		{ LFUN_LINE_END_SELECT, "line-end-select", ReadOnly | SingleParUpdate, Edit },
		{ LFUN_NEW_LINE, "new-line", Noop, Edit },

/**
 * \var lyx::kb_action lyx::LFUN_PARAGRAPH_MOVE_DOWN
 * \li Action: Moves the current paragraph downwards in the document.
 * \li Syntax: paragraph-move-down
 * \li Origin: Edwin, 8 Apr 2006
 */
		{ LFUN_PARAGRAPH_MOVE_DOWN, "paragraph-move-down", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_PARAGRAPH_MOVE_UP
 * \li Action: Moves the current paragraph upwards in the document.
 * \li Syntax: paragraph-move-up
 * \li Origin: Edwin, 8 Apr 2006
 */
		{ LFUN_PARAGRAPH_MOVE_UP, "paragraph-move-up", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_COPY
 * \li Action: Copies to the clipboard the last edit.
 * \li Syntax: copy
 */
		{ LFUN_COPY, "copy", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_CUT
 * \li Action: Cuts to the clipboard.
 * \li Syntax: cut
 */
		{ LFUN_CUT, "cut", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_PASTE
 * \li Action: Pastes from the active clipboard.
 * \li Syntax: paste
 */
		{ LFUN_PASTE, "paste", Noop, Edit },
		{ LFUN_CLIPBOARD_PASTE, "clipboard-paste", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_PRIMARY_SELECTION_PASTE
 * \li Action: Pastes the material currently selected.
 * \li Syntax: primary-selection-paste
 */
		{ LFUN_PRIMARY_SELECTION_PASTE, "primary-selection-paste", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_UNDO
 * \li Action: Undoes the last edit.
 * \li Syntax: undo
 */
		{ LFUN_UNDO, "undo", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_REDO
 * \li Action: Redoes the last thing undone.
 * \li Syntax: redo
 */
		{ LFUN_REDO, "redo", Noop, Edit },
		{ LFUN_REPEAT, "repeat", NoBuffer, Edit },
		{ LFUN_CHARS_TRANSPOSE, "chars-transpose", Noop, Edit },

		{ LFUN_DEPTH_DECREMENT, "depth-decrement", Noop, Edit },
		{ LFUN_DEPTH_INCREMENT, "depth-increment", Noop, Edit },
		{ LFUN_ENVIRONMENT_INSERT, "environment-insert", Noop, Edit },

		{ LFUN_FONT_BOLD, "font-bold", Noop, Layout },
		{ LFUN_FONT_TYPEWRITER, "font-typewriter", Noop, Layout },
		{ LFUN_FONT_DEFAULT, "font-default", Noop, Layout },
		{ LFUN_FONT_EMPH, "font-emph", Noop, Layout },
		{ LFUN_FONT_FREE_APPLY, "font-free-apply", Noop, Layout },
		{ LFUN_FONT_FREE_UPDATE, "font-free-update", Noop, Layout },
		{ LFUN_FONT_NOUN, "font-noun", Noop, Layout },
		{ LFUN_FONT_ROMAN, "font-roman", Noop, Layout },
		{ LFUN_FONT_SANS, "font-sans", Noop, Layout },
		{ LFUN_FONT_FRAK, "font-frak", Noop, Layout },
		{ LFUN_FONT_ITAL, "font-ital", Noop, Layout },
		{ LFUN_FONT_SIZE, "font-size", Noop, Layout },
		{ LFUN_FONT_STATE, "font-state", ReadOnly, Layout },
		{ LFUN_FONT_UNDERLINE, "font-underline", Noop, Layout },
		{ LFUN_SCREEN_FONT_UPDATE, "screen-font-update", NoBuffer, Layout },

		{ LFUN_INDEX_INSERT, "index-insert", Noop, Edit },
		{ LFUN_INDEX_PRINT, "index-print", Noop, Edit },

		{ LFUN_NOMENCL_INSERT, "nomencl-insert", Noop, Edit },
		{ LFUN_NOMENCL_PRINT, "nomencl-print", Noop, Edit },

		{ LFUN_NOTE_INSERT, "note-insert", Noop, Edit },
		{ LFUN_NOTE_NEXT, "note-next", ReadOnly, Edit },

		{ LFUN_BIBITEM_INSERT, "bibitem-insert", Noop, Edit },
		{ LFUN_CITATION_INSERT, "citation-insert", Noop, Edit },
		{ LFUN_BIBTEX_DATABASE_ADD, "bibtex-database-add", Noop, Edit },
		{ LFUN_BIBTEX_DATABASE_DEL, "bibtex-database-del", Noop, Edit },

 /**
 * \var lyx::kb_action lyx::LFUN_LAYOUT
 * \li Action: Sets the layout (that is, environment) for the current paragraph.
 * \li Syntax: layout <LAYOUT>
 * \li Params: <LAYOUT>: the layout to use
 */
		{ LFUN_LAYOUT, "layout", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_LAYOUT_PARAGRAPH
 * \li Action: Launches the paragraph settings dialog.
 * \li Syntax: layout-paragraph
 */
		{ LFUN_LAYOUT_PARAGRAPH, "layout-paragraph", ReadOnly, Layout },
		{ LFUN_LAYOUT_TABULAR, "layout-tabular", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_DROP_LAYOUTS_CHOICE
 * \li Action: Displays list of layout choices.
 * \li Notion: In the current (as of 2007) Qt4 frontend, this LFUN opens the
               dropbox allowing for choice of layout.
 * \li Syntax: drop-layouts-choice
 */
		{ LFUN_DROP_LAYOUTS_CHOICE, "drop-layouts-choice", ReadOnly, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_LAYOUT_MODULES_CLEAR
 * \li Action: Clears the module list.
 * \li Notion: Clears the list of included modules for the current buffer.
 * \li Syntax: layout-modules-clear
 * \li Origin: rgh, 25 August 2007
 */
		{ LFUN_LAYOUT_MODULES_CLEAR, "layout-modules-clear", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_LAYOUT_MODULE_ADD
 * \li Action: Adds a module.
 * \li Notion: Adds a module to the list of included modules for the current buffer.
 * \li Syntax: layout-module-add <MODULE>
 * \li Params: <MODULE>: the module to be added
 * \li Origin: rgh, 25 August 2007
 */
		{ LFUN_LAYOUT_MODULE_ADD, "layout-module-add", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_LAYOUT_RELOAD
 * \li Action: Reloads layout information.
 * \li Notion: Reloads all layout information for the current buffer from disk, thus
               recognizing any changes that have been made to layout files on the fly.
               This is intended to be used only by layout developers and should not be
               used when one is trying to do actual work.
 * \li Syntax: layout-reload
 * \li Origin: rgh, 3 September 2007
 */
		{ LFUN_LAYOUT_RELOAD, "layout-reload", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_TEXTCLASS_APPLY
 * \li Action: Sets the text class for the current buffer.
 * \li Syntax: textclass-apply <TEXTCLASS>
 * \li Params: <TEXTCLASS>: the textclass to set. Note that this must be
                   the filename, minus the ".layout" extension.
 */
		{ LFUN_TEXTCLASS_APPLY, "textclass-apply", Noop, Layout },
/**
 * \var lyx::kb_action lyx::LFUN_TEXTCLASS_LOAD
 * \li Action: Loads information for a textclass from disk.
 * \li Syntax: textclass-load <TEXTCLASS>
 * \li Params: <TEXTCLASS>: the textclass to load. Note that this must be
                   the filename, minus the ".layout" extension.
 */
		{ LFUN_TEXTCLASS_LOAD, "textclass-load", Noop, Layout },

/**
 * \var lyx::kb_action lyx::LFUN_MARK_OFF
 * \li Action: Disable selecting of text-region.
 * \li Syntax: mark-off
 */
		{ LFUN_MARK_OFF, "mark-off", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_MARK_ON
 * \li Action: Enable selecting of text-region.
 * \li Notion: After enabling you can simply move arrow keys to get selected region.
 * \li Syntax: mark-on
 */
		{ LFUN_MARK_ON, "mark-on", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_MARK_TOGGLE
 * \li Action: Toggle between #LFUN_MARK_ON and #LFUN_MARK_OFF .
 * \li Syntax: mark-toggle
 * \li Origin: poenitz, May 5 2006
 */
		{ LFUN_MARK_TOGGLE, "mark-toggle", ReadOnly, Edit },
		
		{ LFUN_MATH_DELIM, "math-delim", Noop, Math },
		{ LFUN_MATH_BIGDELIM, "math-bigdelim", Noop, Math },
		{ LFUN_MATH_DISPLAY, "math-display", Noop, Math },
		{ LFUN_MATH_INSERT, "math-insert", Noop, Math },
		{ LFUN_MATH_SUBSCRIPT, "math-subscript", Noop, Math },
		{ LFUN_MATH_SUPERSCRIPT, "math-superscript", Noop, Math },
		{ LFUN_MATH_LIMITS, "math-limits", Noop, Math },
		{ LFUN_MATH_MACRO, "math-macro", Noop, Math },
		{ LFUN_MATH_MUTATE, "math-mutate", Noop, Math },
		{ LFUN_MATH_SPACE, "math-space", Noop, Math },
		{ LFUN_MATH_IMPORT_SELECTION, "math-import-selection", Noop, Math },
		{ LFUN_MATH_MATRIX, "math-matrix", Noop, Math },
		{ LFUN_MATH_MODE, "math-mode", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_NUMBER_LINE_TOGGLE
 * \li Action: Toggles numbering of the current formula line.
 * \li Notion: Must be in display formula mode.
 * \li Syntax: math-number-line-toggle
 * \li Origin: Alejandro 18 Jun 1996
 */
		{ LFUN_MATH_NUMBER_LINE_TOGGLE, "math-number-line-toggle", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_NUMBER_TOGGLE
 * \li Action: Toggles numbering of the current formula.
 * \li Notion: Must be in display formula mode.
 * \li Syntax: math-number-toggle
 * \li Origin: Alejandro 4 Jun 1996
 */
		{ LFUN_MATH_NUMBER_TOGGLE, "math-number-toggle", Noop, Math },
		{ LFUN_MATH_EXTERN, "math-extern", Noop, Math },
		{ LFUN_MATH_SIZE, "math-size", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_UNFOLD
 * \li Action: Unfold a Math Macro.
 * \li Notion: Unfold the Math Macro the cursor is in, i.e.
               display it as \foo.
 * \li Syntax: math-macro-unfold
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_UNFOLD, "math-macro-unfold", ReadOnly | SingleParUpdate, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_FOLD
 * \li Action: Fold a Math Macro.
 * \li Notion: Fold the Math Macro the cursor is in if it was
               unfolded, i.e. displayed as \foo before.
 * \li Syntax: math-macro-fold
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_FOLD, "math-macro-fold", ReadOnly | SingleParUpdate, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_ADD_PARAM
 * \li Action: Add a parameter.
 * \li Notion: Add a parameter to a Math Macro.
 * \li Params: <NUM>: The number of the parameter behind which the new one
               will be added (1 for the first, i.e. use 0 for add a
               parameter at the left), defaults to the last one.
 * \li Syntax: math-macro-add-param <NUM>
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_ADD_PARAM, "math-macro-add-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_REMOVE_PARAM
 * \li Action: Remove the last parameter.
 * \li Notion: Remove the last parameter of a Math Macro and
               remove its value in all instances of the macro
               in the buffer.
 * \li Params: <NUM>: The number of the parameter to be deleted (1 for
               the first), defaults to the last one.
 * \li Syntax: math-macro-remove-param <NUM>
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_REMOVE_PARAM, "math-macro-remove-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_APPEND_GREEDY_PARAM
 * \li Action: Append a greedy parameter.
 * \li Notion: Append a greedy parameter to a Math Macro which
               eats the following mathed cell in every instance of
               the macro in the buffer.
 * \li Syntax: math-macro-append-greedy-param
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_APPEND_GREEDY_PARAM, "math-macro-append-greedy-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_REMOVE_GREEDY_PARAM
 * \li Action: Remove a greedy parameter.
 * \li Notion: Remove a greedy parameter of a Math Macro and spit
               out the values of it in every instance of the macro
               in the buffer. If it is an optional parameter the [valud]
               format is used.
 * \li Syntax: math-macro-remove-greedy-param
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_REMOVE_GREEDY_PARAM, "math-macro-remove-greedy-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_MAKE_OPTIONAL
 * \li Action: Make a parameter optional.
 * \li Notion: Turn the first non-optional parameter of a Math Macro
               into an optional parameter with a default value.
 * \li Syntax: math-macro-make-optional
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_MAKE_OPTIONAL, "math-macro-make-optional", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_MAKE_NONOPTIONAL
 * \li Action: Make a parameter non-optional.
 * \li Notion: Turn the last optional parameter of a Math Macro
               into a non-optional parameter. The default value is
               remembered to be reused later if the user changes his mind.
 * \li Syntax: math-macro-make-nonoptional
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_MAKE_NONOPTIONAL, "math-macro-make-nonoptional", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_ADD_OPTIONAL_PARAM
 * \li Action: Add an optional parameter.
 * \li Notion: Insert an optional parameter just behind the
               already existing optional parameters.
 * \li Syntax: math-macro-add-optional-param
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_ADD_OPTIONAL_PARAM, "math-macro-add-optional-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_REMOVE_OPTIONAL_PARAM
 * \li Action: Remove the last optional parameter.
 * \li Notion: Remove the last optional parameter of a Math Macro and
               remove it in all the instances of the macro in the buffer.
 * \li Syntax: math-macro-remove-optional-param
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_REMOVE_OPTIONAL_PARAM, "math-macro-remove-optional-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_MATH_MACRO_ADD_GREEDY_OPTIONAL_PARAM
 * \li Action: Add a greedy optional parameter.
 * \li Notion: Add a greedy optional parameter which eats the value
               from the following cells in mathed which are in the [value]
               format.
 * \li Syntax: math-macro-add-greedy-optional-param
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_MATH_MACRO_ADD_GREEDY_OPTIONAL_PARAM, "math-macro-add-greedy-optional-param", Noop, Math },
/**
 * \var lyx::kb_action lyx::LFUN_IN_MATHMACROTEMPLATE
 * \li Action: Only active in Math Macro definition.
 * \li Notion: Dummy function which is only active in a Math Macro definition.
               It's used to toggle the Math Macro toolbar if the cursor moves
               into a Math Macro definition.
 * \li Syntax: in-mathmacrotemplate
 * \li Origin: sts, 06 January 2008
 */
		{ LFUN_IN_MATHMACROTEMPLATE, "in-mathmacrotemplate", Noop, Math },

		{ LFUN_PARAGRAPH_DOWN, "paragraph-down", ReadOnly | NoUpdate, Edit },
		{ LFUN_PARAGRAPH_DOWN_SELECT, "paragraph-down-select", ReadOnly, Edit },
		{ LFUN_PARAGRAPH_GOTO, "paragraph-goto", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_OUTLINE_UP
 * \li Action: Move the current group in the upward direction in the
               structure of the document.
 * \li Notion: The "group" can be Part/Chapter/Section/etc. It moves
               the whole substructure of the group.
 * \li Syntax: outline-up
 * \li Origin: Vermeer, 23 Mar 2006
 */
		{ LFUN_OUTLINE_UP, "outline-up", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_OUTLINE_DOWN
 * \li Action: Move the current group in the downward direction in the
               structure of the document.
 * \li Notion: The "group" can be Part/Chapter/Section/etc. It moves
               the whole substructure of the group.
 * \li Syntax: outline-down
 * \li Origin: Vermeer, 23 Mar 2006
 */
		{ LFUN_OUTLINE_DOWN, "outline-down", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_OUTLINE_IN
 * \li Action: Moves the current group in the downward direction in the
               hierarchy of the document structure.
 * \li Notion: Part -> Chapter -> Section -> etc.
 * \li Syntax: outline-in
 * \li Origin: Vermeer, 23 Mar 2006
 */
		{ LFUN_OUTLINE_IN, "outline-in", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_OUTLINE_OUT
 * \li Action: Moves the current group in the upward direction in the
               hierarchy of the document structure.
 * \li Notion: Part <- Chapter <- Section <- etc.
 * \li Syntax: outline-out
 * \li Origin: Vermeer, 23 Mar 2006
 */
		{ LFUN_OUTLINE_OUT, "outline-out", Noop, Edit },

		{ LFUN_PARAGRAPH_SPACING, "paragraph-spacing", Noop, Edit },
		{ LFUN_PARAGRAPH_UP, "paragraph-up", ReadOnly | NoUpdate, Edit },
		{ LFUN_PARAGRAPH_UP_SELECT, "paragraph-up-select", ReadOnly, Edit },

		{ LFUN_EXTERNAL_EDIT, "external-edit", Noop, Edit },
		{ LFUN_GRAPHICS_EDIT, "graphics-edit", Noop, Edit },

		{ LFUN_CELL_BACKWARD, "cell-backward", Noop, Edit },
		{ LFUN_CELL_FORWARD, "cell-forward", Noop, Edit },
		{ LFUN_CELL_SPLIT, "cell-split", Noop, Edit },
		{ LFUN_TABULAR_INSERT, "tabular-insert", Noop, Edit },
		{ LFUN_TABULAR_FEATURE, "tabular-feature", Noop, Edit },

		{ LFUN_VC_CHECK_IN, "vc-check-in", ReadOnly, System },
		{ LFUN_VC_CHECK_OUT, "vc-check-out", ReadOnly, System },
		{ LFUN_VC_REGISTER, "vc-register", ReadOnly, System },
		{ LFUN_VC_REVERT, "vc-revert", ReadOnly, System },
		{ LFUN_VC_UNDO_LAST, "vc-undo-last", ReadOnly, System },

		{ LFUN_CHANGES_TRACK, "changes-track", Noop, Edit },
		{ LFUN_CHANGES_OUTPUT, "changes-output", Noop, Edit },
		{ LFUN_CHANGE_NEXT, "change-next", ReadOnly, Edit },
		{ LFUN_CHANGES_MERGE, "changes-merge", Noop, Edit },
		{ LFUN_CHANGE_ACCEPT, "change-accept", Noop, Edit },
		{ LFUN_CHANGE_REJECT, "change-reject", Noop, Edit },
		{ LFUN_ALL_CHANGES_ACCEPT, "all-changes-accept", Noop, Edit },
		{ LFUN_ALL_CHANGES_REJECT, "all-changes-reject", Noop, Edit },

		{ LFUN_INSET_APPLY, "inset-apply", Noop, Edit },
		{ LFUN_INSET_DISSOLVE, "inset-dissolve", Noop, Edit },
		{ LFUN_INSET_INSERT, "inset-insert", Noop, Edit },
		{ LFUN_INSET_MODIFY, "", Noop, Hidden },
		{ LFUN_INSET_DIALOG_UPDATE, "", Noop, Hidden },
		{ LFUN_INSET_SETTINGS, "inset-settings", ReadOnly, Edit },
		{ LFUN_INSET_REFRESH, "", Noop, Hidden },
		{ LFUN_NEXT_INSET_TOGGLE, "next-inset-toggle", ReadOnly, Edit },
		{ LFUN_INSET_TOGGLE, "", ReadOnly, Hidden },
		{ LFUN_ALL_INSETS_TOGGLE, "all-insets-toggle", ReadOnly, Edit },

/**
 * \var lyx::kb_action lyx::LFUN_PARAGRAPH_PARAMS
 * \li Action: Change paragraph settings.
 * \li Notion: Modifies the current paragraph, or currently selected paragraphs.
               This function only modifies, and does not override, existing settings.
               Note that the "leftindent" indent setting is deprecated.
 * \li Syntax: paragraph-params [<INDENT>] [<SPACING>] [<ALIGN>] [<OTHERS>]
 * \li Params: <INDENT>:  \\noindent|\\indent|\\indent-toggle|\\leftindent LENGTH\n
               <SPACING>: \\paragraph_spacing default|single|onehalf|double|other\n
               <ALIGN>:   \\align block|left|right|center|default\n
               <OTHERS>:  \\labelwidthstring WIDTH|\\start_of_appendix\n
 * \li Origin: rgh, Aug 15 2007
 */
		{ LFUN_PARAGRAPH_PARAMS, "paragraph-params", Noop, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_PARAGRAPH_PARAMS_APPLY
 * \li Action: Change paragraph settings.
 * \li Notion: Overwrite all nonspecified settings to the default ones.
               Use paragraph-params lfun if you don't want to overwrite others settings.
 * \li Syntax: paragraph-params-apply <INDENT> <SPACING> <ALIGN> <OTHERS>
 * \li Params: For parameters see LFUN_PARAGRAPH_PARAMS
 * \li Origin: leeming, 30 Mar 2004
 */
		{ LFUN_PARAGRAPH_PARAMS_APPLY, "paragraph-params-apply", Noop, Edit },
		{ LFUN_PARAGRAPH_UPDATE, "", Noop, Hidden },

/**
 * \var lyx::kb_action lyx::LFUN_FINISHED_FORWARD
 * \li Action: Moves the cursor out of the current slice, going forward.
 * \li Notion: Cursor movement within an inset may be different than cursor
               movement in the surrounding text. This action should be called
               automatically by the cursor movement within the inset, when
               movement within the inset has ceased (reached the end of the
               last paragraph, for example), in order to move correctly
               back into the surrounding text.
 * \li Syntax: finished-forward
 */
		{ LFUN_FINISHED_FORWARD, "", ReadOnly, Hidden },
/**
 * \var lyx::kb_action lyx::LFUN_FINISHED_BACKWARD
 * \li Action: Moves the cursor out of the current slice, going backwards.
 * \li Notion: See also #LFUN_FINISHED_FORWARD.
 * \li Syntax: finished-backward
 */
		{ LFUN_FINISHED_BACKWARD, "", ReadOnly, Hidden },
/**
 * \var lyx::kb_action lyx::LFUN_FINISHED_RIGHT
 * \li Action: Moves the cursor out of the current slice, going right.
 * \li Notion: See also #LFUN_FINISHED_FORWARD
 * \li Syntax: finished-right
 */
		{ LFUN_FINISHED_RIGHT, "", ReadOnly, Hidden },
/**
 * \var lyx::kb_action lyx::LFUN_FINISHED_LEFT
 * \li Action: Moves the cursor out of the current slice, going left.
 * \li Notion: See also #LFUN_FINISHED_FORWARD.
 * \li Syntax: finished-left
 */
		{ LFUN_FINISHED_LEFT, "", ReadOnly, Hidden },

		{ LFUN_LANGUAGE, "language", Noop, Edit },

		{ LFUN_LABEL_GOTO, "label-goto", ReadOnly, Edit },
		{ LFUN_LABEL_INSERT, "label-insert", Noop, Edit },
		{ LFUN_REFERENCE_NEXT, "reference-next", ReadOnly, Edit },

/**
 * \var lyx::kb_action lyx::LFUN_BOOKMARK_GOTO
 * \li Action: Goto a bookmark.
 * \li Notion: Moves the cursor to the numbered bookmark, opening the file
               if necessary. Note that bookmarsk are saved per-session, not
               per file.
 * \li Syntax: bookmark-goto <NUMBER>
 * \li Params: <NUMBER>: the number of the bookmark to restore.
 * \li Origin: Dekel, 27 January 2001
 */
		{ LFUN_BOOKMARK_GOTO, "bookmark-goto", NoBuffer, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_BOOKMARK_SAVE
 * \li Action: Save a bookmark.
 * \li Notion: Saves a numbered bookmark to the sessions file. The number
               must be between 1 and 9, inclusive. Note that bookmarks are
               saved per-session, not per file.
 * \li Syntax: bookmark-save <NUMBER>
 * \li Params: <NUMBER>: the number of the bookmark to save.
 * \li Origin: Dekel, 27 January 2001
 */
		{ LFUN_BOOKMARK_SAVE, "bookmark-save", ReadOnly, Edit },
/**
 * \var lyx::kb_action lyx::LFUN_BOOKMARK_CLEAR
 * \li Action: Clears the list of saved bookmarks.
 * \li Syntax: bookmark-clear
 * \li Origin: bpeng, 31 October 2006
 */
		{ LFUN_BOOKMARK_CLEAR, "bookmark-clear", NoBuffer, Edit },

		{ LFUN_HELP_OPEN, "help-open", NoBuffer | Argument, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_LYX_QUIT
 * \li Action: Terminates the current LyX instance.
 * \li Notion: Terminates the current LyX instance, asking whether to save
               modified documents, etc.
 * \li Syntax: lyx-quit
 */
		{ LFUN_LYX_QUIT, "lyx-quit", NoBuffer, Buffer },
		{ LFUN_TOOLBAR_TOGGLE, "toolbar-toggle", NoBuffer, Buffer },
		{ LFUN_MENU_OPEN, "menu-open", NoBuffer, Buffer },

		{ LFUN_WINDOW_NEW, "window-new", NoBuffer, Buffer },
		{ LFUN_WINDOW_CLOSE, "window-close", NoBuffer, Buffer },

		{ LFUN_DIALOG_SHOW, "dialog-show", NoBuffer, Edit },
		{ LFUN_DIALOG_SHOW_NEW_INSET, "dialog-show-new-inset", Noop, Edit },
		{ LFUN_DIALOG_UPDATE, "dialog-update", NoBuffer, Edit },
		{ LFUN_DIALOG_HIDE, "dialog-hide", NoBuffer, Edit },
		{ LFUN_DIALOG_TOGGLE, "dialog-toggle", NoBuffer, Edit },
		{ LFUN_DIALOG_DISCONNECT_INSET, "dialog-disconnect-inset", Noop, Edit },

		{ LFUN_MOUSE_PRESS, "", ReadOnly, Hidden },
		{ LFUN_MOUSE_MOTION, "", ReadOnly | SingleParUpdate, Hidden },
		{ LFUN_MOUSE_RELEASE, "", ReadOnly, Hidden },
		{ LFUN_MOUSE_DOUBLE, "", ReadOnly, Hidden },
		{ LFUN_MOUSE_TRIPLE, "", ReadOnly, Hidden },

		{ LFUN_KEYMAP_OFF, "keymap-off", ReadOnly, Edit },
		{ LFUN_KEYMAP_PRIMARY, "keymap-primary", ReadOnly, Edit },
		{ LFUN_KEYMAP_SECONDARY, "keymap-secondary", ReadOnly, Edit },
		{ LFUN_KEYMAP_TOGGLE, "keymap-toggle", ReadOnly, Edit },

		{ LFUN_MESSAGE, "message", NoBuffer, System },
		{ LFUN_FLOAT_LIST, "float-list", Noop, Edit },
		{ LFUN_ESCAPE, "escape", ReadOnly, Edit },

		{ LFUN_SERVER_CHAR_AFTER, "server-char-after", ReadOnly, System },
		{ LFUN_SERVER_GET_FONT, "server-get-font", ReadOnly, System },
		{ LFUN_SERVER_GET_LAYOUT, "server-get-layout", ReadOnly, System },
		{ LFUN_SERVER_GET_NAME, "server-get-name", ReadOnly, System },
		{ LFUN_SERVER_GET_XY, "server-get-xy", ReadOnly, System },
		{ LFUN_SERVER_GOTO_FILE_ROW, "server-goto-file-row", ReadOnly, System },
		{ LFUN_SERVER_NOTIFY, "server-notify", ReadOnly, System },
		{ LFUN_SERVER_SET_XY, "server-set-xy", ReadOnly, System },

		{ LFUN_BUILD_PROGRAM, "build-program", ReadOnly, Buffer },

/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_AUTO_SAVE
 * \li Action: Saves the current buffer to a temporary file.
 * \li Notion: Saves the current buffer to a file named "#filename#". This LFUN
               is called automatically by LyX, to "autosave" the current buffer.
	* \li Syntax: buffer-auto-save
 */
		{ LFUN_BUFFER_AUTO_SAVE, "buffer-auto-save", Noop, Buffer },
		{ LFUN_BUFFER_CHILD_OPEN, "buffer-child-open", ReadOnly, Buffer },
		{ LFUN_BUFFER_CHKTEX, "buffer-chktex", ReadOnly, Buffer },
		{ LFUN_BUFFER_TOGGLE_COMPRESSION, "buffer-toggle-compression", Noop, Buffer },
		{ LFUN_BUFFER_TOGGLE_EMBEDDING, "buffer-toggle-embedding", Noop, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_CLOSE
 * \li Action: Closes the current buffer.
 * \li Notion: Closes the current buffer, asking whether to save it, etc,
               if the buffer has been modified.
 * \li Syntax: buffer-close
 */
		{ LFUN_BUFFER_CLOSE, "buffer-close", ReadOnly, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_EXPORT
 * \li Action: Exports the current buffer (document) to the given format.
 * \li Syntax: buffer-export <FORMAT>
 * \li Params: <FORMAT> is either "custom" or one of the formats which you
                        can find in Tools->Preferences->File formats->Format.
			Usual format you will enter is "pdf2" (pdflatex),
			"pdflatex" (plain tex for pdflatex) or "ps" for postscript.
 * \li Origin: Lgb, 29 Jul 1997
 */
		{ LFUN_BUFFER_EXPORT, "buffer-export", ReadOnly, Buffer },
		{ LFUN_BUFFER_EXPORT_CUSTOM, "buffer-export-custom", ReadOnly, Buffer },
		{ LFUN_BUFFER_PRINT, "buffer-print", ReadOnly, Buffer },
		{ LFUN_BUFFER_IMPORT, "buffer-import", NoBuffer, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_NEW
 * \li Action: Creates a new buffer (that is, document).
 * \li Syntax: buffer-new
 */
		{ LFUN_BUFFER_NEW, "buffer-new", NoBuffer, Buffer },
		{ LFUN_BUFFER_NEW_TEMPLATE,"buffer-new-template", NoBuffer, Buffer },
		{ LFUN_BUFFER_RELOAD, "buffer-reload", ReadOnly, Buffer },
		{ LFUN_BUFFER_SWITCH, "buffer-switch", NoBuffer | ReadOnly, Buffer },
		{ LFUN_BUFFER_TOGGLE_READ_ONLY, "buffer-toggle-read-only", ReadOnly, Buffer },
		{ LFUN_BUFFER_UPDATE, "buffer-update", ReadOnly, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_VIEW
 * \li Action: Displays current buffer in chosen format.
 * \li Notion: Displays the contents of the current buffer in the chosen
               format, for example, PDF or DVI. This runs the necessary
               converter, calls the defined viewer, and so forth.
 * \li Syntax: buffer-view <FORMAT>
 * \li Params: <FORMAT>: The format to display, where this is one of the
                         formats defined (in the current GUI) in the
                         Tools>Preferences>File Formats dialog.
 */
		{ LFUN_BUFFER_VIEW, "buffer-view", ReadOnly, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_WRITE
 * \li Action: Saves the current buffer.
 * \li Notion: Saves the current buffer to disk, using the filename that
               is already associated with the buffer, asking for one if
               none is yet assigned.
 * \li Syntax: buffer-write
 */
		{ LFUN_BUFFER_WRITE, "buffer-write", ReadOnly, Buffer },
/**
 * \var lyx::kb_action lyx::LFUN_BUFFER_WRITE_AS
 * \li Action: Rename and save current buffer.
 * \li Syntax: buffer-write-as <FILENAME>
 * \li Params: <FILENAME>: New name of the buffer/file. A relative path
 *             is with respect to the original location of the buffer/file.
 */
		{ LFUN_BUFFER_WRITE_AS, "buffer-write-as", ReadOnly, Buffer },
		{ LFUN_BUFFER_WRITE_ALL, "buffer-write-all", ReadOnly, Buffer },
		{ LFUN_BUFFER_NEXT, "buffer-next", ReadOnly, Buffer },
		{ LFUN_BUFFER_PREVIOUS, "buffer-previous", ReadOnly, Buffer },
		{ LFUN_MASTER_BUFFER_UPDATE, "master-buffer-update", ReadOnly, Buffer },
		{ LFUN_MASTER_BUFFER_VIEW, "master-buffer-view", ReadOnly, Buffer },
		{ LFUN_BUFFER_LANGUAGE, "buffer-language", Noop, Buffer },
		{ LFUN_BUFFER_SAVE_AS_DEFAULT, "buffer-save-as-default", Noop, Buffer },
		{ LFUN_BUFFER_PARAMS_APPLY, "buffer-params-apply", Noop, Buffer },

		{ LFUN_FILE_INSERT, "file-insert", Noop, Edit },
		{ LFUN_FILE_INSERT_PLAINTEXT, "file-insert-plaintext", Noop, Edit },
		{ LFUN_FILE_INSERT_PLAINTEXT_PARA, "file-insert-plaintext-para", Noop, Edit },
		{ LFUN_FILE_NEW, "file-new", NoBuffer, Buffer },
		{ LFUN_FILE_OPEN, "file-open", NoBuffer, Buffer },


/**
 * \var lyx::kb_action lyx::LFUN_CALL
 * \li Action: Executes a command defined in a .def file.
 * \li Notion: The definitions are by default read from lib/commands/default.def .
               A .def file allows to define a command with \\define "<NAME>" "<LFUN>"
               where <NAME> is the name of the new command and <LFUN> is the lfun code
               to be executed (see e.g. #LFUN_COMMAND_SEQUENCE).
               \\def_file "FileName" allows to include another .def file. \n
               This is particularly useful in connection with toolbar buttons:
               Since the name of the button image for this lfun is 
               lib/images/commands/<NAME>.png this is the way to assign an image
               to a complex command-sequence.
 * \li Syntax: call <NAME>
 * \li Params: <NAME>: Name of the command that must be called.
 * \li Origin: broider, 2 Oct 2007
 */
		{ LFUN_CALL, "call", NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_META_PREFIX
 * \li Action: Simulate halting Meta key (Alt key on PCs).
 * \li Notion: Used for buffer editation not for GUI control.
 * \li Syntax: meta-prefix
 */
		{ LFUN_META_PREFIX, "meta-prefix", NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_CANCEL
 * \li Action: Cancels sequence prepared by #LFUN_META_PREFIX .
 * \li Syntax: cancel
 */
		{ LFUN_CANCEL, "cancel", NoBuffer, System },

/**
 * \var lyx::kb_action lyx::LFUN_COMMAND_EXECUTE
 * \li Action: Opens the minibuffer toolbar so that the user can type in there.
 * \li Notion: Usually bound to M-x shortcut.
 * \li Syntax: command-execute
 */
		{ LFUN_COMMAND_EXECUTE, "command-execute", NoBuffer, Edit },
		{ LFUN_COMMAND_PREFIX, "command-prefix", NoBuffer, Hidden },
/**
 * \var lyx::kb_action lyx::LFUN_COMMAND_SEQUENCE
 * \li Action: Run more commands (LFUN and its parameters) in a sequence.
 * \li Syntax: command-sequence <CMDS>
 * \li Params: <CMDS>: Sequence of commands separated by semicolons.
 * \li Sample: command-sequence cut; ert-insert; self-insert \; paste; self-insert {}; inset-toggle;
 * \li Origin: Andre', 11 Nov 1999
 */
		{ LFUN_COMMAND_SEQUENCE, "command-sequence", NoBuffer, System },

/**
 * \var lyx::kb_action lyx::LFUN_PREFERENCES_SAVE
 * \li Action: Save user preferences.
 * \li Syntax: preferences-save
 * \li Origin: Lgb, 27 Nov 1999
 */
		{ LFUN_PREFERENCES_SAVE, "preferences-save", NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_RECONFIGURE
 * \li Action: Reconfigure the automatic settings.
 * \li Syntax: reconfigure
 * \li Origin: Asger, 14 Feb 1997
 */
		{ LFUN_RECONFIGURE, "reconfigure", NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_LYXRC_APPLY
 * \li Action: Apply the given settings to user preferences.
 * \li Syntax: lyxrc-apply <SETTINGS>
 * \li Params: <SETTINGS>: settings which are to be set. Take a look into ~/.lyx/preferences
                           to get an idea which commands to use and their syntax.
                           #lyx::LyXRC::LyXRCTags has the list of possible commands.
 */
		{ LFUN_LYXRC_APPLY, "lyxrc-apply", NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_TOGGLE_CURSOR_FOLLOWS_SCROLLBAR
 * \li Action: Determine whether keep cursor inside the editing window regardless
               the scrollbar movement.
 * \li Syntax: toggle-cursor-follows-scrollbar
 * \li Origin: ARRae, 2 Dec 1997
 */
		{ LFUN_TOGGLE_CURSOR_FOLLOWS_SCROLLBAR, "toggle-cursor-follows-scrollbar", ReadOnly, System },
/**
 * \var lyx::kb_action lyx::LFUN_SET_COLOR
 * \li Action: Set the given LyX color to the color defined by the X11 name given.
 * \li Notion: A new color entry is created if the color is unknown.
               Color names can be stored as a part of user settings.
 * \li Syntax: set-color <LYX_NAME> <X11_NAME>
 * \li Origin: SLior, 11 Jun 2000
 */
		{ LFUN_SET_COLOR, "set-color", ReadOnly | NoBuffer, System },
/**
 * \var lyx::kb_action lyx::LFUN_STATISTICS
 * \li Action: Count the statistics (number of words and characters)
               in the document or in the given selection.
 * \li Notion: Note that this function gives the number of words/chars written,
               not the number of characters which will be typeset.
 * \li Syntax: statistics
 * \li Origin: lasgouttes, Jan 27 2004; ps, Jan 8 2008
 */
		{ LFUN_STATISTICS, "statistics", ReadOnly, System },

		{ LFUN_NOACTION, "", Noop, Hidden }
#ifndef DOXYGEN_SHOULD_SKIP_THIS
	};

	for (int i = 0; items[i].action != LFUN_NOACTION; ++i) {
		newFunc(items[i].action, items[i].name, items[i].attrib, items[i].type);
	}

	init = true;
}
#endif

LyXAction::LyXAction()
{
	init();
}


// Returns an action tag from a string.
FuncRequest LyXAction::lookupFunc(string const & func) const
{
	string const func2 = trim(func);

	if (func2.empty()) {
		return FuncRequest(LFUN_NOACTION);
	}

	string cmd;
	string const arg = split(func2, cmd, ' ');

	func_map::const_iterator fit = lyx_func_map.find(cmd);

	return fit != lyx_func_map.end() ? FuncRequest(fit->second, arg) : FuncRequest(LFUN_UNKNOWN_ACTION);
}


string const LyXAction::getActionName(kb_action action) const
{
	info_map::const_iterator const it = lyx_info_map.find(action);
	return it != lyx_info_map.end() ? it->second.name : string();
}


LyXAction::func_type const LyXAction::getActionType(kb_action action) const
{
	info_map::const_iterator const it = lyx_info_map.find(action);
	return it != lyx_info_map.end() ? it->second.type : Hidden;
}


bool LyXAction::funcHasFlag(kb_action action,
			    LyXAction::func_attrib flag) const
{
	info_map::const_iterator ici = lyx_info_map.find(action);

	if (ici == lyx_info_map.end()) {
		LYXERR0("action: " << action << " is not known.");
		BOOST_ASSERT(false);
	}

	return ici->second.attrib & flag;
}


LyXAction::const_func_iterator LyXAction::func_begin() const
{
	return lyx_func_map.begin();
}


LyXAction::const_func_iterator LyXAction::func_end() const
{
	return lyx_func_map.end();
}


} // namespace lyx
