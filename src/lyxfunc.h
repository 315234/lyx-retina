// -*- C++ -*-
/**
 * \file lyxfunc.h
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

#ifndef LYXFUNC_H
#define LYXFUNC_H

#include "kbsequence.h"
#include "lfuns.h"

#include "support/docstring.h"

#include <boost/scoped_ptr.hpp>
#include <boost/signals/trackable.hpp>

namespace lyx {

class BufferView;
class FuncRequest;
class FuncStatus;
class LyXKeySym;
class LyXText;
class LyXView;


/** This class encapsulates all the LyX command operations.
    This is the class of the LyX's "high level event handler".
    Every user command is processed here, either invocated from
    keyboard or from the GUI. All GUI objects, including buttons and
    menus should use this class and never call kernel functions directly.
*/
class LyXFunc : public boost::signals::trackable {
public:
	///
	explicit LyXFunc();

	/// LyX dispatcher, executes lyx actions.
	void dispatch(FuncRequest const &);

	///
	void setLyXView(LyXView * lv);

	///
	void initKeySequences(kb_keymap * kb);

	/// return the status bar state string
	docstring const viewStatusMessage();

	///
	void processKeySym(LyXKeySymPtr key, key_modifier::state state);

	///
	FuncStatus getStatus(FuncRequest const & action) const;

	/// The last key was meta
	bool wasMetaKey() const;

	/// True if lyxfunc reports an error
	bool errorStat() const { return errorstat; }
	/// Buffer to store result messages
	void setMessage(docstring const & m) const;
	/// Buffer to store result messages
	void setErrorMessage(docstring const &) const;
	/// Buffer to store result messages
	docstring const getMessage() const { return dispatch_buffer; }
	/// Handle a accented char key sequence
	void handleKeyFunc(kb_action action);
	/// goto a bookmark
	/// openFile: whether or not open a file if the file is not opened
	/// switchToBuffer: whether or not switch to buffer if the buffer is 
	///		not the current buffer
	void gotoBookmark(unsigned int idx, bool openFile, bool switchToBuffer);

private:
	///
	BufferView * view() const;

	///
	LyXView * lyx_view_;

	/// the last character added to the key sequence, in ISO encoded form
	char_type encoded_last_key;

	///
	boost::scoped_ptr<kb_sequence> keyseq;
	///
	boost::scoped_ptr<kb_sequence> cancel_meta_seq;
	///
	key_modifier::state meta_fake_bit;

	/// Error status, only Dispatch can change this flag
	mutable bool errorstat;

	/** Buffer to store messages and result data. Is there a
	    good reason to have this one as static in Dispatch? (Ale)
	*/
	mutable docstring dispatch_buffer;

	/// send a post-dispatch status message
	void sendDispatchMessage(docstring const & msg,
		FuncRequest const & ev);

	// I think the following should be moved to BufferView. (Asger)
	///
	void menuNew(std::string const & argument, bool fromTemplate);
	///
	void open(std::string const &);
	///
	void doImport(std::string const &);
	///
	void closeBuffer();
	///
	void reloadBuffer();
	///
	bool ensureBufferClean(BufferView * bv);
};

/// Implementation is in lyx_main.C
extern LyXFunc & theLyXFunc();

/// Implementation is in lyx_main.C
extern FuncStatus getStatus(FuncRequest const & action);

/// Implementation is in lyx_main.C
extern void dispatch(FuncRequest const & action);

} // namespace lyx

#endif
