// -*- C++ -*-
/**
 * \file CutAndPaste.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author J�rgen Vigna
 * \author Lars Gullik Bj�nnes
 * \author Alfredo Braunstein
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef CUTANDPASTE_H
#define CUTANDPASTE_H

#include "ParagraphList_fwd.h"

#include "support/docstring.h"

#include <vector>

namespace lyx {

class Buffer;
class ErrorList;
class InsetText;
class LyXTextClass;
class LCursor;

namespace cap {

/// Get all elements of the cut buffer in plain text format.
std::vector<docstring> const availableSelections(Buffer const & buffer);
/// Get the number of available elements in the cut buffer.
size_type numberOfSelections();
/// Get the sel_index-th element of the cut buffer in plain text format.
docstring getSelection(Buffer const & buffer, size_t sel_index);

/**
 * Replace using the font of the first selected character and select
 * the new string. When \c backwards == false, set anchor before
 * cursor; otherwise set cursor before anchor.
 * Does handle undo.
 */
void replaceSelectionWithString(LCursor & cur, docstring const & str,
				bool backwards);
/// If a selection exists, delete it without pushing it to the cut buffer.
/// Does handle undo.
void replaceSelection(LCursor & cur);

/**
 * Cut the current selection and possibly push it to the cut buffer and
 * system clipboard.
 * Does handle undo.
 * \param doclear If this is true: Delete leading spaces in paragraphs before
 *                they get merged.
 * \param realcut If this is true: Push the selection to the cut buffer and
 *                system clipboard. Set this to false to only delete the
 *                selection. 
 */
void cutSelection(LCursor & cur, bool doclear = true, bool realcut = true);
/// Push the current selection to the cut buffer and the system clipboard.
void copySelection(LCursor & cur);
/**
 * Push the current selection to the cut buffer and the system clipboard.
 * \param plaintext plain text version of the selection for the system
 *        clipboard
 */
void copySelection(LCursor & cur, docstring const & plaintext);
/// Push the selection buffer to the cut buffer.
void copySelectionToStack();
/// Store the current selection in the internal selection buffer
void saveSelection(LCursor & cur);
/// Is a selection available in our selection buffer?
bool selection();
/// Clear our selection buffer
void clearSelection();
/// Paste the current selection at \p cur
/// Does handle undo. Does only work in text, not mathed.
void pasteSelection(LCursor & cur, ErrorList &);
/// Replace the current selection with the clipboard contents (internal or
/// external: which is newer)
/// Does handle undo. Does only work in text, not mathed.
void pasteClipboard(LCursor & cur, ErrorList & errorList, bool asParagraphs = true);
/// Replace the current selection with cut buffer \c sel_index
/// Does handle undo. Does only work in text, not mathed.
void pasteFromStack(LCursor & cur, ErrorList & errorList, size_t sel_index);

/// Paste the paragraph list \p parlist at the position given by \p cur.
/// Does not handle undo. Does only work in text, not mathed.
void pasteParagraphList(LCursor & cur, ParagraphList const & parlist,
			textclass_type textclass, ErrorList & errorList);


/** Needed to switch between different classes. This works
 *  for a list of paragraphs beginning with the specified par.
 *  It changes layouts and character styles.
 */
void switchBetweenClasses(textclass_type c1, textclass_type c2,
                          InsetText & in, ErrorList &);

/// Get the current selection as a string. Does not change the selection.
/// Does only work if the whole selection is in mathed.
docstring grabSelection(LCursor const & cur);
/// Erase the current selection.
/// Does not handle undo. Does only work if the whole selection is in mathed.
void eraseSelection(LCursor & cur);
/// Erase the selection and return it as a string.
/// Does not handle undo. Does only work if the whole selection is in mathed.
docstring grabAndEraseSelection(LCursor & cur);
// other selection methods
/// Erase the selection if one exists.
/// Does not handle undo. Does only work if the whole selection is in mathed.
void selDel(LCursor & cur);
/// Clear or delete the selection if one exists, depending on lyxrc setting.
/// Does not handle undo. Does only work if the whole selection is in mathed.
void selClearOrDel(LCursor & cur);

/** Tabular has its own paste stack for multiple cells
 *  but it needs to know whether there is a more recent
 *  ordinary paste. Therefore which one is newer.
 */
//FIXME: this is a workaround for bug 1919. Replace this by
//an all-for-one-paste mechanism in 1.5
/// store whether tabular or ordinary paste stack is newer
void dirtyTabularStack(bool b);
/// is the tabular paste stack newer than the ordinary one?
bool tabularStackDirty();
} // namespace cap
} // namespce lyx

#endif
