// -*- C++ -*-
/**
 * \file trans_mgr.h
 * This file is part of LyX, the document processor.
 * Licence details can be found in the file COPYING.
 *
 * \author Lars Gullik Bj�nnes
 * \author Matthias Ettrich
 * \author John Levon
 *
 * Full author contact details are available in file CREDITS.
 */

#ifndef TRANS_MANAGER_H
#define TRANS_MANAGER_H

#include "tex-accent.h"
#include "trans_decl.h"
#include "chset.h"
#include "support/std_string.h"

class LyXText;
class Trans;

/// Translation state
class TransState {
public:
	///
	virtual ~TransState() {}
	///
	virtual string const normalkey(char) = 0;
	///
	virtual bool backspace() = 0;
	///
	virtual string const deadkey(char, KmodInfo) = 0;
	///
	static char const TOKEN_SEP;
};


/// Translation FSM
class TransFSMData {
protected:
	///
	virtual ~TransFSMData() {}
	///
	char deadkey_;
	///
	KmodInfo deadkey_info_;
	///
	char deadkey2_;
	///
	KmodInfo deadkey2_info_;
	///
	Keyexc comb_info_;
	///
	TransState * init_state_;
	///
	TransState * deadkey_state_;
	///
	TransState * combined_state_;
	///
public:
	///
	TransFSMData();
	///
	TransState * currentState;
};


/// Init State
class TransInitState :virtual public TransFSMData, public TransState {
public:
	///
	TransInitState();
	///
	virtual string const normalkey(char);
	///
	virtual bool backspace() { return true; }
	///
	virtual string const deadkey(char, KmodInfo);
};


/// Deadkey State
class TransDeadkeyState : virtual public TransFSMData, public TransState {
public:
	///
	TransDeadkeyState();
	///
	virtual string const normalkey(char);
	///
	virtual bool backspace() {
		currentState = init_state_;
		return false;
	}
	///
	virtual string const deadkey(char, KmodInfo);
};


/// Combined State
class TransCombinedState : virtual public TransFSMData, public TransState {
public:
	///
	TransCombinedState();
	///
	virtual string const normalkey(char);
	///
	virtual bool backspace() {
		// cancel the second deadkey
		deadkey2_ = 0;
		deadkey2_info_.accent = TEX_NOACCENT;
		currentState = deadkey_state_;

		return false;
	}
	///
	virtual string const deadkey(char, KmodInfo);
};


///
class TransFSM : virtual public TransFSMData,
		 public TransInitState,
		 public TransDeadkeyState,
		 public TransCombinedState {
public:
	///
	TransFSM();
};


///
class TransManager {
private:
	///
	TransFSM trans_fsm_;
	///
	Trans * active_;
	///
	Trans * t1_;
	///
	Trans * t2_;
	///
	static Trans default_;
	///
	CharacterSet chset_;
	///
	void insert(string const &, LyXText *);
	///
	void insertVerbatim(string const &, LyXText *);
public:
	///
	TransManager();
	///
	virtual ~TransManager();
	///
	int SetPrimary(string const &);
	///
	int SetSecondary(string const &);
	///
	void EnablePrimary();
	///
	void EnableSecondary();
	///
	void DisableKeymap();
	///
	bool setCharset(string const &);
	///
	bool backspace() {
		return trans_fsm_.currentState->backspace();
	}
	///
	void TranslateAndInsert(char, LyXText *);
	///
	string const deadkey(char, KmodInfo);
	///
	string const normalkey(char);
	///
	void deadkey(char, tex_accent, LyXText *);
};


inline
string const TransManager::normalkey(char c)
{
	return trans_fsm_.currentState->normalkey(c);
}


inline
string const TransManager::deadkey(char c, KmodInfo t)
{
	return trans_fsm_.currentState->deadkey(c, t);
}

#endif // TRANS_MANAGER_H
